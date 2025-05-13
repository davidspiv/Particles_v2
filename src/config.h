#pragma once

#include <string>

std::string const WINDOW_TITLE = "Particle Demo";
constexpr size_t WINDOW_WIDTH = 1920;
constexpr size_t WINDOW_HEIGHT = 1080;
constexpr size_t TARGET_FPS = 120; // measure frame limit appears to reach 1/2 this val?

constexpr size_t MODEL_VARIATIONS = 50;
constexpr size_t PARTICLES_PER_SECOND = 2000;
constexpr int SECONDS_PER_RAINBOW_CYCLE = 3;
