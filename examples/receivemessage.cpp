#include <Logger.hpp>
#include <LoraRadio.hpp>

E32_100 radio({
    .RX = D2,
    .TX = D3,
    .AUX = D5,
    .M0 = D7,
    .M1 = D6,
});

ICACHE_RAM_ATTR void recv() {
  if (radio.getCurrentMode() == 0 && radio.available()) {
    std::vector<char> buf(radio.available());
    radio.readIntoBuffer(buf);
    for (const auto item : buf) {
      Serial.print(item, HEX);
      Serial.print(' ');
    }
    Serial.println();
  }
}

void setup() {
  Serial.begin(115200);
  radio.begin();

  while (!radio) {
    Logger::print(Serial, '.');
    delay(250);
  }

  radio.onReceive(recv);

  radio.switchToNormal();
}

void loop() {}