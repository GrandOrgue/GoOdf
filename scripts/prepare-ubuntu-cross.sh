#!/bin/bash

set -e

sudo apt update

sudo DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential cmake \
  pkg-config g++-mingw-w64-x86-64 imagemagick \
  gcc-mingw-w64-x86-64 mingw-w64-tools \

