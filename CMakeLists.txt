# Copyright 2021 DeepMind Technologies Limited
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.16)

# INTERPROCEDURAL_OPTIMIZATION is enforced when enabled.
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)
# Default to GLVND if available.
set(CMAKE_POLICY_DEFAULT_CMP0072 NEW)

# This line has to appear before 'PROJECT' in order to be able to disable incremental linking
set(MSVC_INCREMENTAL_DEFAULT ON)
include(FetchContent)

project(
  astra-mjc-sim
  VERSION 0.1.0
  DESCRIPTION "Mujoco simulation of ASTRA rocket lander"
  HOMEPAGE_URL "https://github.com/activecontrols/ASTRAFlightSoftware"
)

enable_language(C)
enable_language(CXX)
if(APPLE)
  enable_language(OBJC)
  enable_language(OBJCXX)
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" AND ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
    message(STATUS "Apple Silicon detected: Configuring build for arm64")
    set(ARM64 TRUE)
else()
    set(ARM64 FALSE)
endif()

# Check if we are building as standalone project.
set(SAMPLE_STANDALONE ON)
set(_INSTALL_SAMPLES ON)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(SAMPLE_STANDALONE ON)
  # If standalone, do not install the samples.
  set(_INSTALL_SAMPLES OFF)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

if(SAMPLE_STANDALONE)
  include(SampleOptions)
else()
  enforce_mujoco_macosx_min_version()
endif()
include(SampleDependencies)

set(MUJOCO_SAMPLE_COMPILE_OPTIONS "${AVX_COMPILE_OPTIONS}" "${EXTRA_COMPILE_OPTIONS}")
set(MUJOCO_SAMPLE_LINK_OPTIONS "${EXTRA_LINK_OPTIONS}")

if(MUJOCO_HARDEN)
  if(WIN32)
    set(MUJOCO_SAMPLE_LINK_OPTIONS "${MUJOCO_SAMPLE_LINK_OPTIONS}" -Wl,/DYNAMICBASE)
  else()
    set(MUJOCO_SAMPLE_COMPILE_OPTIONS "${MUJOCO_SAMPLE_COMPILE_OPTIONS}" -fPIE)
    if(APPLE)
      set(MUJOCO_SAMPLE_LINK_OPTIONS "${MUJOCO_SAMPLE_LINK_OPTIONS}" -Wl,-pie)
    else()
      set(MUJOCO_SAMPLE_LINK_OPTIONS "${MUJOCO_SAMPLE_LINK_OPTIONS}" -pie)
    endif()
  endif()
endif()

set(OSQP_EMBEDDED_MODE "1")

# Build samples that require GLFW.
# Tell the compiler this is a mujoco project
file(GLOB MPC_SOURCE "lib/controller/mpc/src/*.c")
file(GLOB SOLVER_SOURCE "lib/controller/mpc/solver_code/src/*.c" "lib/controller/mpc/solver_code/workspace.c")
add_compile_definitions(IS_MUJOCO)
add_executable(main-mujoco
    ${MPC_SOURCE}
    ${SOLVER_SOURCE}
    lib/flightmodule/Scheduler.cpp
    lib/flightmodule/Router.cpp
    lib/IMU/src/MujocoIMU.cpp
    lib/gps/MujocoGPS.cpp
    lib/motor/MujocoMotor.cpp
    lib/estimator/MEKF.cpp
    lib/estimator/MEKFEstimatorModule.cpp
    lib/math/MathFunctions.cpp
    # lib/comms/Comms.cpp
    lib/controller/ControllerModule.cpp
    src/main_mujoco.cpp
)

target_include_directories(main-mujoco PUBLIC include)
target_include_directories(main-mujoco PUBLIC lib/error)
target_include_directories(main-mujoco PUBLIC lib/math)
target_include_directories(main-mujoco PUBLIC lib/gps)
target_include_directories(main-mujoco PUBLIC lib/IMU/include)
target_include_directories(main-mujoco PUBLIC lib/IMU/src)
target_include_directories(main-mujoco PUBLIC lib/math)
target_include_directories(main-mujoco PUBLIC lib/estimator)
target_include_directories(main-mujoco PUBLIC lib/comms)
target_include_directories(main-mujoco PUBLIC lib/motor)
target_include_directories(main-mujoco PUBLIC lib/controller/mpc/include)
target_include_directories(main-mujoco PUBLIC lib/controller/mpc/solver_code/inc/public)
target_include_directories(main-mujoco PUBLIC lib/controller/mpc/solver_code/inc/private)
target_include_directories(main-mujoco PUBLIC lib/controller/mpc/solver_code/)
target_include_directories(main-mujoco PUBLIC lib/controller)
target_include_directories(main-mujoco PUBLIC lib/flightmodule)

target_compile_options(main-mujoco PUBLIC ${MUJOCO_SAMPLE_COMPILE_OPTIONS} -Wno-logical-op-parentheses -Wno-unused-function -Wno-strict-prototypes)
target_link_libraries(
  main-mujoco
  mujoco::mujoco
  glfw
  Threads::Threads
)
target_link_options(main-mujoco PRIVATE ${MUJOCO_SAMPLE_LINK_OPTIONS})

if(APPLE AND MUJOCO_BUILD_MACOS_FRAMEWORKS)
  embed_in_bundle(main-mujoco simulate)
endif()

# Do not install if macOS Bundles are created as RPATH is managed manually there.
if(APPLE AND MUJOCO_BUILD_MACOS_FRAMEWORKS)
  set(_INSTALL_SAMPLES OFF)
endif()

# if(_INSTALL_SAMPLES)
#
#   include(TargetAddRpath)
#
#   # Add support to RPATH for the samples.
#   target_add_rpath(
#     TARGETS
#     basic
#     compile
#     record
#     testspeed
#     INSTALL_DIRECTORY
#     "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
#     LIB_DIRS
#     "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
#     DEPENDS
#     MUJOCO_ENABLE_RPATH
#   )
#
#   install(
#     TARGETS basic
#             compile
#             record
#             testspeed
#     EXPORT ${PROJECT_NAME}
#     RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT samples
#     LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT samples
#     ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT samples
#     BUNDLE DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT samples
#     PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT samples
#   )
#
#   if(NOT MUJOCO_SAMPLES_USE_SYSTEM_GLFW)
#     # We downloaded GLFW. Depending if it is a static or shared LIBRARY we might
#     # need to install it.
#     get_target_property(MJ_GLFW_LIBRARY_TYPE glfw TYPE)
#     if(MJ_GLFW_LIBRARY_TYPE STREQUAL SHARED_LIBRARY)
#       install(
#         TARGETS glfw
#         EXPORT ${PROJECT_NAME}
#         RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT samples
#         LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT samples
#         ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT samples
#         PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT samples
#       )
#     endif()
#   endif()
# endif()
