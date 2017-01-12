# teensy_classic_xinput
Classic Controller -> XInput dongle built with Teensy-LC microcontroller and Arduino.

Requires Teensyduino to be modded with [MSF-XINPUT](https://github.com/zlittell/MSF-XINPUT).

Code originally adapted from [https://havencking.blogspot.com/2015/12/teensy-usb-wii-classic-controller.html](https://havencking.blogspot.com/2015/12/teensy-usb-wii-classic-controller.html)

Tested to work with original Classic Controller, Classic Controller Pro and NES Classic Mini Controller.

# Features
- All buttons work and are matched to same positions as XBox Controller
- Sticks work and are automatically calibrated to reach max range of XInput
- Sticks can be disabled!
- Shoulder buttons work, but no analog (Analog functionality might be added in future)
- Gracefully handles un/plugging controller
- Shoulder swap (Swap which shoulder buttons map to triggers and bumpers)
- Reset Teensy to program mode

# Wiring
![Wiring diagram](https://raw.githubusercontent.com/joonamo/teensy_classic_xinput/master/Docs/pinout.png)

# Example pictures of completed product
![Example 1](https://raw.githubusercontent.com/joonamo/teensy_classic_xinput/master/Docs/example1.jpg)
![Example 2](https://raw.githubusercontent.com/joonamo/teensy_classic_xinput/master/Docs/example2.jpg)
