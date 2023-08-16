#include "effectsManager.h"
#include "globals.h"

AppTime g_AppTime;                                      // Keeps track of frame times
volatile float gVURatio = 1.0;                          // Current VU as a ratio to its recent min and max
std::shared_ptr<LEDMatrixGFX> g_pStrands[NUM_CHANNELS]; // Each LED strip gets its own channel

EffectsManager::EffectsManager()
    : _statusEffect(NULL), _currEffect(NULL), _factory(), _errReporter(NULL), _brightnes(255), _lastErrTime(0)
{
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
    // Initialize the strand controllers depending on how many channels we have
    for (int i = 0; i < NUM_CHANNELS; i++)
        g_pStrands[i] = std::make_unique<LEDMatrixGFX>(MATRIX_WIDTH, MATRIX_HEIGHT);

#if NUM_CHANNELS == 1
    pinMode(LED_PIN0, OUTPUT);
    FastLED.addLeds<WS2812, LED_PIN0, COLOR_ORDER>(g_pStrands[0]->GetLEDBuffer(), g_pStrands[0]->GetLEDCount());
    FastLED[0].setLeds(g_pStrands[0]->GetLEDBuffer(), g_pStrands[0]->GetLEDCount());
#endif

#if NUM_CHANNELS >= 2
    pinMode(LED_PIN0, OUTPUT);
    FastLED.addLeds<WS2812B, LED_PIN0, COLOR_ORDER>(g_pStrands[0]->GetLEDBuffer(), g_pStrands[0]->GetLEDCount());

    pinMode(LED_PIN1, OUTPUT);
    FastLED.addLeds<WS2812B, LED_PIN1, COLOR_ORDER>(g_pStrands[1]->GetLEDBuffer(), g_pStrands[1]->GetLEDCount());
#endif

    FastLED.setBrightness(_brightnes);

    _errReporter = errReporter;
    _statusEffect = new StatusEffect();
    _statusEffect->Init(g_pStrands);
}

void EffectsManager::changeEffect(String jsonParams)
{
    if (_currEffect != NULL)
    {
        delete _currEffect;
        _currEffect = NULL;
    }

    bool result = _factory.CreateEffect(jsonParams, &_currEffect);
    if (!result)
    {
        Println("Error: Effect creation failed!");

        _statusEffect->setError(StatusEffect::ERROR::GENERAL);
        _lastErrTime = millis();
        _errReporter->ReportError(_factory.getLastError());
    }
    else
    {
        _currEffect->Init(g_pStrands);
        _errReporter->ReportError(String(""));
        _statusEffect->setError(StatusEffect::ERROR::NONE);

        Print("Effect successfully created -> ");
        Println(_currEffect->FriendlyName());
    }
}

void EffectsManager::setBrightnes(uint8_t value)
{
    _brightnes = value;
    FastLED.setBrightness(_brightnes);
}

void EffectsManager::loop()
{
    g_AppTime.NewFrame();

    if (StatusEffect::ERROR::NONE != _statusEffect->getError())
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

    FastLED.delay(20);
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