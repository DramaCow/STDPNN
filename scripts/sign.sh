for file in *.*pp; do sed -i '1i /*\n  author: Sam Coward\n  date: Sept 2017\n*/\n' "$file"; done
