FROM ubuntu:26.04

ENV DEBIAN_FRONTEND=noninteractive
ENV PATH="/opt/cmake/bin:/home/ubuntu/.local/bin:${PATH}"

RUN apt-get update && apt-get install -y --no-install-recommends \
    bash-completion \
    build-essential \
    ca-certificates \
    clang-format \
    clangd \
    curl \
    gdb \
    git \
    libgmock-dev \
    libgtest-dev \
    libspdlog-dev \
    ninja-build \
    pipx \
    pkg-config \
    python3 \
    python3-full \
    python3-pip \
    python3-venv \
    qml6-module-qtquick \
    qml6-module-qtquick-controls \
    qml6-module-qtquick-layouts \
    qml6-module-qttest \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-declarative-dev-tools \
    qt6-tools-dev \
    qt6-tools-dev-tools \
    sudo \
    tar \
    xz-utils \
    && rm -rf /var/lib/apt/lists/*

RUN set -eux; \
    case "$(dpkg --print-architecture)" in \
    amd64) cmake_arch=x86_64 ;; \
    arm64) cmake_arch=aarch64 ;; \
    *) echo "Unsupported architecture: $(dpkg --print-architecture)" >&2; exit 1 ;; \
    esac; \
    archive="cmake-4.4.3-linux-${cmake_arch}.tar.gz"; \
    cd /tmp; \
    curl -fsSLO "https://cmake.org/files/v4.4/${archive}"; \
    curl -fsSLO https://cmake.org/files/v4.4/cmake-4.4.3-SHA-256.txt; \
    checksum="$(awk -v file="${archive}" '$2 == file { print $1 }' cmake-4.4.3-SHA-256.txt)"; \
    test -n "${checksum}"; \
    echo "${checksum}  ${archive}" | sha256sum --check -; \
    mkdir -p /opt/cmake; \
    tar -xzf "${archive}" --strip-components=1 -C /opt/cmake; \
    rm -f "${archive}" cmake-4.4.3-SHA-256.txt

ENV PATH="/usr/lib/qt6/bin:${PATH}"

RUN mkdir -p /tmp/user /tmp/.X11-unix \
    && chmod 1777 /tmp/user /tmp/.X11-unix

RUN set -eux; \
    test "$(dpkg --print-architecture)" = amd64; \
    archive=sen-0.6.0-x86_64-linux-gnu-12.4.0-release.tar.gz; \
    curl -fsSLo "/tmp/${archive}" \
    "https://github.com/airbus/sen/releases/download/0.6.0/${archive}"; \
    echo "f683a03532ab468e68c0288c225eed87eac6bca1b3282ed60540d1cd7574462e  /tmp/${archive}" \
    | sha256sum --check -; \
    mkdir -p /opt/sen/0.6.0; \
    tar -xzf "/tmp/${archive}" --strip-components=1 -C /opt/sen/0.6.0; \
    test -x /opt/sen/0.6.0/bin/sen; \
    rm -f "/tmp/${archive}"

ENV SEN_PREFIX="/opt/sen/0.6.0"
ENV PATH="${SEN_PREFIX}/bin:${PATH}"
ENV CMAKE_PREFIX_PATH="${SEN_PREFIX}/cmake"
ENV LD_LIBRARY_PATH="${SEN_PREFIX}/lib:${SEN_PREFIX}/bin"

RUN usermod -aG sudo ubuntu \
    && echo 'ubuntu ALL=(root) NOPASSWD:ALL' > /etc/sudoers.d/ubuntu \
    && chmod 0440 /etc/sudoers.d/ubuntu \
    && mkdir -p /home/ubuntu/.local/share/bash-completion/completions \
    && chown -R ubuntu:ubuntu /home/ubuntu/.local \
    && echo "if [ -f /usr/share/bash-completion/bash_completion ]; then . /usr/share/bash-completion/bash_completion; fi" >> /home/ubuntu/.bashrc \
    && echo "source /usr/share/bash-completion/completions/git 2>/dev/null || true" >> /home/ubuntu/.bashrc

WORKDIR /workspaces/sen-xplane-gui
USER ubuntu

RUN pipx install 'conan>=2,<3' \
    && pipx install pre-commit \
    && pipx install cmakelang

CMD ["bash"]
