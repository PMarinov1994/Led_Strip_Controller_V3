//+--------------------------------------------------------------------------
//
// File:        effects.cpp
//
// NightDriverStrip - (c) 2018 Plummer's Software LLC.  All Rights Reserved.
//
// This file is part of the NightDriver software project.
//
//    NightDriver is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    NightDriver is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Nightdriver.  It is normally found in copying.txt
//    If not, see <https://www.gnu.org/licenses/>.
//
// Description:
//
//    Main table of built-in effects and related constants and data
//
// History:     Jul-14-2021         Davepl      Split off from main.cpp
//---------------------------------------------------------------------------

#include "globals.h"                     // CONFIG and global headers
#include "effects/bouncingballeffect.h"  // bouincing ball effectsenable+
#include "effects/doublepaletteeffect.h" // double palette effect
#include "effects/fireeffect.h"          // fire effects
#include "effects/meteoreffect.h"        // meteor blend effect
#include "effects/misceffects.h"
#include "effects/paletteeffect.h" // palette effects
#include "effects/particles.h"
#include "effects/stareffect.h" // star effects

#include "effectsFactory.h"
#include <ArduinoJson.h>
#include <string>

//
// Externals
//

// extern std::shared_ptr<LEDMatrixGFX> m_pLedStrip;

// Palettes
//
// Palettes that are referenced by effects need to be instantiated first

CRGBPalette256 BlueColors_p =
    {
        CRGB::DarkBlue,
        CRGB::MediumBlue,
        CRGB::Blue,
        CRGB::MediumBlue,
        CRGB::DarkBlue,
        CRGB::MediumBlue,
        CRGB::Blue,
        CRGB::MediumBlue,
        CRGB::DarkBlue,
        CRGB::MediumBlue,
        CRGB::Blue,
        CRGB::MediumBlue,
        CRGB::DarkBlue,
        CRGB::MediumBlue,
        CRGB::Blue,
        CRGB::MediumBlue};

CRGBPalette256 RedColors_p =
    {
        CRGB::Red,
        CRGB::DarkRed,
        CRGB::DarkRed,
        CRGB::DarkRed,

        CRGB::Red,
        CRGB::DarkRed,
        CRGB::DarkRed,
        CRGB::DarkRed,

        CRGB::Red,
        CRGB::DarkRed,
        CRGB::DarkRed,
        CRGB::DarkRed,

        CRGB::Red,
        CRGB::DarkRed,
        CRGB::DarkRed,
        CRGB::OrangeRed,
};

CRGBPalette256 GreenColors_p =
    {
        CRGB::Green,
        CRGB::DarkGreen,
        CRGB::DarkGreen,
        CRGB::DarkGreen,

        CRGB::Green,
        CRGB::DarkGreen,
        CRGB::DarkGreen,
        CRGB::DarkGreen,

        CRGB::Green,
        CRGB::DarkGreen,
        CRGB::DarkGreen,
        CRGB::DarkGreen,

        CRGB::Green,
        CRGB::DarkGreen,
        CRGB::DarkGreen,
        CRGB::LimeGreen,
};

CRGBPalette256 PurpleColors_p =
    {
        CRGB::Purple,
        CRGB::Maroon,
        CRGB::Violet,
        CRGB::DarkViolet,

        CRGB::Purple,
        CRGB::Maroon,
        CRGB::Violet,
        CRGB::DarkViolet,

        CRGB::Purple,
        CRGB::Maroon,
        CRGB::Violet,
        CRGB::DarkViolet,

        CRGB::Pink,
        CRGB::Maroon,
        CRGB::Violet,
        CRGB::DarkViolet,
};

CRGBPalette256 RGBColors_p =
    {
        CRGB::Red,
        CRGB::Green,
        CRGB::Blue,
        CRGB::Red,
        CRGB::Green,
        CRGB::Blue,
        CRGB::Red,
        CRGB::Green,
        CRGB::Blue,
        CRGB::Red,
        CRGB::Green,
        CRGB::Blue,
        CRGB::Red,
        CRGB::Green,
        CRGB::Blue,
        CRGB::Blue};

