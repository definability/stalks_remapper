# Stalks Remapper

[![CMake on a single platform][badge]][badge-status]

[MOZA Multi-functional stalks][moza-stalks] and [MOZA HBP Handbrake][moza-hbp]
are expensive controllers
and [City Car Driving][city-car-driving] is a niche driving simulator
with a lot of possibilities and VR support.

Regrettably, neither the game developers nor the hardware vendor
do anything to fix the compatibility.

## How to use

**Do not launch the `stalks_remapper.exe` until the game map is loaded.**

The remapper uses XBox 360 gamepad emulator, which supports force feedback.
When [City Car Driving][city-car-driving] initialises force feedback
and detects this virtual gamepad,
the game may use this device as the only force feedback receiver.
This means that, if your steering wheel supports force feedback,
you may not feed it if `stalks_remapper.exe` was running
when the game level was loading.

TL;DR: launch `stalks_remapper.exe` only when the game level is loaded,
and you see your car's cockpit.
There is no need to relaunch the remapper when you switch to another level.

### Setup

1. [Build](#how-to-build) or download `stalks_remapper.exe`.
2. Install [ViGEmBus] and restart your computer.
   This is a driver that allows to create a virtual joystick
   the remapper uses to remap the controller keys.
3. Install [HidHide].
4. If you did not connect your
   [MOZA Multi-functional stalks][moza-stalks]
   and [MOZA HBP Handbrake][moza-hbp], connect them now.
   The handbrake must be connected directly via USB.
   Set the handbrake not as an axis but as a button
   in your [MOZA Pit House][moza-pit-house].
5. In [HidHide], either hide the controllers
   from [City Car Driving][city-car-driving]
   (recommended because will not affect the other games)
   or whitelist the `stalks_remapper.exe` (and other games) to use them.

### Play

1. Launch the game.
2. Set up the desired controls (you can do this anytime).
3. Load a challenge or a free driving mode.
4. Launch `stalks_remapper.exe`.
   Do not exit the game while launching the remapper.
   Simply `Alt-Tab` or `Win` from the game to your OS,
   launch `stalks_remapper.exe`, and return to the game.

If you turn off the game, turn off the remapper as well.
If you finish a challenge or free drive and wish to switch to another challenge,
there is **no need** to stop the remapper.

## Supported keys

- Turning indicators (blinkers): left, right, cancel.
- Light modes: off, park, main.
- Far light: on, off, horn (pull the lever to blink).
- Wipers: off, intermittent, slow, fast, mist
  (hold the lever down to keep the wipers on for a while).
- Handbrake: on, off.
- Cruise control: on/off, increase/decrease.
  Not supported by the game, so can be mapped to something else.

## How to build

The project uses [submodules][git-submodule].
Make sure you clone it right:
```shell
git clone --recursive https://github.com/definability/stalks_remapper
```

Nothing special is required to build the project.
The ordinary [Visual Studio Build Tools][vs-build-tools] suffices.

The project is CMake-based.
You can use an IDE that supports such projects or build it manually:
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build buid
```

The executable should be available at `build/stalks_remapper.exe`.

## Alternative

This repository is not the first community attempt
to make the controller working in the game.
There is already a [Python script][python-remapper],
which requires [vJoy] and [Joystick Gremlin][joystick-gremlin].

The solution in this repository requires [ViGEmBus] and [HidHide].
It is more low-level and thus complicated, but may be faster.

The projects both solutions rely on are technically dead.
However, they are downloadable and working.

[badge]: https://github.com/definability/stalks_remapper/actions/workflows/cmake-single-platform.yml/badge.svg
[badge-status]: https://github.com/definability/stalks_remapper/actions/workflows/cmake-single-platform.yml
[city-car-driving]: https://store.steampowered.com/app/493490/City_Car_Driving/
[CMake]: https://cmake.org/download/
[git-submodule]: https://git-scm.com/book/en/v2/Git-Tools-Submodules
[HidHide]: https://github.com/nefarius/HidHide
[joystick-gremlin]: https://whitemagic.github.io/JoystickGremlin/
[moza-stalks]: https://uk.mozaracing.com/products/multi-function-stalks
[moza-hbp]: https://mozaracing.com/products/hbp-handbrake
[moza-pit-house]: https://mozaracing.com/pages/pit-house
[python-remapper]: https://steamcommunity.com/app/493490/discussions/0/599650845622041126/
[ViGEmBus]: https://github.com/nefarius/ViGEmBus
[vJoy]: https://sourceforge.net/projects/vjoystick/
[vs-build-tools]: https://visualstudio.microsoft.com/visual-cpp-build-tools/
