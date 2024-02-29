#!/bin/bash

set -e

sudo apt update

sudo DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential cmake \
  libwxgtk3.0-gtk3-dev imagemagick

