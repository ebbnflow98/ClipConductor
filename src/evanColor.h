//
//  evanColor.h
//  MIDI Hap Player v3
//
//  Created by Evan Douglas Peters on 3/22/20.
//

#ifndef evanColor_h
#define evanColor_h

struct evanColor
{
    evanColor()
    {
        fRed=0.0;
        fGreen=0.0;
        fBlue=0.0;
        iRed=int(ofMap(fRed,0.0,1.0,0,255));
        iGreen=int(ofMap(fGreen,0.0,1.0,0,255));
        iBlue=int(ofMap(fBlue,0.0,1.0,0,255));
    }
    void setColor(float r,float g,float b)
    {
        fRed=r;
        fGreen=g;
        fBlue=b;
        iRed=int(ofMap(fRed,0.0,1.0,0,255));
        iGreen=int(ofMap(fGreen,0.0,1.0,0,255));
        iBlue=int(ofMap(fBlue,0.0,1.0,0,255));
    }
    void setColor(int r,int g,int b)
    {
        iRed=r;
        iGreen=g;
        iBlue=b;
        fRed=float(ofMap(iRed,0,255,0.0,1.0));
        fGreen=float(ofMap(iGreen,0,255,0.0,1.0));
        fBlue=float(ofMap(iBlue,0,255,0.0,1.0));
    }
    void setRed(float r)
    {
        fRed=r;
        iRed=int(ofMap(fRed,0.0,1.0,0,255));
    }
    void setRed(int r)
    {
        iRed=r;
        fRed=float(ofMap(iRed,0,255,0.0,1.0));
    }
    float getRedFloat(){return fRed;}
    int getRedInt() {return iRed;}
    void setGreen(float g)
    {
        fGreen=g;
        iGreen=int(ofMap(fGreen,0.0,1.0,0,255));
    }
    void setGreen(int g)
    {
        iGreen=g;
        fGreen=float(ofMap(iGreen,0,255,0.0,1.0));
    }
    float getGreenFloat(){return fGreen;}
    int getGreenInt(){return iGreen;}
    void setBlue(float b)
    {
        fBlue=b;
        iBlue=int(ofMap(fBlue,0.0,1.0,0,255));
    }
    void setBlue(int b)
    {
        iBlue=b;
        fBlue=float(ofMap(iBlue,0,255,0.0,1.0));
    }
    float getBlueFloat(){return fBlue;}
    int getBlueInt(){return iBlue;}
    
private:
    float fRed=0.0,fGreen=0.0,fBlue=0.0;
    int iRed=0,iGreen=0,iBlue=0;
};

#endif /* evanColor_h */
