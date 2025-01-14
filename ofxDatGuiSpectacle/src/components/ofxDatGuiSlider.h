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
#include "ofxDatGuiTextInputField.h"

class ofxDatGuiSlider : public ofxDatGuiComponent {

    public:
    
        ofxDatGuiSlider(string label, float min, float max, float val) : ofxDatGuiComponent(label)
        {
            textInput=false;
            mTextInputX=0;
            mTextInputWidth=0;
            mMin = min;
            mMax = max;
            setPrecision(2);
            setName(label);
            mType = ofxDatGuiType::SLIDER;
            mTextInput = new ofxDatGuiTextInputField();
            mTextInput->setTextInputFieldType(ofxDatGuiInputType::ALPHA_NUMERIC);
            mTextInput->onInternalEvent(this, &ofxDatGuiSlider::onInputChanged);
            mInput = new ofxDatGuiTextInputField();
            mInput->setTextInputFieldType(ofxDatGuiInputType::NUMERIC);
            mInput->onInternalEvent(this, &ofxDatGuiSlider::onInputChanged);
            mInput->setText("0");
            setTheme(ofxDatGuiComponent::getTheme());
            setValue(val, false); // don't dispatch a change event when component is constructed //
//            ofxDatGuiComponent::setNumberbox(false);
            
        }
    ofxDatGuiSlider(string label, float min, float max, float val, bool t, bool n) : ofxDatGuiComponent(label) //add with editable text box
          {
              textInput=t;
              mTextInputX=0;
              mTextInputWidth=0;
              mNumberbox=n;
              mNumberboxX=0;
              mNumberboxWidth=0;
              setName(label);
              mMin = min;
              mMax = max;
              textInput=t;
              setPrecision(2);
              mType = ofxDatGuiType::SLIDER;
              mTextInput = new ofxDatGuiTextInputField();
              mTextInput->setTextInputFieldType(ofxDatGuiInputType::ALPHA_NUMERIC);
              mTextInput->onInternalEvent(this, &ofxDatGuiSlider::onInputChanged);
              mInput = new ofxDatGuiTextInputField();
              mInput->setTextInputFieldType(ofxDatGuiInputType::NUMERIC);
              mInput->onInternalEvent(this, &ofxDatGuiSlider::onInputChanged);
              mInput->setText("0");
              setTheme(ofxDatGuiComponent::getTheme());
              setValue(val, false); // don't dispatch a change event when component is constructed //
//              ofxDatGuiComponent::setNumberbox(false);
          }
    
        ofxDatGuiSlider(string label, float min, float max) : ofxDatGuiSlider(label, min, max, (max+min)/2) {}
        ofxDatGuiSlider(ofParameter<int> & p) : ofxDatGuiSlider(p.getName(), p.getMin(), p.getMax(), p.get()) {
            textInput=false;
            setName(p.getName());
            mParamI = &p;
            setPrecision(0);
            calculateScale();
            mParamI->addListener(this, &ofxDatGuiSlider::onParamI);
        }
    
        ofxDatGuiSlider(ofParameter<float> & p) : ofxDatGuiSlider(p.getName(), p.getMin(), p.getMax(), p.get()) {
            textInput=false;

            mParamF = &p;
            setPrecision(2);
            calculateScale();
            mParamF->addListener(this, &ofxDatGuiSlider::onParamF);
        }
    
        ~ofxDatGuiSlider()
        {
            delete mInput;
        }
    
        void setTheme(const ofxDatGuiTheme* theme)
        {
            setComponentStyle(theme);
            mSliderFill = theme->color.slider.fill;
            mBackgroundFill = theme->color.inputAreaBackground;
            mStyle.stripe.color = theme->stripe.slider;
            mInput->setTheme(theme);
            mInput->setTextInactiveColor(theme->color.slider.text);
            mTextInput->setTheme(theme);
            mTextInput->setTextInactiveColor(theme->color.slider.text);
            setWidth(theme->layout.width, theme->layout.labelWidth);
        }
    
        void setWidth(int width, float labelWidth)
        {
            ofxDatGuiComponent::setWidth(width, labelWidth);
//            float totalWidth = mStyle.width - mLabel.width;

//            if(mLockedLayout) mSliderX = (x + getWidth()) - mStyle.padding - mInputWidth - mStyle.padding - mSliderWidth;
//            cout<<"x: "<<x<<"; width: "<< getWidth() << "InputWidth: "<<mInputWidth << "sliderWidth: "<<mSliderWidth <<"\n";
            
            mTextInputX = mNumberboxWidth + mNumberboxX + (mStyle.padding * 2);
            mTextInputWidth = int(mStyle.width * .30);
            mTextInput->setWidth(mTextInputWidth);
            mTextInput->setPosition(x+mTextInputX, y + (mStyle.padding*2));
            mSliderWidth = int(mStyle.width * .35);
//            mSliderX = mTextInputWidth + mTextInputX + (mStyle.padding * 2);
            mInputWidth = int(mStyle.width * .15);
//            mInputX = mSliderX + mSliderWidth + (mStyle.padding * 2);
            mInputX = mStyle.width - mStyle.padding - mInputWidth;
            mSliderX = mInputX - mStyle.padding - mSliderWidth;
            mInput->setWidth(mInputWidth);
            mInput->setPosition(x + mInputX, y + mStyle.padding);


//                mTextInput->setPosition(mTextInputX, y + mStyle.padding);
                mTextInput->setBackgroundColor(ofColor::white);
                mTextInput->setTextInactiveColor(ofColor::black);
                mTextInput->setTextActiveColor(ofColor::white);
//            }
//            else
//            cout<<"SliderX: "<<mSliderX <<"\n";
        }
    
