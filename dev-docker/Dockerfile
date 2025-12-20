# Use a suitable base image
FROM ubuntu:22.04

# Set environment variables to prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update package lists and install necessary tools
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        libsdl2-dev \
        git \
    # Clean up APT archives to reduce image size
    && rm -rf /var/lib/apt/lists/*

# Set environment variables for NodeSource
ENV NODE_VERSION=20
ENV NODESOURCE_DISTRO=noderepo

# Install Node.js and npm
RUN apt-get update && \
    apt-get install -y ca-certificates curl gnupg && \
    curl -fsSL https://deb.nodesource.com/setup_$NODE_VERSION.x | bash - && \
    apt-get install -y nodejs && \
    # Install gemini-cli globally
    npm install -g @google/gemini-cli && \
    # Clean up APT archives to reduce image size
    rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container.
# This directory will be typically mapped to your local 'study-6502' project folder by VSCode Remote - Containers.
WORKDIR /workspaces/study-6502

# Default command when the container starts. This provides an interactive shell.
CMD ["bash"]
