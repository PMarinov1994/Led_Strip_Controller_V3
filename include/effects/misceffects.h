//+--------------------------------------------------------------------------
//
// File:        misceffects.h
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
//
// Description:
//
//    Draws bouncing balls using a kinematics formula
//
// History:     Apr-17-2019         Davepl      Adapted from NightDriver
//
//---------------------------------------------------------------------------

#pragma once

#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <math.h>
#define FASTLED_INTERNAL 1
#include "FastLED.h"
#include "colorutils.h"
#include "globals.h"
#include "ledstripeffect.h"

// SimpleRainbowTestEffect
//
// Fills the spokes with a rainbow palette, skipping dots as specified

class SimpleRainbowTestEffect : public LEDStripEffect
{
private:
	uint8_t _EveryNth;
	uint8_t _SpeedDivisor;

public:
	SimpleRainbowTestEffect(uint8_t speedDivisor = 8, uint8_t everyNthPixel = 12)
		: LEDStripEffect("Simple Rainbow"),
		  _EveryNth(everyNthPixel),
		  _SpeedDivisor(speedDivisor)
	{
	}

	virtual void Draw()
	{
		fillRainbowAllChannels(0, _cLEDs, beatsin16(4, 0, 256), 8, _EveryNth);
		delay(10);
	}

	virtual const char *FriendlyName() const
	{
		return "Sample Effect";
	}
};

// SimpleRainbowTestEffect
//
// Fills the spokes with a rainbow palette, skipping dots as specified

class RainbowTwinkleEffect : public LEDStripEffect
{
private:
	float _speedDivisor;
	int _deltaHue;

public:
	RainbowTwinkleEffect(float speedDivisor = 12.0f, int deltaHue = 14)
		: LEDStripEffect("RainowFill Rainbow"),
		  _speedDivisor(speedDivisor),
		  _deltaHue(deltaHue)
	{
	}

	virtual void Draw()
	{
		static float hue = 0.0f;
		static unsigned long lastms = millis();

		unsigned long msElapsed = millis() - lastms;
		lastms = millis();

		hue += (float)msElapsed / _speedDivisor;
		hue = fmod(hue, 256.0);
		fillRainbowAllChannels(0, _cLEDs, hue, _deltaHue);

		if (random(0, 1) == 0)
			setPixel(random(0, _cLEDs), CRGB::White);
		delay(10);
	}

	virtual const char *FriendlyName() const
	{
		return "RainbowTwinkle Effect";
	}
};

// RainbowFillEffect
//
// Fills the spokes with a rainbow palette

class RainbowFillEffect : public LEDStripEffect
{
private:
protected:
	float _speedDivisor;
	int _deltaHue;

public:
	RainbowFillEffect(float speedDivisor = 12.0f, int deltaHue = 14)
		: LEDStripEffect("RainowFill Rainbow"),
		  _speedDivisor(speedDivisor),
		  _deltaHue(deltaHue)
	{
	}

	virtual void Draw()
	{
		static float hue = 0.0f;
		static unsigned long lastms = millis();

		unsigned long msElapsed = millis() - lastms;
		lastms = millis();

		hue += (float)msElapsed / _speedDivisor;
		hue = fmod(hue, 256.0);
		fillRainbowAllChannels(0, _cLEDs, hue, _deltaHue);
		delay(10);
	}

	virtual const char *FriendlyName() const
	{
		return "RainbowFill Effect";
	}
};

// RainbowFillEffect
//
// Fills the spokes with a rainbow palette

class ColorFillEffect : public LEDStripEffect
{
private:
protected:
	int _everyNth;
	CRGB _color;

public:
	ColorFillEffect(CRGB color = CRGB(246, 200, 160), int everyNth = 10)
		: LEDStripEffect("Color Fill"),
		  _everyNth(everyNth),
		  _color(color)
	{
	}

	virtual void Draw()
	{
		fillSolidOnAllChannels(CRGB::Black);
		fillSolidOnAllChannels(_color, 0, NUM_LEDS, _everyNth);
	}

	virtual const char *FriendlyName() const
	{
		return "Color Fill Effect";
	}
};

