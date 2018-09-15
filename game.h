#ifndef GAME_H
#define GAME_H

#include <sfml_game.h>
#include <SFML/Graphics.hpp>

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

    ///Timer, physics, bullets moving, etc.
    ///Everything except user input.
    void process_events();

    ///Key and mouse handling
    void process_input();

    ///This should happen every frame and contains logic
    void update();

    ///The number of times the sfml_game is displayed on screen
    ///Should be approx 60 times per second
    int m_n_displayed{0};

    ///The number of times the sfml_game is displayed before it closes.
    ///This is usefull in testing.
    ///Will be negative if the sfml_game has no time limit
    const int m_n_displayed_max;

};
#endif // GAME_H
