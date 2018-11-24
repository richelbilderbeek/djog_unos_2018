// Always include the header of the unit first
#include "sfml_game.h"
#include "agent.h"
#include "agent_type.h"
#include "game_state.h"
#include "id.h"
#include "sfml_resources.h"
#include <QFile>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

sfml_game::sfml_game(const int window_width,
  const int window_height,
  const sfml_game_delegate& delegate)
  : m_background_music{ sfml_resources::get().get_background_music() },
    m_delegate{ delegate },
    m_window(sf::VideoMode(static_cast<unsigned int>(window_width),
               static_cast<unsigned int>(window_height)),
      "Nature Zen", get_video_mode()),
    m_font{ sfml_resources::get().get_default_font() }
{ // Set up music
  m_background_music.setLoop(true);
  m_background_music.play();
  // Set up window, start location to the center
  const int window_x
    = static_cast<int>(sf::VideoMode::getDesktopMode().width / 2)
    - window_width / 2;
  const int window_y
    = static_cast<int>(sf::VideoMode::getDesktopMode().height / 2)
    - window_height / 2;
  m_window.setPosition(sf::Vector2i(window_x, window_y));
  m_screen_center = Vector2i(window_width / 2, window_height / 2);

  // Set up text
  setup_text();
}

sfml_game::~sfml_game()
{
  m_background_music.stop();
}

void sfml_game::close()
{
  m_window.close();
}

// WARNING function is long
void sfml_game::display() //!OCLINT indeed long, must be made shorter
{
  m_window.clear(sf::Color::Black); // Clear the window with black color
  if (m_game_state == game_state::playing)
  {
    // Display all tiles
    for (const tile& t : m_game.get_tiles())
    {
      sfml_game::display_tile(t);
    }
    sf::Text(sf::String(std::to_string(m_game.get_score())), m_font, 30);
  }
  /////sf::Text(sf::String(std::to_string(m_game.get_score())), m_font, 30);

  sf::Text text(sf::String(std::to_string(m_game.get_score())), m_font, 30);
  text.setPosition(m_window.getSize().x - 80, 10);
  text.setStyle(Text::Bold);
  m_window.draw(text);

  if (m_is_space_pressed)
  {
    reset_input();
  }

  load_game_state();
  //  m_window.draw(text);
  m_window.display(); // Put everything on the screen
}

void sfml_game::display_tile(const tile &t){
    sf::RectangleShape sfml_tile(sf::Vector2f(
      static_cast<float>(t.get_width()), static_cast<float>(t.get_height())));
    // If the camera moves to right/bottom, tiles move relatively
    // left/downwards
    const double screen_x{ t.get_x() - m_camera.x };
    const double screen_y{ t.get_y() - m_camera.y };
    sfml_tile.setPosition(screen_x, screen_y);
    color_tile_shape(sfml_tile, t);
    m_window.draw(sfml_tile);
    // Draw agents
    for (const agent& a : t.get_agents())
    {
      sfml_game::display_agent(a, screen_x, screen_y);
    }
}

void sfml_game::display_agent(const agent &a, double screen_x, double screen_y){
    sf::Sprite sprite;
    set_agent_sprite(a, sprite);
    assert(sprite.getTexture());
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(screen_x + static_cast<float>(a.get_x()),
        screen_y + static_cast<float>(a.get_y()));
   m_window.draw(sprite);
}

void sfml_game::set_agent_sprite(const agent& a, sf::Sprite& sprite) {
  sprite.setTexture(sfml_resources::get().get_agent_sprite(a));
}

int get_video_mode()
{
  int s = sf::Style::Default; //!OCLINT local variable redundant
//  if (std::getenv("TRAVIS"))
//  {
//    s = Style::Default;
//  }
  return s;
}

void sfml_game::load_game_state()
{
  switch (m_game_state)
  {
    case game_state::titlescreen:
      m_window.draw(titleScreenText);
      return;
    case game_state::menuscreen:
      return;
    case game_state::aboutscreen:
      m_window.draw(aboutScreenText);
      return;
    default:
      return;
  }
}

