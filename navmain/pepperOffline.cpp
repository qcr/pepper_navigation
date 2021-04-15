/* Interface in offline mode
 * higher level functionlities
 * Read images from the folder and oerform image-based localisation only
 * Pepper robot not use
 * for image-based localisation only
 * for debugging purposes
 *
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
 * ACRV @ QUT
 */


#include "pepperOffline.h"
#include<thread>


    pepperOffline::pepperOffline(const std::string &opt_ip):pepperInterface(opt_ip){

         K = (cv::Mat_<double>(3, 3) <<296.3437428191,0,158.2733787287, 0, 296.6441607237,118.0622191474 , 0, 0, 1);
         Kd = (cv::Mat_<double>(3, 3) <<281.6392, 0, 161.8189, 0, 282.6421, 127.5787, 0, 0, 1 );

         // Start pepper control
         odom.push_back(0.0);
         odom.push_back(0.0);
         odom.push_back(0.0);

         imgfold = opt_ip;
         imgfold += "/";
         indx = 0;

         destid = 0;

    }

    void pepperOffline::openCamera(int id){

    cv::glob(imgfold + "*.png",imlist);
    destid = id;

    }

    void pepperOffline::openDepthCamera(){

    }

    void pepperOffline::getCurrDepthImage(cv::Mat &I){

    }

    cv::Mat pepperOffline::getK(){
        return K.clone();
    }
    cv::Mat pepperOffline::getKd(){
        return Kd.clone();
    }

    void pepperOffline::startBaseMotionController(){

    }

    void pepperOffline::initPosture(){

    }

    void pepperOffline::startPepper(int id = 0){

        openCamera(id);
        startBaseMotionController();
        initPosture();

    }

    void pepperOffline::adjusthead(){

    }

    void pepperOffline::getCurrImage(cv::Mat &I)
    { 
        //std::cout<<imlist.at(indx)<<std::endl;
        I = cv::imread(imlist.at(indx));
        indx++;
    }

    void  pepperOffline::setBaseVelocities(float vr, float wr){


    }

    void pepperOffline::setBaseVelocities(float vr, float vs, float wr){

    }

    std::vector<float> pepperOffline::getOdometryReading(){

       return (odom);
    }

    void pepperOffline::rotate180()
    {
    }

    void pepperOffline::wait(long t){

        std::this_thread::sleep_for(std::chrono::milliseconds(t));
    }

    int pepperOffline::getmode(){
        return 1;
    }

    int pepperOffline::getid(){
        return(destid);
    }
