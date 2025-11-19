# Raylib CMake Template

This is a minimal template for a C++ raylib project that can be built for desktop and web.

## Prerequisites

You need

- `CMake`
- A build tool (e.g. `make`)
- A compiler (e.g. `gcc`, or `emcc`/[Emscripten](https://emscripten.org/docs/getting_started/downloads.html) if you want to build for web)

## Building

There are four CMake presets available: `release`, `debug`, `web-release`, and `web-debug`.
Pick one and replace `<name>` by it in the instructions below:

```bash
cmake --preset <name>
cmake --build --preset <name>
```

For the desktop presets (`release` and `debug`) this should produce an executable.
For example, on linux, you should be able to run

```bash
./build/release/demo/demo
```

For the web presets (`web-release` and `debug`), the produced `.wasm` is not directly executable.
Instead, there should be an `index.html` in the build directory that you can serve through an HTTP server.
For example, on linux with `emrun`:

```bash
emrun build/web-release/demo/index.html
```