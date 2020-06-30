# cellular++

This will be a cellular automata library in C++, right now it's a work in progress.

## Getting & building

### Requirements:
 * A C++ compiler that supports C++2a
 * Meson
 * Ninja
 * Catch2 (optional, can use meson wrap if not installed on system)

```sh
git clone https://github.com/omrisim210/cellularpp.git
cd cellularpp
meson build
meson compile -C build
meson wrap install catch2
meson test -C build
```

Then look inside `build/bin`.

### Hacking
The library is header-only and located in `include`, while the binary is in `bin`.

## TODO

- [x] Implement automata in their most basic, buggy and unpolished form.
- [ ] Fix stepping (look in `include/cellular.hpp` for bug descriptions).
- [ ] Fix weird extra-cell bug (see previous item).
- [ ] Fix testing (the test just doesn't seem to work).
- [ ] Implement meaningful tests.
- [ ] Implement more automata like a powder toy type thing, WireWorld and some roguelike cave-generation algorithms.
- [ ] Add tutorial/docs/usage instructions, possibly using some automated tool (might not be needed since the librar- y core is so small). For now use `bin/main.cpp` as a reference.
- [ ] Add an actual project binary with pluggable automata, possibly using multiple executables in a certain directory (like how Rust's `cargo` tool handles pluggable subcommands)
