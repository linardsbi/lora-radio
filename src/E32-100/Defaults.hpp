#ifndef _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_LORA_RADIO_LIB_LORARADIO_E32_100_DEFAULTS_HPP
#define _HOME_HVS_DOCUMENTS_PLATFORMIO_PROJECTS_LORA_RADIO_LIB_LORARADIO_E32_100_DEFAULTS_HPP

#include "../ModuleStructBase/ModuleStructBase.hpp"

using uint8_t = unsigned char;
namespace E32 {
constexpr uint8_t HEAD_PWD_SAVE = 0xC0;
constexpr uint8_t HEAD_PWD_DEL = 0xC2;

constexpr uint8_t PARITY_8N1 = 0b11000000;
constexpr uint8_t PARITY_8O1 = 0b01000000;
constexpr uint8_t PARITY_8E1 = 0b10000000;

constexpr uint8_t UART_RATE_9600 = 0b00011000;
constexpr uint8_t UART_RATE_115200 = 0b00111000;

constexpr uint8_t DATA_RATE_0P3 = 0b00000000;
constexpr uint8_t DATA_RATE_1P2 = 0b00000001;
constexpr uint8_t DATA_RATE_2P4 = 0b00000010;
constexpr uint8_t DATA_RATE_4P8 = 0b00000011;
constexpr uint8_t DATA_RATE_9P6 = 0b00000100;
constexpr uint8_t DATA_RATE_19P2 = 0b00000101;

constexpr uint8_t CHANNEL_410 = 0x00;
constexpr uint8_t CHANNEL_433 = 0x17;
constexpr uint8_t CHANNEL_441 = 0x1F;
constexpr uint8_t CHANNEL_855 = 0x00;
constexpr uint8_t CHANNEL_868 = 0x82;
constexpr uint8_t CHANNEL_880 = 0xFF;

constexpr uint8_t FIXED_TRANSMISSION = 0b10000000;
constexpr uint8_t CLEAR_TRANSMISSION = 0b00000000;

constexpr uint8_t IO_DRIVE_PP = 0b01000000;
constexpr uint8_t IO_DRIVE_OPEN = 0b00000000;

constexpr uint8_t WAKEUP_TIME_250 = 0b00000000;
constexpr uint8_t WAKEUP_TIME_500 = 0b00001000;
constexpr uint8_t WAKEUP_TIME_750 = 0b00010000;
constexpr uint8_t WAKEUP_TIME_1K = 0b00011000;
constexpr uint8_t WAKEUP_TIME_1P25K = 0b00100000;
constexpr uint8_t WAKEUP_TIME_1P5K = 0b00101000;
constexpr uint8_t WAKEUP_TIME_1P75K = 0b00110000;
constexpr uint8_t WAKEUP_TIME_2K = 0b00111000;

constexpr uint8_t FEC_ON = 0b00000100;
constexpr uint8_t FEC_OFF = 0b00000000;

constexpr uint8_t POWER_20DB = 0b00000000;
constexpr uint8_t POWER_17DB = 0b00000001;
constexpr uint8_t POWER_14DB = 0b00000010;
constexpr uint8_t POWER_10DB = 0b00000011;

constexpr ModuleParameters E32_DEFAULTS{
    .HEAD = HEAD_PWD_SAVE,
    .ADDH = 0x0,
    .ADDL = 0x0,
    .SPED = PARITY_8N1 ^ UART_RATE_9600 ^ DATA_RATE_2P4,
    .CHAN = CHANNEL_868,
    .OPTIONS = CLEAR_TRANSMISSION ^ IO_DRIVE_PP ^ WAKEUP_TIME_250 ^ FEC_ON ^
               POWER_17DB,
};

constexpr int DEFAULT_DELAY = 50; // milliseconds
constexpr int DEFAULT_TIMEOUT = 2000 / DEFAULT_DELAY; // = # of iterations

} // namespace E32
#endif
