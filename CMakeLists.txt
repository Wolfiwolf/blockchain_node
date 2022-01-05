cmake_minimum_required(VERSION 3.10)

project(BlockchainNode)


set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
file(GLOB SOURCE_FILES ${SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_FILES2 ${SOURCE_DIR}/*/*.cpp)
file(GLOB SOURCE_FILES3 ${SOURCE_DIR}/*/*/*/*.cpp)


add_executable(blockchain_node ${SOURCE_FILES3} ${SOURCE_FILES2} ${SOURCE_FILES})
target_link_libraries(blockchain_node -ldl -lpthread -lsecp256k1)