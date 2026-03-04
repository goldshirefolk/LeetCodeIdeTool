#!/bin/bash

set -e

echo "Installing Dependencies..."
sudo apt update
sudo apt install -y build-essential cmake libcurl4-openssl-dev libjsoncpp-dev

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