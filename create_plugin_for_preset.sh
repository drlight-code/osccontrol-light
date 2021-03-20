#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "Please give the name of the osccontrol preset as single argument."
    exit
fi

mkdir -p build ; cd build
cmake -DPRESET_NAME=$1 ..
cmake --build . --parallel
cp -r ./osccontrol-light_artefacts/VST3/osccontrol-$1.vst3 ../Plugins
