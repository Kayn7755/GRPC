//
// Created by xts on 26-1-28.
//

#ifndef GRPC_TEST_GRPCCLIENT_H
#define GRPC_TEST_GRPCCLIENT_H


#pragma once
#include "Singleton.h"
#include <grpcpp/grpcpp.h>
#include "message.grpc.pb.h"
#include "message.pb.h"
#include <queue>
#include <nlohmann/json.hpp>
#include <condition_variable>
#include <iostream>

// 通过using去掉作用域
using grpc::Channel;
using grpc::Status;
using grpc::ClientContext;

using message::command;
using message::result;
using message::Grpc_test;

class Conpool
{
public:
    Conpool(size_t poolSize,std::string host, std::string port)
        : b_stop_(false),
          poolSize_(poolSize),
          host_(std::move(host)),
          port_(std::move(port))
    {
        std::string address = host_ + ":" + port_;
        std::cout << "[Conpool] 创建连接池，目标地址: " << address << std::endl;
        for(size_t i = 0; i < poolSize;i++)
        {
//             std::shared_ptr<Channel> channel = grpc::CreateChannel(address,
//                                                                    grpc::InsecureChannelCredentials());
            connections_.push(std::move(Grpc_test::NewStub(grpc::CreateChannel(address,
                                                                               grpc::InsecureChannelCredentials()))));
        }
    }

    ~Conpool() {
        std::lock_guard<std::mutex> lock(mutex_);
        Close();
        while (!connections_.empty()) {
            connections_.pop();
        }
    }

    std::unique_ptr<Grpc_test::Stub> getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] {
            if (b_stop_) {
                return true;
            }
            return !connections_.empty();
        }); // 先加锁、通过条件变量(lambda)等待锁

        if (b_stop_) {
            return  nullptr;
        }
        auto context = std::move(connections_.front()); // connections_.fro
        connections_.pop();
        return context;
    }
    void returnConnection(std::unique_ptr<Grpc_test::Stub> context) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            return;
        }
        connections_.push(std::move(context));
        cond_.notify_one();
    }
    void Close() {
        b_stop_ = true;
        cond_.notify_all();
    }


private:
    std::atomic<bool> b_stop_;
    size_t poolSize_;
    std::string host_;
    std::string port_;
    std::queue<std::unique_ptr<Grpc_test::Stub>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

class GrpcClient :public Singleton<GrpcClient> // 模板可继承自己,因为编译期不会过多检查,运行期才会,此时已经生成好了
{
    friend class Singleton<GrpcClient>;
public:
    ~GrpcClient() {

    }
    // 重写grpc(proto文件)中定义的部分操作
    result _donig_fuc(std::string want_to_say,const command& command);

private:
    GrpcClient();
    std::unique_ptr<Conpool> _pools_ptr;
};



#endif //GRPC_TEST_GRPCCLIENT_H
