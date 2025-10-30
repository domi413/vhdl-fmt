FROM archlinux:base-devel

# Install system dependencies
RUN pacman -Syu --noconfirm --needed \
    python python-pip clang cmake git make ninja uv \
    && pacman -Scc --noconfirm

# Create virtual environment and install Python packages
RUN python -m venv /opt/venv \
    && /opt/venv/bin/python -m ensurepip \
    && /opt/venv/bin/pip install --no-cache-dir --upgrade pip \
    && /opt/venv/bin/pip install --no-cache-dir conan gersemi==0.19.3

# Set working directory
WORKDIR /workspace

# Make venv the default environment
ENV PATH="/opt/venv/bin:$PATH"
ENV VIRTUAL_ENV="/opt/venv"

# Default shell
CMD ["/bin/bash"]
