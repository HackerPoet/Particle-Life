#include "Universe.h"
#include "HSV.h"
#include <algorithm>
#include <iostream>
#include <iomanip>

static const float RADIUS = 5.0f;
static const float DIAMETER = 2.0f * RADIUS;
static const float R_SMOOTH = 2.0f;

Universe::Universe(size_t num_types, size_t num_particles, int width, int height) {
  //Initialize everything
  m_rand_gen.seed((unsigned int)time(0));
  SetPopulation(num_types, num_particles);
  SetSize(float(width), float(height));
  m_center_x = m_width * 0.5f;
  m_center_y = m_height * 0.5f;
  m_zoom = 1.0f;
  m_attract_mean = 0.0f;
  m_attract_std = 0.0f;
  m_minr_lower = 0.0f;
  m_minr_upper = 0.0f;
  m_maxr_lower = 0.0f;
  m_maxr_upper = 0.0f;
  m_friction = 0.0f;
  m_flat_force = false;
  m_wrap = false;
}

void Universe::ReSeed(float attract_mean, float attract_std, float minr_lower, float minr_upper,
                      float maxr_lower, float maxr_upper, float friction, bool flat_force) {
  m_attract_mean = attract_mean;
  m_attract_std = attract_std;
  m_minr_lower = minr_lower;
  m_minr_upper = minr_upper;
  m_maxr_lower = maxr_lower;
  m_maxr_upper = maxr_upper;
  m_friction = friction;
  m_flat_force = flat_force;
  SetRandomTypes();
  SetRandomParticles();
}

void Universe::SetPopulation(size_t num_types, size_t num_particles) {
  m_types.Resize(num_types);
  m_particles.resize(num_particles);
}

void Universe::SetRandomTypes() {
  std::normal_distribution<float>       rand_attr(m_attract_mean, m_attract_std);
  std::uniform_real_distribution<float> rand_minr(m_minr_lower, m_minr_upper);
  std::uniform_real_distribution<float> rand_maxr(m_maxr_lower, m_maxr_upper);
  for (size_t i = 0; i < m_types.Size(); ++i) {
    m_types.Color(i) = FromHSV(float(i) / m_types.Size(), 1.0f, float(i % 2)*0.5f + 0.5f);
    for (size_t j = 0; j < m_types.Size(); ++j) {
      if (i == j) {
        m_types.Attaract(i, j) = -std::abs(rand_attr(m_rand_gen));
        m_types.MinR(i, j) = DIAMETER;
      } else {
        m_types.Attaract(i, j) = rand_attr(m_rand_gen);
        m_types.MinR(i, j) = std::max(rand_minr(m_rand_gen), DIAMETER);
      }
      m_types.MaxR(i, j) = std::max(rand_maxr(m_rand_gen), m_types.MinR(i, j));

      //Keep radii symmetric
      m_types.MaxR(j, i) = m_types.MaxR(i, j);
      m_types.MinR(j, i) = m_types.MinR(i, j);
    }
  }
}

void Universe::SetRandomParticles() {
  std::uniform_int_distribution<int>    rand_type(0, int(m_types.Size() - 1));
  std::uniform_real_distribution<float> rand_uni(0.0f, 1.0f);
  std::normal_distribution<float>       rand_norm(0.0f, 1.0f);
  for (size_t i = 0; i < m_particles.size(); ++i) {
    Particle& p = m_particles[i];
    p.type = uint8_t(rand_type(m_rand_gen));
    p.x = (rand_uni(m_rand_gen)*0.5f + 0.25f) * m_width;
    p.y = (rand_uni(m_rand_gen)*0.5f + 0.25f) * m_height;
    p.vx = rand_norm(m_rand_gen) * 0.2f;
    p.vy = rand_norm(m_rand_gen) * 0.2f;
  }
}

