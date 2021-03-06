#ifndef SFML_ZEN_BAR_H
#define SFML_ZEN_BAR_H

#include <SFML/Graphics.hpp>

class sfml_zen_bar
{
public:
    sf::RectangleShape get_drawable_bar(float x_pos, float y_pos, sf::RenderWindow &window);
    sf::RectangleShape get_drawable_ind(float x_pos, float y_pos, sf::RenderWindow &window);
    void set_score(const double score);
    sfml_zen_bar();
private:

    sf::RectangleShape m_zen_bar;
    sf::RectangleShape m_zen_icon;

    double m_zen_balance;
};

#endif // SFML_ZEN_BAR_H
