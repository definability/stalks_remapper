#ifndef DI_JOYSTICK_HPP
#define DI_JOYSTICK_HPP
#include "button_state.hpp"

#define NOMINMAX
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <memory>
#include <string>
#include <string_view>

class DirectInputJoystick;

class JoystickException : public std::exception
{
public:
    explicit JoystickException(const char* message);
};

class Joystick
{
public:
    explicit Joystick(LPDIRECTINPUTDEVICE8 inputDevice, std::string_view
                      deviceName
    );
    ~Joystick();

    const ButtonState& poll();

    const std::string name;
private:
    size_t countButtons() const;
    void processButton(int i, bool isPressed);
    DIJOYSTATE2 getState() const;

    LPDIRECTINPUTDEVICE8 joystick;
    ButtonState button_state;
};

#endif //DI_JOYSTICK_HPP
