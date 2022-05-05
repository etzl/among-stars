# Among Stars 👾

Inspired by old-school arcade games and my room's persian carpet

<img src="docs/carpet.jpg" width=75% height=75% alt="carpet">

If you look closely you can find the enemies.

## How to play

You can move around and submit/shoot with `<ARROW_KEYS>` and `<ENTER>` respectively.

There's also a **Help** button in the menu which explains the game.

### Command Line Arguments

`--no-menu` Go directly to the game (without showing menu)  
`--no-damage`   Die hard - sets the player health to [maximum possible value](https://en.cppreference.com/w/cpp/types/numeric_limits/max)  
`--help`    Show help and quit

### Score & Health

Player's stats is shown on the top-right corner:

![stats picture](/docs/stats.png)

Short for 'Health Point' and 'Kill Point'

## Compiling

There's **no need to install** the game. It can be compiled and run in the same directory.

After installing the dependencies, run:

```bash
make
make run or ./among_stars
```

### Dependencies

- GNU compiler that supports *C++17* or higher (version 9 or later)
- development ncurses library
- pthread library

Packages on Debian (bullseye): `libncurses-dev` and `libncurses6`  
This is probably the same on recent Debian distros and Debian derivatives (like Ubuntu 20.04 or higher).

### On windows

You have two options:

  - Using environments like [Cygwin](https://cygwin.com/) or [WSL](https://docs.microsoft.com/en-us/windows/wsl/install)
  - Build natively using [MSYS2](https://www.msys2.org/)

As for the first ones they "emulate" linux environment and "magically" make the program's functionality available under windows, the build instruction should be the same as for linux. But the second option *is* using ported tools such as `mingw-w64` under windows, so you'll be able to build the program natively.

See https://github.com/etzl/among-stars/wiki/Build#on-windows for more details

## Documentation

You can look through [wikis](https://github.com/etzl/among-stars/wiki) for technical details, and more detailed description, to help you understand the code.

## Contributing

If you want to contribute see [how to contribute](/CONTRIBUTING.md)

Don't get confused with all branches. They're just how the project *could* continue.
