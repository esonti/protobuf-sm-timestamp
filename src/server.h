#pragma once

#include <grpcpp/grpcpp.h>
#include <memory>

namespace timestamp {
namespace v1 {

class TimestampServiceImpl;

class Server {
public:
    Server();
    ~Server();

    // Start the server on the specified address
    void Run(const std::string& server_address = "0.0.0.0:50051");
    
    // Shutdown the server gracefully
    void Shutdown();

private:
    std::unique_ptr<grpc::Server> server_;
    std::unique_ptr<TimestampServiceImpl> service_;
};

} // namespace v1
} // namespace timestamp
