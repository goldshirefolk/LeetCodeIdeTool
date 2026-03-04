#!/bin/bash

set -e

echo "Installing Dependencies..."

if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    OS=$(uname -s)
fi

case $OS in
    ubuntu|debian|mint|pop)
        PKGMGR="sudo apt install -y"
        DEPS="build-essential cmake libcurl4-openssl-dev libjsoncpp-dev"
        ;;
    fedora|rhel|centos|nobara)
        PKGMGR="sudo dnf install -y"
        DEPS="gcc-c++ cmake libcurl-devel jsoncpp-devel"
        ;;
    arch|manjaro|endeavouros)
        PKGMGR="sudo pacman -S --noconfirm"
        DEPS="base-devel cmake curl jsoncpp"
        ;;
    opensuse*|suse)
        PKGMGR="sudo zypper install -y"
        DEPS="gcc-c++ cmake libcurl-devel libjsoncpp-devel"
        ;;
    void)
        PKGMGR="sudo xbps-install -S"
        DEPS="base-devel cmake curl-devel jsoncpp-devel"
        ;;
    alpine)
        PKGMGR="sudo apk add"
        DEPS="build-base cmake curl-dev jsoncpp-dev"
        ;;
    *)
        echo "Unsupported OS: $OS. Please install cmake, libcurl, and jsoncpp manually."
        exit 1
        ;;
esac

echo "Detected $OS. Installing dependencies..."
$PKGMGR $DEPS

rm -rf build
mkdir build && cd build

cmake ..
make

sudo cp lct /usr/local/bin/lct

mkdir -p ~/.lct
if [ ! -f ~/.lct/publicConfig ]; then
    cp ../publicConfig.txt ~/.lct/publicConfig.txt
    echo "Created config at ~/.lct/publicConfig.txt"
fi

echo "------------------------------------------------"
echo "Installation done"
echo "Binary at /usr/local/bin/lct and config file at ~/.lct/publicConfig.txt" 
echo "Run by typing lct"
echo "------------------------------------------------"