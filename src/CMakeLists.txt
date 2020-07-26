PROJECT(rtspserver)

include_directories(
    ${RTSP_SRC_PATHH}
    ${ROOT_PATH}/src
)
LINK_DIRECTORIES(
    ${ROOT_INCLUDE}
    ${RTSP_SRC_PATHH}
)
AUX_SOURCE_DIRECTORY(./ SRCFILE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
link_libraries(
    dl
    rt
    pthread
)
ADD_LIBRARY(
    ${MODULE_NAME}
    rtspserver
    ${SRCFILE}
)
