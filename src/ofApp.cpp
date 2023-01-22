#pragma once

#include "ofApp.h"
#include "ofMain.h"


int snaves=0;

void ofApp::setup()//============================================================
{
    if(snaves==0)ofSetDataPathRoot(ofFilePath::getCurrentExeDir()+"../Resources/data/");
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
    ofSetEscapeQuitsApp(false);
    
    if(snaves==0)
    {
    if(fxShader.load("fxShader.vert","fxShader.frag"))cout<<"mainShader loaded"; else cout<<"mainShader not loaded";
    if(asciiShader.load("asciiShader.vert","asciiShader.frag")) cout<<"asciiShader loaded"; else cout<<"asciiShader not loaded";
    if(ledShader.load("ledShader.vert","ledShader.frag"))cout<<"ledShader loaded"; else cout<<"ledShader not loaded";
    
    if(snaves==0) allocateFBOs();
    
    ofLog()<<(char*)glGetString(GL_VERSION);
    
    dmx.connect(0,numberOfLights);
    
    //---MIDI Setup-----------------------------------------------------
    ofSetVerticalSync(true);
    midiIn.listInPorts(); // print input ports
    midiIn.openPort(0);
    midiIn.ignoreTypes(false, false, true); // don't ignore sysex, timing, & active sense messages, these are ignored by default
    midiIn.addListener(this);   // add ofApp as a listener
    videoCount=0;
}
    //--GUI setup--------------------------------------------------
    
    if(snaves==0)
    {
        gui = new ofxDatGui(ofxDatGuiAnchor::NO_ANCHOR);
        
        gui->setAutoDraw(true);
        gui2 = new ofxDatGui(ofxDatGuiAnchor::NO_ANCHOR);
        gui2->setAutoDraw(true);
        gui3 = new ofxDatGui(ofxDatGuiAnchor::NO_ANCHOR);
        gui3->setAutoDraw(true);
        theme = new ofxDatGuiThemeSpectacle;
        flip = new ofxDatGuiThemeSpectacle;
        
        gui->setTheme(theme);
        gui2->setTheme(flip);
        gui3->setTheme(theme);
        
        savePath = "";
        saveName = "default.xml";
        
        clear=false;
        command=false;
        
        videosLabel = gui2->addLabel("VIDEOS");
//        videosLabel->setNumberbox(false);
        videosLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
        videosLabel->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::FILM);
        videosLabel->setIconAlignment(ofxDatGuiAlignment::RIGHT);
        
        clearToggle = gui2->addToggle("Clear");
        clearAllButton = gui2->addButton("Clear All Videos");
        clearAllButton->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::TRASHCAN);
        
        gui2->setPosition(gui->getWidth()+gui->getPadding()*2,0);
        gui2->addBreak();
        gui2->addBreak();
        
        gui3->setPosition(gui->getWidth()+gui->getPadding()*2+gui2->getWidth() + gui->getPadding()*2, 0);
        
        for(int i=0;i<max_videos;i++)
        {
            videoButtons[i]=(gui2->addButton(ofToString(i+1), true));
            videoButtons[i]->setNumberbox(noteNames[i]);
        }
        
        newProjectButton = gui->addButton("New Project");
        newProjectButton->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::PAGE);
        newProjectButton->setIconAlignment(ofxDatGuiAlignment::CENTER);
        saveButton = gui->addButton("Save");
        saveButton->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::FLOPPY);
        saveButton->setIconAlignment(ofxDatGuiAlignment::CENTER);
        loadButton = gui->addButton("Load");
        loadButton->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::FOLDER);
        loadButton->setIconAlignment(ofxDatGuiAlignment::CENTER);
        refreshMidiButton = gui->addButton("Refresh MIDI Ports");
        refreshMidiButton->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::REFRESH);
        refreshMidiButton->setIconAlignment(ofxDatGuiAlignment::CENTER);
        midiDropdown = gui->addDropdown("MIDI Port:", midiIn.getInPortList());
        midiDropdown->setDropdownDividers(false);
        refreshUsbButton = gui->addButton("Refresh USB Ports");
        refreshUsbButton->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::REFRESH);
        refreshUsbButton->setIconAlignment(ofxDatGuiAlignment::CENTER);
        usbDropdown = gui->addDropdown("Dmx Interface:", dmx.getDevices());
        usbDropdown->setDropdownDividers(false);
        
        clearAllLightsButton = gui3->addButton("Clear All Lights");
        clearAllLightsButton->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::TRASHCAN);
