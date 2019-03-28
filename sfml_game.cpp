// Always include the header of the unit first
#include "sfml_game.h"
#include "agent.h"
#include "agent_type.h"
#include "game_state.h"
#include "tile_id.h"
#include "sfml_resources.h"
#include "tile.h"
#include <QFile>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <SFML/Window.hpp>

sfml_game::sfml_game(
  const sfml_game_delegate& delegate,
  const std::vector<tile>& tiles,
  const std::vector<agent>& agents)
  : m_background_music{ sfml_resources::get().get_background_music() },
    m_ben_ik_een_spin{ sfml_resources::get().get_benikeenspin() },
    m_delegate{ delegate },
    m_game{ game(tiles, agents) },
    m_window{ sfml_window_manager::get().get_window() },
    m_pause_screen()
{ // Set up music
  // m_background_music = sfml_resources::get().get_background_music();
  m_background_music.setLoop(true);

  m_ben_ik_een_spin.setLoop(true);
  start_music();
  setup_display_score();
  setup_essence_symbol();
  setup_tickcounter_text();
  m_game.set_allow_spawning(m_delegate.get_spawning());
  m_game.set_allow_damage(m_delegate.get_damage());
  m_game.set_allow_score(m_delegate.get_score());
}

sfml_game::~sfml_game()
{
  stop_music();
}

void sfml_game::close(game_state s) {
  if (s != game_state::paused) {
    m_camera.reset();
  }
  m_camera.m_movecam_r = false;
  m_camera.m_movecam_l = false;
  m_camera.m_movecam_u = false;
  m_camera.m_movecam_d = false;
  sfml_window_manager::get().set_state(s);
}

void sfml_game::close()
{
  m_window.close();
}

void sfml_game::start_music() {
  stop_music();
  m_background_music.play();
}

void sfml_game::setup_tickcounter_text() {
    m_debug_font.loadFromFile("font.ttf");
    m_tickcounter_text.setFont(m_debug_font);
    m_tickcounter_text.setCharacterSize(20);
}

void sfml_game::setup_display_score() {
  m_zen_bar.setSize(sf::Vector2f(sfml_resources::get().get_zen_bar().getSize()));
  m_zen_bar.setPosition(sf::Vector2f(
                          (m_window.getSize().x/2.0f)-(m_zen_bar.getSize().x/2.0f),
                          15));
  m_zen_bar.setTexture(&sfml_resources::get().get_zen_bar());

  m_zen_ind.setSize(sf::Vector2f(sfml_resources::get().get_zen_ind().getSize()));
  m_zen_ind.setPosition(sf::Vector2f(
                          (m_window.getSize().x/2.0f)-(m_zen_ind.getSize().x/2.0f),
                          15+(m_zen_bar.getSize().y/2.0f)));
  m_zen_ind.setTexture(&sfml_resources::get().get_zen_ind());
}

void sfml_game::setup_essence_symbol()
{
  m_essence_symbol.setSize(0.6f*sf::Vector2f(
    sfml_resources::get().get_essence_texture().getSize()));
  m_essence_symbol.setTexture(&sfml_resources::get().get_essence_texture());
}

void sfml_game::display_essence_symbol()
{
  m_essence_symbol.setPosition(
    m_window.mapPixelToCoords(sf::Vector2i(m_window.getSize().x*51.0f/64.0f, 15)));
  m_essence_symbol.setPosition(
    m_window.mapPixelToCoords(sf::Vector2i(m_essence_symbol.getPosition())));
  m_window.draw(m_essence_symbol);
}

void sfml_game::display_essence()
{
  std::stringstream s;
  s << " : " << m_game.get_essence();
  m_tickcounter_text.setString(s.str());
  m_tickcounter_text.setPosition(
    m_window.mapPixelToCoords(sf::Vector2i(m_window.getSize().x*52.0f/64.0f, 10)));
  m_window.draw(m_tickcounter_text);
}

