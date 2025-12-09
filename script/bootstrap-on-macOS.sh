#!/usr/bin/env bash
# The MIT License (MIT)
#
# Copyright © 2022-2025 Stephen G. Tuggy
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the “Software”), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

set -e

echo "------------------------------------------"
echo "--- bootstrap-on-macOS.sh | 2025-12-08 ---"
echo "------------------------------------------"

brew install \
    gcc \
    python3 \
    boost-python3 \
    ninja \
    abseil \
    nlohmann-json \
    protobuf-c \
    re2 \
    grpc \
    autoconf \
    autoconf-archive \
    automake \
    libtool

# Only install cmake if it isn't installed yet
brew ls --versions cmake || brew install cmake

if [ -z "$VCPKG_ROOT" ]
then
    export VCPKG_ROOT="$(pwd)/../vcpkg"
fi
git clone https://github.com/microsoft/vcpkg.git "$VCPKG_ROOT"
export PATH="$VCPKG_ROOT:$PATH"
pushd "$VCPKG_ROOT"
./bootstrap-vcpkg.sh
popd
