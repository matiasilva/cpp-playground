#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp> 
#include <cmath>

#define WIDTH 800
#define HEIGHT 800
#define SLICES 10
#define N_VERTICES 50

sf::VertexArray create_arc(float start, float end, float radius, sf::Color color) {
    sf::VertexArray arc(sf::TriangleFan, N_VERTICES+1);
    arc[0].position = sf::Vector2f((float)WIDTH/2, (float)HEIGHT/2);
    float angle_incr = (end - start) / N_VERTICES;
    for (float i = start; i < end; i += angle_incr) {
    	float radians = M_PI/180 * i;
    	float x = cos(radians) * radius + WIDTH/2;
    	float y = sin(radians) * radius + HEIGHT/2;
    	arc[i].position = sf::Vector2f(x, y);
    	arc[i].color = color;
    }
    return arc;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "FALLING SAND", sf::Style::Titlebar | sf::Style::Close);

    sf::Clock clock;
    sf::Time frame_time = sf::seconds(0.1f);
    sf::Color colors[] = {sf::Color::Blue, sf::Color::Green};

  	while (window.isOpen()) {
        clock.restart(); // set to 0 here

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // UPDATE LOOP

        // DRAW LOOP
        window.clear(sf::Color::White);
        sf::VertexArray arc = create_arc(50, 80, 50, sf::Color::Black);
        window.draw(arc);
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