//        clearAllLightsButton->setIconAlignment(ofxDatGuiAlignment::RIGHT);
        lightsLabel = gui3->addLabel("LIGHTS");
        lightsLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
//        lightsLabel->setNumberbox(false);
        lightsLabel->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::LIGHTBULB);
        
        bgColor1ColorPicker=gui->addColorPicker("BG Color 1",bgColor1);
//        bgColor1ColorPicker->setNumberbox(false);

        fxLabel = gui->addLabel("FX");
        fxLabel->setLabelAlignment(ofxDatGuiAlignment::CENTER);
//        fxLabel->setNumberbox(false);
        fxLabel->setIcon(ofxDatGuiComponent::ofxDatGuiIconType::EYE);
        
        fxMacroSlider = gui->addSlider("FX DRY/WET",0.0,1.0,fxMacro,false,false);
        
        videoFolder=gui->addFolder("VIDEO CONTROLS");
        videoSpeedSlider=videoFolder->addSlider("Video Speed",0.1,10.0,videoSpeed2);
        videoDivisionSlider=videoFolder->addSlider("Video Division",1,8,1);
        videoSyncToggle=videoFolder->addToggle("Video Sync");
        videoFolder->setDropdownDividers(false);
        
        invertFolder=gui->addFolder("INVERT");
        invertMacroSlider=invertFolder->addSlider("Invert", 0.0, 1.0, invertMacro);
        invertMacroSlider->setPrecision(1);
        invertMacroSlider->setValue(invertMacro);
        invertFolder->setDropdownDividers(false);
        
        rippleFolder=gui->addFolder("RIPPLE");
        rippleMacroSlider = rippleFolder->addSlider("Ripple", 0.0,1.0,rippleMacro);
        rippleSyncToggle=rippleFolder->addToggle("Sync");
        rippleXSlider=rippleFolder->addSlider("X", 0.0, 1.0,rippleX);
        rippleYSlider=rippleFolder->addSlider("Y", 0.0, 1.0,rippleY);
        rippleRateSlider=rippleFolder->addSlider("Rate",0.1, 300.00,rippleRate);
        rippleFolder->setDropdownDividers(false);
        
        filterFolder=gui->addFolder("FILTER");
        filterMacroSlider=filterFolder->addSlider("Filter",0.0,1.0,filterMacro);
        filterRedSlider=filterFolder->addSlider("Red", 0.0,1.0,filterRed);
        filterGreenSlider=filterFolder->addSlider("Green",0.0,1.0,filterGreen);
        filterBlueSlider=filterFolder->addSlider("Blue",0.0,1.0,filterBlue);
        filterFolder->setDropdownDividers(false);
        
        kaleidoscopeFolder=gui->addFolder("KALEIDOSCOPE");
        kaleidoscopeMacroSlider=kaleidoscopeFolder->addSlider("Kaleidoscope",0.0,1.0,kaleidoscopeMacro);
        kaleidoscopeAngleSlider=kaleidoscopeFolder->addSlider("Angle",-180,180,kaleidoscopeAngle);
        kaleidoscopeXSlider=kaleidoscopeFolder->addSlider("X",0.0,1.0,kaleiodioscopeX);
        kaleidoscopeYSlider=kaleidoscopeFolder->addSlider("Y",0.0,1.0,kaleiodioscopeY);
        kaleidoscopeSectorSlider=kaleidoscopeFolder->addSlider("Sectors", 1, 100,kaleidoscopeSectors);
        kaleidoscopeFolder->setDropdownDividers(false);
        
        pixelateFolder=gui->addFolder("PIXELATE");
        pixelateMacroSlider=pixelateFolder->addSlider("Pixelate", 0, 100, pixelateMacro);
        
        fullhouseFolder=gui->addFolder("FULLHOUSE");
        fullhouseMacroSlider=fullhouseFolder->addSlider("Fullhouse", 1, 50, fullhouseMacro);
        
        asciiFolder=gui->addFolder("ASCII");
        asciiMacroSlider=asciiFolder->addSlider("Ascii", 0.0,1.0,asciiMacro);
        asciiDotDistanceSlider=asciiFolder->addSlider("Dot Distance", 0.0, 1.0,asciiDotDistance);
        asciiImageGainSlider=asciiFolder->addSlider("Image Gain", 0.0,1.0,asciiImageGain);
        asciiImageContrastSlider=asciiFolder->addSlider("Image Contrast", 0.0, 1.0,asciiImageContrast);
        asciiInvertToggle=asciiFolder->addToggle("Invert", asciiInvert);
        asciiFolder->setDropdownDividers(false);
        
        rotateFolder=gui->addFolder("ROTATE");
        rotateMacroSlider=rotateFolder->addSlider("Rotate",-1.0,1.0,rotateMacro);
        
        zebraFolder=gui->addFolder("ZEBRA");
        zebraMacroSlider=zebraFolder->addSlider("Zebra", 0.0, 1.0,zebraMacro);
        zebraSpeedSlider=zebraFolder->addSlider("Speed",0.0,1.0,zebraSpeed);
        zebraLevelsSlider=zebraFolder->addSlider("Levels",2,50,zebraLevels);
        zebraFolder->setDropdownDividers(false);
        
