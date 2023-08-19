#ifndef __EFFECTS_FACTORY_H
#define __EFFECTS_FACTORY_H

#pragma once
#include "effects/ledstripeffect.h"
#include <ArduinoJson.h>

#define JSON_DOC_SIZE 1000

class EffectsFactory
{
public:
    bool CreateEffect(String jsonParams, int8_t* piChannel, LEDStripEffect** poutEffect);
    
    String getLastError()
    {
        return m_strLastError;
    }

protected:
    bool CreateStarryNightEffect(StaticJsonDocument<JSON_DOC_SIZE>* doc, LEDStripEffect** poutEffect);
    bool CreatePaletterEffect(StaticJsonDocument<JSON_DOC_SIZE>* doc, LEDStripEffect** poutEffect);

    bool GetPaletterFromString(String name, CRGBPalette256** pout);

protected:
    String m_strLastError;
};

#endif