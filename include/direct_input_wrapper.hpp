#ifndef DIRECT_INPUT_WRAPPER_HPP
#define DIRECT_INPUT_WRAPPER_HPP
#include "joystick.hpp"

#define NOMINMAX
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <string>

class DirectInputWrapper
{
public:
    DirectInputWrapper();
    ~DirectInputWrapper();
    Joystick getDevice(std::string_view name);

private:
    LPDIRECTINPUT8 directInput{};
};

#endif //DIRECT_INPUT_WRAPPER_HPP
