//Clip Conductor Fragment Shader file
//
//
//
//
//#ifdef GL_ES
#version 120
//precision mediump float;
//#endif
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable
#define PI (3.14159265358979323846)
#define TWO_PI (2*PI)

uniform sampler2DRect texture0;

uniform float fxMacro;

uniform float invertMacro;

uniform float time;
uniform float speed;
uniform vec2 resolution;

uniform float rippleMacro;
uniform vec2 rippleXY;
uniform float rippleRate;

uniform float kaleidioscopeMacro;
uniform int ksectors;
uniform float kangleRad;
uniform vec2 kcenter;
uniform vec2 screenCenter;

uniform vec4 filterRGB;
uniform float filterMacro;

uniform int pixelateMacro;

uniform int fullhouseMacro;

uniform float asciiMacro;
uniform sampler2DRect font;
uniform int asciiInvert;
uniform float asciiDotDistance;
uniform float asciiImageGain;
uniform float asciiImageContrast;

uniform float ledMacro;
uniform float ledDotDistance;



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

vec2 Ripple(vec2 pos, vec2 rippleXY, float rippleRate, float rippleMacro)
{
    float amp=sin(pos.x *.03);
    pos.x+=sin(time*rippleRate)*amp*50.0*rippleMacro*rippleXY.x;
    pos.y+=sin(time*rippleRate)*amp*50.0*rippleMacro*rippleXY.y;
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

vec2 Pixelate(vec2 pos, int pixelateMacro)
{
    int pixelation = 100-pixelateMacro;
    vec2 pixelationSection = (resolution/float(pixelation));
    vec2 difference= floor(mod(pos, pixelationSection));
    pos-=difference;
    return pos;
}

vec2 Fullhouse(vec2 pos, int fullhouseMacro)
{
    int tiles=fullhouseMacro;
    vec2 tileScale = float(tiles)*(pos/resolution);
    vec2 tileScaleCoord = fract(tileScale);
    pos=tileScaleCoord*resolution;
    return pos;
}

//vec4 Ascii(vec4 fxColor,  float asciiMacro, float asciiImageGain, float asciiImageContrast, int asciiInvert, float asciiDotDistance )//todo
//{
//    vec2 pos = gl_TexCoord[0].xy; //pos
//    asciiImageGain = asciiImageGain*2.-1.;
////    float imagecontrast = p2;
//
////    int asciiInvert = int(p3); //bool 0 or 1; clamp?
//
//    asciiDotDistance *= 100.0;
//
//    float asciiDotDistanceX = asciiDotDistance * 2.0;
//    float asciiDotDistanceY = asciiDotDistance * 1.0;
//
//    float contrast = ( asciiImageContrast * 2. )* ( asciiImageContrast * 2. ) * ( asciiImageContrast * 2. ) * ( asciiImageContrast * 2. );
//    float gain = asciiImageGain - (asciiImageContrast -0.5) ;
//
//    float baseX = floor((pos.x) / asciiDotDistanceX) * asciiDotDistanceX;
//    float baseY = floor((pos.y) / asciiDotDistanceY) * asciiDotDistanceY;
//
//    vec4 color = texture2DRect(texture0, vec2(baseX, baseY));
//
//
////    color=fxColor;
//
//    float grey = (((color.x + color.y + color.z) / 3.0 ) * contrast + gain) * 255. ;
//
//    grey = clamp ( grey, 0.0, 255.0 );
//
//    grey = 255. - grey;
//
//    float asciinumber = float (((grey - 30.)/ 26.)) * float( grey > 30.);
//
//    float distX = pos.x - baseX;
//    float distY = pos.y - baseY;
//
//    float coordX = ( ( distX / asciiDotDistanceX ) / 23.0 + ( floor(asciinumber) - 1.  ) /23.0 )  * 1380.;
//    float coordY = ( distY / asciiDotDistanceY ) * 30.;
//
//    vec4 col = texture2DRect(font, vec2(coordX, coordY));
//
////    return col;
//
//    vec4 col_inv = 1.0 - col;
//
//    vec4 result = float (1 - asciiInvert) * col + float (asciiInvert) * col_inv;
//
//    return result;
//}

//vec4 LED(vec4 fxColor, vec2 pos, float ledMacro, float ledDotDistance)//todo
//{
//    vec2 offset_red = vec2(.5,.5);
//    vec2 offset_green = offset_red;
//    vec2 offset_blue = offset_red;
//    float halfdistance = ledDotDistance/ 2.0;
//    float quaterdistance = ledDotDistance/ 4.0;
//
////     pos = gl_TexCoord[0].xy;
//    vec2 offsetr = offset_red * (ledDotDistance/ 5.0);
//    vec2 offsetg = offset_green * (ledDotDistance/ 5.0);
//    vec2 offsetb = offset_blue * (ledDotDistance/ 5.0);
//
//    float baseXr = floor((pos.x - offsetr.x) / ledDotDistance) * ledDotDistance+ halfdistance + offsetr.x;
//    float baseYr = floor((pos.y - offsetr.y) / ledDotDistance) * ledDotDistance+ halfdistance + offsetr.y;
//
//    float distanceXr = abs(pos.x - baseXr);
//    float distanceYr = abs(pos.y - baseYr);
//
//    float distancer = sqrt(pow(distanceXr, 2.0) + pow(distanceYr, 2.0));
//
//    float visibilityr = float(distancer< (quaterdistance) ) + float(distancer >= (quaterdistance) ) *  ( (halfdistance - distancer) / quaterdistance);
//
//    float baseXg = floor((pos.x - offsetg.x) / ledDotDistance) * ledDotDistance+ halfdistance + offsetg.x;
//    float baseYg = floor((pos.y - offsetg.y) / ledDotDistance) * ledDotDistance+ halfdistance + offsetg.y;
//
//    float distanceXg = abs(pos.x - baseXg);
//    float distanceYg = abs(pos.y - baseYg);
//
//    float distanceg = sqrt(pow(distanceXg, 2.0) + pow(distanceYg, 2.0));
//
//    float visibilityg = float(distanceg< (quaterdistance) ) + float(distanceg >= (quaterdistance) ) *  ( (halfdistance - distanceg) / quaterdistance);
//
//    float baseXb = floor((pos.x - offsetb.x) / ledDotDistance) * ledDotDistance+ halfdistance + offsetb.x;
//    float baseYb = floor((pos.y - offsetb.y) / ledDotDistance) * ledDotDistance+ halfdistance + offsetb.y;
//
//    float distanceXb = abs(pos.x - baseXb);
//    float distanceYb = abs(pos.y - baseYb);
//
//    float distanceb = sqrt(pow(distanceXb, 2.0) + pow(distanceYb, 2.0));
//
//    float visibilityb = float(distanceb< (quaterdistance) ) + float(distanceb >= (quaterdistance) ) *  ( (halfdistance - distanceb) / quaterdistance);
//
//    vec4 stuffr = texture2DRect(texture0, vec2(baseXr, baseYr));
//    vec4 stuffg = texture2DRect(texture0, vec2(baseXg, baseYg));
//    vec4 stuffb = texture2DRect(texture0, vec2(baseXb, baseYb));
//
//    return vec4(stuffr.x * visibilityr, stuffg.y * visibilityg, stuffb.z * visibilityb, 1.0);
//
//}

void main()
{
    vec4 dryColor = texture2DRect(texture0,gl_TexCoord[0].xy);
    
    vec2 pos=gl_TexCoord[0].xy;
    
    pos=Ripple(pos,rippleXY, rippleRate, rippleMacro);
    
    pos=Kaleidoscope(pos, kcenter, screenCenter, ksectors, kangleRad, kaleidioscopeMacro);

    if(pixelateMacro!=0)pos=Pixelate(pos,pixelateMacro);
    
    if(fullhouseMacro!=0)pos=Fullhouse(pos,fullhouseMacro);
    
    vec4 fxColor=texture2DRect(texture0,pos);
    
    if(filterMacro!=0.0)fxColor=Filter(fxColor,filterMacro,filterRGB);
    
    if(invertMacro!=0.0)fxColor=Invert(fxColor,invertMacro);
    
//    if(asciiMacro!=0.0)fxColor=Ascii(fxColor,asciiMacro,asciiImageGain,asciiImageContrast,asciiInvert,asciiDotDistance);
    
//    if(ledMacro!=0.0)fxColor=LED(fxColor,pos,ledMacro,ledDotDistance);
    
    vec4 color = (1.0 - fxMacro) * dryColor + fxMacro * fxColor;
    
    gl_FragColor=color;
}
