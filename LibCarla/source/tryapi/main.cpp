#include "boost/exception/exception.hpp"
#include "carla/client/Actor.h"
#include "carla/client/Vehicle.h"
#include "carla/geom/Location.h"
#include "carla/geom/Rotation.h"
#include "carla/rpc/AttachmentType.h"
#include "carla/rpc/Location.h"
#include "carla/rpc/Transform.h"
#include <iostream>
#include <iterator>
#include <memory>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <string>
#include <fstream>
#include <streambuf>

#include <carla/client/ActorBlueprint.h>
#include <carla/client/BlueprintLibrary.h>
#include <carla/client/Client.h>
#include <carla/client/Map.h>
#include <carla/client/Sensor.h>
#include <carla/client/TimeoutException.h>
#include <carla/client/World.h>
#include <carla/geom/Transform.h>
#include <carla/image/ImageIO.h>
#include <carla/image/ImageView.h>
#include <carla/sensor/data/Image.h>
#include <carla/trafficmanager/TrafficManager.h>
#include <vector>

namespace cc = carla::client;
namespace cg = carla::geom;
namespace csd = carla::sensor::data;
namespace cr = carla::rpc;
namespace ctm = carla::traffic_manager;

using namespace std::chrono_literals;
using namespace std::string_literals;

#define EXPECT_TRUE(pred) if (!(pred)) { throw std::runtime_error(#pred); }

/// Pick a random element from @a range.
template <typename RangeT, typename RNG>
static auto &RandomChoice(const RangeT &range, RNG &&generator) {
  EXPECT_TRUE(range.size() > 0u);
  std::uniform_int_distribution<size_t> dist{0u, range.size() - 1u};
  return range[dist(std::forward<RNG>(generator))];
}

/// Save a semantic segmentation image to disk converting to CityScapes palette.
/**/
static void SaveSemSegImageToDisk(const csd::Image &image) {
  using namespace carla::image;

  char buffer[9u];
  std::snprintf(buffer, sizeof(buffer), "%08zu", image.GetFrame());
  auto filename = "_images/"s + buffer + ".png";

  auto view = ImageView::MakeColorConvertedView(
      ImageView::MakeView(image),
      ColorConverter::CityScapesPalette());
  ImageIO::WriteView(filename, view);
}


static auto ParseArguments(int argc, const char *argv[]) {
  EXPECT_TRUE((argc == 1u) || (argc == 3u));
  using ResultType = std::tuple<std::string, uint16_t>;
  return argc == 3u ?
      ResultType{argv[1u], std::stoi(argv[2u])} :
      ResultType{"localhost", 2000u};
}


