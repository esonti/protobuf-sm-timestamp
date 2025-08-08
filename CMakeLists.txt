cmake_minimum_required(VERSION 3.15)
project(timestamp_service)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Don't use FetchContent - use system packages only
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Proto files - these will be copied from central registry
set(PROTO_PATH "${CMAKE_CURRENT_SOURCE_DIR}/proto")
file(GLOB PROTO_FILES "${PROTO_PATH}/*.proto")

# Generate protobuf and gRPC files
foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_NAME ${PROTO_FILE} NAME_WE)
    set(GENERATED_PROTOBUF_PATH "${CMAKE_CURRENT_BINARY_DIR}")
    
    set(PROTO_SOURCES "${GENERATED_PROTOBUF_PATH}/${PROTO_NAME}.pb.cc")
    set(PROTO_HEADERS "${GENERATED_PROTOBUF_PATH}/${PROTO_NAME}.pb.h")
    set(GRPC_SOURCES "${GENERATED_PROTOBUF_PATH}/${PROTO_NAME}.grpc.pb.cc")
    set(GRPC_HEADERS "${GENERATED_PROTOBUF_PATH}/${PROTO_NAME}.grpc.pb.h")
    
    add_custom_command(
        OUTPUT ${PROTO_SOURCES} ${PROTO_HEADERS} ${GRPC_SOURCES} ${GRPC_HEADERS}
        COMMAND protoc
        ARGS --cpp_out=${GENERATED_PROTOBUF_PATH}
             --grpc_out=${GENERATED_PROTOBUF_PATH}
             --plugin=protoc-gen-grpc=`which grpc_cpp_plugin`
             --proto_path=${PROTO_PATH}
             ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
        COMMENT "Generating protobuf and gRPC files for ${PROTO_NAME}"
    )
    
    list(APPEND PROTO_GENERATED_FILES ${PROTO_SOURCES} ${PROTO_HEADERS} ${GRPC_SOURCES} ${GRPC_HEADERS})
endforeach()

# Create protobuf library
add_library(proto_lib ${PROTO_GENERATED_FILES})
target_link_libraries(proto_lib ${Protobuf_LIBRARIES} gRPC::grpc++ gRPC::grpc++_reflection)

# Source files
set(SOURCES
    src/main.cpp
    src/timestamp_service_impl.cpp
    src/server.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    proto_lib
    ${Protobuf_LIBRARIES}
    gRPC::grpc++
    gRPC::grpc++_reflection
    Threads::Threads
)

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
