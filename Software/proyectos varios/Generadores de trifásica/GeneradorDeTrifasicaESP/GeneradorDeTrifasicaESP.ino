
#include <WiFi.h>

// ****************************** Senoidal a generar
#define SIN_FREQUENCY 50
#define SIN_OFFSET 1.5
#define SIN_AMPLITUDE 1

// ****************************** Temporización de la interrupción
#define INTERRUPT_FREQUENCY 10000

// ****************************** Características del PWM
#define PWM_FREQUENCY 100000 // Frecuencia en Hz
#define PWM_RESOLUTION 12    // Resolución en bits
#define PWM_MAX_VOLTAGE 3.3  // Tensión de excursión máxima en voltios

#define FASE_A 0
#define FASE_B 1
#define FASE_C 2

#define pinFaseA 25
#define pinFaseB 26
#define pinFaseC 27

#define DC_50 2048

// ****************************** Habilitación PWM
#define pinEnable 32

// ******************************
volatile int count = 0;
volatile bool restart = false;
hw_timer_t *timer = NULL;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile unsigned int inicio, termino, tardanza;
volatile bool interrumpiendo = true;

// Datazo! No se permite el uso de flotantes en las interrupciones!!
void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&timerMux);

  static double incremento_fase = 2 * 3.14159265 * SIN_FREQUENCY / INTERRUPT_FREQUENCY;
  static double fase_120 = 2 * 3.14159265 / 3;
  static double fase_240 = 4 * 3.14159265 / 3;
  static double fase = 0;
  static double kdc = 4095 / PWM_MAX_VOLTAGE; // Transforma de tensión a cuentas

  inicio = micros();
  int dca = 1024, dcb = 2056, dcc = 3500;

  if (interrumpiendo)
  {
    fase = fase + incremento_fase;
    dca = (SIN_OFFSET + SIN_AMPLITUDE * sin(fase)) * kdc; // Argumento en radianes
    dcb = (SIN_OFFSET + SIN_AMPLITUDE * sin((fase + fase_120))) * kdc;
    dcc = (SIN_OFFSET + SIN_AMPLITUDE * sin((fase + fase_240))) * kdc;
  }
  else
  {
    dca = SIN_OFFSET * kdc;
    dcb = SIN_OFFSET * kdc;
    dcc = SIN_OFFSET * kdc;
  }

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

  // Pin de habilitación
  pinMode(pinEnable, INPUT_PULLUP);

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
  Serial.printf("tardanza: %u", termino - inicio);
  Serial.printf("   int: %d\r\n", interrumpiendo);
  restart = true;

  if (digitalRead(pinEnable) == 0)
    interrumpiendo = false;
  else
    interrumpiendo = true;
}

//  ledcWrite(FASE_A, i); // Tarda 7 us en ejecutarse
//  ledcWrite(FASE_B, i); // Los 2 tardan 12 us
//  ledcWrite(FASE_C, i); // Los 3 tardan 18 us
