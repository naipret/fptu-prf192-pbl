#!/bin/bash
# Dọn dẹp file cũ
rm -f obj/*.o main.exe

# Biên dịch toàn bộ file trong src
gcc -std=c11 src/*.c -Iinclude -o main.exe -Wall \
&& echo "=====================" \
&& echo "[OK] Build success!" \
&& echo "=====================" \
&& ./main.exe \
|| echo "[ERROR] Build failed!"