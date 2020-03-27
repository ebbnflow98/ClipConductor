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
        gui3 = new ofxDatGui(ofxDatGuiAnchor::NO_ANCHOR);
        gui3->setAutoDraw(true);
        clear=false;
        
        gui2->addLabel("VIDEOS");
        clearToggle = gui2->addToggle("Clear");
        clearAllButton = gui2->addButton("Clear All");
        gui2->setPosition(gui->getWidth(),0);
        gui2->addBreak();
        gui2->addBreak();
        
        gui3->setPosition(gui->getWidth()+gui2->getWidth(), 0);
        
        
        for(int i=0;i<max_videos;i++)
        {
            videoButtons[i]=(gui2->addButton(videoOptions[i]));
        }
        
        saveButton = gui->addButton("Save");
        loadButton = gui->addButton("Load");
        midiDropdown= gui->addDropdown("MIDI Port:", midiIn.getInPortList());
        
        addLightButton = gui3->addButton("Add Light");
        
        backgroundFolder=gui->addFolder("Background");
        bgColor1ColorPicker=backgroundFolder->addColorPicker("BG Color 1",bgColor1);
//        bgColor2ColorPicker=backgroundFolder->addColorPicker("BG Color 2", bgColor2);
        tempoDivisionSlider=backgroundFolder->addSlider("Tempo Division",1,3,1);
        tripletToggle=backgroundFolder->addToggle("Triplet");
    
        
        gui->addLabel("FX");
        
        fxMacro.slider = gui->addSlider("FX Wet",fxMacro.min,fxMacro.max,fxMacro.value);
        fxMacro.slider->setShowNumberBox(true);
        fxMacro.slider->setNumberBoxNumber(0);
        
        videoFolder=gui->addFolder("Video Controls");
        videoSpeedSlider=videoFolder->addSlider("Video Speed",0.1,10.0,videoSpeed2);
        videoSyncToggle=videoFolder->addToggle("Video Sync");
        videoDivisionSlider=videoFolder->addSlider("Video Division",1,8,1);
        
        //invert fx
        invertFolder=gui->addFolder("Invert");
        invertMacro.slider=invertFolder->addSlider("Invert",invertMacro.min,invertMacro.max,invertMacro.value);
        invertMacro.slider->setPrecision(1);
        invertMacro.slider->setValue(invertMacro.value);
        
        //ripple fx
        rippleFolder=gui->addFolder("Ripple");
        rippleMacro.slider = rippleFolder->addSlider("Ripple",rippleMacro.min,rippleMacro.max,rippleMacro.value);
        rippleSyncToggle=rippleFolder->addToggle("Sync");
        rippleX.slider=rippleFolder->addSlider("X", rippleX.min,rippleX.max,rippleX.value);
        rippleY.slider=rippleFolder->addSlider("Y", rippleX.min,rippleY.max,rippleY.value);
        rippleRate.setMinMaxValue(0.1, 300.00, 1.0);
        rippleRate.slider=rippleFolder->addSlider("Rate", rippleRate.min, rippleRate.max, rippleRate.value);
        
        //filter fx
        filterFolder=gui->addFolder("Filter");
        filterMacro.slider=filterFolder->addSlider("Filter",filterMacro.min,filterMacro.max,filterMacro.value);
        filterRed.slider=filterFolder->addSlider("Red", filterRed.min,filterRed.max,filterRed.value);
        filterGreen.slider=filterFolder->addSlider("Green",filterGreen.min,filterGreen.max,filterGreen.value);
        filterBlue.slider=filterFolder->addSlider("Blue",filterBlue.min,filterBlue.max,filterBlue.value);

        //kaleidioscope fx
        kaleidioscopeFolder=gui->addFolder("Kaleidioscope");
        kaleidoscopeMacro.slider=kaleidioscopeFolder->addSlider("Kaleidoscope",kaleidoscopeMacro.min,kaleidoscopeMacro.max,kaleidoscopeMacro.value);
        kaleidioscopeSectors.setMinMaxValue(1.0, 100.0, 1.0);
        kaleidioscopeSectors.slider=kaleidioscopeFolder->addSlider("Sectors", kaleidioscopeSectors.min, kaleidioscopeSectors.max,kaleidioscopeSectors.value);
        kaleidioscopeAngle.setMinMaxValue(-180, 180, 0);
        kaleidioscopeAngle.slider=kaleidioscopeFolder->addSlider("Angle",kaleidioscopeAngle.min,kaleidioscopeAngle.max,kaleidioscopeAngle.value);
        kaleidioscopeAngle.slider->setPrecision(0);
        kaleiodioscopeX.slider=kaleidioscopeFolder->addSlider("X",kaleiodioscopeX.min,kaleiodioscopeX.max,kaleiodioscopeX.value);
        kaleiodioscopeY.slider=kaleidioscopeFolder->addSlider("Y",kaleiodioscopeY.min,kaleiodioscopeY.max,kaleiodioscopeY.value);
       
        //pixelate fx
        pixelateFolder=gui->addFolder("Pixelate");
        pixelateMacro.setMinMaxValue(0, 100, 0);
        pixelateMacro.slider=pixelateFolder->addSlider("Pixelate", pixelateMacro.min, pixelateMacro.max, pixelateMacro.value);
        pixelateMacro.slider->setPrecision(0);
        
        //fullhouse fx
        fullhouseFolder=gui->addFolder("Fullhouse");
        fullhouseMacro.setMinMaxValue(0, 50, 0);
        fullhouseMacro.slider=fullhouseFolder->addSlider("Fullhouse", fullhouseMacro.min, fullhouseMacro.max, fullhouseMacro.value);
        fullhouseMacro.slider->setPrecision(0);

       //ascii fx
        asciiFolder=gui->addFolder("Ascii");
        asciiMacro.slider=asciiFolder->addSlider("Ascii", asciiMacro.min,asciiMacro.max,asciiMacro.value);
        asciiInvertToggle=asciiFolder->addToggle("Ascii Invert", asciiInvert);
        asciiImageContrast.slider=asciiFolder->addSlider("Ascii Image Contrast", asciiImageContrast.min,asciiImageContrast.max,asciiImageContrast.value);
        asciiImageGain.slider=asciiFolder->addSlider("Ascii Image Gain", asciiImageGain.min,asciiImageGain.max,asciiImageGain.value);
        asciiDotDistance.slider=asciiFolder->addSlider("ASCII Dot Distance", asciiDotDistance.min,asciiDotDistance.max,asciiDotDistance.value);

        //led fx
        ledFolder=gui->addFolder("LED");
        ledMacro.slider=ledFolder->addSlider("LED", ledMacro.min, ledMacro.max,ledMacro.value);
        ledDotDistance.slider=ledFolder->addSlider("LED Dot Distance", ledDotDistance.min, ledDotDistance.max,ledDotDistance.value);
