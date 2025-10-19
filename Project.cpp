#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Graphics/Font.hpp>
#include<SFML/Graphics/Text.hpp>
#include<iostream>
#include<string>
#include<ncurses.h>
using namespace std;
using namespace sf;

void windowLogic(string& binary, vector<Vertex>& lines, const string& title ){
    RenderWindow window(VideoMode({800, 400}), title);

    float x = 50;
    float y_mid = 200;
    float bit_width = 60;
    float amplitude = 50; // distance from middle line
    float level_high = y_mid - amplitude; // up
    float level_low  = y_mid + amplitude; // down

     //adding button
    RectangleShape exitButton(Vector2f(100.f, 35.f));
    exitButton.setPosition({650.f, 350.f});
    exitButton.setFillColor(Color::White);

    Font font("/System/Library/Fonts/Supplemental/Arial.ttf"); // full path to system font
    Text exitText(font, "X exit", 15); // font, string, character size
    exitText.setFillColor(Color::Black);
    Vector2f buttonPos = exitButton.getPosition(); // get button position
    exitText.setPosition({buttonPos.x + 20.f, buttonPos.y + 5.f});

    //window design(intervals, lines, axis, event handling etc.)
    while (window.isOpen()) {
        // Event handling
        while (auto eventOpt = window.pollEvent()) {
            const auto& event = *eventOpt;
            //yeh uper diye gye x sai close hoga
            if (event.is<sf::Event::Closed>()) {
                window.close();
            }
            //yeh button se close hoga
            if (event.is<sf::Event::MouseButtonPressed>()) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                if (exitButton.getGlobalBounds().contains(mousePosF)) {
                    window.close();
                    exit(0); 
                }
            }
        }

        // Clearing previous frame
        window.clear(sf::Color::White);

        //drawing button
        window.draw(exitButton);
        window.draw(exitText);

        // drawing middle horizontal axis
        Vertex axis[] = {
            Vertex({Vector2f(0, y_mid), Color::Blue}),
            Vertex({Vector2f(800, y_mid), Color::Blue})
        };
        window.draw(axis, 4, sf::PrimitiveType::Lines);

        // drawing intervals as dotted lines
        for (int i = 0; i <= binary.size(); i++) {
            float x_pos = 50 + i * bit_width;
            float y_start = y_mid - amplitude - 20;
            float y_end   = y_mid + amplitude + 20;
            float gap = 5;  // length of each dash
            for (float y = y_start; y < y_end; y += 2 * gap) {
                Vertex dash[] = {
                    Vertex({Vector2f(x_pos, y), Color::Red}),
                    Vertex({Vector2f(x_pos, std::min(y + gap, y_end)), Color::Red})
                };
                window.draw(dash, 2, sf::PrimitiveType::Lines);
            }
        }
        // drawing waveform
        if (!lines.empty())
            window.draw(&lines[0], lines.size(), sf::PrimitiveType::LineStrip);

        // displaying frame
        window.display();
    }
}

//NRZ-L
void NRZL(string binary){
    cout<<"\nRunning NRZL"<<endl;
    float x = 50;
    float y_mid = 200;
    float bit_width = 60;
    float amplitude = 50; // distance from middle line
    float level_high = y_mid - amplitude; // up
    float level_low  = y_mid + amplitude; // down


    vector<Vertex> lines;

    float prev_y = y_mid;
    lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));


    for (char bit : binary) {
        float current_y = (bit == '1') ? level_high : level_low; //level logic
        lines.push_back(Vertex({Vector2f(x, current_y), Color::Black}));
        x += bit_width;
        lines.push_back(Vertex({Vector2f(x, current_y), Color::Black}));
    }

    windowLogic(binary, lines, "NRZ-L Visualization");
    
}

//NRZ-I
void NRZI(string binary){
        cout<<"\nRunning NRZI"<<endl;
        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        //tells the starting either 1 is +ve or -ve(i.e., high level or low level)
        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        for(char bit : binary){
            if(bit == '1'){
                prev_y = (prev_y == level_high) ? level_low : level_high;
            }
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }
        
        windowLogic(binary, lines, "NRZ-I Visualization");

}

//B8ZS
void B8ZS(string binary){
    cout<<"\nRunning B8ZS"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));

        //ADD LOGIC HERE
       string pattern = "00000000";
       string replacement = "000VB0VB";

       size_t pos = 0;
       while((pos = binary.find(pattern,pos)) != string::npos){
            binary.replace(pos, pattern.length(), replacement);
            pos += replacement.length(); 
       }
       float last_polarity = level_high;
        for(char bit : binary){
            if(bit == '1'){
                last_polarity = (last_polarity == level_high) ? level_low : level_high;
                prev_y = last_polarity;
            }
            else if(bit == '0'){
                prev_y = y_mid;
            }
            else if(bit == 'V'){
                last_polarity = (last_polarity == level_high) ? level_high : level_low;
                prev_y = last_polarity;
            }
            else if( bit == 'B'){
                last_polarity = (last_polarity == level_high) ? level_low : level_high;
                prev_y = last_polarity;
            }

            
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }
        

        windowLogic(binary, lines, "B8ZS Scrambled Visualization");

}

