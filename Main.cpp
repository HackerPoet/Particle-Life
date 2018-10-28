#include "Universe.h"
#include <SFML/Graphics.hpp>
#include <iostream>

static const int window_w = 1600;
static const int window_h = 900;
static const int steps_per_frame_normal = 10;

int main(int argc, char *argv[]) {
  std::cout << "=========================================================" << std::endl;
  std::cout << std::endl;
  std::cout << "               Welcome to Particle Life" << std::endl;
  std::cout << std::endl;
  std::cout << "  This is a particle-based game of life simulation based" << std::endl;
  std::cout << "on random attraction and repulsion between all particle" << std::endl;
  std::cout << "classes.  For more details about how this works and other" << std::endl;
  std::cout << "fun projects, check out my YouTube channel 'CodeParade'." << std::endl;
  std::cout << std::endl;
  std::cout << "=========================================================" << std::endl;
  std::cout << std::endl;
  std::cout << "Controls:" << std::endl;
  std::cout << "         'B' - Randomize (Balanced)" << std::endl;
  std::cout << "         'C' - Randomize (Chaos)" << std::endl;
  std::cout << "         'D' - Randomize (Diversity)" << std::endl;
  std::cout << "         'F' - Randomize (Frictionless)" << std::endl;
  std::cout << "         'G' - Randomize (Gliders)" << std::endl;
  std::cout << "         'H' - Randomize (Homogeneity)" << std::endl;
  std::cout << "         'L' - Randomize (Large Clusters)" << std::endl;
  std::cout << "         'M' - Randomize (Medium Clusters)" << std::endl;
  std::cout << "         'Q' - Randomize (Quiescence)" << std::endl;
  std::cout << "         'S' - Randomize (Small Clusters)" << std::endl;
  std::cout << "         'W' - Toggle Wrap-Around" << std::endl;
  std::cout << "       Enter - Keep rules, but re-seed particles" << std::endl;
  std::cout << "       Space - Hold for slow-motion" << std::endl;
  std::cout << "         Tab - Print current parameters to console" << std::endl;
  std::cout << "  Left Click - Click a particle to follow it" << std::endl;
  std::cout << " Right Click - Click anywhere to unfollow particle" << std::endl;
  std::cout << "Scroll Wheel - Zoom in/out" << std::endl;
  std::cout << std::endl;
  system("pause");

  //Create the universe of particles
  Universe universe(9, 400, window_w, window_h);
  universe.ReSeed(-0.02f, 0.06f, 0.0f, 20.0f, 20.0f, 70.0f, 0.05f, false);

  //Camera settings
  float cam_x = float(window_w/2);
  float cam_y = float(window_h/2);
  float cam_zoom = 1.0f;
  float cam_x_dest = cam_x;
  float cam_y_dest = cam_y;
  float cam_zoom_dest = cam_zoom;
  int32_t last_scroll_time = 0;
  int track_index = -1;
  int steps_per_frame = steps_per_frame_normal;

  //GL settings
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion = 3;
  settings.minorVersion = 0;

  //Create the window
  const sf::VideoMode screenSize = sf::VideoMode(window_w, window_h, 24);
  sf::RenderWindow window(screenSize, "Particles", sf::Style::Resize | sf::Style::Close, settings);
  window.setFramerateLimit(30);
  window.setVerticalSyncEnabled(true);
  window.setActive(false);
  window.requestFocus();
  sf::Clock clock;

  //Main Loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        break;
      } else if (event.type == sf::Event::KeyPressed) {
        const sf::Keyboard::Key keycode = event.key.code;
        if (keycode == sf::Keyboard::Escape) {
          window.close();
          break;
        } else if (keycode == sf::Keyboard::B) { //Balanced
          universe.SetPopulation(9, 400);
          universe.ReSeed(-0.02f, 0.06f, 0.0f, 20.0f, 20.0f, 70.0f, 0.05f, false);
        } else if (keycode == sf::Keyboard::C) { //Chaos
          universe.SetPopulation(6, 400);
          universe.ReSeed(0.02f, 0.04f, 0.0f, 30.0f, 30.0f, 100.0f, 0.01f, false);
        } else if (keycode == sf::Keyboard::D) { //Diversity
          universe.SetPopulation(12, 400);
          universe.ReSeed(-0.01f, 0.04f, 0.0f, 20.0f, 10.0f, 60.0f, 0.05f, true);
        } else if (keycode == sf::Keyboard::F) { //Frictionless
          universe.SetPopulation(6, 300);
          universe.ReSeed(0.01f, 0.005f, 10.0f, 10.0f, 10.0f, 60.0f, 0.0f, true);
        } else if (keycode == sf::Keyboard::G) { //Gliders
          universe.SetPopulation(6, 400);
          universe.ReSeed(0.0f, 0.06f, 0.0f, 20.0f, 10.0f, 50.0f, 0.1f, true);
        } else if (keycode == sf::Keyboard::H) { //Homogeneity
          universe.SetPopulation(4, 400);
          universe.ReSeed(0.0f, 0.04f, 10.0f, 10.0f, 10.0f, 80.0f, 0.05f, true);
        } else if (keycode == sf::Keyboard::L) { //Large Clusters
          universe.SetPopulation(6, 400);
          universe.ReSeed(0.025f, 0.02f, 0.0f, 30.0f, 30.0f, 100.0f, 0.2f, false);
        } else if (keycode == sf::Keyboard::M) { //Medium Clusters
          universe.SetPopulation(6, 400);
          universe.ReSeed(0.02f, 0.05f, 0.0f, 20.0f, 20.0f, 50.0f, 0.05f, false);
        } else if (keycode == sf::Keyboard::Q) { //Quiescence
          universe.SetPopulation(6, 300);
          universe.ReSeed(-0.02f, 0.1f, 10.0f, 20.0f, 20.0f, 60.0f, 0.2f, false);
        } else if (keycode == sf::Keyboard::S) { //Small Clusters
          universe.SetPopulation(6, 600);
          universe.ReSeed(-0.005f, 0.01f, 10.0f, 10.0f, 20.0f, 50.0f, 0.01f, false);
        } else if (keycode == sf::Keyboard::W) {
          universe.ToggleWrap();
        } else if (keycode == sf::Keyboard::Enter) {
          universe.SetRandomParticles();
        } else if (keycode == sf::Keyboard::Tab) {
          universe.PrintParams();
        } else if (keycode == sf::Keyboard::Space) {
          steps_per_frame = 1;
        }
      } else if (event.type == sf::Event::KeyReleased) {
        const sf::Keyboard::Key keycode = event.key.code;
        if (keycode == sf::Keyboard::Space) {
          steps_per_frame = steps_per_frame_normal;
        }
      } else if (event.type == sf::Event::MouseWheelMoved) {
        cam_zoom_dest *= std::pow(1.1f, event.mouseWheel.delta);
        cam_zoom_dest = std::max(std::min(cam_zoom_dest, 10.0f), 1.0f);
        const int32_t cur_time = clock.getElapsedTime().asMilliseconds();
        if (cur_time - last_scroll_time > 300) {
          //Only update position if scroll just started
          const sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
          universe.ToCenter(mouse_pos.x, mouse_pos.y, cam_x_dest, cam_y_dest);
        }
        last_scroll_time = cur_time;
      } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          track_index = universe.GetIndex(event.mouseButton.x, event.mouseButton.y);
        } else if (event.mouseButton.button == sf::Mouse::Right) {
          track_index = -1;
        }
      }
    }

    //Apply zoom
    if (track_index >= 0) {
      cam_x_dest = universe.GetParticleX(track_index);
      cam_y_dest = universe.GetParticleY(track_index);
    }
    cam_x = cam_x*0.9f + cam_x_dest*0.1f;
    cam_y = cam_y*0.9f + cam_y_dest*0.1f;
    cam_zoom = cam_zoom*0.8f + cam_zoom_dest*0.2f;
    universe.Zoom(cam_x, cam_y, cam_zoom);

    //Apply physics and draw
    window.clear();
    for (int i = 0; i < steps_per_frame; ++i) {
      const float opacity = float(i + 1) / float(steps_per_frame);
      universe.Step();
      universe.Draw(window, opacity);
    }

    //Flip the screen buffer
    window.display();
  }

  return 0;
}
