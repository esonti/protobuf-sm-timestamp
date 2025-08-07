#include "server.h"
#include <iostream>
#include <signal.h>

std::unique_ptr<timestamp::v1::Server> g_server;

void SignalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ". Shutting down..." << std::endl;
    if (g_server) {
        g_server->Shutdown();
    }
}

int main(int argc, char* argv[]) {
    // Set up signal handling
    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);
    
    // Parse command line arguments
    std::string server_address = "0.0.0.0:50051";
    if (argc > 1) {
        server_address = argv[1];
    }
    
    std::cout << "Starting Timestamp Service..." << std::endl;
    std::cout << "Server address: " << server_address << std::endl;
    
    try {
        // Create and run server
        g_server = std::make_unique<timestamp::v1::Server>();
        g_server->Run(server_address);
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "Timestamp Service shutdown complete." << std::endl;
    return 0;
}
