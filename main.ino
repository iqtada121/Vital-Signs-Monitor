#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include "MAX30105.h"
#include "heartRate.h"
#include <STM32FreeRTOS.h>
#include "spo2_algorithm.h"
#include <SPI.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
// ------------------- Shared Values -------------------
volatile float   g_temperature = 0.0;
volatile int     g_heartRate  = 0;
volatile int     g_spo2       = 0;
const int pin_SS   = PA4;   // CS for ADS1293

// ------------------- DWIN UART ----------------------
HardwareSerial dwinSerial(PA3, PA2);   // RX, TX

// Send any value to DWIN (VP, int16)
void sendToDWIN(uint16_t vp, int16_t value) {
  uint8_t frame[8];
  frame[0] = 0x5A;
  frame[1] = 0xA5;
  frame[2] = 0x05;
  frame[3] = 0x82;
  frame[4] = vp >> 8;
  frame[5] = vp & 0xFF;
  frame[6] = value >> 8;
  frame[7] = value & 0xFF;

  dwinSerial.write(frame, sizeof(frame));
}

// -------- Temperature Task (reads every 1 sec) -------
void TaskTemperature(void *pvParameters) {
  for (;;) {
    tempTask();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void vTask_SPO2(void *pvParameters)
{
  Serial.println("Task Started.");
  for(;;) 
  {
    spo2HR();
    vTaskDelay(pdMS_TO_TICKS(500)); 
  }
}

void vTask_ECG(void *pvParameters)
{
  for(;;) 
  {
    ReadAndTrans();
    vTaskDelay(pdMS_TO_TICKS(40)); 
  }
}


void setup() {
  Serial.begin(115200);
  dwinSerial.begin(115200);

  pinMode(pin_SS, OUTPUT);
  SPI.begin();

  setup_ECG();
  max30102Init();
  tempInit();

  Serial.println("RTOS Vital Monitor Template Started");

  xTaskCreate(vTask_ECG, "ECGTask", 1024*2, NULL, 2, NULL);
  xTaskCreate(TaskTemperature, "TempTask", 256, NULL, 1, NULL);
  xTaskCreate(vTask_SPO2, "SpO2Task", 4096, NULL, 1, NULL);
  
  vTaskStartScheduler();
}

// int t = 0;


void loop() {
  // int16_t val1 = (int16_t)(sin(t * 0.1) * 500 + 500);
  // int16_t val2 = (int16_t)(sin(t * 0.1 + 1.5) * 500 + 500);  // phase-shifted

  // DWIN_SendDualCurve(val1, val2);
  // Serial.print(val1); Serial.print(" "); Serial.println(val2);

  // t++;
  // delay(40);
}
