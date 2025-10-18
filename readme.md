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


"#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 400), "Line Encoding Visualization");

    std::string binary = "1011001";
    float x = 50;
    float y_mid = 200;
    float bit_width = 80;
    float level_high = 100;
    float level_low = 300;

    std::vector<sf::Vertex> lines;

    // NRZ-L Encoding Example
    float prev_y = (binary[0] == '1') ? level_high : level_low;
    lines.push_back(sf::Vertex(sf::Vector2f(x, prev_y)));

    for (char bit : binary) {
        float current_y = (bit == '1') ? level_high : level_low;
        lines.push_back(sf::Vertex(sf::Vector2f(x, current_y)));
        x += bit_width;
        lines.push_back(sf::Vertex(sf::Vector2f(x, current_y)));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        window.draw(&lines[0], lines.size(), sf::LinesStrip);
        window.display();
    }

    return 0;
}
"


I used SFML, which is a modern C++ graphics and multimedia library. It provides a simpler API for rendering shapes, text, and real-time graphics, and internally it’s built on top of OpenGL. So it’s still consistent with the requirement to use a graphics library.