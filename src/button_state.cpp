#include "button_state.hpp"

ButtonState::ButtonState(const size_t buttonCount)
  : buttons(buttonCount)
, justPressed(buttonCount)
, justDepressed(buttonCount)
, size{buttonCount}
{}
