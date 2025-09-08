# AdriCHIP-8
A simple CHIP-8 emulator made by Adrian Gjonca.
Written using C and SDL2.


<img width="602" height="464" alt="image" src="https://github.com/user-attachments/assets/74c0b4cf-f306-40a8-b16c-b79ec0615480" />

_AdriChip8 running Brick (Brix hack, 1990)_

## Building
Simply run make on the project root directory
```
make
```
This will generate an executable titled 'adrichip8'

## How to use
Run the executable with the CHIP-8 rom as its sole argument 
### \*nix like
```
./adrichip8 [path to rom]
```
### Windows/ReactOS
```
adrichip8 [path to rom]
```

## Compatability
This project is in its infancy. while it cleanly passes all tests (Chip8 only) from https://github.com/Timendus/chip8-test-suite, certain games still have noticable bugs.
Overtime this will improve but this is not a high-priority project for me and hence bugs will not be fixed very quickly.
However, if you find any bugs please post an issue with information, screen shots and the ROM file and I will attempt to solve them within my abillity.
