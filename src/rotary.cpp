#include <hardware/gpio.h>
#include <rotary.h>

Rotary::Rotary(int pin_a, int pin_b, int pin_sw)
    : pin_a(pin_a), pin_b(pin_b), pin_sw(pin_sw) {
  // ROT A 11
  gpio_init(pin_a);
  gpio_set_dir(pin_a, false);
  gpio_disable_pulls(pin_a);
  // ROT B 10
  gpio_init(pin_b);
  gpio_set_dir(pin_b, false);
  gpio_disable_pulls(pin_b);
  // ROT sw 12
  gpio_init(pin_sw);
  gpio_set_dir(pin_sw, false);
  gpio_set_pulls(pin_sw, true, false);
};
bool Rotary::rotatingClockwise() { return !gpio_get(pin_b); };
int Rotary::getPinA() { return pin_a; };
int Rotary::getPinled() { return pin_a; };
