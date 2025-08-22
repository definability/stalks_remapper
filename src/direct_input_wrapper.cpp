#include "direct_input_wrapper.hpp"

#define DIRECTINPUT_VERSION 0x0800
#include "joystick.hpp"

#include <dinput.h>

namespace
{
BOOL CALLBACK enumerateJoystickCallback(
    const DIDEVICEINSTANCE *currentJoystick, VOID *pContext);
}

DirectInputWrapper::DirectInputWrapper()
{
    auto &&winApiSuccess = DirectInput8Create(
        GetModuleHandle(nullptr),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        reinterpret_cast<VOID **>(&this->directInput),
        nullptr);
    if (FAILED(winApiSuccess))
    {
        this->directInput = nullptr;
        throw JoystickException("Cannot create direct input");
    }
}

DirectInputWrapper::~DirectInputWrapper()
{
    if (this->directInput != nullptr)
    {
        this->directInput->Release();
    }
}

Joystick DirectInputWrapper::getDevice(const std::string_view name)
{
    LPDIRECTINPUTDEVICE8 device{};
    auto &&context = std::make_tuple(directInput, &device, name);
    auto &&winApiSuccess = directInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL,
        enumerateJoystickCallback,
        &context,
        DIEDFL_ATTACHEDONLY);
    if (FAILED(winApiSuccess) || device == nullptr)
    {
        throw JoystickException{"Device was not found"};
    }
    return Joystick{{}, device, name};
}

namespace
{
BOOL CALLBACK enumerateJoystickCallback(
    const DIDEVICEINSTANCE *const currentJoystick, VOID *const pContext)
{
    auto &&[directInput, device, name] = *static_cast<std::tuple<
        LPDIRECTINPUT8,
        LPDIRECTINPUTDEVICE8 *,
        const std::string_view> *>(pContext);
    if (name == currentJoystick->tszProductName)
    {
        auto &&success = directInput->CreateDevice(
            currentJoystick->guidInstance, device, nullptr);
        if (FAILED(success))
        {
            return DIENUM_CONTINUE;
        }

        return DIENUM_STOP; // Stop enumeration
    }

    return DIENUM_CONTINUE;
}
} // namespace
