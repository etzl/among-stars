# Among Stars - Inspired by old-school arcade games and my room's persian carpet

<img src="docs/carpet.jpg" width=75% height=75% alt="carpet">

If you look closely you can find the enemies.

## How to play

You can move around and submit/shoot with `<ARROW_KEYS>` and `<ENTER>` respectively.

There's also a **Help** button in the menu which explains the game.

### Command Line Arguments

`--no-menu` Go directly to the game (without showing menu)  
`--no-damage`   Die hard - sets the player health to [maximum possible value](https://en.cppreference.com/w/cpp/types/numeric_limits/max)

### Score & Health

Player's stats is shown on the top-right corner:

```txt
HP: x
KP: x
```

Short for 'Health Point' and 'Kill Point'

## Compiling

The game won't touch any files on the system (**no need to install**). It can easily be compiled and run in the directory.

Just install the dependencies and then run:

```bash
make
make run or ./among_stars
```

### Dependencies

- A compiler that supports *C++17* or higher
- ncurses library and header files

Packages on Debian (bullseye): `libncurses-dev` and `libncurses6`  
This is probably the same on recent Debian distros and Debian derivatives (like Ubuntu 20.04 or higher).

## Documentation

You can look through [wikis](https://github.com/etzl/among-stars/wiki) for technical details, and more detailed description, which is a work in progress.

## Contributing

If you want to contribute see [how to contribute](/CONTRIBUTING.md)

Don't get confused with all branches. They're just how the project *could* continue.