CRGBPalette256 MagentaColors_p =
    {
        CRGB::Pink,
        CRGB::DeepPink,
        CRGB::HotPink,
        CRGB::LightPink,
        CRGB::LightCoral,
        CRGB::Purple,
        CRGB::MediumPurple,
        CRGB::Magenta,
        CRGB::DarkMagenta,
        CRGB::DarkSalmon,
        CRGB::MediumVioletRed,
        CRGB::Pink,
        CRGB::DeepPink,
        CRGB::HotPink,
        CRGB::LightPink,
        CRGB::Magenta};

CRGBPalette256 spectrumBasicColors =
    {
        CRGB(0xFD0E35), // Red
        CRGB(0xFF8833), // Orange
        CRGB(0xFFEB00), // Middle Yellow
        CRGB(0xAFE313), // Inchworm
        CRGB(0x3AA655), // Green
        CRGB(0x8DD9CC), // Middle Blue Green
        CRGB(0x0066FF), // Blue III
        CRGB(0xDB91EF), // Lilac
        CRGB(0xFD0E35), // Red
        CRGB(0xFF8833), // Orange
        CRGB(0xFFEB00), // Middle Yellow
        CRGB(0xAFE313), // Inchworm
        CRGB(0x3AA655), // Green
        CRGB(0x8DD9CC), // Middle Blue Green
        CRGB(0x0066FF), // Blue III
        CRGB(0xDB91EF)  // Lilac
};

CRGBPalette256 BGColors_p =
    {
        CRGB::White,
        CRGB::White,
        CRGB::White,
        CRGB::White,
        CRGB::White,
        CRGB::White,
        CRGB::Green,
        CRGB::Green,
        CRGB::Green,
        CRGB::Green,
        CRGB::Green,
        CRGB::Red,
        CRGB::Red,
        CRGB::Red,
        CRGB::Red,
        CRGB::Red,
};

// AllEffects
//
// The master effects table

CRGBPalette256 rainbowPalette(RainbowColors_p);
CRGBPalette256 blueSweep(CRGB::Blue, CRGB::Green);

CRGBPalette256 BlueStripes(CRGB::White, CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::White, CRGB::Black, CRGB::Black,
                           CRGB::White, CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::Blue, CRGB::White, CRGB::Black, CRGB::Black);

CRGBPalette256 MagentaStripes(CRGB::White, CRGB::Magenta, CRGB::Magenta, CRGB::Magenta, CRGB::Magenta, CRGB::White, CRGB::Black, CRGB::Black,
                              CRGB::White, CRGB::Magenta, CRGB::Magenta, CRGB::Magenta, CRGB::Magenta, CRGB::White, CRGB::Black, CRGB::Black);

#define STARRYNIGHT_PROBABILITY 1.0
#define STARRYNIGHT_MUSICFACTOR 1.0

