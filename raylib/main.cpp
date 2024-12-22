#include "raylib-cpp.hpp"
#include <raylib.h>
#include <vector>

constexpr int screenWidth = 800;
constexpr int screenHeight = 800;
const raylib::Vector2 gravity(0, 2);

raylib::Color GetRandomColor();

class Particle {
public:
  raylib::Vector2 pos;
  raylib::Vector2 vel;
  raylib::Vector2 acc;
  float mass;
  raylib::Color color;

  Particle(int x, int y) : pos(x, y), vel(0, -16), color(GetRandomColor()) {
    mass = 1.5;
  };

  void applyForce(const raylib::Vector2 &f) { acc += (f / mass); }

  void update() {
    vel += acc;
    pos += vel;
    acc *= 0;
  };

  void render() const { DrawCircle(pos.GetX(), pos.GetY(), 4, color); };
};

class Firework {
public:
  std::vector<Particle> particles;
  Particle &root = particles[0];

  Firework(int x) : particles(1, Particle(x, screenHeight)) {};

  void update() {
    for (auto &p : particles) {
      p.update();
    }
    // std::erase(particles, 3);
  }

  void render() {
    for (const auto &p : particles) {
      p.render();
    }
  }
};

raylib::Color GetRandomColor() {
  using raylib::Color;
  std::vector<Color (*)()> colors = {
      Color::Black, Color::Magenta, Color::White,  Color::Blue,  Color::Green,
      Color::Red,   Color::Yellow,  Color::Purple, Color::Orange};
  int randomIndex = GetRandomValue(0, colors.size() - 1);
  return colors[randomIndex]();
}

int main() {
  raylib::Window window(screenWidth, screenHeight, "fireworks");
  SetTargetFPS(60);

  Particle p(screenWidth / 2, screenHeight / 2);
  Firework f(GetRandomValue(0, screenWidth - 1));

  while (!window.ShouldClose()) { // Detect window close button or ESC key
    BeginDrawing();
    {
      window.ClearBackground(RAYWHITE);

      f.render();
      f.update();
    }
    EndDrawing();
    //----------------------------------------------------------------------------------
  }
  return 0;
}
