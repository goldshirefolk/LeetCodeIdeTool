#!/bin/bash
# Just copy the pre-made binary
sudo cp bin/lct /usr/local/bin/lct
sudo chmod +x /usr/local/bin/lct
mkdir -p ~/.lct
if [ ! -f ~/.lct/publicConfig ]; then
    cp ../publicConfig.txt ~/.lct/publicConfig.txt
    echo "Created config at ~/.lct/publicConfig.txt"
fi

echo "------------------------------------------------"
echo "Static installation done"
echo "Binary at /usr/local/bin/lct and config file at ~/.lct/publicConfig.txt" 
echo "Run by typing lct"
echo "------------------------------------------------"