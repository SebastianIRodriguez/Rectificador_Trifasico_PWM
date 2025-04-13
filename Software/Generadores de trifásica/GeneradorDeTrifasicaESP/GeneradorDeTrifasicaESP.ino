/*
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

*/

/// Sketch de Andreass
#include <WiFi.h>

// ****************************** Senoidal a generar
#define SIN_FREQUENCY 50
#define SIN_OFFSET 1.5

// ****************************** Temporización de la interrupción
#define INTERRUPT_FREQUENCY 10000

// ****************************** Características del PWM
#define PWM_FREQUENCY 100000 // Frecuencia en Hz
#define PWM_RESOLUTION 12   // Resolución en bits
#define PWM_MAX_VOLTAGE 3.3 // Tensión de excursión máxima en voltios

#define FASE_A 0
#define FASE_B 1
#define FASE_C 2

#define pinFaseA 25
#define pinFaseB 26
#define pinFaseC 27

// ******************************
volatile int count = 0;
volatile bool restart = false;
hw_timer_t *timer = NULL;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile unsigned int inicio, termino, tardanza;

// Datazo! No se permite el uso de flotantes en las interrupciones!!
void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&timerMux);

  static double incremento_fase = 2 * 3.14159265 * SIN_FREQUENCY / INTERRUPT_FREQUENCY;
  static double fase_120 = 2 * 3.14159265 / 3;
  static double fase_240 = 4 * 3.14159265 / 3;
  static double fase = 0;
  static double kdc = 4095 / PWM_MAX_VOLTAGE;

  inicio = micros();
  int dca = 1024, dcb = 2056, dcc = 3500;

  fase = fase + incremento_fase;
  dca = (SIN_OFFSET + sin(fase)) * kdc; // Argumento en radianes
  dcb = (SIN_OFFSET + sin((fase + fase_120))) * kdc;
  dcc = (SIN_OFFSET + sin((fase + fase_240))) * kdc;

  ledcWrite(FASE_A, dca);
  ledcWrite(FASE_B, dcb);
  ledcWrite(FASE_C, dcc);

  termino = micros();

  count++;
  if (restart)
  {
    count = 0;
    restart = false;
  }

  tardanza = termino - inicio;

  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup()
{
  WiFi.mode(WIFI_OFF);
  btStop();

  Serial.begin(115200);
  Serial.print("setup() running on core ");
  Serial.println(xPortGetCoreID());

  delay(5000);

  // Fase A
  ledcAttachPin(pinFaseA, FASE_A);
  ledcSetup(FASE_A, PWM_FREQUENCY, PWM_RESOLUTION);

  // Fase B
  ledcAttachPin(pinFaseB, FASE_B);
  ledcSetup(FASE_B, PWM_FREQUENCY, PWM_RESOLUTION);

  // Fase C
  ledcAttachPin(pinFaseC, FASE_C);
  ledcSetup(FASE_C, PWM_FREQUENCY, PWM_RESOLUTION);

  // Configuramos el timer
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000 / INTERRUPT_FREQUENCY, true);
  timerAlarmEnable(timer);
}

void loop()
{
  delay(1000);
  Serial.printf("F: %u - ", count);
  Serial.printf("tardanza: %u\r\n", termino - inicio);
  restart = true;
}

//  ledcWrite(FASE_A, i); // Tarda 7 us en ejecutarse
//  ledcWrite(FASE_B, i); // Los 2 tardan 12 us
//  ledcWrite(FASE_C, i); // Los 3 tardan 18 us