void sfml_game::change_game_state()
{
  switch (m_game_state)
  {
    case game_state::titlescreen:
      reset_input();
      m_game_state = game_state::menuscreen;
      return;
    case game_state::menuscreen:
      reset_input();
      m_game_state = game_state::aboutscreen;
      return;
    case game_state::aboutscreen:
      reset_input();
      m_game_state = game_state::playing;
      return;
    default:
      return;
  }
}

void sfml_game::exec()
{
  while (m_window.isOpen())
  {
    process_input();
    process_events();
    display();
  }
}

void sfml_game::process_events()
{

  m_game.process_events();

  if ((115.0 / m_tile_speed != std::abs(std::floor(115.0 / m_tile_speed))
        || 115.0 / m_tile_speed != std::abs(std::ceil(115.0 / m_tile_speed)))
    || m_tile_speed > 115.0)
  {
    throw std::runtime_error("The set tile speed is not usable");
  }

  if (m_game.m_selected.empty())
  {
    confirm_move();
  }
  else
  {
    follow_tile();
  }

  exec_tile_move(m_game.m_selected);

  manage_timer();

  m_delegate.do_actions(*this);
  ++m_n_displayed;
}

void sfml_game::confirm_move()
{
  if (m_camera.movecam_r == true)
    m_camera.move_camera(sf::Vector2f(0.5, 0));
  if (m_camera.movecam_l == true)
    m_camera.move_camera(sf::Vector2f(-0.5, 0));
  if (m_camera.movecam_u == true)
    m_camera.move_camera(sf::Vector2f(0, -0.5));
  if (m_camera.movecam_d == true)
    m_camera.move_camera(sf::Vector2f(0, 0.5));
}

void sfml_game::follow_tile()
{
  const tile& t = getTileById(m_game.m_selected);
  m_camera.x = t.get_x() + (t.get_width() / 2) - m_screen_center.x;
  m_camera.y = t.get_y() + (t.get_height() / 2) - m_screen_center.y;
}

void sfml_game::manage_timer()
{
  if (m_timer > 0)
  {
    --m_timer;
  }
  else
  {
    m_game.m_selected.clear();
    if (!m_temp_id.empty())
      m_game.m_selected.push_back(m_temp_id[0]);
  }
}

void sfml_game::exec_tile_move(std::vector<int> selected)
{
  if (!selected.empty())
  {
    tile& temp_tile = getTileById(selected);
    if (m_timer > 0)
    {
      temp_tile.move();
    }
    else
    {
      temp_tile.set_dx(0);
      temp_tile.set_dy(0);
    }
  }
}

