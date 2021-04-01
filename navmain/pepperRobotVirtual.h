/* Interface with virtual pepper (images from folder)via ppper_qi
 * higher level functionlities => just for testing 
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


#ifndef PEPPERROBOTVIRTUAL_H
#define PEPPERROBOTVIRTUAL_H

#include "pepperInterface.h"
#include "vpNaoqiRobot.h"
#include "vpNaoqiGrabber.h"
#include "vpControl.hpp"
#include "pepperevents.h"

class pepperRobotVirtual: public pepperInterface
{
private:
    vpNaoqiGrabber *g,*gd;         //pointer to pepper frame grabbers
    vpNaoqiRobot *robot;           // pointer to pepper robot
    qi::SessionPtr m_session;      // session pointer
    qi::AnyObject robotPosture;    //Pepper Robot Proxies
    qi::AnyObject* motionproxy;
    qi::AnyObject navproxy;
    qi::AnyObject memoryproxy;
    qi::AnyObject robotAutoLife;
    boost::shared_ptr<vpControl> ctr;   // pointer for Pepper_Control
    cv::Mat I;
    cv::Mat K;
    cv::Mat Kd;
    int camid;
    std::string imgfold;
     std::vector<cv::String> imlist;
int indx;
public:
    /**
     * @brief pepperRobotVirtual
     * @param opt_ip iamge folder
     */

    pepperRobotVirtual(const std::string &opt_ip);

    /**
     * @brief openCamera Start RGB Camera
     * @param id top= 0 buttom =1
     */

    void openCamera(int id=0);

    /**
     * @brief openDepthCamera start Depth Camera
     */

    void openDepthCamera();

    /**
     * @brief startBaseMotionController
     */

    void startBaseMotionController();

    /**
     * @brief initPosture set initial prefdefined posture for navigation
     */

    void initPosture();

    /**
     * @brief startPepper
     * @param id RGB camera id
     */

    void startPepper(int id);

    /**
     * @brief getCurrImage  acquire image from RGB camera
     * @param I  image in cv::Mat
     */
    void getCurrImage(cv::Mat &I);

    /**
     * @brief getCurrDepthImage acquire image from depth camera
     * @param I image in cv::Mat
     */

    void getCurrDepthImage(cv::Mat &I);

    /**
     * @brief setBaseVelocities
     * @param vr forward velocity
     * @param wr rotational velocity
     */

    void setBaseVelocities(float vr, float wr);

    /**
     * @brief setBaseVelocities
     * @param vr forward velocity
     * @param vs lateral transaltional velocity
     * @param wr rotational velocity
     */

    void setBaseVelocities(float vr, float vs, float wr);

    /**
     * @brief getOdometryReading read current odometry reading
     * @return odometry value (x, y, theta)
     */
    std::vector<float> getOdometryReading();

    /**
     * @brief rotate180 rotate robot by 180
     */

    void rotate180();

    /**
     * @brief wait delay
     * @param t time in ms
     */

    void wait(long t);

    /**
     * @brief adjusthead correct the head postion so that the robot always look forward
     */

    void adjusthead();

    /**
     * @brief getK instrinsic parameter of top RGB camera
     * @return Instrinsic Matrix
     */

    cv::Mat getK();

    /**
     * @brief getKd instrinsic parameter of depth camera
     * @return Instrinsic Matrix
     */

    cv::Mat getKd();

    /**
     * @brief getmode tells it is the online navigation mode for pepper
     * @return  0
     */

    int getmode();

    /**
     * @brief getid returns camera id
     * @return 0=> top cam 1=>bttom cam
     */

    int getid();


};

#endif // PEPPERROBOT_H
