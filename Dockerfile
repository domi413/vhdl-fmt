FROM debian:bookworm

# Install system dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    clang \
    bash \
    python3 \
    pipx \
    git \
    make \
 && rm -rf /var/lib/apt/lists/*

# Install Conan 2
RUN pipx install "conan==2.*" && \
    pipx ensurepath

ENV PATH="/root/.local/bin:$PATH"

WORKDIR /app

# Copy project files
COPY src ./src
COPY tests ./tests
COPY grammars ./grammars
COPY examples ./examples
COPY conan_profiles ./conan_profiles
COPY conanfile.txt ./
COPY CMakeLists.txt ./
COPY Makefile ./

# Configure Conan
RUN conan profile detect || true

# Build the project
RUN make

CMD ["make", "run"]