void sfml_game::display() //!OCLINT indeed long, must be made shorter
{
  m_window.clear(sf::Color::Black); // Clear the window with black color
  // Display all tiles
  for (const tile& t : m_game.get_tiles())
  {
    display_tile(t);
  }
  // Display all agents
  for (const agent& a : m_game.get_agents())
  {
    display_agent(a);
  }
  // Display & Update Tickcounter
  {
    std::stringstream s;
    s << "TICK COUNT: " << m_game.get_n_ticks() << "\n"
      << "SCORE: " << m_game.get_score();
    m_tickcounter_text.setString(s.str());
    m_tickcounter_text.setPosition(m_window.mapPixelToCoords(sf::Vector2i(10, 10)));
    m_window.draw(m_tickcounter_text);
  }
  // Display the essence
  display_essence();
  sfml_game::display_essence_symbol();
  // Display the zen
  {
    m_zen_bar.setPosition(sf::Vector2f(
                            (m_window.getSize().x/2.0f)-(m_zen_bar.getSize().x/2.0f),
                            15));
    m_zen_bar.setPosition(m_window.mapPixelToCoords(sf::Vector2i(m_zen_bar.getPosition())));
    m_window.draw(m_zen_bar);
    m_zen_ind.setPosition(sf::Vector2f(
                              (m_window.getSize().x/2.0f)-
                              (m_zen_ind.getSize().x/2.0f)+
                              m_game.get_score(),
                              15+(m_zen_bar.getSize().y/2.0f)-
                              (m_zen_ind.getSize().x/2.0f))
                            );
    m_zen_ind.setPosition(m_window.mapPixelToCoords(sf::Vector2i(m_zen_ind.getPosition())));
    m_window.draw(m_zen_ind);
  }
  if (active(game_state::playing))
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
    sfml_tile.setPosition(m_window.mapPixelToCoords(sf::Vector2i(sfml_tile.getPosition())));
    color_tile_shape(sfml_tile, t);
    m_window.draw(sfml_tile);
    // Texture
    sf::Sprite sprite;
    set_tile_sprite(t, sprite);
    assert(sprite.getTexture());
    sprite.setPosition(screen_x, screen_y);
    sprite.setPosition(m_window.mapPixelToCoords(sf::Vector2i(sprite.getPosition())));
    m_window.draw(sprite);
}

void sfml_game::display_agent(const agent &a){
  const double screen_x{ a.get_x() - m_camera.x };
  const double screen_y{ a.get_y() - m_camera.y };
  sf::Sprite sprite;
  set_agent_sprite(a, sprite);
  assert(sprite.getTexture());
  sprite.setScale(0.2f, 0.2f);
  sprite.setPosition(screen_x, screen_y);
  sprite.setPosition(m_window.mapPixelToCoords(sf::Vector2i(sprite.getPosition())));
  m_window.draw(sprite);
}

void sfml_game::set_agent_sprite(const agent& a, sf::Sprite& sprite) {
  sprite.setTexture(sfml_resources::get().get_agent_sprite(a));
}

void sfml_game::set_tile_sprite(const tile &t, sf::Sprite &sprite) {
  sprite.setTexture(sfml_resources::get().get_tile_sprite(t));
}

void sfml_game::exec()
{
  sf::View view = m_window.getDefaultView();
  view.setSize(static_cast<float>(m_window.getSize().x),
               static_cast<float>(m_window.getSize().y));
  m_window.setView(view);
  while (active(game_state::playing) || active(game_state::paused))
  {
    if (active(game_state::paused)) {
      display();
      m_pause_screen.exec();
    } else {
      process_input();
      process_events();
      display();
    }
  }
}

void sfml_game::process_events()
{

  m_game.process_events();

  sf::Vector2i current_mouse = sf::Mouse::getPosition();
  sf::Vector2i mouse_delta = current_mouse - m_prev_mouse_pos;
  m_mouse_speed = sqrt(mouse_delta.x * mouse_delta.x + mouse_delta.y * mouse_delta.y);
  m_prev_mouse_pos = current_mouse;

  if ((112.0 / m_tile_speed != std::abs(std::floor(112.0 / m_tile_speed))
        || 112.0 / m_tile_speed != std::abs(std::ceil(112.0 / m_tile_speed)))
    || m_tile_speed > 112.0)
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

  if (m_game.get_score() >= 112 || m_game.get_score() <= -112) {
    close(game_state::gameover);
  }

  manage_timer();

  m_delegate.do_actions(*this);
  ++m_n_displayed;
}

