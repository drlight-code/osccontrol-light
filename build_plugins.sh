#!/bin/bash

function print_usage {
    printf "Usage: %s: [-j <jobs>] [-v] [<preset> ...]\n" $0
    echo ""
    echo "Build the osccontrol plugin."
    echo ""
    echo "The GUI version is always built, as standalone application and VST plugin."
    echo "An optional list of preset names can be passed to build the non-GUI version"
    echo "with automatable host controls that loads the respective preset at runtime."
    echo "Preset names may not contain spaces."
    echo ""
    echo "Options:"
    echo "<preset> ...: list of preset names separated with spaces"
    echo "-j<jobs>: run build in parallel with <jobs> threads"
    echo "-v: print cmake progress output"
}

JOBS=1
VERBOSE=0
while getopts ":hvj:" opt; do
    case ${opt} in
        \? )
            echo "Invalid option: $OPTARG" 1>&2
            print_usage
            exit 2
            ;;
        : )
            echo "Invalid option: $OPTARG requires an argument" 1>&2
            print_usage
            exit 2
            ;;
        h)
            print_usage
            exit 2
            ;;
        j)
            JOBS=$OPTARG
            case $JOBS in
                ''|*[!0-9]*)
                    echo "<jobs> must be an integer"
                    exit 2
                    ;;
            esac
            ;;
        v)
            VERBOSE=1
            ;;
    esac
done
shift $((OPTIND -1))

PRESET_NAMES=""
for preset in "$@"
do
    PRESET_NAMES="$PRESET_NAMES;$preset"
done

echo "creating cmake config in 'build' directory"
mkdir -p build ; cd build
echo "cmake -DPRESET_NAMES=$PRESET_NAMES .."
cmake -DPRESET_NAMES=$PRESET_NAMES ..

echo "building osccontrol plugins... "
if [ "$VERBOSE" -ne 0 ] ; then
    cmake --build . --parallel $JOBS
else
    cmake --build . --parallel $JOBS > /dev/null
fi

echo "copying to 'Plugins' directory... "
mkdir -p ../Plugins
cp -r ./osccontrol-light_artefacts/VST3/osccontrol-light.vst3 ../Plugins
for preset in "$@"
do
    cp -r ./osc-${preset}_artefacts/VST3/osc-${preset}.vst3 ../Plugins
done
