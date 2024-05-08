cmake_minimum_required(VERSION 3.10)
project(Texpa VERSION 1.0)
add_executable(Texpa src/main.cpp)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_FILES
    "MyClass.hpp"
    "MyClass.cpp" # Covers shared implementation parts
)
if(IOS)
    # Covers iOS implementation
    list(APPEND SOURCE_FILES "ios/MyClass.mm")
elseif(APPLE)
    # Covers macOS implementation
    list(APPEND SOURCE_FILES "macos/MyClass.mm")
elseif(ANDROID)
    # Covers Android implementation
    list(APPEND SOURCE_FILES "android/MyClass.cpp")
elseif(WIN32)
    # Covers Windows implementation
    list(APPEND SOURCE_FILES "windows/MyClass.cpp")
endif()