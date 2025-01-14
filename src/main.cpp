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
    settings.setPosition(ofVec2f(800,0));
    settings.resizable = true;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    settings.setPosition(ofVec2f(0,0));
    settings.resizable = false;
    settings.shareContextWith = mainWindow;
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);
    guiWindow->setWindowTitle("Spectacle v2.0.1");

    shared_ptr<ofApp> mainApp(new ofApp);
    mainApp->setup();

    ofPoint Windowsize=(mainApp->windowSize());
    guiWindow->setWindowShape(Windowsize.x, Windowsize.y);
    ofAddListener(guiWindow->events().exit, mainApp.get(), &ofApp::exitGui);
    ofAddListener(guiWindow->events().fileDragEvent, mainApp.get(), &ofApp::dragEvent);
    ofAddListener(guiWindow->events().keyPressed, mainApp.get(), &ofApp::keyPressed);
    ofAddListener(guiWindow->events().keyReleased, mainApp.get(), &ofApp::keyReleased);
    ofAddListener(mainWindow->events().windowResized, mainApp.get(), &ofApp::windowResized);
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
}
