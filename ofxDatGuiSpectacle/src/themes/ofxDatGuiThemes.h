/*
    Copyright (C) 2015 Stephen Braitsch [http://braitsch.io]

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#pragma once
#include "ofxDatGuiTheme.h"

/*
    Stock Themes
*/
class ofxDatGuiThemeSpectacle : public ofxDatGuiTheme{
    
public:
    ofColor palatte[5]={hex(0x000000),hex(0xffffff),hex(0x26ec33),hex(0xbbbbbb),hex(0x000000)};
    
    ofxDatGuiThemeSpectacle()
    {
        stripe.visible = false;
        color.label = palatte[0];
        color.guiBackground = palatte[0];
        color.background = palatte[1];
        color.slider.fill = palatte[0];
        color.slider.text = palatte[1];
        color.inputAreaBackground = palatte[4];
        color.textInput.text = palatte[0];
        color.textInput.highlight = palatte[2];
        color.textInput.backgroundOnActive = palatte[3];
        color.backgroundOnMouseOver = palatte[3];
        color.backgroundOnMouseDown = palatte[2];
        color.matrix.hover.button = palatte[3];
        color.matrix.selected.button = palatte[2];
        init();
    }
};

class ofxDatGuiThemeEvan : public ofxDatGuiTheme{
    
public:
    ofColor palatte[5]={hex(0xfffbfb),hex(0x322b3c),hex(0x522a87),hex(0xe3761c),hex(0x2a114b)};
    
    ofxDatGuiThemeEvan()
    {
        stripe.visible = false;
        color.label = palatte[0];
        color.guiBackground = palatte[1];
        color.background = palatte[2];
        color.slider.fill = palatte[3];
        color.slider.text = palatte[0];
        color.inputAreaBackground = palatte[4];
        color.textInput.text = palatte[0];
        color.textInput.highlight = palatte[4];
        color.textInput.backgroundOnActive = palatte[1];
        color.backgroundOnMouseOver = palatte[4];
        color.backgroundOnMouseDown = palatte[1];
        color.matrix.hover.button = palatte[3];
        color.matrix.selected.button = palatte[1];
        init();
    }
};

class ofxDatGuiThemeEvanFlip : public ofxDatGuiTheme{
    
public:
    ofColor palatte[5]={hex(0xfffbfb),hex(0x522a87),hex(0x322b3c),hex(0xe3761c),hex(0x2a114b)};
    
    ofxDatGuiThemeEvanFlip()
    {
        stripe.visible = false;
        color.label = palatte[0];
        color.guiBackground = palatte[1];
        color.background = palatte[2];
        color.slider.fill = palatte[3];
        color.slider.text = palatte[0];
        color.inputAreaBackground = palatte[4];
        color.textInput.text = palatte[0];
        color.textInput.highlight = palatte[4];
        color.textInput.backgroundOnActive = palatte[1];
        color.backgroundOnMouseOver = palatte[4];
        color.backgroundOnMouseDown = palatte[1];
        color.matrix.hover.button = palatte[3];
        color.matrix.selected.button = palatte[1];
        init();
    }
};



