//
// Created by xts on 26-1-28.
//

#include "GrpcClient.h"

GrpcClient::GrpcClient()
{
    _pools_ptr = std::make_unique<Conpool>(5,"192.168.245.128","50055"); // 此处填对端grpc服务器端口号

}

result GrpcClient::_donig_fuc(std::string want_to_say, const message::command &command)
{
    std::cout<<"want to say: "<<want_to_say<<std::endl;

    result rsp;
    auto &pool = _pools_ptr;
    ClientContext context;
    auto stub = pool->getConnection();
    if (!stub) {
        rsp.set_success(0);
        rsp.set_error("connection pool stopped / no stub");
        return rsp;
    }

    Status status = stub->_donig_fuc(&context,command,&rsp); // 传入grpc的,而不是自己的内容
    pool->returnConnection(std::move(stub));

    if (!status.ok()) {
        std::cout<<"status错误,错误码: "<<status.error_code()
                 <<", 错误信息: "<<status.error_message()<<std::endl;
        rsp.set_success(0);
        rsp.set_error(status.error_message());
        return rsp;
    }
    std::cout<<"调用成功"<<std::endl;
    return rsp;
}