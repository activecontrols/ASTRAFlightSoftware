cmake_minimum_required(VERSION 3.15)
project(osqp_ext_emosqp)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # -fPIC
set(OSQP_EMBEDDED_MODE "1")

file(
  GLOB
  OSQP_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)


file(
  GLOB
  SOLVER_HEAD
  ${CMAKE_CURRENT_SOURCE_DIR}/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/public/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/inc/private/*.h)

set(solver_head ${SOLVER_HEAD} PARENT_SCOPE)

file(
  GLOB
  SOLVER_SRC
  ${OSQP_SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/workspace.c)
set(solver_src ${SOLVER_SRC} PARENT_SCOPE)
