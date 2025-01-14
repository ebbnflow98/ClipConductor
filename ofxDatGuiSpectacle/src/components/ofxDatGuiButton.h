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
#include "ofxDatGuiComponent.h"

class ofxDatGuiButton : public ofxDatGuiComponent {

    public:
    
        ofxDatGuiButton(string label) : ofxDatGuiComponent(label)
        {
            mType = ofxDatGuiType::BUTTON;
            setTheme(ofxDatGuiComponent::getTheme());
            mNumberbox=false;
        }
    ofxDatGuiButton(string label, bool numberbox) : ofxDatGuiComponent(label)
    {
        mType = ofxDatGuiType::BUTTON;
        setTheme(ofxDatGuiComponent::getTheme());
        mNumberbox=true;
    }
    
        void setTheme(const ofxDatGuiTheme* theme)
        {
            setComponentStyle(theme);
            mStyle.stripe.color = theme->stripe.button;
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            ofxDatGuiComponent::setWidth(width, labelWidth);
            mLabel.width = mStyle.width;
            if(mNumberbox) mLabel.width -= mStyle.height * 1.1;
            mLabel.rightAlignedXpos = mLabel.width - mLabel.margin;
            ofxDatGuiComponent::positionLabel();
        }
    
        void draw()
        {
            if (mVisible)
            {
            // anything that extends ofxDatGuiButton has the same rollover effect //
                ofPushStyle();
                
                if (mStyle.border.visible) drawBorder();
                ofFill();
                if (mFocused && mMouseDown) ofSetColor(mStyle.color.onMouseDown, mStyle.opacity);
                else if (mMouseOver && !ofGetMousePressed()) ofSetColor(mStyle.color.onMouseOver, mStyle.opacity); //not super clean...trying to keep options from reacting when mouse is down on slider.
                else ofSetColor(mStyle.color.background, mStyle.opacity);
                ofDrawRectangle(x, y, mStyle.width, mStyle.height);
                drawLabel();
                if (mNumberbox) drawNumberbox(mNumberboxValue);
                if(!mDropdownDividers) drawDropdownDividers();
                if (mStyle.stripe.visible) drawStripe();
                drawCustomIcon();
                
                ofPopStyle();
            }
        }
    
    void drawTrue()
    {
        if (mVisible)
        {
            ofPushStyle();
            if (mStyle.border.visible) drawBorder();
            ofFill();
            if (mFocused && mMouseDown) ofSetColor(mStyle.color.onMouseDown, mStyle.opacity);
            else if (mMouseOver) ofSetColor(mStyle.color.onMouseOver, mStyle.opacity);
            else ofSetColor(mStyle.color.onMouseDown, mStyle.opacity);
            
            ofDrawRectangle(x, y, mStyle.width, mStyle.height);
            drawLabel();
            if (mStyle.stripe.visible) drawStripe();
            ofPopStyle();
        }
    }

        void dispatchEvent()
        {
            if (buttonEventCallback != nullptr) {
                ofxDatGuiButtonEvent e(this);
                buttonEventCallback(e);
            }   else{
                ofxDatGuiLog::write(ofxDatGuiMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxDatGuiButton* getInstance() { return new ofxDatGuiButton("X"); }
    
    protected:
    
        void onMouseRelease(ofPoint m)
        {
            ofxDatGuiComponent::onFocusLost();
            ofxDatGuiComponent::onMouseRelease(m);
            dispatchEvent();
        }
private:
    shared_ptr<ofImage> eye;
    shared_ptr<ofImage> lightbulb;
    shared_ptr<ofImage> film;
    shared_ptr<ofImage> folder;
    shared_ptr<ofImage> trashcan;
    shared_ptr<ofImage> floppy;
    shared_ptr<ofImage> page;
    shared_ptr<ofImage> refresh;
};

class ofxDatGuiToggle : public ofxDatGuiButton {
    
    public:
    
        ofxDatGuiToggle(string label, bool checked = false) : ofxDatGuiButton(label)
        {
            mChecked = checked;
            mType = ofxDatGuiType::TOGGLE;
            setTheme(ofxDatGuiComponent::getTheme());
        }
    
        void setTheme(const ofxDatGuiTheme* theme)
        {
            setComponentStyle(theme);
            radioOn = theme->icon.radioOn;
            radioOff = theme->icon.radioOff;
            mStyle.stripe.color = theme->stripe.toggle;
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth = 1)
        {
            ofxDatGuiComponent::setWidth(width, labelWidth);
            mLabel.width = mStyle.width;
            mLabel.rightAlignedXpos = mIcon.x - mLabel.margin;
            ofxDatGuiComponent::positionLabel();
        }
    
        void toggle()
        {
            mChecked = !mChecked;
        }
    
        void setChecked(bool check)
        {
            mChecked = check;
        }
    
        bool getChecked()
        {
            return mChecked;
        }

        void draw()
        {
            if (mVisible)
            {
                ofPushStyle();
                ofSetColor(mIcon.color);
                if (mChecked == true) ofxDatGuiButton::drawTrue();
                
                
//                    radioOn->draw(x+mIcon.x, y+mIcon.y, mIcon.size, mIcon.size);
//                else ofSetColor()
//                    radioOff->draw(x+mIcon.x, y+mIcon.y, mIcon.size, mIcon.size);
                else ofxDatGuiButton::draw();

                ofPopStyle();
            }
        }
    
        void dispatchEvent()
        {
            if (toggleEventCallback == nullptr) {
        // attempt to call generic button callback //
                ofxDatGuiButton::dispatchEvent();
            }   else {
                toggleEventCallback(ofxDatGuiToggleEvent(this, mChecked));
            }
        }
    
        static ofxDatGuiToggle* getInstance() { return new ofxDatGuiToggle("X"); }
    
    protected:
    
        void onMouseRelease(ofPoint m)
        {
            mChecked = !mChecked;
            ofxDatGuiComponent::onFocusLost();
            ofxDatGuiComponent::onMouseRelease(m);
            dispatchEvent();
        }
    
    private:
        bool mChecked;
        shared_ptr<ofImage> radioOn;
        shared_ptr<ofImage> radioOff;
};



