##============================ 0. 工程配置部分 ============================================ 

cmake_minimum_required(VERSION 3.5.1) 

project(tryapi) 


if (CMAKE_BUILD_TYPE STREQUAL "Client") 
  set(carla_config client) 
elseif (CMAKE_BUILD_TYPE STREQUAL "Server") 
  set(carla_config server) 
endif () 

if (BUILD_RSS_VARIANT) 
  set(carla_target_postfix "_rss") 
else() 
  set(carla_target_postfix "") 
endif() 

#================================= 添加头文件 ============================ 




link_directories(
    "/home/typhon/repos/tryCarlac/libcarla-install/lib"
    ${RPCLIB_LIB_PATH}
    ${GTEST_LIB_PATH}
    ${RECAST_LIB_PATH}
    ${BOOST_LIB_PATH}
    ${CARLA_LIB_PATH})

##------------------------- include files of carla ----------------------------

# Add include.
file(GLOB libcarla_carla_include
    #"${libcarla_source_path}/carla/*.cpp"
    "${libcarla_source_path}/carla/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_include}")
install(FILES ${libcarla_carla_include} DESTINATION include/carla)

file(GLOB libcarla_carla_client_include
    #"${libcarla_source_path}/carla/client/*.cpp"
    "${libcarla_source_path}/carla/client/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_client_include}")
install(FILES ${libcarla_carla_client_include} DESTINATION include/carla/client)

file(GLOB libcarla_carla_client_detail_include
    #"${libcarla_source_path}/carla/client/detail/*.cpp"
    "${libcarla_source_path}/carla/client/detail/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_client_detail_include}")
install(FILES ${libcarla_carla_client_detail_include} DESTINATION include/carla/client/detail)

file(GLOB libcarla_carla_geom_include0
    #"${libcarla_source_path}/carla/geom/*.cpp"
    "${libcarla_source_path}/carla/geom/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_geom_include}")
install(FILES ${libcarla_carla_geom_include} DESTINATION include/carla/geom)

file(GLOB libcarla_carla_image_include
    #"${libcarla_source_path}/carla/image/*.cpp"
    "${libcarla_source_path}/carla/image/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_image_include}")
install(FILES ${libcarla_carla_image_include} DESTINATION include/carla/image)

file(GLOB libcarla_carla_nav_include
    #"${libcarla_source_path}/carla/nav/*.cpp"
    "${libcarla_source_path}/carla/nav/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_nav_include}")
install(FILES ${libcarla_carla_nav_include} DESTINATION include/carla/nav)

file(GLOB libcarla_carla_opendrive_include
    #"${libcarla_source_path}/carla/opendrive/*.cpp"
    "${libcarla_source_path}/carla/opendrive/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_opendrive_include}")
install(FILES ${libcarla_carla_opendrive_include} DESTINATION include/carla/opendrive)

file(GLOB libcarla_carla_opendrive_parser_include
    #"${libcarla_source_path}/carla/opendrive/parser/*.cpp"
    "${libcarla_source_path}/carla/opendrive/parser/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_opendrive_parser_include}")
install(FILES ${libcarla_carla_opendrive_parser_include} DESTINATION include/carla/opendrive/parser)

file(GLOB libcarla_carla_pointcloud_include
    #"${libcarla_source_path}/carla/pointcloud/*.cpp"
    "${libcarla_source_path}/carla/pointcloud/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_pointcloud_include}")
install(FILES ${libcarla_carla_pointcloud_include} DESTINATION include/carla/pointcloud)

file(GLOB libcarla_carla_profiler_headers
    "${libcarla_source_path}/carla/profiler/*.h")
install(FILES ${libcarla_carla_profiler_headers} DESTINATION include/carla/profiler)

file(GLOB libcarla_carla_road_include
    #"${libcarla_source_path}/carla/road/*.cpp"
    "${libcarla_source_path}/carla/road/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_road_include}")
install(FILES ${libcarla_carla_road_include} DESTINATION include/carla/road)

