

#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_LORA_RADIO_LIB_LORARADIO_MODULESTRUCTBASE_MODULESTRUCTBASE_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_LORA_RADIO_LIB_LORARADIO_MODULESTRUCTBASE_MODULESTRUCTBASE_HPP
using uint8_t = unsigned char;
struct ModulePorts {
  const uint8_t RX{};
  const uint8_t TX{};
  const uint8_t AUX{};
  const uint8_t M0{};
  const uint8_t M1{};
};

struct ModuleParameters {
  const uint8_t HEAD{};
  const uint8_t ADDH{};
  const uint8_t ADDL{};
  const uint8_t SPED{};
  const uint8_t CHAN{};
  const uint8_t OPTIONS{};
};

struct ModuleVersion {
  const uint8_t name{};
  const uint8_t model{};
  const uint8_t version{};
  const uint8_t features{};
};
#endif