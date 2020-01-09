#include <ncurses.h>
#include <utility>
#include <map>
#include <vector>

void draw(WINDOW *win, std::map<std::pair<int, int>, int> objectMap, std::vector<std::pair<int, int>> snake);

char getInput(WINDOW *win);

void tick(std::vector<std::pair<int, int>> &snake);

void game(std::pair<int, int> size, std::map<std::pair<int, int>, int> objectMap, int startingSize) {
    std::vector<std::pair<int, int>> snake;
    for (int i = 0; i < startingSize; ++i) {
        snake.push_back(std::make_pair(0 + i, 0));
    }
    initscr();
    noecho();
    WINDOW* win = newwin(size.first, size.second, 0, 0);
    while (1) {
        draw(win, objectMap, snake);
        char input = getInput(win);
        tick(snake);
    }
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

char getInput(WINDOW *win) {
    return wgetch(win);
}

void moveSnake(std::vector<std::pair<int, int>> &snake, int x, int y);

void tick(std::vector<std::pair<int, int>> &snake) {
    moveSnake(snake, 1, 0);
}

void moveSnake(std::vector<std::pair<int, int>> &snake, int x, int y) {
    snake.erase(snake.begin());
    snake.push_back(std::make_pair(snake.back().first + x, snake.back().second + y));
}

int main() {
    std::map<std::pair<int, int>, int> objectMap;
    game(std::make_pair(24, 80), objectMap, 3);
}