// AllEffects
//
// A list of internal effects, if any.
/*
LEDStripEffect * AllEffects[] =
{
    new RainbowFillEffect(6, 2),
    new ClassicFireEffect(),

    // Yes, I made a sparkly LED belt and wore it to a party.  Batteries toO!
    new TwinkleEffect(NUM_LEDS/4, 10),

    // new PaletteFlameEffect("Smooth Red Fire", heatmap_pal),
    new ClassicFireEffect(),
    // new PaletteFlameEffect("Smooth Purple Fire", purpleflame_pal),
    new BouncingBallEffect(),
    new MeteorEffect(),                                                                                                     // Our overlapping color meteors


    new SimpleRainbowTestEffect(8, 4),                                                                            // Rainbow palette simple test of walking pixels

    new DoublePaletteEffect(),
    new ColorFillEffect(CRGB::White, 1),
    new RainbowFillEffect(60, 0),
    new BouncingBallEffect(3, true, true, 1),

    new MeteorEffect(20, 1, 25, .15, .05),
    new MeteorEffect(12, 1, 25, .15, .08),
    new MeteorEffect(6, 1, 25, .15, .12),
    new MeteorEffect(1, 1, 5, .15, .25),
    new MeteorEffect(), // Rainbow palette

    new BouncingBallEffect(),
    // new PaletteFlameEffect("Smooth Red Fire", heatmap_pal),
    //new ClassicFireEffect(),

    // new PaletteFlameEffect("Smooth Purple Fire", purpleflame_pal),

    new MeteorEffect(),                                                                                                     // Our overlapping color meteors



    //        new VUFlameEffect("Sound Flame (Green)",    VUFlameEffect::GREEN),
    //       new VUFlameEffect("Sound Flame (Blue)",     VUFlameEffect::BLUE),

    new SimpleRainbowTestEffect(8, 4),                                                                            // Rainbow palette simple test of walking pixels

    new DoublePaletteEffect(),

    // new PaletteFlameEffect("Smooth Red Fire", heatmap_pal, true, 4.5, 1, 1, 255, 4, false),
    new SimpleRainbowTestEffect(8, 1),  // Rainbow palette simple test of walking pixels
    new RainbowFillEffect(24, 0),
    new RainbowFillEffect(),


    new BouncingBallEffect(),

    // Fast: new PaletteEffect(RainbowStripeColors_p, 8.0, .125, 0, 1, 0), // Rainbow palette
    // Fast: new TwinkleEffect(NUM_LEDS/2, 10),
    new SimpleRainbowTestEffect(8, 1),  // Rainbow palette simple test of walking pixels
    // Fast: new TwinkleEffect(NUM_LEDS/2, 0, 50),
    new RainbowFillEffect(),

    new RainbowFillEffect(24, 0),
    new RainbowFillEffect(32, 1),
    new SimpleRainbowTestEffect(8, 1),  // Rainbow palette simple test of walking pixels
    new SimpleRainbowTestEffect(8, 4),                                                                            // Rainbow palette simple test of walking pixels

    new MeteorEffect(),                                                                                                     // Our overlapping color meteors

    new ClassicFireEffect(true),


    // new PaletteEffect(RainbowStripeColors_p, 8.0, .125, 0, 5, 1), // Rainbow palette
    new StatusEffect(CRGB::White),

    new RainbowFillEffect(24, 0),
    new RainbowFillEffect(32, 1),
    new SimpleRainbowTestEffect(8, 1),  // Rainbow palette simple test of walking pixels

    new DoublePaletteEffect(),

};
*/

