#pragma.once
#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxHapPlayer.h"
#include "ofxXmlSettings.h"
#include "ofxDatGui.h"

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
        
        media()
        {
            full=false;
            which=NULL;
            drawImage=NULL;
            path="";
            size=63;
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
                video.load(loadPath);
                while(!video.isLoaded())
                {
                    cout<<"\\"<< endl << "|"<<endl<<"/"<<endl<<"–"<<endl;
                    if(video.videoIsntHap==true) break;
                }
                bool balls=(video.isLoaded());
                if (video.videoIsntHap)
                {
                    cout<<"load failed; video is not HAP encoded. \n";
                    ofSystemAlertDialog("Load failed. Video file is not HAP encoded.");
                    return false;  //for now, till HAP converter works.
                }
            }
            else if(!which)ofLoadImage(picture,loadPath);
            
            path=loadPath;
            full=true;
            cout<<"path:"<<path<<endl;
            return true;
        }
        void play()
        {
            cout<<"play \n";
            if(which)video.play();
        }
        void stop()
        {
            if(which)video.stop();
        }
        void draw(int x, int y, int width, int height)
        {
            cout<<"draw\n";
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
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void newMidiMessage(ofxMidiMessage& eventArgs);
    void keyPressed(int key);
    void keyReleased(int key);
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

    //------------------------------------------------------------------------
    ofFbo fbo;
    ofShader shader;
    
    ofPoint guiWindowSize={0.0,0.0};
    bool exiting=false;
    float guiHeight=0.0;
    bool videoSync=false;
    float videoSpeed=1;
    int videoDivision=1, videoOff=0;
    
    bool sustain=false;
    
    int playerFromMessage=0;
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
    //Background------------------------------------------------
    ofxDatGuiColorPicker *bgColor1ColorPicker;
    ofColor bgColor1=ofColor::black;
    ofxDatGuiColorPicker *bgColor2ColorPicker;
    ofColor bgColor2;
    int bgColor1Red=0, bgColor1Green=0, bgColor1Blue=0, bgColor2Red=0, bgColor2Green=0, bgColor2Blue=0;
    bool bg=false, backgroundSwitch=false;
    int videoCount=0;
    
    //GUI..........................................----------
    ofxDatGui* gui;
    ofxDatGui* gui2;
    bool clear=false, clearAll=false;//, invertColors=false;
    ofxDatGuiFolder *kaleidioscopeFolder, *filterFolder, *rippleFolder, *invertFolder, *backgroundFolder;
    ofxDatGuiToggle *clearToggle, *backgroundSwitchToggle, *videoSyncToggle, *tripletButton;
    ofxDatGuiSlider *tempoDivisionSlider, *videoDivisionSlider, *fxWetSlider, *videoSpeedSlider;
    
    ofxDatGuiFolder *videoFolder;
//    ofxDatGuiScrollView *scrollVideos;
    string videoOptions[25]={"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25"};
    ofxDatGuiButton *videoButtons[25];

    ofPoint guiPosition;
    ofPoint windowSize();
    
    float fxWet=255;
    
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
    
    //Loading/Saving----------------------------------------
    const int max_videos=25;
    ofxXmlSettings xmlSettings;
    
private:
    ofxMidiIn midiIn;
    std::vector<ofxMidiMessage> midiMessages;
    std::size_t maxMessages = 10; //< max number of messages to keep track of
    
    int imageToDraw=NULL;
};



