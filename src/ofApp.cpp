#pragma once

#include "ofApp.h"
#include "ofMain.h"


int snaves=0;

void ofApp::setup()//============================================================
{
//    if(snaves==0)ofSetDataPathRoot(ofFilePath::getCurrentExeDir()+"../Resources/data/");
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
    if(shader.load("fxShader.vert","fxShader.frag"))cout<<"mainShader loaded"; else cout<<"mainShader not loaded";
    if(asciiShader.load("asciiShader.vert","asciiShader.frag")) cout<<"asciiShader loaded"; else cout<<"asciiShader not loaded";
    if(ledShader.load("ledShader.vert","ledShader.frag"))cout<<"ledShader loaded"; else cout<<"ledShader not loaded";
    if(chromaKeyShader.load("chromaKeyShader.vert","chromaKeyShader.frag"))cout<<"chromaKeyShader loaded"; else cout<<"chromaKeyShader not loaded";
    
    if(snaves==0) allocateFBOs();
    
    ofLog()<<(char*)glGetString(GL_VERSION);

//---MIDI Setup-----------------------------------------------------
    ofSetVerticalSync(true);
    midiIn.listInPorts(); // print input ports
    midiIn.openPort(0);
    midiIn.ignoreTypes(false, false, true); // don't ignore sysex, timing, & active sense messages, these are ignored by default
    midiIn.addListener(this);   // add ofApp as a listener
    videoCount=0;
    
//--GUI setup--------------------------------------------------
    
    if(snaves==0)
    {
        gui = new ofxDatGui(ofxDatGuiAnchor::NO_ANCHOR);
        gui->setAutoDraw(true);
        gui2= new ofxDatGui(ofxDatGuiAnchor::NO_ANCHOR);
        gui2->setAutoDraw(true);
//        gui->addFRM();
        clear=false;
        
        gui2->addLabel("VIDEOS");
        clearToggle = gui2->addToggle("Clear");
        clearAllButton = gui2->addButton("Clear All");
        gui2->setPosition(gui->getWidth(),0);
        gui2->addBreak();
        gui2->addBreak();
        for(int i=0;i<max_videos;i++)
        {
            videoButtons[i]=(gui2->addButton(videoOptions[i]));
        }
        
        saveButton = gui->addButton("Save");
        loadButton = gui->addButton("Load");
        midiDropdown= gui->addDropdown("MIDI Port:", midiIn.getInPortList());
        
        backgroundFolder=gui->addFolder("Background");
        bgColor1ColorPicker=backgroundFolder->addColorPicker("BG Color 1",bgColor1);
//        bgColor2ColorPicker=backgroundFolder->addColorPicker("BG Color 2", bgColor2);
        tempoDivisionSlider=backgroundFolder->addSlider("Tempo Division",1,3,1);
        tripletToggle=backgroundFolder->addToggle("Triplet");
    
        gui->addLabel("FX");
        fxMacroSlider = gui->addSlider("FX Wet",0.0,1.0,fxMacro);
        
        videoFolder=gui->addFolder("Video Controls");
        videoSpeedSlider=videoFolder->addSlider("Video Speed",0.1,10.0,videoSpeed2);
        videoSyncToggle=videoFolder->addToggle("Video Sync");
        videoDivisionSlider=videoFolder->addSlider("Video Division",1,8,1);
        
        invertFolder=gui->addFolder("Invert");
        invertSlider=invertFolder->addSlider("Invert", 0.0, 1.0, invertMacro);
        invertSlider->setPrecision(1);
        invertSlider->setValue(invertMacro);
        
        rippleFolder=gui->addFolder("Ripple");
        rippleSlider = rippleFolder->addSlider("Ripple", 0.0,1.0,rippleMacro);
        rippleSyncToggle=rippleFolder->addToggle("Sync");
        rippleXSlider=rippleFolder->addSlider("X", 0.0, 1.0,rippleX);
        rippleYSlider=rippleFolder->addSlider("Y", 0.0, 1.0,rippleY);
        rippleRateSlider=rippleFolder->addSlider("Rate",0.1, 300.00,rippleRate);
        
        filterFolder=gui->addFolder("Filter");
        filterSlider=filterFolder->addSlider("Filter",0.0,1.0,filterMacro);
        filterRedSlider=filterFolder->addSlider("Red", 0.0,1.0,filterRed);
        filterGreenSlider=filterFolder->addSlider("Green",0.0,1.0,filterGreen);
        filterBlueSlider=filterFolder->addSlider("Blue",0.0,1.0,filterBlue);
//        filterAlphaSlider=filterFolder->addSlider("Alpha",0.0,1.0,filterAlpha);

        kaleidioscopeFolder=gui->addFolder("Kaleidioscope");
        kaleidoscopeSlider=kaleidioscopeFolder->addSlider("Kaleidoscope",0.0,1.0,kaleidoscopeMacro);
        sectorSlider=kaleidioscopeFolder->addSlider("Sectors", 1.0, 100,kaleidioscopeSectors);
        angleSlider=kaleidioscopeFolder->addSlider("Angle",-180,180,kaleidioscopeAngle);
        xSlider=kaleidioscopeFolder->addSlider("X",0.0,1.0,kaleiodioscopeX);
        ySlider=kaleidioscopeFolder->addSlider("Y",0.0,1.0,kaleiodioscopeY);
        
        pixelateFolder=gui->addFolder("Pixelate");
        pixelateSlider=pixelateFolder->addSlider("Pixelate", 0, 100, pixelateMacro);
        
        fullhouseFolder=gui->addFolder("Fullhouse");
        fullhouseSlider=fullhouseFolder->addSlider("Fullhouse", 1, 50, fullhouseMacro);
        
        asciiFolder=gui->addFolder("Ascii");
        asciiMacroSlider=asciiFolder->addSlider("Ascii", 0.0,1.0,asciiMacro);
        asciiInvertToggle=asciiFolder->addToggle("Ascii Invert", asciiInvert);
        asciiImageContrastSlider=asciiFolder->addSlider("Ascii Image Contrast", 0.0, 1.0,asciiImageContrast);
        asciiImageGainSlider=asciiFolder->addSlider("Ascii Image Gain", 0.0,1.0,asciiImageGain);
        asciiDotDistanceSlider=asciiFolder->addSlider("ASCII Dot Distance", 0.0, 1.0,asciiDotDistance);

        ledFolder=gui->addFolder("LED");
        ledMacroSlider=ledFolder->addSlider("LED", 0.0, 1.0,ledMacro);
        ledDotDistanceSlider=ledFolder->addSlider("LED Dot Distance", 0.0, 1.0,ledDotDistance);
//        ledOffsetRXSlider=ledFolder->addSlider("Red Offset X", 0.0 , 1.0, ledOffsetRX);
//        ledOffsetRYSlider=ledFolder->addSlider("Red Offset Y", 0.0 , 1.0, ledOffsetRY);
//        ledOffsetGXSlider=ledFolder->addSlider("Green Offset X", 0.0 , 1.0, ledOffsetGX);
//        ledOffsetGYSlider=ledFolder->addSlider("Green Offset Y", 0.0 , 1.0, ledOffsetGY);
//        ledOffsetBXSlider=ledFolder->addSlider("Blue Offset X", 0.0 , 1.0, ledOffsetBX);
//        ledOffsetBYSlider=ledFolder->addSlider("Blue Offset Y", 0.0 , 1.0, ledOffsetBY);
        
        rotateFolder=gui->addFolder("Rotate");
        rotateMacroSlider=rotateFolder->addSlider("Rotate",-1.0,1.0,rotateMacro);
        
        zebraFolder=gui->addFolder("Zebra");
        zebraMacroSlider=zebraFolder->addSlider("Zebra", 0.0, 1.0,zebraMacro);
        zebraSpeedSlider=zebraFolder->addSlider("Speed",0.0,1.0,zebraSpeed);
        zebraLevelsSlider=zebraFolder->addSlider("Levels",2,50,zebraLevels);
        
        chromaKeyFolder=gui->addFolder("ChromaKey");
        chromaKeyMacroSlider=chromaKeyFolder->addSlider("ChromaKey", 0.0, 1.0,chromaKeyMacro);
        chromaKeyColorPicker=chromaKeyFolder->addColorPicker("Key");
        chromaKeyColorPicker->setColor(ofColor::green);
        chromaKeyThresholdSlider=chromaKeyFolder->addSlider("Threshold", 0.0, 1.0,chromaKeyThreshold);

        gui->addBreak();
        gui->addBreak();
        
        gui->onButtonEvent(this, &ofApp::onButtonEvent);
        gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
        gui->onSliderEvent(this, &ofApp::onSliderEvent);
        gui->onToggleEvent(this, &ofApp::onToggleEvent);
        gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
        gui2->onToggleEvent(this, &ofApp::onToggleEvent);
        gui2->onButtonEvent(this, &ofApp::onButtonEventGui2);
        kaleidioscopeFolder->onSliderEvent(this, &ofApp::onSliderEvent);
        cout<<"here 3"<<endl;
        
        pixelateSlider->setPrecision(0);
        fullhouseSlider->setPrecision(0);
        tempoDivisionSlider->setPrecision(0);
        videoDivisionSlider->setPrecision(0);
        sectorSlider->setPrecision(0);
        zebraLevelsSlider->setPrecision(0);
        tripletToggle->setChecked(triplet);
        clearToggle->setChecked(clear);
        videoSyncToggle->setChecked(videoSync);
        rippleSyncToggle->setChecked(rippleSync);
        cout<<"here 4"<<endl;
        ofxDatGuiThemeEvan *theme = new ofxDatGuiThemeEvan;
        ofxDatGuiThemeEvanFlip *flip = new ofxDatGuiThemeEvanFlip;
        
        gui->setTheme(theme);
        gui2->setTheme(flip);
        gui->draw();
        gui2->draw();
        ofBackground(theme->color.guiBackground);
        if(ofLoadImage( font, "font.jpg" ))cout<<"font loaded"<<endl;
        else cout<<"font not loaded"<<endl;
    }
    snaves=1;
}

