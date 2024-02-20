#include <SFML/Graphics.hpp>
#include <cmath>
#include <array>

#define WIDTH 800
#define HEIGHT 800
#define GRID 0.005
#define N_ROWS ((int)(1/GRID) + 1) // extra row for floor
#define N_COLS ((int)(1/GRID))

const float GRID_SIZE = GRID*WIDTH;
const int N_CELLS = N_ROWS*N_COLS; // computed values as consts

void draw_lines(sf::RenderWindow& window) {
    for (int row = GRID_SIZE; row < WIDTH; row+=GRID_SIZE) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, row), sf::Color::Black),
            sf::Vertex(sf::Vector2f(WIDTH, row), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }
    for (int col = GRID_SIZE; col < HEIGHT; col+=GRID_SIZE) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(col, 0), sf::Color::Black),
            sf::Vertex(sf::Vector2f(col, HEIGHT), sf::Color::Black)
        };
        window.draw(line, 2, sf::Lines);
    }
}

void init_cells(std::array<int, N_CELLS>& grid) {
    for (int i = 0; i < N_COLS; i++) {
            int index = i + (N_ROWS - 1) * N_COLS; // last row
            grid[index] = 1;
        }
    }


void update_cells(const std::array<int, N_CELLS>& grid, std::array<int, N_CELLS>& next_grid) {
    /* ALGORITHM
        1. check if we are 1 and check neighbor below if 1
            a) if 1, don't move
            b) if 0, move into cell below
    */
    for (int i = 0; i < N_ROWS-1; i++) {
        for (int j = 0; j < N_COLS; j++) {
            int index = j + i * N_COLS; // 1D array to 2D
            int index_below = j + (i+1) * N_COLS;
            bool is_active = grid[index] == 1;
            bool below_active = grid[index_below] == 1;
            if (is_active) {
                if(below_active) {
                    next_grid[index] = 1;
                } else {
                    next_grid[index_below] = 1;
                }
            }

        }
    }
}

void draw_cells(sf::RenderWindow& window, const std::array<int, N_CELLS>& grid) {
    for (int i = 0; i < N_ROWS; i++) {
        for (int j = 0; j < N_COLS; j++) {
            int index = j + i * N_COLS; // 1D array to 2D
            bool is_active = grid[index] == 1;

            sf::RectangleShape rect(sf::Vector2f(GRID_SIZE, GRID_SIZE));
            //rect.setOutlineThickness(1.f);
            //rect.setOutlineColor(sf::Color::Black);
            rect.setPosition(j*GRID_SIZE, i*GRID_SIZE);

            if (is_active) {
                rect.setFillColor(sf::Color::Red);
            } else {
                rect.setFillColor(sf::Color::White);
            }
            window.draw(rect);
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
    sf::Time frame_time = sf::seconds(0.025f);

    std::array<int, N_CELLS> cells {};
    init_cells(cells);

    while (window.isOpen()) {
        clock.restart(); // set to 0 here

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // UPDATE LOOP
        std::array<int, N_CELLS> next_cells {};
        init_cells(next_cells);
        update_cells(cells, next_cells);
        cells = next_cells; // copy array

        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        bool within_x_bounds = mouse_pos.x < WIDTH && mouse_pos.x > 0;
        bool within_y_bounds = mouse_pos.y < HEIGHT && mouse_pos.y > 0;
        if (within_x_bounds && within_y_bounds) { 
            int col = (int)floor(mouse_pos.x / GRID_SIZE);
            int row = (int)floor(mouse_pos.y / GRID_SIZE);
            int index = col + row * N_COLS;
            bool is_free = cells[index] == 0;
            bool below_free = cells[index+N_COLS] == 0;
            if (is_free && below_free) {
                cells[index] = 1;                
            } else if (!is_free) {
                // check if top of column is free
                for (int i = row; i >= 0; i--) {
                    int check_free_index = col + i * N_COLS;
                    if (!(cells[check_free_index] == 1)) {
                        cells[check_free_index] = 1; // once found next free cell in col, mark taken
                        break;
                    }
                }
            }
        }

        // DRAW LOOP
        window.clear(sf::Color::White);
        //draw_lines(window);
        draw_cells(window, cells);
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
