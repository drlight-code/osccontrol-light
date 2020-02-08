/*
  ==============================================================================

    ControlElement.h
    Created: 8 Feb 2020 3:28:52pm
    Author:  bzk

  ==============================================================================
*/

#pragma once

#include <memory>

#include <JuceHeader.h>

class ControlElement :
    public Component,
    public Button::Listener
{
public:
    struct CreateInfo {
        std::pair<float, float> range;
        float value;

        std::string name;
        std::string message;
        std::string messageMute;

        bool showNames;
    };

    ControlElement
    (OSCSender & oscSender);

    void resized() override;
    void buttonClicked(Button* button) override;

    virtual int getNumberOfRows() const = 0;
    virtual void send() = 0;

protected:
    OSCSender & oscSender;

    std::unique_ptr<TextButton> buttonMute;
    std::string messageMute;
};
