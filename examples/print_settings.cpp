#include <E32_100.hpp>
#include <stdlib_compatibility.hpp>

#define RADIO_PINS .RX = 3, \
                   .TX = 4, \
                   .AUX = 2, \
                   .M0 = 6, \
                   .M1 = 7

void print_settings(auto& radio) {
  const auto start = millis();

  const auto [name, model, version, features] = radio.read_version();
  Logger::log("Name: ", name, " model: ", model,
                " version: ", version, " features: ", features, '\n');

  const auto [head, addh, addl, speed, chan, options] = radio.read_parameters();
  Logger::log("head: ", head, " addh: ", addh, " addl:", addl,
                " speed: ", speed, " channel: ", chan, " options: ", options);

  Logger::log("\ntime: ", millis() - start, '\n');
}

int main() {
  if constexpr (DEBUG) {
    Serial.begin(115200);
    Logger::log(F("Serial started\n"));
  }

  Radio::E32_100<{RADIO_PINS}> radio;

  radio.begin();

  Logger::log(F("Lora module started\n"));
  
  print_settings(radio);

  return 1;
}