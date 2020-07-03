ARG BASE_IMAGE_VERSION=latest
FROM solarus/mingw-build-env:${BASE_IMAGE_VERSION}

# Image labels
ARG IMAGE_VERSION=latest
LABEL description="MinGW build environment image with Meson" \
      maintainer="Me :)" \
      version="${IMAGE_VERSION}" \
      org.animo.pkglist-cmd="pacman -Q"

# Synchronise pacman repositories and update system using yay
RUN sudo -u yay -- yay --noconfirm -Syu

# Install Meson
RUN sudo -u yay -- yay --noconfirm --needed -S mingw-w64-meson

# Clear yay cache
RUN sudo -u yay -- rm -rf /home/yay/.cache/*