cmake_minimum_required (VERSION 3.0)

project(DemoTP)

set(CMAKE_CXX_STANDARD 11)


add_subdirectory(ext/assimp)
add_subdirectory(ext/tbb)
#add_subdirectory(ext/openexr-1)
#link_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/code) 




###########################################################################
# zlib

FIND_PACKAGE ( ZLIB )
IF(NOT ZLIB_FOUND)
  # Build zlib
  SET(ZLIB_BUILD_STATIC_LIBS ON CACHE BOOL " " FORCE)
  SET(ZLIB_BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
  ADD_SUBDIRECTORY(ext/zlib)

  SET(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ext/zlib")
  SET(ZLIB_LIBRARY zlibstatic)
  SET_PROPERTY(TARGET zlibstatic PROPERTY FOLDER "ext")
  INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR} "${CMAKE_CURRENT_BINARY_DIR}/ext/zlib")
ENDIF()
INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})

###########################################################################
# OpenEXR

SET(ILMBASE_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_NAMESPACE_VERSIONING OFF CACHE BOOL " " FORCE)
SET(OPENEXR_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)
SET(ILMBASE_BUILD_SHARED_LIBS    OFF CACHE BOOL " " FORCE)

ADD_SUBDIRECTORY(ext/openexr)

SET_PROPERTY(TARGET IexMath eLut toFloat b44ExpLogTable dwaLookups IlmThread Half Iex Imath IlmImf PROPERTY FOLDER "ext")

INCLUDE_DIRECTORIES (
  ext/openexr/IlmBase/Imath
  ext/openexr/IlmBase/Half
  ext/openexr/IlmBase/Iex
  ext/openexr/OpenEXR/IlmImf
  ${CMAKE_BINARY_DIR}/ext/openexr/IlmBase/config
  ${CMAKE_BINARY_DIR}/ext/openexr/OpenEXR/config
)
IF(WIN32)
  SET(OPENEXR_LIBS IlmImf Imath Half ${ZLIB_LIBRARY})
ELSE()
  SET(OPENEXR_LIBS IlmImf Imath Half)
ENDIF()

###########################################################################


include_directories(

  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/glm/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/assimp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/stb
  ${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/include
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/rapidjson/include
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/pcg32
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/tbb/include
)

# 收集所有头文件源文件，存入 ALL_FILES 变量中
file(GLOB_RECURSE ALL_FILES "src/*.h*" "src/*.c*" "*.json")

# 定义按目录结构归类的方法
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

foreach(_source IN ITEMS ${ALL_FILES})
    assign_source_group(${_source})
endforeach()

#add 给Tutorial添加一个可执行文件 .cpp文件
add_executable(DemoTP ${ALL_FILES})

#target_link_libraries(SimpleRT ext/assimp)  

IF (APPLE)
add_custom_command(TARGET DemoTP POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/bin/Debug/libassimpd.dylib"              
    $<TARGET_FILE_DIR:DemoTP>)
ELSE()
#复制dll到工作目录
if(CMAKE_CL_64)
   add_custom_command(TARGET DemoTP POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/bin/Debug/assimp-vc141-mtd.dll"              
    $<TARGET_FILE_DIR:DemoTP>)
else()
   add_custom_command(TARGET DemoTP POST_BUILD 
   COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/bin/Debug/assimp-vc141-mtd.dll"              
    $<TARGET_FILE_DIR:DemoTP>)
endif()
ENDIF()

IF (APPLE)
target_link_libraries(DemoTP 
${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/bin/Debug/libassimpd.dylib  
${CMAKE_CURRENT_BINARY_DIR}/ext/tbb/Debug/libtbbd.dylib
#${CMAKE_CURRENT_BINARY_DIR}/ext/openexr-1/build/src/lib/OpenEXR/libOpenEXR-3_1.dylib
${OPENEXR_LIBS}
)
ELSE()
target_link_libraries(DemoTP 
${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/lib/Debug/assimp-vc141-mtd.lib  
${CMAKE_CURRENT_BINARY_DIR}/ext/tbb/Debug/tbb_debug.lib
${OPENEXR_LIBS}
)
ENDIF()

OPTION(DEBUG_MODE "Build the project using macro" ON)
IF(DEBUG_MODE)
    add_definitions("-DDEBUG_MODE")
endif(DEBUG_MODE)
