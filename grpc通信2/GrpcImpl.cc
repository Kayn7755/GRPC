//
// Created by xts on 26-1-28.
//

#include "GrpcImpl.h"
#include <nlohmann/json.hpp>
#include <iostream>

GrpcImpl::GrpcImpl() {}

Status GrpcImpl::_donig_fuc(grpc::ServerContext* context,
                            const message::command* request,
                            message::result* reply)
{
    // 简单示例：打印收到的参数，并返回 success = 1
    std::cout << "Server recv: echo=" << request->echo()
              << ", doing_fun1=" << request->doing_fun1()
              << ", doing_fun2=" << request->doing_fun2() << std::endl;

    grpc_doing();

    reply->set_success(1);
    reply->set_error("");
    return Status::OK;
}

void GrpcImpl::grpc_doing()
{
    std::cout << "执行主机1的函数调用" << std::endl;
}