//HDB3
void HDB3(string binary){
    cout<<"\nRunning HDB3"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));

        //ADD LOGIC HERE
        string pattern = "0000";
        int prev_pulse = 0;
        
            size_t pos = 0;
            while((pos = binary.find(pattern,pos)) != string::npos){
                string replacement = (prev_pulse%2 == 0)? "B00V": "000V";
                    binary.replace(pos, pattern.length(), replacement);
                if (replacement[0] == 'B' || replacement[3] == 'V') {
            prev_pulse += 2; 
        }
            }
            
        x = 50;
        prev_pulse = 0;
        float last_polarity = level_low;
        for(char bit : binary){
            if(bit == '1'){
                last_polarity = (last_polarity == level_high) ? level_low : level_high;
                prev_y = last_polarity;
                prev_pulse++;
            }
            else if(bit == '0'){
                prev_y = y_mid;
            }
            else if(bit == 'V'){
                prev_y = last_polarity;
                prev_pulse++;

            }
            else if( bit == 'B'){
                last_polarity = (last_polarity == level_high) ? level_low : level_high;
                prev_y = last_polarity;
                prev_pulse++;
            }

            
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }
        

        windowLogic(binary, lines, "HDB3 Scrambled Visualization");

}

//AMI
void AMI(string binary){
        int n;
        cout << "Is Scrambled needed?(Enter 1/0 for YES/No)"<<endl;
        cin>>n;
        if( n == 1){
            initscr();
            noecho();
            cbreak();

            int choice2 = 1;
            int ch2;
            while(true){
                mvprintw(0,0, "Select Scrambling Technique:");
                mvprintw(2,0, "[%c] B8ZS ", (choice2 == 1)? 'x': ' ');
                mvprintw(3,0, "[%c] HDB3 ", (choice2 == 2)? 'x': ' ');
                mvprintw(5,0, "Use 'w' to move UP and 's' to move DOWN");

                ch2 = getch();
                if(ch2 == 'w'){
                    if(choice2 > 1) choice2--;
                }
                else if(ch2 == 's'){
                    if(choice2 < 2) choice2++;
                }
                else if (ch2 == '\n'){
                    break;
                }
            }
            endwin();

            if(choice2 == 1){
                B8ZS(binary);
            }
            else if( choice2 == 2){
                HDB3(binary);
            }
            
        }
        else if(n == 0){
            cout<<"\nRunning AMI"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        //ADD LOGIC HERE ami
        float last_polarity = level_low;
        for(char bit : binary){
            if(bit == '1'){
                last_polarity = (last_polarity == level_high) ? level_low : level_high;
                prev_y = last_polarity;
            }
            else if(bit == '0'){
                prev_y = y_mid;
            }


            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }

        windowLogic(binary, lines, "AMI Visualization");
        
        }
        else{
            cout<< "Invalid Input"<<endl;
        }

}

//MANCHESTER
void Manchester(string binary){
        cout<<"\nRunning Manchester"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));

            //ADD LOGIC HERE
        for(char bit : binary){
            float x_mid = x + bit_width/2;
            float x_end = x + bit_width;
            
            float first_half_y, second_half_y;

            if(bit == '1'){
                first_half_y = level_high;
                second_half_y = level_low;
            }
            else if(bit == '0'){
                first_half_y = level_low;
                second_half_y = level_high;
            }

            lines.push_back(Vertex({Vector2f(x, first_half_y), Color::Black}));
            lines.push_back(Vertex({Vector2f(x_mid, first_half_y), Color::Black}));

            lines.push_back(Vertex({Vector2f(x_mid, first_half_y), Color::Black}));
            lines.push_back(Vertex({Vector2f(x_mid, second_half_y), Color::Black}));

            lines.push_back(Vertex({Vector2f(x_mid, second_half_y), Color::Black}));
            lines.push_back(Vertex({Vector2f(x_end, second_half_y), Color::Black}));

            prev_y = second_half_y;
            x = x_end;
        }
        

        windowLogic(binary, lines, "Manchester Visualization");


}

