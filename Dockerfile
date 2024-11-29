# Base image
FROM ubuntu:20.04

# Set non-interactive frontend for apt-get
ENV DEBIAN_FRONTEND=noninteractive

# Update and install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libgtest-dev \
    python3 \
    python3-pip \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Install gtest
RUN apt-get update && apt-get install -y cmake libgtest-dev && \
    cd /usr/src/gtest && \
    cmake CMakeLists.txt && \
    make && \
    cp *.a /usr/lib

# Set the working directory
WORKDIR /app

# Copy the source code into the container
COPY . .

# Create a build directory
RUN mkdir -p build

# Build the project
WORKDIR /app/build
RUN cmake .. && make

# Expose the server port
EXPOSE 50051

# Command to run the server
CMD ["./PricingServer"]

