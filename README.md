Dungeon Battle
[![Build Status](https://travis-ci.org/remi-dupre/dungeon-battle.svg?branch=master)](https://travis-ci.org/remi-dupre/dungeon-battle) 🦄
==============

Software engineering project.

# Compiling

The project is written in **C++17**, in order to compile it you need 

- A C++ STL that supports at least C++11.
- The **SFML** library version `2.4.2` or more recent.

The project can be built on most *Linux* distributions and *macOS*. Building on *Windows* is not supported, though building in *cygwin* or *bash on Windows* should work fine.

## Dependencies

You can install **SFML** on *Debian* distributions with the command :

```bash
apt-get install libsfml-dev
```

The supported compilers are **g++** 7 and **clang++** 5, or any later version.

On *Ubuntu* 17.10, **g++** 7 can be installed with the command :

```bash
apt-get install g++-7
```

**LLVM** binaries for *Debian* and *Ubuntu* which include **clang++** can also be found on [their website](http://releases.llvm.org/download.html).

You should also install **GNU Make** :

```bash
apt-get install make
```

## Compilation

In order to compile the project, assuming you have all the dependencies mentioned above, you have to run the command :

```bash
make release
```

# Documentation

An automatically generated documentation can be found on [github pages](https://remi-dupre.github.io/dungeon-battle/index.html).

You can also generate the documentation by running `make doc`, which requires doxygen.

# Other informations

We defined a preferred coding style in the file [coding_style.md](https://github.com/remi-dupre/dungeon-battle/blob/master/coding_style.md).


Made by Jean-Baptiste Daval, Rémi Dupré, Benjamin Graillot, Lucas Gréaux and Garance Gourdel.