file(GLOB libcarla_carla_road_element_include
    #"${libcarla_source_path}/carla/road/element/*.cpp"
    "${libcarla_source_path}/carla/road/element/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_road_element_include}")
install(FILES ${libcarla_carla_road_element_include} DESTINATION include/carla/road/element)

file(GLOB libcarla_carla_road_general_include
    #"${libcarla_source_path}/carla/road/general/*.cpp"
    "${libcarla_source_path}/carla/road/general/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_road_general_include}")
install(FILES ${libcarla_carla_road_general_include} DESTINATION include/carla/road/general)

file(GLOB libcarla_carla_road_object_include
    #"${libcarla_source_path}/carla/road/object/*.cpp"
    "${libcarla_source_path}/carla/road/object/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_road_object_include}")
install(FILES ${libcarla_carla_road_object_include} DESTINATION include/carla/road/object)

file(GLOB libcarla_carla_road_signal_include
    #"${libcarla_source_path}/carla/road/signal/*.cpp"
    "${libcarla_source_path}/carla/road/signal/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_road_signal_include}")
install(FILES ${libcarla_carla_road_signal_include} DESTINATION include/carla/road/signal)

file(GLOB libcarla_carla_rpc_include
    #"${libcarla_source_path}/carla/rpc/*.cpp"
    "${libcarla_source_path}/carla/rpc/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_rpc_include}")
install(FILES ${libcarla_carla_rpc_include} DESTINATION include/carla/rpc)

if (BUILD_RSS_VARIANT)
  file(GLOB libcarla_carla_rss_include
      #"${libcarla_source_path}/carla/rss/*.cpp"
      "${libcarla_source_path}/carla/rss/*.h")
  set(libcarla_include "${libcarla_include};${libcarla_carla_rss_include}")
  install(FILES ${libcarla_carla_rss_include} DESTINATION include/carla/rss)
endif()

file(GLOB libcarla_carla_sensor_include
    #"${libcarla_source_path}/carla/sensor/*.cpp"
    "${libcarla_source_path}/carla/sensor/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_sensor_include}")
install(FILES ${libcarla_carla_sensor_include} DESTINATION include/carla/sensor)

file(GLOB libcarla_carla_sensor_data_include
    #"${libcarla_source_path}/carla/sensor/data/*.cpp"
    "${libcarla_source_path}/carla/sensor/data/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_sensor_data_include}")
install(FILES ${libcarla_carla_sensor_data_include} DESTINATION include/carla/sensor/data)

file(GLOB libcarla_carla_sensor_s11n_include
    #"${libcarla_source_path}/carla/sensor/s11n/*.cpp"
    "${libcarla_source_path}/carla/sensor/s11n/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_sensor_s11n_include}")
install(FILES ${libcarla_carla_sensor_s11n_include} DESTINATION include/carla/sensor/s11n)

file(GLOB libcarla_carla_streaming_include
    #"${libcarla_source_path}/carla/streaming/*.cpp"
    "${libcarla_source_path}/carla/streaming/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_streaming_include}")
install(FILES ${libcarla_carla_streaming_include} DESTINATION include/carla/streaming)

file(GLOB libcarla_carla_streaming_detail_include
    #"${libcarla_source_path}/carla/streaming/detail/*.cpp"
    "${libcarla_source_path}/carla/streaming/detail/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_streaming_detail_include}")
install(FILES ${libcarla_carla_streaming_detail_include} DESTINATION include/carla/streaming/detail)

file(GLOB libcarla_carla_streaming_detail_tcp_include
    #"${libcarla_source_path}/carla/streaming/detail/tcp/*.cpp"
    "${libcarla_source_path}/carla/streaming/detail/tcp/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_streaming_detail_tcp_include}")
install(FILES ${libcarla_carla_streaming_detail_tcp_include} DESTINATION include/carla/streaming/detail/tcp)

