# conslr - A Widget Based Terminal Emulator
This is a pet project of mine initially created as an abstraction layer to a game I am working on.

It is meant to be relatively extendable and easily modifiable.

![Example of conslr being used](https://github.com/syd-p/conslr/blob/master/res/widgetsexample.png)

## Installation
As there are no precompiled binaries (yet), you have to build it from source

To do so do the following
1. Pull the repo:
```
git clone https://github.com/syd-p/conslr.git
cd conslr
```
2. Create a directory to build in:
```
mkdir Release
cd Release
```
or
```
mkdir Debug
cd Debug
```
3. Create the make files from CMake:
```
cmake -DCMAKE_BUILD_TYPE=[Release/Debug] ..
```
4. Build:
```
cmake --build .
```
This should create two folders, lib contains the build library and bin contains some example programs such as:
```
./bin/widgets
```
