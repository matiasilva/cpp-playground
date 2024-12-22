#include "raylib-cpp.hpp"
#include <random>
#include <raylib.h>

constexpr int screenWidth = 1200;
constexpr int screenHeight = 600;
const raylib::Vector2 gravity(0, 0.5);
std::mt19937 gen;
std::uniform_real_distribution<> dis(0.0f, 1.0f);
std::normal_distribution<> disn(0.0, 1.0);

raylib::Color GetRandomColor();

class Particle {
public:
  raylib::Vector2 pos;
  raylib::Vector2 vel;
  raylib::Vector2 acc;
  raylib::Color color;

  float mass;
  float alphaStep = 0.98 + dis(gen) * 0.01;
  float alpha = 1.0f;
  float drag;
  bool isAlive = true;
  bool isFading = false;

  Particle(int x, int y)
      : pos(x, y), vel(0, -10 - dis(gen) * 28), mass(5), drag(0.95),
        color(GetRandomColor()) {};

  Particle(int x, int y, float heading, raylib::Color color, float intensity)
      : pos(x, y), vel(cos(heading), sin(heading)), mass(2), color(color),
        drag(0.96) {
    vel *= dis(gen) * intensity;
  };

  void applyForce(const raylib::Vector2 &f) { acc += (f / mass); }

  void update() {
    vel += acc;
    vel *= drag;
    pos += vel;
    acc *= 0;
    if (isFading) {
      if (alpha <= 0.15)
        isAlive = false;
      alpha *= alphaStep;
      // mass *= alphaStep;
    }
  };

  void render() const {
    DrawCircle(pos.GetX(), pos.GetY(), mass, color.Fade(alpha));
  };
};

class Firework {
public:
  std::vector<Particle> particles;
  bool exploded = false;

  Firework(int x) : particles(1, Particle(x, screenHeight)) {};

  void update() {
    if (!exploded && root().vel.GetY() >= 0)
      explode();
    for (auto &p : particles) {
      p.applyForce(gravity);
      p.update();
    }
    std::erase_if(particles, [](auto &p) { return !p.isAlive; });
  }

  void render() const {
    for (const auto &p : particles)
      p.render();
  }

  void explode() {
    exploded = true;
    const int numParticles = GetRandomValue(250, 400);
    const float posX = root().pos.GetX(), posY = root().pos.GetY();
    const raylib::Color col = root().color;
    root().isAlive = false;
    for (int i = 0; i < numParticles; ++i) {
      float angle = (PI / 180) * GetRandomValue(0, 359);
      Particle p(posX, posY, angle, col, GetRandomValue(2, 10));
      p.isFading = true;
      particles.push_back(p);
    }
  }

  Particle &root() { return particles.front(); } // dodgy
};

raylib::Color GetRandomColor() {
  using raylib::Color;
  std::vector<Color (*)()> colors = {
      Color::Magenta, Color::White,  Color::Blue, Color::Green,
      Color::Red,     Color::Yellow, Color::Gold, Color::Orange};
  int randomIndex = GetRandomValue(0, colors.size() - 1);
  return colors[randomIndex]();
}

std::vector<Firework> fireworks;

int main() {
  raylib::Window window(screenWidth, screenHeight, "fireworks");
  SetTargetFPS(60);
  window.ClearBackground(RAYWHITE);
  raylib::Rectangle bg(0, 0, screenWidth, screenHeight);

  while (!window.ShouldClose()) {
    BeginDrawing();
    {
      bg.Draw(Fade(BLACK, 0.30));
      if (GetRandomValue(0, 49) == 49)
        fireworks.push_back(
            Firework(GetRandomValue(0.1 * screenWidth, 0.9 * screenWidth - 1)));
      for (auto &f : fireworks) {
        f.render();
        f.update();
      }
      std::erase_if(fireworks, [](auto &x) { return x.particles.empty(); });
    }
    EndDrawing();
  }
  return 0;
}
