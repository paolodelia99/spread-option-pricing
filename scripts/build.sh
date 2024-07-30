#!/bin/bash

BUILD_TYPE=Release
COMPILER=gcc
COMPILERPP=g++
COMPILER_VERSION=13
USE_CLANG=OFF

help() {
    echo "build.sh [-bt|--build-type] - the type of build (Debug/Release)
                   [-c|--compiler] - compiler type (gcc/clang)
                   [-v|--version] - the version of the compiler
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
        -v|--version)
            COMPILER_VERSION="$2"
            shift 2
            ;;
        -c|--compiler)
            COMPILER="$2"
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

# Check type compiler
if [[ "$COMPILER" != "gcc" && "$COMPILER" != "clang" ]]; then
    echo "Invalid compiler. Choose 'gcc' or 'clang'"
    exit 1
fi

# Validate GCC version
if [[ "$COMPILER" ==  "gcc" ]]; then
    COMPILERPP=g++
    if [[ "$COMPILER_VERSION" != "13" && "$COMPILER_VERSION" != "14" ]]; then
        echo "Invalid GCC version. Choose '13' or '14'."
        exit 1
    fi
fi

if [[ "$COMPILER" == "clang" ]]; then
    COMPILERPP=clang++
    USE_CLANG=ON
    if [[ "$COMPILER_VERSION" != "17" && "$COMPILER_VERSION" != "18" ]]; then
        echo "Invalid Clang version. Choose '17' or '18'."
        exit 1
    fi
fi

export CC="/usr/bin/$COMPILER-$COMPILER_VERSION"
export CXX="/usr/bin/$COMPILERPP-$COMPILER_VERSION"

BUILD_DIR=$BASEDIR/build/$COMPILER$COMPILER_VERSION/$BUILD_TYPE

if [ -d $BUILD_DIR ]
then
    rm -rd $BUILD_DIR
fi

mkdir -p $BUILD_DIR

pushd $BUILD_DIR

echo "Running CMake with build type: $BUILD_TYPE"
echo "Using compiler: $COMPILER"
echo "$COMPILER Version: $GCC_VERSION"
cmake $BASEDIR -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DUSE_CLANG=$USE_CLANG

make -j8

popd

# Echo results of make
if [[ $? -eq 0 ]]; then
    echo "Build succeeded."
else
    echo "Build failed."
fi
