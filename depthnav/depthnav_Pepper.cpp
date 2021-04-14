/* Free sace naviagtion of Pepper Robot using Depth Senosr
 * Suman Raj Bista */

#include "vpNaoqiRobot.h"
#include "vpNaoqiGrabber.h"
#include "vpControl.hpp"
#include <al/alvisiondefinitions.h>
#include <al/from_any_value.hpp>
#include <qi/anyobject.hpp>
#include <qi/log.hpp>
#include "freespacenavigation.h"

bool eventraised;
vpNaoqiRobot *robo;

static void clickMouse( int event, int x, int y, int, void* ptr)
{
    if(event == cv::EVENT_LBUTTONDOWN){
        robo->setBaseVelocity(0.0,0.0,0.0);
        std::cout<<"place any key to continue (l,L,r,R,s) :";
        std::cin>>(char*)ptr;

    }
}


class MyService{


   public:
   //MyService(qi::SessionPtr session)
    MyService(qi::AnyObject &almemory/*, vpNaoqiRobot &robot*/)
    {

      //  qi::AnyObject almemory = session->service("ALMemory");
          almemory.call<void>("subscribeToEvent","ALMotion/MoveFailed","MyService","myCallback");
       // almemory.call<void>("subscribeToEvent","ALMotion/Safety/ChainVelocityClipped","MyService","myCallback2");
     //   almemory.call<void>("subscribeToMicroEvent","ALMotion/MoveFailed","MyService","clbkmsg","myCallback");
        almemory.call<void>("subscribeToMicroEvent","ALMotion/Safety/ChainVelocityClipped","MyService","cbmsg22","myCallback2");
        almemory.call<void>("subscribeToEvent","Navigation/"
                                               "","MyService","myCallback1");
     //   robo = &robot;
         eventraised = true;

    }



    void myCallback(const std::string &key, const qi::AnyValue &value, const  qi::AnyValue &message){

        qi ::AnyValue fd = value;

        qi ::AnyValue ms = message;
         std::cout<<"evet raised" <<"\t \t"<<fd.size()<<std::endl;
         std::cout<<key<<std::endl;
        // std::vector<double> wc = fd[2].to<std::vector<double>>();

         std::cout<<"mf = "<<fd[0].to<std::string>()/*<<"\t"<<fd[1].to<int>()<<"\t"<<"( "<<wc[0]<<" , "<<wc[1]<<" , "<<wc[2]<<" ) "*/<<std::endl;
         std::cout<<"ms ="<<ms.as<std::string>()<<std::endl;
         //robo->setBaseVelocity(0.0,0.0,0.0);
          eventraised = true;

    }

    void myCallback2(const std::string &key, const qi::AnyValue &value, const  qi::AnyValue &message){

        qi ::AnyValue fd = value;

        eventraised = true;

        qi ::AnyValue ms =    message;
         std::cout<<"evet raised" <<std::endl;
        std::cout<<key<<std::endl;

        if(fd.size()>0){

            qi::AnyValue rv = fd[0].to<qi::AnyValue>();

             std::cout<<rv[0].to<std::string>()<<"\t";

        //std::vector<double> wc = rv[1].to<std::vector<double>>();
             std::cout/*<<"\t"<<"( "<<wc[0]<<" , "<<wc[1]<<" , "<<wc[2]<<" ) "*/<<std::endl;
    }


        else{
            std::cout<<"Both"<<std::endl;
        }

        std::cout<<"ms ="<<ms.as<std::string>()<<std::endl;
        //robo->setBaseVelocity(0.0,0.0,0.0);

    }

    void myCallback1(const std::string &key, const qi::AnyValue &value, const  qi::AnyValue &message){

        qi ::AnyValue fd = value;

        qi ::AnyValue ms = message;
         std::cout<<"evet raised" <<std::endl;
        std::cout<<key<<std::endl;

     /*   if(fd.size()>0)
            std::cout<<fd[0].to<double>()<<"\t"<<fd[1].to<double>()<<std::endl;



        std::cout<<"ms ="<<ms.as<std::string>()<<std::endl;*/
        //robo->setBaseVelocity(0.0,0.0,0.0);
         eventraised = true;

    }


private:
//    qi::AnyObject _subscriber;
   // vpNaoqiRobot *robo;

   };

