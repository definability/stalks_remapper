#include "joystick.hpp"

namespace
{
LPDIRECTINPUTDEVICE8 createJoystick(LPDIRECTINPUTDEVICE8 joystick);

BOOL CALLBACK
CountButtonsCallback(LPCDIDEVICEOBJECTINSTANCE device, VOID *pContext);
} // namespace

JoystickException::JoystickException(const char *const message)
    : std::exception{message}
{
}

Joystick::Joystick(
    const LPDIRECTINPUTDEVICE8 inputDevice,
    const std::string_view deviceName)
    : name{deviceName}
    , joystick{createJoystick(inputDevice)}
    , button_state{this->countButtons()}
{
}

Joystick::~Joystick()
{
    if (this->joystick)
    {
        this->joystick->Unacquire();
        this->joystick->Release();
    }
}

size_t Joystick::countButtons() const
{
    size_t buttonCount{};
    auto &&status = this->joystick->EnumObjects(
        CountButtonsCallback, &buttonCount, DIDFT_ALL);
    if (FAILED(status))
    {
        throw JoystickException("Failed to enumerate Joysticks");
    }
    return buttonCount;
}

const ButtonState &Joystick::poll()
{
    auto &&joystickState = this->getState();
    for (auto i = 0; i < this->button_state.size; i++)
    {
        this->processButton(i, (joystickState.rgbButtons[i] & 0x80) != 0);
    }
    return this->button_state;
}

void Joystick::processButton(const int index, const bool isPressed)
{
    this->button_state.justPressed[index] = false;
    this->button_state.justDepressed[index] = false;

    if (isPressed && !this->button_state.buttons[index])
    {
        this->button_state.justPressed[index] = true;
    }
    else if (!isPressed && this->button_state.buttons[index])
    {
        this->button_state.justDepressed[index] = true;
    }

    this->button_state.buttons[index] = isPressed;
}

DIJOYSTATE2 Joystick::getState() const
{
    if (FAILED(this->joystick->Poll()))
    {
        if (FAILED(this->joystick->Acquire()))
        {
            throw JoystickException("Failed to acquire joystick");
        }
    }
    DIJOYSTATE2 joystickState;
    if (FAILED(this->joystick->GetDeviceState(
            sizeof(DIJOYSTATE2), &joystickState)))
    {
        throw JoystickException("Failed to get Joystick state");
    }
    return joystickState;
}

namespace
{
LPDIRECTINPUTDEVICE8 createJoystick(const LPDIRECTINPUTDEVICE8 joystick)
{
    if (FAILED(joystick->SetDataFormat(&c_dfDIJoystick2)))
    {
        throw JoystickException("Failed to set data format");
    }
    if (FAILED(joystick->SetCooperativeLevel(
            GetConsoleWindow(), DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
    {
        throw JoystickException("Failed to set cooperative level");
    }

    if (FAILED(joystick->Acquire()))
    {
        throw JoystickException("Failed to acquire joystick");
    }
    return joystick;
}

BOOL CALLBACK CountButtonsCallback(
    const LPCDIDEVICEOBJECTINSTANCE device, VOID *const pContext)
{
    auto &&buttonCount = static_cast<size_t *>(pContext);
    if (device->dwType & DIDFT_BUTTON)
    {
        (*buttonCount)++;
    }
    return DIENUM_CONTINUE;
}
} // namespace
