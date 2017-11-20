#include <EEPROM.h>
char* EEPROMversion = strdup("redis1");

void clearEEPROM() {
  Serial.print("Erasing EEPROM...");
  // Clear EEPROM
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);
  EEPROM.commit();
  Serial.println("OK");
  EEPROM.end();
}

uint8_t readStringFromEEPROM(uint8_t offset, char* &value, uint8_t maxLength) {
  int addr;
  uint8_t len = EEPROM.read(offset);
  if (((offset + 1 + len) > 512) || (len > maxLength)) {
    Serial.println("Bad header...");
    clearEEPROM();
    // Format EEPROM with curent(default) values
    saveToEEPROM();
    EEPROM.begin(512);
    return readStringFromEEPROM(offset, value, maxLength);
  }
  value = (char*)realloc(value, len + 1);
  for (addr = offset + 1; addr <= offset + len; addr++)
    value[addr - offset - 1] = EEPROM.read(addr);
  value[len] = 0;

  return (offset + 1 + len);
}

uint8_t writeStringToEEPROM(uint8_t offset, char* &value) {
  int addr;
  EEPROM.write(offset, strlen(value));
  for (addr = offset + 1; addr <= offset + strlen(value); addr++)
    EEPROM.write(addr, value[addr - offset - 1]);
  return (offset + 1 + strlen(value));
}