QI_REGISTER_MT_OBJECT(MyService, myCallback,myCallback2,myCallback1);

cv::Mat getPepperRT()

{
 double theta[3] ={-1.5936, -0.0161345, -1.57777};

    // Calculate rotation about x axis
    cv::Mat R_x = (cv::Mat_<double>(3,3) <<
               1,       0,              0,
               0,       cos(theta[0]),   -sin(theta[0]),
               0,       sin(theta[0]),   cos(theta[0])
               );

    // Calculate rotation about y axis
    cv::Mat R_y = (cv::Mat_<double>(3,3) <<
               cos(theta[1]),    0,      sin(theta[1]),
               0,               1,      0,
               -sin(theta[1]),   0,      cos(theta[1])
               );

    // Calculate rotation about z axis
    cv::Mat R_z = (cv::Mat_<double>(3,3) <<
               cos(theta[2]),    -sin(theta[2]),      0,
               sin(theta[2]),    cos(theta[2]),       0,
               0,               0,                  1);

   cv::Mat R = R_z * R_y * R_x;
    // Combined rotation matrix
 double T[3]  = {0.0708098, 0.02, 0.118115};

   cv::Mat Rt  = (cv::Mat_<double>(3,4
                                   ) <<
                    R.at<double>(0,0),     R.at<double>(0,1),       R.at<double>(0,2),  T[0],
                    R.at<double>(1,0),     R.at<double>(1,1),       R.at<double>(1,2),  T[1],
                    R.at<double>(2,0),     R.at<double>(2,1),       R.at<double>(2,2),   T[2] );
   return Rt;
}

void processdepth(cv::Mat &Id, cv::Mat &K)
{
   int r = Id.rows;
   int c = Id.cols;
   double X, Y, Z;

   for(int u = 0; u<c; u++){
       for(int v= 0; v<r; v++)
       {
           Z = Id.at<uint16_t>(u,v)/1000.0;
           X =1;
       }
   }
}

