# l2mapconv

<p align="center">
    <img src="assets/cruma.png" width="400">
    <img src="assets/toi.png" width="400">
</p>

Lineage II geodata builder.

- Supported clients: C1, HF.
- Supported geodata formats: L2J.

## Features

- Map and geodata preview.
- L2J geodata building.

## Usage

```sh
l2mapconv.exe --preview/build --client-root <path> -- [maps...]

    --preview          Preview maps
    --build            Build maps (see results in the `output` directory)
    --client-root arg  Path to the Lineage II client
    --log-level arg    Log level (0 - none, 1 - fatal, 2 - error, 3 -
                       warn, 4 - info, 5 - debug, 6 - all) (default: 3)
    --help             Print help
```

## Project building

Requirements:

- Clang
- Cmake
- Ninja
- MVS on Windows and Xcode on macOS (or standalone Clang)

### Windows

```sh
> git clone --recurse-submodules -j8 https://github.com/madyanov/l2mapconv-public.git
> cd l2mapconv-public
> cmake -H. -G Ninja -Bbuild -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang.exe"
> cd build
> ninja
```

### macOS

```sh
$ git clone --recurse-submodules -j8 https://github.com/madyanov/l2mapconv-public.git
$ cd l2mapconv-public
$ cmake -H. -G Ninja -Bbuild -DCMAKE_BUILD_TYPE=Release
$ cd build
$ ninja
```

## Credits

- [Recast](https://github.com/recastnavigation/recastnavigation)
- [L2PackageTools](https://github.com/Bigcheese/L2PackageTools)
- [l2mapper](https://github.com/justgos/l2mapper)
- [UEViewer](https://github.com/gildor2/UEViewer)
- [UT Package Tool](https://www.acordero.org/projects/unreal-tournament-package-tool)
- UT 2003