//        vhsFolder=gui->addFolder("VHS");
//        vhsMacroSlider=vhsFolder->addSlider("VHS",0.0, 1.0, vhsMacro);
//        vhsStrengthSlider=vhsFolder->addSlider("Strength",0.0,1.0,vhsStrength);
//        vhsSpeedSlider=vhsFolder->addSlider("Speed",0,60,vhsSpeed);
        
        gui->addBreak();
        gui->addBreak();
        
        gui->onButtonEvent(this, &ofApp::onButtonEvent);
        gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
        gui->onSliderEvent(this, &ofApp::onSliderEvent);
        gui->onToggleEvent(this, &ofApp::onToggleEvent);
        gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
        
        gui2->onToggleEvent(this, &ofApp::onToggleEvent);
        gui2->onButtonEvent(this, &ofApp::onButtonEventGui2);
        kaleidoscopeFolder->onSliderEvent(this, &ofApp::onSliderEvent);
        
        gui3->onButtonEvent(this, &ofApp::onButtonEventGui3);
        gui3->onSliderEvent(this, &ofApp::onSliderEventGui3);
        for(int i=0;i<numberOfLights;i++)
        {
            lightValues[i]=0;
            lightSliders[i] = gui3->addSlider("Ch."+ofToString(i+1),0,255,0, true,true);
            lightSliders[i]->setNumberbox("Ch"+ofToString(i+1));
            lightSliders[i]->setTextInput("Type Parameter Here");
//            lightSliders[i]->setLockedLayout(true);
            lightSliders[i]->setPrecision(0);
            lightSliders[i]->bind(lightValues[i]);
        }

        videoDivisionSlider->setPrecision(0);
        kaleidoscopeSectorSlider->setPrecision(0);
        zebraLevelsSlider->setPrecision(0);
        pixelateMacroSlider->setPrecision(0);
        fullhouseMacroSlider->setPrecision(0);
        clearToggle->setChecked(clear);
        videoSyncToggle->setChecked(videoSync);
        rippleSyncToggle->setChecked(rippleSync);
        
        ofBackground(ofColor::black);
        if(ofLoadImage( font, "font.jpg" ))cout<<"font loaded"<<endl;
        else cout<<"font not loaded"<<endl;
    }
    fxLabel->setBackgroundColor(ofColor::black);
    fxLabel->setLabelColor(ofColor::white);
    videosLabel->setBackgroundColor(ofColor::black);
    videosLabel->setLabelColor(ofColor::white);
    lightsLabel->setBackgroundColor(ofColor::black);
    lightsLabel->setLabelColor(ofColor::white);
    
    gui3->setWidth(gui2->getWidth()*2);
    snaves=1;
    gui->update();
    gui2->update();
    gui3->update();
}

void ofApp::update()//============================================================
{
    //---------Tempo update--------------------------------
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
//        lightSliders[i]->update();
        dmx.setLevel(i+1, lightValues[i]);
    }
    
    if(dmx.isConnected())
    {
  //      dmx.update();
    }
    
}

