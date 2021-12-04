cmake_minimum_required(VERSION 3.14...3.18)

project(
  isomorph
  VERSION 0.1
  DESCRIPTION "Graph isomorphism tester based on lexicographically minimum spanning tree recoloring"
  LANGUAGES CXX)

set(PROJECT_SOURCES
        src/isomorph.cpp
        src/checker.cpp
        src/utility.cpp
        src/view.cpp
        src/lex_mst_node.cpp)

add_library(isomorph ${PROJECT_SOURCES})

target_include_directories(
  isomorph
  PRIVATE $<BUILD_INTERFACE:${isomorph_SOURCE_DIR}/src>
  PUBLIC $<BUILD_INTERFACE:${isomorph_SOURCE_DIR}/>)

target_link_libraries(isomorph grypho)
