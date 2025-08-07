#include "timestamp_service_impl.h"
#include <chrono>
#include <thread>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace timestamp {
namespace v1 {

TimestampServiceImpl::TimestampServiceImpl() {
    // Constructor
}

Status TimestampServiceImpl::GetCurrentTimestamp(
    ServerContext* context,
    const GetCurrentTimestampRequest* request,
    GetCurrentTimestampResponse* response) {
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // Get the requested precision, default to nanoseconds
    TimestampPrecision precision = request->precision();
    if (precision == TIMESTAMP_PRECISION_UNSPECIFIED) {
        precision = TIMESTAMP_PRECISION_NANOSECONDS;
    }
    
    // Get timezone, default to UTC
    std::string timezone = request->timezone();
    if (timezone.empty()) {
        timezone = "UTC";
    }
    
    // Create timestamp message
    TimestampMessage* timestamp_msg = response->mutable_timestamp_message();
    *timestamp_msg = CreateTimestampMessage(
        request->source(),
        "Current timestamp response",
        timezone,
        precision
    );
    
    // Calculate processing time
    auto end_time = std::chrono::high_resolution_clock::now();
    auto processing_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        end_time - start_time).count();
    response->set_processing_time_ns(processing_time);
    
    return Status::OK;
}

Status TimestampServiceImpl::StreamTimestamps(
    ServerContext* context,
    const StreamTimestampsRequest* request,
    ServerWriter<TimestampMessage>* writer) {
    
    int32_t interval_ms = request->interval_ms();
    if (interval_ms <= 0) {
        interval_ms = 1000; // Default to 1 second
    }
    
    int32_t max_count = request->max_count();
    int32_t count = 0;
    
    TimestampPrecision precision = request->precision();
    if (precision == TIMESTAMP_PRECISION_UNSPECIFIED) {
        precision = TIMESTAMP_PRECISION_NANOSECONDS;
    }
    
    std::string timezone = request->timezone();
    if (timezone.empty()) {
        timezone = "UTC";
    }
    
    while (!context->IsCancelled()) {
        // Check max count limit
        if (max_count > 0 && count >= max_count) {
            break;
        }
        
        // Create and send timestamp message
        TimestampMessage timestamp_msg = CreateTimestampMessage(
            request->source(),
            "Stream timestamp #" + std::to_string(count + 1),
            timezone,
            precision
        );
        
        if (!writer->Write(timestamp_msg)) {
            break; // Client disconnected
        }
        
        count++;
        
        // Sleep for the specified interval
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
    }
    
    return Status::OK;
}

TimestampMessage TimestampServiceImpl::CreateTimestampMessage(
    const std::string& source,
    const std::string& message,
    const std::string& timezone,
    TimestampPrecision precision) const {
    
    TimestampMessage timestamp_msg;
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    
    // Create protobuf timestamp
    google::protobuf::Timestamp* proto_timestamp = timestamp_msg.mutable_timestamp();
    
    // Set seconds
    proto_timestamp->set_seconds(time_t_now);
    
    // Set nanoseconds based on precision
    auto nanos_since_epoch = std::chrono::duration_cast<std::chrono::nanoseconds>(
        now.time_since_epoch()).count();
    int32_t nanos_fraction = nanos_since_epoch % 1000000000;
    
    switch (precision) {
        case TIMESTAMP_PRECISION_SECONDS:
            proto_timestamp->set_nanos(0);
            break;
        case TIMESTAMP_PRECISION_MILLISECONDS:
            proto_timestamp->set_nanos((nanos_fraction / 1000000) * 1000000);
            break;
        case TIMESTAMP_PRECISION_MICROSECONDS:
            proto_timestamp->set_nanos((nanos_fraction / 1000) * 1000);
            break;
        case TIMESTAMP_PRECISION_NANOSECONDS:
        default:
            proto_timestamp->set_nanos(nanos_fraction);
            break;
    }
    
    // Set other fields
    timestamp_msg.set_source(source);
    timestamp_msg.set_message(message);
    timestamp_msg.set_timezone(timezone);
    timestamp_msg.set_precision(precision);
    
    return timestamp_msg;
}

std::string TimestampServiceImpl::GetSystemTimezone() const {
    // Simplified timezone detection - in production, use proper timezone library
    return "UTC";
}

int64_t TimestampServiceImpl::GetProcessingStartTime() const {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

} // namespace v1
} // namespace timestamp
