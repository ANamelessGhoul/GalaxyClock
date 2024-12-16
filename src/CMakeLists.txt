project(GalaxyClock VERSION 1.0)

add_executable(${PROJECT_NAME} main.cpp)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})


target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if (${CMAKE_BUILD_TYPE} MATCHES "Release" AND ${PLATFORM} MATCHES "Desktop")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-Wl,--subsystem,windows")
endif()