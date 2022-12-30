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
project(de.mrh.evtest VERSION 1.1.0
                      DESCRIPTION "MRH event test"
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
#  App Source Paths
#  ----------------
#  The paths to the app source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH_APP "${CMAKE_SOURCE_DIR}/src/App/")
                 
set(SRC_LIST_APP_MODULE_LISTEN "${SRC_DIR_PATH_APP}/Module/Listen/L_Avail_V1.h"
                               "${SRC_DIR_PATH_APP}/Module/Listen/L_GetMethod_V1.h"
                               "${SRC_DIR_PATH_APP}/Module/Listen/L_Custom_V1.h")
                 
set(SRC_LIST_APP_MODULE_SAY "${SRC_DIR_PATH_APP}/Module/Say/S_Avail_V1.h"
                            "${SRC_DIR_PATH_APP}/Module/Say/S_String_V1.h"
                            "${SRC_DIR_PATH_APP}/Module/Say/S_GetMethod_V1.h"
                            "${SRC_DIR_PATH_APP}/Module/Say/S_NotificationApp_V1.h"
                            "${SRC_DIR_PATH_APP}/Module/Say/S_Custom_V1.h")
                 
set(SRC_LIST_APP_MODULE_PASSWORD "${SRC_DIR_PATH_APP}/Module/Password/P_Avail_V1.h"
                                 "${SRC_DIR_PATH_APP}/Module/Password/P_Check_V1.h"
                                 "${SRC_DIR_PATH_APP}/Module/Password/P_Set_V1.h"
                                 "${SRC_DIR_PATH_APP}/Module/Password/P_Custom_V1.h")
                 
set(SRC_LIST_APP_MODULE_USER "${SRC_DIR_PATH_APP}/Module/User/U_Avail_V1.h"
                             "${SRC_DIR_PATH_APP}/Module/User/U_AccessContent_V1.h"
                             "${SRC_DIR_PATH_APP}/Module/User/U_AccessClear_V1.h"
                             "${SRC_DIR_PATH_APP}/Module/User/U_GetLocation_V1.h"
                             "${SRC_DIR_PATH_APP}/Module/User/U_Custom_V1.h")
                 
set(SRC_LIST_APP_MODULE_APPLICATION "${SRC_DIR_PATH_APP}/Module/Application/A_Avail_V1.h"
                                    "${SRC_DIR_PATH_APP}/Module/Application/A_LaunchSOA_V1.h"
                                    "${SRC_DIR_PATH_APP}/Module/Application/A_LaunchSOAClear_V1.h"
                                    "${SRC_DIR_PATH_APP}/Module/Application/A_LaunchSOATimer_V1.h"
                                    "${SRC_DIR_PATH_APP}/Module/Application/A_LaunchSOAClearTimer_V1.h"
                                    "${SRC_DIR_PATH_APP}/Module/Application/A_Custom_V1.h")

set(SRC_LIST_APP "${SRC_DIR_PATH_APP}/Module/EventModule.h"
                 "${SRC_DIR_PATH_APP}/EventSender.h"
                 "${SRC_DIR_PATH_APP}/Revision.h"
                 "${SRC_DIR_PATH_APP}/Main.cpp")

###
#  Service Source Paths
#  --------------------
#  The paths to the service source files to use.
#  Add OS specific source files in their own list.
###
set(SRC_DIR_PATH_SERVICE "${CMAKE_SOURCE_DIR}/src/Service")

set(SRC_LIST_SERVICE "${SRC_DIR_PATH_SERVICE}/Main.cpp")

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
                           ${SRC_LIST_APP_MODULE_LISTEN}
                           ${SRC_LIST_APP_MODULE_SAY}
                           ${SRC_LIST_APP_MODULE_PASSWORD}
                           ${SRC_LIST_APP_MODULE_USER}
                           ${SRC_LIST_APP_MODULE_APPLICATION})
set_target_properties(MRH_App
                      PROPERTIES
                      PREFIX ""
                      OUTPUT_NAME "App"
                      SUFFIX ".so"
                      ARCHIVE_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
                      LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR_PATH}
                      RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR_PATH})
					  
add_library(MRH_Service SHARED ${SRC_LIST_SERVICE})
set_target_properties(MRH_Service
                      PROPERTIES
                      PREFIX ""
                      OUTPUT_NAME "Service"
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
find_library(libmrhvt NAMES mrhvt REQUIRED)
find_library(libmrhs NAMES mrhs REQUIRED)

target_link_libraries(MRH_App PUBLIC Threads::Threads)
target_link_libraries(MRH_App PUBLIC mrh)
target_link_libraries(MRH_App PUBLIC mrhbf)
target_link_libraries(MRH_App PUBLIC mrhevdata)
target_link_libraries(MRH_App PUBLIC mrhab)
target_link_libraries(MRH_App PUBLIC mrhvt)

target_link_libraries(MRH_Service PUBLIC mrhs)
target_link_libraries(MRH_Service PUBLIC mrhevdata)

###
#  Source Definitions
#  ------------------
#  Preprocessor source definitions.
###