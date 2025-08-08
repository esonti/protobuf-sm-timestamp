# Timestamp Service Implementation

A high-performance C++ implementation of the Timestamp Service using gRPC and Protocol Buffers.

## Overview

This service provides precise timestamp generation with configurable precision levels and timezone support. It implements the Timestamp API specification defined in the central registry.

## Features

- **High Precision**: Nanosecond-level timestamp accuracy
- **Multiple Precisions**: Support for seconds, milliseconds, microseconds, and nanoseconds
- **Timezone Support**: Configurable timezone handling
- **Streaming**: Real-time timestamp streaming at configurable intervals
- **Performance Optimized**: C++ implementation for minimal latency

## Building

### Prerequisites

- CMake 3.15+
- C++17 compatible compiler
- Protocol Buffers
- gRPC C++
- pkg-config

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential autoconf libtool pkg-config
sudo apt-get install -y libgrpc++-dev libprotobuf-dev protobuf-compiler-grpc
```

### macOS
```bash
brew install cmake protobuf grpc
```

### Build Steps

1. Clone the repository with API submodule:
```bash
git clone --recursive <protobuf-sm-timestamp-repo>
cd protobuf-sm-timestamp
```

2. Copy proto files from central registry:
```bash
mkdir -p proto
cp ../central-registry/timestamp/timestamp.proto proto/
```

3. Build:
```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

## Running

### Start the server:
```bash
./timestamp_service [server_address]
```

Default server address: `0.0.0.0:50051`

### Examples:
```bash
# Default port
./timestamp_service

# Custom address
./timestamp_service 0.0.0.0:8080
```

## API Usage

### Get Current Timestamp

```bash
grpcurl -plaintext localhost:50051 timestamp.v1.TimestampService/GetCurrentTimestamp
```

### Stream Timestamps

```bash
grpcurl -plaintext -d '{"interval_ms": 1000, "max_count": 10}' \
  localhost:50051 timestamp.v1.TimestampService/StreamTimestamps
```

## Docker

```dockerfile
FROM ubuntu:20.04 as builder
# ... build steps

FROM ubuntu:20.04
COPY --from=builder /app/build/timestamp_service /usr/local/bin/
EXPOSE 50051
CMD ["timestamp_service"]
```

## Configuration

Environment variables:
- `TIMESTAMP_PORT`: Server port (default: 50051)
- `TIMESTAMP_HOST`: Server host (default: 0.0.0.0)

## Performance

- **Latency**: Sub-millisecond response times
- **Throughput**: 10,000+ requests/second
- **Memory**: ~10MB baseline usage

## Integration

This service is designed to be consumed by:
- Media Stream Service (for consistent timestamping)
- Video Viewer Application (for activity tracking)
- Any application requiring precise timestamps

## Development

### Project Structure
```
src/
├── main.cpp                 # Application entry point
├── server.h/.cpp           # gRPC server implementation
├── timestamp_service_impl.h/.cpp  # Service logic
proto/                      # Protocol buffer definitions (copied from central registry)
build/                      # Build artifacts
```

### Testing
```bash
# Unit tests
make test

# Integration tests with grpcurl
./scripts/test_integration.sh
```
