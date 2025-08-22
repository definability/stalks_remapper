#include "direct_input_wrapper.hpp"
#include "gamepad_emulator.hpp"
#include "gamepad_state.hpp"
#include "joystick.hpp"

#include <chrono>
#include <csignal>
#include <iostream>
#include <limits>
#include <string_view>
#include <thread>

using std::string_view_literals::operator""sv;
using std::chrono_literals::operator""ms;

constexpr auto reconnectionDelay = 100ms;
constexpr auto restTime = 30ms;

int main()
try
{
    // GamepadEmulator emulator;
    // std::cout << "Initialise" << std::endl;
    // std::this_thread::sleep_for(3000ms);
    // emulator.update(0);
    // for (uint16_t i = 0; i < 16; ++i)
    // {
    //     auto buttons = static_cast<uint16_t>(1 << i);
    //     std::cout << "Press " << i << " (" << std::hex << buttons << ')' << std::endl;
    //     emulator.update(buttons);
    //     std::this_thread::sleep_for(3000ms);
    // }
    std::cout << "Initialising..." << std::endl;
    DirectInputWrapper directInput;
    auto &&stalks = directInput.getDevice("MOZA Multi-function Stalk"sv);
    std::cout << stalks.name << " is ready" << std::endl;
    auto &&handbrake = directInput.getDevice("HBP Handbrake"sv);
    std::cout << handbrake.name << " is ready" << std::endl;
    GamepadEmulator emulator;
    GamepadState state;

    static auto running{true};
    std::signal(SIGTERM, [](int) { running = false; });
    std::signal(SIGINT, [](int) { running = false; });
    unsigned long previousState{};

    std::cout << "Emulator is ready" << std::endl
              << std::endl
              << "Press Ctrl+C to exit" << std::endl;
    while (running)
    {
        try
        {
            auto &&stalks_state = stalks.poll();
            state.stalks(stalks_state);
        }
        catch (const JoystickException &exception)
        {
            std::cout << stalks.name << " poll failed: " << exception.what()
                      << std::endl
                      << " Try again." << std::endl;
            std::this_thread::sleep_for(reconnectionDelay);
            continue;
        }

        try
        {
            auto &&handbrakeState = handbrake.poll();
            state.handbrake(handbrakeState);
        }
        catch (const JoystickException &exception)
        {
            std::cout << handbrake.name << " poll failed: " << exception.what()
                      << std::endl
                      << " Try again." << std::endl;
            std::this_thread::sleep_for(reconnectionDelay);
            continue;
        }

        auto &&buttons = state.getButtons();
        auto &&currentState = buttons.to_ulong();
        if (currentState != previousState)
        {
            previousState = currentState;
            std::cout << std::chrono::system_clock::now() << ": " << buttons
                      << std::endl;
        }
        if (currentState > std::numeric_limits<uint16_t>::max())
        {
            std::cerr << "Unavailable state 0x" << std::hex << currentState
                      << std::dec << std::endl;
        }
        emulator.update(currentState);

        std::this_thread::sleep_for(restTime);
    }

    std::cout << "Exiting..." << std::endl;
    return EXIT_SUCCESS;
}
catch (const JoystickException &exception)
{
    std::cerr << "Joystick exception: " << exception.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const GamepadEmulatorException &exception)
{
    std::cerr << "ViGEM exception: " << exception.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception &exception)
{
    std::cerr << "Unhandled exception: " << exception.what() << std::endl;
    return EXIT_FAILURE;
}
catch (...)
{
    std::cerr << "Unknown exception" << std::endl;
    return EXIT_FAILURE;
}
