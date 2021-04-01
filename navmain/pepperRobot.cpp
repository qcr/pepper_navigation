/* Interface with pepper via ppper_qi
 * higher level functionlities
 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/
/*
 * Author: Suman Raj Bista
 */


#include "pepperRobot.h"
#include <al/alvisiondefinitions.h>
#include <al/from_any_value.hpp>
#include <chrono>
#include <thread>

    /**
     * @brief pepperRobot
     * @param opt_ip url of Pepper
     */

    pepperRobot::pepperRobot(const std::string &opt_ip):pepperInterface(opt_ip){

         m_session = qi::makeSession();
         std::string ip_port = "tcp://" + opt_ip + ":9559";
         m_session->connect(ip_port);

         if (! opt_ip.empty()) {
              std::cout << "Connect to robot with ip address: " << opt_ip << std::endl;
         }

         K = (cv::Mat_<double>(3, 3) <<296.3437428191,0,158.2733787287, 0, 296.6441607237,118.0622191474 , 0, 0, 1);
         Kd = (cv::Mat_<double>(3, 3) <<281.6392, 0, 161.8189, 0, 282.6421, 127.5787, 0, 0, 1 );
         camid = 0;
    }


    /**
     * @brief openCamera Start RGB Camera
     * @param id top= 0 buttom =1
     */

    void pepperRobot::openCamera(int id){

        g = new vpNaoqiGrabber(m_session);
        camid = id;
        g->setCamera(id);
        g->setFramerate(30);
        g->setCameraParameter(AL::kCameraAutoGainID,1);
        g->setCameraParameter(AL::kCameraAutoExpositionID,0);
        g->setCameraParameter(AL::kCameraAutoWhiteBalanceID,0);

        g->setCameraResolution(AL::kQVGA);
        g->open();

       std::cout << "Dimension image: " << g->getHeight() <<"x" << g->getWidth() << std::endl;
        K = (cv::Mat_<double>(3, 3) <<296.3437428191,0,158.2733787287, 0, 296.6441607237,118.0622191474 , 0, 0, 1);
        Kd = (cv::Mat_<double>(3, 3) <<281.6392, 0, 161.8189, 0, 282.6421, 127.5787, 0, 0, 1 );

       for(int i=0;i<10;i++){
        g->acquire(I);
        wait(1);
       }
    }


    /**
     * @brief openDepthCamera start Depth Camera
     */

    void pepperRobot::openDepthCamera(){

        gd= new vpNaoqiGrabber(m_session);
        gd->setCamera(4);
        gd->setFramerate(30);
        gd->setCameraResolution(AL::kQVGA);
        gd->open();
        std::cout << "Dimension depth image: " << gd->getHeight() <<"x" << gd->getWidth() << std::endl;

        for(int i=0;i<10;i++){
         gd->acquire(I);
         wait(1);
        }
    }

    /**
     * @brief getK instrinsic parameter of top RGB camera
     * @return Instrinsic Matrix
     */

    cv::Mat pepperRobot::getK(){

        return K.clone();
    }


    /**
     * @brief getKd instrinsic parameter of depth camera
     * @return Instrinsic Matrix
     */

    cv::Mat pepperRobot::getKd(){

        return Kd.clone();
    }


    /**
     * @brief startBaseMotionController
     */

    void pepperRobot::startBaseMotionController(){

        ctr = boost::make_shared<vpControl>(m_session);
        m_session->registerService("pepper_control", ctr);

        robot = new vpNaoqiRobot(m_session);
        robot->open();
        motionproxy = robot->getMotionProxy();
        navproxy = m_session->service("ALNavigation");
        robotAutoLife = m_session->service("ALAutonomousLife");
        memoryproxy =  m_session->service("ALMemory");

        robotAutoLife.call<void>("setAutonomousAbilityEnabled","BasicAwareness", false);
        motionproxy->call<void>("setExternalCollisionProtectionEnabled","All",true);
        motionproxy->call<void>("setCollisionProtectionEnabled","Arms",true);
        m_session->registerService("pepperServices", qi::AnyObject(boost::make_shared<pepperServices>(memoryproxy)));
    }


    /**
     * @brief initPosture set initial prefdefined posture for navigation
     */

    void pepperRobot::initPosture(){

        robotPosture =  m_session->service("ALRobotPosture");
        robotPosture.call<void>("goToPosture","StandInit", 0.5f);

        std::vector<std::string> names  = {"HeadYaw", "HeadPitch"};
        std::vector<float> vals = {0.5f, 0.5f};
        motionproxy->call<void>("stiffnessInterpolation",names,vals,2.0);
        wait(2);
        std::vector<float> angles = {0.0f, 0.0f};

         motionproxy->call<void>("setAngles", names, angles, 0.175f);
          wait(10);
    }


    /**
     * @brief startPepper
     * @param id RGB camera id
     */

    void pepperRobot::startPepper(int id = 0){

        openCamera(id);
        startBaseMotionController();
        initPosture();
    }


    /**
     * @brief adjusthead correct the head postion so that the robot always look forward
     */

    void pepperRobot::adjusthead(){
        std::vector<std::string> names  = {"HeadYaw", "HeadPitch"};
        std::vector<float> angles = {0.0f, 0.0f};
        motionproxy->call<void>("setAngles", names, angles, 0.175f);
    }


    /**
     * @brief getCurrImage  acquire image from RGB camera
     * @param I  image in cv::Mat
     */

    void pepperRobot::getCurrImage(cv::Mat &I){

        g->acquire(I);
    }


    /**
     * @brief getCurrDepthImage acquire image from depth camera
     * @param I image in cv::Mat
     */

    void pepperRobot::getCurrDepthImage(cv::Mat &I){

         gd->acquiredepth(I);
    }

    /**
     * @brief setBaseVelocities
     * @param vr forward velocity
     * @param wr rotational velocity
     */

    void  pepperRobot::setBaseVelocities(float vr, float wr){

        robot->setBaseVelocity(vr,0.0,wr);
    }

    /**
     * @brief setBaseVelocities
     * @param vr forward velocity
     * @param vs lateral transaltional velocity
     * @param wr rotational velocity
     */

    void  pepperRobot::setBaseVelocities(float vr, float vs, float wr){

        robot->setBaseVelocity(vr,vs,wr);
    }


    /**
     * @brief getOdometryReading read current odometry reading
     * @return odometry value (x, y, theta)
     */

    std::vector<float> pepperRobot::getOdometryReading(){

        return(motionproxy->call<std::vector<float> >("getRobotPosition",true));
    }


    /**
     * @brief rotate180 rotate robot by 180
     */

    void pepperRobot::rotate180(){

         std::vector<float> ts = motionproxy->call<std::vector<float>>("getRobotPosition",true);
         double sa = ts[2];
         double te = sa;
         if(sa<0) sa+= 2*3.1415;
         double ca;

         do{
             ts = motionproxy->call<std::vector<float> >("getRobotPosition",true);
             ca = ts[2];

             robot->setBaseVelocity(0,0,0.3);
             std::cout<<"Angle = "<<(ts[2]-te)*180/3.1415<<"\t \t"<<(ca-sa)*180/3.1415<<std::endl;
         } while(fabs(fabs(ca-sa)-3.1415)>=0.01);

         std::cout<<"Angle = "<<(ts[2]-te)*180/3.1415<<"\t \t"<<(ca-sa)*180/3.1415<<std::endl;

          std::cout<<"180 turned"<<std::endl;

          robot->setBaseVelocity(0.0,0.0,0.0);
          wait(1);
          robot->setBaseVelocity(0.0,0.0,0.0);
    }


    /**
     * @brief getmode tells it is the online navigation mode for pepper
     * @return  0
     */

    int pepperRobot::getmode(){
        return 0;
    }


    /**
     * @brief getid returns camera id
     * @return 0=> top cam 1=>bttom cam
     */

    int pepperRobot::getid(){
        return camid;
    }


    /**
     * @brief wait delay
     * @param t time in ms
     */

    void pepperRobot::wait(long t){

        std::this_thread::sleep_for(std::chrono::milliseconds(t));
    }
