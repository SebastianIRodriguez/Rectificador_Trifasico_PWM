#include <TimerOne.h>

// This example uses the timer interrupt to blink an LED
// and also demonstrates how to share a variable between
// the interrupt and the main program.

#define PIN_FA 21 // Pin de la fase A
#define PIN_FB 22 // Pin de la fase B
#define PIN_FC 23 // Pin de la fase C

#define SIN_FREQUENCY 50
#define SIN_OFFSET 1.5

#define INTERRUPT_FREQUENCY 1000

#define PWM_MAX_VOLTAGE 3.0
#define PWM_FREQUENCY 90000
#define PWM_RESOLUTION 9 // 512 Cuentas

const int led = LED_BUILTIN; // the pin with a LED

void setup(void)
{
  delay(1000);
  pinMode(led, OUTPUT);

  // Pines PWM
  pinMode(PIN_FA, OUTPUT);
  pinMode(PIN_FB, OUTPUT);
  pinMode(PIN_FC, OUTPUT);

  analogWriteResolution(PWM_RESOLUTION);

  analogWriteFrequency(PIN_FA, PWM_FREQUENCY);
  analogWriteFrequency(PIN_FB, PWM_FREQUENCY);
  analogWriteFrequency(PIN_FC, PWM_FREQUENCY);

  analogWrite(PIN_FA, 512); // 25%
  analogWrite(PIN_FB, 256); // 50%
  analogWrite(PIN_FC, 348); // 75%

  Timer1.initialize(1000000 / INTERRUPT_FREQUENCY); // Se lanza cada 100 Us
  Timer1.attachInterrupt(crearTrifasica);
  Serial.begin(9600);
}

volatile unsigned long blinkCount = 0; // use volatile for shared variables
volatile unsigned int inicio;
volatile unsigned int final;

void crearTrifasica(void)
{
  static float incremento_fase = 2 * 3.14159265 * SIN_FREQUENCY / INTERRUPT_FREQUENCY;
  static float fase_120 = incremento_fase * 120;
  static float fase_240 = incremento_fase * 240;
  static float fase = 0;
  static float kdc = 512 / PWM_MAX_VOLTAGE;
  inicio = micros();
  int dca, dcb, dcc;

  fase = fase + incremento_fase;
  dca = (SIN_OFFSET + sin(fase)) * kdc; // Argumento en radianes
  dcb = (SIN_OFFSET + sin((fase + fase_120))) * kdc;
  dcc = (SIN_OFFSET + sin((fase + fase_240))) * kdc;

  analogWrite(PIN_FA, dca);
  analogWrite(PIN_FB, dcb);
  analogWrite(PIN_FC, dcc);

  blinkCount++;
  final = micros();
}

// The main program will print the blink count
// to the Arduino Serial Monitor
void loop(void)
{
  unsigned long blinkCopy; // holds a copy of the blinkCount
  unsigned long tardanzaCopy;

  // to read a variable which the interrupt code writes, we
  // must temporarily disable interrupts, to be sure it will
  // not change while we are reading.  To minimize the time
  // with interrupts off, just quickly make a copy, and then
  // use the copy while allowing the interrupt to keep working.
  noInterrupts();
  blinkCopy = blinkCount;
  tardanzaCopy = final - inicio;
  interrupts();

  Serial.print("blinkCount = ");
  Serial.print(blinkCopy);
  Serial.print(" t: ");
  Serial.print(tardanzaCopy);
  Serial.print(" F: ");
  Serial.println(F_CPU);
  delay(1000);
}