void sfml_game::confirm_move()
{
  if (m_camera.m_movecam_r == true)
    m_camera.move_camera(sf::Vector2f(0.5, 0));
  if (m_camera.m_movecam_l == true)
    m_camera.move_camera(sf::Vector2f(-0.5, 0));
  if (m_camera.m_movecam_u == true)
    m_camera.move_camera(sf::Vector2f(0, -0.5));
  if (m_camera.m_movecam_d == true)
    m_camera.move_camera(sf::Vector2f(0, 0.5));
}

void sfml_game::follow_tile()
{
  sf::Vector2i screen_center = sfml_window_manager::get().get_window_center();
  const tile& t = getTileById(m_game.m_selected);
  m_camera.x = 0;
  m_camera.y = 0;
  sf::Vector2f new_coords = sf::Vector2f(t.get_x() + (t.get_width() / 2) - screen_center.x,
                                         t.get_y() + (t.get_height() / 2) - screen_center.y);
  m_camera.move_camera(new_coords);
}

void sfml_game::manage_timer()
{
  if (m_timer > 0)
  {
    --m_timer;
  }
}

void sfml_game::exec_tile_move(std::vector<int> selected)
{
  if (!selected.empty())
  {
    tile& temp_tile = getTileById(selected);
    if (m_timer <= 0)
    {
      temp_tile.set_dx(0);
      temp_tile.set_dy(0);      
    }
  }
}