bool EffectsFactory::CreateEffect(String jsonParams, int8_t *piChannel, LEDStripEffect **poutEffect)
{
    const int capacity = JSON_OBJECT_SIZE(15);

    Print("JSON capacity (in bytes) = ");
    Println(capacity);

    StaticJsonDocument<1000> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, jsonParams);

    bool result = true;

    // Test if parsing succeeds.
    // TODO We can publish the error or something
    if (error)
    {
        m_strLastError = "deserializeJson() failed: " + String(error.f_str());
        Println(m_strLastError);

        result = false;
    }
    else if (!doc.containsKey("name"))
    {
        Println("ERROR: JSON no 'name' found.");
        m_strLastError = "Effect name is missing.";

        result = false;
    }

    if (!result)
    {
        doc.clear();
        return false;
    }

    *piChannel = doc["channel"] | -1;

    String effectName = doc["name"];
    if (effectName == "TwinkleStarEffect")
    {
        Println("JSON: name = TwinkleStarEffect");
        *poutEffect = new TwinkleStarEffect();
    }
    else if (effectName == "StarryNightEffect")
    {
        Println("JSON: name = StarryNightEffect");
        result = this->CreateStarryNightEffect(&doc, poutEffect);
    }
    else if (effectName == "PaletterEffect")
    {
        Println("JSON: name = PaletterEffect");
        result = this->CreatePaletterEffect(&doc, poutEffect);
    }
    else if (effectName == "RainbowTwinkleEffect")
    {
        Println("JSON: name = RainbowTwinkleEffect");
        float speed = doc["speedDivisor"] | 12.0f;
        int deltaHue = doc["deltaHue"] | 14;

        *poutEffect = new RainbowTwinkleEffect(speed, deltaHue);
    }
    else if (effectName == "RainbowFillEffect")
    {
        Println("JSON: name = RainbowFillEffect");
        float speed = doc["speedDivisor"] | 12.0f;
        int deltaHue = doc["deltaHue"] | 14;

        *poutEffect = new RainbowFillEffect(speed, deltaHue);
    }
    else if (effectName == "Marquee")
    {
        Println("JSON: name = Marquee");
        bool mirror = doc["mirror"] | false;
        *poutEffect = new Marquee(mirror);
    }
    else if (effectName == "BulgarianFlag")
    {
        Println("JSON: name = BulgarianFlag");
        bool reverse = doc["reverse"] | false;
        *poutEffect = new BulgarianFlag(reverse);
    }
    else if (effectName == "MeteorEffect")
    {
        Println("JSON: name = MeteorEffect");

        int meteors = doc["meteors"] | 4;
        unsigned int size = doc["size"] | 4;
        unsigned int decay = doc["decay"] | 3;
        double minSpeed = doc["minSpeed"] | 0.2;
        double maxSpeed = doc["maxSpeed"] | 0.2;

        *poutEffect = new MeteorEffect(meteors, size, decay, minSpeed, maxSpeed);
    }
    else if (effectName == "FireEffect")
    {
        Println("JSON: name = FireEffect");

        int cellsPerLED = doc["cellsPerLED"] | 1;
        int cooling = doc["cooling"] | 20;
        int sparking = doc["sparking"] | 100;
        int sparks = doc["sparks"] | 3;
        int sparkHeight = doc["sparkHeigh"] | 4;
        bool reversed = doc["reversed"] | false;
        bool mirrored = doc["mirrored"] | false;

        *poutEffect = new FireEffect(NUM_LEDS, cellsPerLED, cooling, sparking, sparks, sparkHeight, reversed, mirrored);
    }
    else if (effectName == "PaletteFlameEffect")
    {
        Println("JSON: name = PaletteFlameEffect");

        CRGBPalette256 *palette;
        String paletteName = doc["palette"] | String("RGB");
        this->GetPaletterFromString(paletteName, &palette);

        int cellsPerLED = doc["cellsPerLED"] | 1;
        int cooling = doc["cooling"] | 1.8;
        int sparking = doc["sparking"] | 100;
        int sparkHeight = doc["sparkHeigh"] | 3;
        bool reversed = doc["reversed"] | false;
        bool mirrored = doc["mirrored"] | false;

        *poutEffect = new PaletteFlameEffect("Custom PaletteFlameEffect", *palette, NUM_LEDS, cellsPerLED, cooling, sparking, sparkHeight, reversed, mirrored);
    }
    else if (effectName == "ClassicFireEffect")
    {
        Println("JSON name = ClassicFireEffect");

        bool mirrored = doc["mirrored"] | false;
        bool reversed = doc["reversed"] | false;
        int cooling = doc["cooling"] | 5;

        *poutEffect = new ClassicFireEffect(mirrored, reversed, cooling);
    }
    else if (effectName == "SmoothFireEffect")
    {
        Println("JSON: name = SmoothFireEffect");

        bool reversed = doc["reversed"] | false;
        float cooling = doc["cooling"] | 1.2f;
        int sparks = doc["sparks"] | 16;
        int driftPasses = doc["driftPasses"] | 1;
        float drift = doc["drift"] | 48;
        int sparkHeight = doc["sparkHeight"] | 12;
        bool turbo = doc["turbo"] | false;
        bool mirrored = doc["mirrored"] | false;

        *poutEffect = new SmoothFireEffect(reversed, cooling, sparks, driftPasses, drift, sparkHeight, turbo, mirrored);
    }
    else if (effectName == "BaseFireEffect")
    {
        Println("JSON: name = BaseFireEffect");

        int cellsPerLED = doc["cellsPerLED"] | 1;
        int cooling = doc["cooling"] | 20;
        int sparking = doc["sparking"] | 100;
        int sparks = doc["sparks"] | 3;
        int sparkHeight = doc["sparkHeight"] | 4;
        bool reversed = doc["reversed"] | false;
        bool mirrored = doc["mirrored"] | false;

        *poutEffect = new BaseFireEffect(NUM_LEDS, cellsPerLED, cooling, sparking, sparks, sparkHeight, reversed, mirrored);
    }
    else if (effectName == "DoublePaletteEffect")
    {
        Println("JSON: name = DoublePaletteEffect");
        *poutEffect = new DoublePaletteEffect();
    }
    else if (effectName == "BouncingBallEffect")
    {
        Println("JSON: name = BouncingBallEffect");

        size_t ballCount = doc["ballCount"] | 3;
        bool mirrored = doc["mirrored"] | false;
        int ballSize = doc["ballSize"] | 5;

        *poutEffect = new BouncingBallEffect(ballCount, mirrored, true, ballSize);
    }
    else if (effectName == "SolidFill")
    {
        Println("JSON: name = SolidFill");

        uint8_t ir = doc["red"] | 255;
        uint8_t ig = doc["green"] | 255;
        uint8_t ib = doc["blue"] | 255;

        *poutEffect = new SolidFillEffect(ir, ig, ib);
    }
    else
    {
        m_strLastError = "Effect " + effectName + " does not exists.";
        Println(m_strLastError);

        result = false;
    }

    doc.clear();
    return result;
}

