#pragma once

#include "ofApp.h"
#include "ofMain.h"

int snaves=0;

void ofApp::setup()////////////////////////////////////////////////////////////
{
    
    if(snaves==0)ofSetDataPathRoot(ofFilePath::getCurrentExeDir()+"../Resources/data/");
    ofSetFrameRate(60);
    ofBackground(ofColor::black);
    shader.load("shaderVert.c","shaderFrag.c");
    if(snaves==0) fbo.allocate(ofGetWidth(), ofGetHeight());

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
        gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
        gui->setAutoDraw(true);
//        scrollVideosGui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
        scrollVideos = new ofxDatGuiScrollView("Videos",10);
        scrollVideoTitle= new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
        scrollVideoTitle->setAutoDraw(true);
        
        
        scrollVideos->setVisible(true);
        scrollVideos->setAutoDraw(true);
        gui->addFRM();
        clear=false;
        
        scrollVideoTitle->addLabel("VIDEOS");
        scrollVideoTitle->setPosition(gui->getWidth(),0);
        for(int i=0;i<max_videos;i++)
        {
            scrollVideos->add(videoOptions[i]);
            cout<<"scroll videos"+ofToString(i)<<endl;
        }
        
        gui->addButton("Save");
        gui->addButton("Load"); 
        gui->addDropdown("MIDI Port:", midiIn.getInPortList());
        
        gui->addBreak();
        gui->addBreak();
        
        backgroundFolder=gui->addFolder("Background");
        backgroundSwitchToggle=backgroundFolder->addToggle("Background Switch");
        bgColor1ColorPicker=backgroundFolder->addColorPicker("BG Color 1",bgColor1);
        bgColor2ColorPicker=backgroundFolder->addColorPicker("BG Color 2", bgColor2);
        tempoDivisionSlider=backgroundFolder->addSlider("Tempo Division",1,3,1);
        tripletButton=backgroundFolder->addToggle("Triplet");
        cout<<"here 0"<<endl;
    
        gui->addLabel("FX");
        fxWetSlider = gui->addSlider("FX Wet",0.0,1.0,fxWet);
        
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
        filterAlphaSlider=filterFolder->addSlider("Alpha",0.0,1.0,filterAlpha);

        kaleidioscopeFolder=gui->addFolder("Kaleidioscope");
        kaleidoscopeSlider=kaleidioscopeFolder->addSlider("Kaleidoscope",0.0,1.0,kaleidoscopeMacro);
        sectorSlider=kaleidioscopeFolder->addSlider("Sectors", 1.0, 100,kaleidioscopeSectors);
        angleSlider=kaleidioscopeFolder->addSlider("Angle",-180,180,kaleidioscopeAngle);
        xSlider=kaleidioscopeFolder->addSlider("X",0.0,1.0,kaleiodioscopeX);
        ySlider=kaleidioscopeFolder->addSlider("Y",0.0,1.0,kaleiodioscopeY);
        
        cout<<"here 1"<<endl;

        gui->addBreak();
        gui->addBreak();
        
        clearToggle = scrollVideoTitle->addToggle("Clear");
        scrollVideoTitle->addButton("Clear All");
        cout<<"here 2"<<endl;
        guiPosition=gui->getPosition();
        scrollVideos->setPosition(gui->getWidth(),scrollVideoTitle->getHeight());
        
        gui->onButtonEvent(this, &ofApp::onButtonEvent);
        gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
        gui->onSliderEvent(this, &ofApp::onSliderEvent);
        gui->onToggleEvent(this, &ofApp::onToggleEvent);
        scrollVideos->onScrollViewEvent(this, &ofApp::onScrollViewEvent);
        gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
        scrollVideoTitle->onToggleEvent(this, &ofApp::onToggleEvent);
        kaleidioscopeFolder->onSliderEvent(this, &ofApp::onSliderEvent);
        cout<<"here 3"<<endl;
        tempoDivisionSlider->setPrecision(0);
        videoDivisionSlider->setPrecision(0);
        tripletButton->setChecked(triplet);
        backgroundSwitchToggle->setChecked(backgroundSwitch);
        clearToggle->setChecked(clear);
        videoSyncToggle->setChecked(videoSync);
        rippleSyncToggle->setChecked(rippleSync);
        cout<<"here 4"<<endl;
        ofxDatGuiThemeEvan *theme = new ofxDatGuiThemeEvan;
        ofxDatGuiThemeEvanFlip *flip = new ofxDatGuiThemeEvanFlip;
        
        gui->setTheme(theme);
        scrollVideos->setTheme(flip);
        scrollVideoTitle->setTheme(theme);
        gui->draw();
        scrollVideos->draw();
        scrollVideoTitle->draw();
        guiWindowSize.x=gui->getWidth()+scrollVideos->getWidth();
        
        ofBackground(theme->color.guiBackground);
    }
    snaves=1;
}

