#ifndef BUTTON_STATE_HPP
#define BUTTON_STATE_HPP
#include <vector>

struct ButtonState
{
    explicit ButtonState(size_t buttonCount);

    std::vector<bool> buttons;
    std::vector<bool> justPressed;
    std::vector<bool> justDepressed;

    const size_t size;
};

#endif //BUTTON_STATE_HPP