void ofApp::draw()//============================================================
{
    drawCount=currentlyDrawing;
    
    fbo.begin();                                            //FBO 1 begin
    ofClear(0,0,0,0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
    //----------Draw Video---------------------------------
    //                                                                          cout<<"video to draw: "<<imageToDraw<<endl;
    ofDisableSmoothing();
    ofBackground(bgColor1);                                 //set background color according to user parameter

    for(int i=0;i<max_videos;i++)
    {
        if(player[i].drawImage==false)player[i].stop();
        if(player[i].drawImage)// && videoCount<4)
        {
            if (videoSync==true)
            {
                bps=(bpm/60);
                videoSpeed=(((player[imageToDraw].video.getDuration())/bps)*videoDivision);
                player[i].setSpeed(videoSpeed/2);
                player[i].play();
            }
            else
            {
                player[i].setSpeed(videoSpeed2);
                if(player[i].isPlaying()==false)player[i].play();
            }
            
//            if(videoCount>4) break;
            
            player[i].setLoopState(OF_LOOP_NORMAL);
            ofSetColor(255,255,255,player[i].opacity);      //set color in order to draw video according to its opacity value
//            if(drawCount!=0)
            player[i].draw(0,0,getWidth,getHeight);         //draw video
            drawCount-=1;
        }
    }
    
    ofClearAlpha();                                         //clear alpha within the fbo itself
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    fbo.end();                                              //FBO end
    
    
    //-------FX FBO/Shader---------------------------------------------------------
    
    fbo2.begin();                                           //FBO 2 begin
    ofClear(0,0,0,0);
    fxShader.begin();                                         //Shader1 begin
    float time = ofGetElapsedTimef();
    
    fxShader.setUniform1f("fxMacro", fxMacro);
    
    fxShader.setUniform1f("time", time);
    fxShader.setUniform2f("resolution", getWidth,getHeight);
    
    fxShader.setUniform1f("filterMacro", filterMacro);
    fxShader.setUniform4f("filterRGB",filterRed,filterGreen,filterBlue,1.0); //commented out FilterAlpha
    
    fxShader.setUniform1f("invertMacro", invertMacro);
    
    fxShader.setUniform1f("rippleMacro", rippleMacro);
    fxShader.setUniform2f("rippleXY", rippleX,rippleY);
    fxShader.setUniform1f("rippleRate",rippleRate);
    
    int ksec=int(kaleidoscopeSectors*kaleidoscopeMacro);
    if(ksec<1)ksec=1;
    fxShader.setUniform1i("ksectors",ksec);
    fxShader.setUniform2f("kcenter", kaleiodioscopeX*kaleidoscopeMacro,kaleiodioscopeY*kaleidoscopeMacro);
    fxShader.setUniform1f("kaleidoscopeMacro", kaleidoscopeMacro);
    fxShader.setUniform1f("kangleRad", (ofDegToRad(kaleidoscopeAngle))*kaleidoscopeMacro);
//    cout<<"ksectors: "<<ksec<< "\n"<<"kcenter: "<<kaleiodioscopeX*kaleidoscopeMacro<<","<< kaleiodioscopeY*kaleidoscopeMacro<<"\n"<< "kaleidoscopeMacro: "<<kaleidoscopeMacro <<"\n";
    if(kaleidoscopeMacro<.5)
    {
        fxShader.setUniform2f("screenCenter",0,0);
//        cout<<"screenCenter: 0,0 \n";
    }
    else
    {
        fxShader.setUniform2f("screenCenter",int(0.5*getWidth),int(0.5*getHeight));
//        int t =int(0.5*getWidth);
//        int o =int(0.5*getHeight);
//        cout<<"screenCenter: "<<t <<","<<o<<"\n";
    }
//    cout<<"--------------------------\n";
    
    fxShader.setUniform1i("pixelateMacro", int(pixelateMacro));
    
    fxShader.setUniform1i("fullhouseMacro", int(fullhouseMacro));
    
    fxShader.setUniform1f("rotateMacro", rotateMacro);
    fxShader.setUniform2f("rotateScreenCenter",0.5*getWidth,0.5*getHeight);
    
    fxShader.setUniform1f("zebraMacro", zebraMacro);
    fxShader.setUniform1f("zebraSpeed", zebraSpeed);
    fxShader.setUniform1i("zebraLevels", zebraLevels);
    
//    shader.setUniform1f("vhsMacro", vhsMacro);
//    shader.setUniform1f("vhsStrength", vhsStrength);
//    shader.setUniform1f("vhsSpeed", vhsSpeed);
    fbo.draw(0,0, getWidth, getHeight);             //first FBO draw
//    blendFbo.draw(0,0,getWidth, getHeight);
    fxShader.end();                                   //shader1 end
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
    asciiShader.setUniform1i("asciiInvert", int(asciiInvert));
    asciiShader.setUniformTexture("font", font, 8);
    
    fbo2.draw(0,0, getWidth, getHeight);              //FBO 2 draw
    
    asciiShader.end();                                //ASCII Shader end
    fbo3.end();                                       //FBO 3 end
    

    fbo3.draw(0,0, getWidth, getHeight);                //FBO 3 draw
}

ofPoint ofApp::windowSize()//============================================================
{
    cout<<"windowSize \n";
    ofPoint Windowsize(gui->getWidth()+gui->getPadding()*2+gui2->getWidth()+gui3->getWidth()+gui->getPadding()*2,gui->getHeight());
    ofSetWindowShape(Windowsize.x, Windowsize.y);
    return Windowsize;
}

void ofApp::exitGui(ofEventArgs &args)//============================================================
{
    cout<<"exitGui \n";
   // if(ofSystemYesNoDialog("Any unsaved progress will be lost. Do you want to save before exiting?"))saveSettings();
    exit();
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
                        
                        
                    case 15:
                        fxMacro=ofMap(msg.value,0,127,0.0,1.0);
                        fxMacroSlider->setValue(fxMacro);
                        break;
                    case 16:
                        bgColor1Red=msg.value;
                        bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                        bgColor1ColorPicker->setColor(bgColor1);
                        break;
                    case 17:
                        bgColor1Green=msg.value;
                        bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                        bgColor1ColorPicker->setColor(bgColor1);
                        break;
                    case 18:
                        bgColor1Blue=msg.value;
                        bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                        bgColor1ColorPicker->setColor(bgColor1);
                        break;
                        
                        
                    case 19:
                        videoSpeed2=ofMap(msg.value, 0, 127, .1, 10.00);
                        videoSpeedSlider->setValue(videoSpeed2);
                        break;
                    case 20:
                        videoDivision=ofMap(msg.value, 0, 127, 1, 8);
                        videoDivisionSlider->setValue(videoDivision);
                        break;
                    case 21:
                        if(msg.value>63) videoSync=true;
                        else videoSync=false;
                        videoSyncToggle->setChecked(videoSync);
                        break;
                        
                        
                    case 22:
                        invertMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                        invertMacroSlider->setValue(invertMacro);
                        break;
                        
                        
                    case 23:
                        rippleMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                        rippleMacroSlider->setValue(rippleMacro);
                        break;
                    case 24:
                    {
                        if(msg.value>63)
                        {
                            rippleSync=true;
                            rippleRate=rippleRate=bpm/60;
                        }
                        else
                        {
                            rippleSync=false;
                            rippleRate=rippleRateSlider->getValue();
                        }
                        rippleSyncToggle->setChecked(rippleSync);
                        
                        break;
                    }
                    case 25:
                        rippleX=ofMap(msg.value,0, 127, 0.0, 1.0);
                        rippleXSlider->setValue(rippleX);
                        break;
                    case 26:
                        rippleY=ofMap(msg.value,0, 127, 0.0, 1.00);
                        rippleYSlider->setValue(rippleY);
                        break;
                    case 27:
                        if(rippleSync) rippleRate=bpm/60;
                        else rippleRate=ofMap(msg.value,0, 127, .1, 300);
                        rippleRateSlider->setValue(rippleRate);
                        break;
                        
                        
                    case 28:
                        filterMacro=ofMap(msg.value,0, 127, 0, 1.0);
                        filterMacroSlider->setValue(filterMacro);
                        break;
                    case 29:
                        filterRed=ofMap(msg.value,0, 127, 0, 1.0);
                        filterRedSlider->setValue(filterRed);
                        break;
                    case 30:
                        filterGreen=ofMap(msg.value,0, 127, 0, 1.0);
                        filterGreenSlider->setValue(filterGreen);
                        break;
                    case 31:
                        filterBlue=ofMap(msg.value,0, 127, 0, 1.0);
                        filterBlueSlider->setValue(filterBlue);
                        break;
                        
                        
                    case 32:
                        kaleidoscopeMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
                        kaleidoscopeMacroSlider->setValue(kaleidoscopeMacro);
                        break;
                    case 33:
                        kaleidoscopeAngle=ofMap(msg.value,0, 127, -180.0, 180.0);
                        kaleidoscopeAngleSlider->setValue(kaleidoscopeAngle);
                        break;
                    case 34:
                        kaleiodioscopeX=ofMap(msg.value,0, 127, 0.0, 1.00);
                        kaleidoscopeXSlider->setValue(kaleiodioscopeX);
                        break;
                    case 35:
                        kaleiodioscopeY=ofMap(msg.value,0, 127, 0.0, 1.00);
                        kaleidoscopeYSlider->setValue(kaleiodioscopeY);
                        break;
                    case 36:
                        kaleidoscopeSectors=ofMap(msg.value, 0, 127, 1.0, 100.0);
                        kaleidoscopeSectorSlider->setValue(kaleidoscopeSectors);
                        break;
                        
                        
                        
                    case 37:
                        pixelateMacro=ofMap(msg.value,0, 127, 0, 100);
                        pixelateMacroSlider->setValue(pixelateMacro);
                        break;
                        
                        
                    case 39:
                        fullhouseMacro=ofMap(msg.value,0,127,1,50);
                        fullhouseMacroSlider->setValue(fullhouseMacro);
                        break;
                        
                        
                    case 40:
                        asciiMacro=ofMap(msg.value,0,127,0.0,1.0);
                        asciiMacroSlider->setValue(asciiMacro);
                        break;
                    case 41:
                        asciiDotDistance=ofMap(msg.value, 0, 127, 0.0, 1.0);
                        asciiDotDistanceSlider->setValue(asciiDotDistance);
                        break;
                    case 42:
                        asciiImageGain=ofMap(msg.value, 0, 127, 0.0, 1.0);
                        asciiImageGainSlider->setValue(asciiImageGain);
                        break;
                    case 43:
                        asciiImageContrast=ofMap(msg.value,0,127,0.0,1.0);
                        asciiImageContrastSlider->setValue(asciiImageContrast);
                        break;
                    case 44:
                        if(msg.value>63) asciiInvert=true;
                        else asciiInvert=false;
                        asciiInvertToggle->setChecked(asciiInvert);
                        break;
                        
                        
                    case 45:
                        rotateMacro=ofMap(msg.value,0,127,-1.0,1.0);
                        rotateMacroSlider->setValue(rotateMacro);
                        break;
                        
                        
                    case 46:
                        zebraMacro=ofMap(msg.value,0,127,0.0,1.0);
                        zebraMacroSlider->setValue(zebraMacro);
                        break;
                    case 47:
                        zebraSpeed=ofMap(msg.value,0,127,0.0,1.0);
                        zebraSpeedSlider->setValue(zebraSpeed);
                        break;
                    case 48:
                        zebraLevels=ofMap(msg.value, 0, 127, 2, 50);
                        zebraLevelsSlider->setValue(zebraLevels);
                        break;
                        
                        
                        //                case 54:
                        //                    vhsMacro=ofMap(msg.value, 0, 127, 0.0, 1.0);
                        //                    vhsMacroSlider->setValue(vhsMacro);
                        //                    break;
                        //                case 55:
                        //                    vhsStrength=ofMap(msg.value,0,127,0.0,1.0);
                        //                    vhsStrengthSlider->setValue(vhsStrength);
                        //                    break;
                        //                case 56:
                        //                    vhsSpeed=ofMap(msg.value, 0, 127, 0.0, 60.0);
                        //                    vhsSpeedSlider->setValue(vhsSpeed);
                        //                    break;
                }
            }
            if(msg.channel==2)
            {
                if(msg.control>0 && msg.control<88) lightValues[msg.control-1]=ofMap(msg.value, 0, 127, 0, 255);
            }
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
    cout<<"midiNoteOff \n";
    player[pitch].drawImage=false;
    player[pitch].stop();
    player[pitch].firstFrame();
}

