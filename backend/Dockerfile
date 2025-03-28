# Base image with build tools and C++17 support
FROM ubuntu:latest

# Set noninteractive mode for apt
ENV DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    unzip \
    pkg-config \
    libmysqlclient-dev \
    libssl-dev \
    curl \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/* 

# Set working directory
WORKDIR /deps

RUN git clone --recurse-submodules -b v1.62.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc.git

RUN mkdir -p /deps/grpc/build && cd /deps/grpc/build && cmake -DgRPC_INSTALL=ON \
    -DgRPC_BUILD_TESTS=OFF .. && make -j8 install

WORKDIR /backend

# Copy your entire project into the container
COPY . .

# Create build directory
RUN mkdir -p build 

# Build your project
RUN cd build && \
    cmake .. && \
    cmake --build . --target server

# Set the default command to run your app
CMD ["./build/server"]
