# Use Ubuntu as the base image
FROM ubuntu:latest

# Set environment variables to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install build tools and dependencies
RUN apt-get update && \
    apt-get install -y \
    g++ \
    make \
    cmake \
    libmysqlclient-dev \
    protobuf-compiler \
    libprotobuf-dev \
    libpthread-stubs0-dev \
    wget \
    curl \
    ca-certificates \
    gnupg2 \
    lsb-release \
    build-essential \
    git \
    && apt-get clean

# Install Protobuf from the official repository
#RUN curl -sSL https://github.com/protocolbuffers/protobuf/releases/download/v21.4/protobuf-all-21.4.tar.gz -o protobuf.tar.gz && \
 #   tar -xzf protobuf.tar.gz && \
  #  cd protobuf-*/ && \
   # ./configure && \
  #  make && \
  #  make install && \
  #  ldconfig && \
  #  cd .. && \
  #  rm -rf protobuf.tar.gz protobuf-*/

# Install gRPC from the official source
#RUN git clone --recurse-submodules -b v1.45.0 https://github.com/grpc/grpc && \
#    cd grpc && \
#    mkdir -p cmake/build && \
#    cd cmake/build && \
#    cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ../.. && \
#    make && \
#    make install && \
#    cd ../.. && \
#    rm -rf grpc
# Set working directory
WORKDIR /backend

# Copy your C++ source files into the container
COPY ./backend /backend

# Create a build directory and move into it
RUN mkdir -p /backend/build
WORKDIR /backend/build

# Run cmake and make to build the project
RUN cmake .. && make

# Expose the port the backend will run on
EXPOSE 50052

# Command to run your compiled C++ backend
CMD ["./server"]
