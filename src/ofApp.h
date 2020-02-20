#pragma.once
#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxHapPlayer.h"
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"
#include "convertVideo.h"


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
            
//            if(which)
//            {
//                videoTexture=video.getTexture();
//                videoTexture.setTextureWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
//            }
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
        
    } player[25];
    
    int getHeight, getWidth;
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
    void midiNoteOff(int pitch);
    void onButtonEvent(ofxDatGuiButtonEvent e);
    void onSliderEvent(ofxDatGuiSliderEvent e);
    void onColorPickerEvent(ofxDatGuiColorPickerEvent e);
    void onButtonEventVideo(ofxDatGuiButtonEvent e);
    void onToggleEvent(ofxDatGuiToggleEvent e);
    void onDropdownEvent(ofxDatGuiDropdownEvent e);

    void dragEvent(ofDragInfo & info);
    bool loadSettings();
    bool saveSettings();
    void exitGui(ofEventArgs & args);
    void onButtonEventGui2(ofxDatGuiButtonEvent e);
    void windowResized(ofResizeEventArgs &resize);
    void allocateFBOs();

    //------------------------------------------------------------------------
    ofFbo fbo,fbo2,fbo3,fbo4,chromaKeyVideoFbo,chromaKeyFxFbo;
    ofShader shader, asciiShader, ledShader, chromaKeyShader;
    
    ofPoint guiWindowSize={0.0,0.0};
    bool exiting=false;
    float guiHeight=0.0;
    bool videoSync=false;
    float videoSpeed=1;
    int videoDivision=1, videoOff=0;
    ofTexture videoTexture;
    
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
//    ofxDatGuiColorPicker *bgColor2ColorPicker;
//    ofColor bgColor2;
    int bgColor1Red=0, bgColor1Green=0, bgColor1Blue=0, bgColor2Red=0, bgColor2Green=0, bgColor2Blue=0;
    bool bg=false, backgroundSwitch=false;
    int videoCount=0;
    
    //-------GUI FX--------------------------------------------------------------------
    ofxDatGui* gui;
    ofxDatGui* gui2;
    bool clear=false, clearAll=false, invertColors=false;
    ofxDatGuiFolder *fullhouseFolder, *pixelateFolder, *kaleidioscopeFolder, *filterFolder, *rippleFolder, *invertFolder, *backgroundFolder, *ledFolder, *asciiFolder, *rotateFolder, *zebraFolder, *chromaKeyFolder;
    ofxDatGuiToggle *clearToggle, *backgroundSwitchToggle, *videoSyncToggle, *tripletToggle;
    ofxDatGuiButton *clearAllButton, *saveButton, *loadButton;
    
    ofxDatGuiDropdown *midiDropdown;
    ofxDatGuiSlider *tempoDivisionSlider, *videoDivisionSlider, *videoSpeedSlider;
    
    ofxDatGuiFolder *videoFolder;
    string videoOptions[25]={"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25"};
    ofxDatGuiButton *videoButtons[25];

    ofPoint guiPosition;
    ofPoint windowSize();
    
    float fxMacro=1.0;
    ofxDatGuiSlider *fxMacroSlider;
    
    float asciiMacro=0.0, asciiDotDistance=0.0, asciiImageGain=0.0, asciiImageContrast=0.0;
    bool asciiInvert=false;
    ofxDatGuiSlider *asciiMacroSlider, *asciiDotDistanceSlider, *asciiImageGainSlider, *asciiImageContrastSlider;
    ofxDatGuiToggle *asciiInvertToggle;
    ofTexture font;
    
    float ledMacro=0.0, ledDotDistance=0.0, ledOffsetRX=0.0, ledOffsetRY=0.0, ledOffsetGX=0.0, ledOffsetGY=0.0, ledOffsetBX=0.0, ledOffsetBY=0.0;
    ofxDatGuiSlider *ledMacroSlider, *ledDotDistanceSlider, *ledOffsetRXSlider, *ledOffsetRYSlider, *ledOffsetGXSlider, *ledOffsetGYSlider, *ledOffsetBXSlider, *ledOffsetBYSlider;
    
    int pixelateMacro=0;
    ofxDatGuiSlider *pixelateSlider;
    
    int fullhouseMacro=0;
    ofxDatGuiSlider *fullhouseSlider;
    
    float kaleidoscopeMacro=0.0, kaleiodioscopeX=0.0, kaleiodioscopeY=0.0, kaleidioscopeAngle=0.0, kaleidioscopeSectors=1;
    ofxDatGuiSlider *kaleidoscopeSlider, *angleSlider, *xSlider, *ySlider, *sectorSlider;
    
    float filterMacro=0.0, filterRed=1.0, filterGreen=1, filterBlue=1.0, filterAlpha=1.0;
    ofxDatGuiSlider *filterSlider,*filterRedSlider, *filterGreenSlider, *filterBlueSlider, *filterAlphaSlider;
    
    float rippleMacro=0.0, rippleX=1.0, rippleY=1.0, rippleRate=60.0;
    ofxDatGuiSlider *rippleSlider ,*rippleXSlider, *rippleYSlider, *rippleRateSlider;
    bool rippleSync=false;
    ofxDatGuiToggle *rippleSyncToggle;
    ofxDatGuiWaveMonitor *rippleWaveMonitor;
    
    float invertMacro=0.0;
    ofxDatGuiSlider *invertSlider;
    
    float rotateMacro=0.0, rotateScreenCenter=0.0;
    ofxDatGuiSlider *rotateMacroSlider;
    
    float zebraMacro=0.0, zebraSpeed=0.0;
    int zebraLevels=2;
    ofxDatGuiSlider *zebraMacroSlider, *zebraSpeedSlider, *zebraLevelsSlider;
    
    float chromaKeyMacro=0.0, chromaKeyThreshold=0.0;
    ofxDatGuiSlider *chromaKeyMacroSlider, *chromaKeyThresholdSlider;
    ofFloatColor chromaKeyColor= ofColor(0.0,1.0,0.0);
    int chromaKeyRed=0, chromaKeyGreen=0, chromaKeyBlue=0;
    ofxDatGuiColorPicker *chromaKeyColorPicker;
    
    //Loading/Saving----------------------------------------
    const int max_videos=25;
    ofxXmlSettings xmlSettings;
    
private:
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
    
    int imageToDraw=NULL;
};

