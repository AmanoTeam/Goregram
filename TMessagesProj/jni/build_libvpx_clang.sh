#!/bin/bash
set -e
function build_one {
	echo "Building ${ARCH}..."

	export AR=${LLVM_BIN}/llvm-ar
	export STRIP=${LLVM_BIN}/llvm-strip
	export RANLIB=${LLVM_BIN}/llvm-ranlib
	export NM=${LLVM_BIN}/llvm-nm

	export CC_PREFIX="${LLVM_BIN}/${CLANG_PREFIX}-linux-${BIN_MIDDLE}${ANDROID_API}-"

	export CC=${CC_PREFIX}clang
	export CXX=${CC_PREFIX}clang++
	export AS=${CC_PREFIX}clang++
	export LD=${CC}
	export CROSS_PREFIX=${LLVM_BIN}/llvm-


	export CFLAGS="-DANDROID -fpic -fpie"
	export CPPFLAGS="${CFLAGS}"
	export CXXFLAGS="${CFLAGS} -std=c++17"
	export ASFLAGS="-D__ANDROID__"

	echo "Cleaning..."
	make clean || true

	echo "Configuring..."

	./configure \
	--prefix=${PREFIX} \
	--target=${TARGET} \
	${CPU_DETECT} \
	--as=yasm \
	--enable-static \
	--enable-pic \
	--disable-docs \
	--enable-libyuv \
	--enable-small \
	--enable-optimizations \
	--enable-better-hw-compatibility \
	--disable-examples \
	--disable-tools \
	--disable-debug \
	--disable-neon-asm \
	--disable-neon-dotprod \
	--disable-unit-tests \
	--disable-install-docs \
	--enable-realtime-only \
	--enable-vp8 \
	--enable-vp9 \
	--disable-webm-io

	make -j$COMPILATION_PROC_COUNT install
}

function setCurrentPlatform {

	CURRENT_PLATFORM="$(uname -s)"
	case "${CURRENT_PLATFORM}" in
		Darwin*)
			BUILD_PLATFORM=darwin-x86_64
			COMPILATION_PROC_COUNT=`sysctl -n hw.physicalcpu`
			;;
		Linux*)
			BUILD_PLATFORM=linux-x86_64
			COMPILATION_PROC_COUNT=$(nproc)
			;;
		*)
			echo -e "\033[33mWarning! Unknown platform ${CURRENT_PLATFORM}! falling back to linux-x86_64\033[0m"
			BUILD_PLATFORM=linux-x86_64
			COMPILATION_PROC_COUNT=1
			;;
	esac

	echo "Build platform: ${BUILD_PLATFORM}"
	echo "Parallel jobs: ${COMPILATION_PROC_COUNT}"

}

function checkPreRequisites {

	if ! [ -d "libvpx" ] || ! [ "$(ls -A libvpx)" ]; then
		echo -e "\033[31mFailed! Submodule 'libvpx' not found!\033[0m"
		echo -e "\033[31mTry to run: 'git submodule init && git submodule update'\033[0m"
		exit
	fi

	if [ -z "$NDK" -a "$NDK" == "" ]; then
		echo -e "\033[31mFailed! NDK is empty. Run 'export NDK=[PATH_TO_NDK]'\033[0m"
		exit
	fi
}

setCurrentPlatform
checkPreRequisites

cd libvpx

## common
LLVM_PREFIX="${NDK}/toolchains/llvm/prebuilt/linux-x86_64"
LLVM_BIN="${LLVM_PREFIX}/bin"
VERSION="4.9"
ANDROID_API=21

function build {
	for arg in "$@"; do
		case "${arg}" in
			arm64)
				ARCH=arm64
				ARCH_NAME=aarch64
				PREBUILT_ARCH=aarch64
				CLANG_PREFIX=aarch64
				BIN_MIDDLE=android
				CPU=arm64-v8a
				TARGET="arm64-android-gcc"
				PREFIX=./build/$CPU
				CPU_DETECT="--disable-runtime-cpu-detect"
				build_one
			;;
			arm)
				ARCH=arm
				ARCH_NAME=arm
				PREBUILT_ARCH=arm
				CLANG_PREFIX=armv7a
				BIN_MIDDLE=androideabi
				CPU=armeabi-v7a
				TARGET="armv7-android-gcc --enable-neon --disable-neon-asm"
				PREFIX=./build/$CPU
				CPU_DETECT="--disable-runtime-cpu-detect"
				build_one
			;;
			*)
			;;
		esac
	done
}

if (( $# == 0 )); then
	build arm arm64
else
	build $@
fi
