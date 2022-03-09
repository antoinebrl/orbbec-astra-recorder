FROM ubuntu:18.04

MAINTAINER Antoine BROYELLE <antoineb@anyvision.co>
WORKDIR /astra_depth_recorder
SHELL ["/bin/bash", "-c"]

ARG DEPENDENCIES="autoconf \
                  automake \
                  build-essential \
                  cmake \
                  cpio \
                  wget \
                  gnupg2 \
                  libdrm2 \
                  libglib2.0-0 \
                  lsb-release \
                  libgtk-3-0 \
                  libtool \
                  udev \
                  unzip \
                  vim \
                  git \
                  ca-certificates \
                  python3-dev python3-numpy \
                  libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev \
                  libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev "

RUN apt-get update && \
    apt-get install -y --no-install-recommends ${DEPENDENCIES} && \
    rm -rf /var/lib/apt/lists/*


# OpenCV
RUN \
  cd /tmp && \
  git clone https://github.com/opencv/opencv.git && \
  cd opencv && mkdir build && cd build && \
  cmake -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    -D INSTALL_C_EXAMPLES=ON \
	-D INSTALL_PYTHON_EXAMPLES=ON \
	-D BUILD_EXAMPLES=ON .. && \
  make -j7 && \
  make install && \
  cd ../.. && rm -r opencv


# Astra SDK
ARG DOWNLOAD_ASTRA_SDK_LINK="http://dl.orbbec3d.com/dist/astra/v2.1.0/AstraSDK-v2.1.0-9bced77c41-Ubuntu-x86_64.zip"
RUN wget -c "${DOWNLOAD_ASTRA_SDK_LINK}" -O AstraSDK.zip && \
    unzip AstraSDK.zip && \
    tar -xf ./*Ubuntu18.04*.tar.gz && \
    rm -f AstraSDK.zip Astra*.tar.gz && \
    mv AstraSDK* AstraSDK && \
    cd AstraSDK && \
    chmod +x ./install/install.sh && \
    ./install/install.sh && \
    echo "export ASTRA_SDK_INCLUDE=/astra_depth_recorder/AstraSDK/include" >> ~/.bashrc && \
    echo "export ASTRA_SDK_LIB=/astra_depth_recorder/AstraSDK/lib" >> ~/.bashrc && \
    rm -rf lib/Plugins/obt/ lib/Plugins/liborbbec_hand.so lib/Plugins/libOrbbecBodyTracking.so  ## remove conflicting files


# Project
COPY . /astra_depth_recorder/src
RUN cd src && mkdir -p build && cd build && rm -rf * && \
  ASTRA_SDK_INCLUDE=/astra_depth_recorder/AstraSDK/include ASTRA_SDK_LIB=/astra_depth_recorder/AstraSDK/lib cmake -D CMAKE_BUILD_TYPE=Release .. && \
  make -j
