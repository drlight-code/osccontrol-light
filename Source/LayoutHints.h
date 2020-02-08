/*
  ==============================================================================

    LayoutHints.h
    Created: 8 Feb 2020 3:56:04pm
    Author:  bzk

  ==============================================================================
*/

#pragma once

struct LayoutHints
{
public:
    static const int heightRow = 32;
    static const int heightTextBox = 24;
    static const int widthTextPort = 48;
    static const int gapSize = 4;

    static int getTextBoxInset();
};
