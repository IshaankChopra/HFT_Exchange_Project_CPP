cmake_minimum_required(VERSION 3.10)
project(HFT_Project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add Release Flags for Maximum Speed (-O3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

# Setup threading support
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Market Injector
add_executable(market_injector src/injector/main.cpp)

# Mock Exchange
add_executable(mock_exchange src/exchange/main.cpp)
target_link_libraries(mock_exchange Threads::Threads)

# Strategy Engine
add_executable(trading_strategy
    src/strategy/main.cpp
    src/strategy/StrategyEngine.cpp
)
target_link_libraries(trading_strategy Threads::Threads)