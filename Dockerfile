# Dockerfile template yoinked from `solarus/mingw-build-env`.
ARG BASE_IMAGE_VERSION=latest
FROM solarus/mingw-build-env:${BASE_IMAGE_VERSION}

# Image labels:
ARG IMAGE_VERSION=latest
LABEL description="MinGW build environment image with Meson" \
      maintainer="Me :)" \
      version="${IMAGE_VERSION}" \
      org.animo.pkglist-cmd="pacman -Q"

# Synchronise pacman repositories and update system using yay.
RUN sudo -u yay -- yay --noconfirm -Syu && \
# Install MinGW Meson wrapper.
    sudo -u yay -- yay --noconfirm --needed -S mingw-w64-meson && \
# Clear yay cache.
    sudo -u yay -- rm -rf /home/yay/.cache/*

# Export meson binary path for easier usage
ENV MESON=/usr/bin/x86_64-w64-mingw32-meson

# Expose port 8080 for miniserve
EXPOSE 8080/tcp

# Change into project directory
WORKDIR /root/
