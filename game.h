#ifndef GAME_H
#define GAME_H

#include <sfml_game.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


//Dont type every time sf::
using namespace sf;
//Dont type every time std::
using namespace std;

class game
{

public:
    ///Initialize game with arguments
    game(const std::vector<std::string>& args = {});

    ///Get how many times the sfml_game has been displayed on screen.
    ///Will be approximately 60 times per second.
    int get_n_displayed() const noexcept { return m_n_displayed; }

    ///Get how many times the sfml_game has been displayed on screen.
    ///Will be approximately 60 times per second.
    int get_n_displayed_max() const noexcept { return m_n_displayed_max; }

    ///sfml_game object
    sfml_game sfml_game_object;

    int exec();
private:
    ///Screen sizes
    int width;
    int height;

    //Key input bools
    bool upPressed, rightPressed, downPressed, leftPressed;
    //Speed Camera is moving
    float moveSpeed = 1.5f;

    ///Background music file object
    sf::Music background_music;

    ///Timer, physics, bullets moving, etc.
    ///Everything except user input.
    void process_events();

    ///Key and mouse handling
    void process_input();

    ///This should happen every frame and contains logic
    void update();

    ///This should happen in every case every frame
    void fixed_update(){
        ++m_n_displayed;
    }

    ///This should happen when the game ends
    void end();

    ///The number of times the sfml_game is displayed on screen
    ///Should be approx 60 times per second
    int m_n_displayed{0};

    ///The number of times the sfml_game is displayed before it closes.
    ///This is usefull in testing.
    ///Will be negative if the sfml_game has no time limit
    const int m_n_displayed_max;

};

///Extract the maximum number of times the sfml_game will be displayed
//STUB: returns 100 by default for now, new dafault must be -1
int extract_n_displayed_max(const std::vector<std::string>& args);

#endif // GAME_H
