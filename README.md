# cellular++

This will be a cellular automata library in C++, right now it's a work in progress.

## Getting & building

### Requirements:
 * A C++ compiler that supports C++2a (although C++17 might also work, YMMV)
 * Meson
 * Ninja
 * Catch2 (optional, can use meson wrap if not installed on system)
 * SDL2 (optional, for the upcoming graphical frontend for the binary)

### Compiling natively on *nix:

```
$ git clone https://github.com/omrisim210/cellularpp.git
$ cd cellularpp
$ meson build
$ meson compile -C build
$ meson wrap install catch2
$ meson test -C build
```

Then look inside `build/bin`.

### Cross compiling for Windows with Docker:

```
$ mkdir docker
$ sudo docker build -t meson-mingw-build-env -f Dockerfile ./docker
```

Then, if running for the first time:

```
$ sudo docker run --name=mingw -it -v $PWD:/root/cellular++ meson-mingw-build-env
```

Else:

```
$ sudo docker start --attach --interactive mingw
```

Then, in the Docker shell:

```
[root@xxxxxxxxxxxx cellular++]# meson build-mingw --cross-file mingw-cross.txt
```

Then, from the container or from outside of it, look inside `build-mingw/bin`.

### Hacking
The library is header-only and located in `include`, while the binary is in `bin`.
Coding style isn't to my liking yet, I think it's some default of `clang-format`.
I'll change it later, for now just try to stick to the project's style.


## TODO:

- [x] Implement automata in their most basic, buggy and unpolished form.
- [ ] Fix stepping (look in `include/cellular.hpp` for bug descriptions).
- [ ] Fix weird extra-cell bug (see previous item).
- [ ] Fix testing (the test just doesn't seem to work).
- [ ] Implement meaningful tests.
- [ ] Implement more automata like a powder toy type thing, WireWorld and some roguelike cave-generation algorithms.
- [ ] Add tutorial/docs/usage instructions, possibly using some automated tool (might not be needed since the librar- y core is so small). For now use `bin/main.cpp` as a reference.
- [ ] Add an actual project binary with pluggable automata, possibly using multiple executables in a certain directory (like how Rust's `cargo` tool handles pluggable subcommands)
- [ ] Automate building with docker
- [ ] Automate testing with docker. We might need to use Wine for that but I use WSL so it shouldn't run on my sysem.
- [ ] Maybe find a way to run the tests on Windows when using WSL and on Wine otherwise? Is that possible?
- [ ] Setup a `.dockerignore` file. I think it's used to not include files we don't need from the base image?
- [ ] Implement generic graphical frontend for automata
- [ ] Maybe integrate Lua for writing automata?
