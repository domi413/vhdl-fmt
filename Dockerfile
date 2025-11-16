# ==================================================
# Stage 1: Builder image
# ==================================================
FROM fedora:43 AS builder

# Install system dependencies
RUN dnf install -y --setopt=install_weak_deps=false \
    python3 \
    clang \
    clang-tools-extra \
    llvm \
    cmake \
    git \
    make \
    ninja-build \
    nodejs \
    && dnf clean all

# Verify LLVM coverage tools are available
RUN llvm-profdata --version && llvm-cov --version

# Get the latest version of uv
COPY --from=ghcr.io/astral-sh/uv:latest /uv /usr/local/bin/uv

# Setup venv
RUN uv venv /opt/venv --seed \
    && /opt/venv/bin/pip install --no-cache-dir conan gersemi==0.19.3

ENV PATH="/opt/venv/bin:$PATH"
ENV VIRTUAL_ENV="/opt/venv"

WORKDIR /app

# Copy only what's needed for dependency resolution
COPY conanfile.txt clang.profile ./

# Prefill Conan dependencies (equivalent to `make conan`)
RUN conan install . \
    --profile:host=clang.profile \
    --profile:build=clang.profile \
    --build=missing \
    -s build_type=Debug && \
    rm -rf /root/.conan2/p/tmp

# ==================================================
# Stage 2.1: Development image
# ==================================================
FROM builder AS dev

RUN dnf install -y --setopt=install_weak_deps=false \
    lldb \
    ccache \
    && dnf clean all

# Add colored prompt for root
RUN echo 'export PS1="\[\e[31m\]\u\[\e[0m\]@\[\e[34m\]\h\[\e[0m\]:\[\e[36m\]\w\[\e[0m\] > "' >> /root/.bashrc

WORKDIR /app
CMD ["/bin/bash"]

# ==================================================
# Stage 2.2: CI image
# ==================================================
FROM builder AS ci

WORKDIR /app
