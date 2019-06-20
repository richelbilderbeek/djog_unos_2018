#ifndef BIOLOGY_H
#define BIOLOGY_H

/// Class that contains all biological constants,
/// for example, initial health, health increase
/// of plants, health increase when eating a prey,
/// health decrease when being eaten, etc.
class biology
{
public:
  /// Constructor
  biology();

  double get_health_increase_when_eating() const noexcept;

  double get_stamina_increase_when_eating() const noexcept;

  double get_health_decrease_when_eaten() const noexcept;

  double get_grass_reproduction_health() const noexcept;

};


/// Test the biological constants
void test_biology();

#endif // BIOLOGY_H