        void setPosition(int x, int y)
        {
            ofxDatGuiComponent::setPosition(x, y);
            mInput->setPosition(x + mInputX, y + mStyle.padding);
            mTextInput->setPosition(x + mTextInputX, y + mStyle.padding);
        }
    
        void setPrecision(int precision)
        {
            mPrecision = precision;
            if (mPrecision > MAX_PRECISION) mPrecision = MAX_PRECISION;
        }
    
        void setMin(float min)
        {
            mMin = min;
            if (mMin < mMax){
                calculateScale();
            }   else{
                onInvalidMinMaxValues();
            }
        }
    
        void setMax(float max)
        {
            mMax = max;
            if (mMax > mMin){
                calculateScale();
            }   else{
                onInvalidMinMaxValues();
            }
        }
    
        void setValue(float value, bool dispatchEvent = true)
        {
            value = round(value, mPrecision);
            if (value != mValue)
            {
                mValue = value;
                if (mValue > mMax)
                {
                    mValue = mMax;
                }
                else if (mValue < mMin)
                {
                    mValue = mMin;
                }
                calculateScale();
                if (dispatchEvent) dispatchSliderChangedEvent();
            }
        }
    
        float getValue()
        {
            return mValue;
        }
    
        void setScale(float scale)
        {
            mScale = scale;
            if (mScale < 0 || mScale > 1){
                ofLogError() << "row #" << mIndex << " : scale must be between 0 & 1" << " [setting to 50%]";
                mScale = 0.5f;
            }
            mValue = ((mMax-mMin) * mScale) + mMin;
        }
    
        float getScale()
        {
            return mScale;
        }


    /*
        variable binding methods
    */
    
        void bind(int &val)
        {
            mBoundi = &val;
            mBoundf = nullptr;
        }
    
        void bind(float &val)
        {
            mBoundf = &val;
            mBoundi = nullptr;
        }

        void bind(int &val, int min, int max)
        {
            mMin = min;
            mMax = max;
            mBoundi = &val;
            mBoundf = nullptr;
        }
    
        void bind(float &val, float min, float max)
        {
            mMin = min;
            mMax = max;
            mBoundf = &val;
            mBoundi = nullptr;
        }
    
        void update(bool acceptEvents = true)
        {
            ofxDatGuiComponent::update(acceptEvents);
        // check for variable bindings //
            if (mBoundf != nullptr && !mInput->hasFocus()) {
                setValue(*mBoundf);
            }   else if (mBoundi != nullptr && !mInput->hasFocus()){
                setValue(*mBoundi);
            }
            if(acceptEvents==false)
            {
                mTextInput->onFocusLost();
                mInput->onFocusLost();
            }
        }

        void draw()
        {
            if (!mVisible) return;
            ofPushStyle();
            ofxDatGuiComponent::draw();
        
            // slider bkgd //
                ofSetColor(mBackgroundFill);
                ofDrawRectangle(x+mSliderX, y+mStyle.padding, mSliderWidth, mStyle.height-(mStyle.padding*2));
            // slider fill //
                if (mScale > 0)
                {
//                    ofSetColor(mSliderFill);
//                    ofDrawRectangle(mSliderX, y+mStyle.padding, mSliderWidth*mScale, mStyle.height-(mStyle.padding*2))
                    gradient(ofPoint(x+mSliderX,y+mStyle.padding),
                             ofPoint(x+mSliderX,y+mStyle.padding+ mStyle.height-(mStyle.padding*2)),ofPoint(x+mSliderX+mSliderWidth*mScale,y+mStyle.padding),ofPoint(x+mSliderX+mSliderWidth*mScale,y+mStyle.padding+mStyle.height-(mStyle.padding*2)),
                             mSliderFill, ofColor::darkGray);
                }
            // numeric input field //
                mInput->draw();
                // textInput //
                        if(textInput)
                        {
                            mTextInput->draw();
                        }
            ofPopStyle();
        }
    
    bool hitTest(ofPoint m)
    {
        if (!mEnabled || !mVisible){
            return false;
        }
        else if (m.x>=x+mSliderX && m.x<= (x+mSliderX+mSliderWidth) && m.y>=y+mStyle.padding && m.y<= y+mStyle.height-mStyle.padding){
            return true;
        }
        else if (mInput->hitTest(m)){
            return true;
        }
        else if (mTextInput->hitTest(m)){
            return true;
        }
        else {
            return false;
        }
    }
    
