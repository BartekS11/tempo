#!/bin/bash

defines="-DENGINE"
warnings="-Wno-writable-strings -Wno-format-security -Wno-deprecated-declarations -Wno-switch"
includes="-Ithird_party -Ithird_party/Include"

timestamp=$(date +%s)

# Check for -format flag in any position
for arg in "$@"; do
    if [[ "$arg" == "-format" ]]; then
        echo "Running clang-format on source files..."
        files=$(find ./src \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \))

        files+=$(find ./include \( -name "*.c" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" \))
        if [[ -z "$files" ]]; then
            echo "No matching files found in ./src."
            exit 0
        fi
        echo "Running clang-format..."
        echo "Files: $files"
        echo "$files" | xargs clang-format -style=file -i
        echo "Formatting complete."
        exit 0
    fi
done
#NOTE: Check on linux and windows

if [[ "$(uname)" == "Linux" ]]; then
    echo "Running on Linux"
    libs="-lX11 -lGL -lfreetype"
    outputFile=game

    rm -f game_* # Remove old game_* files
    clang -g "src/main.cpp" -shared -fPIC -o game_$timestamp.so $warnings $defines
    mv game_$timestamp.so game.so

elif [[ "$(uname)" == "Darwin" ]]; then
    echo "Running on Mac"
    libs="-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a "
    outputFile="build/my_app"
    #TODO: Work on creating game as shared library to be able to hot reload
    rm -f /build/my_app_* # Remove old game_* files

else
    echo "Running on Windows"
    libs="-luser32 -lopengl32 -lgdi32 -lole32 -Lthird_party/lib -lfreetype.lib"
    outputFile=game.exe

    rm -f game_* # Remove old game_* files
    clang -g "src/main.cpp" -shared -o game_$timestamp.dll $warnings $defines
    mv game_$timestamp.dll game.dll
fi

#TODO: Write separate entry points for diffrent platforms to support it native
clang $includes -g src/main.cpp -o$outputFile $libs $warnings $defines
