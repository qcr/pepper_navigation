

#include <cmath>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <iostream>
#include "LineDescriptor.hh"
#include "PairwiseLineMatching.hh"



using namespace std;

void dispImages(std::string title, cv::Mat &IC,ScaleLines linesInLeft,cv::Mat &IK,ScaleLines linesInRight,std::vector<unsigned int> matchResult);

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

    std::string imageName2(argv[2]);

    cv::Mat rightImage = cv::imread(imageName2.c_str(),cv::IMREAD_GRAYSCALE);

	
  
	
    unsigned int imageWidth  = leftImage.cols;
    unsigned int imageHeight = leftImage.rows;

	

	LineDescriptor ld;

	ScaleLines   linesInLeft;
    ld.GetLineDescriptor(leftImage,linesInLeft);

	

    ScaleLines   linesInRight;
    ld.GetLineDescriptor(rightImage,linesInRight);

    std::vector<unsigned int> matchidx, matchLBD;
	
   ld.LineMatchingBinary(linesInLeft,linesInRight,matchidx);

PairwiseLineMatching lineMatch;
lineMatch.LineMatching(linesInLeft,linesInRight,matchLBD);

std::cout<<"number of matched lines "<<matchidx.size()/2<<"\t"<<matchLBD.size()/2<<std::endl;

dispImages("LBDMATCH",leftImage,linesInLeft,rightImage,linesInRight,matchLBD);
dispImages("BINMATCH",leftImage,linesInLeft,rightImage,linesInRight,matchidx);
}


void dispImages(std::string title, cv::Mat &IC,ScaleLines linesInLeft,cv::Mat &IK,ScaleLines linesInRight,std::vector<unsigned int> matchResult) {


     int size,sz;
     int m,n;
     int w, h;

     h =1; w=2;

     size = 320;
     sz = 240;

     int wdh= 0;
     int offsets = 0;
     cv::Mat Ia;
    cv::Mat Ik;
     cv::cvtColor(IC,Ia,cv::COLOR_GRAY2BGR);
     cv::cvtColor(IK,Ik,cv::COLOR_GRAY2BGR);


     cv::Mat DispImage(cv::Size(size*w,int(sz*h+wdh*h)+offsets),CV_8UC3);

    int r,g,b;
    int nmatch = matchResult.size()/2;
    int lineIDLeft;
    int lineIDRight;


     for(int pair=0; pair<nmatch;pair++){
         r = int(rand()%256);
         g = int(rand()%256);
         b = 255 - r;

         lineIDLeft = matchResult[2*pair];
         lineIDRight= matchResult[2*pair+1];

         cv::line(Ia,cv::Point(int(linesInLeft[lineIDLeft][0].startPointX),int(linesInLeft[lineIDLeft][0].startPointY))
                     ,cv::Point(int(linesInLeft[lineIDLeft][0].endPointX),int(linesInLeft[lineIDLeft][0].endPointY)),
                      CV_RGB(r,g,b),2);

         cv::line(Ik,cv::Point(int(linesInRight[lineIDRight][0].startPointX),int(linesInRight[lineIDRight][0].startPointY)),
                   cv::Point(int(linesInRight[lineIDRight][0].endPointX), int(linesInRight[lineIDRight][0].endPointY)),
                    CV_RGB(r,g,b),2);
     }



     cv::Mat temp;

     m= 0; n=wdh+offsets/2;
     cv::resize(Ia,temp,cv::Size(size,sz));
     temp.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m = size; n=wdh+offsets/2;
     cv::resize(Ik,temp,cv::Size(size,sz));
     temp.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     temp.release();

    cv::namedWindow( title );
    cv::imshow( title,DispImage);

  cv::waitKey(0);



}
