#!/usr/bin/env bash

set -eu

declare -r APP_DIRECTORY="$(realpath "$(( [ -n "${BASH_SOURCE}" ] && dirname "$(realpath "${BASH_SOURCE[0]}")" ) || dirname "$(realpath "${0}")")")"

export NDK="${ANDROID_HOME}/ndk/27.3.13750724"

PATH+=":${NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin"

cd "${APP_DIRECTORY}"

git -C ffmpeg stash
git -C libvpx stash
git -C dav1d stash
git -C boringssl stash
git -C tdlib stash

git submodule update --init --remote
git -C ffmpeg checkout 44b04492bfc83215e136f2a68783bff71d328692

./build_ffmpeg_clang.sh
./patch_ffmpeg.sh

./build_libvpx_clang.sh

./build_dav1d_clang.sh

./patch_boringssl.sh
./build_boringssl.sh

./patch_tdlib.sh
./build_tde2e.sh

