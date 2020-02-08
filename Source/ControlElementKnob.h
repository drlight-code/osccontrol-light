/*
  ==============================================================================

    ControlElementKnob.h
    Created: 8 Feb 2020 3:30:01pm
    Author:  bzk

  ==============================================================================
*/

#pragma once

#include "ControlElement.h"
#include "ControlElementKnob.h"

class ControlElementKnob :
    public ControlElement
{
public:
    ControlElementKnob(OSCSender & sender);

    int getNumberOfRows() const override;
    void send() override;
};
