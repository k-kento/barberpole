#!/bin/bash
SRC_DIR="./"
OUT_DIR="../src/main/assets/shaders"

find "$SRC_DIR" -type f \( -name "*.vert" -o -name "*.frag" -o -name "*.comp" -o -name "*.geom" \) | while read -r file; do
  relative_path=$(realpath --relative-to="$SRC_DIR" "$file")
  out_file="$OUT_DIR/${relative_path}.spv"
  mkdir -p "$(dirname "$out_file")"
  glslangValidator -V "$file" -o "$out_file"
  echo "Compiled: $file -> $out_file"
done