class StatusEffect : public LEDStripEffect
{

public:
	enum ERROR
	{
		NONE,
		GENERAL,
		WIFI,
		MQTT
	};

protected:
	int _everyNth;
	CRGB _color;
	ERROR _error;

public:
	StatusEffect(CRGB color = CRGB(246, 200, 160), int everyNth = 3)
		: LEDStripEffect("Color Fill"),
		  _everyNth(everyNth),
		  _color(color),
		  _error(ERROR::NONE)
	{
	}

	void setColor(CRGB color)
	{
		_color = color;
	}

	void setError(ERROR err)
	{
		_error = err;
	}

	ERROR getError()
	{
		return _error;
	}

	virtual void Draw()
	{
		CRGB color = _color;

		if (_error == ERROR::GENERAL)
			color = CRGB::Red;
		else if (_error == ERROR::WIFI)
			color = CRGB::Orange;
		else if (_error == ERROR::MQTT)
			color = CRGB::Purple;
		else
			color = CRGB::Green;

		fillSolidOnAllChannels(CRGB::Black);
		fillSolidOnAllChannels(color, 0, 0, _everyNth);
	}

	virtual const char *FriendlyName() const
	{
		return "Status Fill Effect";
	}
};

#if CLASSIC_GE_C9
static const CRGB TwinkleColors[] =
	{
		CRGB(238, 51, 39),	// Red
		CRGB(0, 172, 87),	// Green
		CRGB(250, 164, 25), // Yellow
		CRGB(0, 131, 203)	// Blue
};
#else
static const CRGB TwinkleColors[] =
	{
		CRGB::Red,
		CRGB::Green,
		CRGB::Blue,
		CRGB::White};
#endif

class TwinkleEffect : public LEDStripEffect
{
protected:
	size_t _countToDraw;
	int _fadeFactor;
	int _updateSpeed;

public:
	TwinkleEffect(size_t countToDraw = NUM_LEDS / 2, uint8_t fadeFactor = 10, int updateSpeed = 10)
		: LEDStripEffect("Twinkle"),
		  _countToDraw(countToDraw),
		  _fadeFactor(fadeFactor),
		  _updateSpeed(updateSpeed)
	{
	}

	const int Count = 99;
	int buffer[99] = {0};

	std::deque<size_t> litPixels;

	virtual void Draw()
	{
		CRGB *pPixels = _GFX->GetLEDBuffer();
		EVERY_N_MILLISECONDS(_updateSpeed)
		{
			if (litPixels.size() > _countToDraw)
			{
				size_t i = litPixels.back();
				litPixels.pop_back();
				pPixels[i] = CRGB::Black;
			}

			// Pick a random pixel and put it in the TOP slot
			int iNew = -1;
			for (int iPass = 0; iPass < NUM_LEDS * 10; iPass++)
			{
				size_t i = random(0, NUM_LEDS);
				if (pPixels[i] != CRGB(0, 0, 0))
					continue;
				iNew = i;
				break;
			}
			if (iNew == -1) // No empty slot could be found!
			{
				litPixels.clear();
				setAllOnAllChannels(0, 0, 0);
				return;
			}

			assert(litPixels.end() == find(litPixels.begin(), litPixels.end(), iNew));
			pPixels[iNew] = TwinkleColors[random(0, ARRAYSIZE(TwinkleColors))];

			if (pPixels[iNew] == CRGB(0, 0, 0))
			{
			}

			litPixels.push_front(iNew);
		}

		EVERY_N_MILLISECONDS(20)
		{
			fadeToBlackBy(FastLED.leds(), NUM_LEDS, _fadeFactor);
		}
	}
};

// class PoliceEffect : public LEDStripEffect
// {
// 	typedef enum { INNERRED, OUTERRED, INNERBLUE, OUTERBLUE, MIXED, STROBE } lampStates;
// 	const lampStates highestState = STROBE;

// 	virtual void Draw()
// 	{
// 	}
// };

class RainbowComet : public LEDStripEffect
{
private:
	int m_iCometSize;
	CRGB *pLeds;

public:
	RainbowComet(int iCometSize = 5)
		: LEDStripEffect("RainbowComet"), m_iCometSize(iCometSize)
	{
	}