ofPoint ofApp::windowSize()/////////////////////////////////////////////////////////////////////
{
    ofPoint Windowsize(gui->getWidth()+scrollVideos->getWidth(),gui->getHeight()*1.5);
    ofSetWindowShape(Windowsize.x, Windowsize.y);
    return Windowsize;
}
void ofApp::exitGui(ofEventArgs &args)//////////////////////////////////////////////////////
{
    exit();
}

void ofApp::update()////////////////////////////////////////////////////////////////////////////
{
    //--Tempo update--------------------------------
//    cout << "update"<< endl;
    if(timecodeRunning && ofGetElapsedTimeMillis() - timecodeTimestamp > 100)
    {
        ofLog() << "timecode stopped";
        timecodeRunning = false;
    }
}

void ofApp::draw()///////////////////////////////////////////////////////////////////
{
    if (backgroundSwitch==1)
//    {
//        if (bg==0)
//        {
//            if(tempo==0)(ofBackgroundGradient(bgColor1,ofColor::black));
//            if(tempo==1)(ofBackgroundGradient(bgColor2,ofColor::white));
//        }
//    }
//    else ofBackgroundGradient(bgColor1,ofColor::black);
    {
        if (bg==0)
        {
            if(tempo==0)(ofBackground(bgColor1));
            if(tempo==1)(ofBackground(bgColor2));
        }
    }
    else ofBackground(bgColor1);
                         
    fbo.begin();
    ofClear(0,0,0,0);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    
//--Draw Video---------------------------------
//    cout<<"video to draw: "<<imageToDraw<<endl;
    ofDisableSmoothing();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(255,fxWet);
    
    for(int i=0;i<max_videos;i++)
    {
        if(player[i].drawImage==false)player[i].stop();
        if(player[i].drawImage && videoCount<4)
        {
            cout<<"videoOff: "<<videoOff<<endl;
            if (videoSync==true)
            {
                bps=(bpm/60);
                videoSpeed=(((player[imageToDraw].video.getDuration())/bps)*videoDivision);
                player[i].setSpeed(videoSpeed/2);
                player[i].play();
                cout<<"image to draw"<<imageToDraw<<endl;
            }
            else
            {
                player[i].setSpeed(videoSpeed2);
                if(player[i].isPlaying()==false)player[i].play();
            }
            
            if(videoCount>4) break;
            
            ofSetColor(255,255,255,255);
            player[i].draw(0,0,ofGetWidth(),ofGetHeight());
        }
    }
    
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    fbo.end();
    
    shader.begin();
    float time = ofGetElapsedTimef();
    
    shader.setUniform1f("time", time);
    
    shader.setUniform1f("filterMacro", filterMacro);
    shader.setUniform4f("filterRGB",filterRed,filterGreen,filterBlue,filterAlpha);
    
    shader.setUniform1f("invertMacro", invertMacro);
    
    shader.setUniform1f("rippleMacro", rippleMacro);
    shader.setUniform2f("rippleXY", rippleX,rippleY);
    shader.setUniform1f("rippleRate",rippleRate);
    
    shader.setUniform1i("ksectors", kaleidioscopeSectors*kaleidoscopeMacro);
    shader.setUniform2f("kcenter", kaleiodioscopeX*kaleidoscopeMacro,kaleiodioscopeY*kaleidoscopeMacro);
    shader.setUniform1f("kaleidoscopeMacro", kaleidoscopeMacro);
    shader.setUniform1f("kangleRad", (ofDegToRad(kaleidioscopeAngle))*kaleidoscopeMacro);
    if(kaleidoscopeMacro<.5) shader.setUniform2f("screenCenter",0,0);
    else shader.setUniform2f("screenCenter",0.5*ofGetWidth(),0.5*ofGetHeight());
    
    ofSetColor(255, 255, 255);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    fbo.draw(0,0,ofGetWidth(),ofGetHeight());
    shader.end();
}

