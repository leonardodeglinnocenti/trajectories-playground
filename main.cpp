#include <iostream>
#include </usr/include/ncurses.h>
#include <chrono>
#include <thread>
#include <vector>

// Function to create a blank map with boolean values representing walls

class Map {
private:
    unsigned int rows;
    unsigned int cols;
    bool** map;
    int number;
    std::pair<int, int> endGoal;

public:
    Map(unsigned int rows, unsigned int cols, int number) : rows(rows), cols(cols), number(number) {
        map = new bool*[rows];
        for (int i = 0; i < rows; i++) {
            map[i] = new bool[cols];
            for (int j = 0; j < cols; j++) {
                if (j > 0 && map[i][j - 1]) {
                    map[i][j] = rand() % 6 != 0;
                } else {
                    map[i][j] = rand() % 60 == 0;
                }
            }
        }
        setRandomEndGoal();
    }

    void displayMap() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (map[i][j]) {
                    addch('#');
                } else {
                    addch(' ');
                }
            }
            addch('\n');
        }
        // Print endGoal in the map using green color
        mvaddch(endGoal.first, endGoal.second, 'G');
        refresh();
        endwin(); // Add this line to fix the issue with the last line of the map
    }

    unsigned int getCols() {
        return cols;
    }

    unsigned int getRows() {
        return rows;
    }
    
    bool getIsWall(int x, int y) {
        return map[y][x];
    }

    void setRandomEndGoal() {
        srand(time(0));
        int x = rand() % cols;
        int y = rand() % rows;
        while (map[y][x]) {
            x = rand() % cols;
            y = rand() % rows;
        }
        endGoal = std::make_pair(y, x);
    }

    void setEndGoal(int x, int y) {
        endGoal = std::make_pair(y, x);
    }

    void hittingAnimation(int x, int y) {
        for (int i = 0; i < 2; i++) {
            mvaddch(y, x, 'X');
            refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            mvaddch(y, x, '#');
            refresh();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    
    }

};

// Class to represent a robot moving in the map
class Robot {
private:
    int x;
    int y;
    Map& map; // Reference to the map object
    bool canBounce;
    unsigned int direction;
    // 1 is up-left, 2 is up, 3 is up-right, 4 is left, 5 is stay, 6 is right, 7 is down-left, 8 is down, 9 is down-right

public:
    Robot(int startX, int startY, Map& map, bool canBounce) : x(startX), y(startY), map(map), canBounce(canBounce), direction(5) {}

    bool isValidMove(int newX, int newY) {
        if (newX >= 0 && newX < map.getCols() && newY >= 0 && newY < map.getRows()) {
            return !map.getIsWall(newX,newY);
        }
        return false;
    }

    void moveUp() {
        if (isValidMove(x, y - 1)) {
            y--;
            direction = 2;
        }
        else if(canBounce) {
            map.hittingAnimation(x, y - 1);
            if (isValidMove(x, y + 1)) {
                y++;
                direction = 8;
            }
        }
        else {
            map.hittingAnimation(x, y - 1);
        }
    }

    void moveDown() {
        if (isValidMove(x, y + 1)) {
            y++;
            direction = 8;
        }
        else if(canBounce) {
            map.hittingAnimation(x, y + 1);
            if (isValidMove(x, y - 1)) {
                y--;
                direction = 2;
            }
        }
        else {
            map.hittingAnimation(x, y + 1);
        }
    }

    void moveLeft() {
        if (isValidMove(x - 1, y)) {
            x--;
            direction = 4;
        }
        else if(canBounce) {
            map.hittingAnimation(x - 1, y);
            x++;
            direction = 6;
        }
        else {
            map.hittingAnimation(x - 1, y);
        }
    }

    void moveRight() {
        if (isValidMove(x + 1, y)) {
            x++;
            direction = 6;
        }
        else if(canBounce) {
            map.hittingAnimation(x + 1, y);
            x--;
            direction = 4;
        }
        else {
            map.hittingAnimation(x + 1, y);
        }
    }

    void moveUpLeft() {
        if (isValidMove(x - 1, y - 1)) {
            x--;
            y--;
            direction = 1;
        }
        else if(canBounce) {
            map.hittingAnimation(x - 1, y - 1);
            if (isValidMove(x - 1, y)) {
                x = x - 2;
                direction = 4;
            }
            else if (isValidMove(x, y - 1)) {
                y--;
            }
        }
        else {
            map.hittingAnimation(x - 1, y - 1);
        }
    }

    void moveUpRight() {
        if (isValidMove(x + 1, y - 1)) {
            x++;
            y--;
            direction = 3;
        }
        else if(canBounce) {
            map.hittingAnimation(x + 1, y - 1);
            if (isValidMove(x + 1, y)) {
                x = x + 2;
                direction = 6;
            }
            else if (isValidMove(x, y - 1)) {
                y--;
                direction = 2;
            }
        }
        else {
            map.hittingAnimation(x + 1, y - 1);
        }
    }

