#!/bin/bash

# Require elevated privileges
if [[ $EUID -ne 0 ]]; then
  echo "This script must be run with root privileges."
  exit 1
fi

# Fix Hash Sum Mismatch
mkdir /etc/gcrypt
echo all >> /etc/gcrypt/hwf.deny
apt-get update

# Update the package lists for upgrades and new package installations
apt-get update

# Install clang-format
apt-get install -y clang-format

# Install pip
apt-get install -y python3-pip

# Install lizard
pip3 install lizard

# Install jinja2
pip3 install jinja2

# meld

# Install diff
sudo apt-get install diffutils

# Install Google Test
sudo apt-get install -y libgtest-dev
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
cd lib
sudo cp *.a /usr/lib
