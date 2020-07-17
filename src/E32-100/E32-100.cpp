#include "./E32-100.hpp"
#include <Logger.hpp>
#include <vector>
#include <iterator>

auto E32_100::getVersion() -> ModuleVersion {
  changeMode(Mode::SLEEP);
  assert(write(0xC3, 0xC3, 0xC3) != ModuleStatus::NOT_WRITABLE &&
         F("The RX and TX pins given are probably wrong"));

  std::array<uint8_t, 4> buf;
  assert(readIntoBuffer(buf) != ModuleStatus::EMPTY_RETURN_BUFFER);

  return {buf[0], buf[1], buf[2], buf[3]};
}

constexpr auto E32_100::getParameters() -> ModuleParameters const {
  return parameters;
}

auto E32_100::readParameters() -> ModuleParameters {
  changeMode(Mode::SLEEP);
  assert(write(0xC1, 0xC1, 0xC1) != ModuleStatus::NOT_WRITABLE);

  std::array<uint8_t, 6> buf;
  assert(readIntoBuffer(buf) != ModuleStatus::EMPTY_RETURN_BUFFER);

  return {buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]};
}

auto E32_100::changeMode(Mode m) -> ModuleStatus {

  if (m == currentMode)
    return ModuleStatus::OK;

  switch (m) {
  case Mode::NORMAL:
    Logger::print(Serial, F("Changing mode to Normal\n"));
    digitalWrite(ports.M0, LOW);
    digitalWrite(ports.M1, LOW);
    break;
  case Mode::POWERSAVE:
    Logger::print(Serial, F("Changing mode to Powersave\n"));
    digitalWrite(ports.M0, LOW);
    digitalWrite(ports.M1, HIGH);
    break;
  case Mode::SLEEP:
    Logger::print(Serial, F("Changing mode to Sleep\n"));
    digitalWrite(ports.M0, HIGH);
    digitalWrite(ports.M1, HIGH);
    break;
  case Mode::WAKEUP:
    Logger::print(Serial, F("Changing mode to Wakeup\n"));
    digitalWrite(ports.M0, HIGH);
    digitalWrite(ports.M1, LOW);
    break;
  default:
    return ModuleStatus::NOT_IMPLEMENTED;
    break;
  }

  waitUntilFinished();
  delay(E32::DEFAULT_DELAY); // changing modes sometimes requires extra time

  currentMode = m;

  return ModuleStatus::OK;
}

auto E32_100::setParameters(const ModuleParameters &) -> ModuleStatus {
  changeMode(Mode::SLEEP);
  // delay(2000);
  Logger::print(Serial, F("Changing mode to Normal"));
  assert(write(parameters.ADDH, parameters.ADDL, parameters.CHAN,
               parameters.HEAD, parameters.OPTIONS,
               parameters.SPED) != ModuleStatus::NOT_WRITABLE);
  // delay(2000);
  resetModule();
  return ModuleStatus::OK;
}

auto E32_100::resetModule() -> ModuleStatus {
  changeMode(Mode::SLEEP);
  Logger::print(Serial, F("Resetting\n"));
  assert(write(0xC4, 0xC4, 0xC4) != ModuleStatus::NOT_WRITABLE);
  delay(1000);

  return ModuleStatus::OK;
}

auto E32_100::waitUntilFinished() -> ModuleStatus {
  for (auto i = 0; isBusy() && i <= E32::DEFAULT_TIMEOUT; ++i) {
    // Logger::print(Serial, '.');
    delay(E32::DEFAULT_DELAY);
  }
  return (isBusy()) ? ModuleStatus::JOB_TIMEOUT : ModuleStatus::OK;
}

