#include "ModuleBase.hpp"
#include <Logger.hpp>
#include <SoftwareSerial.h>
#include <type_traits>

auto ModuleBase::begin() -> ModuleStatus {
  Logger::print(Serial, F("Starting Lora"));

  pinMode(ports.M0, OUTPUT);
  pinMode(ports.M1, OUTPUT);
  pinMode(ports.AUX, INPUT);

  serial = new SoftwareSerial(this->ports.RX, this->ports.TX);
  serial->begin(9600);

  return ModuleStatus::OK;
}

auto ModuleBase::send(const char *) -> ModuleStatus { return ModuleStatus::OK; }
auto ModuleBase::send(const uint8_t, const uint8_t, const uint8_t, const char*)
    -> ModuleStatus {
  return ModuleStatus::OK;
}

auto ModuleBase::isBusy() -> bool { return digitalRead(ports.AUX) == LOW; }
