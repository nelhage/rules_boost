#!/usr/bin/env bash

# This is a helper script to update the config header for LZMA, i.e.
# - config.lzma-android.h,
# - config.lzma-ios-arm64.h
# - config.lzma-ios-armv7.h
# - config.lzma-ios-i386.h
# - config.lzma-linux.h
# - config.lzma-osx-arm64.h
# - config.lzma-osx-x86_64.h
# - config.lzma-windows.h
#
# Those files are dependent on the target OS and architecture.
# Every time the version number of xz is bumped the corresponding config headers should be updated.
# Note: This script has to be executed on macOS, Linux, and Windows since the configuration of the headers is specific to the underlying OS environment

# Fail on error, etc.
set -euxo pipefail

# Get script location
script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Path where the WORKSPACE file is located
rules_boost_dir=$script_dir 

# Create a temporary directory
tmpdir=$(mktemp -d) 

# Get rid of temporary files when script exits
trap "rm -rf $tmpdir" EXIT 

# Version number of xz
xz_version_number="5.4.0"

# Download, and untar xz
cd "$tmpdir"
curl -sL 'https://cfhcable.dl.sourceforge.net/project/lzmautils/xz-'$xz_version_number'.tar.gz' --output 'xz-'$xz_version_number'.tar.gz'
tar -xf 'xz-'$xz_version_number'.tar.gz'

# Switch to xz directory
cd 'xz-'$xz_version_number

# config header files depend on the specifc OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    ./configure
    cp config.h "${rules_boost_dir}/config.lzma-linux.h"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    CC="clang" ./configure
    cp config.h "${rules_boost_dir}/config.lzma-osx-${HOSTTYPE}.h"

    CC="clang -arch arm64 \
    -isysroot $(xcrun --sdk iphoneos --show-sdk-path) \
    ./configure --host=$MACHTYPE
    cp config.h ${rules_boost_dir}/config.lzma-ios-arm64.h"

    rm config.h
    CC="clang -arch armv7 \
    -isysroot $(xcrun --sdk iphoneos --show-sdk-path) \
    ./configure --host=$MACHTYPE
    cp config.h ${rules_boost_dir}/config.lzma-ios-armv7.h"

    rm config.h
    CC="clang -arch i386 \
    -isysroot $(xcrun --sdk iphonesimulator --show-sdk-path) \
    ./configure --host=$MACHTYPE
    cp config.h ${rules_boost_dir}/config.lzma-ios-i386.h"
elif [[ "$OSTYPE" == "msys"* ]]; then
     ./configure
    cp config.h "${rules_boost_dir}/config.windows.h"
else
    echo "Unsupported OS"
    echo "$OSTYPE"
fi
