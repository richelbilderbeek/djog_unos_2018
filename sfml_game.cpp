// Always include the header of the unit first
#include "sfml_game.h"
#include "agent.h"
#include "agent_type.h"
#include "sfml_resources.h"
#include <QFile>
#include <cassert>
#include <cmath>
#include <iostream>

sfml_game::sfml_game(const int window_width, const int window_height,
                     const sfml_game_delegate &delegate)
    : m_background_music{sfml_resources::get().get_background_music()},
      m_delegate{delegate},
      m_window(sf::VideoMode(static_cast<unsigned int>(window_width),
                             static_cast<unsigned int>(window_height)),
               "Nature Zen"),
      m_font{} { // Set up music
  m_background_music.setLoop(true);
  m_background_music.play();
  // Set up window, start location to the center
  const int window_x =
      static_cast<int>(sf::VideoMode::getDesktopMode().width / 2) -
      window_width / 2;
  const int window_y =
      static_cast<int>(sf::VideoMode::getDesktopMode().height / 2) -
      window_height / 2;
  m_window.setPosition(sf::Vector2i(window_x, window_y));
  m_screen_center = Vector2i(window_width / 2, window_height / 2);
  // Set up text
  setup_text();
}

sfml_game::~sfml_game() { m_background_music.stop(); }

void sfml_game::close() { m_window.close(); }

// TODO: Simplify this function
void sfml_game::display() {         //!OCLINT indeed long, must be made shorter
  m_window.clear(sf::Color::Black); // Clear the window with black color

  if (m_game_state == Playing) {
    // Display all tiles
    for (const tile &t : m_game.get_tiles()) {
      sf::RectangleShape sfml_tile(
          sf::Vector2f(static_cast<float>(t.get_width()),
                       static_cast<float>(t.get_height())));
      // If the camera moves to right/bottom, tiles move relatively
      // left/downwards
      const float screen_x{static_cast<float>(t.get_x() - m_camera_x)};
      const float screen_y{static_cast<float>(t.get_y() - m_camera_y)};
      sfml_tile.setPosition(screen_x, screen_y);
      color_tile_shape(sfml_tile, t);
      m_window.draw(sfml_tile);
      // Draw agents
      for (const agent &a : t.get_agents()) {
        sf::Sprite sprite;
        sprite.setTexture(sfml_resources::get().get_cow_texture());
        sprite.setPosition(screen_x + static_cast<float>(a.get_x()),
                           screen_y + static_cast<float>(a.get_y()));
        // sprite.setScale(10,10);
        m_window.draw(sprite);
      }
    }
    sf::Text(sf::String(std::to_string(m_game.get_score())), m_font, 30);
  }
  /////sf::Text(sf::String(std::to_string(m_game.get_score())), m_font, 30);

  sf::Text text(sf::String(std::to_string(m_game.get_score())), m_font, 30);
  text.setPosition(m_window.getSize().x - 80, 10);
  text.setStyle(Text::Bold);
  m_window.draw(text);
  if (m_game_state == TitleScreen) {
      Texture texture = Texture();
      texture.loadFromFile("title_screen_background.PNG");
      Sprite sprite = Sprite();
      sprite.setTexture(texture);
      m_window.draw(sprite);
    m_window.draw(titleScreenText);
    if (space_pressed) {
      reset_input();

      m_game_state = MenuScreen;
    }
  } else if (m_game_state == MenuScreen) {
    m_window.draw(mainMenuScreenText);
    if (space_pressed) {
      reset_input();
      m_game_state = AboutScreen;
    }
  } else if (m_game_state == AboutScreen) {
    m_window.draw(aboutScreenText);
    if (space_pressed) {
      reset_input();
      m_game_state = Playing;
    }
  }
  /////m_window.draw(text);
  load_game_state();
  //  m_window.draw(text);
  m_window.display(); // Put everything on the screen
}

