#########################################################################
#
#  CMAKE
#
#########################################################################

###
#  Minimum Version
#  ---------------
#  The CMake version required.
###
cmake_minimum_required(VERSION 3.1)

###
#  CMake Configuration
#  -------------------
#  Configuration settings for CMake.
#
#  NOTE:
#  These settings have to be applied before the project() setting!
###
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 14)

###
#  Project Info
#  ------------
#  General simple information about our project.
###
project(de.mrh.evtest VERSION 1.0.0
                      DESCRIPTION "MRH event test application"
                      LANGUAGES CXX)

#########################################################################
#
#  PATHS
#
#########################################################################

###
#  Binary Paths
#  ------------
#  The paths for our created binary file(s).
###
set(BIN_DIR_PATH "${CMAKE_SOURCE_DIR}/bin/")
file(MAKE_DIRECTORY ${BIN_DIR_PATH})

###
#  Build Paths
#  -----------
#  The paths for the cmake build.
###
set(BUILD_DIR_PATH "${CMAKE_SOURCE_DIR}/build/")
file(MAKE_DIRECTORY ${BUILD_DIR_PATH})

###
#  Source Paths
#  ------------
#  The paths to the source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH "${CMAKE_SOURCE_DIR}/src/")
             
set(SRC_LIST_APP "${SRC_DIR_PATH}/Revision.h"
                 "${SRC_DIR_PATH}/Main.cpp")
                 
set(SRC_LIST_MODULE_BASE "${SRC_DIR_PATH}/Module/EventModule.h")
                 
set(SRC_LIST_MODULE_RESPONSE_TEST "${SRC_DIR_PATH}/Module/ResponseTest/ResponseTest.cpp"
                                  "${SRC_DIR_PATH}/Module/ResponseTest/ResponseTest.h")
                 
set(SRC_LIST_MODULE_RESPONSE_TEST_V1 "${SRC_DIR_PATH}/Module/ResponseTest/V1/V1Response.cpp"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/V1Response.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/ServiceAvailV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/ServiceCustomCommandV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Custom/CustomV1Response.cpp"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Custom/CustomV1Response.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Listen/ListenV1Response.cpp"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Listen/ListenV1Response.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Listen/ListenGetMethodV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Say/SayV1Response.cpp"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Say/SayV1Response.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Say/SayStringV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Say/SayGetMethodV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Say/SayNotificationV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Password/PasswordV1Response.cpp"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Password/PasswordV1Response.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Password/PasswordCheckV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Password/PasswordSetV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/User/UserV1Response.cpp"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/User/UserV1Response.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/User/UserAccessV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/User/UserGetLocationV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Application/ApplicationV1Response.cpp"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Application/ApplicationV1Response.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Application/ApplicationLaunchV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Application/ApplicationLaunchTimerV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Application/ApplicationClearLaunchV1.h"
                                     "${SRC_DIR_PATH}/Module/ResponseTest/V1/Application/ApplicationClearLaunchTimerV1.h")

#########################################################################
#
#  TARGET
#
#########################################################################

###
#  Target
#  ------
#  The target(s) to build.
#
#  NOTE: 
#  MRH applications are required to be named App.so, 
#  and application services Service.so!
#  They are build as shared objects.
###
add_library(MRH_App SHARED ${SRC_LIST_APP}
                           ${SRC_LIST_MODULE_BASE}
                           ${SRC_LIST_MODULE_RESPONSE_TEST}
                           ${SRC_LIST_MODULE_RESPONSE_TEST_V1})
set_target_properties(MRH_App
                      PROPERTIES
                      PREFIX ""
                      OUTPUT_NAME "App"
                      SUFFIX ".so"
                      ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
                      RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})

###
#  Required Libraries
#  ------------------
#  Libraries required by this application.
###
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

find_package(Threads REQUIRED)
find_library(libmrh NAMES mrh REQUIRED)
find_library(libmrhbf NAMES mrhbf REQUIRED)
find_library(libmrhevdata NAMES mrhevdata REQUIRED)
find_library(libmrhab NAMES mrhab REQUIRED)

target_link_libraries(MRH_App PUBLIC Threads::Threads)
target_link_libraries(MRH_App PUBLIC mrh)
target_link_libraries(MRH_App PUBLIC mrhbf)
target_link_libraries(MRH_App PUBLIC mrhevdata)
target_link_libraries(MRH_App PUBLIC mrhab)

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###