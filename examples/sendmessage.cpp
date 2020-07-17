#include <Logger.hpp>
#include <LoraRadio.hpp>

E32_100 radio({
    .RX = D2,
    .TX = D3,
    .AUX = D5,
    .M0 = D7,
    .M1 = D6,
});

void setup() {
  Serial.begin(115200);
  radio.begin();

  while (!radio) {
    Logger::print(Serial, '.');
    delay(250);
  }
}

void loop() {
  // call send(message) to broadcast transparently,
  // call send(recipient_addh, recipient_addl, recipient_channel, message) to do
  // fixed-point

  if (radio.send(0x0, 0x3, 0x4, "Ping") == ModuleStatus::EMPTY_BUFFER_WRITTEN) {
    Logger::print(Serial, F("Couldn't write to module"));
  }

  radio.send(0x0, 0x3, 0x4, "testing");

  delay(2000);
}