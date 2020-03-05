/*

  osccontrol-light - An audio plugin that speaks OSC.
  Copyright (C) 2020 Patric Schmitz

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <JuceHeader.h>

#include "UIComponentFactory.h"

std::unique_ptr<TextEditor>
UIComponentFactory::
createControlTextEditor ()
{
    auto product = std::make_unique<TextEditor> ("textEditor");

    product->setMultiLine (false);
    product->setReturnKeyStartsNewLine (false);
    product->setScrollbarsShown (false);
    product->setCaretVisible (true);
    product->setPopupMenuEnabled (false);
    product->setReadOnly (true);

    auto backgroundColour =
        product->findColour (TextEditor::backgroundColourId);
    product->setColour
        (TextEditor::outlineColourId,
            backgroundColour);

    return product;
}

std::unique_ptr<TextButton>
UIComponentFactory::
createControlButton ()
{
    auto product = std::make_unique<TextButton> ("textButton");

    product->setButtonText ("");
    product->setClickingTogglesState (true);
    product->setPaintingIsUnclipped (true);

    auto activeColor =
        LookAndFeel::getDefaultLookAndFeel ().findColour
        (Slider::ColourIds::thumbColourId);
    product->setColour (TextButton::buttonOnColourId, activeColor);

    return product;
}
