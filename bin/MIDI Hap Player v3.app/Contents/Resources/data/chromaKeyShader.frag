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

uniform float chromaKeyMacro, chromaKeyThreshold;
uniform vec3 chromaKeyColor;

bool check(vec3 a, vec3 z)
{
    float threshold = chromaKeyThreshold;   //threshold controlled by user-controllable parameter
    float r = a.r - z.r;
    float g = a.g - z.g;
    float b = a.b - z.b;
    return (r*r + g*g + b*b) <= threshold*threshold;
}
 
void main()
{
    vec4 color = texture2DRect(tex0,gl_TexCoord[0].xy); //get color of current pixel
//    vec3 chromaKeyColor = pc1; //default to green color, controlled by "pc1" (color-picker parameter)
    
    if(check(color.rgb, chromaKeyColor)) color.a = 0.0; //check to see if pixel's color is within the tolerance of the user-controlled chroma-key color; if so, change that pixel to be transparent
    gl_FragColor = color;   //return the color of the pixel
}