//        ledOffsetRXSlider=ledFolder->addSlider("Red Offset X", 0.0 , 1.0, ledOffsetRX);
//        ledOffsetRYSlider=ledFolder->addSlider("Red Offset Y", 0.0 , 1.0, ledOffsetRY);
//        ledOffsetGXSlider=ledFolder->addSlider("Green Offset X", 0.0 , 1.0, ledOffsetGX);
//        ledOffsetGYSlider=ledFolder->addSlider("Green Offset Y", 0.0 , 1.0, ledOffsetGY);
//        ledOffsetBXSlider=ledFolder->addSlider("Blue Offset X", 0.0 , 1.0, ledOffsetBX);
//        ledOffsetBYSlider=ledFolder->addSlider("Blue Offset Y", 0.0 , 1.0, ledOffsetBY);
        
        //rotate fx
        rotateFolder=gui->addFolder("Rotate");
        rotateMacro.setMinMaxValue(-1.0, 1.0, 0.0);
        rotateMacro.slider=rotateFolder->addSlider("Rotate",rotateMacro.min,rotateMacro.max,rotateMacro.value);
       
        //zebra fx
        zebraFolder=gui->addFolder("Zebra");
        zebraMacro.slider=zebraFolder->addSlider("Zebra", zebraMacro.min, zebraMacro.max,zebraMacro.value,true, 15);
        zebraMacro.slider->setPreviousNumberBoxNumber(15);
        zebraSpeed.slider=zebraFolder->addSlider("Speed",zebraSpeed.min,zebraSpeed.max,zebraSpeed.value,true, 16);
        zebraSpeed.slider->setPreviousNumberBoxNumber(16);
        zebraLevels.setMinMaxValue(2, 50, 5);
        zebraLevels.slider=zebraFolder->addSlider("Levels",zebraLevels.min,zebraLevels.max,zebraLevels.value,true,17);
        zebraLevels.slider->setPreviousNumberBoxNumber(17);
        zebraLevels.slider->setPrecision(0);
        
        //chromaKey fx
        chromaKeyFolder=gui->addFolder("ChromaKey");
        chromaKeyMacro.slider=chromaKeyFolder->addSlider("ChromaKey", chromaKeyMacro.min, chromaKeyMacro.max,chromaKeyMacro.value);
        chromaKeyColorPicker=chromaKeyFolder->addColorPicker("Key");
        chromaKeyColorPicker->setColor(ofColor::green);
        chromaKeyThreshold.slider=chromaKeyFolder->addSlider("Threshold", chromaKeyMacro.min, chromaKeyMacro.max,chromaKeyThreshold.value);
        
       //squareioscope fx
        squareioscopeFolder=gui->addFolder("Squareioscope");
        squareioscopeMacro2.slider=squareioscopeFolder->addSlider("Squareioscope2", squareioscopeMacro2.min, squareioscopeMacro2.max,squareioscopeMacro2.value);
        squareioscopeMacro.slider=squareioscopeFolder->addSlider("Squareioscope",squareioscopeMacro.min,squareioscopeMacro.max,squareioscopeMacro.value);
        
        //vhs fx
        vhsFolder=gui->addFolder("VHS");
        vhsMacro.slider=vhsFolder->addSlider("VHS",vhsMacro.min, vhsMacro.max, vhsMacro.value);
        vhsStrength.slider=vhsFolder->addSlider("Strength",vhsStrength.min,vhsStrength.max,vhsStrength.value);
        vhsSpeed.slider=vhsFolder->addSlider("Speed",vhsSpeed.min,vhsSpeed.max,vhsSpeed.value);

        
        fxByCC[15].push_back(&fxMacro);
//        fxByCC[16].push_back(&videoSpeed2);
//        fxByCC[17].push_back(&videoDivison);
//        fxByCC[18].push_back(&videoSync);
//        fxByCC[20].push_back(&tempoSync);
//        fxByCC[21].push_back(&tripletToggle);
        
        fxByCC[24].push_back(&invertMacro);
        
        fxByCC[25].push_back(&rippleMacro);
        
//        fxByCC[26].push_back(&rippleSync);
        fxByCC[27].push_back(&rippleX);
        fxByCC[28].push_back(&rippleY);
//        fxByCC[29].push_back(&rippleSync);
        
        fxByCC[30].push_back(&filterMacro);
        fxByCC[31].push_back(&filterRed);
        fxByCC[33].push_back(&filterGreen);
        fxByCC[34].push_back(&filterBlue);
        
        fxByCC[40].push_back(&kaleidoscopeMacro);
        fxByCC[41].push_back(&kaleidioscopeAngle);
        fxByCC[42].push_back(&kaleiodioscopeX);
        fxByCC[43].push_back(&kaleiodioscopeY);
        fxByCC[44].push_back(&kaleidioscopeSectors);
        
//        fxByCC[50].push_back(&bgColor1Red);
//        fxByCC[51].push_back(&bgColor1Green);
//        fxByCC[52].push_back(&bgColor1Blue);
//        fxByCC[53].push_back(&bgColor2Red);
//        fxByCC[54].push_back(&bgColor2Green);
//        fxByCC[55].push_back(&bgColor2Blue);
        
        fxByCC[56].push_back(&pixelateMacro);
        fxByCC[57].push_back(&fullhouseMacro);
        
        fxByCC[60].push_back(&asciiMacro);
        fxByCC[61].push_back(&asciiDotDistance);
        fxByCC[62].push_back(&asciiImageGain);
        fxByCC[63].push_back(&asciiImageContrast);
//        fxByCC[65].push_back(&asciiInvert);
        
        fxByCC[66].push_back(&ledMacro);
        fxByCC[67].push_back(&ledDotDistance);
        fxByCC[68].push_back(&ledOffsetRX);
        fxByCC[69].push_back(&ledOffsetRY);
        fxByCC[70].push_back(&ledOffsetGX);
        fxByCC[71].push_back(&ledOffsetGY);
        fxByCC[72].push_back(&ledOffsetBX);
        fxByCC[73].push_back(&ledOffsetBY);
        
        fxByCC[74].push_back(&rotateMacro);
        
        fxByCC[75].push_back(&zebraMacro);
        fxByCC[76].push_back(&zebraSpeed);
        fxByCC[77].push_back(&zebraLevels);
        
        fxByCC[78].push_back(&chromaKeyMacro);
