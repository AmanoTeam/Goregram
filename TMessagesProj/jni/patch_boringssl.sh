#!/bin/bash

set -e

patch -d boringssl -p1 < patches/boringssl/0001-add-aes-ige-mode.patch
patch -d boringssl -p1 < patches/boringssl/0002-do-not-build-tests.patch
patch -d boringssl -p1 < patches/boringssl/0003-only-build-what-we-need.patch
patch -d boringssl -p1 < patches/boringssl/0004-Add-missing-includes.patch
