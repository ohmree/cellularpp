# Dockerfile template yoinked from `solarus/mingw-build-env`.
ARG BASE_IMAGE_VERSION=latest
FROM solarus/mingw-build-env:${BASE_IMAGE_VERSION}

# Image labels:
ARG IMAGE_VERSION=latest
LABEL description="MinGW build environment image with Meson" \
      maintainer="Me :)" \
      version="${IMAGE_VERSION}" \
      org.animo.pkglist-cmd="pacman -Q"

# Don't clutter the project directory with files that go in $HOME
ENV HOME=/tmp/home

# Synchronise pacman repositories and update system using yay.
RUN sudo -u yay -- yay --noconfirm -Syu && \
# Install MinGW Meson wrapper.
    sudo -u yay -- yay --noconfirm --needed -S mingw-w64-meson && \
# Clear yay cache.
    sudo -u yay -- rm -rf /home/yay/.cache/*

# Export meson binary path for easier usage
ENV MESON_WRAPPER=/usr/bin/x86_64-w64-mingw32-meson

# Change into project directory
WORKDIR /root/
