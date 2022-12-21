#!/bin/bash

function print_usage {
    printf "Usage: %s: [-j <jobs>] [-v] [-e] [<preset> ...]\n" $0
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
    echo "-v: verbosely print cmake output"
    echo "-e: embed presets during build"
}

JOBS=1
VERBOSE=0
EMBED=""
while getopts ":hvej:" opt; do
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
        e)
            EMBED="-DEMBED_PRESETS=ON"
            ;;
    esac
done
shift $((OPTIND -1))

PRESETS=""
for preset in "$@"
do
    PRESETS="$PRESETS;$preset"
done

echo "creating cmake config in 'build' directory"
mkdir -p build ; cd build

COMMAND="cmake -DPRESETS=$PRESETS $EMBED .."
echo $COMMAND
$COMMAND

echo "building osccontrol plugins... "
if [ "$VERBOSE" -ne 0 ] ; then
    cmake --build . --parallel $JOBS
else
    cmake --build . --parallel $JOBS > /dev/null
fi

echo -n "copying VST3 plugins into 'Plugins' directory... "
mkdir -p ../Plugins
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    for preset in "$@"
    do
        find ./osc-${preset}_artefacts/VST3/osc-${preset}.vst3 -type f -name '*.so' -exec cp {} ../Plugins \;
    done
fi
cp ./osccontrol-light_artefacts/VST3/osccontrol-light.vst3/Contents/*/osccontrol-light.so ../Plugins
echo "done!"

echo -n "copying GUI application into top-level source directory... "
cp ./osccontrol-light_artefacts/Standalone/osccontrol-light ../
echo "done!"
