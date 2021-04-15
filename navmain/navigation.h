/* 
* Top level Interface for Image-based Navigation Using Line Segments (Generic)
*
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
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




#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "linenav/linenavigation.h"
#include "linenav/dispnav.h"
#include <fstream>
using namespace std;


/**
 * @brief The navigation class
 */

class navigation
{


linenavigation ln;
cv::Mat K;

std::vector<double> lne;
std::vector<double> lnj;
std::vector<double> pte;
std::vector<double> ptj;

std::vector<double> lne2;
std::vector<double> lnj2;
std::vector<double> pte2;
std::vector<double> ptj2;

double rvL,rvL2,rvP,rvP2;
double rv, rvls, rvwa, rvm;

double nl,nl2;
double np, np2;
double npl, npl2;
double nlpn,nlpnn;
double nlp;
double fetno;
bool useobsavoid;

public:
    ofstream velfile, featfile, jacfile, errfile;
/**
     * @brief navigation
     * @param d pointer to display
     * @param Kc intrinsic matrix of RGB camera
     */

    navigation(dispNav *d,cv::Mat &Kc);


    /**
     * @brief initlocalisation perform global localiztion in the reference image list
     * @param Ic  current image
     * @param kf  reference image list
     */

    void initlocalisation(cv::Mat &Ic, kimRead &kf);


    /**
     * @brief initlocalisation perform global localiztion in the reference image list
     * @param Ic  current image
     * @param kf  reference image list
     * @param idx index of the refernce image in the list that matches best with current image
     * @param nlm  maximum number of matched lines
     */

    void initlocalisation(cv::Mat &Ic, kimRead &kf, int &idx, int &nlm);


    /**
     * @brief step perform succesive localiztion
     * @param Ic current image
     * @return 1 if reference images need to be switched 0 continue <0 = expections in linedetection/ matching
     */

    int step(cv::Mat& Ic);


    /**
     * @brief SwitchtoNewKeyImages  switch reference images
     * @param kf
     * @return 1 for end of topological navigation
     */

    int SwitchtoNewKeyImages(kimRead &kf);

    /**
     * @brief getRotVel calulate rotational velocity based on ibvs
     * @return rotational velocity
     */

    double getRotVel();


    /**
      * @brief getturninginkim
      * @return turnval
      */

     double getturninginkim();

     /**
      * @brief getinitdisp getinitdisplacemt get lateral displacement with refernce images
      * @return dispalcement
      */
     double getinitdisp();

     /**
      * @brief usecollisionavoidance set collisionavoidance flag
      * @param flag  0=> don't use , 1 =>use
      */

     void usecollisionavoidance(bool flag);


     /**
      * @brief usecollisionavoidance
      * @return 0 if collision avoidance is not used 1 if used
      */
     bool usecollisionavoidance();
};

#endif // NAVIGATION_H


