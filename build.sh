RAY_PATH="./raylib/raylib-5.5_linux_amd64"
SOURCES="main.c fplist.c"
CFLAGS="-g -ggdb -Wall -Wextra"
cc $CFLAGS $SOURCES \
   -I"$RAY_PATH/include/" \
   -Wl,-rpath="$RAY_PATH/lib" \
   -L"$RAY_PATH/lib" -l:libraylib.so \
   -ldl -lm -lpthread \
   -o flapeng
