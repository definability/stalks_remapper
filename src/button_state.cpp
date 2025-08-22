#include "button_state.hpp"

ButtonState::ButtonState(const size_t buttonCount)
    : buttons(buttonCount)
    , justPressed(buttonCount)
    , justDepressed(buttonCount)
    , size{buttonCount}
{
}

// for (auto i = 0; i < this->size; i++)
// {
//     if (this->justPressed[i])
//     {
//         std::cout << "Pressed   " << i << std::endl;
//     }
//     if (this->justDepressed[i])
//     {
//         std::cout << "Depressed " << i << std::endl;
//     }
// }
