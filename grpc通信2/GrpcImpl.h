//
// Created by xts on 26-1-28.
//

#ifndef GRPC_TEST_GRPCIMPL_H
#define GRPC_TEST_GRPCIMPL_H
#pragma once
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <mutex>
#include <memory>


using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status; // gRPC 中最重要的返回值类型。它本质上是一个状态结构体，用来表示一次远程调用是“成功”还是“失败”。
using message::result;
using message::command;
using message::Grpc_test;


class GrpcImpl final: public Grpc_test::Service
{
public:
    GrpcImpl();

    // 实现 proto 中定义的 rpc：_donig_fuc(command) returns (result)
    Status _donig_fuc(ServerContext* context, const command* request,
                      result* reply) override;

    void grpc_doing();
//private:
//    std::shared_ptr<CServer> _p_server;
};



#endif //GRPC_TEST_GRPCIMPL_H
