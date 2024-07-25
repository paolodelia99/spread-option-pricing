#!/bin/bash

BUILD_TYPE=Release
GCC_VERSION=13

help() {
    echo "build.sh [-bt|--build-type] - the type of build (Debug/Release)
                   [-gccv|--gcc-version] - the version of GCC
                   [-h|--help] - Output the help message 
    "
    exit 1
}

POSITIONAL_ARGS=()

while [[ "$#" -gt 0 ]]; do
    case $1 in
        -bt|--build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -gccv|--gcc-version)
            GCC_VERSION="$2"
            shift 2
            ;;
        -h|--help)
            help
            exit 0
            ;;
        -*|--*)
            echo "Unknown parameter passed: $1"
            exit 1
            ;;
        *)
            POSITIONAL_ARGS+=("$1")
            shift
    esac
done

# Call basedir.sh
if [[ -z $BASEDIR ]]; then
    source ${BASH_SOURCE%/*}/basedir.sh
fi

# Validate build type
if [[ "$BUILD_TYPE" != "Release" && "$BUILD_TYPE" != "Debug" ]]; then
    echo "Invalid build type. Choose 'RELEASE' or 'DEBUG'."
    exit 1
fi

# Validate GCC version
if [[ "$GCC_VERSION" != "13" && "$GCC_VERSION" != "14" ]]; then
    echo "Invalid GCC version. Choose '13' or '14'."
    exit 1
fi

export CC="/usr/bin/gcc-$GCC_VERSION"
export CXX="/usr/bin/g++-$GCC_VERSION"

BUILD_DIR=$BASEDIR/build/gcc$GCC_VERSION/$BUILD_TYPE

if [ -d $BUILD_DIR ]
then
    rm -rd $BUILD_DIR
fi

mkdir -p $BUILD_DIR

pushd $BUILD_DIR

echo "Running CMake with build type: $BUILD_TYPE"
echo "Gcc Version: $GCC_VERSION"
cmake $BASEDIR -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$BUILD_TYPE

make -j8

popd

# Echo results of make
if [[ $? -eq 0 ]]; then
    echo "Build succeeded."
else
    echo "Build failed."
fi
