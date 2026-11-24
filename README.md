# MQTTControlNode

An Arduino/PlatformIO controller for the ESP32-C3 Super Mini. Four GPIO inputs
use internal pull-ups and publish configurable MQTT topics and payloads for
their HIGH and LOW states. The initial state of each input is published after
the broker connects; subsequent stable changes are debounced for 50 ms.

The controller connects to the MQTT broker at `192.168.0.2:1883` without
authentication or TLS. It retries Wi-Fi and MQTT connections and republishes
all current input states after an MQTT reconnect. Changes while disconnected
are coalesced into the latest state, not queued. Messages are sent at QoS 0
without retain; delivery is not guaranteed.

## Hardware and configuration

Connect each switch or sensor output between its input pin and GND. An open
switch reads HIGH and a closed switch reads LOW. Use only 3.3 V-compatible
signals; do not connect 5 V to an input.

The default input GPIOs are 0, 1, 3 and 4. Edit `include/config.h` to change
the pins, debounce interval, and the two topic/payload pairs for each input.
By default, the onboard LED is assumed to be active-low on GPIO 8; it is lit
when Wi-Fi is connected, regardless of broker connection status. Check the
pinout and LED polarity for your board before wiring or changing these values.

Create a local `data` directory containing these UTF-8 plain-text files:

| File | Contents |
| --- | --- |
| `host.txt` | Wi-Fi hostname, also used as the MQTT client ID. Use a valid hostname unique to each controller. |
| `ssid.txt` | Wi-Fi network name (2.4 GHz). |
| `pass.txt` | Wi-Fi password; leave the file empty for an open network. |

Use one value per file; trailing line endings are removed. These files are
ignored by Git. They are stored unencrypted on the device, so protect physical
access and use a trusted network. Missing files, an empty hostname or SSID, or
a failure to mount LittleFS prevents connection; check the serial monitor.

## Clone, build and upload

Install [Visual Studio Code](https://code.visualstudio.com/) with the
[PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode),
or install PlatformIO Core. Then:

1. Clone the repository and open it as a PlatformIO project:
   ```sh
   git clone https://github.com/swatso/MQTTControlNode.git
   cd MQTTControlNode
   ```
2. Create and populate the three files in `data` as described above.
3. Connect the ESP32-C3 by USB. Build and upload the firmware, then upload the
   LittleFS image (this replaces the existing filesystem contents):
   ```sh
   pio run
   pio run -t upload
   pio run -t uploadfs
   ```
   In VS Code, the equivalent actions are Build, Upload, and Upload Filesystem
   Image in the PlatformIO project tasks. Reset the board after uploading the
   filesystem so it reads the new settings.
4. Open the serial monitor at 115200 baud (`pio device monitor`) and subscribe
   to `mqttcontrolnode/#` on the broker to observe the default messages.

The build enables the ESP32-C3's native USB Serial/JTAG interface for
`Serial`. Select the board's USB serial port in the monitor; its port number
may change after uploading or reconnecting. Startup messages can be missed if
the monitor opens after the board starts, so reset the board with the monitor
open to see them.
