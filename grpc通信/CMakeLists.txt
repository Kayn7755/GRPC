cmake_minimum_required(VERSION 3.16)
project(grpc_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===== gRPC & Protobuf =====
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# ===== nlohmann/json =====
find_package(nlohmann_json QUIET)

add_executable(grpc_test
        main.cc
        GrpcImpl.cc
        GrpcClient.cc
        message.pb.cc
        message.grpc.pb.cc
)

target_link_libraries(grpc_test
        PRIVATE
        gRPC::grpc++
        protobuf::libprotobuf
)