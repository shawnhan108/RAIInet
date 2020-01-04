#include <iostream>
#include <string>
#include "cell.h"
#include "board.h"
#include "graphicsdisplay.h"
#include <stdexcept>
#include <vector>
#include <sstream>

int nmzl(std::string ability, std::string b) {
    char check = b[0];
    for (size_t i = 0; i < ability.length(); ++i) {
        if (ability[i] == check) {
            return i + 1;
        }
    }
    return -999;
}

int main (int argc, char *argv[]) {
    int i = 1;
    std::string abs1 = "LFDSP";
    std::string abs2 = "LFDSP";
    std::string ids1 = "V1V2V3V4D1D2D3D4";
    std::string ids2 = "V1V2V3V4D1D2D3D4";
    std::vector<int> ability1Vec;
    ability1Vec.emplace_back(0); //L
    ability1Vec.emplace_back(0); //F
    ability1Vec.emplace_back(0); //D
    ability1Vec.emplace_back(0); //S
    ability1Vec.emplace_back(0); //P
    ability1Vec.emplace_back(0); //Z
    ability1Vec.emplace_back(0); //A
    ability1Vec.emplace_back(0); //G

    std::vector<int> ability2Vec;
    ability2Vec.emplace_back(0); //L
    ability2Vec.emplace_back(0); //F
    ability2Vec.emplace_back(0); //D
    ability2Vec.emplace_back(0); //P
    ability2Vec.emplace_back(0); //S
    ability2Vec.emplace_back(0); //Z
    ability2Vec.emplace_back(0); //A
    ability2Vec.emplace_back(0); //G

    std::vector<bool> piecesCheck1;
    piecesCheck1.emplace_back(false); //V1
    piecesCheck1.emplace_back(false); //V2
    piecesCheck1.emplace_back(false); //V3 
    piecesCheck1.emplace_back(false); //V4
    piecesCheck1.emplace_back(false); //D1
    piecesCheck1.emplace_back(false); //D2
    piecesCheck1.emplace_back(false); //D3
    piecesCheck1.emplace_back(false); //D4

    std::vector<bool> piecesCheck2;
    piecesCheck2.emplace_back(false); //V1
    piecesCheck2.emplace_back(false); //V2
    piecesCheck2.emplace_back(false); //V3 
    piecesCheck2.emplace_back(false); //V4
    piecesCheck2.emplace_back(false); //D1
    piecesCheck2.emplace_back(false); //D2
    piecesCheck2.emplace_back(false); //D3
    piecesCheck2.emplace_back(false); //D4

    bool initGraphics = false;
    bool isValid = true;

    // get the ability and id from the command line argument
    while (i < argc) {
        std::string firstArg = argv[i];
        if (firstArg == "-graphics"){
            initGraphics = true;
            ++i;
        }else if (firstArg == "-ability1") {
            abs1 = argv[i+1];
            if (abs1.length() != 5) isValid = false;
            for (size_t i = 0; i < abs1.length(); ++i){
                if (abs1[i] == 'L') ability1Vec[0] += 1;
                else if (abs1[i] == 'F') ability1Vec[1] += 1;
                else if (abs1[i] == 'D') ability1Vec[2] += 1;
                else if (abs1[i] == 'S') ability1Vec[3] += 1;
                else if (abs1[i] == 'P') ability1Vec[4] += 1;
                else if (abs1[i] == 'Z') ability1Vec[5] += 1;
                else if (abs1[i] == 'A') ability1Vec[6] += 1;
                else if (abs1[i] == 'G') ability1Vec[7] += 1;
                else {
                    isValid = false;
                    break;
                }
            }
            for (size_t j = 0; j < ability1Vec.size(); ++j){
                if (ability1Vec[j] > 2) isValid = false;
            }
            i += 2;

        } else if (firstArg == "-ability2") {
            abs2 = argv[i+1];
            if (abs2.length() != 5) isValid = false;
            for (size_t i = 0; i < abs2.length(); ++i){
                if (abs2[i] == 'L') ability2Vec[0] += 1;
                else if (abs2[i] == 'F') ability2Vec[1] += 1;
                else if (abs2[i] == 'D') ability2Vec[2] += 1;
                else if (abs2[i] == 'S') ability2Vec[3] += 1;
                else if (abs2[i] == 'P') ability2Vec[4] += 1;
                else if (abs2[i] == 'Z') ability2Vec[5] += 1;
                else if (abs2[i] == 'A') ability2Vec[6] += 1;
                else if (abs2[i] == 'G') ability2Vec[7] += 1;
                else {
                    isValid = false;
                    break;
                }
            }
            for (size_t j = 0; j < ability2Vec.size(); ++j){
                if (ability2Vec[j] > 2){
                    isValid = false;
                    break;
                }
            }
            i += 2;
        } else if (firstArg == "-p1order") {
            ids1 = argv[i+1];
            if (ids1.length() != 16) isValid = false;
            for (size_t i = 0; i < ids1.length(); i+=2){
                if (ids1.substr(i, 2) == "V1") piecesCheck1[0] = true;
                else if (ids1.substr(i, 2) == "V2") piecesCheck1[1] = true;
                else if (ids1.substr(i, 2) == "V3") piecesCheck1[2] = true;
                else if (ids1.substr(i, 2) == "V4") piecesCheck1[3] = true;
                else if (ids1.substr(i, 2) == "D1") piecesCheck1[4] = true;
                else if (ids1.substr(i, 2) == "D2") piecesCheck1[5] = true;
                else if (ids1.substr(i, 2) == "D3") piecesCheck1[6] = true;
                else if (ids1.substr(i, 2) == "D4") piecesCheck1[7] = true;
                else{
                    isValid = false;
                    break;
                }
            }
            for (size_t j = 0; j < piecesCheck1.size(); ++j){
                if (!piecesCheck1[j]){
                    isValid = false;
                    break;
                }
            }
            i += 2;
        } else if (firstArg == "-p2order") {
            ids2 = argv[i+1];
            if (ids2.length() != 16) isValid = false;
            for (size_t i = 0; i < ids2.length(); i+=2){
                if (ids2.substr(i, 2) == "V1") piecesCheck2[0] = true;
                else if (ids2.substr(i, 2) == "V2") piecesCheck2[1] = true;
                else if (ids2.substr(i, 2) == "V3") piecesCheck2[2] = true;
                else if (ids2.substr(i, 2) == "V4") piecesCheck2[3] = true;
                else if (ids2.substr(i, 2) == "D1") piecesCheck2[4] = true;
                else if (ids2.substr(i, 2) == "D2") piecesCheck2[5] = true;
                else if (ids2.substr(i, 2) == "D3") piecesCheck2[6] = true;
                else if (ids2.substr(i, 2) == "D4") piecesCheck2[7] = true;
                else{
                    isValid = false;
                    break;
                }
            }
            for (size_t j = 0; j < piecesCheck2.size(); ++j){
                if (!piecesCheck2[j]){
                    isValid = false;
                    break;
                }
            }
            i += 2;
        } else {
            isValid = false;
            ++i;
        }
        
    }

    if (!isValid){
        std::cerr << "Your commandline argument is not valid. Exiting..." << std::endl;
        return 1;
    }

    Board b;
    b.init(ids1, ids2, abs1, abs2, initGraphics);
    std::cout << b;
    
    std::string LittleYellowDuck;
    std::string prevCommmand = ""; //either is "move" or "ability"
    // 1 for player1, 2 for player2
    i = 1;

    while (getline(std::cin, LittleYellowDuck)) {
        std::istringstream nmsl {LittleYellowDuck};
        std::string a,d,c;

        nmsl >> a;
        if (a == "quit"){ 
            break;
        }else if (a == "move") {

            nmsl >> d;
            nmsl >> c;

            if (c == "up") { c = "N"; 
            }else if(c == "down") {c = "S"; 
            }else if (c == "left") {c = "W"; 
            }else if (c == "right") {c = "E"; 
            }else if (c == "upleft") {c = "NW"; 
            }else if (c == "upright") {c = "NE"; 
            }else if (c == "downleft") {c = "SW"; 
            }else if (c == "downright") {c = "SE"; 
            }else{
                std::cerr << "Invalid move direction (" + c + ")." << std::endl;
                std::cerr << "Please re-enter." << std::endl;
                continue;
            }

            try {
                if (i == 1){
                    if (b.getGD()){
                        b.getGD()->switchPlayer();
                    }
                    b.movePiece(b.getP1(), d, c);
                    
                }else{
                    if (b.getGD()){
                        b.getGD()->switchPlayer();
                    }
                    b.movePiece(b.getP2(), d, c);
                    
                }
            }catch(std::out_of_range r){
                std::cerr << "Error: Invalid move out of bound. Message: " << std::endl;
                std::cerr << r.what() << std::endl;
                continue;
            }catch(std::logic_error l){
                std::cerr << "Error: Invalid move due to bad logic. Message: " << std::endl;
                std::cerr << l.what() << std::endl;
                continue;
            }
            
            if (b.hasWon()) {
                return 0;
            }

            b.Playerflip();
            std::cout << b;

            if (i == 1) {
                i = 2;
            } else {
                i = 1;
            }
            prevCommmand = "move";
        } else if (a == "ability") {
            std::string y;
            int x;
            
            nmsl >> y;
            if ((y.length() > 1) || (!isdigit(y[0]))){
                std::cerr << "Please enter a valid integer to indicate the ability you want to use. " << std::endl;
                std::cerr << "Please re-enter: " << std::endl;
                continue;
            }

            std::istringstream newConvert(y);
            newConvert >> x;
            
            if (prevCommmand == "ability"){
                std::cerr << "Error: You cannot use two abilities in a row." << std::endl;
                std::cerr << "Please move next: " << std::endl;
                continue;
            }
            
            try{
                if (i == 1) {
                    if (x == nmzl(abs1, "F")) {
                        
                        nmsl >> d;
                        if (nmsl >> c) {
                            b.useAbility("player1", x, d, c);
                        }else{
                            std::cerr << "Error: Invalid cell coordinates" << std::endl;
                            std::cerr << "Please enter a pair of integers to indicate the cell coordinates." << std::endl;
                            continue;
                        }

                    } else {
                        nmsl >> d;
                        b.useAbility("player1", x, d);
                        if (b.hasWon()) {return 0;}
                    }
                } else {
                    if (x == nmzl(abs2, "F")) {
                        nmsl >> d;
                        if (nmsl >> c) {
                            b.useAbility("player2", x, d, c);
                        }else{
                            std::cerr << "Error: Invalid cell coordinates" << std::endl;
                            std::cerr << "Please enter a pair of integers to indicate the cell coordinates." << std::endl;
                            continue;
                        }
                    } else {
                        nmsl >> d;
                        b.useAbility("player2", x, d);
                        if (b.hasWon()) {return 0;}
                    }
                }
                prevCommmand = "ability";
            }catch(std::out_of_range r){
                std::cerr << "Error: Ability error with message: " << std::endl;
                std::cerr << r.what() << std::endl;
                continue;
            }

            
            std::cout << b;
        }else{
            std::cerr << "Invalid commmand. Please re-enter: " << std::endl;
        }
    }
}
