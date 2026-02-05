#include <iostream>
#include <thread>
#include <chrono>
#include <grpcpp/grpcpp.h>
#include "GrpcImpl.h"
#include "GrpcClient.h"


// 测试git
// 第二次测试
// ...
int main()
{
    std::string server_address("192.168.245.128:50055");

    GrpcImpl service;
    grpc::ServerBuilder builder;
    builder.AddListeningPort(
            server_address,
            grpc::InsecureServerCredentials()
    );
    builder.RegisterService(&service);

    std::unique_ptr<grpc::Server> server = builder.BuildAndStart();
    if (!server) {
        std::cerr << "Failed to start gRPC server\n";
        return -1;
    }

    std::cout << "gRPC Server listening on " << server_address << std::endl;
    
    // 等待一下，确保对端服务器(grpc通信2)已经启动
    std::cout << "等待对端服务器启动..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    command com;
    com.set_echo(111);
    com.set_doing_fun1(222);
    com.set_doing_fun2(333);

    std::cout << "开始调用对端服务..." << std::endl;
    GrpcClient::GetInstance()->_donig_fuc("hellow",com);

    

    // 正确：阻塞运行
    server->Wait();

    return 0;
}
// ewdfesd