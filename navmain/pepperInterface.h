/* Top level virtual fucntion for interface with Pepper robot or just image sequences
 * See pepperRobot.h for interface with Peepper for online navigation
 * See pepperOffline.h for just image-based localization in opffline mode
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
 */


#ifndef PEPPERINTERFACE_H
#define PEPPERINTERFACE_H

#include <opencv2/opencv.hpp>

class pepperInterface
{
public:
      pepperInterface(const std::string &opt_ip);
      virtual void openCamera(int id) = 0;
      virtual void startBaseMotionController() = 0;
      virtual void initPosture() = 0;
      virtual void startPepper(int id) = 0;
      virtual void getCurrImage(cv::Mat &I) = 0;
      virtual void openDepthCamera() = 0;
      virtual void getCurrDepthImage(cv::Mat &I) = 0;
      virtual void setBaseVelocities(float vr, float wr)= 0;
      virtual void setBaseVelocities(float vr, float vs,float wr)= 0;
      virtual std::vector<float> getOdometryReading()= 0;
      virtual void rotate180()= 0;
      virtual void wait(long t)= 0;
      virtual void adjusthead()= 0;
      virtual cv::Mat getK() = 0;
      virtual cv::Mat getKd() = 0;
      virtual int getid() = 0;
      virtual int getmode() = 0;
      virtual ~pepperInterface() = 0;
};

#endif // PEPPERINTERFACE_H
