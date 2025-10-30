FROM archlinux:base-devel

# Install all required dependencies
RUN pacman -Sy --needed --noconfirm \
    clang \
    cmake \
    git \
    make \
    ninja \
    uv \
    && pacman -Scc --noconfirm

# Create venv and install Python packages
RUN uv venv /opt/venv && \
    . /opt/venv/bin/activate && \
    uv pip install conan gersemi==0.19.3

# Setup Conan profile
RUN . /opt/venv/bin/activate

# Set working directory
WORKDIR /workspace

# Activate venv by default
ENV PATH="/opt/venv/bin:$PATH"
ENV VIRTUAL_ENV="/opt/venv"

# Default command
CMD ["/bin/bash"]
