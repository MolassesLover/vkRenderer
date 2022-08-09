# Molasses' Vulkan Renderer
[![Ko-Fi](https://img.shields.io/badge/donate-kofi-blue?style=for-the-badge&logo=ko-fi&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://ko-fi.com/molasses)
[![Patreon](https://img.shields.io/badge/donate-patreon-blue?style=for-the-badge&logo=patreon&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://www.patreon.com/molasseslover)

This is a Vulkan learning project I started, following the [Vulkan Tutorial](https://vulkan-tutorial.com/).
The `rewrite` branch (which you are on) uses code roughly identical to the tutorial. By the end of the
rewrite, it will look nothing like this. I'm just testing out GLFW.

## Dependencies
- [GLFW](https://pkgs.org/download/glfw)
- [Uncrustify](https://pkgs.org/search/?q=uncrustify)
- [Vulkan](https://pkgs.org/search/?q=vulkan)

## Building

From the project root directory, you can use CMake to build.

```sh
➜ cmake -B bin -DCMAKE_BUILD_TYPE=Release
➜ cmake --build bin -j$(nproc)
```