file(GLOB libcarla_carla_streaming_low_level_include
    #"${libcarla_source_path}/carla/streaming/low_level/*.cpp"
    "${libcarla_source_path}/carla/streaming/low_level/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_streaming_low_level_include}")
install(FILES ${libcarla_carla_streaming_low_level_include} DESTINATION include/carla/streaming/low_level)

file(GLOB libcarla_carla_multigpu_include
    #"${libcarla_source_path}/carla/multigpu/*.cpp"
    "${libcarla_source_path}/carla/multigpu/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_multigpu_include}")
install(FILES ${libcarla_carla_multigpu_include} DESTINATION include/carla/multigpu)

file(GLOB libcarla_odr_spiral_include
    #"${libcarla_source_thirdparty_path}/odrSpiral/*.cpp"
    "${libcarla_source_thirdparty_path}/odrSpiral/*.h")
set(libcarla_include "${libcarla_include};${libcarla_odr_spiral_include}")
install(FILES ${libcarla_odr_spiral_include} DESTINATION include/odrSpiral)

file(GLOB libcarla_moodycamel_include
    "${libcarla_source_thirdparty_path}/moodycamel/*.h")
set(libcarla_include "${libcarla_include};${libcarla_moodycamel_include}")
install(FILES ${libcarla_moodycamel_include} DESTINATION include/moodycamel)

file(GLOB libcarla_pugixml_include
    #"${libcarla_source_thirdparty_path}/pugixml/*.cpp"
    "${libcarla_source_thirdparty_path}/pugixml/*.hpp"
    "${libcarla_source_thirdparty_path}/pugixml/*.h")
set(libcarla_include "${libcarla_include};${libcarla_pugixml_include}")
install(FILES ${libcarla_pugixml_include} DESTINATION include/pugixml)

file(GLOB libcarla_carla_trafficmanager_include
    #"${libcarla_source_path}/carla/trafficmanager/*.cpp"
    "${libcarla_source_path}/carla/trafficmanager/*.h")
set(libcarla_include "${libcarla_include};${libcarla_carla_trafficmanager_include}")
install(FILES ${libcarla_carla_trafficmanager_include} DESTINATION include/carla/trafficmanager)

##------------------------------ end include files of carla ------------------------------
#---------------------------- Install Recast&Detour libraries
install(DIRECTORY "${RECAST_INCLUDE_PATH}/recast" DESTINATION include/system)
file(GLOB libcarla_carla_recastlib "${RECAST_LIB_PATH}/*.*")
install(FILES ${libcarla_carla_recastlib} DESTINATION lib)
# install(TARGETS "${CARLA_LIB_PATH}/libcarla_client_debug.a" DESTINATION lib)




#================================= 添加源文件==========================================
file(GLOB tryapi_sources
    #"${libcarla_source_path}/carla/profiler/*.cpp"
    #"${libcarla_source_path}/carla/profiler/*.h"
    "${libcarla_source_path}/tryapi/*.cpp"
    "${libcarla_source_path}/tryapi/*.h")

#================================= 增加子文件夹 ========================================


 
#================================== 生成可执行文件 ==================================== 

if (LIBCARLA_BUILD_DEBUG) 
  list(APPEND build_targets tryapi_test_${carla_config}_debug) 
endif() 

if (LIBCARLA_BUILD_RELEASE) 
  list(APPEND build_targets tryapi_test_${carla_config}_release) 
endif() 

 
# Create targets for debug and release in the same build type. 

