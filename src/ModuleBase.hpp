#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_LORA_RADIO_LIB_LORARADIO_MODULEBASE_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_LORA_RADIO_LIB_LORARADIO_MODULEBASE_HPP
#include "./ModuleEnumBase/ModuleEnumBase.hpp"
#include "./ModuleStructBase/ModuleStructBase.hpp"
#include <SoftwareSerial.h>

class ModuleBase {
public:
  constexpr ModuleBase(const ModulePorts &ports, const ModuleParameters &params)
      : parameters(params), ports(ports) {}

  auto operator=(const ModuleBase) -> ModuleBase = delete;
  auto operator=(const ModuleBase &) -> ModuleBase & = delete;
  auto operator=(ModuleBase &&) -> ModuleBase & = delete;
  operator bool() { return !this->isBusy(); }

  ~ModuleBase() {
    if (serial)
      delete serial;
  }

  virtual auto begin() -> ModuleStatus;

  // conceptualize
  virtual auto send(const char *) -> ModuleStatus;
  virtual auto send(const uint8_t, const uint8_t, const uint8_t, const char*)
      -> ModuleStatus;

private:
  auto clearBuffer() -> ModuleStatus;

protected:
  const ModuleParameters parameters{};
  const ModulePorts ports{};
  SoftwareSerial *serial{nullptr};
  [[nodiscard]] auto isBusy() -> bool;
};

#endif