	virtual ~RainbowComet(){};

public:
	virtual bool Init(std::shared_ptr<LEDMatrixGFX> gfx)
	{
		if (!LEDStripEffect::Init(gfx))
			return false;

		pLeds = _GFX->GetLEDBuffer();

		return true;
	}

	virtual void Draw()
	{
		EVERY_N_MILLISECONDS(20)
		{
			fadeToBlackBy(pLeds, NUM_LEDS, 64);
			int iPos = beatsin16(32, 0, NUM_LEDS - m_iCometSize);

			uint8_t hue = beatsin8(60);

			for (int i = iPos; i < iPos + m_iCometSize; i++)
				setPixel(i, CHSV(hue, 255, 255));
		}
	}
};

class Marquee : public LEDStripEffect
{
public:
	Marquee(bool mirror = false)
		: LEDStripEffect("Marquee"), m_mirror(mirror), m_scroll(0.0f), m_poss(0){};

	~Marquee(){};

public:
	virtual void Draw()
	{
		m_scroll += 0.1f;
		if (m_scroll > 5.0)
			m_scroll -= 5.0;

		if (!m_mirror)
		{
			m_poss += 4;
			uint8_t k = m_poss;

			CRGB c;
			for (int i = 0; i < NUM_LEDS; i++)
				setPixel(i, c.setHue(k += 8));

			for (float i = m_scroll; i < NUM_LEDS; i += 5)
				setPixel(i, CRGB::Black);
		}
		else
		{
			m_poss += 4;
			uint8_t k = m_poss;

			// Roughly equivalent to fill_rainbow(g_LEDs, NUM_LEDS, j, 8);
			CRGB c;
			for (int i = 0; i < (NUM_LEDS + 1) / 2; i++)
			{
				setPixel(i, c.setHue(k));
				setPixel(NUM_LEDS - 1 - i, c.setHue(k));
				k += 8;
			}

			for (float i = m_scroll; i < NUM_LEDS / 2; i += 5)
			{
				setPixel(i, CRGB::Black);
				setPixel(NUM_LEDS - 1 - i, CRGB::Black);
			}
		}
	};

private:
	bool m_mirror;
	float m_scroll;
	uint8_t m_poss;
};

class BulgarianFlag : public LEDStripEffect
{

private:
	bool m_reverse;

public:
	BulgarianFlag(bool reverse = false) : LEDStripEffect("BulgarianFlag")
	{
		m_reverse = reverse;
	};

	virtual ~BulgarianFlag()
	{
	}

public:
	virtual bool Init(std::shared_ptr<LEDMatrixGFX> gfx)
	{
		if (!LEDStripEffect::Init(gfx))
			return false;

		CRGB *pLeds = gfx->GetLEDBuffer();

		int slice = NUM_LEDS / 3;
		for (int i = 0; i < slice; i++)
			pLeds[i] = !m_reverse ? CRGB::White : CRGB::Red;

		for (int i = 0; i < slice; i++)
			pLeds[i + slice] = CRGB::Green;

		for (int i = 0; i < slice; i++)
			pLeds[i + slice * 2] = !m_reverse ? CRGB::Red : CRGB::White;

		return true;
	};

	virtual void Draw(){};
};

class SolidFillEffect : public LEDStripEffect
{
protected:
	CRGB m_fillColor;

public:
	SolidFillEffect(uint8_t red, uint8_t green, uint8_t blue)
		: LEDStripEffect("SolidFillEffect")
	{
		m_fillColor = CRGB(red, green, blue);
	}

	virtual ~SolidFillEffect()
	{
	}

	virtual bool Init(std::shared_ptr<LEDMatrixGFX> gfx)
	{
		if (!LEDStripEffect::Init(gfx))
			return false;

		CRGB *pLeds = gfx->GetLEDBuffer();
		size_t count = gfx->GetLEDCount();

		for (size_t i = 0; i < count; i++)
			pLeds[i] = m_fillColor;

		return true;
	};

	virtual void Draw(){};
};