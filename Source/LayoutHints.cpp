/*
  ==============================================================================

    LayoutHints.cpp
    Created: 8 Feb 2020 3:56:04pm
    Author:  bzk

  ==============================================================================
*/

#include "LayoutHints.h"

int
LayoutHints::
getTextBoxInset()
{
    return (heightRow - heightTextBox) / 2;
}