        void dispatchEvent()
        {
            if (sliderEventCallback != nullptr) {
                ofxDatGuiSliderEvent e(this, mValue, mScale);
                sliderEventCallback(e);
            }   else{
                ofxDatGuiLog::write("ofxDatGuiSlider", ofxDatGuiMsg::EVENT_HANDLER_NULL);
            }
        }
    
        static ofxDatGuiSlider* getInstance() { return new ofxDatGuiSlider("X", 0, 100); }
        
        int getPrecision()
        {
            return mPrecision;
        }
    
    void setLockedLayout(bool t)
    {
        mLockedLayout = t;
    }
    
    bool getLockedLayout()
    {
        return mLockedLayout;
    }
    
    void setTextInput(string s)
    {
        mTextInput->setText(s);
    }
    
    string getTextInput()
    {
        return mTextInput->getText();
    }
    
    protected:
    
        void onMousePress(ofPoint m)
        {
            ofxDatGuiComponent::onMousePress(m);
            if (mInput->hitTest(m)){
                mInput->onFocus();
            }   else if (mInput->hasFocus()){
                mInput->onFocusLost();
            }
             if (mTextInput->hitTest(m)){
                mTextInput->onFocus();
            }  else if (mTextInput->hasFocus()){
                mTextInput->onFocusLost();
            }
        }
    
        void onMouseDrag(ofPoint m)
        {
            if (mFocused && mInput->hasFocus() == false && mTextInput->hasFocus() == false)
            {
                float s = (m.x-(mSliderX+x))/mSliderWidth;
//                cout<<"mLabel.width: " << mLabel.width << " mSliderWidth: "<<mSliderWidth <<"s: "<<s<<"\n";
                if (s > .999) s = 1;
                if (s < .001) s = 0;
        // don't dispatch an event if scale hasn't changed //
                if (s == mScale) return;
                mScale = s;
                setValue(((mMax-mMin) * mScale) + mMin);
            }
        }
    
        void onMouseRelease(ofPoint m)
        {
            ofxDatGuiComponent::onMouseRelease(m);
//            if (mInput->hitTest(m) == false) mInput->onFocusLost();
//            if (mTextInput->hitTest(m) == false) mTextInput->onFocusLost();
//            if (mInput->hitTest(m) == false && mTextInput->hitTest(m) == false) ofxDatGuiComponent::onFocusLost();
        }
    
        void onFocusLost()
        {
            ofxDatGuiComponent::onFocusLost();
            if (mInput->hasFocus()) mInput->onFocusLost();
            if (mTextInput->hasFocus()) mTextInput->onFocusLost();
        }
    
        void onKeyPressed(int key)
        {
            if (mInput->hasFocus()) mInput->onKeyPressed(key);
            else if (mTextInput->hasFocus()) mTextInput->onKeyPressed(key);
        }
    
        void onInputChanged(ofxDatGuiInternalEvent e)
        {
            setValue(ofToFloat(mInput->getText()));
        }
    
        void dispatchSliderChangedEvent()
        {
        // update any bound variables //
            if (mBoundf != nullptr) {
                *mBoundf = mValue;
            }   else if (mBoundi != nullptr) {
                *mBoundi = mValue;
            }   else if (mParamI != nullptr) {
                mParamI->set(mValue);
            }   else if (mParamF != nullptr) {
                mParamF->set(mValue);
            }
        // dispatch event out to main application //
            dispatchEvent();
        }
    
    bool textInput;
    ofxDatGuiTextInputField* mTextInput;
    int mTextInputX;
    int mTextInputWidth;
    int mSliderX;
    
    bool mLockedLayout;

    private:
    
        float   mMin;
        float   mMax;
        float      mValue;
        float      mScale;
        int     mPrecision;
        int     mInputX;
        int     mInputWidth;
        int     mSliderWidth;
        ofColor mSliderFill;
        ofColor mBackgroundFill;
        ofxDatGuiTextInputField* mInput;
    
        static const int MAX_PRECISION = 4;
    
        int*    mBoundi = nullptr;
        float*  mBoundf = nullptr;
        ofParameter<int>* mParamI = nullptr;
        ofParameter<float>* mParamF = nullptr;
        void onParamI(int& n) { setValue(n); }
        void onParamF(float& n) { setValue(n); }
    
        void calculateScale()
        {
            mScale = ofxDatGuiScale(mValue, mMin, mMax);
            setSliderTextInput();
        }
    
        void setSliderTextInput()
        {
            string v = ofToString(round(mValue, mPrecision));
            if (mValue != mMin && mValue != mMax){
                int p = v.find('.');
                if (p == -1 && mPrecision != 0){
                    v+='.';
                    p = v.find('.');
                }
                while(v.length() - p < (mPrecision + 1)) v+='0';
            }
            mInput->setText(v);
        }
    
        float round(float num, int precision)
        {
            return roundf(num * pow(10, precision)) / pow(10, precision);
        }
    
        void onInvalidMinMaxValues()
        {
            ofLogError() << "row #" << mIndex << " : invalid min & max values" << " [setting to 50%]";
            mMin = 0;
            mMax = 100;
            mScale = 0.5f;
            mValue = (mMax+mMin) * mScale;
        }
        
};

