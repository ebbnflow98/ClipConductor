#pragma.once
#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxHapPlayer.h"
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"
#include "convertVideo.h"
#include "evanColor.h"
#include "modalDialogs.h"
#include "ofxDmx.h"


class ofApp : public ofBaseApp,
public ofxMidiListener
{
    public:

    struct media
    {
        bool which;
        bool full;
        ofxHapPlayer video;
        ofTexture picture;
        string path;
        bool drawImage;
        float size;
        int opacity;
        
        media()
        {
            full=false;
            which=NULL;
            drawImage=NULL;
            path="";
            size=63;
            opacity=255;
        }
        
        bool load(string loadPath)
        {
            cout<<"load \n";
            if(loadPath.find(".mov")!=string::npos)
                which=true;
            else if(loadPath.find(".MOV")!=string::npos)
                which=true;
            else if (loadPath.find(".png")!=string::npos)
                which=false;
            else if(loadPath.find(".jpg")!=string::npos)
                which=false;
            else if(loadPath.find(".jpeg")!=string::npos)
            { which=false;}
            else
            {
                cout<<"load failed; which=NULL \n";
                ofSystemAlertDialog("Load failed. File type not supported.");
                close();
                return false;
            }
            
            if(which)
            {
                video.videoIsntHap=false;
                cout<<"load up those videos \n";
                video.load(loadPath);
                while(!video.isLoaded()) if(video.videoIsntHap==true) break;
                bool balls=(video.isLoaded());
                if (video.videoIsntHap)
                {
                    
                    ofFile input;
                    input.open(loadPath);
                    string convertedVideoPath = convert(loadPath);
                    if(convertedVideoPath!="")
                    {
                        close();
                        load(convertedVideoPath);
                        
                        return true;
                    }
                    else
                    {
                        ofSystemAlertDialog("Load failed. Video file is not HAP encoded and video conversion failed");
                        return false;
                    }
                }
            }
            else if(!which)
            {
                ofLoadImage(picture,loadPath);
                picture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
            }

            path=loadPath;
            full=true;
            cout<<"path:"<<path<<endl;
            return true;
        }
        void play()
        {
//            cout<<"play \n";
            if(which)video.play();
        }
        void stop()
        {
            if(which)video.stop();
        }
        void draw(int x, int y, int width, int height)
        {
//            cout<<"draw\n";
            if(!full) return;
            
            if(which)video.draw(x,y,width,height);
            else picture.draw(x,y,width, height);
        }
        void firstFrame()
        {
            if(which)video.firstFrame();
        }
        void setSpeed(float speed)
        {
            if(which)video.setSpeed(speed);
        }
        string getPath()
        {
            return(path);
        }
        bool isLoaded()
        {
            if(which)return(video.isLoaded());
            else return false;
        }
        void close()
        {
            if(which)video.close();
            else picture.clear();
            path="";
            which=NULL;
        }
        bool isPlaying()
        {
            if(which)return video.isPlaying();
            else if(which==false) return true;
            else return false;
        }
        void setLoopState(ofLoopType state)
        {
            if(which)video.setLoopState(state);
        }
        
    } player[100];

//=========================================================
    

    void setup();
    void update();
    void draw();
    void exit();
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void keyPressed(ofKeyEventArgs & args);
    void keyReleased(ofKeyEventArgs & args);
    bool loadMovie(int i);
    bool midiPort(int midiPortOption);
    
    void clearAllVideos();
    void clearAllLights();
    void midiNoteOff(int pitch);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void onButtonEventVideo(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);
    void onTextInputEventGui1(ofxDatGuiTextInputEvent e);
    bool validMidiCC(int cc);

    void dragEvent(ofDragInfo & info);
    bool loadSettings();
    bool saveSettings();
    void exitGui(ofEventArgs & args);
    void onButtonEventGui2(ofxDatGuiButtonEvent e);
    void onButtonEventGui3(ofxDatGuiButtonEvent e);
    void windowResized(ofResizeEventArgs &e);
    void allocateFBOs();
    void enableGuis();
    void disableGuis();
    void changeDevices(int choice);
    void newProject();
    
//-------------------GUI 3--------------------------------
    void onTextInputEventGui3(ofxDatGuiTextInputEvent e);
//    void onRightClickEventGui3(ofxDatGuiRightClickEvent e);
    void onSliderEventGui3(ofxDatGuiSliderEvent e);
//------------------------------------------------------------------------
    ofFbo fbo,fbo2,fbo3,fbo4,chromaKeyVideoFbo,chromaKeyFxFbo, blendFbo;
    ofShader fxShader, asciiShader, ledShader, chromaKeyShader;
    
    bool exiting=false;
    float guiHeight=0.0;
    bool videoSync=false;
    float videoSpeed=1;
    int videoDivision=1, videoOff=0;
    ofTexture videoTexture;
    ofxDatGuiThemeSpectacle *theme;
    ofxDatGuiThemeSpectacle *flip;
    
    bool sustain=false;
    bool command=false;
    int playerFromMidiMessage=0;
    
    int currentlyDrawing=-1;
    int chromaKeyVideo=-1;
    int drawCount=0;
    
    int videoNumber=0;
// ---------------------------------MIDI---------------------------------
    ofxMidiTimecode timecode; //< timecode message parser
    bool timecodeRunning = false; //< is the timecode sync running?
    long timecodeTimestamp = 0; //< when last quarter frame message was received
    ofxMidiTimecodeFrame frame; //< timecode frame data, ie. H M S frame rate
    bool tempo=1,triplet=false;
    int tempoCount=24, tempoDivisionValues[4]={48,24,16,6}, tempoDivisionValuesTriplet[4]={32,12,8,4};
    float pitch=NULL, bpm=20, bps=NULL, videoSpeed2=1;
    ofxMidiClock clock;
    int tempoDivision=1;
    
//-----Background------------------------------------------------
    ofxDatGuiColorPicker *bgColor1ColorPicker;
    ofColor bgColor1=ofColor::black;

    int bgColor1Red=0, bgColor1Green=0, bgColor1Blue=0, bgColor2Red=0, bgColor2Green=0, bgColor2Blue=0;
    bool bg=false, backgroundSwitch=false;
    int videoCount=0;
    
//-------GUI FX--------------------------------------------------------------------
    ofxDatGui* gui;
    ofxDatGui* gui2;
    bool clear=false, clearAll=false, invertColors=false;
    
    ofxDatGuiFolder *fullhouseFolder, *pixelateFolder, *kaleidioscopeFolder, *filterFolder, *rippleFolder, *invertFolder, *backgroundFolder, *asciiFolder, *rotateFolder, *zebraFolder, *chromaKeyFolder, *vhsFolder;
    
    ofxDatGuiToggle *clearToggle, *backgroundSwitchToggle, *videoSyncToggle;
    ofxDatGuiButton *newProjectButton, *clearAllButton, *saveButton, *loadButton;
    
    ofxDatGuiDropdown *midiDropdown, *usbDropdown;
    ofxDatGuiButton *refreshMidiButton, *refreshUsbButton;
    
    ofxDatGuiSlider *videoDivisionSlider, *videoSpeedSlider;
    
    ofxDatGuiFolder *videoFolder;
    ofxDatGuiButton *videoButtons[100];

    int getHeight, getWidth;
    ofPoint windowSize();
    
    ofxDatGuiButton *clearAllLightsButton;
    
    ofxDatGuiLabel *fxLabel, *videosLabel, *lightsLabel;
    
    float fxMacro=1.0;
    ofxDatGuiSlider *fxMacroSlider;
    
     float asciiMacro=0.0, asciiDotDistance=0.0, asciiImageGain=0.0, asciiImageContrast=0.0;
    bool asciiInvert=false;
    ofxDatGuiSlider *asciiMacroSlider, *asciiDotDistanceSlider, *asciiImageGainSlider, *asciiImageContrastSlider;
    ofxDatGuiToggle *asciiInvertToggle;
    ofTexture font;
    
    int pixelateMacro=0;
    ofxDatGuiSlider *pixelateMacroSlider;
    
    int fullhouseMacro=0;
    ofxDatGuiSlider *fullhouseMacroSlider;
    
    float kaleidoscopeMacro=0.0, kaleiodioscopeX=0.0, kaleiodioscopeY=0.0, kaleidioscopeAngle=0.0, kaleidioscopeSectors=1;
    ofxDatGuiSlider *kaleidoscopeMacroSlider, *kaleidoscopeAngleSlider, *kaleidoscopeXSlider, *kaleidoscopeYSlider, *kaleidoscopeSectorSlider;
    
    float filterMacro=1.0, filterRed=1.0, filterGreen=1, filterBlue=1.0, filterAlpha=1.0;
    ofxDatGuiSlider *filterMacroSlider,*filterRedSlider, *filterGreenSlider, *filterBlueSlider, *filterAlphaSlider;
    
    float rippleMacro=0.0, rippleX=1.0, rippleY=1.0, rippleRate=60.0;
    ofxDatGuiSlider *rippleMacroSlider ,*rippleXSlider, *rippleYSlider, *rippleRateSlider;
    bool rippleSync=false;
    ofxDatGuiToggle *rippleSyncToggle;
    
    float invertMacro=0.0;
    ofxDatGuiSlider *invertMacroSlider;
    
    float rotateMacro=0.0, rotateScreenCenter=0.0;
    ofxDatGuiSlider *rotateMacroSlider;
    
    float zebraMacro=0.0, zebraSpeed=0.0;
    int zebraLevels=2;
    ofxDatGuiSlider *zebraMacroSlider, *zebraSpeedSlider, *zebraLevelsSlider;
    
    float chromaKeyMacro=0.0, chromaKeyThreshold=0.0;
    ofxDatGuiSlider *chromaKeyMacroSlider, *chromaKeyThresholdSlider;
    evanColor chromaKeyColor= evanColor();
    int chromaKeyRed=0, chromaKeyGreen=255, chromaKeyBlue=0;
    ofxDatGuiColorPicker *chromaKeyColorPicker;
    
    float vhsMacro, vhsStrength, vhsSpeed;
    ofxDatGuiSlider *vhsMacroSlider, *vhsStrengthSlider, *vhsSpeedSlider;
    
//Loading/Saving----------------------------------------
    const int max_videos=100;
    ofxXmlSettings xmlSettings;
    
//---------------------------------------
    ofxDmx dmx;
    ofxDatGui *gui3;
    
    int autoCycleRed, autoCycleGreen, autoCycleBlue;
    int lightWidth;
    int runningCount=0;
    
    const int numberOfLights=100;
    int lightValues[100];                    //numberOfLights
    ofxDatGuiSlider *lightSliders[100];      //numberOfLights
    
    string noteNames[128] = {
     "C-2","C#-2","D-2","D#-2","E-2","F-2","F#-2","G-2","G#-2","A-2","A#-2","B-2",
     "C-1","C#-1","D-1","D#-1","E-1","F-1","F#-1","G-1","G#-1","A-1","A#-1","B-1",
     "C0","C#0","D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0",
     "C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1",
     "C2","C#1","D1","D#1","E1","F2","F#2","G2","G#2","A2","A#2","B2",
     "C3","C#1","D1","D#1","E1","F3","F#3","G3","G#3","A3","A#3","B3",
     "C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
     "C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5",
     "C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6",
     "C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7",
     "C8","C#8","D8","D#8","E8","F8","F#8","G8"
     };
    
private:
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
    
    int imageToDraw=NULL;
};

