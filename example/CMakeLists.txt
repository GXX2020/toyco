set(
BASE_LIST
${PROJECT_SOURCE_DIR}/src/base/CountDownLatch.cc
${PROJECT_SOURCE_DIR}/src/base/CurrentThread.cc
${PROJECT_SOURCE_DIR}/src/base/Thread.cc
${PROJECT_SOURCE_DIR}/src/base/TimeStamp.cc
)

set(
SRC_LIST
${PROJECT_SOURCE_DIR}/src/Context.cc
${PROJECT_SOURCE_DIR}/src/Coroutine.cc
${PROJECT_SOURCE_DIR}/src/Epoll.cc
${PROJECT_SOURCE_DIR}/src/Processor.cc
${PROJECT_SOURCE_DIR}/src/ProcessorSelector.cc
${PROJECT_SOURCE_DIR}/src/Scheduler.cc
${PROJECT_SOURCE_DIR}/src/tinyCoApi.cc
${PROJECT_SOURCE_DIR}/src/Timer.cc
${PROJECT_SOURCE_DIR}/src/SSCall.cc
)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/example/bin)

find_package(Threads)

set(TEST_BASELIB_LIST ${PROJECT_SOURCE_DIR}/example/test_baselib.cc)
add_executable(test_baselib ${BASE_LIST} ${TEST_BASELIB_LIST})
target_link_libraries (test_baselib ${CMAKE_THREAD_LIBS_INIT})

# set(TEST_COROUTINE_LIST ${PROJECT_SOURCE_DIR}/example/test_Coroutine.cc)
# add_executable(test_Coroutine ${TEST_COROUTINE_LIST} ${SRC_LIST} ${BASE_LIST})
# target_link_libraries (test_Coroutine ${CMAKE_THREAD_LIBS_INIT})

set(TEST_TIMER_LIST ${PROJECT_SOURCE_DIR}/example/test_Timer.cc)
add_executable(test_Timer ${TEST_TIMER_LIST} ${SRC_LIST} ${BASE_LIST})
target_link_libraries (test_Timer ${CMAKE_THREAD_LIBS_INIT})

set(TEST_SSCALL_LIST ${PROJECT_SOURCE_DIR}/example/test_SSCall.cc)
add_executable(test_SSCall ${TEST_SSCALL_LIST} ${SRC_LIST} ${BASE_LIST})
target_link_libraries (test_SSCall ${CMAKE_THREAD_LIBS_INIT})

set(TEST_SOCKET_LIST ${PROJECT_SOURCE_DIR}/example/test_Socket.cc)
add_executable(test_Socket ${TEST_SOCKET_LIST} ${SRC_LIST} ${BASE_LIST})
target_link_libraries (test_Socket ${CMAKE_THREAD_LIBS_INIT})
