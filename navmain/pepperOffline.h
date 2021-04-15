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

#ifndef PEPPEROFFLINE_H
#define PEPPEROFFLINE_H

#include "pepperInterface.h"


class pepperOffline: public pepperInterface
{
private:
    cv::Mat I;
    cv::Mat K;
    cv::Mat Kd;
    std::vector<float> odom;
    std::string imgfold;
    std::vector<cv::String> imlist;
    int indx;
    int destid;
public:

    /**
         * @brief pepperOffline
         * @param opt_ip = path of the folder where navigation image lies
         */
        pepperOffline(const std::string &opt_ip);

        /**
         * @brief openCamera reads image file names from the folder
         * @param id
         */

        void openCamera(int id=0);

        /* they are just empty for compatibility */
        void openDepthCamera();
        void startBaseMotionController();
        void initPosture();

        /**
         * @brief startPepper  initialies image path for image-based localizations
         * @param id
         */
        void startPepper(int id);

        /**
         * @brief getCurrImage reads image from folder
         * @param I
         */
        void getCurrImage(cv::Mat &I);

        /* they are just empty for compatibility */
        void getCurrDepthImage(cv::Mat &I);
        void setBaseVelocities(float vr, float wr);
        void setBaseVelocities(float vr, float vs, float wr);
        std::vector<float> getOdometryReading();
        void rotate180();

        /**
         * @brief wait delay in ms
         */
        void wait(long t);

        /**
         * return intrinsic parameters
         */

        cv::Mat getK();
        cv::Mat getKd();

        /**
         * @brief getmode tells it is offline mode
         * @return 1
         */
        int getmode();

        /* they are just empty for compatibility */
        void adjusthead();
        int getid();

};

#endif // PEPPEROFFLINE_H
