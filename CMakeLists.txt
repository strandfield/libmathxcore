cmake_minimum_required(VERSION 3.9)
project(libmathxcore)

set(CMAKE_C_STANDARD 99)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()

##################################################################
###### code coverage build
##################################################################

if(ENABLE_COVERAGE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g ")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ftest-coverage")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

##################################################################
###### libmathxcore
##################################################################

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HDR_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)
add_library(libmathxcore ${HDR_FILES} ${SRC_FILES})
target_include_directories(libmathxcore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

foreach(_source IN ITEMS ${HDR_FILES} ${SRC_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

target_compile_definitions(libmathxcore PUBLIC -DLIBMATHXCORE_BUILD_LIB)

set(MATHXCORE_LIMB_SIZE "8" CACHE STRING "limb size")
target_compile_definitions(libmathxcore PUBLIC -DLIBMATHXCORE_LIMB_SIZE=${MATHXCORE_LIMB_SIZE})

##################################################################
###### check if we need to link to the math library ('m')
##################################################################

include(CheckFunctionExists)

CHECK_FUNCTION_EXISTS(log10 LOG10_EXISTS)
if(NOT LOG10_EXISTS)
  unset(LOG10_EXISTS)
  message("Linking to lib m")
  target_link_libraries(libmathxcore m)
endif()

##################################################################
###### tests
##################################################################

add_subdirectory(tests)

##################################################################
###### examples
##################################################################

add_subdirectory(examples)