//DMANCHESTER
void DManchester(string binary){
        cout<<"\nRunning DManchester"<<endl;
        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        //ADD LOGIC HERE
        for(char bit : binary){

            float x_mid = x + bit_width/2;
            float x_end = x + bit_width;

            float first_half_y, second_half_y;

            if(bit == '1'){
                first_half_y = prev_y;
            }
            else if( bit =='0'){
                first_half_y = (prev_y == level_high)?level_low: level_high;
            }
             second_half_y = (first_half_y == level_low)? level_high:level_low;

             lines.push_back(Vertex({Vector2f(x, first_half_y), Color::Black}));
            lines.push_back(Vertex({Vector2f(x_mid, first_half_y), Color::Black}));

            lines.push_back(Vertex({Vector2f(x_mid, first_half_y), Color::Black}));
            lines.push_back(Vertex({Vector2f(x_mid, second_half_y), Color::Black}));

            lines.push_back(Vertex({Vector2f(x_mid, second_half_y), Color::Black}));
            lines.push_back(Vertex({Vector2f(x_end, second_half_y), Color::Black}));

            prev_y = second_half_y;
            x = x_end;
            
        }
        windowLogic(binary, lines, "Differential Manchester Visualization");


}

//PCM
void PCM(string binary){
        cout<<"\nRunning PCM"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        //ADD LOGIC HERE
        for(char bit : binary){

            
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }

        windowLogic(binary, lines, "PCM Visualization");


}

//DM
void DM(string binary){
        printf("\nRunning DM");

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; // distance from middle line
        float level_high = y_mid - amplitude; // up
        float level_low  = y_mid + amplitude; // down

        vector<Vertex> lines;
        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        //ADD LOGIC HERE
        for(char bit : binary){
            

            
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }
        windowLogic(binary, lines, "DM Visualization");

}

void digitalEncodingMenu(string binary){
        cout<<"\nApplying line encoding."<<endl;

        initscr();
        noecho();
        cbreak();

        int choice1 = 1;
        int ch1;
        
        while(true){
            clear();
            mvprintw(0, 0, "Select Line Encoding Technique:");
            mvprintw(2, 0, "[%c] NRZ-L", (choice1 == 1 ? 'x' : ' '));
            mvprintw(3, 0, "[%c] NRZ-I", (choice1 == 2 ? 'x' : ' '));
            mvprintw(4, 0, "[%c] AMI", (choice1 == 3 ? 'x' : ' '));
            mvprintw(5, 0, "[%c] Manchester", (choice1 == 4 ? 'x' : ' '));
            mvprintw(6, 0, "[%c] Differential Manchester", (choice1 == 5 ? 'x' : ' '));
            mvprintw(8, 0, "Use 'w' to move UP and 's' to move DOWN, Enter to select.");

            ch1 = getch();
            if(ch1 == 'w'){
                if(choice1 > 1) choice1--;
            }
            else if(ch1 == 's'){
                if(choice1 < 5) choice1++;
            }
            else if(ch1 == '\n'){
                break;
            }
        }
        endwin();

        switch(choice1){
            case 1: 
                NRZL(binary);
                break;
            case 2: 
                NRZI(binary);
                break;
            case 3: 
                AMI(binary);
                break;
            case 4: 
                Manchester(binary);
                break;
            case 5: 
                DManchester(binary);
                break;
        }
}

void analogEncodingMenu(string binary){
        initscr(); 
        noecho();
        cbreak();
        keypad(stdscr, TRUE);

        int choice = 1;
        int ch;

        while (true) {
            clear();
            mvprintw(0, 0, "Select encoding technique:");
            mvprintw(2, 0, "[%c] PCM", (choice == 1 ? 'x' : ' '));
            mvprintw(3, 0, "[%c] DM (Delta Modulation)", (choice == 2 ? 'x' : ' '));
            mvprintw(5, 0, "Use 'w' to move UP and 's' to move DOWN, Enter to select.");

            ch = getch();
            if (ch == 'w') {
                if(choice >1) choice--;
            }
            else if (ch == 's') {
                if(choice < 2) choice++;
            }
            else if (ch == '\n') {
                break;
            }
        }
        endwin();
        
        if(choice == 1) PCM(binary);
        else if(choice == 2) DM(binary);
}


void mainMenu(){
    cout << "\nWelcome To Data Tranmissions!" <<endl;

    string signal;
    cout << "\nEnter the type of input signal (digital/analog)" <<endl;
    getline(cin, signal);
    string binary;
    cout << "\nEnter the input binary:" <<endl;
    getline(cin, binary);
    if(signal == "digital"){
        digitalEncodingMenu(binary);
    }
    else if(signal == "analog"){
       analogEncodingMenu(binary);
    }
    else{
        cout << "Invalid Input" <<endl;
        cout.flush();
    }
}

int main(){
    mainMenu();
    return 0;
}