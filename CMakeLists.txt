cmake_minimum_required(VERSION 3.28.3)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS 
"src/*.cpp"
)

project(piston_slap)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

#Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

#glm
find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)


#glfw
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/libs/libglfw3.a)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