void sfml_game::load_game_state() {
  switch (m_game_state) {
  case TitleScreen:
    m_window.draw(titleScreenText);
    return;
  case MenuScreen:
    m_window.draw(mainMenuScreenText);
    return;
  case AboutScreen:
    m_window.draw(aboutScreenText);
    return;
  default:
    return;
  }
}

void sfml_game::change_game_state() {
  switch (m_game_state) {
  case TitleScreen:
    reset_input();
    m_game_state = MenuScreen;
    return;
  case MenuScreen:
    reset_input();
    m_game_state = AboutScreen;
    return;
  case AboutScreen:
    reset_input();
    m_game_state = Playing;
    return;
  default:
    return;
  }
}

void sfml_game::exec() {
  while (m_window.isOpen()) {
    process_input();
    process_events();
    display();
  }
}

void sfml_game::move_camera(sf::Vector2f offset) {
  // Dont move the camera in the menu
  if (m_game_state != Playing)
    return;
  m_camera_x += static_cast<double>(offset.x);
  m_camera_y += static_cast<double>(offset.y);
}

void sfml_game::process_events() {
  if ((115.0 / tile_speed != std::abs(std::floor(115.0 / tile_speed)) ||
       115.0 / tile_speed != std::abs(std::ceil(115.0 / tile_speed))) ||
      tile_speed > 115.0) {
    throw std::runtime_error("The set tile speed is not usable");
  }

  if (m_selected.empty()) {
    confirm_move();
  } else {
    follow_tile();
  }

  exec_tile_move(m_selected);

  manage_timer();

  m_delegate.do_actions(*this);
  ++m_n_displayed;
}

void sfml_game::confirm_move() {
  if (movecam_r == true)
    move_camera(sf::Vector2f(0.5, 0));
  if (movecam_l == true)
    move_camera(sf::Vector2f(-0.5, 0));
  if (movecam_u == true)
    move_camera(sf::Vector2f(0, -0.5));
  if (movecam_d == true)
    move_camera(sf::Vector2f(0, 0.5));
}

void sfml_game::follow_tile() {
  tile &t = getTileById(m_selected);
  m_camera_x = t.get_x() + (t.get_width() / 2) - m_screen_center.x;
  m_camera_y = t.get_y() + (t.get_height() / 2) - m_screen_center.y;
}

void sfml_game::manage_timer() {
  if (m_timer > 0) {
    --m_timer;
  } else {
    m_selected.clear();
    if (!m_temp_id.empty())
      m_selected.push_back(m_temp_id[0]);
  }
}

void sfml_game::exec_tile_move(std::vector<int> selected) {
  if (!selected.empty()) {
    tile &temp_tile = getTileById(selected);
    if (m_timer > 0) {
      temp_tile.move();
    } else {
      temp_tile.set_dx(0);
      temp_tile.set_dy(0);
    }
  }
}

void sfml_game::process_event(const sf::Event &event) {
  switch (event.type) {
  case sf::Event::Closed:
    close();
    break;

  case sf::Event::KeyPressed:
    process_keyboard_input(event);
    break;

  case sf::Event::MouseButtonPressed:
    process_mouse_input(event);
    break;

  case sf::Event::KeyReleased:
    process_keyboard_input(event);
    break;

  default:
    // Do nothing by default
    break;
  }
}

void sfml_game::process_input() {
  // check all the window's events that were triggered since the last iteration
  // of the loop
  sf::Event event;
  while (m_window.pollEvent(event)) {
    process_event(event);
  }
}

void sfml_game::process_keyboard_input(const sf::Event &event) {
  // Only keyboard events
  assert(event.type == sf::Event::KeyPressed ||
         event.type == sf::Event::KeyReleased);

  if (event.type == sf::Event::KeyPressed) {
    check_change_game_state(event);
    arrows(true, event);
    if (!m_selected.empty())
      tile_movement(true, event, getTileById(m_selected));
    if (m_timer > 0)
      tile_movement(false, event, getTileById(m_selected));
  } else {
    arrows(false, event);
  }
}

void sfml_game::check_change_game_state(const sf::Event &event) {
  if (event.key.code == sf::Keyboard::Space)
    change_game_state();
  if (m_game_state == Playing && event.key.code == sf::Keyboard::Escape)
    m_game_state = MenuScreen;
}

