#include <curses.h>
#include <utility>
#include <map>
#include <vector>

void draw(WINDOW *win, std::map<std::pair<int, int>, int> objectMap, std::vector<std::pair<int, int>> snake);

void input(WINDOW *win, std::pair<int, int> &direction, bool &quit);

void tick(std::vector<std::pair<int, int>> &snake, std::pair<int, int> direction);

void game(std::pair<int, int> size, std::map<std::pair<int, int>, int> objectMap, int startingSize) {
    std::vector<std::pair<int, int>> snake;
    for (int i = 0; i < startingSize; ++i) {
        snake.push_back(std::make_pair(0 + i, 0));
    }
    initscr();
    noecho();
    WINDOW* win = newwin(size.second, size.first, 0, 0);
    keypad(win, 1);
    std::pair<int, int> direction;
    bool quit = 0;
    while (!quit) {
        draw(win, objectMap, snake);
        input(win, direction, quit);
        tick(snake, direction);
    }
    delwin(win);
    endwin();
}

void draw(WINDOW *win, std::map<std::pair<int, int>, int> objectMap, std::vector<std::pair<int, int>> snake) {
    wclear(win);
    for (auto object: objectMap) {
        wmove(win, object.first.second, object.first.first);
        wprintw(win, "x");
    }
    for (auto snakePart: snake) {
        wmove(win, snakePart.second, snakePart.first);
        wprintw(win, "@");
    }
    wrefresh(win);
}

void input(WINDOW *win, std::pair<int, int> &direction, bool &quit) {
    int ch = wgetch(win);
    switch (ch) {
        case KEY_UP:
            direction = std::make_pair(0, -1);
            break;
        case KEY_DOWN:
            direction = std::make_pair(0, 1);
            break;
        case KEY_LEFT:
            direction = std::make_pair(-1, 0);
            break;
        case KEY_RIGHT:
            direction = std::make_pair(1, 0);
            break;
        case 'q':
            quit = 1;
            break;
    }
}

void moveSnake(std::vector<std::pair<int, int>> &snake, std::pair<int, int> direction);

void tick(std::vector<std::pair<int, int>> &snake, std::pair<int, int> direction) {
    moveSnake(snake, direction);
}

void moveSnake(std::vector<std::pair<int, int>> &snake, std::pair<int, int> direction) {
    snake.erase(snake.begin());
    snake.push_back(std::make_pair(snake.back().first + direction.first, snake.back().second + direction.second));
}

int main() {
    std::map<std::pair<int, int>, int> objectMap;
    game(std::make_pair(80, 24), objectMap, 3);
}