void sfml_game::process_event(const sf::Event& event)
{
  sf::View view = m_window.getDefaultView();
  switch (event.type)
  {
    case sf::Event::Closed:
      close();
      break;

    case sf::Event::KeyReleased:
    case sf::Event::KeyPressed:
      process_keyboard_input(event);
      break;

    case sf::Event::MouseButtonPressed:
      process_mouse_input(event);
      break;

    case sf::Event::Resized:
      sfml_window_manager::get().update();
      view.setSize(static_cast<float>(event.size.width),
                   static_cast<float>(event.size.height));
      m_window.setView(view);
      break;

    default:
      sfml_window_manager::get().process();
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

void sfml_game::process_keyboard_input(const sf::Event& event) //OCLINT complexity accepted for now
{
  // Only keyboard events
  assert(event.type == sf::Event::KeyPressed
    || event.type == sf::Event::KeyReleased);

  if (event.type == sf::Event::KeyPressed)
  {
    arrows(true, event);
    if (!m_game.m_selected.empty())
      control_tile(true, event, getTileById(m_game.m_selected));
    if (m_timer > 0)
      control_tile(false, event, getTileById(m_game.m_selected));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
      close(game_state::paused);
  }
  else
  {
    arrows(false, event);
  }
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
  m_camera.m_movecam_r = false;
  m_camera.m_movecam_l = false;
  m_camera.m_movecam_u = false;
  m_camera.m_movecam_d = false;
}

void sfml_game::process_mouse_input(const sf::Event& event)
{
  // Only mouse input
  assert(event.type == sf::Event::MouseButtonPressed);

  if (event.mouseButton.button == sf::Mouse::Left)
  {
    m_game.move_tiles(m_window, m_camera);
    m_clicked_tile = false;
    if (m_game.get_agents().size() == 1 &&
        m_game.get_tiles().size() > 0)
      ben_ik_een_spin();
  }
  else if (event.mouseButton.button == sf::Mouse::Right){
    m_game.remove_tile(m_window, m_camera);
  }
}

void sfml_game::ben_ik_een_spin() {
  assert(m_game.get_agents().size() == 1);
  assert(m_game.get_tiles().size() > 0);
  agent& spin = m_game.get_agents()[0];
  if (spin.get_type() == agent_type::spider &&
      spin.is_clicked(sf::Mouse::getPosition(m_window).x + m_camera.x,
                      sf::Mouse::getPosition(m_window).y + m_camera.y,
                      sfml_resources::get().get_agent_sprite(spin)) &&
      m_ben_ik_een_spin.getStatus() != sf::Music::Playing)
  {
    stop_music();
    m_ben_ik_een_spin.play();
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
  if (m_background_music.getStatus() != sf::Music::Stopped)
    m_background_music.stop();
  if (m_ben_ik_een_spin.getStatus() != sf::Music::Stopped)
    m_ben_ik_een_spin.stop();
}

void sfml_game::arrows(bool b, const sf::Event& event)
{
  if (event.key.code == sf::Keyboard::D)
    m_camera.m_movecam_r = b;
  if (event.key.code == sf::Keyboard::A)
    m_camera.m_movecam_l = b;
  if (event.key.code == sf::Keyboard::W)
    m_camera.m_movecam_u = b;
  if (event.key.code == sf::Keyboard::S)
    m_camera.m_movecam_d = b;
}

void sfml_game::control_tile(bool b, const sf::Event& event, tile& t)
{
  if (m_timer == 0)
  {
    if (b == true)
    {
      tile_move_ctrl(event, t);
      m_timer += (1 / m_tile_speed) * 111;
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
  if (event.key.code == sf::Keyboard::R)
    rotate(t);
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
    //confirm_tile_move(t, direction);
    m_game.confirm_tile_move(t, direction, m_tile_speed);
  }
  if (get_collision_id(v.x, v.y)[0] != 0 && will_colide(direction, t)
      && check_merge(t, getTileById(get_collision_id(v.x, v.y)))
      && getTileById(get_collision_id(v.x, v.y)).get_width() == t.get_width()
      && getTileById(get_collision_id(v.x, v.y)).get_height() == t.get_height())
  {
    //confirm_tile_move(t, direction);
    m_game.confirm_tile_move(t, direction, m_tile_speed);
    sf::Vector2f b = get_direction_pos(direction, t, 112);
    if (get_collision_id(b.x, b.y)[0] == get_collision_id(v.x, v.y)[0])
    {
      t.set_dx(t.get_dx() * 2);
      t.set_dy(t.get_dy() * 2);
    }
  }
}

bool sfml_game::check_merge(tile& t1, tile& t2)
{
  return get_merge_type(t1.get_type(), t2.get_type()) != tile_type::nonetile;
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
  if (id > tile_id::get_old_tile_id()) {
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

// First color: Fill  |  Second color: Outline
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
    case tile_type::water:
      color_shape(sfml_tile,
                  sf::Color(0, 0, 255-t.get_depth() * 2),
                  sf::Color(0, 0, 100-t.get_depth()));
      break;
    case tile_type::savannah:
      color_shape(sfml_tile, sf::Color(245, 190, 0), sf::Color(235, 170, 0));
      break;
    case tile_type::swamp:
      color_shape(sfml_tile, sf::Color(82, 104, 27), sf::Color(67, 84, 26));
      break;
    case tile_type::mangrove:
      color_shape(sfml_tile, sf::Color(61, 39, 26), sf::Color(33, 23, 17));
      break;// TODO improve tile colors \/
    case tile_type::arctic:
      color_shape(sfml_tile, sf::Color(50, 230, 255), sf::Color(10, 200, 255));
      break;
    case tile_type::desert:
      color_shape(sfml_tile, sf::Color(250, 210, 80), sf::Color(255, 180, 50));
      break;
    case tile_type::woods:
      color_shape(sfml_tile, sf::Color(34, 139, 34), sf::Color(0, 128, 0));
      break;
    case tile_type:: dunes:
      color_shape(sfml_tile, sf::Color(220, 180, 40), sf::Color(255, 180, 50));
      break;
    case tile_type::tundra:
      color_shape(sfml_tile, sf::Color(178, 58, 5), sf::Color(185, 175, 173));
      break;
    case tile_type::hills:
      color_shape(sfml_tile, sf::Color(145, 156, 48), sf::Color(148, 145, 44));
      break;
    case tile_type::rainforest:
      color_shape(sfml_tile, sf::Color(41,47,13), sf::Color(33,19,4));
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
    sfml_tile.setOutlineColor(m_outline);
  }
}

void sfml_game::color_shape(
  sf::RectangleShape& sfml_tile, sf::Color c1, sf::Color c2)
{
  sfml_tile.setFillColor(c1);
  m_outline = sf::Color(c2);
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
    if (contains(t, x + 12, y + 12) || contains(t, x - 12, y - 12))
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
    if (contains(t, x, y))
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

void test_sfml_game() //!OCLINT tests may be long
{
  return; // STUB
}
