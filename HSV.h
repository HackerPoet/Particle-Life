#pragma once
#include <SFML/Graphics.hpp>

inline sf::Color FromHSV(float h, float s, float v) {
  //All input ranges expected to be 0-1
  const int i = int(h * 6);
  const float f = h * 6 - i;
  const float p = v * (1 - s);
  const float q = v * (1 - f * s);
  const float t = v * (1 - (1 - f) * s);

  //Special cases
  float r, g, b;
  switch (i % 6) {
  case 0: r = v, g = t, b = p; break;
  case 1: r = q, g = v, b = p; break;
  case 2: r = p, g = v, b = t; break;
  case 3: r = p, g = q, b = v; break;
  case 4: r = t, g = p, b = v; break;
  case 5: r = v, g = p, b = q; break;
  }

  //Convert color range from 0-1 to 0-255
  return sf::Color(uint8_t(r * 255), uint8_t(g * 255), uint8_t(b * 255));
}