void ofApp::exit()//============================================================
{
    cout<<"exit \n";
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
    cout<<"keyPressed \n";
    //------------Ability to preview via QWERTY keyboard presses------------------
    bool def = false;
    int key = args.key;
    if(key==OF_KEY_COMMAND) command=true;
    

    if(command)
    {
        if(key=='s')
        {
            saveButton->drawTrue();
            saveSettings();
        }
        else if(key=='l') loadSettings();
//        else if(key=='n') if(ofSystemYesNoDialog("This will clear your video and text box fields. Continue?"))newProject();
    }
    else
    {
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
            case 'h': playerFromMidiMessage=25; break;
            case 'j': playerFromMidiMessage=26; break;
            case 'k': playerFromMidiMessage=27; break;
            case 'l': playerFromMidiMessage=28; break;
            case 'z': playerFromMidiMessage=29; break;
            case 'x': playerFromMidiMessage=30; break;
            case 'c': playerFromMidiMessage=31; break;
            case 'v': playerFromMidiMessage=32; break;
            case 'b': playerFromMidiMessage=33; break;
            case 'n': playerFromMidiMessage=34; break;
            case 'm': playerFromMidiMessage=35; break;
            default: def = true; break;
        }
    
        if(def!=true && player[playerFromMidiMessage].drawImage!=true)
        {
            player[playerFromMidiMessage].drawImage=true;
            player[playerFromMidiMessage].size=127;
            player[playerFromMidiMessage].size=ofMap(player[playerFromMidiMessage].size, 0, 127, 0.0, 2.0);
            currentlyDrawing+=1;
        }
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(ofKeyEventArgs & args)//============================================================
{
    cout<<"keyReleased \n";
    int key=args.key;
    if(key==OF_KEY_COMMAND) command=false;
    
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
        case 'h': videoNumber=25; break;
        case 'j': videoNumber=26; break;
        case 'k': videoNumber=27; break;
        case 'l': videoNumber=28; break;
        case 'z': videoNumber=29; break;
        case 'x': videoNumber=30; break;
        case 'c': videoNumber=31; break;
        case 'v': videoNumber=32; break;
        case 'b': videoNumber=33; break;
        case 'n': videoNumber=34; break;
        case 'm': videoNumber=35; break;
    }
    
    if(player[videoNumber].drawImage!=false)
    {
        player[videoNumber].drawImage=false;
        player[videoNumber].stop();
        player[videoNumber].firstFrame();
        currentlyDrawing-=1;
    }
}


