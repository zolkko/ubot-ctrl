#include <SPI.h>

int cs = PA_4;

void setup()
{
  pinMode(cs, OUTPUT);
  digitalWrite(cs, HIGH);

  Serial.begin(115200);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV64);
}


void loop()
{
  if (Serial.available()) {
    digitalWrite(cs, LOW);

    while (Serial.available()) {
      int idata = Serial.read();
      int odata = SPI.transfer(idata);
      Serial.write(odata);
    }

    digitalWrite(cs, HIGH);
  }
}

