////
////  evanColor.h
////  MIDI Hap Player v3
////
////  Created by Evan Douglas Peters on 3/22/20.
////
//
//#ifndef modalDialogs_h
//#define modalDialogs_h
//
//#include "ofxModal.h"
//#include "ofxDatGui.h"
//
//class addNewLight : public ofxModalWindow
//{
//    
//public:
//    ofxDatGuiTextInput* lightName;
//    ofxDatGuiSlider* lightNumberOfChannels;
//    
//    int oldChannels=3;
//    
//    addNewLight()
//    {
//        lightName = addComponent(new ofxDatGuiTextInput("Light Name","name"));
//        lightNumberOfChannels = addComponent(new ofxDatGuiSlider("Number of Channels",3,16,3));
//        lightNumberOfChannels->setPrecision(0);
//        lightNumberOfChannels->onSliderEvent(this,&addNewLight::onSliderEvent);
//        addComponent(new ofxDatGuiTextInput("Channel 1", "Parameter 1"));
//        addComponent(new ofxDatGuiTextInput("Channel 2", "Parameter 2"));
//        addComponent(new ofxDatGuiTextInput("Channel 3", "Parameter 3"));
//        
//    }
//    
//    void onSliderEvent(ofxDatGuiSliderEvent e)
//    {
//        int difference=e.target->getValue()-oldChannels;
//        
////        if(difference>0)
////        {
////            for(int i=0;i<difference;i++) addComponent(new ofxDatGuiTextInput("Channel "+ ofToString(i+oldChannels), "Parameter "+ofToString(i+oldChannels)));
////        }
////        else
////        {
////            for(int i=difference;i<0;i++) removeComponent;
////        }
//    }
//    
//    void show(int midiCCStart, int dmxStart)
//    {
//        
//        ofxModalWindow::show();
//    }
//    
//    ofxDatGuiButton* getButton(int index)
//    {
//        return ofxModalWindow::getButton(index);
//    }
//    
//private:
//    
//};
//
//class editLight : public ofxModalWindow
//{
//    
//};
//#endif /* modalDialogs_h */
