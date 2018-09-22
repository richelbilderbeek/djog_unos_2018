#include <game.h>

game::game(const std::vector<std::string>& args)
  : sfml_game_object(800, 600),
    m_n_displayed_max{extract_n_displayed_max(args)}
{

}

int game::exec()
{
    background_music.setLoop(true);
    if (!background_music.openFromFile("background_music.ogg"))
        return 1;
    background_music.play();
    sfml_game_object.init();
    while (sfml_game_object.m_window.isOpen())
    {
      fixed_update();
      process_input();
      process_events();
      update();
      sfml_game_object.display();
    }

    return 0;
}

void game::update(){
    sf::RectangleShape shape(sf::Vector2f(400, 200));
      shape.setPosition(200, 200);
      shape.setFillColor(sf::Color(0, 255, 0));
      shape.setOutlineThickness(10);
      shape.setOutlineColor(sf::Color(0, 100, 0));
    sfml_game_object.add_shape(shape);


    //Move the camera
    Vector2f move = Vector2f(0,0);
    //Return when noting is pressed (SOLVES A BUG)
    if (!upPressed && !downPressed && !rightPressed && !leftPressed) {
        move = Vector2f(0,0);
        return;
    }
   if (upPressed)
       move.y = moveSpeed;
   if (downPressed)
       move.y = -moveSpeed;
   if (rightPressed)
       move.x = -moveSpeed;
   if (leftPressed)
       move.x = moveSpeed;
   cout << upPressed << " " << rightPressed << " " << downPressed << " " << leftPressed << endl;
   //Apply movement
   sfml_game_object.move_camera(move);
}

void game::process_events()
{
  //Close if frames are limited
  if (m_n_displayed_max > 0 && m_n_displayed == m_n_displayed_max)
  {
    end();
  }
}

void game::process_input()
{
  // check all the window's events that were triggered since the last iteration of the loop
  sf::Event event;
  while (sfml_game_object.m_window.pollEvent(event))
  {
      switch (event.type) {
          case sf::Event::Closed:
              end();
              break;

          case sf::Event::KeyPressed:
              if (event.key.code == sf::Keyboard::Right)
                  rightPressed = true;
              if (event.key.code == sf::Keyboard::Left)
                  leftPressed = true;
              if (event.key.code == sf::Keyboard::Up)
                  upPressed = true;
              if (event.key.code == sf::Keyboard::Down)
                  downPressed = true;
              break;
          case sf::Event::KeyReleased:
              if (event.key.code == sf::Keyboard::Right)
                  rightPressed = false;
              if (event.key.code == sf::Keyboard::Left)
                  leftPressed = false;
              if (event.key.code == sf::Keyboard::Up)
                  upPressed = false;
              if (event.key.code == sf::Keyboard::Down)
                  downPressed = false;
          case sf::Event::MouseButtonPressed:
              if (event.mouseButton.button == sf::Mouse::Left && background_music.getStatus() != sf::Music::Playing)
                  background_music.play();
              if (event.mouseButton.button == sf::Mouse::Right && background_music.getStatus() != sf::Music::Paused)
                  background_music.pause();
          default:
              //Do nothing by default
              break;
      }
  }
}

void game::end()
{
    background_music.stop();
    sfml_game_object.m_window.close();
}

int extract_n_displayed_max(const std::vector<std::string>& /* args */)
{
//  //STUB
//  return 100;
      //Default
      return -1;
}