void sfml_game::move_selected_tile_randomly() {
  if (m_selected.empty())
    return;
  const int id = m_selected[0];
  this->getTileById({id}).set_dx(5);
}

void sfml_game::reset_input() {
  m_camera_x = 0;
  m_camera_y = 0;
  movecam_r = false;
  movecam_l = false;
  movecam_u = false;
  movecam_d = false;
}
void sfml_game::process_mouse_input(const sf::Event &event) {
  // Only mouse input
  assert(event.type == sf::Event::MouseButtonPressed);

  if (event.mouseButton.button == sf::Mouse::Left) {
    std::vector<tile> game_tiles = m_game.get_tiles();
    for (unsigned i = 0; i < game_tiles.size(); i++) {
      if (game_tiles.at(i).tile_contains(
              sf::Mouse::getPosition(m_window).x + m_camera_x,
              sf::Mouse::getPosition(m_window).y + m_camera_y)) {
        m_temp_id.clear();
        m_temp_id.push_back(game_tiles.at(i).get_id());
        clicked_tile = true;
      }
    }
    if (clicked_tile == false) {
      m_temp_id.clear();
    }
    clicked_tile = false;
  }
}

void sfml_game::select_random_tile() {
  const auto &tiles = m_game.get_tiles();
  const int i = std::rand() % tiles.size();
  const int id = tiles[i].get_id();
  m_selected.resize(1);
  m_selected[0] = id;
}

void sfml_game::stop_music() { m_background_music.stop(); }

// NOTE Changed it to show_title (was show_menu)
void sfml_game::show_title() { m_game_state = TitleScreen; }

void sfml_game::arrows(bool b, const sf::Event &event) {
  if (event.key.code == sf::Keyboard::D)
    movecam_r = b;
  if (event.key.code == sf::Keyboard::A)
    movecam_l = b;
  if (event.key.code == sf::Keyboard::W)
    movecam_u = b;
  if (event.key.code == sf::Keyboard::S)
    movecam_d = b;
}

void sfml_game::tile_movement(bool b, const sf::Event &event, tile &t) {
  if (m_timer == 0) {
    if (b == true) {
      tile_move_ctrl(event, t);
      m_timer += (1 / tile_speed) * 115;
    } else {
      t.set_dx(0);
      t.set_dy(0);
    }
  }
}

void sfml_game::tile_move_ctrl(const sf::Event &event, tile &t) {
  if (event.key.code == sf::Keyboard::D && !will_colide(2, t))
    t.set_dx(tile_speed);
  if (event.key.code == sf::Keyboard::A && !will_colide(4, t))
    t.set_dx(-tile_speed);
  if (event.key.code == sf::Keyboard::W && !will_colide(1, t))
    t.set_dy(-tile_speed);
  if (event.key.code == sf::Keyboard::S && !will_colide(3, t))
    t.set_dy(tile_speed);
}

int sfml_game::vectortoint(std::vector<int> v) {
  reverse(v.begin(), v.end());
  int decimal = 1;
  int total = 0;
  for (auto &it : v) {
    total += it * decimal;
    decimal *= 10;
  }
  return total;
}

tile &sfml_game::getTileById(std::vector<int> tile_id) {
  assert(!tile_id.empty());
  const int id = tile_id[0];
  // if (id > m_game.old_id)
  //  assert(!"Tile id has not been used yet"); //!OCLINT accepted idiom
  for (tile &t : m_game.get_tiles()) {
    if (t.get_id() == id) {
      return t;
    }
  }
  assert(!"Should never get here"); //!OCLINT accepted idiom
  throw std::runtime_error("ID not found");
}

