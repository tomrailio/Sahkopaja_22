# INSTRUCTIONS TO CREATE 1-BIT BITMAPS 

A 1-bit BMP is an image array of black-white values represented by 1-0 boolean values.

We want a nice such image to use as startup screen for the 128x32 display we have.

You need to find a good image (simpler is better)
Then the image is turned to black-and-white (can be done in GIMP)
Finally it must be converted to a C/C++ array
For the last step you can use software such as this bitmap converter
http://en.radzio.dxp.pl/bitmap_converter/
(It should work on any machine, I have an ubuntu laptop and it works on mine)

1. Find a good image to use
2. Resize it to 128x32 or 64x32 or 32x32
3. Change the image's color mode to 1-bit monochrome
4. Save the image as a .bmp file, then open it in Bitmap Converter/LCD Assistant
5. You don't need to change the settings (except maybe orientation? not sure)
6. Save to a .cpp file

This produces images in ready array format to print on any LCD screen.

You can automatically turn any image to 1-bit monochrome in GIMP (Google how to)
or in most image manipulating software probably.
If not, it's also possible to program a solution yourself but this may be a bit time consuming.
