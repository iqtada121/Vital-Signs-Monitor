const unsigned long int ADCmax = 0x800000;
unsigned long int databuffer[3];
float filteredECG[3] = {0, 0, 0};  // For each channel
float prevRawECG[3] = {0, 0, 0};   // Previous raw inputs
const float alpha = 0.927;  // Adjust this as needed

void setup_ECG() {  // datasheet ads1293
  //Follow the next steps to configure the device for this example, starting from default registers values.
  //1. Set address 0x01 = 0x11: Connect channel 1’s INP to IN2 and INN to IN1.
  writeRegister(0x01, 0x11);
  delay(20);
  //2. Set address 0x02 = 0x19: Connect channel 2’s INP to IN3 and INN to IN1.
  writeRegister(0x02, 0x19);
  delay(20);
  //3. Set address 0x03 = 0x2E: Connects channel 3’s INP to IN5 and INN to IN6.
  writeRegister(0x03, 0x2E);
  delay(20);

  writeRegister(0x04, 0x00);
  delay(20);

  writeRegister(0x07, 0x00);
  delay(20);
  //4. Set address 0x0A = 0x07: Enables the common-mode detector on input pins IN1, IN2 and IN3.
  writeRegister(0x0A, 0x0F);
  delay(20);
  //5. Set address 0x12 = 0x04: Use external crystal and feed the internal oscillator's output to the digital.
  writeRegister(0x12, 0x04);
  delay(20);

  writeRegister(0x0B, 0x07);
  delay(10);
  //6. Set addresses 0x0D = 0x01, 0x0E = 0x02, 0x0F = 0x03: Connects the first buffer of the Wilson reference to the IN1 pin, the second buffer to the IN2 pin, and the third buffer to the IN3 pin.
  writeRegister(0x0C, 0x74);
  delay(10);
  writeRegister(0x0D, 0x01);
  delay(20);
  writeRegister(0x0E, 0x02);
  delay(20);
  writeRegister(0x0F, 0x03);
  delay(20);
  //7. Set address 0x10 = 0x01: Connects the output of the Wilson reference internally to IN6
  writeRegister(0x10, 0x01);
  delay(20);
  //8. Set address 0x12 = 0x04: Uses external crystal and feeds the output of the internal oscillator module to the digital.
  writeRegister(0x12, 0x04);
  delay(20);
  writeRegister(0x13, 0x3F);
  delay(20);
  writeRegister(0x14, 0x00);
  delay(20);
  writeRegister(0x1F, 0x03);
  delay(20);
  //9. Set address 0x21 = 0x08: Configures the R2 decimation rate as 8 for all channels.
  writeRegister(0x21, 0x08);
  delay(20);
  //10. Set address 0x22 = 0x10: Configures the R3 decimation rate as 16 for channel 1.
  writeRegister(0x22, 0x01);
  delay(20);
  //11. Set address 0x23 = 0x10: Configures the R3 decimation rate as 16 for channel 2.
  writeRegister(0x23, 0x10);
  delay(20);
  //12. Set address 0x24 = 0x10: Configures the R3 decimation rate as 16 for channel 3.
  writeRegister(0x24, 0x10);
  delay(20);

  writeRegister(0x25, 0x07);
  delay(20);
  //13. Set address 0x27 = 0x08: Configures the DRDYB source to ECG channel 1 (or fastest channel).
  writeRegister(0x27, 0x08);
  delay(20);

  //14. Set address 0x2F = 0x70: Enables ECG channel 1, ECG channel 2, and ECG channel 3 for loop read-back mode
  // writeRegister(0x2E, 0x30);
  // delay(20);
  writeRegister(0x2F, 0x70);
  delay(20);
  //15. Set address 0x00 = 0x01: Starts data conversion
  writeRegister(0x00, 0x01);
  delay(20);
}

byte readRegister(byte reg) { 
  byte data;
  reg |= 1 << 7;
  digitalWrite(pin_SS, LOW);
  SPI.transfer(reg);
  data = SPI.transfer(0);
  digitalWrite(pin_SS, HIGH);
  return data;
}

