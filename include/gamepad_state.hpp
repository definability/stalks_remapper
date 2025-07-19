#ifndef GAMEPAD_STATE_HPP
#define GAMEPAD_STATE_HPP
#include "button_state.hpp"

#include <bitset>

enum class ButtonName
{
    light,
    farLight,
    blinkerLeft,
    blinkerRight,
    wiper,
    cruiseControl,
    cruiseIncrease,
    cruiseDecrease,
    cruiseCancel,
    handbrake,
};

enum class LightIntensity
{
    off,
    park,
    main,
    dipped,
};

enum class BlinkerState
{
    off,
    left,
    right,
};

enum class WiperState
{
    off,
    intermittent,
    slow,
    fast,
    count,
};

class GamepadState
{
public:
    static constexpr size_t buttonCount = 16;

    void stalks(const ButtonState &controller);
    void handbrake(const ButtonState &controller);

    [[nodiscard]] const std::bitset<buttonCount>& getButtons() const;

private:
    void remapBlinkers(const ButtonState &controller);
    void remapLights(const ButtonState &controller);
    void remapWipers(const ButtonState &controller);
    void remapCruiseControl(const ButtonState &controller);

    std::bitset<buttonCount> buttons;

    BlinkerState currentBlinker{BlinkerState::off};
    BlinkerState targetBlinker{BlinkerState::off};
    LightIntensity currentLightIntensity{LightIntensity::off};
    LightIntensity targetLightIntensity{LightIntensity::off};
    WiperState currentWiperState{WiperState::off};
    WiperState targetWiperState{WiperState::off};
};

#endif // GAMEPAD_STATE_HPP
