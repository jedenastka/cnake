#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <ncurses.h>

int random(int from, int to) {
    return rand() % (to + 1) + from;
}

class Snake {
    public:
        Snake();
        void move(std::pair<int, int> direction, bool makeBigger);
        std::vector<std::pair<int, int>> getBody();
    private:
        std::vector<std::pair<int, int>> body;
};

Snake::Snake() {
    for (int i = 1; i <= 7; i++) {
        body.push_back(std::make_pair(i, 1));
    }
}

void Snake::move(std::pair<int, int> direction, bool makeBigger) {
    if (!makeBigger) {
        body.erase(body.begin());
    }
    std::pair<int, int> newHead = std::make_pair(std::get<0>(body.back()) + std::get<0>(direction), std::get<1>(body.back()) + std::get<1>(direction));
    body.push_back(newHead);
}

std::vector<std::pair<int, int>> Snake::getBody() {
    return body;
}

class Game {
    public:
        Game(int widthArg, int heightArg, int tickTimeArg);
        void start();
    private:
        // settings
        int width, height;
        int tickTime;
        // objects
        Snake snake;
        std::vector<std::pair<int, int>> rocks;
        std::pair<int, int> apple;
        bool appleCollected;
        // directions
        std::pair<int, int> direction;
        int directionIndicator;
        std::pair<int, int> directions[4];
        // screen
        int screen[80][24];
        WINDOW* win;
        // functions
        std::pair<int, int> randomApple();
        void over();
        void input();
        void tick();
        void draw(bool dead);
};

Game::Game(int widthArg, int heightArg, int tickTimeArg) {
    // set settings
    width = widthArg;
    height = heightArg;
    tickTime = tickTimeArg;
    // make a win and configure
    win = newwin(height, width, 0, 0);
    keypad(win, 1);
    nodelay(win, 1);
    // make map (rocks)
    for (int i = 0; i < 24; i++) {
        rocks.push_back(std::make_pair(0, i));
        rocks.push_back(std::make_pair(80-1, i));
        /*map[0][i] = 4;
        map[80-1][i] = 4;*/
    }
    for (int i = 0; i < 80; i++) {
        rocks.push_back(std::make_pair(i, 0));
        rocks.push_back(std::make_pair(i, 24-1));
        /*map[i][0] = 4;
        map[i][24-1] = 4;*/
    }
    // directions
    directions[0] = std::make_pair(1, 0);
    directions[1] = std::make_pair(0, 1);
    directions[2] = std::make_pair(-1, 0);
    directions[3] = std::make_pair(0, -1);
    directionIndicator = 0;
}

void Game::start() {
    appleCollected = 0;
    apple = randomApple();
    while (1) {
        input();
        draw(0);
        tick();
        slk_refresh_sp()
    }
}

std::pair<int, int> randomApple() {
    return std::make_pair(random(0, width-1), random(0, height-1));
}

void Game::over() {
    draw(1);
    printw("Game over!\n");
    endwin();
    exit(0);
}

void Game::input() {
    if (random(0, 3) > 2/*rightArrow*/) {
        directionIndicator++;
    } else if (random(0, 3) > 2/*leftArrow*/) {
        directionIndicator--;
    }
    if (directionIndicator == -1) {
        directionIndicator = 3;
    } else if (directionIndicator == 4) {
        directionIndicator = 0;
    }
    // not implemented yet
}

void Game::tick() {
    direction = directions[directionIndicator];
    auto snakeBody = snake.getBody();
    auto snakeHead = snakeBody.back();
    for (auto itr = snakeBody.begin(); itr != snakeBody.end() - 1; ++itr) {
        if (*itr == snakeHead) {
            over();
        }
    }
    for (auto i: rocks) {
        if (snakeHead == i) {
            over();
        }
    }
    if (apple == snakeHead) {
        appleCollected = 1;
        apple = std::make_pair(random(0, 79), random(0, 23));
    }
    snake.move(direction, appleCollected);
    appleCollected = 0;
}

void Game::draw(bool dead) {
    refresh();
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 24; j++) {
            screen[i][j] = 0;
        }
    }
    screen[std::get<0>(apple)][std::get<1>(apple)] = 4;
    auto snakeBody = snake.getBody();
    auto snakeHead = snakeBody.back();
    for (auto i: snakeBody) {
        screen[std::get<0>(i)][std::get<1>(i)] = 1;
    }
    for (auto i: rocks) {
        screen[std::get<0>(i)][std::get<1>(i)] = 5;
    }
    int snakeHeadDraw;
    if (!dead) {
        snakeHeadDraw = 2;
    } else {
        snakeHeadDraw = 3;
    }
    screen[std::get<0>(snakeHead)][std::get<1>(snakeHead)] = snakeHeadDraw;
    clear();
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 80; j++) {
            if (screen[j][i] == 0) {
                printw(" ");
            } else if (screen[j][i] == 1) {
                printw("#");
            } else if (screen[j][i] == 2) {
                printw("@");
            } else if (screen[j][i] == 3) {
                printw("X");
            } else if (screen[j][i] == 4) {
                printw("$");
            } else if (screen[j][i] == 5) {
                printw("%%");
            }
        }
        //printw("\n");
    }
}

int main() {
    srand(time(NULL));
    initscr();
    Game game;
    game.start(80, 24);
    endwin();
}