//        fxByCC[79].push_back(&chromaKeyRed);
//        fxByCC[80].push_back(&chromaKeyGreen);
//        fxByCC[81].push_back(&chromaKeyBlue);
        fxByCC[82].push_back(&chromaKeyThreshold);
        
        fxByCC[83].push_back(&squareioscopeMacro);
        fxByCC[84].push_back(&squareioscopeMacro2);
        
        fxByCC[85].push_back(&vhsMacro);
        fxByCC[86].push_back(&vhsStrength);
        fxByCC[87].push_back(&vhsSpeed);

        gui->addBreak();
        gui->addBreak();
        
        gui->onButtonEvent(this, &ofApp::onButtonEvent);
        gui->onColorPickerEvent(this, &ofApp::onColorPickerEvent);
        gui->onSliderEvent(this, &ofApp::onSliderEvent);
        gui->onToggleEvent(this, &ofApp::onToggleEvent);
        gui->onDropdownEvent(this, &ofApp::onDropdownEvent);
        gui->onNumberBoxChangedEvent(this, &ofApp::onNumberBoxChangedEventGui1);
        
        gui2->onToggleEvent(this, &ofApp::onToggleEvent);
        gui2->onButtonEvent(this, &ofApp::onButtonEventGui2);
        kaleidioscopeFolder->onSliderEvent(this, &ofApp::onSliderEvent);
        cout<<"here 3"<<endl;
        
        gui3->onToggleEvent(this, &ofApp::onToggleEventGui3);
        gui3->onButtonEvent(this, &ofApp::onButtonEventGui3);
        gui3->onRightClickEvent(this, &ofApp::onRightClickEventGui3);
        
        mAddNewLight.addListener(this, &ofApp::onAddNewLightEventGui3);
        submitButton=mAddNewLight.getButton(0);
        cancelButton=mAddNewLight.getButton(1);
        
        mEditLight.addListener(this, &ofApp::onEditLightEventGui3);
        gui3->onTextInputEvent(this, &ofApp::onTextInputEventGui3);
        

        tempoDivisionSlider->setPrecision(0);
        videoDivisionSlider->setPrecision(0);
        kaleidioscopeSectors.slider->setPrecision(0);
        zebraLevels.slider->setPrecision(0);
        pixelateMacro.slider->setPrecision(0);
        fullhouseMacro.slider->setPrecision(0);
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
    ofPoint Windowsize(gui->getWidth()+gui2->getWidth()+gui3->getWidth(),gui->getHeight()*1.5);
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
    
//    cout<<"currentlyDrawing: "<<currentlyDrawing<<"\n";
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
     
    if(chromaKeyVideo!= -1)player[chromaKeyVideo].draw(0, 0, getWidth, getHeight);
    
    chromaKeyVideoFbo.end();
    
//-----Chroma Key FX FBO/Shader--------------------------------------------------

    chromaKeyFxFbo.begin();
    ofClear(0,0,0,0);
    
    chromaKeyShader.begin();
    
    shader.setUniform1f("fxMacro", fxMacro.value);
    chromaKeyShader.setUniform1f("chromaKeyMacro", chromaKeyMacro.value);
    chromaKeyShader.setUniform3f("chromaKeyColor", chromaKeyColor.getRedFloat(),chromaKeyColor.getGreenFloat(),chromaKeyColor.getBlueFloat());
    chromaKeyShader.setUniform1f("chromaKeyThreshold",chromaKeyThreshold.value);
    
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
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    shader.begin();                                         //Shader1 begin
    float time = ofGetElapsedTimef();

    shader.setUniform1f("fxMacro", fxMacro.value);

    shader.setUniform1f("time", time);
    shader.setUniform2f("resolution", getWidth,getHeight);

    shader.setUniform1f("filterMacro", filterMacro.value);
    shader.setUniform4f("filterRGB",filterRed.value,filterGreen.value,filterBlue.value,1.0); //commented out FilterAlpha

    shader.setUniform1f("invertMacro", invertMacro.value);

    shader.setUniform1f("rippleMacro", rippleMacro.value);
    shader.setUniform2f("rippleXY", rippleX.value,rippleY.value);
    shader.setUniform1f("rippleRate",rippleRate.value);

    shader.setUniform1i("ksectors", int(kaleidioscopeSectors.value*kaleidoscopeMacro.value));
    shader.setUniform2f("kcenter", kaleiodioscopeX.value*kaleidoscopeMacro.value,kaleiodioscopeY.value*kaleidoscopeMacro.value);
    shader.setUniform1f("kaleidoscopeMacro", kaleidoscopeMacro.value);
    shader.setUniform1f("kangleRad", (ofDegToRad(kaleidioscopeAngle.value))*kaleidoscopeMacro.value);
    if(kaleidoscopeMacro.value<.5) shader.setUniform2f("screenCenter",0,0);
    else shader.setUniform2f("screenCenter",0.5*getWidth,0.5*getHeight);

    shader.setUniform1i("pixelateMacro", int(pixelateMacro.value));

    shader.setUniform1i("fullhouseMacro", int(fullhouseMacro.value));

    shader.setUniform1f("rotateMacro", rotateMacro.value);
    shader.setUniform2f("rotateScreenCenter",0.5*getWidth,0.5*getHeight);

    shader.setUniform1f("zebraMacro", zebraMacro.value);
    shader.setUniform1f("zebraSpeed", zebraSpeed.value);
    shader.setUniform1i("zebraLevels", zebraLevels.value);
    
    shader.setUniform1f("squareioscopeMacro", squareioscopeMacro.value);
    shader.setUniform1f("squareioscopeMacro2", squareioscopeMacro2.value);
    
    shader.setUniform1f("vhsMacro", vhsMacro.value);
    shader.setUniform1f("vhsStrength", vhsStrength.value);
    shader.setUniform1f("vhsSpeed", vhsSpeed.value);
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

    asciiShader.setUniform1f("fxMacro", fxMacro.value);
    asciiShader.setUniform1f("asciiMacro", asciiMacro.value);
    asciiShader.setUniform1f("asciiDotDistance", asciiDotDistance.value);
    asciiShader.setUniform1f("asciiImageGain", asciiImageGain.value);
    asciiShader.setUniform1f("asciiImageContrast", asciiImageContrast.value);
    asciiShader.setUniform1i("asciiInvert", int(asciiInvert));
    asciiShader.setUniformTexture("font", font, 8);

    fbo2.draw(0,0, getWidth, getHeight);              //FBO 2 draw

    asciiShader.end();                                //ASCII Shader end
    fbo3.end();                                       //FBO 3 end