bool EffectsFactory::CreatePaletterEffect(StaticJsonDocument<JSON_DOC_SIZE> *doc, LEDStripEffect **poutEffect)
{
    if (doc->containsKey("buildIn"))
    {
        String buildIn = doc->getMember("buildIn");

        if (buildIn == "Rainbow2")
            *poutEffect = new PaletteEffect(RainbowColors_p); // Rainbow palette
        else if (buildIn == "Rainbow")
            *poutEffect = new PaletteEffect(MagentaColors_p); // Rainbow palette
        else if (buildIn == "RanbowSimple")
            *poutEffect = new PaletteEffect(rainbowPalette, 256 / 16, .2, 0); // Simple rainbow pallette
        else
        {
            m_strLastError = "PleatterEffect buildIn effect was not found";
            return false;
        }

        return true;
    }

    String paletteName = doc->getMember("palette") | "RGB";
    CRGBPalette256 *palette;
    this->GetPaletterFromString(paletteName, &palette);

    *poutEffect = new PaletteEffect(*palette);
    return true;
}

bool EffectsFactory::CreateStarryNightEffect(StaticJsonDocument<JSON_DOC_SIZE> *doc, LEDStripEffect **poutEffect)
{
    if (doc->containsKey("buildIn"))
    {
        Println("Trying to create buildIn effect");

        String buildIn = doc->getMember("buildIn");

        if (buildIn == "Rainbow Twinkle Stars")
            *poutEffect = new StarryNightEffect<QuietStar>("Rainbow Twinkle Stars", RainbowColors_p, STARRYNIGHT_PROBABILITY, 1, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Rainbow Twinkle
        else if (buildIn == "Green Twinkle")
            *poutEffect = new StarryNightEffect<QuietStar>("Magenta Twinkle Stars", GreenColors_p, STARRYNIGHT_PROBABILITY, 1, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Green Twinkle
        else if (buildIn == "Blue Sparkle")
            *poutEffect = new StarryNightEffect<Star>("Blue Sparkle Stars", BlueColors_p, STARRYNIGHT_PROBABILITY, 1, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Blue Sparkle
        else if (buildIn == "Red Twinkle")
            *poutEffect = new StarryNightEffect<QuietStar>("Red Twinkle Stars", MagentaColors_p, 1.0, 1, LINEARBLEND, 2.0); // Red Twinkle
        else if (buildIn == "Lava Stars")
            *poutEffect = new StarryNightEffect<Star>("Lava Stars", MagentaColors_p, STARRYNIGHT_PROBABILITY, 1, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Lava Stars
        else if (buildIn == "Blooming Little Rainbow Stars")
            *poutEffect = new StarryNightEffect<BubblyStar>("Little Blooming Rainbow Stars", MagentaColors_p, STARRYNIGHT_PROBABILITY, 4, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Blooming Little Rainbow Stars
        else if (buildIn == "Blooming Rainbow Stars")
            *poutEffect = new StarryNightEffect<BubblyStar>("Big Blooming Rainbow Stars", MagentaColors_p, 2, 12, LINEARBLEND, 1.0); // Blooming Rainbow Stars
        else if (buildIn == "Neon Bars")
            *poutEffect = new StarryNightEffect<BubblyStar>("Neon Bars", MagentaColors_p, 0.5, 64, NOBLEND, 0); // Neon Bars
        else if (buildIn == "Lava Stars")
            new StarryNightEffect<HotWhiteStar>("Lava Stars", HeatColors_p, STARRYNIGHT_PROBABILITY, 1, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Lava Stars
        else if (buildIn == "Little Blooming Rainbow Stars")
            *poutEffect = new StarryNightEffect<BubblyStar>("Little Blooming Rainbow Stars", BlueColors_p, STARRYNIGHT_PROBABILITY, 4, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Blooming Little Rainbow Stars
        else if (buildIn == "Green Twinkle Stars")
            *poutEffect = new StarryNightEffect<QuietStar>("Green Twinkle Stars", GreenColors_p, STARRYNIGHT_PROBABILITY, 1, LINEARBLEND, 2.0, 0.0, STARRYNIGHT_MUSICFACTOR); // Green Twinkle

        if (*poutEffect != NULL)
            return true;
    }

    Println("Defaulting to creating a star from constructor");

    String starEffectName = doc->getMember("starEffect") | String("StarryNightEffect");
    String starTypeName = doc->getMember("starType") | String("BubblyStar");

    String paletteName = doc->getMember("palette");
    CRGBPalette256 *paletter;
    if (!this->GetPaletterFromString(paletteName, &paletter))
        return false;

    float probability = doc->getMember("probability") | 1.0;
    float starSize = doc->getMember("starSize") | 1.0;
    double maxSpeed = doc->getMember("maxSpeed") | 100.0;
    double blurFactor = doc->getMember("blurFactor") | 0.0;

    if (starEffectName == "StarryNightEffect")
    {
        if (starTypeName == "BubblyStar")
            *poutEffect = new StarryNightEffect<BubblyStar>("BubblyStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "FlashStar")
            *poutEffect = new StarryNightEffect<FlashStar>("FlashStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "ColorCycleStar")
            *poutEffect = new StarryNightEffect<ColorCycleStar>("ColorCycleStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "MultiColorStar")
            *poutEffect = new StarryNightEffect<MultiColorStar>("MultiColorStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "ChristmasLightStar")
            *poutEffect = new StarryNightEffect<ChristmasLightStar>("ChristmasLightStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "HotWhiteStar")
            *poutEffect = new StarryNightEffect<HotWhiteStar>("HotWhiteStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "RandomPaletteColorStar")
            *poutEffect = new StarryNightEffect<MultiColorStar>("RandomPaletteColorStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "LongLifeSparkleStar")
            *poutEffect = new StarryNightEffect<LongLifeSparkleStar>("LongLifeSparkleStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else if (starTypeName == "QuietStar")
            *poutEffect = new StarryNightEffect<QuietStar>("QuietStar StarryNightEffect", *paletter, probability, starSize, LINEARBLEND, maxSpeed, blurFactor);
        else
        {
            m_strLastError = "Star type not found";
            return false;
        }
    }
    else if (starEffectName == "BlurStarEffect")
    {
        if (starTypeName == "BubblyStar")
            *poutEffect = new BlurStarEffect<BubblyStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "FlashStar")
            *poutEffect = new BlurStarEffect<FlashStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "ColorCycleStar")
            *poutEffect = new BlurStarEffect<ColorCycleStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "MultiColorStar")
            *poutEffect = new BlurStarEffect<MultiColorStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "ChristmasLightStar")
            *poutEffect = new BlurStarEffect<ChristmasLightStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "HotWhiteStar")
            *poutEffect = new BlurStarEffect<HotWhiteStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "RandomPaletteColorStar")
            *poutEffect = new BlurStarEffect<MultiColorStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "LongLifeSparkleStar")
            *poutEffect = new BlurStarEffect<LongLifeSparkleStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else if (starTypeName == "QuietStar")
            *poutEffect = new BlurStarEffect<QuietStar>(*paletter, probability, starSize, LINEARBLEND, maxSpeed);
        else
        {
            m_strLastError = "Star type not found";
            return false;
        }
    }
    else
    {
        m_strLastError = String("Unknow starEffectName: ") + starEffectName;
        return false;
    }

    return true;
}

bool EffectsFactory::GetPaletterFromString(String name, CRGBPalette256 **pout)
{
    if (name == "RGB")
        *pout = &RGBColors_p;
    else if (name == "Blue")
        *pout = &BlueColors_p;
    else if (name == "Red")
        *pout = &RedColors_p;
    else if (name == "Green")
        *pout = &GreenColors_p;
    else if (name == "Magenta")
        *pout = &MagentaColors_p;
    else if (name == "spectrum")
        *pout = &spectrumBasicColors;
    else if (name == "BG")
        *pout = &BGColors_p;
    else if (name == "blueSweep")
        *pout = &blueSweep;
    else if (name == "BlueStripes")
        *pout = &BlueStripes;
    else if (name == "MagentaStripes")
        *pout = &MagentaStripes;
    else if (name == "rainbowPalette")
        *pout = &rainbowPalette;
    else
    {
        m_strLastError = String("Unknow paletter name: ") + name;
        return false;
    }

    return true;
}