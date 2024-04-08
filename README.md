# Gaze Engine

This repository hosts the sources of the *Gaze Interactive Experiences Engine*.

## Dependencies

- A C++20-capable compiler (e.g. GCC/Clang/MSVC)
- [CMake](https://cmake.org) (>=3.19)
- [Catch2](https://github.com/catchorg/Catch2) (v3) *(Required only if testing is enabled)*
- [GLFW](https://glfw.org)
- [GLM](https://github.com/g-truc/glm)
- [Bullet](https://github.com/bulletphysics/bullet3)
- [Assimp](https://www.assimp.org)
- [spdlog](https://github.com/gabime/spdlog)

## Build

Clone the repository:
```sh
git clone --recursive https://gitlab.com/0xIsho/Gaze.git && cd Gaze
```
> If you've already cloned the repository without the `--recursive` flag. Run `git submodule init` then `git submodule update`

Configure:
```sh
cmake --preset <preset>
```
> For example:
>
> ```sh
> cmake --preset linux-debug
> ```

> Available `<preset>`s can be listed by issuing `cmake --list-presets` on the command-line.

Build:
```sh
cmake --build <build-path>
```

> `<build-path>` is printed out after the configure step is done.
>
> For example:
>
> ```sh
> cmake --build out/build/debug
> ```
>

Test:
```sh
ctest --test-dir <build-path>
```

> If tests fail in a clean checkout (non-modified copy) of the repository, please file a bug report!

## Supported Platforms

| Platform | Support |
| -------- | ------- |
| Windows  | ✔️      |
| Linux    | ✔️      |
| macOS    | ❌      |
| Android  | ❌      |
| iOS      | ❌      |
