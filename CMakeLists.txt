cmake_minimum_required(VERSION 3.9)
project(libmathxcore)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # disables warning about DLL export 
  # this is annoying when using C++ STL
  add_compile_options(/wd4251)  
endif()


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

add_subdirectory(tests)