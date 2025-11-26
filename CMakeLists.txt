cmake_minimum_required(VERSION 3.10)
project(HFT_Project)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Setup threading support
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# 1. Market Injector
add_executable(market_injector src/injector/main.cpp)

# 2. Mock Exchange
add_executable(mock_exchange src/exchange/main.cpp)
target_link_libraries(mock_exchange Threads::Threads)

# 3. Strategy Engine
# Note: We include StrategyEngine.cpp here as we separated it
add_executable(trading_strategy
    src/strategy/main.cpp
    src/strategy/StrategyEngine.cpp
)
target_link_libraries(trading_strategy Threads::Threads)