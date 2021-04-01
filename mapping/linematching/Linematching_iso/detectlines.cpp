

#include <cmath>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "LineDescriptor.hh"



using namespace std;



void usage(int argc, char** argv){
	cout<<"Usage: "<<argv[0]<<"  image_file"<<endl;
	cout<<"Usage: "<<argv[0]<<"  image_file"<<" lines_out_file"<<endl;
}



int main(int argc, char** argv)
{
	int ret = -1;
	if(argc<2){
		usage(argc,argv);
		return ret;
	}
  //load first image from file
	std::string imageName1(argv[1]);

    cv::Mat leftImage = cv::imread(imageName1.c_str(),cv::IMREAD_GRAYSCALE);

	
  
	
    unsigned int imageWidth  = leftImage.cols;
    unsigned int imageHeight = leftImage.rows;

	
	int lowest=100, highest=255;
	int range=(highest-lowest)+1;
	unsigned int r, g, b; //the color of lines

	LineDescriptor ld;

	ScaleLines   linesInLeft;
	if(!ld.OctaveKeyLines(leftImage,linesInLeft)){
		cout<<"No lines detected"<<endl;
		return -1;
	}

	ld.findLineDesc(linesInLeft);
	//initial variables
	
	

	
  ///////////####################################################################
  ///////////####################################################################
	//extract lines, compute their descriptors and match lines
	


	
	


	//(int ff=0;ff<matchResult.size();ff++)
	  //  std::cout<< matchResult.at(ff)<<endl;
	//timer.Stop();
	//timer.Print();
	//timer.Reset();

	if(argc==3){
			std::ofstream ofile1(argv[2]);
			//ofile1<<linesInLeft.size()<<" "<<0<<" "<<0<<" "<<0<<" "<<0<<" "<<linesInLeft.size()<<endl;
			for (int i = 0;i < linesInLeft.size();i++){
				ofile1<<linesInLeft[i][0].startPointX<<" "<<linesInLeft[i][0].startPointY<<" "<<linesInLeft[i][0].endPointX<<" "<<linesInLeft[i][0].endPointY<<" "<<linesInLeft[i][0].lineLength<<" "<<linesInLeft[i][0].direction<<" "<<linesInLeft[i][0].salience<<" "<<linesInLeft[i][0].numOfPixels;
			for(int j=0;j<linesInLeft[i][0].descriptor.size();j++)
				ofile1<<" "<<linesInLeft[i][0].descriptor[j];
			ofile1<<std::endl;
			//std::cout<<linesInLeft[i][0].descriptor.size()<<endl;

			}
	ofile1.close();
	}

  //std::cout<<"number of lines = "<<linesInLeft.size()<<std::endl;
  ///////////####################################################################
  ///////////####################################################################
	//draw  extracted lines into images

	if(argc <3){


        cv::Mat cvLeftColorImage     = cv::imread(imageName1.c_str());


        cv::Point startPoint;
        cv::Point endPoint;
        cv::Point point;


	for(unsigned int i=0; i<linesInLeft.size(); i++){
		r = lowest+int(rand()%range);
		g = lowest+int(rand()%range);
		b = lowest+int(rand()%range);
        startPoint = cv::Point(int(linesInLeft[i][0].startPointX),int(linesInLeft[i][0].startPointY));
        endPoint   = cv::Point(int(linesInLeft[i][0].endPointX),  int(linesInLeft[i][0].endPointY));
        cv::line( cvLeftColorImage,startPoint,endPoint,CV_RGB(r,g,b), 2, cv::LINE_AA);
}

    cv::imshow("linesdetected",cvLeftColorImage);
    cv::imwrite("imgout.png",cvLeftColorImage);
    cv::waitKey(0);
	
	
}

}
