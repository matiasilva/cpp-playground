#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp> 
#include <cmath>

#define WIDTH 800
#define HEIGHT 800
#define SLICES 30
#define N_VERTICES 200

sf::VertexArray create_arc(float start, float end, float radius, sf::Color color) {
    sf::VertexArray arc(sf::TriangleFan, N_VERTICES+1);
    arc[0].position = sf::Vector2f((float)WIDTH/2, (float)HEIGHT/2);
    arc[0].color = color;
    float angle_incr = (end - start) / N_VERTICES;
    for (float i = 1; i < N_VERTICES+1; i++) {
    	float radians = M_PI/180 * (i*angle_incr + start);
    	float x = cos(radians) * radius + WIDTH/2;
    	float y = sin(radians) * radius + HEIGHT/2;
    	arc[i].position = sf::Vector2f(x, y);
    	arc[i].color = color;
    }
    return arc;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "RAINBOW WHEEL", sf::Style::Titlebar | sf::Style::Close);

    sf::Clock clock;
    sf::Time frame_time = sf::seconds(0.05f);
    sf::Color colors[] = {sf::Color(0xff0000ff), sf::Color(0xffa500ff), sf::Color(0xffff00ff), sf::Color(0x008000ff), sf::Color(0x0000ffff), sf::Color(0x4b0082ff), sf::Color(0xee82eeff)};
    int loop_counter = 0;

  	while (window.isOpen()) {
        clock.restart(); // set to 0 here

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // UPDATE LOOP
        loop_counter++;

        // DRAW LOOP
        window.clear(sf::Color::White);
        for (int i = 0; i < SLICES; i++) {
        	float slice_width = (float) 360 / SLICES;
        	float start = slice_width*i;
        	int color_select = i + (loop_counter >> 1);
	        sf::VertexArray arc = create_arc(start, start+slice_width+0.01*slice_width, 600, colors[color_select % 7]);
            window.draw(arc);
        }
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