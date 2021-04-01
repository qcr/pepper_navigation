/* freespace navigation class perform navigation in free space
 * It reuires depth image and odometry value. It outputs rotational velocity
 * necessary for free space navigation anf parameter H for fusion of control as
 * discussed in the papaer
 */


/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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



#ifndef FREESPACENAVIGATION_H
#define FREESPACENAVIGATION_H

#include "DepthImageScanner.h"
#include "alpose2d.h"

class freespacenavigation
{
        depthimagescanner::DepthImageScanner *dls;
        cv::Mat Igrid;
        AL::Math::Pose2D initialPose;
        AL::Math::Pose2D currPose;
        AL::Math::Pose2D diffPose;
        std::vector<double> od;
        int flno;


        std::vector<double> wrd;
        std::vector<double> wrx;
        std::vector<double> wr;
        double dis[4];

        double fs1;
        double fs2;
        double fs3;
        double fsx;
        int flagx;
        double H;
        double v;


public:

    /**
     * @brief freespacenavigation constructor
     * @param kd : instrinsic parmaters of the depth camera
     */

         freespacenavigation(cv::Mat &kd);

    /**
     * @brief freespacenavigation constructor
     * @param kd : instrinsic parmaters of the depth camera
     * @param scan_height : no of rows from center considered from the depth image for 2d grid map
     */
            freespacenavigation(cv::Mat &kd, int scan_height);

    /**
     * @brief setinitialpose initial odometry reading
     * @param pose vector consists of odometry value x,y, and theta
     */

            void setinitialpose(const std::vector<float> &pose);

    /**
     * @brief setcurrentpose set current odometry reading
     * @param pose vector consists of odometry value x,y, and theta
     */

             void setcurrentpose(const std::vector<float> &pose);

    /**
     * @brief getvelfreespace returns velocity for freespace navigation
     * @param Id depth image
     * @return rotational velocity
     */
    double getvelfreespace(cv::Mat &Id);

    /**
     * @brief getH  returns weights required for fusion of control
     * @return H
     */
    double getH();

    /**
     * @brief setbasevel set base forward velocity. Required for ibvs
     * @param vt forward velocity
     */
    void setbasevel(double vt);
};

#endif // FREESPACENAVIGATION_H
