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

<details>
<summary>[Windows] How to setup the dependencies in a CMake-friendly way?</summary>

---

Windows doesn't have a set of standard(-ish) paths for development libraries,
so we'll have to improvise! We have 2 options when it comes to setting up the
required dependencies in a way that allows CMake to find them. Both options
requires you to have the dependencies installed somewhere.

### Option 1 (Manual)

If you already have the dependencies installed and want to use those. Use the
CMake GUI to specify the required paths.

- Point the CMake GUI to the Engine's source directory
- Specify the build/cache directory path (normally under `out/build` in the root of the repository, but you're free to choose a different path)
- Run the Configure step
- You'll be presented with "Couldn't find library X" errors; but the cache will be populated with the variables
- Set the required variables (include directories, library paths, etc.) of each of the dependencies to the appropriate paths
- Rerun the Configure step
- Build!

### Option 2 (Automated (Kinda))

Install all of the dependencies under a common prefix and point CMake at said
prefix. It will automatically figure out the rest.

Assuming that the install prefix (where dependencies will be installed) is
under `C:\dev\` (you can put them wherever you like); we will call this path
`DEV_HOME` for the purposes of this guide. We will store the source code under
`<DEV_HOME>/src` and the build cache under `<DEV_HOME>/build`.

The setup is as follows:

- Open a Powershell instance (`Win+R` > type `powershell` > Hit `Enter`) and navigate to `<DEV_HOME>` (`cd <DEV_HOME>`)
- For each dependency (We will use GLFW for demonstration, works the same for the rest of them)
	- Clone the source code.
	```
	git clone https://github.com/glfw/glfw.git src/glfw
	```
	- Generate the build cache (The **-D\<FOO>** options are important! Specify them for all the dependencies).
	```
	cmake -S src/glfw -B build/glfw -DCMAKE_INSTALL_PREFIX=<DEV_HOME> -DCMAKE_DEBUG_POSTFIX="d"
	```
	> The options `-DCMAKE_INSTALL_PREFIX=<DEV_HOME> -DCMAKE_DEBUG_POSTFIX="d"` MUST be specified for every dependency's Configure step!
	- Build the library
	```
	cmake --build build/glfw --config Release
	```
	- Install
	```
	cmake --install build/glfw --config Release
	```
	> If you're planning on building the Engine in Debug mode you will need the Debug version of the libraries as well,		
		so build and install that too (repeat the two commands above replacing `Release` with `Debug`)

	This installs the library's files (headers, libraries, configurations, etc.) in a structure similar to that of Linux,
	so binaries and DLLs are under `bin/`, static libraries under `lib/`, etc. All under `<DEV_HOME>`. What's left is to
	specify the `<DEV_HOME>` path to CMake when building the Engine. We can use the `CMakeUserPresets.json` file for that
	so we don't have to type out the commands ourselves. Keep `CMakeUserPresets.json` away from version control!

- Create a file named `CMakeUserPresets.json` (case-sensitive) in the root of the Engine's repository
- Copy paste the following content into it, replacing `<DEV_HOME>` with the appropriate value (Keep the double-quotes (`"`)):
```
{
	"version": 3,
	"configurePresets": [
		{
			"name": "local-windows-release",
			"displayName": "Windows Release (Local Config)",
			"inherits": "windows-release",
			"cacheVariables": {
				"CMAKE_PREFIX_PATH": "<DEV_HOME>"
			}
		}
	]
}

```
- Head over to the build section!

---
</details>

## Build

First things first, you will have to clone the repository. Open a terminal
instance (Bash, Powershell, whatever) and `cd` to where you want to clone the
repo then execute:

```
git clone --recursive https://gitlab.com/0xIsho/Gaze.git
```
> If you've already cloned the repository without the `--recursive` flag. Run `git submodule init` then `git submodule update`

### Using Visual Studio 2022 (Windows-only)

- Open the Engine's repository in Visual Studio 2022 (Use `Open Folder` *NOT* `Open Project`!)
- Configure the project by selecting `Project > Configure Gaze` and wait for the Configuration step to finish
- Click the Configurations drop-down (found in the toolbar right below the top menu-bar) and select `Windows Release (Local Config)`
- Build the project by selecting `Build > Build All` (or simply press `Ctrl+Shift+B`)
- Run the tests (if you have them enabled; they are by default) by selecting `Test > Run CTests for Gaze` from the menu-bar
- Build a game :)

### The 1337 H4XØR Way

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
