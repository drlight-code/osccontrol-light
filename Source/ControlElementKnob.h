/*
  ==============================================================================

    ControlElementKnob.h
    Created: 8 Feb 2020 3:30:01pm
    Author:  bzk

  ==============================================================================
*/

#pragma once

#include <utility>

#include "ControlElement.h"
#include "ControlElementKnob.h"

class ControlElementKnob :
    public ControlElement,
    public Slider::Listener
{
public:
    struct CreateInfo {
        std::pair<float, float> range;
        float value;

        std::string message;
    };

    ControlElementKnob (const CreateInfo & info,
                        OSCSender & oscSender);

    int getNumberOfRows() const override;
    void sliderValueChanged (Slider* slider) override;
    void send() override;

    void resized() override;

private:
    std::string message;
    std::unique_ptr<Slider> knob;
    std::unique_ptr<TextEditor> textEditor;
};