void ofApp::newMidiMessage (ofxMidiMessage& msg)/////////////////////////////////////////////////////////////
{
    if(snaves==1)
    {
    //--New MIDI Message--------------------------
    if(msg.status==MIDI_NOTE_ON && msg.pitch>=60 && msg.pitch<=84)
    {
        midiMessages.push_back(msg); // add the latest message to the message queue
        while(midiMessages.size() > maxMessages) midiMessages.erase(midiMessages.begin());
        
        cout<<"msg.channel: "<<msg.channel<<endl;
        
        pitch=(msg.pitch);
        
            cout<<"midi note channel 2, pitch: "<<msg.pitch<<endl;
        
        if(sustain)midiNoteOff(msg.pitch);
        
            switch (msg.pitch)
            {
                case 60: playerFromMessage=0; break;
                case 61: playerFromMessage=1; break;
                case 62: playerFromMessage=2; break;
                case 63: playerFromMessage=3; break;
                case 64: playerFromMessage=4; break;
                case 65: playerFromMessage=5; break;
                case 66: playerFromMessage=6; break;
                case 67: playerFromMessage=7; break;
                case 68: playerFromMessage=8; break;
                case 69: playerFromMessage=9; break;
                case 70: playerFromMessage=10; break;
                case 71: playerFromMessage=11; break;
                case 72: playerFromMessage=12; break;
                case 73: playerFromMessage=13; break;
                case 74: playerFromMessage=14; break;
                case 75: playerFromMessage=15; break;
                case 76: playerFromMessage=16; break;
                case 77: playerFromMessage=17; break;
                case 78: playerFromMessage=18; break;
                case 79: playerFromMessage=19; break;
                case 80: playerFromMessage=20; break;
                case 81: playerFromMessage=21; break;
                case 82: playerFromMessage=22; break;
                case 83: playerFromMessage=23; break;
                case 84: playerFromMessage=24; break;
                default: break;
            }
        videoCount+=1;                                              cout<<"videoCount: "<<videoCount<<endl;
        player[playerFromMessage].size=msg.velocity;
        player[playerFromMessage].drawImage=true;
        player[playerFromMessage].size=ofMap(player[playerFromMessage].size, 0, 127, 0.0, 2.0);
    }
    
    if(msg.status==MIDI_NOTE_OFF && msg.pitch>=60 && msg.pitch<=84)
    {
        if(sustain==false)
        {
            midiNoteOff(msg.pitch);
            videoCount=-1;
        }
    }
    
    if(msg.status==MIDI_CONTROL_CHANGE)
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
                fxWet=ofMap(msg.value,0, 127, 0.0, 1.0);
                fxWetSlider->setValue(fxWet);
                break;
            case 16:
                videoSpeed2=ofMap(msg.value, 0, 127, .1, 10.00);
                videoSpeedSlider->setValue(videoSpeed2);
                break;
            case 17:
                videoDivision=msg.value;
                videoDivisionSlider->setValue(videoDivision);
            case 19:
                if(msg.value>63) backgroundSwitch=true;
                else backgroundSwitch=false;
                backgroundSwitchToggle->setChecked(backgroundSwitch);
                break;
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
                tripletButton->setChecked(triplet);
                break;
//            case 21:
//                bgColor1=ofFloatColor(ofMap(msg.value,0, 127, 0.0, 1.0));
//                bgColor1ColorPicker->setColor(bgColor1);
//                break;
//            case 22:
//                bgColor2=ofFloatColor(ofMap(msg.value,0, 127, 0.0, 1.0));
//                bgColor1ColorPicker->setColor(bgColor1);
//                break;
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
            case 35:
                filterAlpha=ofMap(msg.value,0, 127, 0, 1.0);
                filterAlphaSlider->setValue(filterAlpha);
                break;
                
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
            case 53:
                bgColor2Red=msg.value;
                bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                bgColor2ColorPicker->setColor(bgColor2);
                break;
            case 54:
                bgColor2Green=msg.value;
                bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                bgColor2ColorPicker->setColor(bgColor2);
                break;
            case 55:
                bgColor2Blue=msg.value;
                bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
                bgColor2ColorPicker->setColor(bgColor2);
                break;
        }
        
    }
       
