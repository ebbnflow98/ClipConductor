#pragma once

#include "ofApp.h"
#include "ofMain.h"


int snaves=0;

void ofApp::setup()//============================================================
{
    ofLog(OF_LOG_NOTICE, "hayyy \n");
//    if(snaves==0)ofSetDataPathRoot(ofFilePath::getCurrentExeDir()+"../Resources/data/");
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
    if(shader.load("fxShader.vert","fxShader.frag"))cout<<"mainShader loaded"; else cout<<"mainShader not loaded";
    if(asciiShader.load("asciiShader.vert","asciiShader.frag")) cout<<"asciiShader loaded"; else cout<<"asciiShader not loaded";
    if(ledShader.load("ledShader.vert","ledShader.frag"))cout<<"ledShader loaded"; else cout<<"ledShader not loaded";
    if(chromaKeyShader.load("chromaKeyShader.vert","chromaKeyShader.frag"))cout<<"chromaKeyShader loaded"; else cout<<"chromaKeyShader not loaded";
    
    if(snaves==0) allocateFBOs();
    
    ofLog()<<(char*)glGetString(GL_VERSION);
    
    if(snaves==0)dmx.connect("/dev/tty.usbserial-ENVVVCOF",44); // use the name
    
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
        gui3 = new ofxDatGui(ofxDatGuiAnchor::NO_ANCHOR);
        gui3->setAutoDraw(true);
        clear=false;
        
        gui2->addLabel("Videos");
        gui2->getLabel("Videos")->setLabelAlignment(ofxDatGuiAlignment::CENTER);

//        gui2->getLabel("Videos")->drawPicture=true;
//        gui2->getLabel("Videos")->pictureChoice=ofxDatGuiLabel::differentPictures::FILM;
        
        clearToggle = gui2->addToggle("Clear");
        clearAllButton = gui2->addButton("Clear All");
        gui2->setPosition(gui->getWidth()+gui->getPadding(),0);
        gui2->addBreak();
        gui2->addBreak();
        
        gui3->setPosition(gui->getWidth()+gui->getPadding()+gui2->getWidth()+gui->getPadding(), 0);
        
        
        for(int i=0;i<max_videos;i++)
        {
            videoButtons[i]=(gui2->addButton(ofToString(i+1)));
        }
        
        saveButton = gui->addButton("Save");
        loadButton = gui->addButton("Load");
        midiDropdown= gui->addDropdown("MIDI Port:", midiIn.getInPortList());
        
        gui3->addLabel("Lights");
        gui3->getLabel("Lights")->setLabelAlignment(ofxDatGuiAlignment::CENTER);
//        gui3->getLabel("Lights")->drawPicture=true;
//        gui3->getLabel("Lights")->pictureChoice=ofxDatGuiLabel::differentPictures::LIGHTBULB;
        
//        addLightButton = gui3->addButton("Add Light");
//        addLightButton->drawPlus=true;
        
//        serialDeviceInfo=serial.getDeviceList();
//        for(int i=0;i<serialDeviceInfo.size();i++){serialInputs[i].push_back(serialDeviceInfo[i].getDeviceName());};
//        gui3->addDropdown("DMX Devices", serialInputs);
        
        
        backgroundFolder=gui->addFolder("Background");
        bgColor1ColorPicker=backgroundFolder->addColorPicker("BG Color 1",bgColor1);
        tempoDivisionSlider=backgroundFolder->addSlider("Tempo Division",1,3,1);
        tripletToggle=backgroundFolder->addToggle("Triplet");
        
        gui->addLabel("FX");
        gui->getLabel("FX")->setLabelAlignment(ofxDatGuiAlignment::CENTER);

//        gui->getLabel("FX")->drawPicture=true;
//        gui->getLabel("FX")->pictureChoice=ofxDatGuiLabel::differentPictures::EYE;
        fxMacroSlider = gui->addSlider("FX Wet",0.0,1.0,fxMacro);
        
        videoFolder=gui->addFolder("Video Controls");
        videoSpeedSlider=videoFolder->addSlider("Video Speed",0.1,10.0,videoSpeed2);
        videoSyncToggle=videoFolder->addToggle("Video Sync");
        videoDivisionSlider=videoFolder->addSlider("Video Division",1,8,1);
        
        invertFolder=gui->addFolder("Invert");
        invertMacroSlider=invertFolder->addSlider("Invert", 0.0, 1.0, invertMacro);
        invertMacroSlider->setPrecision(1);
        invertMacroSlider->setValue(invertMacro);
        
        rippleFolder=gui->addFolder("Ripple");
        rippleMacroSlider = rippleFolder->addSlider("Ripple", 0.0,1.0,rippleMacro);
        rippleSyncToggle=rippleFolder->addToggle("Sync");
        rippleXSlider=rippleFolder->addSlider("X", 0.0, 1.0,rippleX);
        rippleYSlider=rippleFolder->addSlider("Y", 0.0, 1.0,rippleY);
        rippleRateSlider=rippleFolder->addSlider("Rate",0.1, 300.00,rippleRate);
        
        filterFolder=gui->addFolder("Filter");
        filterMacroSlider=filterFolder->addSlider("Filter",0.0,1.0,filterMacro);
        filterRedSlider=filterFolder->addSlider("Red", 0.0,1.0,filterRed);
        filterGreenSlider=filterFolder->addSlider("Green",0.0,1.0,filterGreen);
        filterBlueSlider=filterFolder->addSlider("Blue",0.0,1.0,filterBlue);
        //        filterAlphaSlider=filterFolder->addSlider("Alpha",0.0,1.0,filterAlpha);
        
        kaleidioscopeFolder=gui->addFolder("Kaleidioscope");
        kaleidoscopeMacroSlider=kaleidioscopeFolder->addSlider("Kaleidoscope",0.0,1.0,kaleidoscopeMacro);
        kaleidoscopeSectorSlider=kaleidioscopeFolder->addSlider("Sectors", 1.0, 100,kaleidioscopeSectors);
        kaleidoscopeAngleSlider=kaleidioscopeFolder->addSlider("Angle",-180,180,kaleidioscopeAngle);
        kaleidoscopeXSlider=kaleidioscopeFolder->addSlider("X",0.0,1.0,kaleiodioscopeX);
        kaleidoscopeYSlider=kaleidioscopeFolder->addSlider("Y",0.0,1.0,kaleiodioscopeY);
        
        pixelateFolder=gui->addFolder("Pixelate");
        pixelateMacroSlider=pixelateFolder->addSlider("Pixelate", 0, 100, pixelateMacro);
        
        fullhouseFolder=gui->addFolder("Fullhouse");
        fullhouseMacroSlider=fullhouseFolder->addSlider("Fullhouse", 1, 50, fullhouseMacro);
        
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
        
        squareioscopeFolder=gui->addFolder("Squareioscope");
        squareioscopeMacroSlider=squareioscopeFolder->addSlider("Squareioscope", 0.0, 1.0, squareioscopeMacro);
        squareioscopeMacro2Slider=squareioscopeFolder->addSlider("Squareioscope2", 0.0, 1.0,squareioscopeMacro);
        
        vhsFolder=gui->addFolder("VHS");
        vhsMacroSlider=vhsFolder->addSlider("VHS",0.0, 1.0, vhsMacro);
        vhsStrengthSlider=vhsFolder->addSlider("Strength",0.0,1.0,vhsStrength);
        vhsSpeedSlider=vhsFolder->addSlider("Speed",0,60,vhsSpeed);
        
        gui->addBreak();
        gui->addBreak();
        
        gui->onButtonEvent(this, &ofApp::onButtonEvent);
        gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
        gui->onSliderEvent(this, &ofApp::onSliderEvent);
        gui->onToggleEvent(this, &ofApp::onToggleEvent);
        gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
//        gui->onNumberBoxChangedEvent(this, &ofApp::onNumberBoxChangedEventGui1);
        
        gui2->onToggleEvent(this, &ofApp::onToggleEvent);
        gui2->onButtonEvent(this, &ofApp::onButtonEventGui2);
        kaleidioscopeFolder->onSliderEvent(this, &ofApp::onSliderEvent);
        cout<<"here 3"<<endl;
        
//        gui3->onToggleEvent(this, &ofApp::onToggleEventGui3);
//        gui3->onButtonEvent(this, &ofApp::onButtonEventGui3);
//        gui3->onRightClickEvent(this, &ofApp::onRightClickEventGui3);
//
//        mAddNewLight.addListener(this, &ofApp::onAddNewLightEventGui3);
//        submitButton=mAddNewLight.getButton(0);
//        cancelButton=mAddNewLight.getButton(1);
        
//        mEditLight.addListener(this, &ofApp::onEditLightEventGui3);
//        gui3->onTextInputEvent(this, &ofApp::onTextInputEventGui3);
        
       gui3->onSliderEvent(this, &ofApp::onSliderEventGui3);
        for(int i=0;i<numberOfLights;i++)
        {
            lightValues[i]=0;
            lightValues[i].set("Ch."+ofToString(i+1),0,0,255);
            gui3->addSlider(lightValues[i]);
            lightSliders[i] = new ofxDatGuiSlider(lightValues[i]);
            lightSliders[i]->onSliderEvent(this,&ofApp::onSliderEventGui3);
        }
        
        
        
        tempoDivisionSlider->setPrecision(0);
        videoDivisionSlider->setPrecision(0);
        kaleidoscopeSectorSlider->setPrecision(0);
        zebraLevelsSlider->setPrecision(0);
        pixelateMacroSlider->setPrecision(0);
        fullhouseMacroSlider->setPrecision(0);
        tripletToggle->setChecked(triplet);
        clearToggle->setChecked(clear);
        videoSyncToggle->setChecked(videoSync);
        rippleSyncToggle->setChecked(rippleSync);
        cout<<"here 4"<<endl;
        ofxDatGuiThemeSpectacle *theme = new ofxDatGuiThemeSpectacle;
        ofxDatGuiThemeSpectacle *flip = new ofxDatGuiThemeSpectacle;
        
        gui->setTheme(theme);
        gui2->setTheme(flip);
        gui3->setTheme(theme);
        gui->draw();
        gui2->draw();
        gui3->draw();
        //        ofBackground(theme->color.guiBackground);
        ofBackground(ofColor::black);
        //        mAddNewLight.setTheme(theme);
        //        mEditLight.setTheme(theme);
        if(ofLoadImage( font, "font.jpg" ))cout<<"font loaded"<<endl;
        else cout<<"font not loaded"<<endl;
    }
    snaves=1;
}