//-------LED FBO/Shader---------------------------------------------------------
    fbo4.begin();                                       //FBO 4 begin
    ofClear(0,0,0,0);
    ledShader.begin();                                 //LED Shader begin

    ledShader.setUniform1f("fxMacro", fxMacro.value);
    ledShader.setUniform1f("ledMacro", ledMacro.value);
    ledShader.setUniform1f("ledDotDistance", ledDotDistance.value);
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
                    
                default:
                    for(int i=0;i<fxByCC[msg.control].size();i++)
                    {
                        fxByCC[msg.control][i]->onMidiMessage(msg.value);
                    }
//                case 15:
//
//                    break;
//                case 16:
//                    videoSpeed2=ofMap(msg.value, 0, 127, .1, 10.00);
//                    videoSpeedSlider->setValue(videoSpeed2);
//                    break;
//                case 17:
//                    videoDivision=msg.value;
//                    videoDivisionSlider->setValue(videoDivision);
//                    break;
//                    //            case 19:
//                    //                if(msg.value>63) backgroundSwitch=true;
//                    //                else backgroundSwitch=false;
//                    //                backgroundSwitchToggle->setChecked(backgroundSwitch);
//                    //                break;
//                case 18:
//                    if(msg.value>63) videoSync=true;
//                    else videoSync=false;
//                    videoSyncToggle->setChecked(videoSync);
//                    break;
//                case 20:
//                    tempoDivision=msg.value;
//                    tempoDivisionSlider->setValue(tempoDivision);
//                    break;
//                case 21:
//                    if(msg.value>63)triplet=true;
//                    else triplet=false;
//                    tripletToggle->setChecked(triplet);
//                    break;
//                    //            case 21:
//                    //                bgColor1=ofFloatColor(ofMap(msg.value,0, 127, 0.0, 1.0));
//                    //                bgColor1ColorPicker->setColor(bgColor1);
//                    //                break;
//
//                    //EMPTY CASE 22
//
//                case 24:
//                    invertMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
//                    invertSlider->setValue(invertMacro);
//                    break;
//                case 25:
//                    rippleMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
//                    rippleSlider->setValue(rippleMacro);
//                    break;
//                case 26:
//                {
//                    if(msg.value>63) rippleSync=true;
//                    else rippleSync=false;
//                    rippleSyncToggle->setChecked(rippleSync);
//                    break;
//                }
//                case 27:
//                    rippleX=ofMap(msg.value,0, 127, 0.0, 1.0);
//                    rippleXSlider->setValue(rippleX);
//                    break;
//                case 28:
//                    rippleY=ofMap(msg.value,0, 127, 0.0, 1.00);
//                    rippleYSlider->setValue(rippleY);
//                    break;
//                case 29:
//                    if(rippleSync) rippleRate=bpm/60;
//                    else rippleRate=ofMap(msg.value,0, 127, .1, 300);
//                    rippleRateSlider->setValue(rippleRate);
//                    break;
//
//                case 30:
//                    filterMacro=ofMap(msg.value,0, 127, 0, 1.0);
//                    filterSlider->setValue(filterMacro);
//                    break;
//                case 31:
//                    filterRed=ofMap(msg.value,0, 127, 0, 1.0);
//                    filterRedSlider->setValue(filterRed);
//                    break;
//                case 33:
//                    filterGreen=ofMap(msg.value,0, 127, 0, 1.0);
//                    filterGreenSlider->setValue(filterGreen);
//                    break;
//                case 34:
//                    filterBlue=ofMap(msg.value,0, 127, 0, 1.0);
//                    filterBlueSlider->setValue(filterBlue);
//                    break;
//                    //EMPTY CASE 35
//
//                case 40:
//                    kaleidoscopeMacro=ofMap(msg.value,0, 127, 0.0, 1.0);
//                    kaleidoscopeSlider->setValue(kaleidoscopeMacro);
//                    break;
//                case 41:
//                    kaleidioscopeAngle=ofMap(msg.value,0, 127, -180.0, 180.0);
//                    angleSlider->setValue(kaleidioscopeAngle);
//                    break;
//                case 42:
//                    kaleiodioscopeX=ofMap(msg.value,0, 127, 0.0, 1.00);
//                    xSlider->setValue(kaleiodioscopeX);
//                    break;
//                case 43:
//                    kaleiodioscopeY=ofMap(msg.value,0, 127, 0.0, 1.00);
//                    ySlider->setValue(kaleiodioscopeY);
//                    break;
//                case 44:
//                    kaleidioscopeSectors=ofMap(msg.value, 0, 127, 1.0, 100.0);
//                    sectorSlider->setValue(kaleidioscopeSectors);
//                    break;
//                case 50:
//                    bgColor1Red=msg.value;
//                    bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    bgColor1ColorPicker->setColor(bgColor1);
//                    break;
//                case 51:
//                    bgColor1Green=msg.value;
//                    bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    bgColor1ColorPicker->setColor(bgColor1);
//                    break;
//                case 52:
//                    bgColor1Blue=msg.value;
//                    bgColor1.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    bgColor1ColorPicker->setColor(bgColor1);
//                    break;
//                    //                case 53:
//                    //                    bgColor2Red=msg.value;
//                    //                    bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    //                    bgColor2ColorPicker->setColor(bgColor2);
//                    //                    break;
//                    //                case 54:
//                    //                    bgColor2Green=msg.value;
//                    //                    bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    //                    bgColor2ColorPicker->setColor(bgColor2);
//                    //                    break;
//                    //                case 55:
//                    //                    bgColor2Blue=msg.value;
//                    //                    bgColor2.set(bgColor1Red,bgColor1Green,bgColor1Blue);
//                    //                    bgColor2ColorPicker->setColor(bgColor2);
//                    //                    break;
//                case 56:
//                    pixelateMacro=ofMap(msg.value,0, 127, 0, 100);
//                    pixelateSlider->setValue(pixelateMacro);
//                    break;
//                case 57:
//                    fullhouseMacro=ofMap(msg.value,0,127,1,50);
//                    fullhouseSlider->setValue(fullhouseMacro);
//                    break;
//                case 60:
//                    asciiMacro=ofMap(msg.value,0,127,0.0,1.0);
//                    asciiMacroSlider->setValue(asciiMacro);
//                    break;
//                case 61:
//                    asciiDotDistance=ofMap(msg.value, 0, 127, 0.0, 1.0);
//                    asciiDotDistanceSlider->setValue(asciiDotDistance);
//                    break;
//                case 62:
//                    asciiImageGain=ofMap(msg.value, 0, 127, 0.0, 1.0);
//                    asciiImageGainSlider->setValue(asciiImageGain);
//                    break;
//                case 63:
//                    asciiImageContrast=ofMap(msg.value,0,127,0.0,1.0);
//                    asciiImageContrastSlider->setValue(asciiImageContrast);
//                    break;
//                case 65:
//                    if(msg.value>63) asciiInvert=true;
//                    else asciiInvert=false;
//                    asciiInvertToggle->setChecked(asciiInvert);
//                    break;
//                case 66:
//                    ledMacro=ofMap(msg.value,0,127,0.0,1.0);
//                    ledMacroSlider->setValue(ledMacro);
//                    break;
//                case 67:
//                    ledDotDistance=ofMap(msg.value, 0, 127, 0.0, 1.0);
//                    ledDotDistanceSlider->setValue(ledDotDistance);
//                    break;
//                    //            case 68:
//                    //                ledOffsetRX=ofMap(msg.value,0,127,0.0,100.0);
//                    //                ledOffsetRXSlider->setValue(ledOffsetRX);
//                    //                break;
//                    //            case 69:
//                    //                ledOffsetRY=ofMap(msg.value, 0, 127, 0.0, 100.0);
//                    //                ledOffsetRYSlider->setValue(ledOffsetRY);
//                    //                break;
//                    //            case 70:
//                    //                ledOffsetGX=ofMap(msg.value, 0, 127, 0.0, 100.0);
//                    //                ledOffsetGXSlider->setValue(ledOffsetGX);
//                    //                break;
//                    //            case 71:
//                    //                ledOffsetGY=ofMap(msg.value, 0, 127, 0.0, 100.0);
//                    //                ledOffsetGYSlider->setValue(ledOffsetGY);
//                    //                break;
//                    //            case 72:
//                    //                ledOffsetBX=ofMap(msg.value, 0, 127, 0.0, 100.0);
//                    //                ledOffsetBXSlider->setValue(ledOffsetBX);
//                    //                break;
//                    //            case 73:
//                    //                ledOffsetGY=ofMap(msg.value, 0, 127, 0.0, 100.0);
//                    //                ledOffsetGYSlider->setValue(ledOffsetGY);
//                    //                break;
//                case 74:
//                    rotateMacro=ofMap(msg.value,0,127,0.0,1.0);
//                    rotateMacroSlider->setValue(rotateMacro);
//                    break;
//                case 75:
//                    zebraMacro=ofMap(msg.value,0,127,0.0,1.0);
//                    zebraMacroSlider->setValue(zebraMacro);
//                    break;
//                case 76:
//                    zebraSpeed=ofMap(msg.value,0,127,0.0,1.0);
//                    zebraSpeedSlider->setValue(zebraSpeed);
//                    break;
//
//                case 77:
//                    zebraLevels=ofMap(msg.value, 0, 127, 2, 50);
//                    zebraLevelsSlider->setValue(zebraLevels);
//                    break;
//                case 78:
//                    chromaKeyMacro=ofMap(msg.value,0,127,0.0,1.0);
//                    chromaKeyMacroSlider->setValue(chromaKeyMacro);
//                    break;
//                case 79:
//                    chromaKeyColor.setRed(ofMap(msg.value,0,127,0,255));
//                    chromaKeyColorPicker->setColor(ofColor(chromaKeyColor.getRedInt(),chromaKeyColor.getGreenInt(),chromaKeyColor.getBlueInt()));
//                    break;
//                case 80:
//                    chromaKeyGreen=ofMap(msg.value,0,127,0,255);
//                    chromaKeyColorPicker->setColor(ofColor(chromaKeyColor.getRedInt(),chromaKeyColor.getGreenInt(),chromaKeyColor.getBlueInt()));
//                    break;
//                case 81:
//                    chromaKeyBlue=ofMap(msg.value,0,127,0,255);
//                    chromaKeyColorPicker->setColor(ofColor(chromaKeyColor.getRedInt(),chromaKeyColor.getGreenInt(),chromaKeyColor.getBlueInt()));
//                    break;
//                case 82:
//                    chromaKeyThreshold=ofMap(msg.value, 0, 127, 0, 255);
//                    chromaKeyThresholdSlider->setValue(chromaKeyThreshold);
//                    break;
//                case 83:
//                    squareioscopeMacro=ofMap(msg.value, 0, 127, 0.0, 1.0);
//                    squareioscopeMacroSlider->setValue(squareioscopeMacro);
//                    break;
//                case 84:
//                    squareioscopeMacro2=ofMap(msg.value, 0, 127, 0.0, 1.0);
//                    squareioscopeMacroSlider->setValue(squareioscopeMacro2);
//                    break;
//                case 85:
//                    vhsMacro=ofMap(msg.value, 0, 127, 0.0, 1.0);
//                    vhsMacroSlider->setValue(vhsMacro);
//                    break;
//                case 86:
//                    vhsStrength=ofMap(msg.value,0,127,0.0,1.0);
//                    vhsStrengthSlider->setValue(vhsStrength);
//                    break;
//                case 87:
//                    vhsSpeed=ofMap(msg.value, 0, 127, 0.0, 60.0);
//                    vhsSpeedSlider->setValue(vhsSpeed);
//                    break;
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

