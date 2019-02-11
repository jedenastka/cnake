#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <ncurses.h>
#include <string>

std::string version = "1.1b";

int random(int from, int to) {
    return rand() % (to + 1) + from;
}

class Snake {
    public:
        Snake(int startingSize);
        void move(std::pair<int, int> direction, bool makeBigger);
        std::vector<std::pair<int, int>> getBody();
    private:
        std::vector<std::pair<int, int>> body;
};

Snake::Snake(int startingSize) {
    for (int i = 1; i <= startingSize; i++) {
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
        Game(int widthArg, int heightArg, int tickTimeArg, int startingSizeSnake);
        void start();
    private:
        // settings
        const int width, height;
        const int tickTime;
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

Game::Game(int widthArg, int heightArg, int tickTimeArg, int startingSizeSnake)
    : snake(startingSizeSnake)
    , directions{std::make_pair(0, -1), std::make_pair(1, 0), std::make_pair(0, 1), std::make_pair(-1, 0)}
    , width(widthArg)
    , height(heightArg)
    , tickTime(tickTimeArg)
    {
    // make a win and configure
    win = newwin(height, width, 0, 0);
    keypad(win, 1);
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
}

void Game::start() {
    nodelay(win, 1);
    appleCollected = 0;
    apple = randomApple();
    directionIndicator = 1;
    while (1) {
        input();
        tick();
        draw(0);
    }
}

std::pair<int, int> Game::randomApple() {
    return std::make_pair(random(0+1, width-2), random(0+1, height-2));
}

void Game::over() {
    draw(1);
    refresh();
    nodelay(win, 0);
    wgetch(win);
    endwin();
    exit(0);
}

void Game::input() {
    wtimeout(win, tickTime);
    auto key = wgetch(win);
    int oldDirectionIndicator = directionIndicator;
    if (key == KEY_UP) {
        directionIndicator = 0;
    } else if (key == KEY_RIGHT) {
        directionIndicator = 1;
    } else if (key == KEY_DOWN) {
        directionIndicator = 2;
    } else if (key == KEY_LEFT) {
        directionIndicator = 3;
    }
    // prevent from going to opposite direction
    if (oldDirectionIndicator-2 == directionIndicator || oldDirectionIndicator+2 == directionIndicator) {
        directionIndicator = oldDirectionIndicator;
    }
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
        apple = randomApple();
    }
    snake.move(direction, appleCollected);
    appleCollected = 0;
}

void Game::draw(bool dead) {
    wrefresh(win);
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
    wclear(win);
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 80; j++) {
            if (screen[j][i] == 0) {
                wprintw(win, " ");
            } else if (screen[j][i] == 1) {
                wprintw(win, "#");
            } else if (screen[j][i] == 2) {
                wprintw(win, "@");
            } else if (screen[j][i] == 3) {
                wprintw(win, "X");
            } else if (screen[j][i] == 4) {
                wprintw(win, "$");
            } else if (screen[j][i] == 5) {
                wprintw(win, "%%");
            }
        }
    }
}

int main() {
    srand(time(NULL));
    initscr();
    Game game(80, 24, 500, 3);
    game.start();
    endwin();
}