ofPoint ofApp::windowSize()//============================================================
{
    ofPoint Windowsize(gui->getWidth()+gui2->getWidth()+gui3->getWidth()+gui->getPadding()*2,gui->getHeight());
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
    
    //dmx update:
    for(int i=0;i<numberOfLights;i++)
    {
        lightSliders[i]->update();
        dmx.setLevel(i+1, lightValues[i]);
    }
    
    if(dmx.isConnected())
    {
        dmx.update();
    }
}

void ofApp::draw()//============================================================
{
    if(gui2->mEnabled)
    {
//        cout<<"uh oh gui2 \n";
    }
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
    
    if(chromaKeyVideo!= -1)player[chromaKeyVideo].draw(0, 0, getWidth, getHeight);
    
    chromaKeyVideoFbo.end();
    
    //-----Chroma Key FX FBO/Shader--------------------------------------------------
    
    chromaKeyFxFbo.begin();
    ofClear(0,0,0,0);
    
    chromaKeyShader.begin();
    
    shader.setUniform1f("fxMacro", fxMacro);
    chromaKeyShader.setUniform1f("chromaKeyMacro", chromaKeyMacro);
    chromaKeyShader.setUniform3f("chromaKeyColor", chromaKeyColor.getRedFloat(),chromaKeyColor.getGreenFloat(),chromaKeyColor.getBlueFloat());
    chromaKeyShader.setUniform1f("chromaKeyThreshold",chromaKeyThreshold);
    
    chromaKeyVideoFbo.draw(0,0,getWidth,getHeight);
    
    chromaKeyShader.end();
    chromaKeyFxFbo.end();
    
    //-------Blend FBO---------------------------------------------------------
    
    blendFbo.begin();
    ofClear(0,0,0,0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    ofDisableSmoothing();
    fbo.draw(0,0, getWidth, getHeight);             //first FBO draw
    chromaKeyFxFbo.draw(0,0,getWidth,getHeight);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    blendFbo.end();
    
    
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
    
    shader.setUniform1i("ksectors", int(kaleidioscopeSectors*kaleidoscopeMacro));
    shader.setUniform2f("kcenter", kaleiodioscopeX*kaleidoscopeMacro,kaleiodioscopeY*kaleidoscopeMacro);
    shader.setUniform1f("kaleidoscopeMacro", kaleidoscopeMacro);
    shader.setUniform1f("kangleRad", (ofDegToRad(kaleidioscopeAngle))*kaleidoscopeMacro);
    if(kaleidoscopeMacro<.5) shader.setUniform2f("screenCenter",0,0);
    else shader.setUniform2f("screenCenter",0.5*getWidth,0.5*getHeight);
    
    shader.setUniform1i("pixelateMacro", int(pixelateMacro));
    
    shader.setUniform1i("fullhouseMacro", int(fullhouseMacro));
    
    shader.setUniform1f("rotateMacro", rotateMacro);
    shader.setUniform2f("rotateScreenCenter",0.5*getWidth,0.5*getHeight);
    
    shader.setUniform1f("zebraMacro", zebraMacro);
    shader.setUniform1f("zebraSpeed", zebraSpeed);
    shader.setUniform1i("zebraLevels", zebraLevels);
    
    shader.setUniform1f("squareioscopeMacro", squareioscopeMacro);
    shader.setUniform1f("squareioscopeMacro2", squareioscopeMacro2);
    
    shader.setUniform1f("vhsMacro", vhsMacro);
    shader.setUniform1f("vhsStrength", vhsStrength);
    shader.setUniform1f("vhsSpeed", vhsSpeed);
    //    fbo.draw(0,0, getWidth, getHeight);             //first FBO draw
    //    chromaKeyFxFbo.draw(0,0,getWidth,getHeight);
    blendFbo.draw(0,0,getWidth, getHeight);
    shader.end();                                   //shader1 end
    fbo2.end();                                     // FBO 2 end
    
    //    fbo2.draw(0,0,getWidth,getHeight);
    
    //------ASCII FBO/Shader---------------------------------------------------------------------------------------------
    fbo3.begin();                                   //FBO 3 begin
    ofClear(0,0,0,0);
    asciiShader.begin();                            //ASCII Shader begin
    
    asciiShader.setUniform1f("fxMacro", fxMacro);
    asciiShader.setUniform1f("asciiMacro", asciiMacro);
    asciiShader.setUniform1f("asciiDotDistance", asciiDotDistance);
    asciiShader.setUniform1f("asciiImageGain", asciiImageGain);
    asciiShader.setUniform1f("asciiImageContrast", asciiImageContrast);
    asciiShader.setUniform1i("asciiInvert", int(asciiInvert));
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
        
        if(msg.status==MIDI_NOTE_ON && msg.pitch>=0 && msg.pitch<=99)
        {
            midiMessages.push_back(msg); // add the latest message to the message queue
            while(midiMessages.size() > maxMessages) midiMessages.erase(midiMessages.begin());
            
            pitch=(msg.pitch);
            
            if(sustain)midiNoteOff(msg.pitch);
            
            if(msg.pitch>=0 && msg.pitch<=99)
            {
                playerFromMidiMessage=(msg.pitch);
                videoCount+=1;
                //                                                                        cout<<"videoCount: "<<videoCount<<endl;
                player[playerFromMidiMessage].opacity=msg.velocity;
                player[playerFromMidiMessage].drawImage=true;
                player[playerFromMidiMessage].opacity=ofMap(player[playerFromMidiMessage].opacity, 0, 127, 0, 255);
            }
        }
        
        if(msg.status==MIDI_NOTE_OFF && msg.pitch>=0 && msg.pitch<=99)
        {
            if(sustain==false)
            {
                midiNoteOff(msg.pitch);
                videoCount=-1;
            }
        }
        
        if(msg.status==MIDI_CONTROL_CHANGE)         //MIDI CC (FX) change cases
        {
            if(msg.channel==1)
            {
            switch(msg.control)
            {
                case 123:
                {
                    for(int i=0;i<max_videos;i++) midiNoteOff(i);
                }
                    break;
                case 64: if(msg.value>63)sustain=true; else sustain=false;
                    
//                default:
//                    for(int i=0;i<fxByCC[msg.control].size();i++)
//                    {
//                        fxByCC[msg.control][i]->onMidiMessage(msg.value);
//                    }
                case 15:
                    fxMacro=ofMap(msg.value,0,127,0.0,1.0);
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
                    
                    //EMPTY CASE 22
                    
                case 24:
                    invertMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                    invertMacroSlider->setValue(invertMacro);
                    break;
                case 25:
                    rippleMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                    rippleMacroSlider->setValue(rippleMacro);
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
                    filterMacroSlider->setValue(filterMacro);
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
                    kaleidoscopeMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                    kaleidoscopeMacroSlider->setValue(kaleidoscopeMacro);
                    break;
                case 41:
                    kaleidioscopeAngle=ofMap(msg.value,0, 127, -180.0, 180.0);
                    kaleidoscopeAngleSlider->setValue(kaleidioscopeAngle);
                    break;
                case 42:
                    kaleiodioscopeX=ofMap(msg.value,0, 127, 0.0, 1.00);
                    kaleidoscopeXSlider->setValue(kaleiodioscopeX);
                    break;
                case 43:
                    kaleiodioscopeY=ofMap(msg.value,0, 127, 0.0, 1.00);
                    kaleidoscopeYSlider->setValue(kaleiodioscopeY);
                    break;
                case 44:
                    kaleidioscopeSectors=ofMap(msg.value, 0, 127, 1.0, 100.0);
                    kaleidoscopeSectorSlider->setValue(kaleidioscopeSectors);
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
                    pixelateMacroSlider->setValue(pixelateMacro);
                    break;
                case 57:
                    fullhouseMacro=ofMap(msg.value,0,127,1,50);
                    fullhouseMacroSlider->setValue(fullhouseMacro);
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
                    chromaKeyColor.setRed(ofMap(msg.value,0,127,0,255));
                    chromaKeyColorPicker->setColor(ofColor(chromaKeyColor.getRedInt(),chromaKeyColor.getGreenInt(),chromaKeyColor.getBlueInt()));
                    break;
                case 80:
                    chromaKeyGreen=ofMap(msg.value,0,127,0,255);
                    chromaKeyColorPicker->setColor(ofColor(chromaKeyColor.getRedInt(),chromaKeyColor.getGreenInt(),chromaKeyColor.getBlueInt()));
                    break;
                case 81:
                    chromaKeyBlue=ofMap(msg.value,0,127,0,255);
                    chromaKeyColorPicker->setColor(ofColor(chromaKeyColor.getRedInt(),chromaKeyColor.getGreenInt(),chromaKeyColor.getBlueInt()));
                    break;
                case 82:
                    chromaKeyThreshold=ofMap(msg.value, 0, 127, 0, 255);
                    chromaKeyThresholdSlider->setValue(chromaKeyThreshold);
                    break;
                case 83:
                    squareioscopeMacro=ofMap(msg.value, 0, 127, 0.0, 1.0);
                    squareioscopeMacroSlider->setValue(squareioscopeMacro);
                    break;
                case 84:
                    squareioscopeMacro2=ofMap(msg.value, 0, 127, 0.0, 1.0);
                    squareioscopeMacroSlider->setValue(squareioscopeMacro2);
                    break;
                case 85:
                    vhsMacro=ofMap(msg.value, 0, 127, 0.0, 1.0);
                    vhsMacroSlider->setValue(vhsMacro);
                    break;
                case 86:
                    vhsStrength=ofMap(msg.value,0,127,0.0,1.0);
                    vhsStrengthSlider->setValue(vhsStrength);
                    break;
                case 87:
                    vhsSpeed=ofMap(msg.value, 0, 127, 0.0, 60.0);
                    vhsSpeedSlider->setValue(vhsSpeed);
                    break;
            }
        }
            if(msg.channel==2)lightValues[msg.control-1]=ofMap(msg.value, 0, 127, 0, 255);
        }
        
        if (msg.status==MIDI_TIME_CLOCK && snaves==1)
           {
               if (triplet==0) if(tempoCount%(tempoDivisionValues[tempoDivision])==0) (tempo=!tempo);
               if(triplet==1) if(tempoCount%(tempoDivisionValuesTriplet[tempoDivision])==0) (tempo=!tempo);
               tempoCount=tempoCount+1;
               bpm+=(clock.getBpm()-bpm)/5;
           }
    }
}
void ofApp::midiNoteOff(int pitch)//============================================================
{
    player[pitch].drawImage=false;
    player[pitch].stop();
    player[pitch].firstFrame();
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
    //                                                                    cout<<"key pressed\n"<<endl;
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
        currentlyDrawing+=1;
    }
    
}
//--------------------------------------------------------------
void ofApp::keyReleased(ofKeyEventArgs & args)//============================================================
{
    //  cout<<"Key Released"<<endl;
    int key=args.key;
    
    switch (key)
    {
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
        currentlyDrawing-=1;
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

//void ofApp::onAddNewLightEventGui3(ofxModalEvent e)//==========================
//{
//    if(e.type==ofxModalEvent::SHOWN) disableGuis();
//    else if(e.type==ofxModalEvent::HIDDEN) enableGuis();
//    else if (e.type == ofxModalEvent::CANCEL) cout << "cancel button was selected" << endl;
//    else if (e.type == ofxModalEvent::CONFIRM)
//    {
//        cout << "confirm button was selected" << endl;
//        addLight();
//    }
//}

//void ofApp::onRightClickEventGui3(ofxDatGuiRightClickEvent e)//============================================================
//{
//    if(e.target->getType()==ofxDatGuiType::FOLDER)
//    {
//        int l = e.target->getCount();
//        mEditLight.show(&(rig[e.target->getCount()-1]));  //show editLight dialog according to count of the target
//        cout<<"right click:"+e.target->getLabel()+"\n";
//    }
//}



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

//void ofApp::onButtonEventGui3(ofxDatGuiButtonEvent e)//============================================================
//{
//    if(e.target==addLightButton)
//    {
//
//        mAddNewLight.show(getRigSize(), getRigSize()); //clear light first and pass in midiCCStart and dmxChannelStart
//    }
//}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)//============================================================
{//--------Toggle event handler function----------------------
    
    if (e.target==tripletToggle)triplet=!triplet;
    //    else if (e.target->is("Background Switch"))backgroundSwitch=!backgroundSwitch;
    else if (e.target==videoSyncToggle)videoSync=!videoSync;
    
    else if(e.target==clearToggle) clear=!clear;
    
    else if(e.target==rippleSyncToggle)rippleRate=bpm*60;
    
    else if(e.target==asciiInvertToggle)asciiInvert=!asciiInvert;
}

//void ofApp::onToggleEventGui3(ofxDatGuiToggleEvent e)//============================================================
//{
//    int toggle = e.target->getCount();
//}

void ofApp::onTextInputEventGui1(ofxDatGuiTextInputEvent e)
{
    if(e.type==ofxDatGuiNumberBoxEventType::NUMBERBOX)
    {   cout<<"NumberBox event \n";
    }
}

//void ofApp::onTextInputEventGui3(ofxDatGuiTextInputEvent e)
//{
//    if(e.type==ofxDatGuiNumberBoxEventType::NUMBERBOX)cout<<"Text input number event\n";
//    if(e.type==ofxDatGuiNumberBoxEventType::INPUT) cout<<"mInput event \n";
//}

//void ofApp::onNumberBoxChangedEventGui1(ofxDatGuiNumberBoxChangedEvent e)
//{
//    cout<<"Evan's fun NumberBox Event \n";
//
//    if(!validMidiCC(e.value))
//    {
//        e.target->setNumberBoxNumber(e.target->getPreviousNumberBoxNumber());
//        return;
//    }
//    removeFxParameter(e.target, e.target->getPreviousNumberBoxNumber());
//    fxByCC[e.target->getNumberBoxNumber()].push_back(getFxParameter(e.target));
//}

//ofApp::fxParameter* ofApp::getFxParameter(ofxDatGuiComponent *e)
//{
//    for(int i=0;i<127;i++)
//    {
//        for(int j=0;j<fxByCC[i].size();j++)
//        {
//            if(fxByCC[i][j]->compare(e))return fxByCC[i][j];
//        }
//    }
//    cout<<"No cooresponding fx found\n";
//    return 0;
//}

bool ofApp::validMidiCC(int cc)
{
    if(cc>=0 && cc<=127)
    {
        if(cc==64 || cc==123) return false;
        else return true;
    }
    else ofSystemAlertDialog("Invalid MIDI CC number entered");
    return false;
}
//
//void ofApp::removeFxParameter(ofxDatGuiComponent *e, int previous)
//{
//    for(int i=0;i<fxByCC[previous].size();i++)
//    {
//        if(fxByCC[previous][i]->compare(e))
//        {
//            fxByCC[previous].erase(fxByCC[previous].begin()+i);
//            cout<<"fxParameter added at "<<previous<< ", "<< i <<"\n";
//        }
//    }
//}



void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)//============================================================
{
    //--------Slider event handler function----------------------
    
    if(e.target==tempoDivisionSlider)tempoDivision=(e.target->getValue());
    if(e.target==videoDivisionSlider)videoDivision=(e.target->getValue());
    if(e.target==fxMacroSlider)fxMacro=(e.target->getValue());
    
    else if(e.target==kaleidoscopeMacroSlider)kaleidoscopeMacro = (e.target->getValue());
    else if(e.target==kaleidoscopeAngleSlider)kaleidioscopeAngle=(e.target->getValue());
    else if(e.target==kaleidoscopeXSlider)kaleiodioscopeX=(e.target->getValue());
    else if(e.target==kaleidoscopeYSlider)kaleiodioscopeY=(e.target->getValue());
    else if(e.target==kaleidoscopeSectorSlider)kaleidioscopeSectors=(e.target->getValue());
    
    else if(e.target==filterMacroSlider)filterMacro=(e.target->getValue());
    else if(e.target==filterRedSlider)filterRed=(e.target->getValue());
    else if(e.target==filterBlueSlider)filterBlue=(e.target->getValue());
    else if(e.target==filterGreenSlider)filterGreen=(e.target->getValue());
    
    else if(e.target==rippleMacroSlider)rippleMacro=(e.target->getValue());
    else if(e.target==rippleYSlider)rippleY=(e.target->getValue());
    else if(e.target==rippleXSlider)rippleX=(e.target->getValue());
    else if(e.target==rippleRateSlider)
    {
        if(rippleSync==false)rippleRate=(e.target->getValue());
    }
    
    else if(e.target==invertMacroSlider)invertMacro=(e.target->getValue());
    
    else if(e.target==videoSpeedSlider)videoSpeed2=(e.target->getValue());
    
    else if(e.target==pixelateMacroSlider)pixelateMacro=(e.target->getValue());
    
    else if(e.target==fullhouseMacroSlider)fullhouseMacro=(e.target->getValue());
    
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
    
    else if(e.target==squareioscopeMacroSlider)squareioscopeMacro=(e.target->getValue());
    else if(e.target==squareioscopeMacro2Slider)squareioscopeMacro2=(e.target->getValue());
    
    else if(e.target==vhsMacroSlider)vhsMacro=(e.target->getValue());
    else if(e.target==vhsStrengthSlider)vhsStrength=(e.target->getValue());
    else if(e.target==vhsSpeedSlider)vhsSpeed=(e.target->getValue());
    
    else if(e.target==chromaKeyMacroSlider)chromaKeyMacro=(e.target->getValue());
    else if(e.target==chromaKeyThresholdSlider)chromaKeyThreshold=(e.target->getValue());
}

void ofApp::onSliderEventGui3(ofxDatGuiSliderEvent e)//============================================================
{
//    cout<<"gui3 slider \n";
//    lightValues[ofToInt(e.target->getLabel())-1]=(e.target->getValue());
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
        chromaKeyColor.setColor((e.target->getColor()).r,e.target->getColor().g,e.target->getColor().b);
    }
}

