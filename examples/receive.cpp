#include <stdlib_compatibility.hpp>
#include <E32_100.hpp>

#define RADIO_PINS .RX = 3, \
                   .TX = 4, \
                   .AUX = 2, \
                   .M0 = 6, \
                   .M1 = 7
                   
int main() {
  if constexpr (DEBUG) {
    Serial.begin(115200);
    Logger::log(F("Serial started\n"));
  }

  Radio::E32_100<{RADIO_PINS}> radio;

  radio.begin();

  Logger::log(F("Lora module started\n"));

  radio.on_receive([&radio]() {
    if (radio.current_mode() == Radio::Mode::NORMAL && radio.bytes_available() > 0) {
      Logger::log(F("Received: \n"));
      for (const auto ch : radio.raw_bytes()) {
        Serial.print(ch, HEX);
        Serial.print(' ');
      }
      Serial.println();
    }
  });

  return 1;
}