    void moveDownLeft() {
        if (isValidMove(x - 1, y + 1)) {
            x--;
            y++;
            direction = 7;
        }
        else if(canBounce) {
            map.hittingAnimation(x - 1, y + 1);
            if (isValidMove(x - 2, y)) {
                x = x - 2;
                direction = 4;
            }
            else if (isValidMove(x, y + 1)) {
                y++;
                direction = 8;
            }
        }
        else {
            map.hittingAnimation(x - 1, y + 1);
        }
    }

    void moveDownRight() {
        if (isValidMove(x + 1, y + 1)) {
            x++;
            y++;
            direction = 9;
        }
        else if(canBounce) {
            map.hittingAnimation(x + 1, y + 1);
            if (isValidMove(x + 1, y)) {
                x = x + 2;
                direction = 6;
            }
            else if (isValidMove(x, y + 1)) {
                y++;
                direction = 8;
            }
        }
        else {
            map.hittingAnimation(x + 1, y + 1);
        }
    }

    std::pair<int, int> getRobotPosition() {
        return std::make_pair(x, y);
    }

    void displayRobot() {
        mvaddch(y, x, 'R');
        refresh();
    }

    void updateDisplayedRobotPosition() {
        clear();
        map.displayMap();
        displayRobot();
    }

    unsigned int getDirection() {
        return direction;
    }

    // Bounce forward feature for the robot, takes into account the previous position
    // If the robot hits a wall diagonally, it will bounce forward
    // If the robot hits a wall horizontally or vertically, it will bounce back
    

};

// Class to represent a path containing a list of routes
class Path {
private:
    std::vector<std::pair<int, int>> routes;
    Robot robot;
    Map map;

public:
    Path(int x, int y, Robot& robot, Map& map) : robot(robot), map(map) {
        routes.push_back(robot.getRobotPosition());
    }

    void performRandomPath(int x, int y, int steps, int timeInterval) {
        for (int i = 0; i < steps; i++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
            // The seed is set to the current time
            srand(time(0));
            int direction = rand() % 8;
            switch (direction) {
                case 0:
                    robot.moveUp();
                    robot.updateDisplayedRobotPosition();
                    break;
                case 1:
                    robot.moveDown();
                    robot.updateDisplayedRobotPosition();
                    break;
                case 2:
                    robot.moveLeft();
                    robot.updateDisplayedRobotPosition();
                    break;
                case 3:
                    robot.moveRight();
                    robot.updateDisplayedRobotPosition();
                    break;
                case 4:
                    robot.moveUpLeft();
                    robot.updateDisplayedRobotPosition();
                    break;
                case 5:
                    robot.moveUpRight();
                    robot.updateDisplayedRobotPosition();
                    break;
                case 6:
                    robot.moveDownLeft();
                    robot.updateDisplayedRobotPosition();
                    break;
                case 7:
                    robot.moveDownRight();
                    robot.updateDisplayedRobotPosition();
                    break;
            }
            routes.push_back(robot.getRobotPosition());
        }
    }

    void moveKeepingDirection(int direction) {
        switch (direction) {
            case 1:
                robot.moveUpLeft();
                break;
            case 2:
                robot.moveUp();
                break;
            case 3:
                robot.moveUpRight();
                break;
            case 4:
                robot.moveLeft();
                break;
            case 5:
                break;
            case 6:
                robot.moveRight();
                break;
            case 7:
                robot.moveDownLeft();
                break;
            case 8:
                robot.moveDown();
                break;
            case 9:
                robot.moveDownRight();
                break;
        }
        robot.updateDisplayedRobotPosition();
    }

};


// Main function
int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);

    Map map(20, 50, 1);
    Robot robot(1, 1, map, true);

    map.displayMap();

    while (true) {
        robot.updateDisplayedRobotPosition();

        // Create a random path
        //Path path = Path(1, 1, robot, map);
        //path.performRandomPath(1, 1, 1000, 100);

        // Loop takes in input a first movement and then moves the robot in the same direction
        Path path = Path(1, 1, robot, map);

        int ch = getch();
        switch (ch) {
            case KEY_UP:
                robot.moveUp();
                break;
            case KEY_DOWN:
                robot.moveDown();
                break;
            case KEY_LEFT:
                robot.moveLeft();
                break;
            case KEY_RIGHT:
                robot.moveRight();
                break;
            case 'w':
                robot.moveUpLeft();
                break;
            case 'e':
                robot.moveUpRight();
                break;
            case 's':
                robot.moveDownLeft();
                break;
            case 'd':
                robot.moveDownRight();
                break;
            case 'q':
                endwin();
                return 0;
        }
        while(true){
            path.moveKeepingDirection(robot.getDirection());
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    endwin();
    return 0;
}