ofPoint ofApp::windowSize()//============================================================
{
    ofPoint Windowsize(gui->getWidth()+gui2->getWidth(),gui->getHeight()*1.5);
    ofSetWindowShape(Windowsize.x, Windowsize.y);
    return Windowsize;
}
void ofApp::exitGui(ofEventArgs &args)//============================================================
{
    exit();
}

void ofApp::update()//============================================================
{//---------Tempo update--------------------------------
//                                                                          cout << "update"<< endl;
    if(timecodeRunning && ofGetElapsedTimeMillis() - timecodeTimestamp > 100)
    {
        ofLog() << "timecode stopped";
        timecodeRunning = false;
    }
    getWidth=ofGetWidth();
    getHeight=ofGetHeight();
    
    cout<<"currentlyDrawing: "<<currentlyDrawing<<"\n";
}

void ofApp::draw()//============================================================
{
    drawCount=currentlyDrawing;
    chromaKeyVideo=-1;
    
    ofBackground(bgColor1);                                 //set background color according to user parameter
    
    fbo.begin();                                            //FBO 1 begin
    ofClear(0,0,0,0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
//----------Draw Video---------------------------------
//                                                                          cout<<"video to draw: "<<imageToDraw<<endl;
    ofDisableSmoothing();
    
    for(int i=0;i<max_videos;i++)
    {
        if(player[i].drawImage==false)player[i].stop();
        if(player[i].drawImage && videoCount<4)
        {
            if (videoSync==true)
            {
                bps=(bpm/60);
                videoSpeed=(((player[imageToDraw].video.getDuration())/bps)*videoDivision);
                player[i].setSpeed(videoSpeed/2);
                player[i].play();
//                                                                      cout<<"image to draw"<<imageToDraw<<endl;
            }
            else
            {
                player[i].setSpeed(videoSpeed2);
                if(player[i].isPlaying()==false)player[i].play();
            }
                    
            if(videoCount>4) break;
          
            player[i].setLoopState(OF_LOOP_NORMAL);
            ofSetColor(255,255,255,player[i].opacity);      //set color in order to draw video according to its opacity value
            if(drawCount!=0)player[i].draw(0,0,getWidth,getHeight);         //draw video
            else(chromaKeyVideo=i);
            drawCount-=1;
        }
    }
    
    ofClearAlpha();                                         //clear alpha within the fbo itself
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    fbo.end();                                              //FBO end
    
//-----Chroma Key Video FBO--------------------------------------------------
    
    chromaKeyVideoFbo.begin();
    ofClear(0,0,0,0);

    if(chromaKeyVideo!= -1)
        player[chromaKeyVideo].draw(0, 0, getWidth, getHeight);
    
    chromaKeyVideoFbo.end();
    
//-----Chroma Key FX FBO/Shader--------------------------------------------------

    chromaKeyFxFbo.begin();
    chromaKeyShader.begin();
    
    shader.setUniform1f("fxMacro", fxMacro);
    chromaKeyShader.setUniform1f("chromaKeyMacro", chromaKeyMacro);
    chromaKeyShader.setUniform3i("chromaKeyColor", chromaKeyColor.r,chromaKeyColor.g,chromaKeyColor.b);
    chromaKeyShader.setUniform1f("chromaKeyThreshold",chromaKeyThreshold);
    
    chromaKeyVideoFbo.draw(0,0,getWidth,getHeight);
    
    chromaKeyShader.end();
    chromaKeyFxFbo.end();
    
//-------FX FBO/Shader---------------------------------------------------------

    fbo2.begin();                                           //FBO 2 begin
    ofClear(0,0,0,0);
    shader.begin();                                         //Shader1 begin
    float time = ofGetElapsedTimef();
        
    shader.setUniform1f("fxMacro", fxMacro);

    shader.setUniform1f("time", time);
    shader.setUniform2f("resolution", getWidth,getHeight);

    shader.setUniform1f("filterMacro", filterMacro);
    shader.setUniform4f("filterRGB",filterRed,filterGreen,filterBlue,1.0); //commented out FilterAlpha

    shader.setUniform1f("invertMacro", invertMacro);

    shader.setUniform1f("rippleMacro", rippleMacro);
    shader.setUniform2f("rippleXY", rippleX,rippleY);
    shader.setUniform1f("rippleRate",rippleRate);

    shader.setUniform1i("ksectors", kaleidioscopeSectors*kaleidoscopeMacro);
    shader.setUniform2f("kcenter", kaleiodioscopeX*kaleidoscopeMacro,kaleiodioscopeY*kaleidoscopeMacro);
    shader.setUniform1f("kaleidoscopeMacro", kaleidoscopeMacro);
    shader.setUniform1f("kangleRad", (ofDegToRad(kaleidioscopeAngle))*kaleidoscopeMacro);
    if(kaleidoscopeMacro<.5) shader.setUniform2f("screenCenter",0,0);
    else shader.setUniform2f("screenCenter",0.5*getWidth,0.5*getHeight);

    shader.setUniform1i("pixelateMacro", pixelateMacro);

    shader.setUniform1i("fullhouseMacro", fullhouseMacro);
    
    shader.setUniform1f("rotateMacro", rotateMacro);
    shader.setUniform2f("rotateScreenCenter",0.5*getWidth,0.5*getHeight);
    
    shader.setUniform1f("zebraMacro", zebraMacro);
    shader.setUniform1f("zebraSpeed", zebraSpeed);
    shader.setUniform1i("zebraLevels", zebraLevels);
    
    fbo.draw(0,0, getWidth, getHeight);             //first FBO draw
    chromaKeyFxFbo.draw(0,0,getWidth,getHeight);
    shader.end();                                   //shader1 end
    fbo2.end();                                     // FBO 2 end

//------ASCII FBO/Shader---------------------------------------------------------------------------------------------
    fbo3.begin();                                   //FBO 3 begin
    ofClear(0,0,0,0);
    asciiShader.begin();                            //ASCII Shader begin

    asciiShader.setUniform1f("fxMacro", fxMacro);
    asciiShader.setUniform1f("asciiMacro", asciiMacro);
    asciiShader.setUniform1f("asciiDotDistance", asciiDotDistance);
    asciiShader.setUniform1f("asciiImageGain", asciiImageGain);
    asciiShader.setUniform1f("asciiImageContrast", asciiImageContrast);
    asciiShader.setUniform1i("asciiInvert", asciiInvert);
    asciiShader.setUniformTexture("font", font, 8);

    fbo2.draw(0,0, getWidth, getHeight);              //FBO 2 draw

    asciiShader.end();                                //ASCII Shader end
    fbo3.end();                                       //FBO 3 end
    
//-------LED FBO/Shader---------------------------------------------------------
    fbo4.begin();                                       //FBO 4 begin
    ofClear(0,0,0,0);
    ledShader.begin();                                 //LED Shader begin
    
    ledShader.setUniform1f("fxMacro", fxMacro);
    ledShader.setUniform1f("ledMacro", ledMacro);
    ledShader.setUniform1f("ledDotDistance", ledDotDistance);
//    ledShader.setUniform2f("ledOffsetRed", ledOffsetRX, ledOffsetRY);
//    ledShader.setUniform2f("ledOffsetGreen", ledOffsetGX, ledOffsetGY);
//    ledShader.setUniform2f("ledOffsetBlue", ledOffsetBX, ledOffsetBY);

//    ofSetColor(255, 255, 255);
//    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    fbo3.draw(0,0, getWidth, getHeight);                //FBO 3 draw
        
    ledShader.end();                                    //LED Shader end
    fbo4.end();                                         //FBO 4 end
    
    fbo4.draw(0,0, getWidth, getHeight);                //FBO 4 draw (final draw)
}

void ofApp::newMidiMessage (ofxMidiMessage& msg)//============================================================
{
    if(snaves==1)
    {
        
        if(msg.status==MIDI_NOTE_ON && msg.pitch>=60 && msg.pitch<=84)
        {
            midiMessages.push_back(msg); // add the latest message to the message queue
            while(midiMessages.size() > maxMessages) midiMessages.erase(midiMessages.begin());
            
            pitch=(msg.pitch);

            if(sustain)midiNoteOff(msg.pitch);
            
            if(msg.pitch>=60 && msg.pitch<=84)
            {
                playerFromMidiMessage=(msg.pitch-60);
                videoCount+=1;
    //                                                                        cout<<"videoCount: "<<videoCount<<endl;
                player[playerFromMidiMessage].opacity=msg.velocity;
                player[playerFromMidiMessage].drawImage=true;
                player[playerFromMidiMessage].opacity=ofMap(player[playerFromMidiMessage].opacity, 0, 127, 0, 255);
            }
        }
        
        if(msg.status==MIDI_NOTE_OFF && msg.pitch>=60 && msg.pitch<=84)
        {
            if(sustain==false)
            {
                midiNoteOff(msg.pitch);
                videoCount=-1;
            }
        }

        if(msg.status==MIDI_CONTROL_CHANGE)         //MIDI CC (FX) change cases
        {
            switch(msg.control)
            {
                case 123:
                {
                    for(int i=0;i<max_videos;i++) midiNoteOff(i+60);
                }
                    break;
                case 64: if(msg.value>63)sustain=true; else sustain=false;
                case 15:
                    fxMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                    fxMacroSlider->setValue(fxMacro);
                    break;
                case 16:
                    videoSpeed2=ofMap(msg.value, 0, 127, .1, 10.00);
                    videoSpeedSlider->setValue(videoSpeed2);
                    break;
                case 17:
                    videoDivision=msg.value;
                    videoDivisionSlider->setValue(videoDivision);
                    break;
    //            case 19:
    //                if(msg.value>63) backgroundSwitch=true;
    //                else backgroundSwitch=false;
    //                backgroundSwitchToggle->setChecked(backgroundSwitch);
    //                break;
                case 18:
                    if(msg.value>63) videoSync=true;
                    else videoSync=false;
                    videoSyncToggle->setChecked(videoSync);
                    break;
                case 20:
                    tempoDivision=msg.value;
                    tempoDivisionSlider->setValue(tempoDivision);
                    break;
                case 21:
                    if(msg.value>63)triplet=true;
                    else triplet=false;
                    tripletToggle->setChecked(triplet);
                    break;
    //            case 21:
    //                bgColor1=ofFloatColor(ofMap(msg.value,0, 127, 0.0, 1.0));
    //                bgColor1ColorPicker->setColor(bgColor1);
    //                break;

                                                                                //EMPTY CASE 22
                    
                case 24:
                    invertMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                     invertSlider->setValue(invertMacro);
                    break;
                case 25:
                    rippleMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                    rippleSlider->setValue(rippleMacro);
                    break;
                case 26:
                {
                    if(msg.value>63) rippleSync=true;
                    else rippleSync=false;
                    rippleSyncToggle->setChecked(rippleSync);
                    break;
                }
                case 27:
                    rippleX=ofMap(msg.value,0, 127, 0.0, 1.0);
                    rippleXSlider->setValue(rippleX);
                    break;
                case 28:
                    rippleY=ofMap(msg.value,0, 127, 0.0, 1.00);
                    rippleYSlider->setValue(rippleY);
                    break;
                case 29:
                    if(rippleSync) rippleRate=bpm/60;
                    else rippleRate=ofMap(msg.value,0, 127, .1, 300);
                    rippleRateSlider->setValue(rippleRate);
                    break;

                case 30:
                    filterMacro=ofMap(msg.value,0, 127, 0, 1.0);
                    filterSlider->setValue(filterMacro);
                    break;
                case 31:
                    filterRed=ofMap(msg.value,0, 127, 0, 1.0);
                    filterRedSlider->setValue(filterRed);
                    break;
                case 33:
                    filterGreen=ofMap(msg.value,0, 127, 0, 1.0);
                    filterGreenSlider->setValue(filterGreen);
                    break;
                case 34:
                    filterBlue=ofMap(msg.value,0, 127, 0, 1.0);
                    filterBlueSlider->setValue(filterBlue);
                    break;
                                                                                        //EMPTY CASE 35
                    
                case 40:
                    kaleidoscopeMacro=ofMap(msg.value,0, 127, 0.0, 1.00);
                    kaleidoscopeSlider->setValue(kaleidoscopeMacro);
                    break;
                case 41:
                    kaleidioscopeAngle=ofMap(msg.value,0, 127, -180.0, 180.0);
                    angleSlider->setValue(kaleidioscopeAngle);
                    break;
                case 42:
                    kaleiodioscopeX=ofMap(msg.value,0, 127, 0.0, 1.00);
                    xSlider->setValue(kaleiodioscopeX);
                    break;
                case 43:
                    kaleiodioscopeY=ofMap(msg.value,0, 127, 0.0, 1.00);
                    ySlider->setValue(kaleiodioscopeY);
                    break;
                case 44:
                    kaleidioscopeSectors=ofMap(msg.value, 0, 127, 1.0, 100.0);
                    sectorSlider->setValue(kaleidioscopeSectors);
                    break;
                case 50:
                    bgColor1Red=msg.value;
                    bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                    bgColor1ColorPicker->setColor(bgColor1);
                    break;
                case 51:
                    bgColor1Green=msg.value;
                    bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                    bgColor1ColorPicker->setColor(bgColor1);
                    break;
                case 52:
                    bgColor1Blue=msg.value;
                    bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                    bgColor1ColorPicker->setColor(bgColor1);
                    break;
//                case 53:
//                    bgColor2Red=msg.value;
//                    bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    bgColor2ColorPicker->setColor(bgColor2);
//                    break;
//                case 54:
//                    bgColor2Green=msg.value;
//                    bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    bgColor2ColorPicker->setColor(bgColor2);
//                    break;
//                case 55:
//                    bgColor2Blue=msg.value;
//                    bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    bgColor2ColorPicker->setColor(bgColor2);
//                    break;
                case 56:
                    pixelateMacro=ofMap(msg.value,0, 127, 0, 100);
                    pixelateSlider->setValue(pixelateMacro);
                    break;
                case 57:
                    fullhouseMacro=ofMap(msg.value,0,127,1,50);
                    fullhouseSlider->setValue(fullhouseMacro);
                    break;
                case 60:
                    asciiMacro=ofMap(msg.value,0,127,0.0,1.0);
                    asciiMacroSlider->setValue(asciiMacro);
                    break;
                case 61:
                    asciiDotDistance=ofMap(msg.value, 0, 127, 0.0, 1.0);
                    asciiDotDistanceSlider->setValue(asciiDotDistance);
                    break;
                case 62:
                    asciiImageGain=ofMap(msg.value, 0, 127, 0.0, 1.0);
                    asciiImageGainSlider->setValue(asciiImageGain);
                    break;
                case 63:
                    asciiImageContrast=ofMap(msg.value,0,127,0.0,1.0);
                    asciiImageContrastSlider->setValue(asciiImageContrast);
                    break;
                case 65:
                    if(msg.value>63) asciiInvert=true;
                    else asciiInvert=false;
                    asciiInvertToggle->setChecked(asciiInvert);
                    break;
                case 66:
                    ledMacro=ofMap(msg.value,0,127,0.0,1.0);
                    ledMacroSlider->setValue(ledMacro);
                    break;
                case 67:
                    ledDotDistance=ofMap(msg.value, 0, 127, 0.0, 1.0);
                    ledDotDistanceSlider->setValue(ledDotDistance);
                    break;
    //            case 68:
    //                ledOffsetRX=ofMap(msg.value,0,127,0.0,100.0);
    //                ledOffsetRXSlider->setValue(ledOffsetRX);
    //                break;
    //            case 69:
    //                ledOffsetRY=ofMap(msg.value, 0, 127, 0.0, 100.0);
    //                ledOffsetRYSlider->setValue(ledOffsetRY);
    //                break;
    //            case 70:
    //                ledOffsetGX=ofMap(msg.value, 0, 127, 0.0, 100.0);
    //                ledOffsetGXSlider->setValue(ledOffsetGX);
    //                break;
    //            case 71:
    //                ledOffsetGY=ofMap(msg.value, 0, 127, 0.0, 100.0);
    //                ledOffsetGYSlider->setValue(ledOffsetGY);
    //                break;
    //            case 72:
    //                ledOffsetBX=ofMap(msg.value, 0, 127, 0.0, 100.0);
    //                ledOffsetBXSlider->setValue(ledOffsetBX);
    //                break;
    //            case 73:
    //                ledOffsetGY=ofMap(msg.value, 0, 127, 0.0, 100.0);
    //                ledOffsetGYSlider->setValue(ledOffsetGY);
    //                break;
                case 74:
                    rotateMacro=ofMap(msg.value,0,127,0.0,1.0);
                    rotateMacroSlider->setValue(rotateMacro);
                    break;
                case 75:
                    zebraMacro=ofMap(msg.value,0,127,0.0,1.0);
                    zebraMacroSlider->setValue(zebraMacro);
                    break;
                case 76:
                    zebraSpeed=ofMap(msg.value,0,127,0.0,1.0);
                    zebraSpeedSlider->setValue(zebraSpeed);
                    break;
                    
                case 77:
                    zebraLevels=ofMap(msg.value, 0, 127, 2, 50);
                    zebraLevelsSlider->setValue(zebraLevels);
                    break;
                case 78:
                    chromaKeyMacro=ofMap(msg.value,0,127,0.0,1.0);
                    chromaKeyMacroSlider->setValue(chromaKeyMacro);
                    break;
                case 79:
                    chromaKeyRed=ofMap(msg.value,0,127,0,255);
                    chromaKeyColorPicker->setColor(chromaKeyColor);
                    chromaKeyColor.r=ofMap(chromaKeyRed,0,255,0.0,1.0);
                    break;
                case 80:
                    chromaKeyGreen=ofMap(msg.value,0,127,0,255);
                    chromaKeyColorPicker->setColor(chromaKeyColor);
                    chromaKeyColor.g=ofMap(chromaKeyGreen,0,255,0.0,1.0);
                    break;
                case 81:
                    chromaKeyBlue=ofMap(msg.value,0,127,0,255);
                    chromaKeyColorPicker->setColor(chromaKeyColor);
                    chromaKeyColor.b=ofMap(chromaKeyBlue ,0,255,0.0,1.0);
                    break;
                case 82:
                    chromaKeyThreshold=ofMap(msg.value, 0, 127, 0, 255);
                    chromaKeyThresholdSlider->setValue(chromaKeyThreshold);
                    break;
            }
        }
    }
}
void ofApp::midiNoteOff(int pitch)//============================================================
{
        player[pitch-60].drawImage=false;
        player[pitch-60].stop();
        player[pitch-60].firstFrame();
}

void ofApp::exit()//============================================================
{
    midiIn.closePort();
    midiIn.removeListener(this);
    
    if (!exiting)
    {
        exiting = true;
        ofExit();
    }
}


void ofApp::keyPressed(ofKeyEventArgs & args)//============================================================
{
    //------------Ability to preview via QWERTY keyboard presses------------------
                                                                    cout<<"key pressed\n"<<endl;
    int key = args.key;
    if(key==OF_KEY_COMMAND) command=true;
    
    switch (key)
    {
        case '1': playerFromMidiMessage=0; break;
        case '2': playerFromMidiMessage=1; break;
        case '3': playerFromMidiMessage=2; break;
        case '4': playerFromMidiMessage=3; break;
        case '5': playerFromMidiMessage=4; break;
        case '6': playerFromMidiMessage=5; break;
        case '7': playerFromMidiMessage=6; break;
        case '8': playerFromMidiMessage=7; break;
        case '9': playerFromMidiMessage=8; break;
        case '0': playerFromMidiMessage=9; break;
        case 'q': playerFromMidiMessage=10; break;
        case 'w': playerFromMidiMessage=11; break;
        case 'e': playerFromMidiMessage=12; break;
        case 'r': playerFromMidiMessage=13; break;
        case 't': playerFromMidiMessage=14; break;
        case 'y': playerFromMidiMessage=15; break;
        case 'u': playerFromMidiMessage=16; break;
        case 'i': playerFromMidiMessage=17; break;
        case 'o': playerFromMidiMessage=18; break;
        case 'p': playerFromMidiMessage=19; break;
        case 'a': playerFromMidiMessage=20; break;
        case 's': playerFromMidiMessage=21; break;
        case 'd': playerFromMidiMessage=22; break;
        case 'f': playerFromMidiMessage=23; break;
        case 'g': playerFromMidiMessage=24; break;
        default: break;
    }
    
    if(player[playerFromMidiMessage].drawImage!=true)
    {
        player[playerFromMidiMessage].drawImage=true;
        player[playerFromMidiMessage].size=127;
        player[playerFromMidiMessage].size=ofMap(player[playerFromMidiMessage].size, 0, 127, 0.0, 2.0);

        cout<<"Currently Drawing increment \n";
        currentlyDrawing+=1;
    }
    
}
//--------------------------------------------------------------
void ofApp::keyReleased(ofKeyEventArgs & args)//============================================================
{
                                                                            cout<<"Key Released"<<endl;
    
    int key=args.key;
    
    switch (key)
    {
//        case '1': player[0].drawImage=false; player[0].stop(); player[0].firstFrame(); break;
//        case '2': player[1].drawImage=false; player[1].stop(); player[1].firstFrame(); break;
//        case '3': player[2].drawImage=false; player[2].stop(); player[2].firstFrame(); break;
//        case '4': player[3].drawImage=false; player[3].stop(); player[3].firstFrame(); break;
//        case '5': player[4].drawImage=false; player[4].stop(); player[4].firstFrame(); break;
//        case '6': player[5].drawImage=false; player[5].stop(); player[5].firstFrame(); break;
//        case '7': player[6].drawImage=false; player[6].stop(); player[6].firstFrame(); break;
//        case '8': player[7].drawImage=false; player[7].stop(); player[7].firstFrame(); break;
//        case '9': player[8].drawImage=false; player[8].stop(); player[8].firstFrame(); break;
//        case '0': player[9].drawImage=false; player[9].stop(); player[9].firstFrame(); break;
//        case 'q': player[10].drawImage=false; player[10].stop(); player[10].firstFrame();break;
//        case 'w': player[11].drawImage=false; player[11].stop(); player[11].firstFrame();break;
//        case 'e': player[12].drawImage=false; player[12].stop(); player[12].firstFrame();break;
//        case 'r': player[13].drawImage=false; player[13].stop(); player[13].firstFrame();break;
//        case 't': player[14].drawImage=false; player[14].stop(); player[14].firstFrame();break;
//        case 'y': player[15].drawImage=false; player[15].stop(); player[15].firstFrame();break;
//        case 'u': player[16].drawImage=false; player[16].stop(); player[16].firstFrame();break;
//        case 'i': player[17].drawImage=false; player[17].stop(); player[17].firstFrame();break;
//        case 'o': player[18].drawImage=false; player[18].stop(); player[18].firstFrame();break;
//        case 'p': player[19].drawImage=false; player[19].stop(); player[19].firstFrame();break;
//        case 'a': player[20].drawImage=false; player[20].stop(); player[20].firstFrame();break;
//        case 's': player[21].drawImage=false; player[21].stop(); player[21].firstFrame();break;
//        case 'd': player[22].drawImage=false; player[22].stop(); player[22].firstFrame();break;
//        case 'f': player[23].drawImage=false; player[23].stop(); player[23].firstFrame();break;
//        case 'g': player[24].drawImage=false; player[24].stop(); player[24].firstFrame();break;
            
            case '1': videoNumber=0; break;
            case '2': videoNumber=1; break;
            case '3': videoNumber=2; break;
            case '4': videoNumber=3; break;
            case '5': videoNumber=4; break;
            case '6': videoNumber=5; break;
            case '7': videoNumber=6; break;
            case '8': videoNumber=7; break;
            case '9': videoNumber=8; break;
            case '0': videoNumber=9; break;
            case 'q': videoNumber=10; break;
            case 'w': videoNumber=11; break;
            case 'e': videoNumber=12; break;
            case 'r': videoNumber=13; break;
            case 't': videoNumber=14; break;
            case 'y': videoNumber=15; break;
            case 'u': videoNumber=16; break;
            case 'i': videoNumber=17; break;
            case 'o': videoNumber=18; break;
            case 'p': videoNumber=19; break;
            case 'a': videoNumber=20; break;
            case 's': videoNumber=21; break;
            case 'd': videoNumber=22; break;
            case 'f': videoNumber=23; break;
            case 'g': videoNumber=24; break;
    }
    
    if(player[videoNumber].drawImage!=false)
    {
        player[videoNumber].drawImage=false;
        player[videoNumber].stop();
        player[videoNumber].firstFrame();
        currentlyDrawing-=1;                            cout<<"Currently Drawing decrement \n";
    }
}
////--------------------------------------------------------------
//void ofApp::mouseMoved(int x, int y) {}
////--------------------------------------------------------------
//void ofApp::mouseDragged(int x, int y, int button) {}
////--------------------------------------------------------------
//void ofApp::mousePressed(int x, int y, int button) {}
////--------------------------------------------------------------
//void ofApp::mouseReleased() {}
//--------------------------------------------------------------

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)//============================================================
{
    midiPort((e.target->getSelected())->getIndex());
}

