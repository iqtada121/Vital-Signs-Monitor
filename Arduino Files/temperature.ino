TwoWire tempWire(PB4, PA8);

void tempInit()
{
  tempWire.begin();
  if (!mlx.begin(0x5A, &tempWire)) {
    Serial.println("ERROR: MLX90614 not found at 0x5A on custom TwoWire.");
    Serial.println("Check wiring: SDA=PB4, SCL=PA8, 3.3V power and pull-ups.");
  } else {
    Serial.println("MLX90614 initialized.");
  }
}

void tempTask()
{
  int temp = mlx.readObjectTempC();
  // Serial.print(temp);
  // Serial.print(" ");
  sendToDWIN(0x6000, temp);

}