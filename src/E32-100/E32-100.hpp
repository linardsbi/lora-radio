#include "../ModuleBase.hpp"
#include "Defaults.hpp"
#include <functional>
#include <string_view>

#ifndef E32_100_H
#define E32_100_H

template <typename T> concept WritableType = requires(T t) {
  std::is_arithmetic_v<T>;
  sizeof(T) == 1;
  t++;
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
  constexpr E32_100(const ModulePorts &ports,
                    const ModuleParameters &params = E32::E32_DEFAULTS)
      : ModuleBase(ports, params) {}

  //   using FuncP = std::function<void(void)>;

  //   inline void setOnReceive(const FuncP &callback) {
  //     onReceiveMessage = callback;
  //   }
  //   inline void setOnTransmit(const FuncP &callback) { onSendMessage =
  //   callback; }

  [[nodiscard]] virtual auto getVersion() -> ModuleVersion;

  [[nodiscard]] virtual constexpr auto getParameters()
      -> ModuleParameters const;
  [[nodiscard]] virtual auto readParameters() -> ModuleParameters;

  auto send(std::string_view message) -> ModuleStatus;

  auto send(const uint8_t addh, const uint8_t addl, const uint8_t chan,
            std::string_view message) -> ModuleStatus;

  virtual auto setParameters(const ModuleParameters & = E32::E32_DEFAULTS)
      -> ModuleStatus;

  template <WritableContainerType buffer>
  auto readIntoBuffer(buffer &buf) -> ModuleStatus;

private:
  //   FuncP onReceiveMessage{};
  //   FuncP onSendMessage{};

  enum class Mode : uint8_t {
    NORMAL,
    WAKEUP,
    POWERSAVE,
    SLEEP,
    NONE,
  };

  Mode currentMode = Mode::NONE;

  auto changeMode(Mode) -> ModuleStatus;
  auto resetModule() -> ModuleStatus;

  template <WritableType... Args>
  auto write(const Args... args) -> ModuleStatus;

  template <WritableContainerType... Args>
  auto write(const Args... args) -> ModuleStatus;

  auto waitUntilFinished() -> ModuleStatus;
};

#endif