void writeRegister(byte reg, byte data) {
  reg &= ~(1 << 7);
  digitalWrite(pin_SS, LOW);
  SPI.transfer(reg);
  SPI.transfer(data);
  digitalWrite(pin_SS, HIGH);
}

void readDataLoop(unsigned long int* databuffer)  //byte byteToRead) //ADS1293 expects address to be in the lower 7 bits. byToRead should be equal to 3.
{
  //
  unsigned long int result = 0x000000000000000000000000;
  //////////
  //////////// Read lead I
  result = readRegister(0x37);
  result = result << 8;
  result |= readRegister(0x38);
  result = result << 8;
  result |= readRegister(0x39);
  databuffer[0] = result;

  /////////// Read lead II
  result = readRegister(0x3A);
  result = result << 8;
  result |= readRegister(0x3B);
  result = result << 8;
  result |= readRegister(0x3C);
  databuffer[1] = result;

  //////Read lead V1
  result = readRegister(0x3D);
  result = result << 8;
  result |= readRegister(0x3E);
  result = result << 8;
  result |= readRegister(0x3F);
  databuffer[2] = result;
}

float convertRawECGtoMillivolts(int32_t rawValue) {
  const float VREF = 2.42;                                    // ADS1293 internal reference voltage (Volts)
  const float GAIN = 3.5;                                     // Default PGA gain
  const float SCALE = (VREF / (pow(2, 23) * GAIN)) * 1000.0;  // mV per bit

  return rawValue * SCALE;  // Result in millivolts
}

void ReadAndTrans() {
  readDataLoop(databuffer);  // Get raw 24-bit values

  for (int i = 0; i < 3; i++) {
    int32_t signedECG = databuffer[i] - 0x400000;

    float raw = (float)signedECG;

    // High-pass filter
    filteredECG[i] = alpha * (filteredECG[i] + raw - prevRawECG[i]);
    prevRawECG[i] = raw;
  }

  // Convert to mV or scaled integer for DWIN
  float lead1_mV = convertRawECGtoMillivolts(filteredECG[1]);
  float lead2_mV = convertRawECGtoMillivolts(filteredECG[2]);
  
  // ---------- SEND TO DWIN ----------
  // DWIN_SendCurve(0x00, (lead1_mV+10)*100);   // channel 0 = Lead I
  // DWIN_SendCurve(0x01, (lead2_mV+10)*100);   // channel 1 = Lead II
  // DWIN_SendDualCurve(int((lead1_mV+10)*100), int((lead2_mV+10)*100));
  

  // ---------- Debug Serial ----------
  Serial.print((lead1_mV+10)*100, 3);
  Serial.print("  ");
  Serial.println((lead2_mV+10)*100, 3);
}

void DWIN_SendDualCurve(int16_t signal1, int16_t signal2) {
    // 17 bytes total: 
    // Header(2) + Len(1) + Write(1) + Addr(2) + BuffHeader(2) + BlockCount(2) 
    // + Chan0Head(2) + Chan0Val(2) + Chan1Head(2) + Chan1Val(2)
    uint8_t packet[18]; 

    packet[0] = 0x5A;
    packet[1] = 0xA5;
    packet[2] = 0x11;      // Length = 17 bytes (0x11)
    packet[3] = 0x82;      // Write instruction
    
    // Address for Curve Buffer (Standard is 0x0310)
    packet[4] = 0x03;
    packet[5] = 0x10; 

    packet[6] = 0x5A;      // Buffer write header
    packet[7] = 0xA5; 

    packet[8] = 0x02;      // Number of curve blocks (2 signals)
    packet[9] = 0x00;      // Reserved

    // --- Channel 0 Block ---
    packet[10] = 0x00;     // Channel 0
    packet[11] = 0x01;     // 1-word length
    packet[12] = highByte(signal1);
    packet[13] = lowByte(signal1);

    // --- Channel 1 Block ---
    packet[14] = 0x01;     // Channel 1
    packet[15] = 0x01;     // 1-word length
    packet[16] = highByte(signal2);
    packet[17] = lowByte(signal2);

    dwinSerial.write(packet, 18);
}