# =========================
# Stage 1: Builder (shared base)
# =========================
FROM archlinux:base-devel AS builder

# Install system dependencies
RUN pacman -Syu --noconfirm --needed \
    python python-pip clang cmake git make ninja uv \
    && pacman -Scc --noconfirm \
    && rm -rf /var/cache/pacman/pkg/* /tmp/*

# Setup venv
RUN python -m venv /opt/venv \
    && /opt/venv/bin/python -m ensurepip \
    && /opt/venv/bin/pip install --no-cache-dir --upgrade pip \
    && /opt/venv/bin/pip install --no-cache-dir conan gersemi==0.19.3

ENV PATH="/opt/venv/bin:$PATH"
ENV VIRTUAL_ENV="/opt/venv"

WORKDIR /app

# Copy only what’s needed for dependency resolution
COPY conanfile.txt clang.profile ./

# Prefill Conan dependencies (equivalent to `make conan`)
RUN conan profile detect || true && \
    conan install . \
    --profile:host=clang.profile \
    --profile:build=clang.profile \
    --build=missing \
    -s build_type=Debug && \
    rm -rf /root/.conan2/p/tmp

# =========================
# Stage 2: Development image
# =========================
FROM builder AS dev

# Add colored prompt for root
RUN echo 'export PS1="\[\e[31m\]\u\[\e[0m\]@\[\e[34m\]\h\[\e[0m\]:\[\e[36m\]\w\[\e[0m\] > "' >> /root/.bashrc

WORKDIR /app

CMD ["/bin/bash"]

# =========================
# Stage 3: CI image
# =========================
FROM builder AS ci

WORKDIR /app