//-Background changing with tempo (via MIDI clock)-----------------------------
    if (msg.status==MIDI_TIME_CLOCK && snaves==1)
    {
        if (triplet==0) if(tempoCount%(tempoDivisionValues[tempoDivision])==0) (tempo=!tempo);
        if(triplet==1) if(tempoCount%(tempoDivisionValuesTriplet[tempoDivision])==0) (tempo=!tempo);
        tempoCount=tempoCount+1;
        bpm+=(clock.getBpm()-bpm)/5;
    }
    }
}
void ofApp::midiNoteOff(int pitch)
{
    switch (pitch)
    {
        case 60: player[0].drawImage=false; break;
        case 61: player[1].drawImage=false; break;
        case 62: player[2].drawImage=false; break;
        case 63: player[3].drawImage=false; break;
        case 64: player[4].drawImage=false; break;
        case 65: player[5].drawImage=false; break;
        case 66: player[6].drawImage=false; break;
        case 67: player[7].drawImage=false; break;
        case 68: player[8].drawImage=false; break;
        case 69: player[9].drawImage=false; break;
        case 70: player[10].drawImage=false; break;
        case 71: player[11].drawImage=false; break;
        case 72: player[12].drawImage=false; break;
        case 73: player[13].drawImage=false; break;
        case 74: player[14].drawImage=false; break;
        case 75: player[15].drawImage=false; break;
        case 76: player[16].drawImage=false; break;
        case 77: player[17].drawImage=false; break;
        case 78: player[18].drawImage=false; break;
        case 79: player[19].drawImage=false; break;
        case 80: player[20].drawImage=false; break;
        case 81: player[21].drawImage=false; break;
        case 82: player[22].drawImage=false; break;
        case 83: player[23].drawImage=false; break;
        case 84: player[24].drawImage=false; break;
        default: break;
    }
}
void ofApp::exit()//////////////////////////////////////////////////////////////////////////////////////////////
{
    midiIn.closePort();
    midiIn.removeListener(this);
    
    if (!exiting)
    {
        exiting = true;
        ofExit();
    }
}