void ofApp::dragEvent(ofDragInfo & dragInfo)//============================================================
{//--------event handler function for when a file is dragged into Spectacle window----------------------

    if(dragInfo.files.size() > 0)
    {
        for(int m=0;m<dragInfo.files.size();m++)
        {
            int i=0, j=0;
            bool brake=false;
            
            for(j=0;j<max_videos;j++)
            {
                if(player[j].getPath()=="")
                {
                    i=j;
                    brake=true;
                    break;
                }
                if(brake==true) break;
            }
            cout<<"new movie path:"<<ofFilePath::getFileName(dragInfo.files[m])<<endl<<"player array: "<<dragInfo.files[m]<<endl;
            player[i].video.setPixelFormat(OF_PIXELS_RGBA);
            if(player[i].load(dragInfo.files[m]))
            {
                videoButtons[i]->setLabel(ofFilePath::getFileName(dragInfo.files[m]));
                player[i].setLoopState(OF_LOOP_NORMAL);
            }
        }
    }
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)//============================================================
{//--------Button event handler function for FX buttons----------------------

    if(e.target==saveButton) saveSettings();
    else if(e.target==loadButton) loadSettings();
}

void ofApp::onButtonEventGui2(ofxDatGuiButtonEvent e)///============================================================
{//--------Button event handler function for video array part of GUI----------------------
//                                                            cout << "onButtonEvent: " << e.target->getLabel() << endl;
    if(e.target==clearAllButton) clearAllVideos();
    
    else
    {
        int iLabel=stoi(e.target->getName());
        iLabel-=1;
        if(clear==true)
        {
            if(player[iLabel].isLoaded()) player[iLabel].close();
            clear=false;
            clearToggle->setBackgroundColor(ofColor::black);
            clearToggle->setLabelColor(ofColor::white);
            e.target->setLabel(ofToString(iLabel));
        }
        else
        {
            loadMovie(iLabel);
        }
    }
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)//============================================================
{//--------Toggle event handler function----------------------

    if (e.target==tripletToggle)triplet=!triplet;
//    else if (e.target->is("Background Switch"))backgroundSwitch=!backgroundSwitch;
    else if (e.target==videoSyncToggle)videoSync=!videoSync;
    
    else if(e.target==clearToggle) clear=!clear;
    
    else if(e.target==rippleSyncToggle)rippleRate=bpm*60;
    
    else if(e.target==asciiInvertToggle)asciiInvert=!asciiInvert;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)//============================================================
{
    //--------Slider event handler function----------------------
    
    if(e.target==tempoDivisionSlider)tempoDivision=(e.target->getValue());
    if(e.target==videoDivisionSlider)videoDivision=(e.target->getValue());
    if(e.target==fxMacroSlider)fxMacro=(e.target->getValue());
    
    else if(e.target==kaleidoscopeSlider)kaleidoscopeMacro = (e.target->getValue());
    else if(e.target==angleSlider)kaleidioscopeAngle=(e.target->getValue());
    else if(e.target==xSlider)kaleiodioscopeX=(e.target->getValue());
    else if(e.target==ySlider)kaleiodioscopeY=(e.target->getValue());
    else if(e.target==sectorSlider)kaleidioscopeSectors=(e.target->getValue());
    
    else if(e.target==filterSlider)filterMacro=(e.target->getValue());
    else if(e.target==filterRedSlider)filterRed=(e.target->getValue());
    else if(e.target==filterBlueSlider)filterBlue=(e.target->getValue());
    else if(e.target==filterGreenSlider)filterGreen=(e.target->getValue());
    
    else if(e.target==rippleSlider)rippleMacro=(e.target->getValue());
    else if(e.target==rippleYSlider)rippleY=(e.target->getValue());
    else if(e.target==rippleXSlider)rippleX=(e.target->getValue());
    else if(e.target==rippleRateSlider)
    {
        if(rippleSync==false)rippleRate=(e.target->getValue());
    }
    else if(e.target==invertSlider)invertMacro=(e.target->getValue());
    
    else if(e.target==videoSpeedSlider)videoSpeed2=(e.target->getValue());
    
    else if(e.target==pixelateSlider)pixelateMacro=(e.target->getValue());
    
    else if(e.target==fullhouseSlider)fullhouseMacro=(e.target->getValue());
    
    else if(e.target==asciiMacroSlider)asciiMacro=(e.target->getValue());
    else if(e.target==asciiImageGainSlider)asciiImageGain=(e.target->getValue());
    else if(e.target==asciiImageContrastSlider)asciiImageContrast=(e.target->getValue());
    else if(e.target==asciiDotDistanceSlider)asciiDotDistance=(e.target->getValue());

    else if(e.target==ledMacroSlider)ledMacro=(e.target->getValue());
    else if(e.target==ledDotDistanceSlider)ledDotDistance=(e.target->getValue());
    else if(e.target==ledOffsetRXSlider)ledOffsetRX=(e.target->getValue());
    else if(e.target==ledOffsetRYSlider)ledOffsetRY=(e.target->getValue());
    else if(e.target==ledOffsetGXSlider)ledOffsetGX=(e.target->getValue());
    else if(e.target==ledOffsetGYSlider)ledOffsetGY=(e.target->getValue());
    else if(e.target==ledOffsetBXSlider)ledOffsetBX=(e.target->getValue());
    else if(e.target==ledOffsetBYSlider)ledOffsetBY=(e.target->getValue());
    
    else if(e.target==rotateMacroSlider)rotateMacro=(e.target->getValue());
    
    else if(e.target==zebraMacroSlider)zebraMacro=(e.target->getValue());
    else if(e.target==zebraLevelsSlider)zebraLevels=(e.target->getValue());
    else if(e.target==zebraSpeedSlider)zebraSpeed=(e.target->getValue());
    
    else if(e.target==chromaKeyMacroSlider)chromaKeyMacro=(e.target->getValue());
    else if(e.target==chromaKeyThresholdSlider)chromaKeyThreshold=(e.target->getValue());
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)//============================================================
{    //--------Color picker event handler function----------------------
//                                        cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    if (e.target->is("BG Color 1"))
    {
        bgColor1=(e.color);
        bgColor1Red=e.color.r;
        bgColor1Green=e.color.g;
        bgColor1Blue=e.color.b;
    }
    if(e.target->is("Key"))
    {
        chromaKeyColor=(e.color);
        chromaKeyColorPicker->setColor(chromaKeyColor);
    }
}

