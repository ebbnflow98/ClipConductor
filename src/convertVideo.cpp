//
//  convertVideo.cpp
//  MIDI Hap Player v3
//
//  Created by Evan Douglas Peters on 10/19/19.
//

//#include <stdio.h>
#include "convertVideo.h"

string convert(string input)/////////////////////////////////////////////////////////////
{
    ofFile file;
    file.open(input);
    string path = file.getAbsolutePath();
    string folder = file.getEnclosingDirectory();
    string name = file.getFileName();
    ofFileDialogResult result = ofSystemSaveDialog("hap_"+name,"Save Hap video as:");
    if(!result.bSuccess) return "";
    ofFile resultFile;
    string output = result.getPath();
    string command = "export PATH=/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/opt/X11/bin && cd data/data; ffmpeg -y -i "+path+" -c:v hap_"+output+";";
    string outputTerminal = ofSystem(command.c_str());
    
    cout<<"Output: " <<outputTerminal << "\n";
    ofSystemAlertDialog("Video encoded successfully");
    return output;
}
