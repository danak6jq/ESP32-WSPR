# ESP32-WSPR
Complete stand-alone WSPR2 transmitter using ESP32 + Si5351 modules. Uses SNTP for transmit time-sync

Prototype WSPR2 beacon transmitter for VLF to VHF operation.
Uses ESP32 module (DevKitC in prototype) connected to Si5351A module (QRP Labs synth in prototype, though any Si5351 module should work). This Si5351A modules uses a 27.0MHz crystal oscillator, which is hard coded in line 345 of main/user_main.c:

    si5351_init(SI5351_CRYSTAL_LOAD_10PF, 27000000, 175310);

If you use a module with a 25MHz crystal, this needs to be changed. Of course, this ought to be a confiugurable parameter, send me a pull request if you implement that.

This is a prototype built in a couple of days, lacks a lot of features, so add them and share them.

Built using latest 'master' ESP-IDF (ESP-IDF v3.3-beta1)

Connection:
  - ESP32 GPIO18 to SDA
  - ESP32 GPIO19 to SCL

Required:
- Add an appropriate low-pass filter to the output of the Si5351 module.
- You may wish to add a power amplifier as well.

Uses high-precision timer events in ESP32/ESP-IDF to synchronize baud transmission. I would have used the less-expensive ESP8266, but it lacks the high-precision timer service required for accurate bit-timing.

Customize the WiFi SSID/password for your location in user_main.c:
```
#define EXAMPLE_WIFI_SSID "BidenHarris2020"
#define EXAMPLE_WIFI_PASS "ImpeachTrump"
```
- [ ] make that configurable via menuconfig
- [ ] add a startup WebUI to autoconfigure WiFi

Transmission frequency is determined by this line in user_main.c:
```
static uint64_t txFreq = (14095600 + 1500 - 50) * 100;
```
Transmit power is roughly controlled by the output current setting on the Si5351 in this version; if you add an external power amplifier, you'll have to revise that. I measured +10.4dBm into a 50 ohm load using output current of 8mA.

Set your WSPR info in this line of user_main.c:
```
    (void) get_wspr_channel_symbols("<K6JQ> CM88WE 10", syms);
```

- [ ] add duty cycle control, add frequency hopping
- [ ] use GPIO to select low-pass filters when selecting frequency
- [ ] add a WebUI to control/configure the transmitter

Transmitter automatically does SNTP time sync, sufficient for WSPR transmission.
Kudos to @NT7S for the comprehensive Si5351 code; I started with his Arduino library (that I previously contributed to) and pared it back down to C for inclusion in the ESP-IDF project.

A simple power amplifier can be built with CMOS (74HC, 74AHC, 74AC) inverters/buffers ganged in parallel and fed to a low-pass filter. Note that the Si5351A output is 3v3 logic, so you'll need to use HCT/AHCT/ACT logic to do level-conversion (or whatever other scheme you desire, but this is simplest). For the output low-pass filter, design a T network rather than a Pi network - the T network is considerably more efficient. For ideas and specific circuits, search for "74HC240 74AC240 power amplifier" or somesuch.
