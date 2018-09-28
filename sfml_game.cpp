//Always include the header of the unit first
#include "sfml_game.h"

#include <iostream>
#include <cassert>
#include <QFile>

sfml_game::sfml_game(
  const int window_width,
  const int window_height,
  const sfml_game_delegate& delegate
)
  : m_background_music{},
    m_delegate{delegate},
    m_window(sf::VideoMode(window_width, window_height), "Nature Zen"),
    m_font{}
{

  //Resources
  {
    //Re-create resource at executable's location
    { QFile f(":/nature_zen/resources/background_music.ogg"); f.copy("background_music.ogg"); }

    //Re-create font
    { QFile f(":/nature_zen/resources/font.ttf"); f.copy("font.ttf"); }
  }
  //Set up music
  {
    m_background_music.setLoop(true);
    if (!m_background_music.openFromFile("background_music.ogg"))
    {
      throw std::runtime_error("Cannot find music file 'background_music.ogg'");
    }
    m_background_music.play();
  }
  //Set up window, start location to the center
  m_window.setPosition(
    sf::Vector2i(
        sf::VideoMode::getDesktopMode().width * 0.5 - window_width * 0.5,
        sf::VideoMode::getDesktopMode().height * 0.5 - window_height * 0.5
    )
  );
  //Set up font
  if (!m_font.loadFromFile("font.ttf"))
  {
    throw std::runtime_error("Cannot find font file font.ttf");
  }
  screen_center = Vector2i(sf::VideoMode::getDesktopMode().width * 0.5 - window_width * 0.5,
          sf::VideoMode::getDesktopMode().height * 0.5 - window_height * 0.5);

  //Set up text
  titleScreenText.setFont(m_font);
  titleScreenText.setString("Title Screen \n press space to go next");
  titleScreenText.setOrigin(titleScreenText.getGlobalBounds().left + titleScreenText.getGlobalBounds().width /2.0f,
                            titleScreenText.getGlobalBounds().top + titleScreenText.getGlobalBounds().height /2.0f);
  titleScreenText.setPosition(screen_center.x, screen_center.y);


  mainMenuScreenText.setFont(m_font);
  mainMenuScreenText.setString("Main Menu \n press space to go next");
  mainMenuScreenText.setOrigin(mainMenuScreenText.getGlobalBounds().left + mainMenuScreenText.getGlobalBounds().width /2.0f,
                            mainMenuScreenText.getGlobalBounds().top + mainMenuScreenText.getGlobalBounds().height /2.0f);
  mainMenuScreenText.setPosition(screen_center.x, screen_center.y);


  aboutScreenText.setFont(m_font);
  aboutScreenText.setString("About Screen \n press space to play");
  titleScreenText.setOrigin(aboutScreenText.getGlobalBounds().left + aboutScreenText.getGlobalBounds().width /2.0f,
                            aboutScreenText.getGlobalBounds().top + aboutScreenText.getGlobalBounds().height /2.0f);
  aboutScreenText.setPosition(screen_center.x, screen_center.y);
}

sfml_game::~sfml_game()
{
  m_background_music.stop();
}

void sfml_game::close()
{
  m_window.close();
}

void sfml_game::display()
{
  m_window.clear(sf::Color::Black);//Clear the window with black color
  if (gameState == Playing) {
      for (const tile& t: m_game.get_tiles())
      {
        sf::RectangleShape sfml_tile(
          sf::Vector2f(t.get_width(), t.get_height())
        );
        //If the camera moves to right/bottom, tiles move relatively left/downwards
        sfml_tile.setPosition(
          t.get_x() - m_camera_x,
          t.get_y() - m_camera_y
        );
        color_tile_shape(sfml_tile, t);
        m_window.draw(sfml_tile);
      }
  }
  sf::Text(sf::String(std::to_string(m_game.get_score())), m_font, 30);
  if (gameState == TitleScreen) {
    m_window.draw(titleScreenText);
    if (space_pressed) {
        reset_input();
        gameState = MenuScreen;
    }
  } else if (gameState == MenuScreen) {
    m_window.draw(mainMenuScreenText);
    if (space_pressed) {
        reset_input();
        gameState = AboutScreen;
    }
  } else if (gameState == AboutScreen) {
    m_window.draw(aboutScreenText);
    if (space_pressed) {
        reset_input();
        gameState = Playing;
    }
  }
//  m_window.draw(text);
  m_window.display();//Put everything on the screen
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

void sfml_game::move_camera(sf::Vector2f offset)
{
  //Dont move the camera in the menu
  if (gameState != Playing)
      return;
  m_camera_x += offset.x;
  m_camera_y += offset.y;
}

void sfml_game::process_events()
{
 if ((115/tile_speed != std::abs(std::floor(115/tile_speed)) ||
      115/tile_speed != std::abs(std::ceil(115/tile_speed))) ||
      tile_speed > 115.0) {
   throw std::runtime_error("The set tile speed is not usable");
 }

  if (movecam_r == true)
    move_camera(sf::Vector2f(0.5, 0));
  if (movecam_l == true)
    move_camera(sf::Vector2f(-0.5, 0));
  if (movecam_u == true)
    move_camera(sf::Vector2f(0, -0.5));
  if (movecam_d == true)
    move_camera(sf::Vector2f(0, 0.5));

  if (m_timer > 0) {
    getTileById(m_selected).move();
    m_timer--;
  } else {
    if (!m_selected.empty()) {
      getTileById(m_selected).set_dx(0);
      getTileById(m_selected).set_dy(0);
    }
  }

  m_delegate.do_actions(*this);
  ++m_n_displayed;
}

void sfml_game::process_input()
{
  // check all the window's events that were triggered since the last iteration of the loop
  sf::Event event;
  while (m_window.pollEvent(event))
  {
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
        //Do nothing by default
        break;
    }
  }
}

