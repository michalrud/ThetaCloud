cmake_minimum_required(VERSION 2.8)
project( ThetaCloudTests )
set (CMAKE_CXX_STANDARD 11)
set(thetaCloudBaseSrc ThetaCloud.cpp)
set(thetaCloudBaseMockSrc test/fakeIncludes/MockThetaCloud.cpp)
include_directories(${CMAKE_CURRENT_LIST_DIR})

enable_testing()
find_package(Threads REQUIRED)
include(ExternalProject)
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)
ExternalProject_Get_Property(gtest source_dir binary_dir)

add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

set(ANNOYING_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wlogical-op -Werror")
set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS "-g -O0 ${ANNOYING_FLAGS} ${COVERAGE_FLAGS}")
set(CMAKE_C_FLAGS "-g -O0 ${ANNOYING_FLAGS} -W ${COVERAGE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${COVERAGE_FLAGS}")

include_directories( test/fakeIncludes )
set ( fakeCpps test/fakeIncludes/StaticStuff.cpp test/fakeIncludes/Arduino.cpp test/fakeIncludes/FakeUtils.cpp )

set (theta_cloud_base_tests test/main.cpp test/theta_cloud_base_tests.cpp)
add_executable(thetaCloudBaseTests ${theta_cloud_base_tests} ${thetaCloudBaseSrc} ${fakeCpps})
target_link_libraries( thetaCloudBaseTests libgtest libgmock gcov )
add_test(NAME thetaCloudBase COMMAND valgrind ./thetaCloudBaseTests )

set (i2c_helpers_tests test/main.cpp test/i2c_helpers_tests.cpp)
add_executable(i2cHelpersTests ${i2c_helpers_tests} ${thetaCloudBaseSrc} ${fakeCpps})
target_link_libraries( i2cHelpersTests libgtest libgmock gcov )
add_test(NAME i2cHelpers COMMAND valgrind ./i2cHelpersTests )

set (light_tests test/main.cpp test/light_tests.cpp)
add_executable(lightTests ${light_tests} ThetaCloudLight.cpp ${fakeCpps} ${thetaCloudBaseMockSrc})
target_link_libraries( lightTests libgtest libgmock gcov )
add_test(NAME light COMMAND valgrind ./lightTests )

set (humidity_tests test/main.cpp test/humidity_tests.cpp)
add_executable(humidityTests ${humidity_tests} ThetaCloudHumidity.cpp ${fakeCpps} ${thetaCloudBaseMockSrc})
target_link_libraries( humidityTests libgtest libgmock gcov )
add_test(NAME humidity COMMAND valgrind ./humidityTests )

set (environment_tests test/main.cpp test/environment_tests.cpp)
add_executable(envTests ${environment_tests} ThetaCloudEnvironment.cpp ${fakeCpps} ${thetaCloudBaseMockSrc})
target_link_libraries( envTests libgtest libgmock gcov )
add_test(NAME env COMMAND valgrind ./envTests )

set (pir_tests test/main.cpp test/pir_tests.cpp)
add_executable(pirTests ${pir_tests} ThetaCloudPIR.cpp ${fakeCpps} ${thetaCloudBaseMockSrc})
target_link_libraries( pirTests libgtest libgmock gcov )
add_test(NAME pir COMMAND valgrind ./pirTests )

set (relay_tests test/main.cpp test/relay_tests.cpp)
add_executable(relayTests ${relay_tests} ThetaCloudRelay.cpp ${fakeCpps} ${thetaCloudBaseMockSrc})
target_link_libraries( relayTests libgtest libgmock gcov )
add_test(NAME relay COMMAND valgrind ./relayTests )
