#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP
#include "button_state.hpp"

#define NOMINMAX
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include <memory>
#include <string>
#include <string_view>

class JoystickException : public std::exception
{
  public:
    explicit JoystickException(const char *message);
};

class Joystick
{
  public:
    explicit Joystick(
        LPDIRECTINPUTDEVICE8 inputDevice,
        std::string_view deviceName);
    ~Joystick();

    const ButtonState &poll();

    const std::string name;

  private:
    [[nodiscard]] size_t countButtons() const;
    [[nodiscard]] DIJOYSTATE2 getState() const;
    void processButton(int index, bool isPressed);

    LPDIRECTINPUTDEVICE8 joystick;
    ButtonState button_state;
};

#endif // JOYSTICK_HPP
