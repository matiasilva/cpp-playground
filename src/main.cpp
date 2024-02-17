#include <SFML/Graphics.hpp>
#include <array>

#define WIDTH 800
#define HEIGHT 800
#define GRID 0.05
#define GRID_SIZE GRID*WIDTH
#define N_ROWS ((int)(1/GRID) + 1) // extra row for floor
#define N_COLS ((int)(1/GRID))
#define N_CELLS N_ROWS*N_COLS

void draw_lines(sf::RenderWindow *window) {
    for (int row = GRID_SIZE; row < WIDTH; row+=GRID_SIZE) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, row), sf::Color::Black),
            sf::Vertex(sf::Vector2f(WIDTH, row), sf::Color::Black)
        };
        window->draw(line, 2, sf::Lines);
    }
    for (int col = GRID_SIZE; col < HEIGHT; col+=GRID_SIZE) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(col, 0), sf::Color::Black),
            sf::Vertex(sf::Vector2f(col, HEIGHT), sf::Color::Black)
        };
        window->draw(line, 2, sf::Lines);
    }
}

void init_cells(int grid[][N_COLS]) {
    for (int i = 0; i < N_ROWS; i++) {
        for (int j = 0; j < N_COLS; j++) {
            if (i == (N_ROWS - 1)) {
                // last row
                grid[i][j] = 1;
                continue;
            }
            grid[i][j] = 0;
        }
    }
}

void update_cells(int grid[][N_COLS], int next_grid[][N_COLS]) {
    /* ALGORITHM
        1. check if we are 1 and check neighbor below if 1
            a) if 1, don't move
            b) if 0, move into cell below
    */
    for (int i = 0; i < N_ROWS; i++) {
        for (int j = 0; j < N_COLS; j++) {
            bool is_active = grid[i][j] == 1;
            bool below_active = grid[i+1][j] == 1;
            if (is_active && !below_active) {
                next_grid[i+1][j] = 1;
            }
        }
    }
}

void draw_cells(sf::RenderWindow *window, int grid[][N_COLS]) {
    for (int i = 0; i < N_ROWS; i++) {
        for (int j = 0; j < N_COLS; j++) {
            sf::RectangleShape rect(sf::Vector2f(GRID_SIZE, GRID_SIZE));
            rect.setFillColor(sf::Color::Red);
            rect.setOutlineThickness(0.f);
            rect.setPosition(i*GRID_SIZE, j*GRID_SIZE);
            window->draw(rect);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "FALLING SAND", sf::Style::Titlebar | sf::Style::Close);
    sf::CircleShape shape;
    shape.setRadius(40.f);
    shape.setPosition(100.f, 100.f);
    shape.setFillColor(sf::Color::Cyan);

    sf::Clock clock;
    sf::Time frame_time = sf::seconds(0.1f);

    std::array<int, N_COLS*N_ROWS> cells {};
    init_cells(cells);

    while (window.isOpen()) {
        clock.restart(); // set to 0 here

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // UPDATE LOOP
        init_cells(next_cells);
        std::array<int, N_COLS*N_ROWS> next_cells {};
        update_cells(cells, next_cells);
        cells = next_cells;

        // DRAW LOOP
        window.clear(sf::Color::White);
        draw_lines(&window);
        draw_cells(cells);
        window.display();

        // very basic frame rate manager
        sf::Time elapsed = clock.getElapsedTime();
        if(elapsed > frame_time) {
            printf("%.2f, %.2f: we're running too slowly!\n", elapsed.asSeconds(), frame_time.asSeconds());
        } else {
            while((elapsed = clock.getElapsedTime()) < frame_time); // block until
        }
        
    }
}
