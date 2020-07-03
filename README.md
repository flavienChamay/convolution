# convolution
Implementation of a convolution using LIMACE for images in .pgm and .ppm
From the Paul Sabatier University

WARNINGS:
The instructions (on the "instructions" file, in the folder "instrucitons") and the documentation for LIMACE are in french.

HOW TO COMPILE:
make

HOW TO USE:
You have different transformations that you can use : "binarisation", "dilatation", "erosion", "etalement", "inversion", "median" and "moyen". Each file produced by the make command corresponds to a type of transformation.
If you want to do a dilatation type:
./erosion inputImage.pbm filter.mx outputImage.pbm

Use the -h option to have additionnal and more complete informations.

You have the desired images in the folder "imgOutputToCompare", you have the input images at "imgInput" and the images produced (the result of the program) at "imgResult". Each images in "imgIput" has its correspondent in the "imgResult" folder. After, you can check if the images in "imgResult" corresponds with the program "comparaison" (use -h more more infos).
