#include "gamepad_emulator.hpp"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ViGEm/Client.h>

GamepadEmulatorException::GamepadEmulatorException(const char *message)
    : std::exception{message}
{
}

GamepadEmulator::GamepadEmulator()
    : client{vigem_alloc()}
    , target{vigem_target_x360_alloc()}
{
    if (!VIGEM_SUCCESS(vigem_connect(client)))
    {
        throw GamepadEmulatorException("Cannot connect to bus");
    }
    if (!VIGEM_SUCCESS(vigem_target_add(client, target)))
    {
        throw GamepadEmulatorException("Cannot add target");
    }
}

GamepadEmulator::~GamepadEmulator()
{
    if (client != nullptr && target != nullptr)
    {
        vigem_target_remove(client, target);
    }
    if (target != nullptr)
    {
        vigem_target_free(target);
    }
    if (client != nullptr)
    {
        vigem_disconnect(client);
        vigem_free(client);
    }
}

void GamepadEmulator::update(const uint16_t buttons) const
{
    if (!VIGEM_SUCCESS(
            vigem_target_x360_update(client, target, {.wButtons = buttons})))
    {
        throw GamepadEmulatorException("Cannot send buttons");
    }
}
