# ESP32-WSPR
Complete stand-alone WSPR2 transmitter using ESP32 + Si5351 modules. Uses SNTP for transmit time-sync

Prototype WSPR2 beacon transmitter for VLF to VHF operation
Uses ESP32 module (DevKitC in prototype) connected to Si5351A module (QRP Labs synth in prototype, though any Si5351 module should work).
This is a prototype built in a couple of days, lacks a lot of features, so add them and share them.

Connection:
  ESP32 GPIO18 to SDA
  ESP32 GPIO19 to SCL

Required:
  Add an appropriate low-pass filter to the output of the Si5351 module.
  You may wish to add a power amplifier as well.

Uses high-precision timer events in ESP32/ESP-IDF to synchronize baud transmission.
Customize the WiFi SSID/password for your location in user_main.c:
  '/*
   * Set the WiFi info
   */
  #define EXAMPLE_WIFI_SSID "Pelosi2019"
  #define EXAMPLE_WIFI_PASS "ImpeachTheBums"'

TODO: make that configurable via menuconfig
TODO: add a startup WebUI to autoconfigure WiFi

Transmission frequency is determined by this line in user_main.c:
  'static uint64_t txFreq = (14095600 + 1500 - 50) * 100;'

TODO: add duty cycle control, add frequency hopping
TODO: use GPIO to select low-pass filters when selecting frequency
TODO: add a WebUI to control/configure the transmitter

Transmitter automatically does SNTP time sync, sufficient for WSPR transmission.