void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)//============================================================
{
    cout<<"onDropdownEvent \n";
    if(e.target==usbDropdown) changeDevices(e.target->getSelected()->getIndex());
    if(e.target==midiDropdown)midiPort((e.target->getSelected())->getIndex());
}


void ofApp::changeDevices(int choice)
{
    cout<<"changeDevices \n";
    dmx.disconnect();
    dmx.connect(choice, numberOfLights);
}


void ofApp::dragEvent(ofDragInfo & dragInfo)//============================================================
{//--------event handler function for when a file is dragged into Spectacle window----------------------
    cout<<"dragEvent \n";
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
    cout<<"onButtonEvent \n";
    if(e.target==saveButton) saveSettings();
    else if(e.target==loadButton) loadSettings();
    else if(e.target==refreshUsbButton)
    {
        usbDropdown->changeOptions(dmx.getDevices());
        usbDropdown->setTheme(theme);
    }
    else if(e.target==refreshMidiButton)
    {
        midiDropdown->changeOptions(midiIn.getInPortList());
        midiDropdown->setTheme(theme);
    }
   // else if(e.target==newProjectButton) if(ofSystemYesNoDialog("This will clear your video and text box fields. Continue?"))newProject();
}


void ofApp::onButtonEventGui2(ofxDatGuiButtonEvent e)///============================================================
{//--------Button event handler function for video array part of GUI----------------------
                                                                cout << "onButtonEvent: " << e.target->getLabel() << endl;
    if(e.target==clearAllButton) clearAllVideos();
    
    else 
    {
        int iLabel=stoi(e.target->getName());
        iLabel-=1;
        if(clear==true)
        {
            if(player[iLabel].isLoaded()) player[iLabel].close();
            clear=false;
            clearToggle->setChecked(clear);
//            clearToggle->setBackgroundColor(ofColor::white);
//            clearToggle->setLabelColor(ofColor::black);
            e.target->setLabel(ofToString(iLabel+1));
        }
        else
        {
            loadMovie(iLabel);
        }
    }
}


