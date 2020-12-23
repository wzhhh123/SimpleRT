cmake_minimum_required (VERSION 3.0)

project(SimpleRT)

set(CMAKE_CXX_STANDARD 17)


#add_subdirectory(ext/assimp)
#link_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/code) 

include_directories(

  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/ext/glm/glm

)

# 收集所有头文件源文件，存入 ALL_FILES 变量中
file(GLOB_RECURSE ALL_FILES "src/*.h*" "src/*.c*")

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
add_executable(SimpleRT ${ALL_FILES})

#arget_link_libraries(SimpleRT ${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/lib/Debug/assimpd.lib ${CMAKE_CURRENT_BINARY_DIR}/ext/assimp/lib/Debug/assimp-vc141-mtd.lib)
