 terminal based:
 1. include<ncurse>
 2. compile with ncurses > clang++ Projectdotcpp-o project -lncurses
 3. "initscr()" > is used to start the ncurses mode..everything comes after it.
 4. "noecho()" > whatever we type/click in the terminal ... up down key etc would not be shown on the terminal.
 5. "cbreak()" > it disables line buffering.

gui console based:
 1. include<SFML/Graphicsdothpp>
 2. run to include sfml > brew install sfml
 3  compile with sfml > clang++ Projectdotcpp-o project -lsfml-graphics -lsfml-window -lsfml-system

 Use thiss to compileee > 
 "clang++ -std=c++17 Project.cpp -o project \
-I/opt/homebrew/opt/sfml/include \
-L/opt/homebrew/opt/sfml/lib \
-lsfml-graphics -lsfml-window -lsfml-system -lncurses"


I used SFML, which is a modern C++ graphics and multimedia library. It provides a simpler API for rendering shapes, text, and real-time graphics, and internally it’s built on top of OpenGL. So it’s still consistent with the requirement to use a graphics library.