void ofApp::keyPressed(int key)////////////////////////////////////////////////////////////////////////////////
{                                                    cout<<"key pressed: "<<key<<endl;
    switch (key)
    {
        case '1': playerFromMessage=0; break;
        case '2': playerFromMessage=1; break;
        case '3': playerFromMessage=2; break;
        case '4': playerFromMessage=3; break;
        case '5': playerFromMessage=4; break;
        case '6': playerFromMessage=5; break;
        case '7': playerFromMessage=6; break;
        case '8': playerFromMessage=7; break;
        case '9': playerFromMessage=8; break;
        case '0': playerFromMessage=9; break;
        case 'q': playerFromMessage=10; break;
        case 'w': playerFromMessage=11; break;
        case 'e': playerFromMessage=12; break;
        case 'r': playerFromMessage=13; break;
        case 't': playerFromMessage=14; break;
        case 'y': playerFromMessage=15; break;
        case 'u': playerFromMessage=16; break;
        case 'i': playerFromMessage=17; break;
        case 'o': playerFromMessage=18; break;
        case 'p': playerFromMessage=19; break;
        case 'a': playerFromMessage=20; break;
        case 's': playerFromMessage=21; break;
        case 'd': playerFromMessage=22; break;
        case 'f': playerFromMessage=23; break;
        case 'g': playerFromMessage=24; break;
        default: break;
    }
    player[playerFromMessage].drawImage=true;
    player[playerFromMessage].size=127;
    player[playerFromMessage].size=ofMap(player[playerFromMessage].size, 0, 127, 0.0, 2.0);
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{                                                              cout<<"Key Released"<<endl;
    switch (key)
    {
        case '1': player[0].drawImage=false; player[0].stop(); player[0].firstFrame(); break;
        case '2': player[1].drawImage=false; player[1].stop(); player[1].firstFrame(); break;
        case '3': player[2].drawImage=false; player[2].stop(); player[2].firstFrame(); break;
        case '4': player[3].drawImage=false; player[3].stop(); player[3].firstFrame(); break;
        case '5': player[4].drawImage=false; player[4].stop(); player[4].firstFrame(); break;
        case '6': player[5].drawImage=false; player[5].stop(); player[5].firstFrame(); break;
        case '7': player[6].drawImage=false; player[6].stop(); player[6].firstFrame(); break;
        case '8': player[7].drawImage=false; player[7].stop(); player[7].firstFrame(); break;
        case '9': player[8].drawImage=false; player[8].stop(); player[8].firstFrame(); break;
        case '0': player[9].drawImage=false; player[9].stop(); player[9].firstFrame(); break;
        case 'q': player[10].drawImage=false; player[10].stop(); player[10].firstFrame();break;
        case 'w': player[11].drawImage=false; player[11].stop(); player[11].firstFrame();break;
        case 'e': player[12].drawImage=false; player[12].stop(); player[12].firstFrame();break;
        case 'r': player[13].drawImage=false; player[13].stop(); player[13].firstFrame();break;
        case 't': player[14].drawImage=false; player[14].stop(); player[14].firstFrame();break;
        case 'y': player[15].drawImage=false; player[15].stop(); player[15].firstFrame();break;
        case 'u': player[16].drawImage=false; player[16].stop(); player[16].firstFrame();break;
        case 'i': player[17].drawImage=false; player[17].stop(); player[17].firstFrame();break;
        case 'o': player[18].drawImage=false; player[18].stop(); player[18].firstFrame();break;
        case 'p': player[19].drawImage=false; player[19].stop(); player[19].firstFrame();break;
        case 'a': player[20].drawImage=false; player[20].stop(); player[20].firstFrame();break;
        case 's': player[21].drawImage=false; player[21].stop(); player[21].firstFrame();break;
        case 'd': player[22].drawImage=false; player[22].stop(); player[22].firstFrame();break;
        case 'f': player[23].drawImage=false; player[23].stop(); player[23].firstFrame();break;
        case 'g': player[24].drawImage=false; player[24].stop(); player[24].firstFrame();break;
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

void ofApp::onDropdownEvent(ofxDatGuiDropdownEvent e)//////////////////////////////////////////////////////////////
{
    midiPort((e.target->getSelected())->getIndex());
}

void ofApp::onScrollViewEvent(ofxDatGuiScrollViewEvent e)//////////////////////////////////////////////////////////////
{
    int iLabel=(e.target->getIndex());
    if(clear==true)
    {
        if(player[iLabel].isLoaded()) player[iLabel].close();
        clear=false;
        clearToggle->setBackgroundColor(ofColor::black);
        clearToggle->setLabelColor(ofColor::white);
        e.target->setLabel(ofToString(iLabel));
    }
    else
    {                                               cout<<"option selected:"<<e.target->getLabel()<<endl;
        loadMovie(iLabel);
    }
}

void ofApp::dragEvent(ofDragInfo & dragInfo)////////////////////////////////////////////////////////////////////////////////////////////////////
{
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
                    player[i].load(dragInfo.files[m]);
                    scrollVideos->getItemAtIndex(i)->setLabel(ofFilePath::getFileName(dragInfo.files[m]));
                    player[i].setLoopState(OF_LOOP_NORMAL);
        }
    }
}

void ofApp::onButtonEvent(ofxDatGuiButtonEvent e)//////////////////////////////////////////////////////////////
{
    if(e.target->is("Save")) saveSettings();
    else if(e.target->is("Load")) loadSettings();
    else if(e.target->is("Clear All")) clearAllVideos();             cout << "onButtonEvent: " << e.target->getLabel() << endl;
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)///////////////////////////////////////////////////////////
{
    if (e.target->is("Triplet"))triplet=!triplet;
    else if (e.target->is("Background Switch"))backgroundSwitch=!backgroundSwitch;
    else if (e.target->is("Video Sync"))videoSync=!videoSync;
//    else if (e.target==rippleSyncToggle)rippleRate=;
    
    else if(e.target->is("Clear")) clear=!clear;
    
    else if(e.target==rippleSyncToggle)rippleRate=bpm*60;
}

void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)//////////////////////////////////////////////////////////////
{
    if(e.target==tempoDivisionSlider)tempoDivision=(e.target->getValue());
    if(e.target==videoDivisionSlider)videoDivision=(e.target->getValue());
    if(e.target==fxWetSlider)fxWet=(e.target->getValue());
    
    else if(e.target==kaleidoscopeSlider)kaleidoscopeMacro = (e.target->getValue());
    else if(e.target==angleSlider)kaleidioscopeAngle=(e.target->getValue());
    else if(e.target==xSlider)kaleiodioscopeX=(e.target->getValue());
    else if(e.target==ySlider)kaleiodioscopeY=(e.target->getValue());
    else if(e.target==sectorSlider)kaleidioscopeSectors=(e.target->getValue());
    
    else if(e.target==filterSlider)filterMacro=(e.target->getValue());
    else if(e.target==filterRedSlider)filterRed=(e.target->getValue());
    else if(e.target==filterBlueSlider)filterBlue=(e.target->getValue());
    else if(e.target==filterGreenSlider)filterGreen=(e.target->getValue());
    else if(e.target==filterAlphaSlider)filterAlpha=(e.target->getValue());
    
    else if(e.target==rippleSlider)rippleMacro=(e.target->getValue());
    else if(e.target==rippleYSlider)rippleY=(e.target->getValue());
    else if(e.target==rippleXSlider)rippleX=(e.target->getValue());
    else if(e.target==rippleRateSlider)
    {
        if(rippleSync==false)rippleRate=(e.target->getValue());
    }
    else if(e.target==invertSlider)invertMacro=(e.target->getValue());
    
    else if(e.target==videoSpeedSlider)videoSpeed2=(e.target->getValue());
}