//void ofApp::onEditLightEventGui3(ofxModalEvent e)//============================================================
//{
//    if(e.type==ofxModalEvent::SHOWN) disableGuis();
//    if(e.type==ofxModalEvent::HIDDEN) enableGuis();
////    if(e.type==ofxModalEvent::DELETE)
////    {
////        light* a=mEditLight.getLight();
////        deleteLight(a);
////    }
//    cout<<"DELETE event sent \n";
//    if (e.type == ofxModalEvent::CANCEL) cout << "cancel button was selected" << endl;
//    else if (e.type == ofxModalEvent::CONFIRM)
//    {
//        cout << "confirm button was selected" << endl;
//        editLight();
//    }
//}

void ofApp::enableGuis()
{
    gui->setEnabled(true);
    gui2->setEnabled(true);
    gui3->setEnabled(true);
}

void ofApp::disableGuis()
{
    gui->setEnabled(false);
    gui2->setEnabled(false);
    gui3->setEnabled(false);
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
            invertMacroSlider->setValue(invertMacro);
            
            rippleMacro=xmlSettings.getValue("xmlSettings:ripple:ripple", 0);
            rippleMacroSlider->setValue(rippleMacro);
            rippleSync=xmlSettings.getValue("xmlSettings:ripple:sync", 0);
            rippleSyncToggle->setChecked(rippleSync);
            rippleX=xmlSettings.getValue("xmlSettings:ripple:x", 0.0);
            rippleXSlider->setValue(rippleX);
            rippleY=xmlSettings.getValue("xmlSettings:ripple:y", 0.0);
            rippleYSlider->setValue(rippleY);
            rippleRate=xmlSettings.getValue("xmlSettings:ripple:rate",60);
            rippleRateSlider->setValue(rippleRate);
            
            filterMacro=xmlSettings.getValue("xmlSettings:filter:filter", 0.0);
            filterMacroSlider->setValue(filterMacro);
            filterRed=xmlSettings.getValue("xmlSettings:filter:red",0.0);
            filterRedSlider->setValue(filterRed);
            filterGreen=xmlSettings.getValue("xmlSettings:filter:green",0.0);
            filterGreenSlider->setValue(filterGreen);
            filterBlue=xmlSettings.getValue("xmlSettings:filter:blue",0.0);
            filterBlueSlider->setValue(filterBlue);
            
            kaleidoscopeMacro=xmlSettings.getValue("xmlSettings:kaleidoscope:kaleidoscope", 0.0);
            kaleidoscopeMacroSlider->setValue(kaleidoscopeMacro);
            kaleidioscopeSectors=xmlSettings.getValue("xmlSettings:kaleidoscope:sectors", 1);
            kaleidoscopeSectorSlider->setValue(kaleidioscopeSectors);
            kaleidioscopeAngle=xmlSettings.getValue("xmlSettings:kaleidoscope:angle", 0.0);
            kaleidoscopeAngleSlider->setValue(kaleidioscopeAngle);
            kaleiodioscopeX=xmlSettings.getValue("xmlSettings:kaleidoscope:x", 0.0);
            kaleidoscopeXSlider->setValue(kaleiodioscopeX);
            kaleiodioscopeY=xmlSettings.getValue("xmlSettings:kaleidoscope:y", 0.0);
            kaleidoscopeYSlider->setValue(kaleiodioscopeY);
            
            pixelateMacro=xmlSettings.getValue("xmlSettings:pixelate:pixelate", 0.0);
            pixelateMacroSlider->setValue(pixelateMacro);
            
            fullhouseMacro=xmlSettings.getValue("xmlSettings:fullhouse:fullhouse", 0.0);
            fullhouseMacroSlider->setValue(fullhouseMacro);
            
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
            rotateMacroSlider->setValue(rotateMacro);
            
            zebraMacro=xmlSettings.getValue("xmlSettings:zebra:zebraMacro", 0.0);
            zebraMacroSlider->setValue(zebraMacro);
            zebraSpeed=xmlSettings.getValue("xmlSettings:zebra:zebraSpeed", 0.0);
            zebraSpeedSlider->setValue(zebraSpeed);
            zebraLevels=xmlSettings.getValue("xmlSettings:zebra:zebraLevels", 2);
            zebraLevelsSlider->setValue(zebraLevels);
            
            squareioscopeMacro=xmlSettings.getValue("xmlSettings:square:squareioscope", 0.0);
            squareioscopeMacroSlider->setValue(squareioscopeMacro);
            squareioscopeMacro2=xmlSettings.getValue("xmlSettings:square:squareioscope2", 0.0);
            squareioscopeMacro2Slider->setValue(squareioscopeMacro2);
            
            vhsMacro=xmlSettings.getValue("xmlSettings:vhs:vhsMacro",0.0);
            vhsMacroSlider->setValue(vhsMacro);
            vhsSpeed=xmlSettings.getValue("xmlSettings:vhs:vhsSpeed",0.0);
            vhsSpeedSlider->setValue(vhsSpeed);
            vhsStrength=xmlSettings.getValue("xmlSettings:vhs:vhsStrength", 0.0);
            vhsStrengthSlider->setValue(vhsStrength);
            
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
        
        xmlSettings.setValue("xmlSettings:fxWet:fxWet", fxMacro);
        xmlSettings.setValue("xmlSettings:video:speed", videoSpeed);
        xmlSettings.setValue("xmlSettings:video:sync", videoSync);
        xmlSettings.setValue("xmlSettings:video:division", videoDivision);
        xmlSettings.setValue("xmlSettings:video:triplet", triplet);
        xmlSettings.setValue("xmlSettings:video:tempoDivision", tempoDivision);
        
        xmlSettings.setValue("xmlSettings:color:bgColor1Red", bgColor1Red);
        xmlSettings.setValue("xmlSettings:color:bgColor1Green", bgColor1Green);
        xmlSettings.setValue("xmlSettings:color:bgColor1Blue", bgColor1Blue);
        
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
        
        xmlSettings.setValue("xmlSettings:square:squareioscope", squareioscopeMacro);
        xmlSettings.setValue("xmlSettings:square:squareioscope2", squareioscopeMacro2);
        
        xmlSettings.setValue("xmlSettings:vhs:vhsMacro", vhsMacro);
        xmlSettings.setValue("xmlSettings:vhs:vhsStrength", vhsStrength);
        xmlSettings.setValue("xmlSettings:vhs:vhsSpeed",vhsSpeed);
        
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
    chromaKeyVideoFbo.allocate(w,h);
    chromaKeyFxFbo.allocate(w,h);
    blendFbo.allocate(w, h);
    fbo2.allocate(w,h);
    fbo3.allocate(w,h);
    fbo4.allocate(w,h);
}