void ofApp::clearAllVideos()//============================================================
{//--------function to clear all videos from array----------------------
    for(int i=0;i<max_videos;i++)
    {
//        ofxDatGuiButton *e=videoButtons[i];
        if(player[i].isLoaded()) player[i].close();
        videoButtons[i]->setLabel(ofToString(i+1));
    }
}

bool ofApp::loadMovie(int i)//============================================================
{//--------Load video function---------------------

//                                                                    cout << "loading videos" << endl;
    ofFileDialogResult result = ofSystemLoadDialog("Load file \n");
    
    if(result.bSuccess)
    {
        if (!player[i].load(result.getPath())) return false;
        cout<<"hay \n";
        videoButtons[i]->setLabel(result.getName()); //change this sequence to get the arrarys correct
        player[i].setLoopState(OF_LOOP_NORMAL);
        return true;
    }
    else return false;
}

bool ofApp::loadSettings()//============================================================
{//--------Load saved video array---------------------

    ofFileDialogResult result = ofSystemLoadDialog("Load file");
    cout << "load settings \n";
    if(result.bSuccess)
    {
        string loadPath = result.getPath();
        xmlSettings.loadFile(loadPath);
        if(xmlSettings.loadFile(loadPath))
        {
            fxMacro = xmlSettings.getValue("xmlSettings:fxWet:fxWet", 1.0);
            fxMacroSlider->setValue(fxMacro);
            videoSpeed = xmlSettings.getValue("xmlSettings:video:speed", 1.0);
            videoSpeedSlider->setValue(videoSpeed);
            videoSync = xmlSettings.getValue("xmlSettings:video:sync", 0);
            videoSyncToggle->setChecked(videoSync);
            videoDivision = xmlSettings.getValue("xmlSettings:video:division", 1);
            videoDivisionSlider->setValue(videoDivision);
            triplet = xmlSettings.getValue("xmlSettings:video:triplet", triplet);
            tripletToggle->setChecked(triplet);
            
            tempoDivision = xmlSettings.getValue("xmlSettings:video:tempoDivision", tempoDivision);
            tempoDivisionSlider->setValue(tempoDivision);
            
            bgColor1Red=xmlSettings.getValue("xmlSettings:color:bgColor1Red", 0);
            bgColor1Green=xmlSettings.getValue("xmlSettings:color:bgColor1Green", 0);
            bgColor1Blue=xmlSettings.getValue("xmlSettings:color:bgColor1Blue", 0);
//            bgColor2Red=xmlSettings.getValue("xmlSettings:color:bgColor2Red", 0);
//            bgColor2Green=xmlSettings.getValue("xmlSettings:color:bgColor2Green", 0);
//            bgColor2Blue=xmlSettings.getValue("xmlSettings:color:bgColor2Blue", 0);
            
            bgColor1=(ofColor::fromHsb(bgColor1Red, bgColor1Green, bgColor1Blue));
            bgColor1ColorPicker->setColor(bgColor1);
//            bgColor2=(ofColor::fromHsb(bgColor2Red, bgColor2Green, bgColor2Blue));
//            bgColor2ColorPicker->setColor(bgColor2);

            invertMacro=xmlSettings.getValue("xmlSettings:invert:invert", 0);
            invertSlider->setValue(invertMacro);
            
            rippleMacro=xmlSettings.getValue("xmlSettings:ripple:ripple", 0);
            rippleSlider->setValue(rippleMacro);
            rippleSync=xmlSettings.getValue("xmlSettings:ripple:sync", 0);
            rippleSyncToggle->setChecked(rippleSync);
            rippleX=xmlSettings.getValue("xmlSettings:ripple:x", 0.0);
            rippleXSlider->setValue(rippleX);
            rippleY=xmlSettings.getValue("xmlSettings:ripple:y", 0.0);
            rippleYSlider->setValue(rippleY);
            rippleRate=xmlSettings.getValue("xmlSettings:ripple:rate",60);
            rippleRateSlider->setValue(rippleRate);
            
            filterMacro=xmlSettings.getValue("xmlSettings:filter:filter", 0.0);
            filterSlider->setValue(filterMacro);
            filterRed=xmlSettings.getValue("xmlSettings:filter:red",0.0);
            filterRedSlider->setValue(filterRed);
            filterGreen=xmlSettings.getValue("xmlSettings:filter:green",0.0);
            filterGreenSlider->setValue(filterGreen);
            filterBlue=xmlSettings.getValue("xmlSettings:filter:blue",0.0);
            filterBlueSlider->setValue(filterBlue);
            
            kaleidoscopeMacro=xmlSettings.getValue("xmlSettings:kaleidoscope:kaleidoscope", 0.0);
            kaleidoscopeSlider->setValue(kaleidoscopeMacro);
            kaleidioscopeSectors=xmlSettings.getValue("xmlSettings:kaleidoscope:sectors", 1);
            sectorSlider->setValue(kaleidioscopeSectors);
            kaleidioscopeAngle=xmlSettings.getValue("xmlSettings:kaleidoscope:angle", 0.0);
            angleSlider->setValue(kaleidioscopeAngle);
            kaleiodioscopeX=xmlSettings.getValue("xmlSettings:kaleidoscope:x", 0.0);
            xSlider->setValue(kaleiodioscopeX);
            kaleiodioscopeY=xmlSettings.getValue("xmlSettings:kaleidoscope:y", 0.0);
            ySlider->setValue(kaleiodioscopeY);

            pixelateMacro=xmlSettings.getValue("xmlSettings:pixelate:pixelate", 0.0);
            pixelateSlider->setValue(pixelateMacro);
            
            fullhouseMacro=xmlSettings.getValue("xmlSettings:fullhouse:fullhouse", 0.0);
            fullhouseSlider->setValue(fullhouseMacro);
            
            asciiMacro=xmlSettings.getValue("xmlSettings:ascii:ascii", 0.0);
            asciiMacroSlider->setValue(asciiMacro);
            asciiImageContrast=xmlSettings.getValue("xmlSettings:ascii:imageContrast", 0.0);
            asciiImageContrastSlider->setValue(asciiImageContrast);
            asciiImageGain=xmlSettings.getValue("xmlSettings:ascii:imageGain", 0.0);
            asciiImageGainSlider->setValue(asciiImageGain);
            asciiDotDistance=xmlSettings.getValue("xmlSettings:ascii:dotDistance", 0.0);
            asciiDotDistanceSlider->setValue(asciiDotDistance);
            
            ledMacro=xmlSettings.getValue("xmlSettings:led:led", 0.0);
            ledMacroSlider->setValue(ledMacro);
            ledDotDistance=xmlSettings.getValue("xmlSettings:led:dotDistance", 0.0);
            ledDotDistanceSlider->setValue(ledDotDistance);
            
            rotateMacro=xmlSettings.getValue("xmlSettings:rotate:rotateMacro", 0.0);
            
            zebraMacro=xmlSettings.getValue("xmlSettings:zebra:zebraMacro", 0.0);
            zebraSpeed=xmlSettings.getValue("xmlSettings:zebra:zebraSpeed", 0.0);
            zebraLevels=xmlSettings.getValue("xmlSettings:zebra:zebraLevels", 2);
            
            
            for(int i = 0; i <max_videos; i++)
            {
                player[i].full=(xmlSettings.getValue("xmlSettings:media:full"+ofToString(i), 0));
                if(player[i].full==true)
                {
                    player[i].which=(xmlSettings.getValue("xmlSettings:media:which"+ofToString(i),int(NULL)));
                    string media = (xmlSettings.getValue(("xmlSettings:media:media"+ofToString(i)),player[i].getPath()));
                    
                    ofFile mediaFile;
                    mediaFile.open(media);
                    if(!mediaFile.isFile())
                    {
                        ofSystemAlertDialog("Media file " + ofToString(i) + " has been moved or renamed. Failed to locate.");
                    }
                    else
                    {
                        player[i].load(media);
                        string name = mediaFile.getFileName();
                        videoButtons[i]->setLabel(name);
                    }
                }
            }
            
            ofSystemAlertDialog("File loaded successfully.");
            return true;
        }
        else
        {
            cout<<"  file did not load!  \n"<<endl;;
            ofSystemAlertDialog("File failed to load.\n");
            return false;
        }
    }
    else return false;
}

