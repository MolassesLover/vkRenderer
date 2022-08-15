# Molasses' Vulkan Renderer
[![Ko-Fi](https://img.shields.io/badge/donate-kofi-blue?style=for-the-badge&logo=ko-fi&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://ko-fi.com/molasses)
[![Patreon](https://img.shields.io/badge/donate-patreon-blue?style=for-the-badge&logo=patreon&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://www.patreon.com/molasseslover)

This is a Vulkan learning project I started, following the [Vulkan Tutorial](https://vulkan-tutorial.com/).
Currently, the project uses code roughly identical to the tutorial. By the
end of an ongoing rewrite, it will look nothing like this. I'm just testing out GLFW.
Many changes are coming, and have not yet been pushed to this repository,
in order to take care of legal information and credits.

It's also important to note that this is 100% a hobby project. It takes shortcuts,
uses other open-source projects as a crutch, and doesn't take itself that seriously.
I plan on making games with this renderer, but this isn't aiming to be the next
[BGFX](https://github.com/bkaradzic/bgfx) or [DiligentEngine](https://github.com/DiligentGraphics/DiligentEngine), by any means.

My end goal is to make a 3D rendering engine that targets 'mixed' lighting, which
is a combination of both statically baked, and realtime lightmaps. It is specifically
designed for newer hardware, and specs into quality moreso than raw performance.
If you plan on using this renderer in a project, keep in mind that this is likely
unsupported on older hardware.

I do not plan on using another graphics API like OpenGL or DirectX, either. 

![screenshot](www/img/screenshot.png)
> Model by [@fletcherkinnear](https://sketchfab.com/fletcherkinnear)

## Dependencies
> **Note**: The list of dependencies might be incomplete, sorry!

| Dependency | [pkgs.org](https://pkgs.org/)                                                                                                                                            | [brew.sh](https://brew.sh/)                                                                                                                                                                              |
|------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| GLFW       | [![pkgs.org](https://img.shields.io/badge/install-pkgs-blue?style=for-the-badge&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://pkgs.org/search/?q=glfw)       | [![pkgs.org](https://img.shields.io/badge/install-brew-blue?style=for-the-badge&logo=homebrew&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://formulae.brew.sh/formula/glfw#default)           |
| Uncrustify | [![pkgs.org](https://img.shields.io/badge/install-pkgs-blue?style=for-the-badge&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://pkgs.org/search/?q=uncrustify) | [![pkgs.org](https://img.shields.io/badge/install-brew-blue?style=for-the-badge&logo=homebrew&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://formulae.brew.sh/formula/uncrustify#default)     |
| Vulkan     | [![pkgs.org](https://img.shields.io/badge/install-pkgs-blue?style=for-the-badge&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://pkgs.org/search/?q=vulkann)    | [![pkgs.org](https://img.shields.io/badge/install-brew-blue?style=for-the-badge&logo=homebrew&color=E35B57&logoColor=FFFFFF&labelColor=232323)](https://formulae.brew.sh/formula/vulkan-headers#default) |

## Building

From the project root directory, you can use CMake to build.

```sh
➜ cmake -B bin -DCMAKE_BUILD_TYPE=Release
➜ cmake --build bin -j$(nproc)
```

> **Note**: If you are unfamiliar with `$(nproc)`; it is a command that prints out
how many processing units are available. With the `-j` flag we tell CMake 
to create jobs equal to the number provided by `$(nproc)`. If you are on XNU/macOS,
 `$(nproc)` is not available. You should use `sysctl -n hw.physicalcpu` instead!

Once CMake finishes, the binaries should be available in the directory
equal to CMake's `-B` flag. In the case of the example commands, that should be
in the `bin` directory. 