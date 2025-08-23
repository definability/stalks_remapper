# 1.0.0

## Features

Implement a remapper for [City Car Driving][city-car-driving].
Simulate an Xbox 360 gamepad and remap the following buttons onto it:
- Turning indicators (blinkers): left, right, cancel.
- Light modes: off, park, main.
- Far light: on, off, horn (pull the lever to blink).
- Wipers: off, intermittent, slow, fast, mist
  (hold the lever down to keep the wipers on for a while).
- Handbrake: on, off.
- Cruise control: on/off, increase/decrease.
  Not supported by the game, so it can be mapped to something else.

## Mapping

If you wish to check whether the application works correctly,
these are the buttons that must be emitted
when you push the corresponding levers of your
[MOZA Multi-functional stalks][moza-stalks]
or [MOZA HBP Handbrake][moza-hbp]

- light: D-pad up,
- far light: D-pad down,
- left turn indicator: D-pad left,
- right turn indicator: D-pad right,
- wiper: Start,
- handbrake: Back,
- enable cruise control: A,
- cruise control increase speed: B,
- cruise control decrease speed: X,
- disable cruise control: Y.

[city-car-driving]: https://store.steampowered.com/app/493490/City_Car_Driving/
[moza-stalks]: https://uk.mozaracing.com/products/multi-function-stalks
[moza-hbp]: https://mozaracing.com/products/hbp-handbrake