//void ofApp::deleteLight(light* a)//============================================================
//{
//    gui3->removeFolder(a->name);    //to delete the components inside of the folder
//    rig.erase(rig.begin()+a->count-1);  //delete light from the rig
//
//}

//int ofApp::getRigSize()//============================================================
//{
//    int rigSize=0;
//    for(int i=0;i<rig.size();i+=1)
//    {
//        rigSize+=rig[i].values.size();
//    }
//    return rigSize;
//}

//void ofApp::addLight()//============================================================
//{
//    light *a = mAddNewLight.getLight();
//    ofxDatGuiFolder *folder=gui3->addFolder(a->name);
//    ofxDatGuiSlider *newSlider;
//    for(int i=0;i<a->values.size();i+=1)
//    {
//        a->values[i].value.set(a->values[i].parameter,120,0,255);
//        newSlider=folder->addSlider(a->values[i].value,true,a->dmxChannelStart+i);
//        newSlider->setLabel(a->values[i].parameter);
//    }
//    runningCount=runningCount+1;
//    a->count=runningCount;
//    folder->setCount(runningCount);
//    rig.push_back(*a);
//}

//void ofApp::editLight()//============================================================
//{
//    light *a = mEditLight.getLight();
//    int existingSliders=rig[a->count-1].numberOfChannels;
//    rig[a->count-1] = *a;
//    ofxDatGuiFolder *folder = gui3->getFolder(a->name);
//    for(int i=0;i<existingSliders;i++)
//    {
//        folder->children.pop_back();
//    }
//    for(int i=0;i<a->values.size();i+=1)
//    {
//        a->values[i].value.set(a->values[i].parameter,120,0,255);
//        folder->addSlider(a->values[i].value,true,a->values[i].dmxChannel);
//    }
//    a->count=runningCount;
//    folder->setCount(runningCount);
//    folder->collapse();
//}

