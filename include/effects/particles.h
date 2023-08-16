//+--------------------------------------------------------------------------
//
// File:        Particles.h
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
//   Classes for moving and fading little render objects over time, 
//   used as a base for the star and insulator effects
//
// History:     Jul-7-2021         Davepl      Commented
//
//---------------------------------------------------------------------------

#pragma once

#include <sys/types.h>
#include <errno.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <deque>
#include "colorutils.h"
#include "globals.h"
#include "ledstripeffect.h"

extern AppTime g_AppTime;

// Lifespan
// 
// Base class that knows only when it was created, so it can later tell you old it is, which is
// used by the fading classes 

class Lifespan
{
  protected:

    double                       _birthTime;

  public:

    Lifespan() :_birthTime(g_AppTime.FrameStartTime())
    {
    }

    virtual ~Lifespan()
    {}    

    double Age() const
    {
        return g_AppTime.FrameStartTime() - _birthTime;
    }    

    virtual double TotalLifetime() const = 0;
};

// MovingObject
//
// Keeps track of a position, velocity, and maximum speed, mo
// and manage it's velocity (postion drift).  Updates its position
// automatically based on speed if called every frame.  

class MovingObject 
{
protected:

    double                       _velocity;
    double                       _maxSpeed;

public:

    double                       _iPos;

    MovingObject(double maxSpeed = 0.25) : _maxSpeed(maxSpeed)
    {
        _velocity = randomDouble(0, _maxSpeed * 2) - _maxSpeed;
    }

    virtual ~MovingObject()
    {}

    virtual void UpdatePosition()
    {
        _iPos += _velocity * g_AppTime.DeltaTime();
    }
};

// FadingObject
// 
// Based on its age and provided information about how long each life stage lasts, provides a
// 'FadeoutAmount' function that indicates how much the particle should be dimmed from aging

class FadingObject : public Lifespan
{
  protected:

    virtual float PreignitionTime() const         { return 0.0f;  }
    virtual float IgnitionTime()    const         { return 0.5f;  }
    virtual float HoldTime()        const         { return 1.00f; }
    virtual float FadeTime()        const         { return 1.5f;  }

  public:

    virtual double TotalLifetime() const
    {
        return PreignitionTime() + IgnitionTime() + HoldTime() + FadeTime();
    }

    virtual float FadeoutAmount() const
    {
        float age = Age();
        if (age < 0)
            age = 0;
        
        if (age < PreignitionTime() && PreignitionTime() != 0.0f)
            return 1.0 - (age / PreignitionTime());
        age -= PreignitionTime();
        if (age < IgnitionTime() && IgnitionTime() != 0.0f)
            return (age / IgnitionTime());
        age -= IgnitionTime();
        if (age < HoldTime())
            return 0.0f;                                                // Just born
        if (age > HoldTime() + FadeTime())
            return 1.0f;                                                // Black hole, all faded out
        age -= (HoldTime() + IgnitionTime());
            return (age / FadeTime());                                  // Fading star
    } 
};

// FadingColoredObject
//
// Object that flashes white during ignition and fades color throughout the the rest of its lifetime

class FadingColoredObject : public FadingObject
{
  protected:

    CRGB                         _baseColor;
  
  public:

    FadingColoredObject(CRGB baseColor)
      : _baseColor(baseColor)
    {
    }

    virtual CRGB ObjectColor() const
    {
        if (Age() >= PreignitionTime() && Age() < IgnitionTime() + PreignitionTime())
        {
            CRGB c = CRGB::White;
            c.fadeToBlackBy(255 - ((Age() - PreignitionTime()) / IgnitionTime() * 255));
            return c + _baseColor;
        }

        CRGB c = _baseColor;
        fadeToBlackBy(&c, 1, 255 * FadeoutAmount());
        return c;        
    }    
};

// FadingPaletteObject
//
// Uses a palette plus an index to generate its ObjectColor, fades over its lifetime, flashes white at ignition

class FadingPaletteObject : public FadingObject
{
  protected:

    const CRGBPalette256 & _palette;
    const TBlendType       _blendType = NOBLEND;
    uint8_t                _colorIndex;

  public:

    FadingPaletteObject(const CRGBPalette256 & palette, TBlendType blendType = NOBLEND, uint8_t colorIndex =  0)
      : _palette(palette),
        _blendType(blendType),
        _colorIndex(colorIndex)
    {
    }

    virtual CRGB ObjectColor() const
    {
        CRGB c = ColorFromPalette(_palette, _colorIndex, 255, _blendType);
        if (Age() < PreignitionTime())
        {
            fadeToBlackBy(&c, 1, 255 * FadeoutAmount());
            return c;
        }
        else if (Age() >= PreignitionTime() && (Age() < (IgnitionTime() + PreignitionTime())))
        {
            CRGB c = CRGB::Green;
            fadeToBlackBy(&c, 1, 255 * FadeoutAmount());
            return c;
        }
        
        fadeToBlackBy(&c, 1, 255 * FadeoutAmount());
        return c;        
    }    

    virtual void SetColorIndex(uint8_t index)
    {
        _colorIndex = index;
    }

    virtual uint8_t GetColorIndex() const
    {
        return _colorIndex;
    }
};


// MovingFadingColoredObject
//
// Moves, fades over it's lifetime, based on palette+index color

class MovingFadingPaletteObject: public FadingPaletteObject, public MovingObject
{
  public:

    MovingFadingPaletteObject(const CRGBPalette256 & palette, TBlendType blendType = NOBLEND, double maxSpeed = 1.0, uint8_t colorIndex = 0)
      : FadingPaletteObject(palette, blendType, colorIndex), 
        MovingObject(maxSpeed)
    {
    }
};

// MovingFadingColoredObject
//
// Moves, fades over it's lifetime, based on CRGB color

class MovingFadingColoredObject: public FadingColoredObject, public MovingObject
{
  public:

    MovingFadingColoredObject(CRGB baseColor, double maxSpeed = 1.0)
      : FadingColoredObject(baseColor),
        MovingObject(maxSpeed)
    {
    }
};

// ObjectSize
//
// Super-simple class which holds a size

class ObjectSize 
{
    public:
      double _objectSize;

      ObjectSize(double size = 1.0)
        :_objectSize(size)
      {
      }
};

class DrawableParticle : public Lifespan
{
  protected:

  public:
     virtual void Render() = 0;
};

template <typename Type = DrawableParticle> class ParticleSystemEffect : public virtual LEDStripEffect
{
  protected:

    std::deque<Type> _allParticles;

    // Once per frame we are called to update all particles, which includes aging out old ones

  public:
    
    ParticleSystemEffect<Type>(const char * pszName) : LEDStripEffect(pszName)
    {
    }

    virtual void Draw()
    {
        while (_allParticles.size() > 0 && _allParticles.front().Age() >= _allParticles.front().TotalLifetime())
            _allParticles.pop_front();

        while (_allParticles.size() > _cLEDs)
            _allParticles.pop_front();

        for(auto i = _allParticles.begin(); i != _allParticles.end(); i++)
            i->Render();            
    }
};