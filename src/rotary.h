
#ifndef RP2040_FREERTOS_IRQ_ROTARY_H
#define RP2040_FREERTOS_IRQ_ROTARY_H
class Rotary {
public:
  Rotary(int pin_a, int pin_b, int pin_sw);
  bool rotatingClockwise();
  int getPinA();
  int getPinled();

private:
  int pin_a;
  int pin_b;
  int pin_sw;
};
#endif // RP2040_FREERTOS_IRQ_ROTARY_H