void Universe::Step() {
  for (size_t i = 0; i < m_particles.size(); ++i) {
    //Current particle
    Particle& p = m_particles[i];

    //Interactions
    for (size_t j = 0; j < m_particles.size(); ++j) {
      //Other particle
      const Particle& q = m_particles[j];

      //Get deltas
      float dx = q.x - p.x;
      float dy = q.y - p.y;
      if (m_wrap) {
        if (dx > m_width*0.5f) {
          dx -= m_width;
        } else if (dx < -m_width*0.5f) {
          dx += m_width;
        }
        if (dy > m_height*0.5f) {
          dy -= m_height;
        } else if (dy < -m_height*0.5f) {
          dy += m_height;
        }
      }

      //Get distance squared
      const float r2 = dx*dx + dy*dy;
      const float minR = m_types.MinR(p.type, q.type);
      const float maxR = m_types.MaxR(p.type, q.type);

      if (r2 > maxR*maxR || r2 < 0.01f) {
        continue;
      }

      //Normalize displacement
      const float r = std::sqrt(r2);
      dx /= r;
      dy /= r;

      //Calculate force
      float f = 0.0f;
      if (r > minR) {
        if (m_flat_force) {
          f = m_types.Attaract(p.type, q.type);
        } else {
          const float numer = 2.0f * std::abs(r - 0.5f*(maxR + minR));
          const float denom = maxR - minR;
          f = m_types.Attaract(p.type, q.type) * (1.0f - numer / denom);
        }
      } else {
        f = R_SMOOTH*minR*(1.0f/(minR + R_SMOOTH) - 1.0f / (r + R_SMOOTH));
      }

      //Apply force
      p.vx += f * dx;
      p.vy += f * dy;
    }
  }

  //Update position
  for (size_t i = 0; i < m_particles.size(); ++i) {
    //Current particle
    Particle& p = m_particles[i];

    //Update position and velocity
    p.x += p.vx;
    p.y += p.vy;
    p.vx *= (1.0f - m_friction);
    p.vy *= (1.0f - m_friction);

    //Check for wall collisions
    if (m_wrap) {
      if (p.x < 0) {
        p.x += m_width;
      } else if (p.x >= m_width) {
        p.x -= m_width;
      }
      if (p.y < 0) {
        p.y += m_height;
      } else if (p.y >= m_height) {
        p.y -= m_height;
      }
    } else {
      if (p.x <= DIAMETER) {
        p.vx = -p.vx;
        p.x = DIAMETER;
      } else if (p.x >= m_width - DIAMETER) {
        p.vx = -p.vx;
        p.x = m_width - DIAMETER;
      }
      if (p.y <= DIAMETER) {
        p.vy = -p.vy;
        p.y = DIAMETER;
      } else if (p.y >= m_height - DIAMETER) {
        p.vy = -p.vy;
        p.y = m_height - DIAMETER;
      }
    }
  }
}

void Universe::Draw(sf::RenderWindow& window, float opacity) const {
  sf::CircleShape circle;
  circle.setRadius(RADIUS * m_zoom);
  circle.setOrigin(circle.getRadius(), circle.getRadius());
  for (size_t i = 0; i < m_particles.size(); ++i) {
    const Particle& p = m_particles[i];
    const float x = (p.x - m_center_x)*m_zoom + float(m_width/2);
    const float y = (p.y - m_center_y)*m_zoom + float(m_height/2);
    circle.setPosition(x, y);
    sf::Color col = m_types.Color(p.type);
    col.a = uint8_t(opacity * 255);
    circle.setFillColor(col);
    window.draw(circle);
  }
}


int Universe::GetIndex(int x, int y) const {
  float cx, cy;
  ToCenter(x, y, cx, cy);
  for (size_t i = 0; i < m_particles.size(); ++i) {
    const float dx = m_particles[i].x - cx;
    const float dy = m_particles[i].y - cy;
    if (dx*dx + dy*dy < RADIUS*RADIUS) {
      return int(i);
    }
  }
  return -1;
}

float Universe::GetParticleX(int index) const {
  return m_particles[index].x;
}

float Universe::GetParticleY(int index) const {
  return m_particles[index].y;
}

void Universe::ToCenter(int x, int y, float& cx, float& cy) const {
  cx = m_center_x + float(x - m_width / 2) / m_zoom;
  cy = m_center_y + float(y - m_height / 2) / m_zoom;
}

void Universe::Zoom(float cx, float cy, float zoom) {
  //Apply the zoom
  m_center_x = cx;
  m_center_y = cy;
  m_zoom = std::max(1.0f, zoom);

  //Clamp to make sure camera doesn't go out of bounds
  m_center_x = std::min(m_center_x, float(m_width) * (1.0f - 0.5f/m_zoom));
  m_center_y = std::min(m_center_y, float(m_height) * (1.0f - 0.5f / m_zoom));
  m_center_x = std::max(m_center_x, float(m_width) * (0.5f / m_zoom));
  m_center_y = std::max(m_center_y, float(m_height) * (0.5f / m_zoom));
}

void Universe::PrintParams() const {
  std::cout << "\nAttract:\n";
  for (size_t i = 0; i < m_types.Size(); ++i) {
    for (size_t j = 0; j < m_types.Size(); ++j) {
      std::cout << std::fixed << std::setw(8) << std::setprecision(4) << m_types.Attaract(i, j) << "  ";
    }
    std::cout << "\n";
  }
  std::cout << "MinR:\n";
  for (size_t i = 0; i < m_types.Size(); ++i) {
    for (size_t j = 0; j < m_types.Size(); ++j) {
      std::cout << std::fixed << std::setw(8) << std::setprecision(4) << m_types.MinR(i, j) << "  ";
    }
    std::cout << "\n";
  }
  std::cout << "MaxR:\n";
  for (size_t i = 0; i < m_types.Size(); ++i) {
    for (size_t j = 0; j < m_types.Size(); ++j) {
      std::cout << std::fixed << std::setw(8) << std::setprecision(4) << m_types.MaxR(i, j) << "  ";
    }
    std::cout << "\n";
  }
}
