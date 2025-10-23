#!/bin/bash
SRC_DIR="./"
OUT_DIR="../src/main/assets/shaders"

mkdir -p "$OUT_DIR"

for file in $SRC_DIR/*.{vert,frag,comp}; do
  [ -e "$file" ] || continue
  filename=$(basename "$file")
  glslangValidator -V "$file" -o "$OUT_DIR/$filename.spv"
  echo "Compiled: $file -> $OUT_DIR/$filename.spv"
done