void ofApp::onButtonEventGui3(ofxDatGuiButtonEvent e)
{
    cout<<"onButtonEventGui3 \n";
    if(e.target==clearAllLightsButton) clearAllLights();
}


void ofApp::clearAllLights()
{
    for(int i=0;i<numberOfLights;i++) lightSliders[i]->setTextInput("Type Parameter Here");
}


void ofApp::newProject()
{
    saveName = "default.xml";
    savePath = "";
//    if(ofSystemAlertDialog( ));
    clearAllLights();
    clearAllVideos();
}


void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)//============================================================
{//--------Toggle event handler function----------------------
    
    if (e.target==videoSyncToggle)videoSync=!videoSync;
    
    else if(e.target==clearToggle)
    {
        clear=!clear;
        clearToggle->setChecked(clear);
    }
    
    else if(e.target==rippleSyncToggle)rippleRate=bpm*60;
    
    else if(e.target==asciiInvertToggle)asciiInvert=!asciiInvert;
}

void ofApp::onTextInputEventGui1(ofxDatGuiTextInputEvent e)
{
    cout<<"onTextInputEventGui1 \n";
}


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


void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)//============================================================
{
    //--------Slider event handler function----------------------
    
    if(e.target==videoDivisionSlider)videoDivision=(e.target->getValue());
    if(e.target==fxMacroSlider)fxMacro=(e.target->getValue());
    
    else if(e.target==kaleidoscopeMacroSlider)kaleidoscopeMacro = (e.target->getValue());
    else if(e.target==kaleidoscopeAngleSlider)kaleidoscopeAngle=(e.target->getValue());
    else if(e.target==kaleidoscopeXSlider)kaleiodioscopeX=(e.target->getValue());
    else if(e.target==kaleidoscopeYSlider)kaleiodioscopeY=(e.target->getValue());
    else if(e.target==kaleidoscopeSectorSlider)kaleidoscopeSectors=(e.target->getValue());
    
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
    
    else if(e.target==rotateMacroSlider)rotateMacro=(e.target->getValue());
    
    else if(e.target==zebraMacroSlider)zebraMacro=(e.target->getValue());
    else if(e.target==zebraLevelsSlider)zebraLevels=(e.target->getValue());
    else if(e.target==zebraSpeedSlider)zebraSpeed=(e.target->getValue());
    
//    else if(e.target==vhsMacroSlider)vhsMacro=(e.target->getValue());
//    else if(e.target==vhsStrengthSlider)vhsStrength=(e.target->getValue());
//    else if(e.target==vhsSpeedSlider)vhsSpeed=(e.target->getValue());
//
}


