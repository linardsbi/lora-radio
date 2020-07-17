#include "../ModuleBase.hpp"
#include "Defaults.hpp"
#include <Logger.hpp>
#include <cstring>
#include <functional>
#include <string_view>

#ifndef E32_100_H
#define E32_100_H

template <typename T>
concept WritableType = std::is_arithmetic_v<T> && !std::is_pointer_v<T>;

template <typename T> concept WritableArrayType = requires(T t) {
  *t;
  std::is_pointer_v<T> || std::is_array_v<T>;
  t[0];
};

template <typename T> concept WritableContainerType = requires(T t) {
  t.size();
  t.data();
  t[0];
  std::is_arithmetic_v<typename T::value_type>;
  sizeof(typename T::value_type) == 1;
};

class E32_100 : public ModuleBase {
public:
  E32_100(const ModulePorts &ports,
          const ModuleParameters &params = E32::E32_DEFAULTS)
      : ModuleBase(ports, params) {}

  void onReceive(void (*callback)()) {
    if (!serial) {
      Logger::print(Serial, F("Serial interface not yet started\n"));
      return;
    }
    attachInterrupt(digitalPinToInterrupt(ports.AUX), callback, RISING);
  }

  [[nodiscard]] virtual auto getVersion() -> ModuleVersion;

  [[nodiscard]] virtual constexpr auto getParameters()
      -> ModuleParameters const;
  [[nodiscard]] virtual auto readParameters() -> ModuleParameters;

  template <WritableType T>
  auto send(const uint8_t addh, const uint8_t addl, const uint8_t chan,
            T message) -> ModuleStatus {
    Logger::print(Serial, "Sending ", message, " to ", addh, ' ', addl, ' ',
                  chan, '\n');

    changeMode(Mode::NORMAL);

    return write(addh, addl, chan, message);
  }

  template <WritableContainerType T>
  auto send(const uint8_t addh, const uint8_t addl, const uint8_t chan,
            T message) -> ModuleStatus {

    Logger::print(Serial, "Sending ", message.data(), " to ", addh, ' ', addl,
                  ' ', chan, '\n');

    changeMode(Mode::NORMAL);

    return write(std::array<uint8_t, 3>{addh, addl, chan}, message);
  }

  template <WritableArrayType T>
  auto send(const uint8_t addh, const uint8_t addl, const uint8_t chan,
            T message) -> ModuleStatus {

    Logger::print(Serial, "Sending ", String(message), " to ", addh, ' ', addl,
                  ' ', chan, '\n');

    changeMode(Mode::NORMAL);

    // write(addh, addl, chan);
    serial->write(addh);
    serial->write(addl);
    serial->write(chan);
    serial->write(message, strlen(std::forward<T>(message)));

    return ModuleStatus::OK;
  }

  template <WritableContainerType T> auto send(T message) -> ModuleStatus {
    Logger::print(Serial, "Sending ", message.data(), " transparently\n");
    changeMode(Mode::NORMAL);
    return (serial->write(message.data(), message.size()) == 0)
               ? ModuleStatus::EMPTY_BUFFER_WRITTEN
               : ModuleStatus::OK;
  }

  template <WritableType T> auto send(T message) -> ModuleStatus {
    Logger::print(Serial, "Sending ", message, " transparently\n");
    changeMode(Mode::NORMAL);
    return (serial->write(std::forward<T>(message)) == 0)
               ? ModuleStatus::EMPTY_BUFFER_WRITTEN
               : ModuleStatus::OK;
  }

  template <WritableArrayType T> auto send(T message) -> ModuleStatus {
    Logger::print(Serial, "Sending ", " transparently\n");
    changeMode(Mode::NORMAL);

    return (serial->write(std::forward<T>(message),
                          strlen(std::forward<T>(message))) == 0)
               ? ModuleStatus::EMPTY_BUFFER_WRITTEN
               : ModuleStatus::OK;
  }

  virtual auto setParameters(const ModuleParameters & = E32::E32_DEFAULTS)
      -> ModuleStatus;

  template <WritableContainerType buffer>
  auto readIntoBuffer(buffer &buf) -> ModuleStatus {
    const auto totalBytes = serial->available();

    if (totalBytes == 0)
      return ModuleStatus::EMPTY_RETURN_BUFFER;

    Logger::print(Serial, F("Bytes in serial buf: "), totalBytes, '\n');

    std::generate(buf.begin(), buf.end(), [this]() { return serial->read(); });

    return ModuleStatus::OK;
  }

  [[nodiscard]] auto available() { return serial->available(); }

  void switchToNormal() { changeMode(Mode::NORMAL); }

  [[nodiscard]] constexpr auto getCurrentMode() -> bool const {
    return static_cast<uint8_t>(currentMode);
  }

private:
  enum class Mode : uint8_t {
    NORMAL,
    WAKEUP,
    POWERSAVE,
    SLEEP,
    NONE,
  };

  Mode currentMode{Mode::NONE};

  auto changeMode(Mode) -> ModuleStatus;
  auto resetModule() -> ModuleStatus;

  template <WritableType... Args>
  auto write(const Args... args) -> ModuleStatus {
    if (!serial->availableForWrite())
      return ModuleStatus::NOT_WRITABLE;

    (serial->write(args), ...);

    waitUntilFinished();

    return ModuleStatus::OK;
  }

  template <WritableContainerType... Args>
  auto write(const Args... args) -> ModuleStatus {
    if (!serial->availableForWrite())
      return ModuleStatus::NOT_WRITABLE;

    (serial->write(args.data(), args.size()), ...);

    waitUntilFinished();

    return ModuleStatus::OK;
  }

  auto waitUntilFinished() -> ModuleStatus;
};

#endif