void sfml_game::color_tile_shape(sf::RectangleShape &sfml_tile, const tile &t) {
  switch (t.get_type()) {
  case tile_type::grassland:
    color_shape(sfml_tile, sf::Color(0, 255, 0), sf::Color(0, 100, 0));
    break;

  case tile_type::mountains:
    color_shape(sfml_tile, sf::Color(120, 120, 120), sf::Color(50, 50, 50));
    break;

  case tile_type::ocean:
    color_shape(sfml_tile, sf::Color(0, 0, 255), sf::Color(0, 0, 100));
    break;

  case tile_type::savannah:
    color_shape(sfml_tile, sf::Color(245, 190, 0), sf::Color(235, 170, 0));
    break;

  case tile_type::arctic:
    color_shape(sfml_tile, sf::Color(50, 230, 255), sf::Color(10, 200, 255));
    break;

  case tile_type::desert:
    color_shape(sfml_tile, sf::Color(250, 210, 80), sf::Color(255, 180, 50));
    break;
  }
  sfml_tile.setOutlineThickness(5);
  auto selected = vectortoint(m_selected);
  if (t.get_id() == selected) {
    sfml_tile.setOutlineColor(sf::Color(255, 255, 255));
  } else {
    sfml_tile.setOutlineColor(outline);
  }
}

void sfml_game::color_shape(sf::RectangleShape &sfml_tile, sf::Color c1,
                            sf::Color c2) {
  sfml_tile.setFillColor(c1);
  outline = sf::Color(c2);
}

bool sfml_game::check_collision(double x, double y) {
  for (tile &t : m_game.get_tiles()) {
    //._____.
    //|     |
    //|_A___|
    //._____.
    //|   B |
    //|_____|
    //
    if (t.tile_contains(x + 15, y + 15) || t.tile_contains(x - 15, y - 15)) {
      return true;
    }
  }
  return false;
}

// Direction: 1 = /\, 2 = >, 3 = \/, 4 = <
bool sfml_game::will_colide(int direction, tile &t) {
  switch (direction) {
  case 1:
    if (sfml_game::check_collision(t.get_x() + (t.get_width() / 2),
                                   t.get_y() - (t.get_height() / 2))) {
      return true;
    }
    return false;
  case 2:
    if (sfml_game::check_collision(t.get_x() + (t.get_width() * 1.5),
                                   t.get_y() + (t.get_height() / 2))) {
      return true;
    }
    return false;
  case 3:
    if (sfml_game::check_collision(t.get_x() + (t.get_width() / 2),
                                   t.get_y() + (t.get_height() * 1.5))) {
      return true;
    }
    return false;
  case 4:
    if (sfml_game::check_collision(t.get_x() - (t.get_width() / 2),
                                   t.get_y() + (t.get_height() / 2))) {
      return true;
    }
    return false;
  default:
    return false;
  }
}

void sfml_game::setup_text() {
  // Set up text
  titleScreenText.setFont(m_font);

  titleScreenText.setString("Title Screen");
  titleScreenText.setOrigin(titleScreenText.getGlobalBounds().left +
                                titleScreenText.getGlobalBounds().width / 2.0f,
                            titleScreenText.getGlobalBounds().top +
                                titleScreenText.getGlobalBounds().height /
                                    2.0f);
  titleScreenText.setPosition(m_screen_center.x, m_screen_center.y);

  mainMenuScreenText.setFont(m_font);
  mainMenuScreenText.setString("Main Menu");
  mainMenuScreenText.setOrigin(
      mainMenuScreenText.getGlobalBounds().left +
          mainMenuScreenText.getGlobalBounds().width / 2.0f,
      mainMenuScreenText.getGlobalBounds().top +
          mainMenuScreenText.getGlobalBounds().height / 2.0f);
  mainMenuScreenText.setPosition(m_screen_center.x, m_screen_center.y);

  aboutScreenText.setFont(m_font);
  aboutScreenText.setString("About Screen");
  titleScreenText.setOrigin(aboutScreenText.getGlobalBounds().left +
                                aboutScreenText.getGlobalBounds().width / 2.0f,
                            aboutScreenText.getGlobalBounds().top +
                                aboutScreenText.getGlobalBounds().height /
                                    2.0f);
  aboutScreenText.setPosition(m_screen_center.x, m_screen_center.y);
}
