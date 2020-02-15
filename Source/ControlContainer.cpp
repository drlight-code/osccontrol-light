/*
  ==============================================================================

    ControlContainer.cpp
    Created: 8 Feb 2020 9:48:27pm
    Author:  bzk

  ==============================================================================
*/

#include "LayoutHints.h"
#include "ControlElement.h"

#include "ControlContainer.h"

void
ControlContainer::
resized()
{
    auto area = getLocalBounds();

    for (auto & element : listControlElements) {
        element->setBounds
            (area.removeFromTop(LayoutHints::heightRow) *
                element->getNumberOfRows());
    }
}

std::list<ControlElementUniq> &
ControlContainer::
getElementList()
{
    return listControlElements;
}

OSCSender &
ControlContainer::
getOSCSender()
{
    return oscSender;
}
