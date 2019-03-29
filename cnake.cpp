#include <ncurses.h>
#include <vector>
#include <ctime>
#include <chrono>
#include <thread>

std::string version = "1.2.0";

int random(int from, int to) {
    return rand() % to + 1 + from;
}

void input(WINDOW* win, int &key) {
    while (1) {
        key = wgetch(win);
    }
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
	// make snake body
    for (int i = 1; i <= startingSize; i++) {
        body.push_back(std::make_pair(i+1, 2));
    }
}

void Snake::move(std::pair<int, int> direction, bool makeBigger) {
    if (!makeBigger) {
		// delete tail
        body.erase(body.begin());
    }
    // make new head
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
        // directions
        std::pair<int, int> direction;
        int directionIndicator;
        std::pair<int, int> directions[4];
        // screen
        int screen[80][24];
        WINDOW* win;
        // functions
        std::pair<int, int> randomApple();
        void tick();
        void draw();
        // input
        int key;
        // triggers
        bool isDead;
        bool appleCollected;
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
    // TODO: custom maps
    for (int i = 0; i < 24; i++) {
        rocks.push_back(std::make_pair(0, i));
        rocks.push_back(std::make_pair(80-1, i));
    }
    for (int i = 0; i < 80; i++) {
        rocks.push_back(std::make_pair(i, 0));
        rocks.push_back(std::make_pair(i, 24-1));
    }
}



void Game::start() {
	// thread for getting input
    std::thread inputThread(input, win, std::ref(key));
    // zeroing
    appleCollected = 0;
    isDead = 0;
    apple = randomApple();
    directionIndicator = 1;
    while (!isDead) {
		// game loop
        draw();
        tick();
        draw();
        // sleep between ticks
        std::this_thread::sleep_for(std::chrono::milliseconds(tickTime));
    }
    draw();
    refresh();
    wgetch(win);
    endwin();
    exit(0);
}

std::pair<int, int> Game::randomApple() {
	// return random apple position
	// TODO: bug with dissapearing apple
    return std::make_pair(random(0+1, width-1-1), random(0+1, height-1-1));
}

void Game::tick() {
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

    direction = directions[directionIndicator];
    auto snakeBody = snake.getBody();
    auto snakeHead = snakeBody.back();
    // check for dead
    for (auto itr = snakeBody.begin(); itr != snakeBody.end() - 1; ++itr) {
        if (*itr == snakeHead) {
            isDead = 1;
        }
    }
    for (auto i: rocks) {
        if (snakeHead == i) {
            isDead = 1;
        }
    }
    if (isDead) {
        return;
    }
    // apple collecting
    if (apple == snakeHead) {
        appleCollected = 1;
        apple = randomApple();
    }
    snake.move(direction, appleCollected);
    appleCollected = 0;
}

void Game::draw() {
    wrefresh(win);
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 24; j++) {
            screen[i][j] = 0;
        }
    }
    auto snakeBody = snake.getBody();
    auto snakeHead = snakeBody.back();
    for (auto i: snakeBody) {
        screen[std::get<0>(i)][std::get<1>(i)] = 1;
    }
    for (auto i: rocks) {
        screen[std::get<0>(i)][std::get<1>(i)] = 5;
    }
    screen[std::get<0>(apple)][std::get<1>(apple)] = 4;
    int snakeHeadDraw;
    if (!isDead) {
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
