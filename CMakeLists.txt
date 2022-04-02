cmake_minimum_required(VERSION 3.1)
set(CMAKE_TOOLCHAIN_FILE Arduino-CMake-Toolchain/Arduino-toolchain.cmake)
project(NextMultitool)
set(CMAKE_CXX_STANDARD 20)

file(GLOB ${PROJECT_NAME}_SRC "src/*.cpp" "src/*.h" "src/*.hpp")
add_executable(NextMultitool ${${PROJECT_NAME}_SRC})
target_link_arduino_libraries(NextMultitool PRIVATE
        IRremote LiquidCrystal WiFi ESPmDNS WebServer
        core)
target_enable_arduino_upload(NextMultitool)
