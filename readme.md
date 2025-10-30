## Digital Signal Generator
A terminal and GUI-based application to simulate digital line coding schemes such as NRZ-L, NRZ-I, Manchester, Differential Manchester, AMI, and scrambling techniques like B8ZS and HDB3.
It also supports PCM and Delta Modulation visualization.

## Tech Stack
### Language: 
        C++
### Libraries:
        SFML (for GUI), ncurses (for terminal interface)

## Install dependencies
### For macOS:
 brew install sfml ncurses
### For windows: 
 sudo apt update
 sudo apt install libsfml-dev libncurses5-dev libncursesw5-dev   
  
  Native Windows: Download SFML from: https://www.sfml-dev.org/download.php
 (or better check documentation)

## Compilation: 
### macOS:
 clang++ -std=c++17 Project.cpp -o project \
-I/opt/homebrew/opt/sfml/include \
-L/opt/homebrew/opt/sfml/lib \
-lsfml-graphics -lsfml-window -lsfml-system -lncurses 

### windows(using MinGW):
g++ -std=c++17 Project.cpp -o project.exe \
-I C:\SFML\include -L C:\SFML\lib \
-lsfml-graphics -lsfml-window -lsfml-system -lpdcurses


## Terminal based:

 1. include<ncurse>
 2. compile with ncurses > clang++ Projectdotcpp-o project -lncurses
 3. "initscr()" > is used to start the ncurses mode..everything comes after it.
 4. "noecho()" > whatever we type/click in the terminal ... up down key etc would not be shown on the terminal.
 5. "cbreak()" > it disables line buffering.

## Gui console based:

 1. include<SFML/Graphicsdothpp>
 2. run to include sfml > brew install sfml
 3  compile with sfml > clang++ Projectdotcpp-o project -lsfml-graphics -lsfml-window -lsfml-system


I used SFML, which is a modern C++ graphics and multimedia library. It provides a simpler API for rendering shapes, text, and real-time graphics, and internally it’s built on top of OpenGL. So it’s still consistent with the requirement to use a graphics library.