void sfml_game::process_keyboard_input(const sf::Event& event)
{
  //Only keyboard events
  assert(event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased);

  if (event.type == sf::Event::KeyPressed)
  {
    arrows(true, event);
    if (event.key.code == sf::Keyboard::Space)
        space_pressed = true;
    if (m_selected.size() > 0)
      tile_movement(true, event, getTileById(m_selected));
    if (m_timer > 0)
      tile_movement(false, event, getTileById(m_selected));
  }
  else
  {
    arrows(false, event);
    if (event.key.code == sf::Keyboard::Space)
        space_pressed = false;
  }
}

void sfml_game::reset_input() {
    space_pressed = false;
    movecam_r = false;
    movecam_l = false;
    movecam_u = false;
    movecam_d = false;
}
void sfml_game::process_mouse_input(const sf::Event& event)
{
  //Only mouse input
  assert(event.type == sf::Event::MouseButtonPressed);

  if (event.mouseButton.button == sf::Mouse::Left) {
   std::vector<tile> game_tiles = m_game.get_tiles();
   for (unsigned i = 0; i<game_tiles.size(); i++) {
     if (game_tiles.at(i).tile_contains(
           sf::Mouse::getPosition(m_window).x+m_camera_x,
           sf::Mouse::getPosition(m_window).y+m_camera_y)) {
       m_selected.clear();
       m_selected.push_back(game_tiles.at(i).get_id());
       clicked_tile = true;
     }
    }
   if (clicked_tile == false) {
      m_selected.clear();
    }
    clicked_tile = false;
  }
}

void sfml_game::stop_music()
{
  m_background_music.stop();
}
void sfml_game::show_menu()
{
    gameState = TitleScreen;
}
void sfml_game::arrows(bool b, const sf::Event& event)
{
  if (event.key.code == sf::Keyboard::Right)
      movecam_r = b;
  if (event.key.code == sf::Keyboard::Left)
      movecam_l = b;
  if (event.key.code == sf::Keyboard::Up)
      movecam_u = b;
  if (event.key.code == sf::Keyboard::Down)
      movecam_d = b;
}

void sfml_game::tile_movement(bool b, const sf::Event& event, tile& t)
{
  if (m_timer == 0) {
    if (b == true) {
      if (event.key.code == sf::Keyboard::D)
        t.set_dx(tile_speed);
      if (event.key.code == sf::Keyboard::A)
        t.set_dx(-tile_speed);
      if (event.key.code == sf::Keyboard::W)
        t.set_dy(-tile_speed);
      if (event.key.code == sf::Keyboard::S)
        t.set_dy(tile_speed);
      m_timer += (1/tile_speed)*115;
    } else {
      t.set_dx(0);
      t.set_dy(0);
    }
  }//TODO make it so this executes only when m_timer = 0
}

int sfml_game::vectortoint(std::vector<int> v)
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

tile& sfml_game::getTileById(std::vector<int> tile_id) {
  assert(!tile_id.empty());
  const int id = tile_id[0];
  //if (id > m_game.old_id)
  //  assert(!"Tile id has not been used yet"); //!OCLINT accepted idiom
  for (tile& t: m_game.get_tiles())
  {
    if (t.get_id() == id) {
      return t;
    }
  }
  assert(!"Should never get here"); //!OCLINT accepted idiom
  throw std::runtime_error("ID not found");
}

void sfml_game::color_tile_shape(sf::RectangleShape& sfml_tile, const tile& t) {
  sf::Color outline;
  if (t.get_type() == tile_type::grassland) {
    sfml_tile.setFillColor(sf::Color(0, 255, 0));
    sfml_tile.setOutlineThickness(5); outline = sf::Color(0, 100, 0);
  }
  else if (t.get_type() == tile_type::mountains) {
    sfml_tile.setFillColor(sf::Color(120, 120, 120));
    sfml_tile.setOutlineThickness(5); outline = sf::Color(50, 50, 50);
  }
  else if (t.get_type() == tile_type::ocean) {
    sfml_tile.setFillColor(sf::Color(0, 0, 255));
    sfml_tile.setOutlineThickness(5); outline = sf::Color(0, 0, 100);
  }
  else if (t.get_type() == tile_type::savannah) {
    sfml_tile.setFillColor(sf::Color(235, 170, 0));
    sfml_tile.setOutlineThickness(5); outline = sf::Color(245, 190, 0);
  }
  else if (t.get_type() == tile_type::arctic) {
    sfml_tile.setFillColor(sf::Color(50, 230, 255));
    sfml_tile.setOutlineThickness(5); outline = sf::Color(10, 200, 255);
  } else {
    assert(!"Display of this tile type not implemented yet"); //!OCLINT accepted idiom
  }
  auto selected = vectortoint(m_selected);
  if (t.get_id() == selected) {
      sfml_tile.setOutlineColor(sf::Color(255,255,255));
    } else {
      sfml_tile.setOutlineColor(outline);
    }
}
