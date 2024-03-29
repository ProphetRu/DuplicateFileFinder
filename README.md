# Duplicate File Finder
Educational project with [googletest](https://github.com/google/googletest), [doxygen](https://github.com/doxygen/doxygen) and [boost](https://github.com/boostorg)

## Build local Linux
```shell
sudo apt-get update && sudo apt-get install cmake libgtest-dev libboost-all-dev -y

cd DuplicateFileFinder
mkdir build && cd build

cmake ..

# build release
cmake --build . --config Release

# build deb-package
cmake --build . --target package
```

## Build local Windows
```shell
vcpkg install gtest boost-program-options boost-filesystem boost-uuid boost-crc
vcpkg integrate install

cd DuplicateFileFinder
mkdir build && cd build

cmake .. -DCMAKE_TOOLCHAIN_FILE="path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"

# build release
cmake --build . --config Release
```

## Arguments
The program searches for duplicate files in specified directories.
The program takes the following arguments:
* `--help` - Produce help message.
* `--directories or -D` - Directories to scan.
* `--exclude or -E` - Directories to exclude from finding.
* `--deep-scan` - `1` to including subdirectories, `0` to search only in the specified directory.
* `--min-file-size or -F` - Minimum file size in bytes that will included when searching.
* `--file-mask or -M` - File search mask, supports regular expressions.
* `--block-size or -S` - Block size for reading files.
* `--hashing-type or -H` - Hashing type `1` to md5, `0` to crc32.

Example:
```shell
DuplicateFileFinder -D /some/path -E exclude_dir --deep-scan 1 -F 4 -S 6 -H 1
```
