# Spectacle
A MIDI-triggered performance visuals app for DIY musicians.

Spectacle (v2.0) by Evan Douglas Peters

With Spectacle, you can control performance visuals from inside your DAW via MIDI messages. 

## Installation

-Download the Spectacle dmg and open. Drag Spectacle into your Applications folder. If you'd like to install the Max for Live devices, drag them and drop them on an Ableton Live window.

![alt text](https://github.com/ebbnflow98/Spectacle/blob/master/readMe%20Resources/dmg.png)


## Setup

### Triggering videos via the IAC Driver

-Create a blank MIDI track and route the output to your IAC driver on Channel 1 (tips on setting up your IAC driver [here](https://help.ableton.com/hc/en-us/articles/209774225-How-to-setup-a-virtual-MIDI-bus)). For Ableton Live users, drop the Spectacle Max for Live device on this channel. This track will control your videos.

![alt text](https://github.com/ebbnflow98/ClipConductor/blob/master/readMe%20Resources/io.png)

-If you want to control lights via DMX, add a second MIDI track and route the output to your IAC driver on Channel 2.

-Open Spectacle, and load videos into the array by clicking on one of the slots in the scroll menu. You can also drag and drop your videos into Spectacle to add them. Make sure that you've converted your videos to HAP .mov files (see TIPS section below).
![alt text](https://github.com/ebbnflow98/ClipConductor/blob/master/readMe%20Resources/gui.png)

-In your MIDI track, draw notes into a MIDI clip to trigger the videos.

-You can also trigger the videos with the number keys on your keyboard: 

| Key | Video |  | Key | Video |
|-----|-------| -- |-----|-------|
| 1   | 1     |    | R   | 14    |
| 2   | 2     |    | T   | 15    |
| 3   | 3     |    | Y   | 16    |
| 4   | 4     |    | U   | 17    |
| 5   | 5     |    | I   | 18    |
| 6   | 6     |    | O   | 19    |
| 7   | 7     |    | P   | 20    |
| 8   | 8     |    | A   | 21    |
| 9   | 9     |    | S   | 22    |
| 0   | 10    |    | D   | 23    |
| Q   | 11    |    | F   | 24    |
| W   | 12    |    | G   | 25    |
| E   | 13    |
 
 ## Triggering video FX via the IAC Driver
 
 -FX are manipulated with control messages. With the above track setup, automate the MIDI CC's that coorespond to the desired FX from the table below (for all toggle-type controls, MIDI values >63 set the toggle to ON, and MIDI values <=63 set the toggle to OFF) :

CC # | Spectacle FX                  | Function                                                                                                           |
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
| 40   | Kaleidoscope Macro       | Shockingly, it makes the image look like a kaleidoscope.                                                            |
| 41   | Kaleidoscope Angle       | The angle of the kaleidoscope                                                            |
| 42   | Kaleidoscope Skew X      | Another fun thing to make the kaleidoscope move.                                                         |
| 43   | Kaleidoscope Skew Y      | Yet, another fun thing to play with                                                                                 |
| 44   | Kaleidoscope Sectors     | The amount of divisions in a half of the kaleidoscope.                                                             |
| 50   | Background Color 1 Red   | The red component of BG Color #1                                                                                   |
| 51   | Background Color 1 Green | The green component of BG Color #1                                                                                 |
| 52   | Background Color 1 Blue  | The blue component of BG Color #1                                                                                  |
| 53   | Background Color 2 Red   | The red component of BG Color #2                                                                                   |
| 54   | Background Color 2 Green | The green component of BG Color #2                                                                                 |
| 55   | Background Color 2 Blue  | The blue component of BG Color #2                                                                                  |

-For Ableton Live Suite users, there is a Max for Live controller device that accompanies the Spectacle download. This device simply takes the MIDI parameters used by Spectacle and labels them (so that the user doesn't have to worry about matching CC values to cooresponding FX using the table above). To install the device, drag the Spectacle.amxd device and drop it on your Ableton window. Put this on the track routed to channel 1.
![alt text](https://github.com/ebbnflow98/Spectacle/blob/master/readMe%20Resources/m4l%20gui.png)

–The dropdown boxes below the knobs on the M4L device allow users to change the input MIDI CC that the FX are looking for. This only applies to users with hardware controllers that don't allow for CC re-assignment via MIDI mapping; so most users shouldn't change these values. 

-You can also control the FX values with the sliders in the GUI, however, these changes won't be reflected in your DAW automation or the Spectacle Max for Live device.

## Triggering Lights via the IAC Driver and interface
-Row 3 has sliders that indicate the DMX value being sent to each light.

-To label any of those sliders with a custom label, simply click where it says "Type Parameter Here" and type in the your label. These labels can be saved and recalled later via the SAVE and then the LOAD buttoms.

-Ableton Live users need to put the Spectacle DMX Max for Live device on a second MIDI track, with its output routed to Channel 2.
![alt text](https://github.com/ebbnflow98/Spectacle/blob/master/readMe%20Resources/dmx%20m4l.png)


## Using the Spectacle GUI
-NEW PROJECT will clear the video array and any custom names of the DMX sliders that have been typed in.

-SAVE will save the paths of the videos in the array, as well as the custom names typed in to the DMX sliders (FX values are not saved, because those should be saved in your session in automation). LOAD loads these values.

-The MIDI input port will be set to the first available port by default. The port can be selected through the MIDI PORT dropdown. Refresh the available ports with the REFRESH MIDI PORTS button.

-Like the MIDI input ports, the DMX output device will be set to the first available USB port by default. To change, select the USB Port from the DMX INTERFACE dropdown. Refresh the options with the REFRESH USB PORTS button.  

-To change a video in the array, simply click on it. To delete a video from the array, click the CLEAR toggle and then click the video in the array. To clear the whole array, click the CLEAR ALL button.  

-Dragging videos into the Spectacle window will cause Spectacle to find the first open video slot and fill it with the selected video (and do the same for any subsequent videos dragged in).  

-In the VIDEO dropdown, you can control the video speed (VIDEO SPEED slider). If you select the VIDEO SYNC option, then the video will be played back at a speed that coincides with the rhythmic division of the tempo you've selected, regardless of the length of the video. This tempo is read from the MIDI messages.  


### Tips
-Spectacle supports .PNG and JPEG image formats and .MOV (Quicktime) video format. All videos MUST be encoded using the [HAP codec](https://hap.video/using-hap.html). To convert your .mov files to HAP, I suggest using the [AVF Batch Converter](https://github.com/Vidvox/hap-in-avfoundation/releases/tag/1.5.3).   

-Make sure that you export your videos in a format that supports alpha channels (list of these formats and codecs [here](https://www.rocketstock.com/blog/video-codecs-and-image-sequences-with-alpha-channels/).  

-You'll probably notice that HAP videos are huge files. Unfortunately, that's the tradeoff for having fast, smooth, high resolution playback.	  

-Make sure you set up your IAC Driver to allow you to route MIDI to Spectacle. Instructions on setting up IAC driver are [here](https://help.ableton.com/hc/en-us/articles/209774225-How-to-setup-a-virtual-MIDI-bus)).   

## Credits
•   The "Kaledioscope" and "Ripple" FX were adapted from Dennis Perelov's book, [*Mastering openFrameworks*](http://b.parsons.edu/~traviss/booKs/oF/Mastering%20openFrameworks%20-%20Yanc,%20Chris_compressed.pdf).  
•   The "ASCII" effect was adapted from a shader I found online over a year ago that I have been unable to find once again to this point. If the ASCII code looks like yours, please come forward and claim it! :)  
•   The "Zebra" effect was adapted from [Color Zebra](https://www.shadertoy.com/view/Xsl3z8) by gtoledo3.  

-Here are the addons and other software that I used to build this project!  

•   [openFrameworks](https://openframeworks.cc/about/)  
•   [ofxDatGui](https://braitsch.github.io/ofxDatGui/) by Stephen Braitsch  
•   [ofxMidi](https://github.com/danomatika/ofxMidi) by Dan Wilcox   
•   [ofxHapPlayer](https://github.com/bangnoise/ofxHapPlayer) by Tom Butterworth   
•   [ofxDmx](https://github.com/kylemcdonald/ofxDmx) by Kyle McDonald

•   [Max For Live](https://www.ableton.com/en/live/max-for-live/)  
•   [Control Change 8](http://roberthenke.com/technology/m4l.html) by Robert Henke    
•   [HAP](https://hap.video/)  




