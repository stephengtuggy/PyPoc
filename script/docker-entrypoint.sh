#!/usr/bin/env bash
# The MIT License (MIT)
#
# Copyright © 2025 Stephen G. Tuggy
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

echo "-----------------------------------------"
echo "--- docker-entrypoint.sh | 2025-11-27 ---"
echo "-----------------------------------------"

#----------------------------------
# validate parameters
#----------------------------------

while [ $# -gt 0 ]; do
  case "$1" in
    --preset_name=*)
      preset_name="${1#*=}"
      ;;
    --preset-name=*)
      preset_name="${1#*=}"
      ;;
    --build_type=*)
      build_type="${1#*=}"
      ;;
    --build-type=*)
      build_type="${1#*=}"
      ;;
    *)
      printf "***************************\n"
      printf "* Error: Invalid argument.*\n"
      printf "***************************\n"
      exit 1
      ;;
  esac
  shift
done

if [ -z "$preset_name" ] && [ -n "$PRESET_NAME" ]
then
    preset_name="${PRESET_NAME}"
fi

script/bootstrap

script/build --preset_name="${preset_name}"

build/"${preset_name}"/"${build_type}"/PyPoc
