# Mapping

### Selection of Reference Images Based on Line Segment Matching 
Bista SR, Giordano PR, Chaumette F. Appearance-based indoor navigation by IBVS using line segments. IEEE Robotics and Automation Letters. 2016 Jan 26;1(1):423-30.

https://github.com/suuman/selectKeyImagesLines.git

## Build

	1. Get Source codes from the repository.
		$ git clone https://github.com/suuman/selectKeyImagesLines.git
	
	2. Build executables required for mapping. 
		$ ./build_linematching.sh
	
The above command will build executables ***detectinesED*** and ***matchlines*** to *./linematching* folder and is equivalent to the following commands
	
	        $ cd linematching/Linematching_iso && mkdir build $$ cd build 
	        $ cmake .. && make -j8
 
## Usage
 The code for the mapping is in Matlab. Make sure the line detection and matching codes have been properly built and executables have been placed in the correct folder.  
 For selecting the reference images, we need to provide the path of the image sequence folder and the folder to store the reference images.

	1. Open selectRefImages.m
	
	2. Set the path of the image sequence. e.g imseq = '../roboroom/imgs_acquired'
	
	3. Set the path to store the reference images. e.g refimpath='../roboroom/ref_imgs'
	
	4. Run selectRefImages.m

The reference image folder will contain

	1. Reference Images.
	
	2. The text files with the detected line segments and their descriptors. 
       There is one .txt file for each reference image.

## Creating Toplogical Map for Naviagtion
Bista SR, Ward B, Corke P. Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots.  

To build a topological map from reference images, please refer to the [Reference Manual](https://github.com/suuman/pepper_navigation/tree/main/docs/Pepper_Navigation_Reference_Manual.pdf) in [*../docs*](https://github.com/suuman/pepper_navigation/tree/main/docs) folder.
