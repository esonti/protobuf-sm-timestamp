# Makefile for Protobuf Sub-Module Timestamp C++ Implementation
# Uses buf CLI for all code generation

.PHONY: all clean proto-gen build test docker

# Configuration
SERVICE_NAME := timestamp_service
BUILD_DIR := build
BIN_DIR := bin
GENERATED_DIR := generated

# Default target
all: proto-gen build

# Clean generated files and build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(GENERATED_DIR)
	@echo "✅ Clean complete"

# Generate protocol buffer code using buf CLI
proto-gen:
	@echo "🔄 Generating protocol buffer code from GitHub APIs..."
	@echo "   Source: github.com/esonti/protobuf-sm-timestamp-api"
	buf generate
	@echo "✅ Code generation complete"

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the C++ service
build: $(BUILD_DIR)
	@echo "🔨 Building C++ timestamp service..."
	cd $(BUILD_DIR) && cmake .. && make
	@echo "✅ Build complete: $(BIN_DIR)/$(SERVICE_NAME)"

# Run tests
test: build
	@echo "🧪 Running tests..."
	cd $(BUILD_DIR) && make test
	@echo "✅ Tests complete"

# Build Docker image
docker:
	@echo "🐳 Building Docker image..."
	docker build -t protobuf-sm-timestamp:latest .
	@echo "✅ Docker image built: protobuf-sm-timestamp:latest"

# Development helpers
dev-setup:
	@echo "🛠️  Setting up development environment..."
	@echo "   Installing dependencies..."
	# Add any development setup commands here
	@echo "✅ Development setup complete"

# Lint protocol buffers
lint:
	@echo "🔍 Linting protocol buffers..."
	buf lint
	@echo "✅ Lint complete"

# Format protocol buffers
format:
	@echo "🎨 Formatting protocol buffers..."
	buf format --write
	@echo "✅ Format complete"

# Show dependency information
deps:
	@echo "📦 API Dependencies:"
	@echo "   APIs Implemented:"
	@echo "   - timestamp-api (github.com/esonti/protobuf-sm-timestamp-api)"
	@echo "   APIs Consumed:"
	@echo "   - (none - base service)"
	buf mod ls-lint-deps

# Help target
help:
	@echo "🚀 Protobuf Sub-Module Timestamp Service"
	@echo ""
	@echo "Available targets:"
	@echo "  proto-gen    Generate code from GitHub APIs"
	@echo "  build        Build C++ service"
	@echo "  test         Run tests"
	@echo "  clean        Clean build artifacts"
	@echo "  docker       Build Docker image"
	@echo "  lint         Lint protocol buffers"
	@echo "  format       Format protocol buffers"
	@echo "  deps         Show dependency information"
	@echo "  help         Show this help message"
