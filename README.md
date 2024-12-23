# C++ Playground

As the name indicates, this is a place for me to play around with C++. Historically, I've used [Processing](https://processing.org/) and [p5.js](https://p5js.org/reference/) for this sort of stuff, but I wanted to sharpen my C/C++ skills.

## Raylib

I used C++ bindings for [Raylib](https://github.com/RobLoach/raylib-cpp) to produce a moderately advanced fireworks
simulation, making use of Euler integration to approximate particle behavior.

<img src="images/fireworks.gif">

To try out web support, I used empscripten to compile the C++ to WASM. The
[out-of-the-box results](https://matiasilva.github.io/cpp-playground/) were promising.

## SFML

### Falling sand simulation

<img src="images/falling-sand.png" width="150">

### Rotating rainbow wheel

<img src="images/rainbow.png" width="150">
