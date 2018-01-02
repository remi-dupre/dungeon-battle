Dungeon Battle
[![Build Status](https://travis-ci.org/remi-dupre/dungeon-battle.svg?branch=master)](https://travis-ci.org/remi-dupre/dungeon-battle) 🦄 🎉
==============

Software engineering project.

# Compiling

The project is written in **C++14**, in order to compile it you need:

- A C++ STL that supports at least C++11.
- The **SFML** library version `2.3.2` or more recent.

The project can be built on most *Linux* distributions and *macOS*. Building on *Windows* is not supported, though building in *cygwin* or *bash on Windows* should work fine.

## Dependencies

You can install **SFML** on *Debian* distributions with the command:

```bash
apt-get install libsfml-dev
```

The supported compilers are **g++** 5 and **clang++** 4, or any later version.

On *Ubuntu* 16.04, **g++** 5 can be installed with the command:

```bash
apt-get install g++
```

**LLVM** binaries for *Debian* and *Ubuntu* which include **clang++** can also be found on [their website](http://releases.llvm.org/download.html).

You should also install **GNU Make**:

```bash
apt-get install make
```

### Optional dependencies
Documentation is generated with **doxygen** and is required to `make doc`:
```bash
apt-get install doxygen
```

Some extra tests can be done by running `make test` and it requires **cppcheck** and **cxxgen**:
```bash
apt-get install cppcheck cxxgen
```

To execute the tests use `make test`:
```bash
make test     # Build tester and runs it
tests/test -v # Run the test again with extra informations
```

If you want to build packages using `make package` you will need **gzip**, **dpkg** and **fakeroot**:
```bash
apt-get install gzip dpkg fakeroot
```

## Compilation

In order to compile the project, assuming you have all the dependencies mentioned above, you have to run the command:

```bash
make release
```

If the flag DPACKAGE is set, the program will be compiled to find its ressources in /usr/var/dungeon-battle

## Publication

To publish the release, you need to add a tag on current commit:
```bash
git tag -a v1.4 -m "short title for version 1.4"
git push --tag
```

# Documentation

An automatically generated documentation can be found on [github pages](https://remi-dupre.github.io/dungeon-battle/index.html).

You can also generate the documentation by running `make doc`, which requires doxygen.

# Other informations

We defined a preferred coding style in the file [CONTRIBUTING.md](https://github.com/remi-dupre/dungeon-battle/blob/master/CONTRIBUTING.md).

Some warning from the linter are listed on [github pages](https://remi-dupre.github.io/dungeon-battle/check/index.html).


Made by Jean-Baptiste Daval, Rémi Dupré, Garance Gourdel, Benjamin Graillot and Lucas Gréaux.
