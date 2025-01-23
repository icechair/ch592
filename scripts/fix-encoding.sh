#!/bin/bash

convert_inplace() {
  file=$1
  if iconv -f GB18030 -t UTF-8 $file > "$file.new"; then
    mv -f "$file.new" "$file"
  else
    echo "$file: iconv failed"
  fi
}

#c files
find . -type f -iname "*.c"  -print0 | while read -r -d '' file; do
  convert_inplace $file
done

#h files
find . -type f -iname "*.h"  -print0 | while read -r -d '' file; do
  convert_inplace $file
done

#s files
find . -type f -iname "*.s"  -print0 | while read -r -d '' file; do
  convert_inplace $file
done
