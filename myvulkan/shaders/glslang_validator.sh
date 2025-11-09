#!/bin/bash
SRC_DIR="./"
OUT_DIR="../src/main/assets/shaders"

find "$SRC_DIR" -type f \( -name "*.vert" -o -name "*.frag" -o -name "*.comp" -o -name "*.geom" \) | while read -r file; do
  # 相対パス（./ を削除）
  relative_path="${file#./}"

  # 出力ファイルは元の拡張子を残して .spv を追加
  out_file="$OUT_DIR/${relative_path}.spv"

  # 出力ディレクトリを作成
  mkdir -p "$(dirname "$out_file")"

  # GLSL → SPIR-V
  glslangValidator -V "$file" -o "$out_file"

  if [ $? -eq 0 ]; then
      echo "Compiled: $file -> $out_file"
  else
      echo "Failed to compile: $file"
  fi
done
