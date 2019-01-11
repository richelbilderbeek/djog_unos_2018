#include "sfml_window_manager.h"
#include <cassert>

sfml_window_manager *sfml_window_manager::m_instance = nullptr; //!OCLINT static accepted singleton

sfml_window_manager::sfml_window_manager()
{

}

sf::RenderWindow &sfml_window_manager::get() {
  if (!m_instance) {
    m_instance = new sfml_window_manager();
  }
  assert(m_instance);
  return m_instance->m_window;
}
