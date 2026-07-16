#!/bin/bash

set -e

patch -d tdlib -p1 < patches/tdlib/icf.patch
