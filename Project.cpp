#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Window.hpp>
#include<SFML/Graphics/Font.hpp>
#include<SFML/Graphics/Text.hpp>
#include<iostream>
#include<string>
#include<cmath>
#include<ncurses.h>
using namespace std;
using namespace sf;

void windowLogic(string& binary, vector<Vertex>& lines, const string& title, function<string(vector<float>)> decodeFunc,vector<float>& signalLevels  ){
    RenderWindow window(VideoMode({800, 400}), title);

    float x = 50;
    float y_mid = 200;
    float bit_width = 60;
    float amplitude = 50; 
    float level_high = y_mid - amplitude;
    float level_low  = y_mid + amplitude;

    //adding exit button
    RectangleShape exitButton(Vector2f(100.f, 35.f));
    exitButton.setPosition({650.f, 350.f});
    exitButton.setFillColor(Color::White);

    //adding decoder button
    RectangleShape decodeButton(Vector2f(100.f, 35.f));
    decodeButton.setPosition({50.f, 350.f});
    decodeButton.setFillColor(Color::White);

    Font font("/System/Library/Fonts/Supplemental/Arial.ttf"); 

    Text exitText(font, "X exit", 15); 
    exitText.setFillColor(Color::Black);
    Vector2f buttonPos = exitButton.getPosition(); 
    exitText.setPosition({buttonPos.x + 20.f, buttonPos.y + 5.f});

    Text decodeText(font, "Decode", 15); 
    decodeText.setFillColor(Color::Black);
    Vector2f decodePos = decodeButton.getPosition();
    decodeText.setPosition({decodePos.x + 10.f, decodePos.y + 5.f});

    Text decodedText(font, " ", 15);
    decodedText.setFillColor(Color::Black);
    decodedText.setPosition({decodePos.x, decodePos.y + 25.f});

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

                if (decodeButton.getGlobalBounds().contains(mousePosF)) {
                    if (decodeFunc) {
                        string decoded = decodeFunc(signalLevels);
                        decodedText.setString("Decoded Binary: " + decoded);
                    }
                }
            }
        }

        // Clearing previous frame
        window.clear(sf::Color::White);

        //drawing button
        window.draw(exitButton);
        window.draw(exitText);

        window.draw(decodeButton);
        window.draw(decodeText);
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

        // drawing decodedText.
        window.draw(decodedText);
        // displaying frame
        window.display();
    }
}
//NRZ-L
string decodeNRZL(vector<float> signalLevels){
    string bits = "";
        if (signalLevels.empty()) return bits;

        float level_high = 150;
        float level_low  = 250;

        for (float level : signalLevels) {
            bits += (level < (level_high + level_low) / 2) ? '1' : '0';
        }
    return bits;
}
void NRZL(string binary){
    cout<<"\nRunning NRZL"<<endl;
    float x = 50;
    float y_mid = 200;
    float bit_width = 60;
    float amplitude = 50;
    float level_high = y_mid - amplitude; 
    float level_low  = y_mid + amplitude; 


    vector<Vertex> lines;
    vector<float> signalLevels;
    float prev_y = level_low;
    lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));


    for (char bit : binary) {
        float current_y = (bit == '1') ? level_high : level_low; //level logic

        signalLevels.push_back(current_y);

        lines.push_back(Vertex({Vector2f(x, current_y), Color::Black}));
        x += bit_width;
        lines.push_back(Vertex({Vector2f(x, current_y), Color::Black}));
    }

    windowLogic(binary, lines, "NRZ-L Visualization", decodeNRZL, signalLevels);
}
//NRZ-I
string decodeNRZI(vector<float> signalLevels){
    string bits = "";
    if (signalLevels.empty()) return bits;

    float level_high = 150;
    float level_low  = 250;
    float prev = signalLevels[0];
    
    for (size_t i = 1; i < signalLevels.size(); i++) {
        bits += (signalLevels[i] != prev) ? '1' : '0';
        prev = signalLevels[i];
    }
    return bits;
}
void NRZI(string binary){
        cout<<"\nRunning NRZI"<<endl;
        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; 
        float level_high = y_mid - amplitude; 
        float level_low  = y_mid + amplitude; 

        //tells the starting either 1 is +ve or -ve(i.e., high level or low level)
        vector<Vertex> lines;
        vector<float> signalLevels;

        float prev_y = level_low;
        signalLevels.push_back(prev_y);
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        for(char bit : binary){
            if(bit == '1'){
                prev_y = (prev_y == level_high) ? level_low : level_high;
            }
            signalLevels.push_back(prev_y);

            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }
        
        windowLogic(binary, lines, "NRZ-I Visualization", decodeNRZI, signalLevels);

}
//B8ZS
string decodeB8ZS(vector<float> signalLevels){
    string bits = "";
    if (signalLevels.empty()) return bits;

    float high = 150;   
    float low  = 250;  
    float mid  = (high + low) / 2;
    
    vector<char> symbols;
    for (float level : signalLevels) {
        if (abs(level - mid) < 10) symbols.push_back('0'); 
        else if (level < mid) symbols.push_back('+');
        else symbols.push_back('-');
    }

    for (size_t i = 0; i < symbols.size(); i++) {
        // Check for B8ZS patterns
        if (i + 7 < symbols.size()) {
            string pattern(symbols.begin() + i, symbols.begin() + i + 8);
            if (pattern == "000+-0-+" || pattern == "000-+0+-") {
                bits += "00000000"; 
                i += 7; 
                continue;
            }
        }
        bits += (symbols[i] == '0') ? '0' : '1';
    }
    return bits;
}
void B8ZS(string binary){
    cout<<"\nRunning B8ZS"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; 
        float level_high = y_mid - amplitude; 
        float level_low  = y_mid + amplitude; 

        vector<Vertex> lines;
        vector<float> signalLevels;

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

            signalLevels.push_back(prev_y);
            
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }
        

        windowLogic(binary, lines, "B8ZS Scrambled Visualization", decodeB8ZS, signalLevels);

}
//HDB3
string decodeHDB3(vector<float> signalLevels){
    string bits = "";
    if (signalLevels.empty()) return bits;

    float y_mid = 200;         
    float amplitude = 50;        
    float level_high = y_mid - amplitude; 
    float level_low  = y_mid + amplitude; 
    float threshold = amplitude / 2; 

    float last_polarity = 0;
    int zeroCount = 0;

    for (float level : signalLevels) {
        if (abs(level - y_mid) < threshold) {
            bits += '0';
            zeroCount++;
        } 
        else {
            if (last_polarity != 0 && level == last_polarity) {
                for (int i = bits.size() - 3; i < bits.size(); ++i) {
                    if (i >= 0 && i < bits.size()) bits[i] = '0';
                }
                bits += '0'; 
            } 
            else {
                bits += '1';
                last_polarity = level;
                zeroCount = 0;
            }
        }
    }
    return bits;
}
void HDB3(string binary){
    cout<<"\nRunning HDB3"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; 
        float level_high = y_mid - amplitude; 
        float level_low  = y_mid + amplitude; 

        vector<Vertex> lines;
        vector<float> signalLevels;

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

            signalLevels.push_back(prev_y);

            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }
        

        windowLogic(binary, lines, "HDB3 Scrambled Visualization", decodeHDB3, signalLevels);

}
//AMI
string decodeAMI(vector<float> signalLevels){
    string bits = "";
    if (signalLevels.empty()) return bits;

     float level_high = 150;
    float level_low  = 250;
    float mid = (150 + 250) / 2; 

    for (float level : signalLevels) {
        if (abs(level - mid) < 10)
            bits += '0';
        else
            bits += '1';
    }

    return bits;
}
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
        float amplitude = 50; 
        float level_high = y_mid - amplitude; 
        float level_low  = y_mid + amplitude; 

        vector<Vertex> lines;
        vector<float> signalLevels;

        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        //ADD LOGIC HERE
        float last_polarity = level_low;
        for(char bit : binary){
            if(bit == '1'){
                last_polarity = (last_polarity == level_high) ? level_low : level_high;
                prev_y = last_polarity;
            }
            else if(bit == '0'){
                prev_y = y_mid;
            }

            signalLevels.push_back(prev_y);

            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x += bit_width;
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
        }

        windowLogic(binary, lines, "AMI Visualization", decodeAMI, signalLevels);
        
        }
        else{
            cout<< "Invalid Input"<<endl;
        }

}
//MANCHESTER
string decodeManchester(vector<float> signalLevels){
    string bits = "";
    if (signalLevels.size() < 2) return bits;

    float y_mid = 200;
    float amplitude = 50;
    float level_high = y_mid - amplitude;
    float level_low = y_mid + amplitude;
    float threshold = amplitude / 2;

    for (size_t i = 0; i + 1 < signalLevels.size(); i += 2) {
        float first = signalLevels[i];
        float second = signalLevels[i + 1];

        if (first < y_mid && second > y_mid)
            bits += '1'; 
        else if (first > y_mid && second < y_mid)
            bits += '0'; 
    }

    return bits;
}
void Manchester(string binary){
        cout<<"\nRunning Manchester"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; 
        float level_high = y_mid - amplitude; 
        float level_low  = y_mid + amplitude; 

        vector<Vertex> lines;
        vector<float> signalLevels;

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

            signalLevels.push_back(first_half_y);
            signalLevels.push_back(second_half_y);
        }
        

        windowLogic(binary, lines, "Manchester Visualization", decodeManchester, signalLevels);


}
//DMANCHESTER
string decodeDManchester(vector<float> signalLevels){
     string bits = "";
    if (signalLevels.size() < 2) return bits;

    float y_mid = 200;
    float amplitude = 50;
    float threshold = amplitude / 2;

    float prev_first = signalLevels[0];
    bool prevHigh = prev_first < y_mid - threshold;

    for (size_t i = 0; i + 1 < signalLevels.size(); i += 2) {
        float first = signalLevels[i];
        float second = signalLevels[i + 1];

        bool firstHigh  = first < y_mid - threshold;
        bool secondHigh = second < y_mid - threshold;
        bool transitionAtStart = (firstHigh != prevHigh);

        if (transitionAtStart)
            bits += '0'; 
        else
            bits += '1'; 
        prevHigh = secondHigh;
    }
    return bits;
}
void DManchester(string binary){
        cout<<"\nRunning DManchester"<<endl;
        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; 
        float level_high = y_mid - amplitude; 
        float level_low  = y_mid + amplitude; 

        vector<Vertex> lines;
        vector<float> signalLevels;

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
            
            signalLevels.push_back(first_half_y);
            signalLevels.push_back(second_half_y);
        }
        windowLogic(binary, lines, "Differential Manchester Visualization", decodeDManchester, signalLevels);


}
//PCM
string decodePCM(vector<float> signalLevels){
    string bits = "";
    float y_mid = 200;
    float amplitude = 50;
    float threshold = amplitude / 2;

    for (float level : signalLevels) {
        if (level < y_mid - threshold)
            bits += '1'; 
        else
            bits += '0'; 
    }
    return bits;
}
void PCM(string binary){
        cout<<"\nRunning PCM"<<endl;

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; 
        float level_high = y_mid - amplitude; 
        float level_low  = y_mid + amplitude; 

        vector<Vertex> lines;
        vector<float> signalLevels;

        float prev_y = y_mid;
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        //ADDING LOGIC HERE  
        for(char bit : binary){
            float x_end = x + bit_width;

            if(bit == '1'){
                prev_y = level_high;
            }
            else{
                prev_y = level_low;

            }
            signalLevels.push_back(prev_y);
            
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            lines.push_back(Vertex({Vector2f(x, prev_y), Color::Black}));
            x = x_end;
        }

        windowLogic(binary, lines, "PCM Visualization", decodePCM, signalLevels);


}
//DM
string decodeDM(vector<float> signalLevels){
    string bits = "";
    if (signalLevels.size() < 2) return bits;

    for (size_t i = 1; i < signalLevels.size(); i++) {
        if (signalLevels[i] < signalLevels[i - 1])
            bits += '1'; 
        else
            bits += '0'; 
    }
    return bits;
}
void DM(string binary){
        printf("\nRunning DM");

        float x = 50;
        float y_mid = 200;
        float bit_width = 60;
        float amplitude = 50; 

        vector<Vertex> lines;
        vector<float> signalLevels;

        float prev_y = y_mid;
        signalLevels.push_back(prev_y);
        lines.push_back(Vertex({Vector2f(x, prev_y),Color::Black}));

        //ADD LOGIC HERE
        float step = 40;
        for(char bit : binary){
            float x_end = x + bit_width;
            if(bit == '1'){
                prev_y -= step;
            }         
            else{
                prev_y += step;
            }
            signalLevels.push_back(prev_y);

            lines.push_back(Vertex({Vector2f(x_end, prev_y), Color::Black}));
            x = x_end;
        }
        windowLogic(binary, lines, "DM Visualization", decodeDM, signalLevels);
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
            mvprintw(0, 0, "Assuming the binary to be analog sample");
            mvprintw(2, 0, "Select encoding technique:");
            mvprintw(3, 0, "[%c] PCM", (choice == 1 ? 'x' : ' '));
            mvprintw(4, 0, "[%c] DM (Delta Modulation)", (choice == 2 ? 'x' : ' '));
            mvprintw(6, 0, "Use 'w' to move UP and 's' to move DOWN, Enter to select.");

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

void pallindrome(string binary){
    int n = binary.size();
   int st = 0, maxLen = 1;

    for (int center = 0; center < n; center++) {
        int l = center, r = center;
        while (l >= 0 && r < n && binary[l] == binary[r]) {
            if (r - l + 1 > maxLen) {
                st = l;
                maxLen = r - l + 1;
            }
            l--; r++;
        }
        l = center, r = center + 1;
        while (l >= 0 && r < n && binary[l] == binary[r]) {
            if (r - l + 1 > maxLen) {
                st = l;
                maxLen = r - l + 1;
            }
            l--; r++;
        }
    }

    string longest = binary.substr(st, maxLen);
    cout << "\nLongest Palindrome in the data stream: " << longest << endl;
}
void mainMenu(){
    cout << "\nWelcome To Data Tranmissions!" <<endl;
    string signal;
    cout << "\nEnter the type of input signal (digital/analog)" <<endl;
    getline(cin, signal);

    transform(signal.begin(), signal.end(), signal.begin(), ::tolower);
    
    if (signal == "digital" || signal == "analog") {
        string binary;
        cout << "\nEnter the input binary: ";
        getline(cin, binary);
        pallindrome(binary);
        if (signal == "digital")
            digitalEncodingMenu(binary);
        else
            analogEncodingMenu(binary);
    } 
    else {
        cout << "Invalid Input" << endl;
    }
}
int main(){
    mainMenu();
    return 0;
}