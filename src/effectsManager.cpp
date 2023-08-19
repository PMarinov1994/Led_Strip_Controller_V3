#include "effectsManager.h"
#include "globals.h"

extern AppTime g_AppTime;
volatile float gVURatio = 1.0; // Current VU as a ratio to its recent min and max
// std::shared_ptr<LEDMatrixGFX> m_pLedStrip; // Each LED strip gets its own channel

EffectsManager::EffectsManager(uint8_t bChannelNum)
    : _statusEffect(NULL), _currEffect(NULL), _factory(), _errReporter(NULL), _brightnes(255), _lastErrTime(0), _bChannelNum(bChannelNum), _bEnabled(false)
{
    // m_pLedStrip = std::make_unique<LEDMatrixGFX>(MATRIX_WIDTH, MATRIX_HEIGHT);

    // effect = new BulgarianFlag();
    // effect = new Marquee(false);
    // effect = new RainbowComet(NUM_LEDS / 2);
    // effect = new BouncingBallEffect(1, false);
    // effect = new DoublePaletteEffect(); // No memory ?? try it on ESP
    // effect = new PaletteFlameEffect(vuPaletteGreen); //  No memory ?? try it on ESP
    // effect = new PaletteFlameEffect(vuPaletteGreen); // No memory ?? try it on ESP
    // effect = new ClassicFireEffect(true); // No FastLED.clear();
    // effect = new MeteorEffect(); No FastLED.clear();
    // effect = new SimpleRainbowTestEffect(4, 2); // No FastLED.clear();
    // effect = new RainbowTwinkleEffect(); // No FastLED.clear();
    // effect = new RainbowFillEffect(); // No FastLED.clear();
    // effect = new ColorFillEffect(); // No FastLED.clear();
    // effect = new TwinkleEffect(); // No FastLED.clear();
    // effect = new TwinkleStarEffect(); // No FastLED.clear();
}

EffectsManager::~EffectsManager()
{
    if (_statusEffect != NULL)
        delete _statusEffect;

    if (_currEffect != NULL)
        delete _currEffect;
}

void EffectsManager::init(IErrorReporter *errReporter)
{
    m_pLedStrip = std::make_unique<LEDMatrixGFX>(MATRIX_WIDTH, MATRIX_HEIGHT);

    if (_bChannelNum == 0)
    {
        pinMode(LED_PIN0, OUTPUT);
        FastLED.addLeds<WS2812, LED_PIN0, COLOR_ORDER>(m_pLedStrip->GetLEDBuffer(), m_pLedStrip->GetLEDCount());
    }
    else if (_bChannelNum == 1)
    {
        pinMode(LED_PIN1, OUTPUT);
        FastLED.addLeds<WS2812, LED_PIN1, COLOR_ORDER>(m_pLedStrip->GetLEDBuffer(), m_pLedStrip->GetLEDCount());
    }

    FastLED[_bChannelNum].setLeds(m_pLedStrip->GetLEDBuffer(), m_pLedStrip->GetLEDCount());

    _errReporter = errReporter;
    _statusEffect = new StatusEffect();
    _statusEffect->Init(m_pLedStrip);
}


void EffectsManager::setEnabled(bool bEnabled)
{
    this->_bEnabled = bEnabled;
    if (!this->_bEnabled)
    {
        FastLED[_bChannelNum].clearLedData();
        FastLED[_bChannelNum].showLeds();
    }
}


void EffectsManager::changeEffect(String jsonParams)
{
    int8_t iChannel = -1;
    LEDStripEffect* newEffect = NULL;
    bool result = _factory.CreateEffect(jsonParams, &iChannel, &newEffect);

    bool bIgnore = iChannel >= 0 && iChannel != _bChannelNum;

    if ((!result || !bIgnore) && _currEffect != NULL)
    {
        delete _currEffect;
        _currEffect = NULL;
    }

    if (!result)
    {
        if (newEffect != NULL) // Should not be allocated if we failed, but just in case.
            delete newEffect;

        Println("Error: Effect creation failed!");

        _statusEffect->setError(StatusEffect::ERROR::GENERAL);
        _lastErrTime = millis();
        _errReporter->ReportError(_factory.getLastError());

        return;
    }

    if (bIgnore)
        return;

    _currEffect = newEffect;
    _currEffect->Init(m_pLedStrip);
    _errReporter->ReportError(String(""));
    _statusEffect->setError(StatusEffect::ERROR::NONE);

    Print("Effect successfully created -> ");
    Println(_currEffect->FriendlyName());
}



void EffectsManager::setBrightnes(uint8_t value)
{
    _brightnes = value;
}



void EffectsManager::loop()
{
    if (SYS_LED_CHANNEL == _bChannelNum && StatusEffect::ERROR::NONE != _statusEffect->getError())
    {
        _statusEffect->Draw();

        // Reset the error log
        if (_lastErrTime + ERROR_SHOW_TIME < millis())
            _statusEffect->setError(StatusEffect::ERROR::NONE);
    }
    else if (_currEffect != NULL)
        _currEffect->Draw();
    else
        _statusEffect->Draw();

    FastLED[_bChannelNum].showLeds(_brightnes);
}

void EffectsManager::onWiFiStatusChanged(bool up)
{
    if (up)
        _statusEffect->setError(StatusEffect::ERROR::NONE);
    else
        _statusEffect->setError(StatusEffect::ERROR::WIFI);
}

void EffectsManager::onMqttStatusChanged(bool up)
{
    if (up)
        _statusEffect->setError(StatusEffect::ERROR::NONE);
    else
        _statusEffect->setError(StatusEffect::ERROR::MQTT);
}

String EffectsManager::getCurrEffectName()
{
    if (_currEffect != NULL)
        return String(_currEffect->FriendlyName());
    else
        return String("No Effect Playing");
}