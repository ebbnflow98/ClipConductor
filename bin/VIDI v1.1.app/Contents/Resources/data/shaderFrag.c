#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)

uniform sampler2DRect texture0;

uniform float invertMacro;

uniform float time;
uniform float speed;

uniform float rippleMacro;
uniform vec2 rippleXY;

uniform float kaleidioscopeMacro;
uniform int ksectors;
uniform float kangleRad;
uniform vec2 kcenter;
uniform vec2 screenCenter;

uniform vec4 filterRGB;
uniform float filterMacro;


vec4 Invert(vec4 color,float invertMacro)
{
    vec3 color1=color.rgb;
    vec3 invertedColor=vec3(1.0-color1.r,1.0-color1.g,1.0-color1.b);
    if(invertMacro==0.0) return color;
    else return vec4(mix(color1, invertedColor, invertMacro),color.a);
}

vec4 Filter(vec4 color, float filterMacro, vec4 filterRGB)
{
    color.r*=filterRGB.r*filterMacro;
    color.g*=filterRGB.g*filterMacro;
    color.b*=filterRGB.b*filterMacro;
    color.a*=filterRGB.a*filterMacro;
    
    return color;
}

vec2 Ripple(vec2 pos, vec2 rippleXY, float rippleMacro)
{
    float amp=sin(pos.x *.03);
    pos.x+=sin(time*2.0)*amp*50.0*rippleMacro*rippleXY.x;
    pos.y+=sin(time*2.0)*amp*50.0*rippleMacro*rippleXY.y;

    return pos;
}

vec2 Kaleidoscope(vec2 pos, vec2 kcenter, vec2 screenCenter, float ksectors, float kangleRad, float kaleidioscopeMacro)
{
    vec2 v = pos - screenCenter;//    kaleidoscope (openFrameworks Essentials book)
    float r = length(v);
    float a = atan(v.y,v.x);
    float A = TWO_PI / float(ksectors);
    a = mod( a, A );
    if ( a > A/2.0 ) a = A - a;
    a -= kangleRad;
    vec2 u = vec2(cos(a),sin(a)) * r;
    u += kcenter;
    return u;
}

void main()
{
    vec2 pos=gl_TexCoord[0].xy;
    
    pos=Ripple(pos,rippleXY,rippleMacro);
    pos=Kaleidoscope(pos, kcenter, screenCenter, ksectors, kangleRad, kaleidioscopeMacro);
    
    vec4 color=texture2DRect(texture0,pos);
    
    if(filterMacro!=0.0)color=Filter(color,filterMacro,filterRGB);
    
    if(invertMacro!=0.0)color=Invert(color,invertMacro);
    
    gl_FragColor=color;
}
