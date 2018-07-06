cmake_minimum_required(VERSION 2.8)
project( ThetaCloudTests )
set (CMAKE_CXX_STANDARD 11)
set(src ThetaCloud.cpp)
include_directories(${CMAKE_CURRENT_LIST_DIR})

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/single_include CACHE INTERNAL "Path to include folder for Catch")
include_directories( ${CATCH_INCLUDE_DIR} )
include_directories( test/fakeIncludes )
set ( fakeCpps test/fakeIncludes/StaticStuff.cpp )
set (theta_cloud_base_tests test/main.cpp test/theta_cloud_base_tests.cpp)
add_executable(thetaCloudBaseTests ${theta_cloud_base_tests} ${src} ${fakeCpps})
target_link_libraries( thetaCloudBaseTests ${OpenCV_LIBS} )
add_dependencies(thetaCloudBaseTests catch)
add_custom_command(TARGET thetaCloudBaseTests POST_BUILD
	COMMAND thetaCloudBaseTests )
