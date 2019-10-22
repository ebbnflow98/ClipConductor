//
//  shader1Frag.cpp
//  shaderTest
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

uniform float ledDotDistance;
uniform float ledMacro;

uniform vec2 screenCenter;
uniform float time;
uniform vec2 resolution;

void main()
{
    vec4 dryColor = texture2DRect(tex0,gl_TexCoord[0].xy);

    vec2 offset_red = vec2(1.,1.);
    vec2 offset_green = vec2(1.,1.);
    vec2 offset_blue = vec2(1.,1.);
    
    float dotdistance = ledDotDistance;
    
    float halfdistance = dotdistance / 2.0;
    float quaterdistance = dotdistance / 4.0;
    
    vec2 pos = gl_TexCoord[0].xy;
    vec2 offsetr = offset_red * (dotdistance / 5.0);
    vec2 offsetg = offset_green * (dotdistance / 5.0);
    vec2 offsetb = offset_blue * (dotdistance / 5.0);
    
    float baseXr = floor((pos.x - offsetr.x) / dotdistance) * dotdistance + halfdistance + offsetr.x;
    float baseYr = floor((pos.y - offsetr.y) / dotdistance) * dotdistance + halfdistance + offsetr.y;
    
    float distanceXr = abs(pos.x - baseXr);
    float distanceYr = abs(pos.y - baseYr);
    
    float distancer = sqrt(pow(distanceXr, 2.0) + pow(distanceYr, 2.0));
    
    float visibilityr = float(distancer< (quaterdistance) ) + float(distancer >= (quaterdistance) ) *  ( (halfdistance - distancer) / quaterdistance);
    
    float baseXg = floor((pos.x - offsetg.x) / dotdistance) * dotdistance + halfdistance + offsetg.x;
    float baseYg = floor((pos.y - offsetg.y) / dotdistance) * dotdistance + halfdistance + offsetg.y;
    
    float distanceXg = abs(pos.x - baseXg);
    float distanceYg = abs(pos.y - baseYg);
    
    float distanceg = sqrt(pow(distanceXg, 2.0) + pow(distanceYg, 2.0));
    
    float visibilityg = float(distanceg< (quaterdistance) ) + float(distanceg >= (quaterdistance) ) *  ( (halfdistance - distanceg) / quaterdistance);
    
    float baseXb = floor((pos.x - offsetb.x) / dotdistance) * dotdistance + halfdistance + offsetb.x;
    float baseYb = floor((pos.y - offsetb.y) / dotdistance) * dotdistance + halfdistance + offsetb.y;
    
    float distanceXb = abs(pos.x - baseXb);
    float distanceYb = abs(pos.y - baseYb);
    
    float distanceb = sqrt(pow(distanceXb, 2.0) + pow(distanceYb, 2.0));
    
    float visibilityb = float(distanceb< (quaterdistance) ) + float(distanceb >= (quaterdistance) ) *  ( (halfdistance - distanceb) / quaterdistance);
    
    vec4 stuffr = texture2DRect(tex0, vec2(baseXr, baseYr));
    vec4 stuffg = texture2DRect(tex0, vec2(baseXg, baseYg));
    vec4 stuffb = texture2DRect(tex0, vec2(baseXb, baseYb));
    
    vec4 fxColor = vec4(stuffr.x * visibilityr, stuffr.y * visibilityg, stuffr.z * visibilityb, ledMacro);
    
    vec4 color = (1.0 - ledShader) * dryColor + ledShader * fxColor;
    
    gl_FragColor = color;
}
