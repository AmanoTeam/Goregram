#!/bin/bash

set -e

patch --forward -d tdlib -p1 < patches/tdlib/icf.patch