foreach(target ${build_targets}) 

    add_executable(${target} ${tryapi_sources}) 
    
    target_compile_definitions(${target} PUBLIC
    -DLIBCARLA_ENABLE_PROFILER
    -DLIBCARLA_WITH_GTEST)

    #target_include_directories(${target} PUBLIC " "）
    target_include_directories(${target} SYSTEM PUBLIC
                                "${BOOST_INCLUDE_PATH}"
                                "${RPCLIB_INCLUDE_PATH}"
                                "${GTEST_INCLUDE_PATH}"
                                "${LIBPNG_INCLUDE_PATH}"
                                "${RECAST_INCLUDE_PATH}"
                                )
    target_include_directories(${target} PUBLIC
                                "${libcarla_source_path}/tryapi"
                                "${CARLA_INCLUDE_PATH}"
                                "/home/typhon/repos/tryCarlac/libcarla-install/include/system"
                                #"${libcarla_source_path}/carla"
                                #"${libcarla_source_path}/carla/client"
                                #"${libcarla_source_path}/carla/client/detail"
                                #"${libcarla_source_path}/carla/geom"
                                #"${libcarla_source_path}/carla/image"
                                #"${libcarla_source_path}/carla/nav"
                                #"${libcarla_source_path}/carla/opendrive"
                                #"${libcarla_source_path}/carla/opendrive/parser"
                                #"${libcarla_source_path}/carla/pointcloud"
                                #"${libcarla_source_path}/carla/profiler"
                                #"${libcarla_source_path}/carla/road"
                                #"${libcarla_source_path}/carla/road/element"
                                #"${libcarla_source_path}/carla/road/general"
                                #"${libcarla_source_path}/carla/road/object"
                                #"${libcarla_source_path}/carla/road/signal"
                                #"${libcarla_source_path}/carla/rpc"
                                #"${libcarla_source_path}/carla/rss"
                                #"${libcarla_source_path}/carla/sensor"
                                #"${libcarla_source_path}/carla/sensor/data"
                                #"${libcarla_source_path}/carla/sensor/s11n"
                                #"${libcarla_source_path}/carla/streaming"
                                #"${libcarla_source_path}/carla/streaming/detail"
                                #"${libcarla_source_path}/carla/streaming/detail/tcp"
                                #"${libcarla_source_path}/carla/streaming/low_level"
                                #"${libcarla_source_path}/carla/multigpu"
                                #"${libcarla_source_thirdparty_path}/odrSpiral"
                                #"${libcarla_source_thirdparty_path}/moodycamel"
                                #"${libcarla_source_thirdparty_path}/pugixml"
                                #"${libcarla_source_path}/carla/trafficmanager"
                                )
    #                            -lpng -ltiff -ljpeg -lRecast -lDetour -lDetourCrowd
    target_link_libraries(${target} "-lcarla_client")
    target_link_libraries(${target} "-lrpc")
    target_link_libraries(${target} "-lgtest_main")
    target_link_libraries(${target} "-lgtest")
    target_link_libraries(${target} "-lpng")
    target_link_libraries(${target} "-ltiff")
    target_link_libraries(${target} "-ljpeg")
    target_link_libraries(${target} "-lpng")
    target_link_libraries(${target} "-lRecast")
    target_link_libraries(${target} "-lDetour")
    target_link_libraries(${target} "-lDetourCrowd")


    #  加l是动态链接库



endforeach(target)

if (LIBCARLA_BUILD_DEBUG)
  # Specific options for debug.
  set_target_properties(tryapi_test_${carla_config}_debug PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
  #target_link_libraries(tryapi_test_${carla_config}_debug "carla_${carla_config}${carla_target_postfix}_debug")#到底家不加debug
  target_compile_definitions(tryapi_test_${carla_config}_debug PUBLIC -DBOOST_ASIO_ENABLE_BUFFER_DEBUGGING)
  if (CMAKE_BUILD_TYPE STREQUAL "Client")
      target_link_libraries(tryapi_test_${carla_config}_debug "${BOOST_LIB_PATH}/libboost_filesystem.a")
  endif()
endif()

if (LIBCARLA_BUILD_RELEASE)
  # Specific options for release.
  set_target_properties(tryapi_test_${carla_config}_release PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
  #target_link_libraries(tryapi_test_${carla_config}_release "carla_${carla_config}${carla_target_postfix}_debug")
  if (CMAKE_BUILD_TYPE STREQUAL "Client")
      target_link_libraries(tryapi_test_${carla_config}_release "${BOOST_LIB_PATH}/libboost_filesystem.a")
  endif()
endif()