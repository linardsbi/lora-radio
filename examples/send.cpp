#include <stdlib_compatibility.hpp>
#include <E32_100.hpp>

#define RADIO_PINS .RX = 3, \
                   .TX = 4, \
                   .AUX = 2, \
                   .M0 = 6, \
                   .M1 = 7
                   
int main() {
  using namespace std::literals;

  if constexpr (DEBUG) {
    Serial.begin(115200);
    Logger::log(F("Serial started\n"));
  }

  Radio::E32_100<{RADIO_PINS}> radio;

  radio.begin();

  Logger::log(F("Lora module started\n"));

  Radio::Address recipient{0x0, 0x3, 0x4};
  while (true) {
    if (!radio.send(recipient, "Ping"sv)) {
      Logger::log(F("Couldn't write to module"));
    }

    radio.send(recipient, "testing"sv);

    delay(2000);
  }

  return 1;
}