
#include <math.h>
#include <time.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>
#include <opencv2/imgproc/imgproc_c.h>
#include <iostream>
//#include <QString>
//#include <QDir>
//#include <QStringList>
#include <algorithm>


//#include <ariac.h>


#include <ctime>
#include "linenav/kimread.h"
#include "navigation.h"
#include "linenav/dispnav.h"
#include "vpNaoqiRobot.h"
#include "vpNaoqiGrabber.h"

using namespace std;

void usage(int argc, char** argv){
    std::cout<<"Usage: "<<argv[0]<<" Imgpath "<<" keyframepath " <<std::endl;
}




int main(int argc, char** argv)
{
    int ret = -1;
    int dispflag = 0;
    if(argc<3){
        usage(argc,argv);
        return ret;
    }
    if(argc>4)
        dispflag = 1;

    // cv::Mat K = (cv::Mat_<double>(3, 3) <<594.4009264,0,319.2854032, 0, 592.4133601,233.5686585 , 0, 0, 1);
     cv::Mat K = (cv::Mat_<double>(3, 3) <<296.3437428191,0,158.2733787287, 0, 296.6441607237,118.0622191474 , 0, 0, 1);

 int startindex = 0;

    std::string imdir(argv[2]);
    imdir += "/";

        std::vector<cv::String> currims;
        cv::glob(imdir + "*.png",currims);
        
         std::stringstream cimname;

         //   cimname <<argv[2]<<"/" <<currims.at(startindex).toStdString();
         //   cimname <<argv[2]<<"/" <<currims.at(startindex);


         //   cv::Mat cim = cv::imread(cimname.str(),CV_LOAD_IMAGE_GRAYSCALE);

            cv::Mat cim = cv::imread(currims.at(startindex),cv::IMREAD_GRAYSCALE);


          kimRead kf(argv[1]);



     

            dispNav *d;
            d=new dispNav;
            d->saveimage(true);
            d->displayimage(true);

            navigation nav(d,K);

             d->setDisptime(100);


            nav.initlocalisation(cim,kf);
          //  cv::waitKey(0);

        std::string currim;
        std::string velimg;
        std::string navimg;
         int flag;
         int count = 0;
         int islast;
        // for(int kk= startindex+1;kk<currims.length();kk++)

         std::cout<<"\n ---------------------------------------------"<<std::endl;

         for(int kk= startindex+1;kk<currims.size();kk++)
         {
		  clock_t tStart = clock();

     
            cim = cv::imread(currims.at(kk),cv::IMREAD_GRAYSCALE);

            flag = nav.step(cim);
            //std::cout<<flag<<std::endl;
            if(flag==0) count = 0;
            else if(flag == 1) count++;
            else continue;

            std::cout<<"Rot Vel = "<<nav.getRotVel()<<std::endl;
            std::cout<<"---------------------------------------------"<<std::endl;
            if(count>2){
                islast = nav.SwitchtoNewKeyImages(kf);
                if(islast){
                    std::cout<<"end of topological navigation"<<std::endl;
                    break;
                }
            }
	     std::cout<<"\n exec time = "<<((double)(clock() - tStart)/CLOCKS_PER_SEC)<<std::endl;
         std::cout<<"---------------------------------------------"<<std::endl;
         }


}




