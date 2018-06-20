# OFColorMatrix

This code is a port of http://www.quasimondo.com/colormatrix/ColorMatrix.as.<br>
This is only tested on windows 10.
<br>
In order to use this code, you need a fragment shader code where it'S located at "./bin/data/shader/" directory.

original image:<br>
![Screenshot](original.JPG)
<br>

invert:<br>
![Screenshot](invert.jpg)
<br>

saturation:<br>
![Screenshot](saturation.jpg)
<br>

contrast:<br>
![Screenshot](contrast.jpg)
<br>

brightness:<br>
![Screenshot](brightness.jpg)
<br>

grayscale:<br>
![Screenshot](grayscale.jpg)
<br>

hue:<br>
![Screenshot](hue.jpg)
<br>

## Example Code

Here is a one of the example applying color matrix effect to an image.

```C#

void ofApp::setup(){
    :
    colorMat.adjustBrightness(100, 100, 100);
    :
}

void ofApp::draw(){
    :
    colorMat.apply(shader);
    :
}

```