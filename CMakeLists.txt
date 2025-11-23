cmake_minimum_required(VERSION 3.10)
project(HFT_Project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your source files here
add_executable(market_injector
        src/injector/main.cpp
        # add other .cpp files if needed
)
