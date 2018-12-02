#ifndef SFML_GAME_DELEGATE_H
#define SFML_GAME_DELEGATE_H

class sfml_game;

/// This class will modify the SFML game when needed.
/// It will be called each game cycle once.
class sfml_game_delegate {
public:
  ///@param close_at number of frames at which the sfml_game
  ///  will be closed, which is useful in testing. Set to a negative
  ///  value to let the game run indefinitely
  sfml_game_delegate(const int close_at = -1);

  /// Do possible actions on the sfml_game. This member
  /// function is called once each game cycle.
  ///@param sg the sfml_game to be modified (e.g. closed) or ignored (e.g.
  ///   this class has no tasks anymore)
  ///@return true if it has modified the sfml_game, false otherwise
  bool do_actions(sfml_game &sg);

  /// Get the frame number this class will close an sfml_game
  int get_close_at() const noexcept { return m_close_at; }

private:
  /// The number of frames at which the sfml_game will be closed.
  /// A negative value lets the game run indefinitely
  const int m_close_at;
};

/// Tests this class
void test_sfml_game_delegate();

#endif // SFML_GAME_DELEGATE_H
