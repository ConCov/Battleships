#include <iostream>
#include "./termcolor.hpp"
//imported library from GitHub "https://github.com/ikalnytskyi/termcolor" (used for debug)
#include "./macro.h"

class Entity {
    protected:
        int pos[2];
        int colour;
    
    public:
        int * getPos(){
            return pos;
        }

        int getColour(){
            return colour;
        }

        void setColour(int colour){
            this->colour = colour;
        }

        void move(int x, int y){
            pos[0] = pos[0] + x;
            pos[1] = pos[1] + y;
        }
};

class Point: public Entity {
    public:
        Point(){
            colour = (RED);
        }
        Point(int x, int y){
            colour = (RED);
            pos[0] = x; pos[1] = y;
        }
        Point(int x, int y, int colour){
            this->colour = colour;
            pos[0] = x; pos[1] = y;
        }
};

class Ship: public Entity {
	int l, range[2];
    int totat_health;
public:
	Ship(){}
	Ship(int l){
        this->l = l;
		pos[0] = 5; pos[1] = 5;
        range[0] = 0; range[1] = l;
        totat_health = l;
        colour = (GREEN);
	}

	void rotate(){
        int swap = range[0];
        range[0] = -range[1];
		range[1] = swap;
	}

	bool isHit(Point n){
        int *p = n.getPos();
        int x = *p - pos[0];
        int y = *(p+1) - pos[1];
		if (0 <= x <= range[0] &&
            1 <= y <= range[1]){
                return true;
        }
        return false;
	}

    bool isIntersecting(Ship s){
        int *s_pos = s.getPos();
        int *s_range = s.getRange();
        return false;
    }

    void damage(){
        if (colour == (GREEN)){
            colour = (YELLOW);
        }
        if (!totat_health){
            colour = (RED);
        }
    }

    int * getRange(){
        return range;
    }

    int getLength(){
        return l;
    }
};

int xyTOint(int x, int y){
    return 10*(y-1) + (x-1);
}

void clearScreen(int *ptr){
    for (int i = 0; i < 100; i++){
        *ptr = (BLACK);
        ptr += 1;
    }
}

void logScreen(int grid[]){
    for (int i = 0; i < 100; i++){
        if (i%10 == 0){
            std::cout << "\n";
        }
        
        switch (grid[i]){
            case 0:
                std::cout << termcolor::white << grid[i] << "  " << termcolor::reset;
                break;
            case 1:
                std::cout << termcolor::white << grid[i] << "  " << termcolor::reset;
                break;
            case 2:
                std::cout << termcolor::yellow << grid[i] << "  " << termcolor::reset;
                break;
            case 3:
                std::cout << termcolor::red << grid[i] << "  " << termcolor::reset;
                break;
            case 4:
                std::cout << termcolor::green << grid[i] << "  " << termcolor::reset;
                break;
            default:
                std::cout << "e  ";
        }
        
        //std::cout << grid[i] << "  ";
        //std::cout << termcolor::red << "Hello, Colorful World!" << termcolor::reset << std::endl;

    }
}

std::string getInput(std::string prompt){
    std::string input;
    std::cout << prompt;
    std::cin >> input;
    return input;
}

int main(int argc, char const *argv[])
{
    int grid[100];
    int *grid_ptr = grid;

    Ship ships[10] = {5, 4, 4, 3, 3, 3, 2, 2, 2, 2};
    int s = 0;
    int* pos_ptr;
    int* range_ptr;

    Point points[100];
    int p = 0;

    Ship current;
    int x, y, dist;
    bool rotate;

    bool STOP = false;
    bool SETUP = true;
    bool CONFIRM = true;
    bool TML = true;
    int frames = 0;

    while (!STOP){
        if (SETUP && CONFIRM){
            s ++;
            CONFIRM = false;
        } else if (SETUP){
            for (int i = 0; i < s-1; i ++){
                if (ships[s-1].isIntersecting(ships[i])){
                    ships[s-1].setColour(RED);
                }
            }
        }

        clearScreen(grid);
        for (int i = 0; i < s; i++){
            current = ships[i];
            pos_ptr = current.getPos();
            range_ptr = current.getRange();
            for (int r = 0; r < current.getLength(); r++){
                x = *pos_ptr + (*range_ptr/current.getLength())*r;
                y = *(pos_ptr+1) + (*(range_ptr+1)/current.getLength())*r;
                grid[xyTOint(x, y)] = current.getColour();
            }
        }
        for (int i = 0; i < p; i++){
            pos_ptr = points[i].getPos();
            grid[xyTOint(*pos_ptr, *(pos_ptr+1))] = points[i].getColour();
        }
        logScreen(grid);

        if (SETUP){
            if (TML){
                x = stoi(getInput("\nX:"));
                y = stoi(getInput("\nY:"));
                dist = stoi(getInput("\nDISTANCE:"));
                rotate = stoi(getInput("\nROTATE:"));       
                CONFIRM = stoi(getInput("\nCONFIRM:"));
            } else {
                //get inputs from the sensors (buttons, joystick)
            }
            ships[s-1].move(x*dist, y*dist);
            if (rotate){
                ships[s-1].rotate();
            }
        }

        if (frames == 9){
            STOP = true;
        }
        frames ++;
    }

    return 0;
}

