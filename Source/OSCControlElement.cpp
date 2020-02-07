/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.4.6

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2017 - ROLI Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "OSCControlElement.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
OSCControlElement::OSCControlElement ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    knob.reset (new Slider ("knob"));
    addAndMakeVisible (knob.get());
    knob->setRange (0, 10, 0);
    knob->setSliderStyle (Slider::RotaryVerticalDrag);
    knob->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    knob->addListener (this);

    knob->setBounds (-6, -6, 44, 48);

    textEditor.reset (new TextEditor ("textEditor"));
    addAndMakeVisible (textEditor.get());
    textEditor->setMultiLine (false);
    textEditor->setReturnKeyStartsNewLine (false);
    textEditor->setReadOnly (false);
    textEditor->setScrollbarsShown (false);
    textEditor->setCaretVisible (true);
    textEditor->setPopupMenuEnabled (false);
    textEditor->setText (String());

    textButton.reset (new TextButton ("textButton"));
    addAndMakeVisible (textButton.get());
    textButton->setButtonText (TRANS("m"));
    textButton->addListener (this);
    textButton->setColour (TextButton::buttonOnColourId, Colours::crimson);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    textButton->setClickingTogglesState(true);

    //[/Constructor]
}

OSCControlElement::~OSCControlElement()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    knob = nullptr;
    textEditor = nullptr;
    textButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void OSCControlElement::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void OSCControlElement::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    textEditor->setBounds (32, 6, getWidth() - 64, 24);
    textButton->setBounds (32 + (getWidth() - 64) - -4, 6, 24, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void OSCControlElement::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == knob.get())
    {
        //[UserSliderCode_knob] -- add your slider handling code here..
		send();
        //[/UserSliderCode_knob]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void OSCControlElement::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == textButton.get())
    {
        //[UserButtonCode_textButton] -- add your button handler code here..
        //[/UserButtonCode_textButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void OSCControlElement::setOSCSender(OSCSender * sender) {
	oscSender = sender;
}

void OSCControlElement::send() {
	if (!textButton->getToggleState()) {
		auto value = knob->getValue();
		auto message = textEditor->getText();

		auto oscMessage = OSCMessage(message, float(value));
		DBG(String("sending oscmessage"));
		oscSender->send(oscMessage);
	}
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="OSCControlElement" componentName=""
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <SLIDER name="knob" id="95542922d5be1f90" memberName="knob" virtualName=""
          explicitFocusOrder="0" pos="-6 -6 44 48" min="0.0" max="10.0"
          int="0.0" style="RotaryVerticalDrag" textBoxPos="NoTextBox" textBoxEditable="1"
          textBoxWidth="80" textBoxHeight="20" skewFactor="1.0" needsCallback="1"/>
  <TEXTEDITOR name="textEditor" id="31e4208a2191fc1a" memberName="textEditor"
              virtualName="" explicitFocusOrder="0" pos="32 6 64M 24" initialText=""
              multiline="0" retKeyStartsLine="0" readonly="0" scrollbars="0"
              caret="1" popupmenu="0"/>
  <TEXTBUTTON name="textButton" id="bfa68f48e96f5be0" memberName="textButton"
              virtualName="" explicitFocusOrder="0" pos="-4R 6 24 24" posRelativeX="31e4208a2191fc1a"
              bgColOn="ffdc143c" buttonText="m" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

