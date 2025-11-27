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
echo "--- bootstrap-on-linux.sh | 2025-11-26 ---"
echo "------------------------------------------"

UPDATE_ALL_SYSTEM_PACKAGES="$1"

if [ -f /etc/os-release ]
then
    OS_RELEASE_LOCATION="/etc/os-release"
elif [ -f /usr/lib/os-release ]
then
    OS_RELEASE_LOCATION="/usr/lib/os-release"
else
    echo "os-release file not found; unable to continue"
    exit 1
fi
LINUX_ID=$(grep ^ID= $OS_RELEASE_LOCATION | sed 's/^ID=//' | tr -d '"\n')
echo "LINUX_ID = ${LINUX_ID}"
LINUX_CODENAME=$(grep ^VERSION_CODENAME= $OS_RELEASE_LOCATION | sed 's/^VERSION_CODENAME=//' | tr -d '"\n')
echo "LINUX_CODENAME = ${LINUX_CODENAME}"
LINUX_VERSION_ID=$(grep ^VERSION_ID= $OS_RELEASE_LOCATION | sed 's/^VERSION_ID=//' | tr -d '"\n')
echo "LINUX_VERSION_ID = ${LINUX_VERSION_ID}"

function bootstrapOnDebian()
{
    apt-get update

    if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
    then
        apt-get -qy upgrade
    fi

    case "$LINUX_CODENAME" in
        "trixie")
            apt-get -qy install \
                            git \
                            cmake \
                            python3-dev \
                            build-essential \
                            libboost-python-dev \
                            lsb-release \
                            nlohmann-json3-dev \
                            libprotobuf-dev \
                            opentelemetry-cpp-dev \
                            make \
                            pkg-config \
                            ninja-build
            ;;
        "bookworm")
            apt-get -qy remove \
                            libboost-python-dev \
                            libboost-log-dev \
                            libboost-regex-dev
            apt-get -qy autoremove
            apt-get -qy install \
                            git \
                            cmake \
                            python3-dev \
                            build-essential \
                            libboost-python1.81-dev \
                            lsb-release \
                            nlohmann-json3-dev \
                            libprotobuf-dev \
                            opentelemetry-cpp-dev \
                            make \
                            pkg-config \
                            ninja-build
            ;;
        "bullseye"|"buster"|"stretch")
            echo "Sorry, Debian ${LINUX_CODENAME} is no longer supported"
            exit 2
            ;;
        *)
            echo "Sorry, this version of Debian is unsupported"
            exit 2
            ;;
    esac
}

function bootstrapOnUbuntu()
{
    apt-get update

    if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
    then
        apt-get -qy upgrade
    fi

    case "$LINUX_CODENAME" in
        "questing")
            apt-get -qy install \
                            git \
                            cmake \
                            python3-dev \
                            build-essential \
                            libboost-python-dev \
                            lsb-release \
                            make \
                            pkg-config \
                            nlohmann-json3-dev \
                            libprotobuf-dev \
                            opentelemetry-cpp-dev \
                            ninja-build
            ;;
        "plucky")
            apt-get -qy install \
                            git \
                            cmake \
                            python3-dev \
                            build-essential \
                            libboost-python-dev \
                            lsb-release \
                            make \
                            pkg-config \
                            nlohmann-json3-dev \
                            libprotobuf-dev \
                            opentelemetry-cpp-dev \
                            ninja-build
            ;;
        "noble")
            apt-get -qy install \
                            git \
                            cmake \
                            python3-dev \
                            build-essential \
                            libboost-python-dev \
                            lsb-release \
                            make \
                            pkg-config \
                            nlohmann-json3-dev \
                            libprotobuf-dev \
                            ninja-build
            ;;
        "jammy"|"hirsute"|"impish"|"focal"|"bionic"|"xenial")
            echo "Sorry, Ubuntu ${LINUX_CODENAME} is no longer supported"
            exit 2
            ;;
        *)
            echo "Sorry, this version of Ubuntu is unsupported"
            exit 2
            ;;
    esac
}

function bootstrapOnLinuxMint ()
{
    apt-get update

    if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
    then
        apt-get -qy upgrade
    fi

    echo "Linux Mint base Distros do NOT support SDL3"
    case "$LINUX_CODENAME" in
        "wilma"|"zara")
            apt-get -qy install \
                            git \
                            cmake \
                            python3-dev \
                            build-essential \
                            libboost-python-dev \
                            lsb-release \
                            make \
                            pkg-config \
                            nlohmann-json3-dev \
                            libprotobuf-dev \
                            ninja-build
            ;;
        "virginia"|"victoria"|"vera"|"vanessa"|"ulyana")
            echo "Sorry, Linux Mint ${LINUX_CODENAME} is no longer supported"
            exit 2
            ;;
        *)
            echo "This version of Linux Mint is not directly supported. You may be able to use the corresponding Ubuntu installation package"
            exit 2
            ;;
    esac
}

function bootstrapOnOpenSuseLeap ()
{
    case "${LINUX_VERSION_ID}" in
        "15.1"|"15.2"|"15.3"|"15.4"|"15.5")
            echo "Sorry, openSUSE Leap ${LINUX_VERSION_ID} is no longer supported"
            exit 2
            ;;
        "15.6")
            zypper --non-interactive refresh
            zypper --non-interactive install -y \
                                    libboost_python-py3-1_75_0-devel \
                                    cmake \
                                    gcc-c++ \
                                    python3-devel \
                                    git \
                                    rpm-build \
            ;;
        "16.0")
            zypper --non-interactive refresh
            zypper --non-interactive install -y \
                                    libboost_python-py3-1_86_0-devel \
                                    cmake \
                                    gcc-c++ \
                                    python3-devel \
                                    git \
                                    rpm-build
            ;;
        *)
            echo "Sorry, this version of openSUSE Leap is unsupported"
            exit 2
            ;;
    esac
}

