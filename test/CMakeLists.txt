PROJECT(test)
include_directories(
    ${RTSP_SRC_PATHH}
    ${ROOT_INCLUDE}
    ${Base_SRC_PATH}
)
LINK_DIRECTORIES(
    ${LIB_PATH}
)
link_libraries(
    rtspserver
    Env
)
set(SRC1 ${TEST_PATH}/rtspserver_test.cpp)
add_executable(rtspserverTest ${SRC1})

set(SRC2 ${TEST_PATH}/testTimeer.cpp)
add_executable(testTimeer ${SRC2})

set(SRC3 ${TEST_PATH}/testSignalThread.cpp)
add_executable(testSignalThread ${SRC3})