void updateSpectator(boost::shared_ptr<carla::client::Vehicle> vehicle,std::shared_ptr<carla::client::Client> client,std::shared_ptr<std::vector<boost::shared_ptr<carla::client::Actor>>> spectator_obj_list){
  auto world=client->GetWorld();
  world.GetSpectator();
  //获得车辆的中心点，并将点定位到右上角，便于运算
  double bound_x = 0.5 + vehicle->GetBoundingBox().extent.x;
  double bound_y = 0.5 + vehicle->GetBoundingBox().extent.y;
  double bound_z = 0.5 + vehicle->GetBoundingBox().extent.z;
  //查找相机蓝图
  auto camera_bp = world.GetBlueprintLibrary()->Find("sensor.camera.rgb");

  //设置Camera的附加类型Camera跟随车辆(幽灵模式)；
  auto Atment_SpringArmGhost = carla::rpc::AttachmentType::SpringArmGhost;
  auto Atment_Rigid = carla::rpc::AttachmentType::Rigid;

  //设置相对车辆的安装位置，配置上帝视图(Camera无法实现上帝视图，画面会抖动)
  std::vector<cg::Transform> Vehicle_transform_list=std::vector<cg::Transform>();
  Vehicle_transform_list.push_back(
    cg::Transform(
      cg::Location(0.0f, 0.0f, 50.0f),
      cg::Rotation(-90,0,0)));
  //设置camera的安装位置，配置后往前视图以及前后左右视图
  std::vector<cg::Transform> Camera_transform_list=std::vector<cg::Transform>();
  Camera_transform_list.push_back(cg::Transform(cg::Location(0.0f, 0.0f, 50.0f),
      cg::Rotation(-90,0,0)));
  auto vehicle_actor=boost::static_pointer_cast<cc::Actor>(vehicle);
  //  auto vehicle = boost::static_pointer_cast<cc::Vehicle>(actor);

  //    #上帝视图坐标系以及所有camera对象填入spectator_obj_list；
  auto spectator_obj=cg::Transform();
  for(auto iter=Vehicle_transform_list.begin();iter!=Vehicle_transform_list.end();iter++){
    if(iter-Vehicle_transform_list.begin()==0){//第一个是上帝视图坐标系
      //spectator_obj_list.push_back(*iter);
      spectator_obj=*iter;
    }
    else{
      auto camera=world.SpawnActor(*camera_bp,*iter,vehicle_actor.get(),Atment_SpringArmGhost);
      spectator_obj_list->push_back(camera);
    }

  }
  for(auto iter=Camera_transform_list.begin();iter!=Camera_transform_list.end();iter++){
    if(iter-Camera_transform_list.begin()==0){//第一个是上帝视图坐标系
      //spectator_obj_list.push_back(*iter);
      //spectator_obj=*iter;
    }
    else{
      auto camera=world.SpawnActor(*camera_bp,*iter,vehicle_actor.get(),Atment_SpringArmGhost);
      spectator_obj_list->push_back(camera);
    }
  }
  while(1){
    for(auto iter=spectator_obj_list->begin();iter!=spectator_obj_list->end();iter++){
      if(iter-spectator_obj_list->begin()!=0){//第一个是上帝视图坐标系
        auto vehicle_transform=carla::rpc::Transform(iter->get()->GetTransform());
        world.GetSpectator()->SetTransform(vehicle_transform);
      }
    }
  }

}



void autopilot(std::shared_ptr<carla::client::Vehicle> vehicle){
  vehicle->SetAutopilot();
}