function bootstrapOnFedora ()
{
    case "${LINUX_VERSION_ID}" in
        30|31|32|33|34|35|36|37|38|39)
            echo "Sorry, Fedora ${LINUX_VERSION_ID} is no longer supported"
            exit 2
            ;;
        40)
            dnf install -y \
                                git \
                                cmake \
                                boost-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        41)
            dnf install -y \
                                git \
                                cmake \
                                boost-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        42)
            dnf install -y \
                                git \
                                cmake \
                                boost-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        43)
            dnf install -y \
                                git \
                                cmake \
                                boost-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        *)
            echo "Sorry, this version of Fedora is unsupported"
            exit 2
            ;;
    esac
}

function bootstrapOnRedHat ()
{
    case "${LINUX_VERSION_ID}" in
        "9.6")
            if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
            then
                dnf -y upgrade --refresh
            fi
            dnf -y update
            dnf -y install \
                                git \
                                cmake \
                                boost-devel \
                                boost-python3-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        "10.0")
            if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
            then
                dnf -y upgrade --refresh
            fi
            dnf -y update
            dnf -y install \
                                git \
                                cmake \
                                boost-devel \
                                boost-python3-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        "10.1")
            if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
            then
                dnf -y upgrade --refresh
            fi
            dnf -y update
            dnf -y install \
                                git \
                                cmake \
                                boost-devel \
                                boost-python3-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        *)
            echo "Sorry, this version of Red Hat is unsupported"
            exit 2
            ;;
    esac
}

function bootstrapOnRockyLinux ()
{
    case "${LINUX_VERSION_ID}" in
        "9.6")
            if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
            then
                dnf -y upgrade --refresh
            fi
            dnf -y update
            dnf -y install \
                                git \
                                cmake \
                                boost-devel \
                                boost-python3-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        "10.0")
            if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
            then
                dnf -y upgrade --refresh
            fi
            dnf -y update
            dnf -y install \
                                git \
                                cmake \
                                boost-devel \
                                boost-python3-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        "10.1")
            if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
            then
                dnf -y upgrade --refresh
            fi
            dnf -y update
            dnf -y install \
                                git \
                                cmake \
                                boost-devel \
                                boost-python3-devel \
                                gcc-c++ \
                                python3-devel \
                                rpm-build \
                                make \
                                json-devel
            ;;
        *)
            echo "Sorry, this version of Rocky Linux is unsupported"
            exit 2
            ;;
    esac
}

function bootstrapOnManjaro ()
{
        pacman -Sy --noconfirm archlinux-keyring manjaro-keyring

        if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
        then
            pacman -Syyu --refresh --noconfirm
        fi

        pacman -Sy --noconfirm cmake \
                         boost \
                         gcc \
                         gcc-libs \
                         python \
                         git \
                         make
}

function bootstrapOnFuntoo ()
{
    ego sync
    dispatch-conf
    # enable `autounmask-write` so that USE flags
    # change in the image appropriately
    USE="-libffi -userland_GNU" emerge --autounmask-write \
              cmake \
              boost \
              python \
              git \
              make
}

function bootstrapOnArch ()
{
    if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
    then
        pacman -Syyu --refresh --noconfirm
    fi

    # NOTE: Arch requires GCC 12 right now
    # also installing latest GCC.
    pacman -Sy --noconfirm \
              base-devel \
              cmake \
              boost \
              gcc \
              gcc12 \
              python \
              git \
              make
}

function bootstrapOnEndeavourOS ()
{
    if [ "${UPDATE_ALL_SYSTEM_PACKAGES}" -eq 1 ]
    then
        pacman -Syyu --refresh --noconfirm
    fi

    # NOTE: Arch requires GCC 12 right now
    # also installing latest GCC.
    pacman -Sy --noconfirm \
              base-devel \
              icu \
              cmake \
              boost \
              gcc \
              sdl2 \
              python \
              git \
              make
}

case "${LINUX_ID}" in
    "debian")
        bootstrapOnDebian
        ;;
    "ubuntu")
        bootstrapOnUbuntu
        ;;
    "linuxmint")
        bootstrapOnLinuxMint
        ;;
    "opensuse-leap")
        bootstrapOnOpenSuseLeap
        ;;
    "fedora")
        bootstrapOnFedora
        ;;
    "rhel")
        bootstrapOnRedHat
        ;;
    "redhat")
        bootstrapOnRedHat
        ;;
    "rocky")
        bootstrapOnRockyLinux
        ;;
    "manjaro")
        bootstrapOnManjaro
        ;;
    "funtoo")
        bootstrapOnFuntoo
        ;;
    "arch")
        bootstrapOnArch
        ;;
    "endeavouros")
        bootstrapOnEndeavourOS
        ;;
    *)
        echo "Sorry, unrecognized/unsupported Linux distribution"
        exit 2
        ;;
esac

mkdir -p /usr/src/PyPoc

echo "Bootstrapping finished!"
