#pragma once
#include "Particles.h"
#include <random>

class Universe {
public:
  Universe(size_t num_types, size_t num_particles, int width, int height);

  void ReSeed(float attract_mean, float attract_std, float minr_lower, float minr_upper,
    float maxr_lower, float maxr_upper, float friction, bool flat_force);

  void SetPopulation(size_t num_types, size_t num_particles);
  void SetSize(float width, float height) { m_width = width; m_height = height; }
  void SetRandomTypes();
  void SetRandomParticles();
  void ToggleWrap() { m_wrap = !m_wrap; }

  void Step();
  void Draw(sf::RenderWindow& window, float opacity) const;
  void Zoom(float cx, float cy, float zoom);

  int GetIndex(int x, int y) const;
  float GetParticleX(int index) const;
  float GetParticleY(int index) const;
  void ToCenter(int x, int y, float& cx, float& cy) const;

  void PrintParams() const;

private:
  float m_center_x;
  float m_center_y;
  float m_zoom;
  bool m_wrap;

  std::vector<Particle> m_particles;
  ParticleTypes m_types;
  float m_width;
  float m_height;

  std::mt19937 m_rand_gen;

  //Random settings
  float m_attract_mean;
  float m_attract_std;
  float m_minr_lower;
  float m_minr_upper;
  float m_maxr_lower;
  float m_maxr_upper;
  float m_friction;
  bool m_flat_force;
};
