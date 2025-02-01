#!/bin/bash


echo "Getting Eigen through git submodule commands"
git submodule init; git submodule update

# if [ -d lib/math/Eigen ]; then
#     echo "It looks like Eigen is already installed."
#     echo "If you want to reinstall, run rm -r lib/math/Eigen."
#     echo "If this is a fresh clone, you may just need to run `git submodule init; git submodule update`"
#     exit 1
# fi

# echo "Downloading Eigen..."
# wget https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
# tar xf eigen-3.4.0.tar.gz
# mv eigen-3.4.0/Eigen lib/math

# # Remove temp
# rm -rf eigen-3.4.0 eigen-3.4.0.tar.gz
# echo "Done!"
