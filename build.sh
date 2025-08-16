#!/bin/bash

defines="-DENGINE"
warnings="-Wall -Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch"

# Start with fixed include paths
includes="-Ithird_party -Ithird_party/Include -I./lib/SDL/include"

# Add all subdirectories under include
for dir in $(find include -type d); do
    includes="$includes -I$dir"
done

src_files=$(find src -name "*.cpp")
timestamp=$(date +%s)

# Check for -format flag in any position
for arg in "$@"; do
    if [[ "$arg" == "-format" ]]; then
        echo "Running clang-format on source files..."
        files=$(find ./src ./include \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \))
        if [[ -z "$files" ]]; then
            echo "No matching files found."
            exit 1
        fi
        echo "Running clang-format..."
        echo "Files: $files"
        echo "$files" | xargs clang-format -style=file -i
        echo "Formatting complete."
        exit 0
    fi
done

if [[ "$(uname)" == "Linux" ]]; then
    echo "Running on Linux"
    libs="-lX11 -lGL -lfreetype"
    outputFile=game
    rm -f game_*
    clang $includes -g $src_files -shared -fPIC -o game_$timestamp.so $warnings $defines $libs
    mv game_$timestamp.so game.so

elif [[ "$(uname)" == "Darwin" ]]; then
    echo "Running on Mac"
    libs="$(pkg-config --cflags --libs sdl3)"
    outputFile="build/my_app"
    rm -f build/my_app_*
    clang $includes -g $src_files -o $outputFile $warnings $defines $libs

else
    echo "Running on Windows"
    libs="-luser32 -lopengl32 -lgdi32 -lole32 -Lthird_party/lib -lfreetype.lib"
    outputFile=game.exe
    rm -f game_*
    clang $includes -g $src_files -shared -o game_$timestamp.dll $warnings $defines $libs
    mv game_$timestamp.dll game.dll
fi