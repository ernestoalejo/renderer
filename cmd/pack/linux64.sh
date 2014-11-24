#!/bin/sh

set -e
set -u

NAME=renderer-linux64
DEST=/tmp/pack/$NAME
VERSION=$1

# Build the binaries
scons -c
scons

# Copy the files to an independent folder
rm -rf $DEST
mkdir -p $DEST
cp bin/* $DEST
cp -R dist/renderer.node $DEST/bin
cp -R /cef/* $DEST
rm -r $DEST/include

# Compress the pack with the files
(cd /tmp/pack && tar -vpczf $NAME.tar.gz $NAME)

# Move the packs to the correct folder versioning them
rm -rf packs
mkdir packs
cd packs
mv $DEST.tar.gz $NAME.$VERSION.tar.gz
