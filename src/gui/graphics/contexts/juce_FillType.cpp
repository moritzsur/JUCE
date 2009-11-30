/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-9 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "../../../core/juce_StandardHeader.h"

BEGIN_JUCE_NAMESPACE

#include "juce_FillType.h"


//==============================================================================
FillType::FillType() throw()
    : colour (0xff000000), gradient (0), image (0)
{
}

FillType::FillType (const Colour& colour_) throw()
    : colour (colour_), gradient (0), image (0)
{
}

FillType::FillType (const ColourGradient& gradient) throw()
    : colour (0xff000000), gradient (new ColourGradient (gradient)), image (0)
{
}

FillType::FillType (const Image& image_, const AffineTransform& transform_) throw()
    : colour (0xff000000), gradient (0),
      image (&image_), transform (transform_)
{
}

FillType::FillType (const FillType& other) throw()
    : colour (other.colour),
      gradient (other.gradient != 0 ? new ColourGradient (*other.gradient) : 0),
      image (other.image), transform (other.transform)
{
}

const FillType& FillType::operator= (const FillType& other) throw()
{
    if (this != &other)
    {
        colour = other.colour;
        delete gradient;
        gradient = (other.gradient != 0 ? new ColourGradient (*other.gradient) : 0);
        image = other.image;
        transform = other.transform;
    }

    return *this;
}

FillType::~FillType() throw()
{
    delete gradient;
}

void FillType::setColour (const Colour& newColour) throw()
{
    deleteAndZero (gradient);
    image = 0;
    colour = newColour;
}

void FillType::setGradient (const ColourGradient& newGradient) throw()
{
    if (gradient != 0)
    {
        *gradient = newGradient;
    }
    else
    {
        image = 0;
        gradient = new ColourGradient (newGradient);
    }
}

void FillType::setTiledImage (const Image& image_, const AffineTransform& transform_) throw()
{
    deleteAndZero (gradient);
    image = &image_;
    transform = transform_;
}


END_JUCE_NAMESPACE