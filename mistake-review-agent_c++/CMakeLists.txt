cmake_minimum_required(VERSION 3.15)
project(mistake_review_agent_cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

# 只扫描我们自己写的文件，绝对不会扫到CMake的垃圾文件
set(SOURCE_FILES
        main.cpp
        src/http_client.cpp
        src/utils.cpp
        src/nodes/node1_input.cpp
        src/nodes/node2_extract.cpp
        src/nodes/node3_kimi_analysis.cpp
        src/nodes/node4_judge1.cpp
        src/nodes/node5_branch_basic.cpp
        src/nodes/node6_branch_logic.cpp
        src/nodes/node7_qwen_generate.cpp
        src/nodes/node8_judge2.cpp
        src/nodes/node9_branch_explain.cpp
        src/nodes/node10_branch_exercise.cpp
        src/nodes/node11_format.cpp
        src/nodes/node12_output.cpp
)

add_executable(mistake_review_agent_cpp ${SOURCE_FILES})

target_link_libraries(mistake_review_agent_cpp PRIVATE winhttp ws2_32)