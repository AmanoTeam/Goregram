#!/usr/bin/env bash

set -eu

declare -r APP_DIRECTORY="$(realpath "$(( [ -n "${BASH_SOURCE}" ] && dirname "$(realpath "${BASH_SOURCE[0]}")" ) || dirname "$(realpath "${0}")")")"

export NDK="${ANDROID_HOME}/ndk/27.3.13750724"

PATH+=":${NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin"

cd "${APP_DIRECTORY}"

./build_libvpx_clang.sh

./build_ffmpeg_clang.sh
./patch_ffmpeg.sh

./build_dav1d_clang.sh

./build_libressl.sh

./patch_tdlib.sh
./build_tde2e.sh

