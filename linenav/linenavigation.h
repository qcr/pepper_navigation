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
 * Initial version @Inria Rennes Breatgne Atlantique
 * Modified for navigation of the Pepper Robot @ACRV QUT
 */


#ifndef LINENAVIGATION_H
#define LINENAVIGATION_H

#include "LineStructure.hh"
#include "LineDescriptor.hh"
#include "linematch.h"
#include <opencv2/opencv.hpp>
#include "kimread.h"
#include "dispnav.h"


class linenavigation
{


    ScaleLines pLines,nLines,nnLines,cLines;
    linematch* lm;
    //IplImage *Ip;
    std::vector<unsigned int> mrPC,mrCN,mrCNN,mrNNN,mrPNN,matchResult;
    double nCP,nCN,nCNN;
    double rPCN,rPCNN;
    int nlc,nlp,nln;

    std::vector< std::vector< int> > opcn,ocnnn,onnnc;
    dispNav *disp;
    int count;
    std::string fn;
    cv::Mat K;
    double sx,sy,w,h;
    std::vector<int> linect;
     std::string kim,kil;

     double xcn;   double ycn;
     double xcnn;  double ycnn;
     double jcnx;  double jcny;
     double jcnnx; double jcnny;
     double vel;

     double xcn2;   double ycn2;
     double xcnn2;  double ycnn2;
     double jcn2x;  double jcn2y;
     double jcnn2x; double jcnn2y;
     double vel2;

     double jacx, ha, jacx2;

     /**
      * @brief calcLineParameters for ibvs
      */
     void calcLineParameters();
     void calcLineParameters2();
     double calturningkeyim();

     double difn2nn;

protected:
        std::string previmg,nextimg,nextnextimg;
public:

     /**
     * @brief linenavigation
     * @param d pointer to display
     * @param Kc intrinsic matrix of RGB camera
     */

    linenavigation(dispNav *d,cv::Mat &Kc);
    linenavigation();

    /**
     * @brief setDisplay
     * @param d pointer to display
     */

    void setDisplay(dispNav *d);

    /**
     * @brief setK
     * @param Kc intrinsic matrix of RGB camera
     */

    void setK(cv::Mat &Kc);

    /**
     * @brief initlocalisation perform global localiztion in the reference image list
     * @param Ic  current image
     * @param kf  reference image list
     */

    int initlocalisation(cv::Mat &Ic, kimRead &kf);


    /**
     * @brief initlocalisation perform global localiztion in the reference image list
     * @param Ic  current image
     * @param kf  reference image list
     * @param idx index of the refernce image in the list that matches best with current image
     * @param nlm  maximum number of matched lines
     */
    void initiallocalisationLines(cv::Mat &Ic, kimRead &kf, int &idx, int &nml);


    /**
     * @brief SetKeyImages set current reference images
     * @param pim  path of previous reference image
     * @param nim  path of next reference image
     * @param nnim path of second-next reference image
     */

    void SetKeyImages(std::string pim, std::string nim,std::string nnim);

    /**
     * @brief SwitchKeyImages Switch reference images
     * @param nnim path of second-next reference image
     */

    void SwitchKeyImages(std::string nnim);


    /**
     * @brief SwitchtoNewKeyImages switch reference images
     * @param kf
     * @return 1 for end of topological navigation
     */

    int SwitchtoNewKeyImages(kimRead &kf);



    /**
     * @brief step perform succesive localiztion
     * @param Ic current image
     * @return 1 if reference images need to be switched 0 continue <0 = expections in linedetection/ matching
     */

    int step(cv::Mat& Ic);


    /**
     * @brief initlocalisation perform global localiztion in the reference image list
     * @param Ic  current image
     * @param kf  reference image list
     */

    int initiallocalisationLines(cv::Mat &Ic, kimRead &kf);

    /**
     * @brief setInitialKeyImages set inital set of reference images
     * @param Ic curr image
     * @param kf reference image list
     * @param index index in the list
     * @return
     */

    int setInitialKeyImages(cv::Mat &Ic, kimRead &kf, int index);

    /**
     * @brief getRotVel calucluate rotational velocity 3 view version
     * @return  rotational velocity
     */

    double getRotVel();


    /**
     * @brief getRotVel2 calucluate rotational velocity 2 view version
     * @return rotational velocity
     */

    double getRotVel2();


    int linesno;
    int linesnoN;
    int linesnoNN;

    /**
     * @brief getRotVel calucluate rotational velocity 3 view version
     * @param err error
     * @param jac jacobian
     * @return rotational velocity
     */

    double getRotVel(std::vector<double>& err, std::vector<double>& jac);


    /**
     * @brief getRotVel2 calucluate rotational velocity 2 view version
     * @param err error
     * @param jac jacobian
     * @return rotational velocity
     */

    double getRotVel2(std::vector<double>& err, std::vector<double>& jac);


    /**
     * @brief getdiffN2NN
     * @return lateral displacement beween reference images
     */

    double getdiffN2NN();



};

#endif // LINENAVIGATION_H
