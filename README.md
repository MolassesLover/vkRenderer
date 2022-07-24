# Molasses' Vulkan Renderer
[![Ko-Fi](https://img.shields.io/badge/donate-kofi-blue?style=for-the-badge&logo=ko-fi&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://ko-fi.com/molasses)
[![Patreon](https://img.shields.io/badge/donate-patreon-blue?style=for-the-badge&logo=patreon&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://www.patreon.com/molasseslover)

This is a Vulkan learning project I started, following the [Vulkan Tutorial](https://vulkan-tutorial.com/).

## Dependencies
- [SDL](https://pkgs.org/search/?q=SDL2)
- [Uncrustify](https://pkgs.org/search/?q=uncrustify)
- [Vulkan](https://pkgs.org/search/?q=vulkan)

## Building

From the project root directory, you can use CMake to build.

```sh
➜ cmake -B bin -DCMAKE_BUILD_TYPE=Release
➜ cmake --build bin -j$(nproc)
```
