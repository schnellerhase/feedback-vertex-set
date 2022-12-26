FROM ubuntu:20.04

RUN apt-get update
RUN apt-get install tzdata -y
RUN TZ="America/New_York"

RUN apt-get upgrade -y
RUN apt-get install -y \
    build-essential \
    clang \
    clang-format \
    clang-tidy \
    cmake \
    git \
    lib32ncurses5-dev \
    libbenchmark-dev \
    libblas-dev \
    libboost-program-options-dev \
    libgmp3-dev \
    libgtest-dev \
    libreadline-dev \
    libz-dev \
    ninja-build \
    patchelf \
    wget

RUN wget -q --no-check-certificate --post-data "tname=blub&email=blub%40gmail.com&inst=&city=&country=blub" https://scipopt.org/download.php?fname=SCIPOptSuite-8.0.3-Linux-ubuntu.deb -O scipoptsuite.deb
RUN apt-get install ./scipoptsuite.deb -y
