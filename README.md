# Esp32-Thermostate-System
https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display/discussions/102
https://github.com/Elecrow-RD/esp32-display Both for lvgl and squerline studio test and examples

using this for the touchscreened esp32: https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display

Must use lv_conf.h with lvgl.h v3.8.6 and User_Setup.h with TFT_eSPI.h v2.5.43

hint: sd slot, and touch screen is using different pins, but both use VSPI bus, therefor at the same time just one of them can be useable.
      To use screen and touch pad, at the same time with wifi communication, you shoud use an usb hub which is automaticaly adjust board current. Just to be sure, that the cyd get enought power.
      Because low power could cause the effect of touching the screen. 