void ofApp::onColorPickerEvent(ofxDatGuiColorPickerEvent e)//////////////////////////////////////////////////////////////
{
    cout << "onColorPickerEvent: " << e.target->getLabel() << " " << e.target->getColor() << endl;
    if (e.target->is("BG Color 1"))
    {
        bgColor1=(e.color);
        bgColor1Red=e.color.r;
        bgColor1Green=e.color.g;
        bgColor1Blue=e.color.b;
    }
    else if (e.target->is("BG Color 2"))
    {
        bgColor2=(e.color);
        bgColor2Red=e.color.r;
        bgColor2Green=e.color.g;
        bgColor2Blue=e.color.b;
    }
    
}

void ofApp::clearAllVideos()//////////////////////////////////////////////////////////////
{
    for(int i=0;i<max_videos;i++)
    {
        ofxDatGuiButton *e=scrollVideos->getItemAtIndex(i);
        if(player[i].isLoaded()) player[i].close();
        e->setLabel(ofToString(i+1));
    }
}

bool ofApp::loadMovie(int i)///////////////////////////////////////////////////
{                                                                           cout << "loading videos" << endl;
    ofFileDialogResult result = ofSystemLoadDialog("Load file");
    if(result.bSuccess)
    {
        cout<<"new movie path:"<<result.getName()<<endl<<"player array: "<<player[i].getPath()<<endl;
        if(player[i].getError())
        {
//            bool d = (player[i].video.getError().length());
            //warning dialogue, "you've selected an incompatible file type";
        }
        player[i].video.setPixelFormat(OF_PIXELS_RGBA);
        player[i].load(result.getPath());
    
        scrollVideos->getItemAtIndex(i)->setLabel(result.getName());
        player[i].setLoopState(OF_LOOP_NORMAL);
        return true;
    }
    else return false;
}

