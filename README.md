# cellular++

This will be a cellular automata library in C++, right now it's just a project skeleton.

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

Then look inside `build/src`.