void ofApp::onSliderEventGui3(ofxDatGuiSliderEvent e)//============================================================
{
//    lightValues[ofToInt(e.target->getLabel())-1]=(e.target->getValue());
}


void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)//============================================================
{    //--------Color picker event handler function----------------------
    if (e.target->is("BG Color 1"))
    {
        bgColor1=(e.color);
        bgColor1Red=e.color.r;
        bgColor1Green=e.color.g;
        bgColor1Blue=e.color.b;
    }
}


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
        savePath = loadPath;
        saveName = result.getName();
        
        xmlSettings.loadFile(loadPath);
        if(xmlSettings.loadFile(loadPath))
        {
            bool fail = false;
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
                        ofSystemAlertDialog("Media file " + ofToString(i+1) + " has been moved or renamed. Failed to locate.");
                        fail=true;
                    }
                    else
                    {
                        player[i].load(media);
                        string name = mediaFile.getFileName();
                        videoButtons[i]->setLabel(name);
                    }
                }
            }
            
            for(int i=0;i<numberOfLights;i++)
            {
                lightSliders[i]->setTextInput(xmlSettings.getValue("xmlSettings:lights:light"+ofToString(i),"Type Parameter Here"));
            }
            
            if(!fail)ofSystemAlertDialog("File done loading. It loaded successfully.");
            else ofSystemAlertDialog("File done loading. It was unsuccessful.");
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
    
    if(saveName=="default.xml")
    {
        ofFileDialogResult result = ofSystemSaveDialog("default.xml", "Save");
        if(result.bSuccess)
        {
            for(int i = 0; i <max_videos; i++)
            {
                if(player[i].full==true)
                {
                    xmlSettings.setValue("xmlSettings:media:full"+ofToString(i), player[i].full);
                    xmlSettings.setValue("xmlSettings:media:which"+ofToString(i), player[i].which);
                    xmlSettings.setValue("xmlSettings:media:media"+ofToString(i), player[i].path);
                }
            }
            
            for(int i=0;i<numberOfLights;i++)
            {
                xmlSettings.setValue("xmlSettings:lights:light"+ofToString(i), lightSliders[i]->getTextInput());
            }
            
            savePath = result.getPath();
            saveName = result.getName();
            xmlSettings.save(result.getPath());
            
            ofSystemAlertDialog("Save successful. \n");
            return true;
        }
        else
        {
            ofSystemAlertDialog("Save failed. \n");
            return false;
        }
    }
    
    else
        for(int i = 0; i <max_videos; i++)
        {
            if(player[i].full==true)
            {
                xmlSettings.setValue("xmlSettings:media:full"+ofToString(i), player[i].full);
                xmlSettings.setValue("xmlSettings:media:which"+ofToString(i), player[i].which);
                xmlSettings.setValue("xmlSettings:media:media"+ofToString(i), player[i].path);
            }
        }
        
        for(int i=0;i<numberOfLights;i++)
        {
            xmlSettings.setValue("xmlSettings:lights:light"+ofToString(i), lightSliders[i]->getTextInput());
        }
        
        xmlSettings.save(savePath);
        return true;
}


bool ofApp::midiPort(int midiPortOption)//============================================================
{
    if(midiIn.openPort(midiPortOption)) return true;
    else return false;
}


void ofApp::windowResized(ofResizeEventArgs &e)
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
}
