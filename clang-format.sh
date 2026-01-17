#!/bin/bash
echo "Running clang-format..."

find ./myvulkan/src/main/cpp/ \
  -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) \
  ! -path "*/external/*" \
  -exec clang-format -i {} +

echo "Done!"
