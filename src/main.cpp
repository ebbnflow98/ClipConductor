//#include "ofMain.h"
//#include "ofApp.h"
//
////========================================================================
//int main( ){
//    ofSetupOpenGL(1024,768,OF_WINDOW);            // <-------- setup the GL context
//
//    // this kicks off the running of my app
//    // can be OF_WINDOW or OF_FULLSCREEN
//    // pass in width and height too:
//    ofRunApp(new ofApp());
//
//}
#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main(){
    ofGLFWWindowSettings settings;
    settings.setSize(1024,768);
    settings.setPosition(ofVec2f(1856,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    settings.setPosition(ofVec2f(0,0));
    settings.resizable = true;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);

    shared_ptr<ofApp> mainApp(new ofApp);
    cout << " NUMBER 1  "<< endl;
    mainApp->setup();

    ofPoint Windowsize=(mainApp->windowSize());
    guiWindow->setWindowShape(Windowsize.x, Windowsize.y);
    cout << " NUMBER 2  "<< endl;
    ofAddListener(guiWindow->events().exit, mainApp.get(), &ofApp::exitGui);
    ofAddListener(guiWindow->events().fileDragEvent, mainApp.get(), &ofApp::dragEvent);
    ofAddListener(guiWindow->events().keyPressed, mainApp.get(), &ofApp::keyPressed);
    ofAddListener(guiWindow->events().keyReleased, mainApp.get(), &ofApp::keyReleased);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
    
//    ofGLFWWindowSettings settings;
//    settings.setSize(1024,768);
//    settings.setGLVersion(3, 2);
//    settings.setPosition(ofVec2f(1856,0));
//    settings.resizable = true;
//    auto mainWin=ofCreateWindow(settings);
//    mainWin->setVerticalSync(false);
//
//    ofGLFWWindowSettings settingsGUI;
//    settingsGUI.numSamples=0;
//    settings.setGLVersion(3, 2);
//    settingsGUI.setPosition(ofVec2f(0,0));
//    settingsGUI.resizable = true;
//    settingsGUI.shareContextWith=mainWin;
//    auto guiWindow=ofCreateWindow(settingsGUI);
//
//    auto app = make_shared<ofApp>();
//    mainWin=dynamic_pointer_cast<ofAppGLFWWindow>(mainWin);
//    guiWindow=dynamic_pointer_cast<ofAppGLFWWindow>(guiWindow);
//
////    ofAddListener(guiWindow->events().draw,app.get(),&ofApp::onRenderWindowDraw);
//
//    ofRunApp(mainWin,app);
//    ofRunMainLoop();
}
