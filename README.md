# ClipConductor
A MIDI-triggered VJ app for DIY musicians.

VIDI (v1.1) by Evan Douglas Peters
With VIDI, you can control performance visuals from inside your DAW via MIDI messages. 

-Create a blank MIDI track and route the output to your IAC driver (tips on setting up your IAC driver here). 
![alt text](https://github.com/ebbnflow98/ClipConductor/blob/master/download/io.png)

-Open VIDI, and load videos into the array by clicking on one of the slots in the scroll menu. Make sure that you've converted your videos to HAP .mov files (see TIPS section below).
![alt text](https://github.com/ebbnflow98/ClipConductor/blob/master/download/gui.png)

-In your MIDI track, draw notes into a scene to trigger the videos and FX.
-Pitches 60-85 trigger videos.
-You can also trigger the videos with the number keys on your keyboard: 

| Key | Video |
|-----|-------|
| 1   | 1     |
| 2   | 2     |
| 3   | 3     |
| 4   | 4     |
| 5   | 5     |
| 6   | 6     |
| 7   | 7     |
| 8   | 8     |
| 9   | 9     |
| 0   | 10    |
| Q   | 11    |
| W   | 12    |
| E   | 13    |
| R   | 14    |
| T   | 15    |
| Y   | 16    |
| U   | 17    |
| I   | 18    |
| O   | 19    |
| P   | 20    |
| A   | 21    |
| S   | 22    |
| D   | 23    |
| F   | 24    |
| G   | 25    |
 
-FX are manipulated with control messages. For all toggles, MIDI values >63=ON; <=63=OFF:

CC # | VIDI FX                  | Function                                                                                                           |
|------|--------------------------|--------------------------------------------------------------------------------------------------------------------|
| 15   | FX Dry/Wet               | Controls opacity of all FX                                                                                         |
| 16   | Video Playback Speed     | Changes speed of playback (unless Video Sync is toggled)                                                           |
| 17   | Video Division           | Changes duration of synced playback when Video Sync is toggled.                                                    |
| 18   | Video Sync               | Changes video playback to sync with tempo of your session rather than the Video Playback Speed slider.             |
| 19   | Background Switch        | When nothing is drawn, the background will alternate (with your session tempo) between BG Color #1 and BG Color #2 |
| 20   | Tempo Division           | Changes the note value duration by which the background switches.                                                  |
| 21   | Triplet                  | Tempo Division becomes a triplet of whatever note duration is selected.                                            |
| 24   | Invert Dry/Wet           | Changes colors to their inverse.                                                                                   |
| 25   | Ripple Dry/Wet           | Sends ripples through the image/video.                                                                             |
| 26   | Ripple Sync              | The rate of the ripples will correspond to the tempo of the session.                                               |
| 27   | Horizontal Ripple Amount | The amplitude of the horizontal ripples!                                                                           |
| 28   | Vertical Ripple Amount   | The amplitude of the vertical ripples!                                                                             |
| 29   | Ripple Rate              | The frequency by which the image ripples!                                                                          |
| 30   | Filter Dry/Wet           | Puts a filter with a custom color on the image.                                                                    |
| 31   | Filter Red               | Red filter component.                                                                                              |
| 33   | Filter Green             | Green filter component.                                                                                            |
| 34   | Filter Blue              | Blue filter component.                                                                                             |
| 35   | Filter Alpha             | The opacity of the filter.                                                                                         |
| 40   | Kaleidoscope Macro       | Shockingly it makes the image look like a kaleidoscope.                                                            |
| 41   | Kaleidoscope Angle       | The angle of the kaleidoscope (I don't really know)                                                                |
| 42   | Kaleidoscope Skew X      | Another fun thing to make the kaleidoscope move.                                                                   |
| 43   | Kaleidoscope Skew Y      | Yet another fun thing to play with                                                                                 |
| 44   | Kaleidoscope Sectors     | The amount of divisions in a half of the kaleidoscope.                                                             |
| 50   | Background Color 1 Red   | The red component of BG Color #1                                                                                   |
| 51   | Background Color 1 Green | The green component of BG Color #1                                                                                 |
| 52   | Background Color 1 Blue  | The blue component of BG Color #1                                                                                  |
| 53   | Background Color 2 Red   | The red component of BG Color #2                                                                                   |
| 54   | Background Color 2 Green | The green component of BG Color #2                                                                                 |
| 55   | Background Color 2 Blue  | The blue component of BG Color #2                                                                                  |

-For Ableton Live Suite users, there is a Max for Live controller device that accompanies the VIDI download. This device doesn't expand functionality at all; it simply takes the MIDI parameters used by VIDI and puts them in one place. To install, move the VIDI.amxd device to your Ableton User Library (for me, that was located at ~/Music/Ableton/User Library/Presets/MIDI Effects).
![alt text](https://github.com/ebbnflow98/ClipConductor/blob/master/download/m4l%20gui.png)

-You can also control the FX values with the sliders in the GUI, however, these changes won't be reflected in your DAW automation or the VIDI Max for Live Controller device.

-To change a video in the array, simply click on it. To delete a video from the array, click the CLEAR toggle and then click the video in the array. To clear the whole array, click the CLEAR ALL button.

-If nothing is drawn, then a default color will be displayed. Choose this color with the BG COLOR #1 color selector. If you select the BACKGROUND SWITCH toggle, this color will change with the tempo of the session in your DAW. Select the second background color with the BG COLOR #2 color selector. You can change the rhythmic division by which the background changes with the TEMPO DIVISION slider and TRIPLET toggle.

-In the VIDEO dropdown, you can control the video speed (VIDEO SPEED slider). If you select the VIDEO SYNC option, then the video will be played back at a speed that coincides with the rhythmic division you've selected, regardless of the length of the video.

TIPS
-VIDI supports .PNG and JPEG image formats and .MOV (Quicktime) video format. All videos MUST be encoded using the HAP codec (https://hap.video/using-hap.html). To convert your .mov files to HAP, use the AVF Batch Converter (https://github.com/Vidvox/hap-in-avfoundation/releases/tag/1.5.3). 
-Make sure that you export your videos in a format that supports alpha channels (list of these formats/codecs here: https://www.rocketstock.com/blog/video-codecs-and-image-sequences-with-alpha-channels/). 
-For Adobe Premiere users, here's an encoder you can install that will allow you to export your videos to the HAP codec directly from Adobe Premiere: (https://github.com/disguise-one/hap-encoder-adobe-cc/releases/tag/1.0.8-rc4).
-I included a bunch of royalty-free videos in the download folder that you are free to use. Some of them are transparent, so, that's pretty cool. I got all of them on pixabay.com. Go crazy.
-You'll probably notice that HAP videos are huge files. Unfortunately, that's the tradeoff for having fast, smooth, high resolution playback.	

IAC Driver:
-Make sure you set up your IAC Driver to allow you to route MIDI to VIDI   (https://help.ableton.com/hc/en-us/articles/209774225-How-to-setup-a-virtual-MIDI-bus).

-"Kaledioscope" and "Ripple" FX were adapted from Dennis Perelov's Mastering openFrameworks book.

-Here are all of the great addons and other software that I used to build this project.
•	openFrameworks (https://openframeworks.cc/about/)
•	Max For Live (https://www.ableton.com/en/live/max-for-live/)
•	ofxDatGui by Stephen Braitsch (https://braitsch.github.io/ofxDatGui/)
•	ofxMidi by Dan Wilcox (https://github.com/danomatika/ofxMidi)
•	ofxHapPlayer by Tom Butterworth (https://github.com/bangnoise)
•	Control Change 8 by Robert Henke (http://roberthenke.com/technology/m4l.html)

