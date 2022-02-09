#pragma once

#include <Arduino.h>
#include <bit>
#include <cassert>
#include <SoftwareSerial.h>
#include <string_view>
#include <type_traits>

#include <Logger.hpp>
#include "defaults.hpp"
#include <Interrupt.hpp>
#include "../Common.hpp"

namespace Radio {
    struct Address {
        uint8_t high;
        uint8_t low;
        uint8_t channel;
    };

    struct Version {
        uint8_t name{};
        uint8_t model{};
        uint8_t version{};
        uint8_t features{};
    };

    struct Pins {
        uint8_t RX;
        uint8_t TX;
        uint8_t AUX;
        uint8_t M0;
        uint8_t M1;
    };

    struct Parameters {
        uint8_t HEAD{};
        uint8_t ADDH{};
        uint8_t ADDL{};
        uint8_t SPED{};
        uint8_t CHAN{};
        uint8_t OPTIONS{};
    };

    enum class Mode : uint8_t {
            NORMAL,
            WAKEUP,
            POWERSAVE,
            SLEEP,
            NONE,
        };

    template <Pins Pins_>
    class E32_100 {
    public:
        consteval E32_100(const Parameters& params = E32_DEFAULTS)
        : m_params(params)
         {}

        SoftwareSerial& serial() const {
            static SoftwareSerial serial_connection(Pins_.RX, Pins_.TX);
            return serial_connection;
        }

        void begin() const {
            pinMode(Pins_.M0, OUTPUT);
            pinMode(Pins_.M1, OUTPUT);
            pinMode(Pins_.AUX, INPUT);
            serial().begin(9600);
        }

        bool broadcast(BufferType auto& message) {
            Logger::log(F("Sending "), message.data(), F(" transparently\n"));
            change_to(Mode::NORMAL);
            return write(std::bit_cast<const uint8_t*>(message.data()), message.size());
        }

        bool send(const Address& address, BufferType auto&& message) {
            Logger::log(F("Sending "), message.data(), 
                       F(" to "), address.high, ' ', address.low, ' ', address.channel, '\n');

            change_to(Mode::NORMAL);

            serial().write(address.high);
            serial().write(address.low);
            serial().write(address.channel);

            return serial().write(message.data(), message.size());
        }

        void on_receive(auto callback) const {
            if (!serial()) {
                Logger::log(F("Serial interface not yet started\n"));
                return;
            }
            Interrupt::add<digitalPinToInterrupt(Pins_.AUX)>(RISING, callback);
        }

        [[nodiscard]] constexpr auto &get_parameters() const {
            return m_params;
        }

        [[nodiscard]] Version read_version() {
            change_to(Mode::SLEEP);
            assert(write(0xC3, 0xC3, 0xC3) != false &&
                    F("Are the RX and TX pins connected properly?"));

            std::array<uint8_t, 4> buf;
            assert(read_into(buf) != false);

            return {buf[0], buf[1], buf[2], buf[3]};
        }

        [[nodiscard]] Parameters read_parameters() {
            change_to(Mode::SLEEP);
            assert(write(0xC1, 0xC1, 0xC1) != false &&
                    F("Are the RX and TX pins connected properly?"));

            std::array<uint8_t, 6> buf;
            assert(read_into(buf) != false);

            return {buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]};
        }

        Mode change_to(Mode mode) {
            if (mode == m_current_mode)
                return mode;

            switch (mode) {
            case Mode::NORMAL:
                Logger::log(F("Changing mode to Normal\n"));
                digitalWrite(Pins_.M0, LOW);
                digitalWrite(Pins_.M1, LOW);
                break;
            case Mode::POWERSAVE:
                Logger::log(F("Changing mode to Powersave\n"));
                digitalWrite(Pins_.M0, LOW);
                digitalWrite(Pins_.M1, HIGH);
                break;
            case Mode::SLEEP:
                Logger::log(F("Changing mode to Sleep\n"));
                digitalWrite(Pins_.M0, HIGH);
                digitalWrite(Pins_.M1, HIGH);
                break;
            case Mode::WAKEUP:
                Logger::log(F("Changing mode to Wakeup\n"));
                digitalWrite(Pins_.M0, HIGH);
                digitalWrite(Pins_.M1, LOW);
                break;
            default: [[unlikely]]
                assert(false && "Unreachable");
                break;
            }

            wait_until_not_busy();
            delay(DEFAULT_DELAY); // changing modes sometimes requires extra time

            const auto old_mode = std::exchange(m_current_mode, mode);

            return old_mode;
        }

        bool reset() const {
            change_to(Mode::SLEEP);

            Logger::log(F("Resetting\n"));
            assert(write(0xC4, 0xC4, 0xC4) != false);
            delay(1000);

            return true;
        }

        void change_to_normal() {
            change_to(Mode::NORMAL);
        }

        bool is_busy() const {
            return digitalRead(Pins_.AUX) == LOW;
        }

        Mode current_mode() const {
            return m_current_mode;
        }

        auto bytes_available() const {
            return serial().available();
        }

        auto raw_bytes() const {
            constexpr uint8_t buffer_size = 64;
            // too bad we can't access the receive buffer of SoftwareSerial directly
            std::array<uint8_t, buffer_size> raw_buffer{};
            read_into(raw_buffer);

            return raw_buffer;
        }

    private:
    
        Parameters m_params;
        Mode m_current_mode{Mode::NONE};

        
        bool write(const uint8_t* buffer, size_t size) const {
            if (!serial().availableForWrite()) {
                return false;
            }
            
            serial().write(buffer, size);

            wait_until_not_busy();

            return true;
        }

        template <typename... Args>
        bool write(Args... args) const {
            static_assert((std::is_integral_v<Args> && ...));

            if (!serial().availableForWrite()) {
                return false;
            }
            
            (serial().write(static_cast<uint8_t>(std::forward<Args>(args))), ...);

            wait_until_not_busy();

            return true;
        }

        bool read_into(BufferType auto &buf) const {
            const auto totalBytes = serial().available();

            if (totalBytes == 0) {
                return false;
            }

            Logger::log(F("Bytes in serial buf: "), totalBytes, '\n');

            serial().readBytes(buf.data(), buf.size());

            return true;
        }

        bool wait_until_not_busy() const {
            for (int i = 0; is_busy() && i <= DEFAULT_TIMEOUT; ++i) {
                // Logger::print(Serial, '.');
                delay(DEFAULT_DELAY);
            }
            return is_busy();
        }

        static constexpr Parameters E32_DEFAULTS{
            .HEAD = HEAD_PWD_SAVE,
            .ADDH = 0x0,
            .ADDL = 0x0,
            .SPED = PARITY_8N1 ^ UART_RATE_9600 ^ DATA_RATE_2P4,
            .CHAN = CHANNEL_868,
            .OPTIONS = CLEAR_TRANSMISSION ^ IO_DRIVE_PP ^ WAKEUP_TIME_250 ^ FEC_ON ^
                    POWER_17DB,
        };
    };
}
