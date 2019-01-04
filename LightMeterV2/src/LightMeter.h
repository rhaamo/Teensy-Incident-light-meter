#ifndef LUX_PANEL_H
#define LUX_PANEL_H

// Main Library
#include <Arduino.h>

// Battery Babysitter
#include <SparkFunBQ27441.h>
// Light, Lux, Meter
#include <SparkFunTSL2561.h>
// For user config
#include <EEPROM.h>
// Oled display
#include <TeensyView.h>
// Input handling
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>              // http://www.pjrc.com/teensy/td_libs_Encoder.html
#include <Button.h>               // https://github.com/r89m/Button
#include <ButtonEventCallback.h>  // Button too
#include <PushButton.h>           // https://github.com/r89m/PushButton
#include <Bounce2.h>              // https://github.com/thomasfredericks/Bounce2

// Custom OLED routines
#include "OledFunctions.h"

// Pinout declaration of TeensyView module
// The module is in Alternate configuration
#define PIN_RESET 2 // RST
#define PIN_DC 21   // DC
#define PIN_CS 20   // CS
#define PIN_SCK 14  // SCK
#define PIN_MOSI 7  // MOSI


#define PIN_POWER_ON 16

/* Calibration constant (default was: 16)
 * ISO 2720:1973 recommands a K range from 10.6 to 13.4
 * Sekonic, Canon and Nikon uses 12.5
 * Minolta, Kenko and Pentax uses 14
 * Difference between thoses two is 1/6 EV
 */
#define KValue 12.5

#define LIPO_CAPACITY 1000 // mAh

// Menu State
enum MState_t {
    Minit,                  // Initial state page
    MDisplayPhotoValueInit, // Init page for photo light metering
    MDisplayPhotoValue,     // Page for photo light metering
    MDisplayVideoValueInit, // Init for video light metering
    MDisplayVideoValue,     // Video light metering
    MSetISOInit,            // Init for ISO Selection
    MSetISO,                // ISO Selection
    MSystemInit,
    MSystem,                // System informations
    MMaryInit,
    MMary,                  // <3
    MDisplayLuxValueInit,
    MDisplayLuxValue        // Lux value only
};

// Hold State
enum holdState_t {
    hSRun,
    hSAveraging,
    hSHeld
};

/*
 * Fuji Body: 22 16 11 8 5.6 4 2.8 2 1.4
 * Fuji 28: 16 11 8 5.6 4 2.8
 * Fuji 80-200: 16 11 8 5.6 3.8
 * Fuji 135: 16 11 8 5.6 4 2.8
 * Fuji 50: 16 11 8 5.6 4 2.8 1.9
 * Soligor 35-140: 22 16 11 8 5.6 3.8
 * Telemar 22A: 22 16 11 8 5.6
 * ARSAT: 22 16 11 8 5.6 3.5
 * MTO: 10.5
 * Warra box: 11
 * 
 * Fuji mixed: 22, 16, 11, 8, 5.6, 4, 3.8, 2.8, 2, 1.9, 1.4
 * 
 * Full: 22, 20, 18, 16, 14, 13, 11, 10, 10.5, 9, 8, 7.1, 6.3, 5.6, 5, 4.5, 4, 3.8, 3.5, 3.2, 2.8, 2.5, 2.2, 2, 1.9, 1.8, 1.4
 */

enum FS_table {
	TableMto,
	TableWarra,
	TableFuji,
	TableFull // default
};

// Custom tables
const float fStopTableMto[1] = {10.5};
const float fStopTableWarra[1] = {11};
const float fStopTableFuji[11] = {1.4, 1.9, 2, 2.8, 3.8, 4, 5.6, 8, 11, 16, 22};
// Default table
const float fStopTable[27] = {1.4, 1.8, 1.9, 2, 2.2, 2.5, 2.8,
							  3.2, 3.5, 3.8, 4, 4.5, 5, 5.6,
							  6.3, 7.1, 8, 9, 10.5, 10, 11, 13,
							  14, 16, 18, 20, 22};

const float exposureTable[3] = {0.041666666, 0.033333333, 0.016666666};
#define ISO_COUNT 9
uint8_t isoTableSize = ISO_COUNT;
const float isoTable[ISO_COUNT] = {25, 100, 125, 200, 400, 800, 1600, 3200, 6400};

// EEprom user config
struct CfgUser {
	FS_table fStops;             // Unused for now
    uint8_t fStopSetting = 14;   // 5.6; Saved on shutdown
    uint8_t exposureSetting = 1; // 0.04; Saved on shutdown
    uint8_t isoSetting = 5;	     // 400; Saved on change
};

class LightMeter {
    public:
        LightMeter(void);
        void process();
        void powerDown(void);

        bool lipoGood;
        unsigned char sensorID;
        unsigned char sensorTime = 2;
        unsigned int  sensorMs; // Integration ("shutter") time in milliseconds
        bool gain = 0; // Gain setting, 0 = X1, 1 = X16;

    private:
        void updateLux(double inputLux); // averaging function
        void loadConfigUser();
        void saveConfigUser();
        void getLuxAndCompute(bool fstop);
        void getLux();
        uint32_t eeprom_crc(uint16_t position);

        MState_t state;
        holdState_t triggerState;
        double luxAccumulator;
        double lux;
        uint32_t sampleCount;
        uint8_t heartCount;

        // Our Marker to validate EEPROM is valid
        const String progName = "LightMeter";
	    int addrConfigUser = 0;      // Always save at position 0 in EEPROM
        uint32_t addrMarker = 2000;       // Store marker at 2000 bytes in EEPROM

        long encoderPos = -999;

        CfgUser ConfigUser;
};

#endif