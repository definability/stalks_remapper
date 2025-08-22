#ifndef VIGEM_CLIENT_WRAPPER_HPP
#define VIGEM_CLIENT_WRAPPER_HPP
#include <bitset>

#include <exception>

typedef struct _VIGEM_CLIENT_T *PVIGEM_CLIENT;
typedef struct _VIGEM_TARGET_T *PVIGEM_TARGET;

class GamepadEmulatorException final : public std::exception
{
  public:
    explicit GamepadEmulatorException(const char *message);
};

class GamepadEmulator
{
  public:
    GamepadEmulator();

    ~GamepadEmulator();

    void update(uint16_t buttons) const;

  private:
    PVIGEM_CLIENT client;
    PVIGEM_TARGET target;
};

#endif // VIGEM_CLIENT_WRAPPER_HPP
