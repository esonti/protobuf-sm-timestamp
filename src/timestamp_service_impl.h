#pragma once

#include <grpcpp/grpcpp.h>
#include "timestamp.grpc.pb.h"
#include <memory>

using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;

namespace timestamp {
namespace v1 {

class TimestampServiceImpl final : public TimestampService::Service {
public:
    TimestampServiceImpl();
    ~TimestampServiceImpl() override = default;

    // Get current timestamp
    Status GetCurrentTimestamp(
        ServerContext* context,
        const GetCurrentTimestampRequest* request,
        GetCurrentTimestampResponse* response) override;

    // Stream timestamps at regular intervals
    Status StreamTimestamps(
        ServerContext* context,
        const StreamTimestampsRequest* request,
        ServerWriter<TimestampMessage>* writer) override;

private:
    // Helper methods
    TimestampMessage CreateTimestampMessage(
        const std::string& source,
        const std::string& message = "",
        const std::string& timezone = "UTC",
        TimestampPrecision precision = TIMESTAMP_PRECISION_NANOSECONDS) const;
        
    std::string GetSystemTimezone() const;
    int64_t GetProcessingStartTime() const;
};

} // namespace v1
} // namespace timestamp
