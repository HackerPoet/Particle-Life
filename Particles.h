#pragma once
#include <SFML/Graphics.hpp>
#include <cstdint>
#include <vector>

struct Particle {
  float x, y;
  float vx, vy;
  uint8_t type;
};

class ParticleTypes {
public:
  void Resize(size_t size) {
    m_col.resize(size);
    m_attract.resize(size * size);
    m_minR.resize(size * size);
    m_maxR.resize(size * size);
  }

  size_t Size() const { return m_col.size(); }
  const sf::Color& Color(size_t i) const { return m_col[i]; }
  sf::Color& Color(size_t i) { return m_col[i]; }
  float Attaract(size_t i, size_t j) const { return m_attract[i*m_col.size() + j]; }
  float& Attaract(size_t i, size_t j) { return m_attract[i*m_col.size() + j]; }
  float MinR(size_t i, size_t j) const { return m_minR[i*m_col.size() + j]; }
  float& MinR(size_t i, size_t j) { return m_minR[i*m_col.size() + j]; }
  float MaxR(size_t i, size_t j) const { return m_maxR[i*m_col.size() + j]; }
  float& MaxR(size_t i, size_t j) { return m_maxR[i*m_col.size() + j]; }

private:
  std::vector<sf::Color> m_col;
  std::vector<float> m_attract;
  std::vector<float> m_minR;
  std::vector<float> m_maxR;
};