void sfml_game::process_event(const sf::Event& event)
{
  switch (event.type)
  {
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

void sfml_game::process_input()
{
  // check all the window's events that were triggered since the last iteration
  // of the loop
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    process_event(event);
  }
}

void sfml_game::process_keyboard_input(const sf::Event& event)
{
  // Only keyboard events
  assert(event.type == sf::Event::KeyPressed
    || event.type == sf::Event::KeyReleased);

  if (event.type == sf::Event::KeyPressed)
  {
    check_change_game_state(event);
    arrows(true, event);
    if (!m_game.m_selected.empty())
      tile_movement(true, event, getTileById(m_game.m_selected));
    if (m_timer > 0)
      tile_movement(false, event, getTileById(m_game.m_selected));
  }
  else
  {
    arrows(false, event);
  }
}

void sfml_game::check_change_game_state(const sf::Event& event)
{
  if (event.key.code == sf::Keyboard::Space)
    change_game_state();
  if (m_game_state == game_state::playing
    && event.key.code == sf::Keyboard::Escape)
    m_game_state = game_state::menuscreen;
}

void sfml_game::move_selected_tile_randomly()
{
  if (m_game.m_selected.empty())
    return;
  const int id = m_game.m_selected[0];
  this->getTileById({ id }).set_dx(5);
}

void sfml_game::reset_input()
{
  m_camera.x = 0;
  m_camera.y = 0;
  m_camera.movecam_r = false;
  m_camera.movecam_l = false;
  m_camera.movecam_u = false;
  m_camera.movecam_d = false;
}

void sfml_game::process_mouse_input(const sf::Event& event)
{
  // Only mouse input
  assert(event.type == sf::Event::MouseButtonPressed);

  if (event.mouseButton.button == sf::Mouse::Left)
  {
    std::vector<tile> game_tiles = m_game.get_tiles();
    for (unsigned i = 0; i < game_tiles.size(); i++)
    {
      if (game_tiles.at(i).tile_contains(
            sf::Mouse::getPosition(m_window).x + m_camera.x,
            sf::Mouse::getPosition(m_window).y + m_camera.y))
      {
        m_temp_id.clear();
        m_temp_id.push_back(game_tiles.at(i).get_id());
        clicked_tile = true;
      }
    }
    if (clicked_tile == false)
    {
      m_temp_id.clear();
    }
    clicked_tile = false;
  }
}

void sfml_game::select_random_tile()
{
  const auto& tiles = m_game.get_tiles();
  const int i = std::rand() % tiles.size();
  const int id = tiles[i].get_id();
  m_game.m_selected.resize(1);
  m_game.m_selected[0] = id;
}

void sfml_game::stop_music()
{
  m_background_music.stop();
}

// NOTE Changed it to show_title (was show_menu)
void sfml_game::show_title()
{
  m_game_state = game_state::titlescreen;
}

void sfml_game::arrows(bool b, const sf::Event& event)
{
  if (event.key.code == sf::Keyboard::D)
    m_camera.movecam_r = b;
  if (event.key.code == sf::Keyboard::A)
    m_camera.movecam_l = b;
  if (event.key.code == sf::Keyboard::W)
    m_camera.movecam_u = b;
  if (event.key.code == sf::Keyboard::S)
    m_camera.movecam_d = b;
}

void sfml_game::tile_movement(bool b, const sf::Event& event, tile& t)
{
  if (m_timer == 0)
  {
    if (b == true)
    {
      tile_move_ctrl(event, t);
      m_timer += (1 / m_tile_speed) * 115;
    }
    else
    {
      t.set_dx(0);
      t.set_dy(0);
    }
  }
}

void sfml_game::tile_move_ctrl(const sf::Event& event, tile& t)
{
  if (event.key.code == sf::Keyboard::D)
    switch_collide(t, 2);
  if (event.key.code == sf::Keyboard::A)
    switch_collide(t, 4);
  if (event.key.code == sf::Keyboard::W)
    switch_collide(t, 1);
  if (event.key.code == sf::Keyboard::S)
    switch_collide(t, 3);
}

void sfml_game::confirm_tile_move(tile& t, int direction)
{
  switch (direction)
  {
    case 1:
      t.set_dy(-m_tile_speed);
      return;
    case 2:
      t.set_dx(m_tile_speed);
      return;
    case 3:
      t.set_dy(m_tile_speed);
      return;
    case 4:
      t.set_dx(-m_tile_speed);
      return;
    default:
      return;
  }
}

void sfml_game::switch_collide(tile& t, int direction)
{
  sf::Vector2f v = get_direction_pos(direction, t, 0);
  //std::vector<tile> added_tiles;
  if (!will_colide(direction, t))
  {
    confirm_tile_move(t, direction);
  }
    if (get_collision_id(v.x, v.y)[0] != 0 && will_colide(direction, t)
      && check_merge(t, getTileById(get_collision_id(v.x, v.y)))
      && getTileById(get_collision_id(v.x, v.y)).get_width() == t.get_width()
      && getTileById(get_collision_id(v.x, v.y)).get_height() == t.get_height())
      {
        confirm_tile_move(t, direction);
        sf::Vector2f b = get_direction_pos(direction, t, 115);
        if (get_collision_id(b.x, b.y)[0] == get_collision_id(v.x, v.y)[0])
        {
          t.set_dx(t.get_dx() * 2);
          t.set_dy(t.get_dy() * 2);
        }
//    tile& collide_tile = getTileById(get_collision_id(v.x, v.y));
//    {
//      tile new_t(collide_tile.get_x(),
//        collide_tile.get_y(),
//        collide_tile.get_z(),
//        collide_tile.get_width(),
//        collide_tile.get_height(),
//        get_merge_type(t.get_type(), collide_tile.get_type()),
//        new_id());
//      added_tiles.push_back(new_t);
//    }
//    deleted_tiles.push_back(t);
//    deleted_tiles.push_back(collide_tile);
//    m_game.add_tiles(added_tiles);
//    // TODO delete old tiles
//    // m_game.delete_tiles(deleted_tiles);
  }
}

bool sfml_game::check_merge(tile& t1, tile& t2)
{
  if (get_merge_type(t1.get_type(), t2.get_type()) == tile_type::nonetile)
  {
    return false;
  }
  return true;
}

sf::Vector2f sfml_game::get_direction_pos(int direction, tile& t, double plus)
{
  switch (direction)
  {
    case 1:
      return sf::Vector2f(t.get_x() + (t.get_width() / 2),
        t.get_y() - (t.get_height() / 2) - plus);
    case 2:
      return sf::Vector2f(t.get_x() + (t.get_width() * 1.5) + plus,
        t.get_y() + (t.get_height() / 2));
    case 3:
      return sf::Vector2f(t.get_x() + (t.get_width() / 2),
        t.get_y() + (t.get_height() * 1.5) + plus);
    case 4:
      return sf::Vector2f(t.get_x() - (t.get_width() / 2) - plus,
        t.get_y() + (t.get_height() / 2));
    default:
      return sf::Vector2f(0, 0);
  }
  return sf::Vector2f(0, 0);
}

int vectortoint(std::vector<int> v)
{
  reverse(v.begin(), v.end());
  int decimal = 1;
  int total = 0;
  for (auto& it : v)
  {
    total += it * decimal;
    decimal *= 10;
  }
  return total;
}

tile& sfml_game::getTileById(const std::vector<int>& tile_id)
{
  assert(!tile_id.empty());
  const int id = tile_id[0];
  if (id > get_old_id()) {
    assert(!"Tile id has not been used yet"); //!OCLINT accepted idiom
    throw std::runtime_error("ID not found");
  }
  for (tile& t : m_game.get_tiles())
  {
    if (t.get_id() == id)
    {
      return t;
    }
  }
  //assert(!"The tile with that id has been deleted"); //!OCLINT accepted idiom
  throw std::runtime_error("ID not found");
}

void sfml_game::color_tile_shape(sf::RectangleShape& sfml_tile, const tile& t) //!OCLINT no 32 statements
{
  switch (t.get_type())
  {

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

    case tile_type::swamp:
      color_shape(sfml_tile, sf::Color(130, 100, 15), sf::Color(100, 80, 15));
      break;

    case tile_type::arctic:
      color_shape(sfml_tile, sf::Color(50, 230, 255), sf::Color(10, 200, 255));
      break;

    case tile_type::desert:
      color_shape(sfml_tile, sf::Color(250, 210, 80), sf::Color(255, 180, 50));
      break;

    case tile_type::woods:
      color_shape(sfml_tile, sf::Color(34, 139, 34), sf::Color(0, 128, 0));
      break;

    default:
      color_shape(
        sfml_tile, sf::Color(205, 205, 205), sf::Color(255, 255, 255));
      break;
  }
  sfml_tile.setOutlineThickness(5);
  auto selected = vectortoint(m_game.m_selected);
  if (t.get_id() == selected)
  {
    sfml_tile.setOutlineColor(sf::Color(255, 255, 255));
  }
  else
  {
    sfml_tile.setOutlineColor(outline);
  }
}

void sfml_game::color_shape(
  sf::RectangleShape& sfml_tile, sf::Color c1, sf::Color c2)
{
  sfml_tile.setFillColor(c1);
  outline = sf::Color(c2);
}

bool sfml_game::check_collision(double x, double y)
{
  for (tile& t : m_game.get_tiles())
  {
    // ._____.
    // |     |
    // |_A___|
    // ._____.
    // |   B |
    // |_____|
    //
    if (t.tile_contains(x + 15, y + 15) || t.tile_contains(x - 15, y - 15))
    {
      return true;
    }
  }
  return false;
}

std::vector<int> sfml_game::get_collision_id(double x, double y) const
{
  for (const tile& t : m_game.get_tiles())
  {
    if (t.tile_contains(x, y))
    {
      return { t.get_id() };
    }
  }
  return { 0 } ;
}

// Direction: 1 = /\, 2 = >, 3 = \/, 4 = <
bool sfml_game::will_colide(int direction, tile& t)
{
  switch (direction)
  {
    case 1:
      if (sfml_game::check_collision(
            t.get_x() + (t.get_width() / 2), t.get_y() - (t.get_height() / 2)))
      {
        return true;
      }
      return false;
    case 2:
      if (sfml_game::check_collision(t.get_x() + (t.get_width() * 1.5),
            t.get_y() + (t.get_height() / 2)))
      {
        return true;
      }
      return false;
    case 3:
      if (sfml_game::check_collision(t.get_x() + (t.get_width() / 2),
            t.get_y() + (t.get_height() * 1.5)))
      {
        return true;
      }
      return false;
    case 4:
      if (sfml_game::check_collision(
            t.get_x() - (t.get_width() / 2), t.get_y() + (t.get_height() / 2)))
      {
        return true;
      }
      return false;
    default:
      break;
  }
  return false;
}

void sfml_game::setup_text()
{
  // Set up text
  titleScreenText.setFont(m_font);
  titleScreenText.setString("Nature Zen");
  titleScreenText.setOrigin(titleScreenText.getGlobalBounds().left
      + titleScreenText.getGlobalBounds().width / 2.0f,
    titleScreenText.getGlobalBounds().top
      + titleScreenText.getGlobalBounds().height /2.0f);
  titleScreenText.setPosition(m_screen_center.x, m_screen_center.y);



  aboutScreenText.setFont(m_font);
  aboutScreenText.setString("About Screen");
  titleScreenText.setOrigin(aboutScreenText.getGlobalBounds().left
      + aboutScreenText.getGlobalBounds().width / 2.0f,
    aboutScreenText.getGlobalBounds().top
      + aboutScreenText.getGlobalBounds().height / 2.0f);
  aboutScreenText.setPosition(m_screen_center.x, m_screen_center.y);
}

void test_sfml_game() //!OCLINT tests may be long
{
  {
    //A game can change state
    sfml_game g;
    for (int i = 0; i < 3; i++)
    {
      g.change_game_state();
    }
  }
  //#define FIX_ISSUE_221
  #ifdef FIX_ISSUE_221
  {
    //Get the fill color of a tile type
    assert(get_fill_color(tile_type::grassland) == sf::Color(0, 255, 0));
    assert(get_fill_color(tile_type::mountains) == sf::Color(120, 120, 120));
    assert(get_fill_color(tile_type::ocean) == sf::Color(0, 0, 255));
    assert(get_fill_color(tile_type::savannah) == sf::Color(245, 190, 0));
    assert(get_fill_color(tile_type::swamp) == sf::Color(130, 100, 15));
    assert(get_fill_color(tile_type::arctic) == sf::Color(50, 230, 255));
    assert(get_fill_color(tile_type::desert) == sf::Color(250, 210, 80));
    assert(get_fill_color(tile_type::woods) == sf::Color(34, 139, 34));
  }
  #endif // FIX_ISSUE_221
  //#define FIX_ISSUE_222
  #ifdef FIX_ISSUE_222
  {
    //Get the outline/border color of a tile tipe
    assert(get_outline_color(tile_type::grassland) == sf::Color(0, 100, 0));
    assert(get_outline_color(tile_type::mountains) == sf::Color(50, 50, 50));
    assert(get_outline_color(tile_type::ocean) == sf::Color(0, 0, 100));
    assert(get_outline_color(tile_type::savannah) == sf::Color(245, 190, 0));
    assert(get_outline_color(tile_type::swamp) == sf::Color(100, 80, 15));
    assert(get_outline_color(tile_type::arctic) == sf::Color(10, 200, 255));
    assert(get_outline_color(tile_type::desert) == sf::Color(255, 180, 50));
    assert(get_outline_color(tile_type::woods) == sf::Color(0, 128, 0));
  }
  #endif // FIX_ISSUE_222
}
