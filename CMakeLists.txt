cmake_minimum_required(VERSION 3.0)
project(QitbitBC)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_MIN_VERSION "1.55")

find_package(Boost ${BOOST_MIN_VERSION} REQUIRED system)
find_package(Threads)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

include_directories(${Boost_INCLUDE_DIR})
include_directories("include")

link_directories(${Boost_LIBRARY_DIRS})

add_executable(QitbitBC bc.cpp
               src/transaction.cpp include/transaction.h
               src/block.cpp include/block.h
               src/SHA256.cpp include/SHA256.h
               src/blockchain.cpp include/blockchain.h
               src/q_blockchain.cpp include/q_blockchain.h
               src/qitbit.cpp include/qitbit.h
               src/types.cpp include/types.h)

add_executable(TestBC test.cpp)

target_link_libraries(QitbitBC ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${CURSES_LIBRARIES})
target_link_libraries(TestBC ${CURSES_LIBRARIES})
