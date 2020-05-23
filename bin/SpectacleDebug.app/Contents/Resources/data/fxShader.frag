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

uniform float rotateMacro;
uniform vec2 rotateScreenCenter;

uniform float zebraMacro;
uniform int zebraLevels;
uniform float zebraSpeed;

uniform float vhsMacro;
uniform float vhsStrength;
uniform float vhsSpeed;

vec4 Invert(vec4 color, float invertMacro)
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
//    color.a*=filterRGB.a*filterMacro;
    
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

vec2 Rotate(vec2 pos, float rotateMacro, vec2 resolution, vec2 screenCenter)
{
    rotateMacro=rotateMacro*6.28;
    vec2 v = pos - rotateScreenCenter;    //    v is a vec2 length to the center of the screen
    float r = length(v);            //    r is the tangent to that point
    float a = atan(v.y, v.x);          //    a is the inverse tangent of
    a -= rotateMacro;
    vec2 u = vec2(cos(a),sin(a)) * r;
    u += vec2(rotateScreenCenter);
    return u;
}

vec4 Zebra(vec4 dryColor, float zebraMacro, int zebraLevels, float zebraSpeed)
{
    float phase=time*zebraSpeed;
    vec4 x=dryColor;
    
    x = mod(x + phase, 1.);
    x = floor(x*float(zebraLevels));
    x = mod(x,2.);
    
    vec4 fxColor = vec4(vec3(x), dryColor.a);
    
    vec4 color = (1.0 - zebraMacro) * dryColor + zebraMacro * fxColor;
    
    return color;
}

float rand(vec2 co){
       return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
   }

vec4 Vhs(vec4 dryColor, float vhsMacro, float vhsStrength, float vhsSpeed)
{
//        vec2 uv = fragCoord.xy / iResolution.xy;
    vec2 uv = gl_TexCoord[0].xy;
//    uv *= resolution.y/resolution.x;
//    uv += 1.0;
//    uv /= 2.0;
//    uv *= resolution.xy;
//
//    uv.y = resolution.y - uv.y;
//    uv = mod(uv, resolution);
//    uv *= texResolution/resolution;
    
        vec2 uv_org = vec2(uv);

        float t = mod(time, 360.0);
        float t2 = floor(t*0.6);

        float x,y,yt,xt;

            yt= abs(cos(t)) * rand(vec2(t,t)) * 100.0;
            xt=sin(360.0*rand(vec2(t,t)))*0.25;
            if (xt < 0.0) {
                xt=0.125;
                    }
            x=uv.x-xt*exp(-pow(uv.y*100.0-yt,2.0)/24.0);
            y=uv.y;
            
            
        uv.x=x;
        uv.y=y;

       yt=0.5*cos((yt/100.0)/100.0*360.0);
       float yr=0.1*cos((yt/100.0)/100.0*360.0);
        if (uv_org.y > yt && uv_org.y < yt+rand(vec2(t2,t))*0.25)
        {
            float md = mod(x*100.0,10.0);
            if (md*sin(t) > sin(yr*360.0) || rand(vec2(md,md))>0.4)
            {
                vec4 org_c = texture2DRect(texture0, uv);
                float colx = rand(vec2(t2,t2)) * 0.75;
                float coly = rand(vec2(uv.x+t,t));// * 0.5;
                float colz = rand(vec2(t2,t2));// * 0.5;
                vec4 fxColor = vec4(org_c.x+colx,org_c.y+colx,org_c.z+colx,0.0);
                vec4 color = (1.0 - vhsMacro) * dryColor + vhsMacro * fxColor;
                return color;
            }
        }
        else if (y<cos(t) && mod(x*40.0,2.0)>rand(vec2(y*t,t*t))*1.0 ||  mod(y*12.0,2.0)<rand(vec2(x,t))*1.0)
        {
            if (rand(vec2(x+t,y+t))>0.8)
            {
               vec4 fxColor = vec4(rand(vec2(x*t,y*t)),rand(vec2(x*t,y*t)),rand(vec2(x*t,y*t)),0.0);
                vec4 color = (1.0 - vhsMacro) * dryColor + vhsMacro * fxColor;
                return color;
            }
            else
            {
            vec4 fxColor = texture2DRect(texture0, uv);
            vec4 color = (1.0 - vhsMacro) * dryColor + vhsMacro * fxColor;
            return color;
            }
        }
        else
        {
            uv.x = uv.x + rand(vec2(t,uv.y)) * 0.0087 * sin(y*2.0);
            vec4 fxColor = texture2DRect(texture0, uv);
            vec4 color = (1.0 - vhsMacro) * dryColor + vhsMacro * fxColor;
            return color;
        }
}

void main()
{
    vec4 dryColor = texture2DRect(texture0,gl_TexCoord[0].xy);
    
    vec2 pos=gl_TexCoord[0].xy;
    
    pos=Ripple(pos,rippleXY, rippleRate, rippleMacro);
    
    pos=Kaleidoscope(pos, kcenter, screenCenter, ksectors, kangleRad, kaleidioscopeMacro);

    if(pixelateMacro!=0)pos=Pixelate(pos,pixelateMacro);
    
    if(fullhouseMacro!=0)pos=Fullhouse(pos,fullhouseMacro);
    
    pos=Rotate(pos, rotateMacro, resolution, rotateScreenCenter);
    
    vec4 fxColor=texture2DRect(texture0,pos);//====================
    
    if(filterMacro>=0.0)fxColor=Filter(fxColor,filterMacro,filterRGB);
    
    if(invertMacro!=0.0)fxColor=Invert(fxColor,invertMacro);
    
    if(zebraMacro!=0.0)fxColor = Zebra(fxColor, zebraMacro, zebraLevels, zebraSpeed);
    
//    if(vhsMacro!=0.0) fxColor = Vhs(fxColor, vhsMacro, vhsSpeed, vhsStrength);
    
    vec4 color = (1.0 - fxMacro) * dryColor + fxMacro * fxColor;
    
    gl_FragColor=color;
}
