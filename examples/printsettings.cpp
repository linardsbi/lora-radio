#include <Logger.hpp>
#include <LoraRadio.hpp>

E32_100 radio({
    .RX = D2,
    .TX = D3,
    .AUX = D5,
    .M0 = D7,
    .M1 = D6,
});

void printSettings() {
  auto start = millis();

  const auto [name, model, version, features] = radio.getVersion();
  Logger::print(Serial, "Name: ", name, " model: ", model,
                " version: ", version, " features: ", features, '\n');

  const auto [head, addh, addl, speed, chan, options] = radio.readParameters();
  Logger::print(Serial, "head: ", head, " addh: ", addh, " addl:", addl,
                " speed: ", speed, " channel: ", chan, " options: ", options);

  Logger::print(Serial, "\ntime: ", millis() - start, '\n');
}

void setup() {
  Serial.begin(115200);
  radio.begin();

  while (!radio) {
    Logger::print(Serial, '.');
    delay(250);
  }

  printSettings();
}

void loop() {}