bool ofApp::saveSettings()//============================================================
{//--------Save video array---------------------

//                                                                          cout << "save settings" << endl;
    ofFileDialogResult result = ofSystemSaveDialog("default.xml", "Save");
    if(result.bSuccess)
    {
        bgColor1Red=(bgColor1.getHue());
        bgColor1Green=(bgColor1.getSaturation());
        bgColor1Blue=(bgColor1.getBrightness());
//        bgColor2Red=(bgColor2.getHue());
//        bgColor2Green=(bgColor2.getSaturation());
//        bgColor2Blue=(bgColor2.getBrightness());
        
        xmlSettings.setValue("xmlSettings:fxWet:fxWet", fxMacro);
        xmlSettings.setValue("xmlSettings:video:speed", videoSpeed);
        xmlSettings.setValue("xmlSettings:video:sync", videoSync);
        xmlSettings.setValue("xmlSettings:video:division", videoDivision);
        xmlSettings.setValue("xmlSettings:video:triplet", triplet);
        xmlSettings.setValue("xmlSettings:video:tempoDivision", tempoDivision);
        
        xmlSettings.setValue("xmlSettings:color:bgColor1Red", bgColor1Red);
        xmlSettings.setValue("xmlSettings:color:bgColor1Green", bgColor1Green);
        xmlSettings.setValue("xmlSettings:color:bgColor1Blue", bgColor1Blue);
//        xmlSettings.setValue("xmlSettings:color:bgColor2Red", bgColor2Red);
//        xmlSettings.setValue("xmlSettings:color:bgColor2Green", bgColor2Green);
//        xmlSettings.setValue("xmlSettings:color:bgColor2Blue", bgColor2Blue);
        
        xmlSettings.setValue("xmlSettings:invert:invert", invertMacro);
        
        xmlSettings.setValue("xmlSettings:ripple:ripple", rippleMacro);
        xmlSettings.setValue("xmlSettings:ripple:sync", rippleSync);
        xmlSettings.setValue("xmlSettings:ripple:x", rippleX);
        xmlSettings.setValue("xmlSettings:ripple:y", rippleY);
        xmlSettings.setValue("xmlSettings:ripple:rate", rippleRate);
        
        xmlSettings.setValue("xmlSettings:filter:filter", filterMacro);
        xmlSettings.setValue("xmlSettings:filter:red", filterRed);
        xmlSettings.setValue("xmlSettings:filter:green", filterGreen);
        xmlSettings.setValue("xmlSettings:filter:blue", filterBlue);
        
        xmlSettings.setValue("xmlSettings:kaleidoscope:kaleidoscope",kaleidoscopeMacro);
        xmlSettings.setValue("xmlSettings:kaleidoscope:sectors",kaleidioscopeSectors);
        xmlSettings.setValue("xmlSettings:kaleidoscope:angle",kaleidioscopeAngle);
        xmlSettings.setValue("xmlSettings:kaleidoscope:x",kaleiodioscopeX);
        xmlSettings.setValue("xmlSettings:kaleidoscope:y",kaleiodioscopeY);

        xmlSettings.setValue("xmlSettings:pixelate:pixelate", pixelateMacro);
        
        xmlSettings.setValue("xmlSettings:fullhouse:fullhouse", fullhouseMacro);
        
        xmlSettings.setValue("xmlSettings:ascii:ascii", asciiMacro);
        xmlSettings.setValue("xmlSettings:ascii:imageContrast", asciiImageContrast);
        xmlSettings.setValue("xmlSettings:ascii:imageGain", asciiImageGain);
        xmlSettings.setValue("xmlSettings:ascii:dotDistance", asciiDotDistance);

        xmlSettings.setValue("xmlSettings:led:led", ledMacro);
        xmlSettings.setValue("xmlSettings:led:dotDistance", ledDotDistance);
        
        xmlSettings.setValue("xmlSettings:rotate:rotateMacro",rotateMacro);
        
        xmlSettings.setValue("xmlSettings:zebra:zebraMacro", zebraMacro);
        xmlSettings.setValue("xmlSettings:zebra:zebraLevels", zebraLevels);
        xmlSettings.setValue("xmlSettings:zebra:zebraSpeed", zebraSpeed);
        
        for(int i = 0; i <max_videos; i++)
        {
            if(player[i].full==true)
            {
                xmlSettings.setValue("xmlSettings:media:full"+ofToString(i), player[i].full);
                xmlSettings.setValue("xmlSettings:media:which"+ofToString(i), player[i].which);
                xmlSettings.setValue("xmlSettings:media:media"+ofToString(i), player[i].path);
            }
        }
//                                                                                        cout << "save out" << endl;
        xmlSettings.save(result.getPath());
        
        ofSystemAlertDialog("Save successful. \n");
        return true;
    }
    else
    {
//                                                                                    cout<<"save failed"<<endl;
        ofSystemAlertDialog("Save failed. \n");
        return false;
    }
    return false;
}

bool ofApp::midiPort(int midiPortOption)//============================================================
{
    if(midiIn.openPort(midiPortOption)) return true;
    else return false;
}

void ofApp::windowResized(ofResizeEventArgs &resize)
{ //--------Event handler for window resizing----------------------

    allocateFBOs();
}

void ofApp::allocateFBOs()//============================================================
{//---------allocating FBOs for window resizing-------------
            int h = ofGetHeight();
            int w = ofGetWidth();
            fbo.allocate(w,h);
            fbo2.allocate(w,h);
            fbo3.allocate(w,h);
            fbo4.allocate(w,h);
            chromaKeyVideoFbo.allocate(w,h);
}
