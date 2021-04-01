
#include <math.h>
#include <time.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <string>
#include <sstream>

#include <iostream>
//#include <QString>
//#include <QDir>
//#include <QStringList>
#include <algorithm>



#include <ctime>
#include "linenav/kimread.h"
#include "navigation.h"
#include "linenav/dispnav.h"

#include <vpNaoqiGrabber.h>
#include <vpNaoqiRobot.h>
#include <al/alvisiondefinitions.h>
#include <qi/session.hpp>
#include <qi/applicationsession.hpp>
#include <qi/anymodule.hpp>
#include <qi/anyobject.hpp>
#include "vpControl.hpp"



using namespace std;

void usage(int argc, char** argv){
    std::cout<<"Usage: "<<argv[0]<<" keyframepath "<<std::endl;
}

const std::string currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d_%H.%M.%S", &tstruct);

  return buf;
}



int main(int argc, char** argv)
{
    int ret = -1;

    if(argc<2){
        usage(argc,argv);
        return ret;
    }


 //int startindex = 0;
// start index from current acquired images

   // cv::Mat K = (cv::Mat_<double>(3, 3) <<594.4009264,0,319.2854032, 0, 592.4133601,233.5686585 , 0, 0, 1);
    cv::Mat K = (cv::Mat_<double>(3, 3) <<296.3437428191,0,158.2733787287, 0, 296.6441607237,118.0622191474 , 0, 0, 1);
 try
 {

            std::string opt_ip = "172.19.226.238";

             int opt_cam = 0;
             bool opt_record = true;
             bool opt_VGA = false;

      // Create a session to connect with the Robot
        qi::SessionPtr m_session = qi::makeSession();
        std::string ip_port = "tcp://" + opt_ip + ":9559";
         m_session->connect(ip_port);
         if (! opt_ip.empty()) {
             std::cout << "Connect to robot with ip address: " << opt_ip << std::endl;
           }

// Open Grabber

         boost::shared_ptr<vpControl> ctr = boost::make_shared<vpControl>(m_session);
                  m_session->registerService("pepper_control", ctr);

   vpNaoqiGrabber g(m_session);


         g.setCamera(0);
         g.setFramerate(30);
         g.setCameraParameter(AL::kCameraAutoGainID,1);
         g.setCameraParameter(AL::kCameraAutoExpositionID,0);
         g.setCameraParameter(AL::kCameraAutoWhiteBalanceID,0);

               g.setCameraResolution(AL::kQVGA);
         g.open();
        // std::cout << "Open camera parameters: " << g.getCameraParameters() << std::endl;
         std::cout << "Dimension image: " << g.getWidth() <<"x" << g.getHeight() << std::endl;

         cv::Mat I;

      //  vpImage<unsigned char> I; // Create a gray level image container
        g.acquire(I);

         std::cout << "Image size: " << I.cols << " " << I.rows << std::endl;

          vpNaoqiRobot robot(m_session);
          robot.open();




 	qi::AnyObject robotPosture =  m_session->service("ALRobotPosture");
	 robotPosture.call<void>("goToPosture","StandInit", 0.1f);


     std::vector<std::string> names  = {"HeadYaw", "HeadPitch"};
     std::vector<std::string> bodynames  = {"KneePitch", "HipPitch", "HipRoll"};
     std::vector<float> angles = {0.0f, 0.0f};
     std::vector<float> bodyangles = {0.0f, -0.12f, 0.0f};


     robot.getMotionProxy()->call<void>("setAngles", names, angles, 0.175f);
     robot.getMotionProxy()->call<void>("setAngles", bodynames, bodyangles, 0.01f);

     qi::AnyObject robotAutoLife = m_session->service("ALAutonomousLife");
     robotAutoLife.call<void>("setAutonomousAbilityEnabled","BasicAwareness", false);

     robotAutoLife.call<void>("setAutonomousAbilityEnabled","BackgroundMovement", false);




    robot.getMotionProxy()->call<void>("setAngles", names, angles, 0.175f);

        if (robot.getRobotType() != vpNaoqiRobot::Pepper)
        {
          std::cout << "ERROR: You are not connected to Pepper, but to a different Robot. Check the IP. " << std::endl;
          return 0;
        }

      cv::Mat cim;

 robot.getMotionProxy()->call<void>("setAngles", names, angles, 0.175f);

                 for(int i=0;i<10;i++){
                       g.acquire(I);
                       //vpTime::wait(100);

                 }




         cv::cvtColor(I,cim,cv::COLOR_BGR2GRAY);



          kimRead kf(argv[1]);

          std::string imdir(argv[1]);
          imdir += "/";

            dispNav *d;
            d=new dispNav;

            d->saveimage(true);
            d->displayimage(true);

            navigation nav(d,K);

             d->setDisptime(100);











            nav.initlocalisation(cim,kf);
          //  cv::waitKey(0);


            robot.setBaseVelocity(0.0,0.0,0.0);


          std::vector<std::string>  bodyname = robot.getBodyNames("Body");
          std::vector<float> values(bodyname.size(),0.0);





      //  std::string currim;
         int flag;
         int count = 0;
         int islast;
         double rv = 0.0;


         double v[2];

             v[0]=0.05;
             v[1]=rv;



         std::ofstream ofilepose("poseinfo.txt");

         std::vector<float> tt;

          tt = robot.getMotionProxy()->call<std::vector<float>>("getRobotPosition",true);
       //   tt = robot.getProxy()->getRobotPosition(true);

      //   std::cout<<tt[0]<"\t"<<tt[1]<<"\t"<<tt[2]<<std::endl;
 std::cout<<"---------------------------------------------"<<std::endl;

         double sp = 1.0;
         int flno = 0;
         //for(int kk= startindex;kk<currims.length();kk++)
         int kk = 0;
         while(1)
         {
      clock_t tStart = clock();
           // currim.assign(imdir+currims.at(kk).toStdString());
            //std::cout<<currim;
           // cim = cv::imread(currim,CV_LOAD_IMAGE_GRAYSCALE);

            g.acquire(I);
           //make std::vector<float> tx;
            robot.getMotionProxy()->call<void>("setAngles", names, angles, 0.175f);

       //      tx = robot.getProxy()->getRobotPosition(true);
            std::vector<float> tx=robot.getMotionProxy()->call<std::vector<float>>("getRobotPosition",true);
             cv::cvtColor(I,cim,cv::COLOR_BGR2GRAY);

            std::stringstream aa;
            if(flno<10)
              aa<<"jmg0000"<<flno<<".png";
            else if (flno<100)
                 aa<<"jmg000"<<flno<<".png";
            else if (flno<1000)
                    aa<<"jmg00"<<flno<<".png";
            else if (flno<10000)
                    aa<<"jmg0"<<flno<<".png";
            else
                aa<<"jmg"<<flno<<".png";

            cv::imwrite(aa.str().c_str(),cim);
            flno++;
          //  robot.getMotionProxy()->call<void>("setAngles", bodyname, values, 0.5f);

           // robot.getProxy()->setAngles(bodyname,values,0.5f);

            sp = 1.0;
 std::cout<<"------------step------------------"<<std::endl;
            flag = nav.step(cim);
           // std::cout<<flag<<std::endl;
            if(flag==0) count = 0;

            else if(flag == 1) count++;
            else if(flag <0) { v[0] = 0.0; v[1] = 0.0; robot.setBaseVelocity(v[0],0.0,v[1]); continue;}
            else continue;


            rv = nav.getRotVel();
            std::cout<<"Rot Vel = "<<rv<<std::endl;
            v[1] = rv;
            if(fabs(rv)>0.08)
              sp = 0.3;

            v[0]  = sp*0.15;


            robot.setBaseVelocity(v[0],0.0,v[1]);
 std::cout<<".................................................................."<<std::endl;

       ofilepose<<setw(5)<<kk<<"\t"<<setw(10)<<tx[0]-tt[0]<<"\t"<<setw(10)<<tx[1]-tt[1]<<"\t"<<setw(10)<<tx[2]-tt[2]<<"\t"<<setw(10)<<v[1]<<std::endl;
        kk++;
            if(count>2){
              //  count = 0;
                islast = nav.SwitchtoNewKeyImages(kf);
               robot.getMotionProxy()->call<void>("setAngles", names, angles, 0.175f);

                if(islast){
                    std::cout<<"end of topological navigation"<<std::endl;
                    break;
                }
            }
            std::cout<<"\n exec time = "<<((double)(clock() - tStart)/CLOCKS_PER_SEC)<<std::endl;
      std::cout<<"---------------------------------------------"<<std::endl;

         }

  robot.stopBase();
  g.cleanup();
 // robotAutoLife.call<void>("setAutonomousAbilityEnabled","BasicAwareness", true);

  //robotAutoLife.call<void>("setAutonomousAbilityEnabled","BackgroundMovement", false);


        }


 catch (int e)
 {
   std::cerr << "Caught exception: "<< std::endl;
 }

 return 0;

}




