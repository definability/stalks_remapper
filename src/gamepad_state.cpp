#include "gamepad_state.hpp"

#include "button_state.hpp"

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <ViGEm/Common.h>

#include <bit>
#include <bitset>
#include <utility>
#include <vector>

namespace
{
enum class ButtonName
{
#define BUTTON_INDEX(BUTTON)                                                   \
    std::countr_zero(static_cast<uint16_t>(XUSB_GAMEPAD_##BUTTON))
    light = BUTTON_INDEX(DPAD_UP),
    farLight = BUTTON_INDEX(DPAD_DOWN),
    blinkerLeft = BUTTON_INDEX(DPAD_LEFT),
    blinkerRight = BUTTON_INDEX(DPAD_RIGHT),
    wiper = BUTTON_INDEX(START),
    handbrake = BUTTON_INDEX(BACK),
    cruiseControl = BUTTON_INDEX(A),
    cruiseIncrease = BUTTON_INDEX(B),
    cruiseDecrease = BUTTON_INDEX(X),
    cruiseCancel = BUTTON_INDEX(Y),
#undef BUTTON_INDEX
};
} // namespace

void GamepadState::stalks(const ButtonState &controller)
{
    remapBlinkers(controller);
    remapLights(controller);
    remapWipers(controller);
    remapCruiseControl(controller);
}

void GamepadState::handbrake(const ButtonState &controller)
{
    static constexpr auto handbrakeButton = 43;
    this->buttons[std::to_underlying(ButtonName::handbrake)] =
        controller.buttons[handbrakeButton];
}

const std::bitset<GamepadState::buttonCount> &GamepadState::getButtons() const
{
    return this->buttons;
}

void GamepadState::remapBlinkers(const ButtonState &controller)
{
    static constexpr auto leftBlinker = 9;
    static constexpr auto rightBlinker = 7;
    static constexpr auto offBlinker = 8;

    if (controller.justPressed[offBlinker])
    {
        this->targetBlinker = BlinkerState::off;
    }
    else if (controller.justPressed[leftBlinker])
    {
        this->targetBlinker = BlinkerState::left;
    }
    else if (controller.justPressed[rightBlinker])
    {
        this->targetBlinker = BlinkerState::right;
    }

    if (this->currentBlinker == this->targetBlinker)
    {
        this->buttons[std::to_underlying(ButtonName::blinkerLeft)] = false;
        this->buttons[std::to_underlying(ButtonName::blinkerRight)] = false;
        return;
    }

    switch (this->targetBlinker)
    {
    case BlinkerState::left:
        if (this->buttons[std::to_underlying(ButtonName::blinkerLeft)])
        {
            this->buttons[std::to_underlying(ButtonName::blinkerLeft)] = false;
        }
        else
        {
            this->buttons[std::to_underlying(ButtonName::blinkerLeft)] = true;
            this->buttons[std::to_underlying(ButtonName::blinkerRight)] = false;
            this->currentBlinker = BlinkerState::left;
        }
        break;
    case BlinkerState::right:
        if (this->buttons[std::to_underlying(ButtonName::blinkerRight)])
        {
            this->buttons[std::to_underlying(ButtonName::blinkerRight)] = false;
        }
        else
        {
            this->buttons[std::to_underlying(ButtonName::blinkerRight)] = true;
            this->buttons[std::to_underlying(ButtonName::blinkerLeft)] = false;
            this->currentBlinker = BlinkerState::right;
        }
        break;
    case BlinkerState::off:
        if (this->buttons[std::to_underlying(ButtonName::blinkerLeft)])
        {
            this->buttons[std::to_underlying(ButtonName::blinkerLeft)] = false;
        }
        else if (this->currentBlinker == BlinkerState::left)
        {
            this->buttons[std::to_underlying(ButtonName::blinkerLeft)] = true;
            this->currentBlinker = BlinkerState::off;
        }
        if (this->buttons[std::to_underlying(ButtonName::blinkerRight)])
        {
            this->buttons[std::to_underlying(ButtonName::blinkerRight)] = false;
        }
        else if (this->currentBlinker == BlinkerState::right)
        {
            this->buttons[std::to_underlying(ButtonName::blinkerRight)] = true;
            this->currentBlinker = BlinkerState::off;
        }
        break;
    }
}

void GamepadState::remapLights(const ButtonState &controller)
{
    static constexpr auto lightOff{0};
    static constexpr auto lightPark{1};
    static constexpr auto lightMain{2};
    static constexpr auto lightDipped{3};

    static constexpr auto lightDippedTemporaryOff = 4;
    static constexpr auto lightDippedTemporaryOn = 5;

    if (controller.justPressed[lightOff])
    {
        this->targetLightIntensity = LightIntensity::off;
    }
    else if (controller.justPressed[lightPark])
    {
        this->targetLightIntensity = LightIntensity::park;
    }
    else if (controller.justPressed[lightMain])
    {
        this->targetLightIntensity = LightIntensity::main;
    }
    else if (
        controller.justPressed[lightDipped] &&
        this->currentLightIntensity == this->targetLightIntensity)
    {
        if (this->targetLightIntensity == LightIntensity::main)
        {
            this->targetLightIntensity = LightIntensity::dipped;
        }
        else if (this->targetLightIntensity == LightIntensity::dipped)
        {
            this->targetLightIntensity = LightIntensity::main;
        }
    }

    if (this->currentLightIntensity == LightIntensity::main &&
            this->targetLightIntensity == LightIntensity::dipped ||
        this->currentLightIntensity == LightIntensity::dipped &&
            this->targetLightIntensity == LightIntensity::main)
    {
        if (this->buttons[std::to_underlying(ButtonName::farLight)])
        {
            this->buttons[std::to_underlying(ButtonName::farLight)] = false;
            this->currentLightIntensity = this->targetLightIntensity;
        }
        else
        {
            this->buttons[std::to_underlying(ButtonName::farLight)] = true;
        }
        return;
    }

    if (this->targetLightIntensity != this->currentLightIntensity)
    {
        this->buttons[std::to_underlying(ButtonName::farLight)] = false;
        if (this->buttons[std::to_underlying(ButtonName::light)])
        {
            this->buttons[std::to_underlying(ButtonName::light)] = false;
            this->currentLightIntensity = static_cast<LightIntensity>(
                (std::to_underlying(this->currentLightIntensity) + 1) %
                (std::to_underlying(LightIntensity::main) + 1));
        }
        else
        {
            this->buttons[std::to_underlying(ButtonName::light)] = true;
        }
        return;
    }

    if (controller.buttons[lightDippedTemporaryOn] &&
        this->targetLightIntensity == this->currentLightIntensity &&
        (this->currentLightIntensity == LightIntensity::off ||
         this->currentLightIntensity == LightIntensity::park))
    {
        this->buttons[std::to_underlying(ButtonName::farLight)] = true;
    }
    if (this->buttons[std::to_underlying(ButtonName::farLight)] &&
        controller.buttons[lightDippedTemporaryOff])
    {
        this->buttons[std::to_underlying(ButtonName::farLight)] = false;
    }
}

void GamepadState::remapWipers(const ButtonState &controller)
{
    static constexpr auto wipersOff{20};
    static constexpr auto wipersIntermittent{21};
    static constexpr auto wipersSlow{22};
    static constexpr auto wipersFast{23};
    static constexpr auto wipersMist{19};

    if (controller.justPressed[wipersOff])
    {
        this->targetWiperState = WiperState::off;
    }
    else if (
        controller.justPressed[wipersIntermittent] ||
        controller.justPressed[wipersMist])
    {
        this->targetWiperState = WiperState::intermittent;
    }
    else if (controller.justPressed[wipersSlow])
    {
        this->targetWiperState = WiperState::slow;
    }
    else if (controller.justPressed[wipersFast])
    {
        this->targetWiperState = WiperState::fast;
    }

    if (this->targetWiperState != this->currentWiperState)
    {
        if (this->buttons[std::to_underlying(ButtonName::wiper)])
        {
            this->buttons[std::to_underlying(ButtonName::wiper)] = false;
            this->currentWiperState = static_cast<WiperState>(
                (std::to_underlying(this->currentWiperState) + 1) %
                std::to_underlying(WiperState::count));
        }
        else
        {
            this->buttons[std::to_underlying(ButtonName::wiper)] = true;
        }
    }
}

void GamepadState::remapCruiseControl(const ButtonState &controller)
{
    static constexpr auto cruiseControl{24};
    static constexpr auto cruiseIncrease{26};
    static constexpr auto cruiseDecrease{25};
    static constexpr auto cruiseCancel{27};
    this->buttons[std::to_underlying(ButtonName::cruiseControl)] =
        controller.buttons[cruiseControl];
    this->buttons[std::to_underlying(ButtonName::cruiseIncrease)] =
        controller.buttons[cruiseIncrease];
    this->buttons[std::to_underlying(ButtonName::cruiseDecrease)] =
        controller.buttons[cruiseDecrease];
    this->buttons[std::to_underlying(ButtonName::cruiseCancel)] =
        controller.buttons[cruiseCancel];
}
