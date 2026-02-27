#!/bin/bash

# Define paths
SPITE_DIR="$HOME/.spite"
BIN_DIR="$SPITE_DIR/bin"
TARGET_EXE="$BIN_DIR/spite"
# Raw URL for the source code
SOURCE_URL="https://raw.githubusercontent.com/PandaTwoxx/Spite/main/spite.cpp"

echo "--- SpiteLang Cloud Installer: The C# Killer ---"

# 1. Check for C++ Compiler
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ not found. Please install a C++ compiler to continue."
    exit 1
fi

# 2. Create directories
mkdir -p "$BIN_DIR"

# 3. Download the latest source from GitHub
echo "Fetching latest SpiteLang source from GitHub..."
curl -s -L "$SOURCE_URL" -o "$SPITE_DIR/spite.cpp"

if [ ! -f "$SPITE_DIR/spite.cpp" ]; then
    echo "Error: Failed to download source code from GitHub."
    exit 1
fi

# 4. Compile and Install
echo "Compiling SpiteLang..."
g++ -O3 "$SPITE_DIR/spite.cpp" -o "$TARGET_EXE"

if [ $? -eq 0 ]; then
    echo "Successfully installed/updated SpiteLang at $TARGET_EXE"
else
    echo "Compilation failed."
    exit 1
fi

# 5. Add to PATH logic
PATH_LINE="export PATH=\"\$HOME/.spite/bin:\$PATH\""

if [[ "$SHELL" == */zsh ]]; then
    PROFILE="$HOME/.zshrc"
elif [[ "$SHELL" == */bash ]]; then
    PROFILE="$HOME/.bashrc"
    [ -f "$HOME/.bash_profile" ] && PROFILE="$HOME/.bash_profile"
else
    PROFILE="$HOME/.profile"
fi

if ! grep -q ".spite/bin" "$PROFILE"; then
    echo "Adding SpiteLang to PATH in $PROFILE..."
    echo -e "\n# SpiteLang Path\n$PATH_LINE" >> "$PROFILE"
    echo "Done! Please run 'source $PROFILE' or restart your terminal."
else
    echo "SpiteLang is already in your PATH."
fi

echo "--- Installation Complete ---"