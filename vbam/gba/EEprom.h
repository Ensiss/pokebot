#ifndef EEPROM_H
#define EEPROM_H

#ifdef __LIBRETRO__
extern void eepromSaveGame(u8* &data);
extern void eepromReadGame(const u8 *&data, int version);
#else
extern void eepromSaveGame(gzFile _gzFile);
extern void eepromReadGame(gzFile _gzFile, int version);
#endif
extern void eepromReadGameSkip(gzFile _gzFile, int version);
extern int eepromRead(u32 address);
extern void eepromWrite(u32 address, u8 value);
extern void eepromInit();
extern void eepromReset();
#ifdef __LIBRETRO__
extern u8 *eepromData;
#else
extern u8 eepromData[0x2000];
#endif
extern bool eepromInUse;
extern int eepromSize;

#define EEPROM_IDLE           0
#define EEPROM_READADDRESS    1
#define EEPROM_READDATA       2
#define EEPROM_READDATA2      3
#define EEPROM_WRITEDATA      4

#endif // EEPROM_H
