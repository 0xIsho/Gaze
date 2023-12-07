# Gaze Engine

This repository hosts the sources of the *Gaze Interactive Experiences Engine*.

## Dependencies

- A C++20-capable compiler (e.g. GCC/Clang/MSVC)
- [CMake](https://cmake.org) (>=3.19)
- [Catch2](https://github.com/catchorg/Catch2/tree/v2.x) (v2.x) *(Required only if testing is enabled)*
- [GLFW](https://glfw.org)
- [GLM](https://github.com/g-truc/glm)

## Build

Clone the repository:
```sh
git clone https://gitlab.com/0xIsho/Gaze.git && cd Gaze
```

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
| Windows  | ❌      |
| Linux    | ✔️      |
| macOS    | ❌      |
| Android  | ❌      |
| iOS      | ❌      |
