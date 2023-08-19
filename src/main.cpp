#include <Arduino.h>

#ifdef WITH_GDB
#include <GDBStub.h>
#endif

#include "globals.h"

// FileSystem headers
#include <FS.h>
#include <LITTLEFS.h>

#include "WorkingStation.h"

CWorkingStation workStation;

void setup()
{
    // Initialize Serial output
    SERIAL_CONFIGURE;

#ifdef WITH_GDB
    gdbstub_init();
#endif

    delay(2000);

    Println("");
    Println("Starting...");

    bool littleFsEn = LittleFS.begin();
    Print("LittleFS Status: ");
    Println(littleFsEn);

    workStation.Init();

    Println("Setup finished.");
}

void loop()
{
    workStation.Work();
}