/*
  ==============================================================================

    ControlElementKnob.cpp
    Created: 8 Feb 2020 3:30:01pm
    Author:  bzk

  ==============================================================================
*/

#include "ControlElementKnob.h"

ControlElementKnob::
ControlElementKnob
(OSCSender & sender) :
    ControlElement(sender)
{
}

int
ControlElementKnob::
getNumberOfRows() const
{
    return 1;
}

void
ControlElementKnob::
send()
{
}
