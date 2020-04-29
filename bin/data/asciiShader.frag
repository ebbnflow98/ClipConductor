//
//  asciiShader.frag
//  Clip Conductor
//
//  Created by Evan Douglas Peters on 10/20/19.
//
#ifdef GL_ES
#version 120
precision mediump float;
#endif
#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)
#extension GL_ARB_texture_rectangle: enable

uniform sampler2DRect tex0;
uniform sampler2DRect font;

uniform float asciiImageGain,asciiImageContrast,asciiDotDistance, asciiMacro;
uniform bool asciiInvert;

uniform vec2 screenCenter;
uniform float time;
uniform vec2 resolution;
uniform vec2 fontResolution;

void main()
{
    vec4 dryColor = texture2DRect(tex0,gl_TexCoord[0].xy);
    
    float imagecontrast=asciiImageContrast;
    float dotdistance=asciiDotDistance;
    
    float imagegain=asciiImageGain*2.-1.;
    float contrast = ( imagecontrast * 2. )* ( imagecontrast * 2. ) * ( imagecontrast * 2. ) * ( imagecontrast * 2. );
    float gain = imagegain - (imagecontrast -0.5) ;
    
    vec2 texcoord0=gl_TexCoord[0].xy;
    float dotdistanceX = dotdistance * 2.0;
    float dotdistanceY = dotdistance * 1.0;
    float halfdistanceX = dotdistanceX / 2.0;
    float halfdistanceY = dotdistanceY / 2.0;
    float quaterdistanceX = dotdistanceX / 4.0;
    float quaterdistanceY = dotdistanceY / 4.0;
    float baseX = floor((texcoord0.x) / dotdistanceX) * dotdistanceX;
    float baseY = floor((texcoord0.y) / dotdistanceY) * dotdistanceY;
    
    vec4 firstColor = texture2DRect(tex0, vec2(baseX, baseY));
    
    float grey = ( ( (firstColor.x + firstColor.y + firstColor.z) / 3.0 ) * contrast + gain ) * 255. ;
    grey = clamp( grey, 0.0, 255.0 );
    
    if(!asciiInvert) grey = 255. - grey;
    
    float asciinumber = float ( ( ( grey - 30. ) / 26. ) ) * float( grey > 30.);
    
    float distX = texcoord0.x - baseX;
    float distY = texcoord0.y - baseY;
    
    float coordX = ( ( distX / dotdistanceX ) / 23.0 + ( floor(asciinumber) - 1.  ) /23.0 )  * 1380.;
    float coordY = ( distY / dotdistanceY ) * 30.;
    
    vec4 col = texture2DRect(font, vec2(coordX, coordY));
    vec4 fxColor = vec4(col.r,col.g,col.b,asciiMacro);
    
    vec4 color = (1.0 - asciiMacro) * dryColor + asciiMacro * fxColor;
    
    gl_FragColor = color;
    
}

