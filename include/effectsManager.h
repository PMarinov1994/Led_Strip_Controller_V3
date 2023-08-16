#ifndef __EFFECTS_MANAGER_H
#define __EFFECTS_MANAGER_H

#pragma once
#include "effects/ledstripeffect.h"
#include "effectsFactory.h"
#include <string>
#include "effects/misceffects.h"
#include "IErrorReported.h"

class EffectsManager
{

public:
    EffectsManager();
    virtual ~EffectsManager();


public:

    void setBrightnes(uint8_t value);
    uint8_t getBrightnes() { return _brightnes; };

    void init(IErrorReporter* errReporter);
    void changeEffect(String jsonParams);
    void loop();

    void onWiFiStatusChanged(bool up);
    void onMqttStatusChanged(bool up);

    String getCurrEffectName();

private:

    StatusEffect* _statusEffect;
    LEDStripEffect* _currEffect;
    EffectsFactory _factory;
    IErrorReporter* _errReporter;

    uint8_t _brightnes;
    unsigned long _lastErrTime;
};

#endif