int main(int argc, char** argv)
{
    eventraised = false;

    try
   {

        std::string opt_ip = "172.19.226.236";

      int opt_cam = 0;
      bool opt_record = true;
      bool opt_VGA = false;
     // std::cout<<getPepperRT();

      cv::Mat K = (cv::Mat_<double>(3, 3) <<296.3437428191,0,158.2733787287, 0, 296.6441607237,118.0622191474 , 0, 0, 1);
   //   cv::Mat Kd = (cv::Mat_<double>(3, 3) <<283.0146, 0, 156.5386, 0, 283.095, 126.3683, 0, 0, 1 );
      cv::Mat Kd = (cv::Mat_<double>(3, 3) <<281.6392, 0, 161.8189, 0, 282.6421, 127.5787, 0, 0, 1 );


      for (unsigned int i=0; i<argc; i++) {
           if (std::string(argv[i]) == "--ip")
             opt_ip = argv[2];
           if (std::string(argv[i]) == "--cam")
             opt_cam = atoi(argv[i+1]);
           if (std::string(argv[i]) == "--record")
             opt_record = true;
           if (std::string(argv[i]) == "--vga")
             opt_VGA = true;
           else if (std::string(argv[i]) == "--help") {
             std::cout << "Usage: " << argv[0] << "[--ip <robot address>] [--record] [--cam num_camera] [--help]" << std::endl;
             return 0;
           }
         }

         // Create a session to connect with the Robot
         qi::SessionPtr m_session = qi::makeSession();
         std::string ip_port = "tcp://" + opt_ip + ":9559";
         m_session->connect(ip_port);
         if (! opt_ip.empty()) {
           std::cout << "Connect to robot with ip address: " << opt_ip << std::endl;
         }


         boost::shared_ptr<vpControl> ctr = boost::make_shared<vpControl>(m_session);
         m_session->registerService("pepper_control", ctr);

         // Open Grabber

     vpNaoqiGrabber g(m_session);
      g.setCamera(4);
      g.setFramerate(30);

     /* g.setCameraParameter(AL::kCameraAutoGainID,1);
      g.setCameraParameter(AL::kCameraAutoExpositionID,0);
      g.setCameraParameter(AL::kCameraAutoWhiteBalanceID,0);*/

      g.setCameraResolution(AL::kQVGA);
      g.open();

      std::cout << "Dimension image depth: " <<  g.getWidth() <<"x" << g.getHeight() << std::endl;

      vpNaoqiGrabber gd(m_session);
       gd.setCamera(0);
       gd.setFramerate(30);

      /* g.setCameraParameter(AL::kCameraAutoGainID,1);
       g.setCameraParameter(AL::kCameraAutoExpositionID,0);
       g.setCameraParameter(AL::kCameraAutoWhiteBalanceID,0);*/

       gd.setCameraResolution(AL::kQVGA);
       gd.open();

       std::cout << "Dimension image rgb: " <<  gd.getWidth() <<"x" << gd.getHeight() << std::endl;


     cv::Mat I;

 // vpImage<unsigned char> I(480/2,640/2); // Create a gray level image container
  vpNaoqiRobot robot(m_session);


//  vpDisplayOpenCV d(I);
  unsigned int num_frame = 0;



   qi::AnyObject robotPosture =  m_session->service("ALRobotPosture");
   robotPosture.call<void>("goToPosture","StandInit", 0.1f);

   qi::AnyObject motionproxy =  m_session->service("ALMotion");

   qi::AnyObject navproxy = m_session->service("ALNavigation");
   qi::AnyObject memoryproxy =  m_session->service("ALMemory");

   qi::AnyObject sonarproxy = m_session->service("ALSonar");
   qi::AnyObject laserproxy = m_session->service("ALLaser");
  // qi::AnyObject tabletproxy = m_session->service("ALTabletService");
   laserproxy.call<void>("laserON");
   sonarproxy.call<void>("subscribe","myApplication");


  qi::AnyObject robotAutoLife = m_session->service("ALAutonomousLife");
  robotAutoLife.call<void>("setAutonomousAbilityEnabled","BasicAwareness", false);


  qi::AnyValue val, valL,valR;

  int ct = 0;
  int stoptime = 4;


  std::vector<std::string> names  = {"HeadYaw", "HeadPitch"};
  std::vector<float> angles = {0.0f, 0.0f};
  motionproxy.call<void>("setAngles", names, angles, 0.175f);



  motionproxy.call<void>("setExternalCollisionProtectionEnabled","All",true);

 /* vpPlot plot(1,700,700,100,100,"Dist Laser");
  plot.initGraph(0,2);
  plot.initRange(0,-1,320,-1,1);*/

 // fd = memoryproxy.call<qi::AnyValue>("getData","Navigation/AvoidanceNavigator/ObstacleDetected");
  //naoqi::tools::fromAnyValueToFloatVector(fd, result_value);
 //std::cout<<"fid = "<<fd[0] <<"\t"<<fd[1]<<std::endl;
 cv::Mat Id;

 for(int i=0;i<10;i++){
     g.acquiredepth(I);
    // gd.acquirevoxel(Id);
    gd.acquire(Id);

 }

 std::vector<float> tt;

 tt = motionproxy.call<std::vector<float>>("getRobotPosition",true);

 std::vector<float> tt1;
 tt1 = motionproxy.call<std::vector<float>>("getRobotPosition",false);

 std::ofstream ofile("odotext.txt");

 ct = 0;
 int i = 0;

 double v[2];
 v[1] = 0;
 v[0] = 0.0;

  m_session->registerService("MyService", qi::AnyObject(boost::make_shared<MyService>(memoryproxy/*,robot*/)));
  cv::namedWindow("disp");
  char cc;
 cv::Mat Idd, bd;
 cv::Mat diffs;
  cv::setMouseCallback("disp",clickMouse,&cc);
 cc = 0;
 double min;
 double max;


 freespacenavigation fsn(Kd);
 fsn.setbasevel(0.18);
 fsn.setinitialpose(tt);
 int flag = 0;
 cv::Mat Igrid;
 std::vector<double> od;
 od.reserve(3);
  robo = &robot;
   while(1) {


       std::cout<<"\n **************************************************************"<<std::endl;

        const clock_t begin_time = clock();
       // double t = vpTime::measureTimeMs();
        g.acquiredepth(I);
       // gd.acquirevoxel(Id);
         gd.acquire(Id);

    //plot.resetPointList(0);
       std::stringstream currimName, currimName2;
       ct++;

       if(ct<10)
        currimName<<"img_0000" <<ct<<".png";
       else if(ct<100)
           currimName<<"img_000" <<ct<<".png";
       else if(ct<1000)
           currimName<<"img_00" <<ct<<".png";
       else if(ct<10000)
           currimName<<"img_0" <<ct<<".png";
       else
           currimName<<"img_" <<ct<<".png";

     //  dls.setImname(ct);

      /* val = motionproxy.call<qi::AnyValue>("isCollision","Arms");
       valL = motionproxy.call<qi::AnyValue>("isCollision","LArm");
       valR = motionproxy.call<qi::AnyValue>("isCollision","RArm");
       std::cout<<val.asString()<<"\t"<< valL.asString()<<"\t"<<valR.asString()<<std::endl; */

       //std::vector<float> valR, valL;

       //valL = motionproxy.call< std::vector<float> >("getChainClosestObstaclePosition", "LArm", 0);
       //valR = motionproxy.call< std::vector<float> >("getChainClosestObstaclePosition", "RArm", 0);


     //  std::cout<<"Lt Arm"<<"\t"<<valL[0]<<"\t"<<valL[1]<<"\t"<<valL[2]<<std::endl;
     //  std::cout<<"Rt Arm"<<"\t"<<valR[0]<<"\t"<<valR[1]<<"\t"<<valR[2]<<std::endl;


       std::vector<float> tx=motionproxy.call<std::vector<float>>("getRobotPosition",true);
    //  AL::Math::Pose2D currPose(tx);
     // std::vector<float> tx1=motionproxy.call<std::vector<float>>("getRobotPosition",false);

    //   AL::Math::Pose2D diffpose =  AL::Math::pose2DInverse(currPose)*poseinitial;

    fsn.setcurrentpose(tx);

        //std::cout<<"I pose ="<<pi.x<<"\t"<<pi.y<<"\t"<<pi.theta<<std::endl;
       //  std::cout<<"I posed ="<<diffpose.x<<"\t"<<diffpose.y<<"\t"<<diffpose.theta<<std::endl;

       motionproxy.call<void>("setAngles", names, angles, 0.175f);
     cv::imwrite(currimName.str(),Id);
    //cv::medianBlur(I,I,7);
    double fs = 0;
    std::vector<double> wr;
    wr.reserve(4);
    std::vector<double> WR;
    WR.reserve(4);

    double fs1 = 0.0;
    double fs2 = 0.0;
    double fs3 = 0.0;
    double fsx = 0.0;

    /*if(ct==1){ od[0]=0; od[1]=0; od[2]=0;}
     else{

    od[0] = -diffpose.x;
    od[1] = diffpose.y;
    od[2] = diffpose.theta;
    }

    poseinitial = currPose;
    */
    //dls.readlasersonar(memoryproxy);
    //fsconvert_msg(I,Igrid, od,wr,WR);
    fs = fsn.getvelfreespace(I);


      cv::minMaxIdx(I, &min, &max);

     cv::convertScaleAbs(I, bd, 255 / max);


     //fs1 = wr[0]; fs2 = wr[1]; fs3 =  wr[2]; fsx = wr[4];

     //* wr[0] =  ((fs1-0)+fs1*0.18/1)/(1+fs1*fs1);
     // wr[1] =  ((fs2-0)+fs2*0.18/2)/(1+fs2*fs2);
      //wr[2] =  ((fs3-0)+fs3*0.18/3.25)/(1+fs3*fs3);
    //  wr[3] =  ((fsx-0)+fsx*0.18/0.9)/(1+fsx*fsx);

    /*for(int idx=0;idx<320;idx++){
      //std::cout<<lscan[idx]<<"\t";
      if(!std::isnan(lscan[idx]))
      plot.plot(0,0,idx*1.0,lscan[idx]);
    }
    std::cout<<std::endl;*/

      cv::imshow("disp",bd);
    //cv::imwrite(currimName.str(),bd);
    //cv::imshow("2dgrid",Igrid);

       cv::waitKey(10);



      //std::cout<<std::endl;

       /* fs = (0.6*wr[1] + 0.4*wr[2]);

      // if((wr[1]>0 &wr[2]<0) | (wr[1]<0 & wr[2]>0)){ fs = (0.5*wr[1] + 0.5*wr[0]);}
        if(std::isnan(wr[2])) fs = (0.5*wr[1] + 0.5*wr[0]);
        if(std::isnan(wr[1])) fs = (0.6*wr[3] + 0.4*wr[0]);
        if(std::isnan(wr[0]) )  fs = std::numeric_limits<float>::quiet_NaN();

    */

    //memoryproxy.call<void>("subscribeToMicroEvent","ALMotion/MoveFailed","MyModule","helloo","mycallback");

    //  g.acquirevoxel(Id);

       std::cout<<"\n ......................................................"<<std::endl;

    if(std::isnan(fs)){
        v[0] = 0;
        v[1] = 0;
       std::cout<<"obs det"<<std::endl;

    }
    else{

      v[0]= 0.2;
    //  v[1] = (0.1*(fs-0)+fs*v[0]/2)/(1+fs*fs);
      v[1] = fs;
      std::cout<<"wro = "<<v[1]<<std::endl;
      if(fabs(v[1]>0.3))
          v[1] = 0.3*v[1]/fabs(v[1]);



    }
    double wrx = v[1];
      std:: cout<<"wr = "<<v[1]<<std::endl;
      if(fabs(wrx)<0.00001) std::cout<<"STRAIGHT"<<std::endl;
      else if(wrx>0) std::cout<<"LEFT"<<std::endl;
      else if(wrx<0) std::cout<<"RIGHT"<<std::endl;
      else  std::cout<<"STRAIGHT"<<std::endl;
     // v[0] = 0;
    //v[1] = 0;
        //fd = memoryproxy.call<std::vector<float> >("getData","Navigation/AvoidanceNavigator/ObstacleDetected");
        //naoqi::tools::fromAnyValueToFloatVector(fd, result_value);
    //std::cout<<"fid = "<<fd[0] <<"\t"<<fd[1]<<std::endl;
    if(flag==1){flag =0; v[0]=0.0; v[1]=0.0;}

      std::cout<<std::setw(5)<<"Odo red = "<<std::setw(12)<<tx[0]-tt[0]<<"\t"<<std::setw(12)<<tx[1]-tt[1]<<"\t"<<std::setw(12)<<tx[2]-tt[2]<<"\t"<<std::setw(12)<<std::endl;
     //  ofile<<std::setw(5)<<ct+1<<std::setw(12)<<od[0]<<"\t"<<std::setw(12)<<od[1]<<"\t"<<std::setw(12)<<od[2]<<"\t"<<std::setw(12)<<tt1[0]-tx1[0]<<"\t"<<std::setw(12)<<tt1[1]-tx1[1]<<"\t"<<std::setw(12)<<tt1[2]-tx1[2]<<std::endl;
     //ofile<<std::setw(5)<<ct+1<<std::setw(12)<<od[0]<<"\t"<<std::setw(12)<<od[1]<<"\t"<<std::setw(12)<<od[2]<<"\t"<<std::setw(12)<<diffpose.x<<"\t"<<std::setw(12)<<diffpose.y<<"\t"<<std::setw(12)<<diffpose.theta<<"\t"<<v[1]<<std::endl;

      if (cc != 0){


         robot.setBaseVelocity(0.0,0.0,0.0);
         std::cin>>cc;

             if(cc=='s'){
                 cc = 0;
                 break;
             }
              cc = 0;
           }

if(eventraised){
    eventraised = false;
       robot.setBaseVelocity(0.0,0.0,0.0);
    std::cout<<" I am forced to stop :("<<std::endl;

}
else{
    robot.setBaseVelocity(v[0],0.0,v[1]);

    //   std::cout<<ct<<"\t";
}

    //   tabletproxy.call<void>("showImage",currimName.str());
        //motionproxy.call<void>("setAngles", names, angles, 0.175f);

  //     std::cout<<tx[0]<<"\t"<<tx[1]<<"\t"<<tx[2]<<std::endl;
     //  vpTime::wait(t,stoptime);
      std::cout <<std::endl <<"Time exec = \t"<<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<std::endl;
    //   ofile<<std::setw(5)<<i+1<<std::setw(12)<<tx[0]-tt[0]<<"\t"<<std::setw(12)<<tx[1]-tt[1]<<"\t"<<std::setw(12)<<tx[2]-tt[2]<<"\t"<<std::setw(12)<<std::endl;
      i++;
     }



  ofile.close();

  //robot.stopBase();

  //robot.stopPepperControl();



  }
  catch (int e)
  {
    std::cerr << "Caught exception: "  << std::endl;
  }


  return 0;

}