void ofApp::onAddNewLightEventGui3(ofxModalEvent e)//==========================
{
     if (e.type == ofxModalEvent::CANCEL) cout << "cancel button was selected" << endl;
     else if (e.type == ofxModalEvent::CONFIRM)
     {
         cout << "confirm button was selected" << endl;
         addLight();
     }
}

void ofApp::onRightClickEventGui3(ofxDatGuiRightClickEvent e)//============================================================
{
    if(e.target->getType()==ofxDatGuiType::FOLDER)
    {
        int l = e.target->getCount();
        mEditLight.show(&(rig[e.target->getCount()-1]));  //show editLight dialog according to count of the target
        cout<<"right click:"+e.target->getLabel()+"\n";
    }
}

void ofApp::onSliderEventGui3(ofxDatGuiSliderEvent e)//============================================================
{
    int toggle = e.target->getCount();
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

void ofApp::onButtonEventGui3(ofxDatGuiButtonEvent e)//============================================================
{
    if(e.target==addLightButton) mAddNewLight.show(getRigSize(), getRigSize()); //clear light first and pass in midiCCStart and dmxChannelStart
}

void ofApp::onToggleEvent(ofxDatGuiToggleEvent e)//============================================================
{//--------Toggle event handler function----------------------

    if (e.target==tripletToggle)triplet=!triplet;
//    else if (e.target->is("Background Switch"))backgroundSwitch=!backgroundSwitch;
    else if (e.target==videoSyncToggle)videoSync=!videoSync;
    
    else if(e.target==clearToggle) clear=!clear;
    
    else if(e.target==rippleSyncToggle)rippleRate.value=bpm*60;
    
    else if(e.target==asciiInvertToggle)asciiInvert=!asciiInvert;
}

void ofApp::onToggleEventGui3(ofxDatGuiToggleEvent e)//============================================================
{
    int toggle = e.target->getCount();
}

void ofApp::onTextInputEventGui1(ofxDatGuiTextInputEvent e)
{
    if(e.type==ofxDatGuiNumberBoxEventType::NUMBERBOX)
    {   cout<<"NumberBox event \n";
    }
}

void ofApp::onTextInputEventGui3(ofxDatGuiTextInputEvent e)
{
    if(e.type==ofxDatGuiNumberBoxEventType::NUMBERBOX)cout<<"Text input number event\n";
    if(e.type==ofxDatGuiNumberBoxEventType::INPUT) cout<<"mInput event \n";
}

void ofApp::onNumberBoxChangedEventGui1(ofxDatGuiNumberBoxChangedEvent e)
{
    cout<<"Evan's fun NumberBox Event \n";
    
    if(!validMidiCC(e.value))
    {
        e.target->setNumberBoxNumber(e.target->getPreviousNumberBoxNumber());
        return;
    }
    removeFxParameter(e.target, e.target->getPreviousNumberBoxNumber());
    fxByCC[e.target->getNumberBoxNumber()].push_back(getFxParameter(e.target));
}

ofApp::fxParameter* ofApp::getFxParameter(ofxDatGuiComponent *e)
{
    for(int i=0;i<127;i++)
    {
        for(int j=0;j<fxByCC[i].size();j++)
        {
            if(fxByCC[i][j]->compare(e))return fxByCC[i][j];
        }
    }
    cout<<"No cooresponding fx found\n";
    return 0;
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

 void ofApp::removeFxParameter(ofxDatGuiComponent *e, int previous)
{
    for(int i=0;i<fxByCC[previous].size();i++)
    {
        if(fxByCC[previous][i]->compare(e))
        {
            fxByCC[previous].erase(fxByCC[previous].begin()+i);
            cout<<"fxParameter added at "<<previous<< ", "<< i <<"\n";
        }
    }
}



void ofApp::onSliderEvent(ofxDatGuiSliderEvent e)//============================================================
{
    //--------Slider event handler function----------------------
    
    if(e.target==tempoDivisionSlider)tempoDivision=(e.target->getValue());
    if(e.target==videoDivisionSlider)videoDivision=(e.target->getValue());
    if(e.target==fxMacro.slider)fxMacro.value=(e.target->getValue());
    
    else if(e.target==kaleidoscopeMacro.slider)kaleidoscopeMacro.value = (e.target->getValue());
    else if(e.target==kaleidioscopeAngle.slider)kaleidioscopeAngle.value=(e.target->getValue());
    else if(e.target==kaleiodioscopeX.slider)kaleiodioscopeX.value=(e.target->getValue());
    else if(e.target==kaleiodioscopeY.slider)kaleiodioscopeY.value=(e.target->getValue());
    else if(e.target==kaleidioscopeSectors.slider)kaleidioscopeSectors.value=(e.target->getValue());
    
    else if(e.target==filterMacro.slider)filterMacro.value=(e.target->getValue());
    else if(e.target==filterRed.slider)filterRed.value=(e.target->getValue());
    else if(e.target==filterBlue.slider)filterBlue.value=(e.target->getValue());
    else if(e.target==filterGreen.slider)filterGreen.value=(e.target->getValue());
    
    else if(e.target==rippleMacro.slider)rippleMacro.value=(e.target->getValue());
    else if(e.target==rippleY.slider)rippleY.value=(e.target->getValue());
    else if(e.target==rippleX.slider)rippleX.value=(e.target->getValue());
    else if(e.target==rippleRate.slider)
    {
        if(rippleSync==false)rippleRate.value=(e.target->getValue());
    }
    
    else if(e.target==invertMacro.slider)invertMacro.value=(e.target->getValue());
    
    else if(e.target==videoSpeedSlider)videoSpeed2=(e.target->getValue());
    
    else if(e.target==pixelateMacro.slider)pixelateMacro.value=(e.target->getValue());
    
    else if(e.target==fullhouseMacro.slider)fullhouseMacro.value=(e.target->getValue());
    
    else if(e.target==asciiMacro.slider)asciiMacro.value=(e.target->getValue());
    else if(e.target==asciiImageGain.slider)asciiImageGain.value=(e.target->getValue());
    else if(e.target==asciiImageContrast.slider)asciiImageContrast.value=(e.target->getValue());
    else if(e.target==asciiDotDistance.slider)asciiDotDistance.value=(e.target->getValue());

    else if(e.target==ledMacro.slider)ledMacro.value=(e.target->getValue());
    else if(e.target==ledDotDistance.slider)ledDotDistance.value=(e.target->getValue());
    else if(e.target==ledOffsetRX.slider)ledOffsetRX.value=(e.target->getValue());
    else if(e.target==ledOffsetRY.slider)ledOffsetRY.value=(e.target->getValue());
    else if(e.target==ledOffsetGX.slider)ledOffsetGX.value=(e.target->getValue());
    else if(e.target==ledOffsetGY.slider)ledOffsetGY.value=(e.target->getValue());
    else if(e.target==ledOffsetBX.slider)ledOffsetBX.value=(e.target->getValue());
    else if(e.target==ledOffsetBY.slider)ledOffsetBY.value=(e.target->getValue());
    
    else if(e.target==rotateMacro.slider)rotateMacro.value=(e.target->getValue());
    
    else if(e.target==zebraMacro.slider)zebraMacro.value=(e.target->getValue());
    else if(e.target==zebraLevels.slider)zebraLevels.value=(e.target->getValue());
    else if(e.target==zebraSpeed.slider)zebraSpeed.value=(e.target->getValue());
    
    else if(e.target==squareioscopeMacro.slider)squareioscopeMacro.value=(e.target->getValue());
    else if(e.target==squareioscopeMacro2.slider)squareioscopeMacro2.value=(e.target->getValue());
    
    else if(e.target==vhsMacro.slider)vhsMacro.value=(e.target->getValue());
    else if(e.target==vhsStrength.slider)vhsStrength.value=(e.target->getValue());
    else if(e.target==vhsSpeed.slider)vhsSpeed.value=(e.target->getValue());
    
    else if(e.target==chromaKeyMacro.slider)chromaKeyMacro.value=(e.target->getValue());
    else if(e.target==chromaKeyThreshold.slider)chromaKeyThreshold.value=(e.target->getValue());
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

void ofApp::onEditLightEventGui3(ofxModalEvent e)//============================================================
{
    if(e.type==ofxModalEvent::SHOWN) disableGuis();
    if(e.type==ofxModalEvent::HIDDEN) enableGuis();
    if(e.type==ofxModalEvent::DELETE)
    {
        light* a=mEditLight.getLight();
        deleteLight(a);
    }
    cout<<"DELETE event sent \n";
    if (e.type == ofxModalEvent::CANCEL) cout << "cancel button was selected" << endl;
    else if (e.type == ofxModalEvent::CONFIRM)
    {
        cout << "confirm button was selected" << endl;
        editLight();
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
        xmlSettings.loadFile(loadPath);
        if(xmlSettings.loadFile(loadPath))
        {
            fxMacro.value = xmlSettings.getValue("xmlSettings:fxWet:fxWet", 1.0);
            fxMacro.slider->setValue(fxMacro.value);
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

            invertMacro.value=xmlSettings.getValue("xmlSettings:invert:invert", 0);
            invertMacro.slider->setValue(invertMacro.value);
            
            rippleMacro.value=xmlSettings.getValue("xmlSettings:ripple:ripple", 0);
            rippleMacro.slider->setValue(rippleMacro.value);
            rippleSync=xmlSettings.getValue("xmlSettings:ripple:sync", 0);
            rippleSyncToggle->setChecked(rippleSync);
            rippleX.value=xmlSettings.getValue("xmlSettings:ripple:x", 0.0);
            rippleX.slider->setValue(rippleX.value);
            rippleY.value=xmlSettings.getValue("xmlSettings:ripple:y", 0.0);
            rippleY.slider->setValue(rippleY.value);
            rippleRate.value=xmlSettings.getValue("xmlSettings:ripple:rate",60);
            rippleRate.slider->setValue(rippleRate.value);
            
            filterMacro.value=xmlSettings.getValue("xmlSettings:filter:filter", 0.0);
            filterMacro.slider->setValue(filterMacro.value);
            filterRed.value=xmlSettings.getValue("xmlSettings:filter:red",0.0);
            filterRed.slider->setValue(filterRed.value);
            filterGreen.value=xmlSettings.getValue("xmlSettings:filter:green",0.0);
            filterGreen.slider->setValue(filterGreen.value);
            filterBlue.value=xmlSettings.getValue("xmlSettings:filter:blue",0.0);
            filterBlue.slider->setValue(filterBlue.value);
            
            kaleidoscopeMacro.value=xmlSettings.getValue("xmlSettings:kaleidoscope:kaleidoscope", 0.0);
            kaleidoscopeMacro.slider->setValue(kaleidoscopeMacro.value);
            kaleidioscopeSectors.value=xmlSettings.getValue("xmlSettings:kaleidoscope:sectors", 1);
            kaleidioscopeSectors.slider->setValue(kaleidioscopeSectors.value);
            kaleidioscopeAngle.value=xmlSettings.getValue("xmlSettings:kaleidoscope:angle", 0.0);
            kaleidioscopeAngle.slider->setValue(kaleidioscopeAngle.value);
            kaleiodioscopeX.value=xmlSettings.getValue("xmlSettings:kaleidoscope:x", 0.0);
            kaleiodioscopeX.slider->setValue(kaleiodioscopeX.value);
            kaleiodioscopeY.value=xmlSettings.getValue("xmlSettings:kaleidoscope:y", 0.0);
            kaleiodioscopeY.slider->setValue(kaleiodioscopeY.value);

            pixelateMacro.value=xmlSettings.getValue("xmlSettings:pixelate:pixelate", 0.0);
            pixelateMacro.slider->setValue(pixelateMacro.value);
            
            fullhouseMacro.value=xmlSettings.getValue("xmlSettings:fullhouse:fullhouse", 0.0);
            fullhouseMacro.slider->setValue(fullhouseMacro.value);
            
            asciiMacro.value=xmlSettings.getValue("xmlSettings:ascii:ascii", 0.0);
            asciiMacro.slider->setValue(asciiMacro.value);
            asciiImageContrast.value=xmlSettings.getValue("xmlSettings:ascii:imageContrast", 0.0);
            asciiImageContrast.slider->setValue(asciiImageContrast.value);
            asciiImageGain.value=xmlSettings.getValue("xmlSettings:ascii:imageGain", 0.0);
            asciiImageGain.slider->setValue(asciiImageGain.value);
            asciiDotDistance.value=xmlSettings.getValue("xmlSettings:ascii:dotDistance", 0.0);
            asciiDotDistance.slider->setValue(asciiDotDistance.value);
            
            ledMacro.value=xmlSettings.getValue("xmlSettings:led:led", 0.0);
            ledMacro.slider->setValue(ledMacro.value);
            ledDotDistance.value=xmlSettings.getValue("xmlSettings:led:dotDistance", 0.0);
            ledDotDistance.slider->setValue(ledDotDistance.value);
            
            rotateMacro.value=xmlSettings.getValue("xmlSettings:rotate:rotateMacro", 0.0);
            rotateMacro.slider->setValue(rotateMacro.value);
            
            zebraMacro.value=xmlSettings.getValue("xmlSettings:zebra:zebraMacro", 0.0);
            zebraMacro.slider->setValue(zebraMacro.value);
            zebraSpeed.value=xmlSettings.getValue("xmlSettings:zebra:zebraSpeed", 0.0);
            zebraSpeed.slider->setValue(zebraSpeed.value);
            zebraLevels.value=xmlSettings.getValue("xmlSettings:zebra:zebraLevels", 2);
            zebraLevels.slider->setValue(zebraLevels.value);
            
            squareioscopeMacro.value=xmlSettings.getValue("xmlSettings:square:squareioscope", 0.0);
            squareioscopeMacro.slider->setValue(squareioscopeMacro.value);
            squareioscopeMacro2.value=xmlSettings.getValue("xmlSettings:square:squareioscope2", 0.0);
            squareioscopeMacro2.slider->setValue(squareioscopeMacro2.value);
            
            vhsMacro.value=xmlSettings.getValue("xmlSettings:vhs:vhsMacro",0.0);
            vhsMacro.slider->setValue(vhsMacro.value);
            vhsSpeed.value=xmlSettings.getValue("xmlSettings:vhs:vhsSpeed",0.0);
            vhsSpeed.slider->setValue(vhsSpeed.value);
            vhsStrength.value=xmlSettings.getValue("xmlSettings:vhs:vhsStrength", 0.0);
            vhsStrength.slider->setValue(vhsStrength.value);
            
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
        
        xmlSettings.setValue("xmlSettings:fxWet:fxWet", fxMacro.value);
        xmlSettings.setValue("xmlSettings:video:speed", videoSpeed);
        xmlSettings.setValue("xmlSettings:video:sync", videoSync);
        xmlSettings.setValue("xmlSettings:video:division", videoDivision);
        xmlSettings.setValue("xmlSettings:video:triplet", triplet);
        xmlSettings.setValue("xmlSettings:video:tempoDivision", tempoDivision);
        
        xmlSettings.setValue("xmlSettings:color:bgColor1Red", bgColor1Red);
        xmlSettings.setValue("xmlSettings:color:bgColor1Green", bgColor1Green);
        xmlSettings.setValue("xmlSettings:color:bgColor1Blue", bgColor1Blue);
        
        xmlSettings.setValue("xmlSettings:invert:invert", invertMacro.value);
        
        xmlSettings.setValue("xmlSettings:ripple:ripple", rippleMacro.value);
        xmlSettings.setValue("xmlSettings:ripple:sync", rippleSync);
        xmlSettings.setValue("xmlSettings:ripple:x", rippleX.value);
        xmlSettings.setValue("xmlSettings:ripple:y", rippleY.value);
        xmlSettings.setValue("xmlSettings:ripple:rate", rippleRate.value);
        
        xmlSettings.setValue("xmlSettings:filter:filter", filterMacro.value);
        xmlSettings.setValue("xmlSettings:filter:red", filterRed.value);
        xmlSettings.setValue("xmlSettings:filter:green", filterGreen.value);
        xmlSettings.setValue("xmlSettings:filter:blue", filterBlue.value);
        
        xmlSettings.setValue("xmlSettings:kaleidoscope:kaleidoscope",kaleidoscopeMacro.value);
        xmlSettings.setValue("xmlSettings:kaleidoscope:sectors",kaleidioscopeSectors.value);
        xmlSettings.setValue("xmlSettings:kaleidoscope:angle",kaleidioscopeAngle.value);
        xmlSettings.setValue("xmlSettings:kaleidoscope:x",kaleiodioscopeX.value);
        xmlSettings.setValue("xmlSettings:kaleidoscope:y",kaleiodioscopeY.value);

        xmlSettings.setValue("xmlSettings:pixelate:pixelate", pixelateMacro.value);
        
        xmlSettings.setValue("xmlSettings:fullhouse:fullhouse", fullhouseMacro.value);
        
        xmlSettings.setValue("xmlSettings:ascii:ascii", asciiMacro.value);
        xmlSettings.setValue("xmlSettings:ascii:imageContrast", asciiImageContrast.value);
        xmlSettings.setValue("xmlSettings:ascii:imageGain", asciiImageGain.value);
        xmlSettings.setValue("xmlSettings:ascii:dotDistance", asciiDotDistance.value);

        xmlSettings.setValue("xmlSettings:led:led", ledMacro.value);
        xmlSettings.setValue("xmlSettings:led:dotDistance", ledDotDistance.value);
        
        xmlSettings.setValue("xmlSettings:rotate:rotateMacro",rotateMacro.value);
        
        xmlSettings.setValue("xmlSettings:zebra:zebraMacro", zebraMacro.value);
        xmlSettings.setValue("xmlSettings:zebra:zebraLevels", zebraLevels.value);
        xmlSettings.setValue("xmlSettings:zebra:zebraSpeed", zebraSpeed.value);
        
        xmlSettings.setValue("xmlSettings:square:squareioscope", squareioscopeMacro.value);
        xmlSettings.setValue("xmlSettings:square:squareioscope2", squareioscopeMacro2.value);
        
        xmlSettings.setValue("xmlSettings:vhs:vhsMacro", vhsMacro.value);
        xmlSettings.setValue("xmlSettings:vhs:vhsStrength", vhsStrength.value);
        xmlSettings.setValue("xmlSettings:vhs:vhsSpeed",vhsSpeed.value);
        
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

void ofApp::deleteLight(light* a)//============================================================
{
    gui3->removeFolder(a->name);    //to delete the components inside of the folder
    rig.erase(rig.begin()+a->count-1);  //delete light from the rig
    
}

int ofApp::getRigSize()//============================================================
{
    int rigSize=0;
    for(int i=0;i<rig.size();i+=1)
    {
        rigSize+=rig[i].values.size();
    }
    return rigSize;
}

void ofApp::addLight()//============================================================
{
    light *a = mAddNewLight.getLight();
    ofxDatGuiFolder *folder=gui3->addFolder(a->name);
    for(int i=0;i<a->values.size();i+=1)
    {
        a->values[i].value.set(a->values[i].parameter,120,0,255);
        folder->addSlider(a->values[i].value,true,a->midiCCStart+i);
    }
    runningCount=runningCount+1;
    a->count=runningCount;
    folder->setCount(runningCount);
    rig.push_back(*a);
}

void ofApp::editLight()//============================================================
{
    light *a = mEditLight.getLight();
    int existingSliders=rig[a->count-1].numberOfChannels;
    rig[a->count-1] = *a;
    ofxDatGuiFolder *folder = gui3->getFolder(a->name);
    for(int i=0;i<existingSliders;i++)
    {
        folder->children.pop_back();
    }
    for(int i=0;i<a->values.size();i+=1)
    {
        a->values[i].value.set(a->values[i].parameter,120,0,255);
        folder->addSlider(a->values[i].value,true,a->values[i].midiCC);
    }
    a->count=runningCount;
    folder->setCount(runningCount);
    folder->collapse();
}

