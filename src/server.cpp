#include "server.h"
#include "timestamp_service_impl.h"
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/health_check_service_interface.h>
#include <iostream>

namespace timestamp {
namespace v1 {

Server::Server() : service_(std::make_unique<TimestampServiceImpl>()) {
}

Server::~Server() {
    Shutdown();
}

void Server::Run(const std::string& server_address) {
    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    
    grpc::ServerBuilder builder;
    
    // Listen on the given address without any authentication mechanism
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    
    // Register service
    builder.RegisterService(service_.get());
    
    // Build and start the server
    server_ = builder.BuildAndStart();
    std::cout << "Timestamp Server listening on " << server_address << std::endl;
    
    // Wait for the server to shutdown
    server_->Wait();
}

void Server::Shutdown() {
    if (server_) {
        server_->Shutdown();
    }
}

} // namespace v1
} // namespace timestamp