int main(int argc, const char *argv[]) {
  try {
    // =================== 连接到服务器=========================================
    std::string host;
    uint16_t port;
    std::tie(host, port) = ParseArguments(argc, argv);
    std::mt19937_64 rng((std::random_device())());
    auto client = cc::Client(host, port);
    client.SetTimeout(40s);

    std::cout << "Client API version : " << client.GetClientVersion() << '\n';
    std::cout << "Server API version : " << client.GetServerVersion() << '\n';
    
            /*
            // Load a random town.
            auto town_name = RandomChoice(client.GetAvailableMaps(), rng);
            std::cout << "Loading world: " << town_name << std::endl;
            auto world = client.LoadWorld(town_name);
            */
    // =================== 设置地图,获取世界world=========================================
    // 读取文件
    std::ifstream odrStream("TownBig.xodr");
    std::string odrData((std::istreambuf_iterator<char>(odrStream)),
                    std::istreambuf_iterator<char>());
    // 设定参数
    double vertex_distance = 2.0;//  # in meters
    double max_road_length = 500.0;// # in meters
    double wall_height = 1.0      ;//# in meters
    double extra_width = 0.6      ;//# in meters
            // bool smooth_junc;
            // bool e_visibility;
            // bool e_pedestrian;
    auto odrParameters=cr::OpendriveGenerationParameters(vertex_distance,max_road_length,wall_height,extra_width,true,true,false);
    auto world = client.GenerateOpenDriveWorld(odrData,odrParameters);
            //auto world=client.LoadWorld(opfile);

    //==================== 获取车辆原型===================================================
    // Get a random vehicle blueprint.
    auto blueprint_library = world.GetBlueprintLibrary();
    auto vehicles = blueprint_library->Filter("vehicle.audi.a2");
    auto blueprint = RandomChoice(*vehicles, rng);
            //auto blueprint=world.GetBlueprintLibrary()->Filter("vehicle.audi.a2");

    // Randomize the blueprint.
    if (blueprint.ContainsAttribute("color")) {
      auto &attribute = blueprint.GetAttribute("color");
      blueprint.SetAttribute(
          "color",
          RandomChoice(attribute.GetRecommendedValues(), rng));
    }
    //==================== 在随机位置生成车辆==================================================
    // Find a valid spawn point.
    auto map = world.GetMap();
    auto transform = RandomChoice(map->GetRecommendedSpawnPoints(), rng);

    // Spawn the vehicle.
    auto actor = world.SpawnActor(blueprint, transform);
    std::cout << "Spawned " << actor->GetDisplayId() << '\n';
    auto vehicle = boost::static_pointer_cast<cc::Vehicle>(actor);

    std::shared_ptr<std::vector<boost::shared_ptr<carla::client::Actor>>> spectator_obj_list=std::make_shared<std::vector<boost::shared_ptr<carla::client::Actor>>>();

    

  /*
    // 设置traffic manager
    auto traffic_manager=client.GetInstanceTM();
    traffic_manager.SetGlobalDistanceToLeadingVehicle(3.0);
    traffic_manager.SetHybridPhysicsMode(true);
    traffic_manager.SetGlobalPercentageSpeedDifference(80);
    
    bool argsSync=true;
    bool synchronous_master=false;
    // Suggest using syncmode
    //true
    auto settings=world.GetSettings();
    traffic_manager.SetSynchronousMode(true);
    if(settings.synchronous_mode==false){
        synchronous_master=true;
        settings.synchronous_mode=true;
        settings.fixed_delta_seconds=boost::optional<double>(0.05);
        world.ApplySettings(settings,carla::time_duration::seconds(1));
    }

    // Apply control to vehicle.
    cc::Vehicle::Control control;
    control.throttle = 1.0f;
            //vehicle->ApplyControl(control);
    vehicle->SetAutopilot();
    
    // Move spectator so we can see the vehicle from the simulator window.
    auto spectator = world.GetSpectator();
            // transform.location += 32.0f * transform.GetForwardVector();
            // transform.location.z += 2.0f;
            // transform.rotation.yaw += 180.0f;
            // transform.rotation.pitch = -15.0f;
    transform.location.z+=50.f;
    transform.rotation.pitch = -90.0f;
            // spectator->SetTransform(transform);
    spectator->SetTransform(transform);
    auto camera_bp=blueprint_library->Find("sensor.camera.rgb");
    EXPECT_TRUE(camera_bp != nullptr);
    auto camera_transform = cg::Transform{
        cg::Location{-8.0f, 5.0f, 0.0f},   // x, y, z.
        cg::Rotation{10.0f, 0.0f, 0.0f}}; // pitch, yaw, roll.
    auto cam_actor = world.SpawnActor(*camera_bp, camera_transform, actor.get());
    auto camera = boost::static_pointer_cast<cc::Sensor>(cam_actor);

    // Register a callback to save images to disk.
    camera->Listen([](auto data) {
        auto image = boost::static_pointer_cast<csd::Image>(data);
        EXPECT_TRUE(image != nullptr);
                    //SaveSemSegImageToDisk(*image);
    });
    std::this_thread::sleep_for(100s);
    while(true){
      if(argsSync&&synchronous_master){
        world.Tick();
      }
      

    }
    */

  /*
      // Find a camera blueprint.
      auto camera_bp = blueprint_library->Find("sensor.camera.semantic_segmentation");
      EXPECT_TRUE(camera_bp != nullptr);

      // Spawn a camera attached to the vehicle.
      auto camera_transform = cg::Transform{
          cg::Location{-5.5f, 0.0f, 2.8f},   // x, y, z.
          cg::Rotation{-15.0f, 0.0f, 0.0f}}; // pitch, yaw, roll.
      auto cam_actor = world.SpawnActor(*camera_bp, camera_transform, actor.get());
      auto camera = boost::static_pointer_cast<cc::Sensor>(cam_actor);

      // Register a callback to save images to disk.
      camera->Listen([](auto data) {
          auto image = boost::static_pointer_cast<csd::Image>(data);
          EXPECT_TRUE(image != nullptr);
          SaveSemSegImageToDisk(*image);
      });

      std::this_thread::sleep_for(10s);

      // Remove actors from the simulation.
      camera->Destroy();
  */
    vehicle->Destroy();
    std::cout << "Actors destroyed." << std::endl;
    //camera->Destroy();
    for(auto iter=spectator_obj_list->begin();iter!=spectator_obj_list->end();iter++){
      iter->get()->Destroy();
    }

  } catch (const cc::TimeoutException &e) {
    std::cout << '\n' << e.what() << std::endl;
    return 1;
  } catch (const std::exception &e) {
    std::cout << "\nException: " << e.what() << std::endl;
    return 2;
  }
}