bool ofApp::loadSettings()///////////////////////////////////////////////////////////////////////////////////////////
{
    ofFileDialogResult result = ofSystemLoadDialog("Load file");
    cout << "  load  ";
    if(result.bSuccess)
    {
        string loadPath = result.getPath();
        xmlSettings.loadFile(loadPath);
        if(xmlSettings.loadFile(loadPath))
        {
            for(int i = 0; i <max_videos; i++)
            {
                player[i].which=(xmlSettings.getValue("xmlSettings:media:which"+ofToString(i),int(NULL)));
                string media = (xmlSettings.getValue(("xmlSettings:media:video"+ofToString(i)),player[i].getPath()));
                player[i].load(media);
            }
            
            bgColor1Red=xmlSettings.getValue("xmlSettings:color:bgColor1Red", 0);
            bgColor1Green=xmlSettings.getValue("xmlSettings:color:bgColor1Green", 0);
            bgColor1Blue=xmlSettings.getValue("xmlSettings:color:bgColor1Blue", 0);
            bgColor2Red=xmlSettings.getValue("xmlSettings:color:bgColor2Red", 0);
            bgColor2Green=xmlSettings.getValue("xmlSettings:color:bgColor2Green", 0);
            bgColor2Blue=xmlSettings.getValue("xmlSettings:color:bgColor2Blue", 0);
            
            bgColor1=(ofColor::fromHsb(bgColor1Red, bgColor1Green, bgColor1Blue));
            bgColor2=(ofColor::fromHsb(bgColor2Red, bgColor2Green, bgColor2Blue));
            
            videoSync=xmlSettings.getValue("xmlSettings:gui:videoSync", false);
            triplet=xmlSettings.getValue("xmlSettings:gui:triplet", false);
            backgroundSwitch=xmlSettings.getValue("xmlSettings:gui:background", 0);
            tempoDivision=xmlSettings.getValue("xmlSettings:gui:tempoDivision", 1);
            videoDivision=xmlSettings.getValue("xmlSettings:color:videoDivision", 1);
            
            return true;
        }
        else
        {
            cout<<"  file did not load!  "<<endl;;
            return false;
        }
    }
    else return false;
}

bool ofApp::saveSettings()/////////////////////////////////////////////////////////////////////////////////////////////
{
    cout << "save" << endl;
    ofFileDialogResult result = ofSystemSaveDialog("default.xml", "Save");
    if(result.bSuccess)
    {
        cout << "save success" << endl;
        for(int i = 0; i <max_videos; i++)
        {
            xmlSettings.addValue("xmlSettings:media:which"+ofToString(i),player[i].which);
            xmlSettings.addValue("xmlSettings:media:video"+ofToString(i), player[i].getPath());
        }
        
        bgColor1Red=(bgColor1.getHue());
        bgColor1Green=(bgColor1.getSaturation());
        bgColor1Blue=(bgColor1.getBrightness());
        bgColor2Red=(bgColor2.getHue());
        bgColor2Green=(bgColor2.getSaturation());
        bgColor2Blue=(bgColor2.getBrightness());
        
        xmlSettings.setValue("xmlSettings:color:bgColor1Red", bgColor1Red);
        xmlSettings.setValue("xmlSettings:color:bgColor1Green", bgColor1Green);
        xmlSettings.setValue("xmlSettings:color:bgColor1Blue", bgColor1Blue);
        xmlSettings.setValue("xmlSettings:color:bgColor2Red", bgColor2Red);
        xmlSettings.setValue("xmlSettings:color:bgColor2Green", bgColor2Green);
        xmlSettings.setValue("xmlSettings:color:bgColor2Blue", bgColor2Blue);
        cout << "save out" << endl;
        xmlSettings.save(result.getPath());
        
        xmlSettings.setValue("xmlSettings:gui:videoSync", videoSync);
        xmlSettings.setValue("xmlSettings:gui:triplet", triplet);
        xmlSettings.setValue("xmlSettings:gui:background", backgroundSwitch);
        
        xmlSettings.setValue("xmlSettings:gui:tempoDivision", tempoDivision);
        xmlSettings.setValue("xmlSettings:color:videoDivision", videoDivision);
        
        return true;
    }
    else
    {
        cout<<"save failed"<<endl;
        return false;
    }
    return false;
}

bool ofApp::midiPort(int midiPortOption)//////////////////////////////////////////////////////////////
{
    if(midiIn.openPort(midiPortOption)) return true;
    else return false;
}
