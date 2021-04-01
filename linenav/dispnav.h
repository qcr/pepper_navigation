/* Code for display image based navigation */

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



#ifndef DISPNAV_H
#define DISPNAV_H


#include <opencv2/opencv.hpp>
#include "LineStructure.hh"

/**
 * @brief The dispNav class
 */

class dispNav
{
    cv::Mat Ip;
    cv::Mat In;
    cv::Mat Inn;
    cv::Mat Ia;

    cv::Mat INt;
    cv::Mat INNt;
    cv::Mat IPt;

    cv::Mat DIMG;



    cv::Mat ctop,cmid,cbut;

  // cv::VideoWriter outputVideo;
    int count;

    int waittime;
    bool showdisp;
    bool savedispimg;
    bool featflag;
        std::vector<std::vector<int> >pcol;

public:



    int size,sz;
    /**
     * @brief dispNav
     */
    dispNav();


    /**
     * @brief setDisptime delay foe dispaly window
     * @param delay time in ms
     */

    void setDisptime(int delay);


    /**
     * @brief displayimage show navigation images
     * @param flag 0 don't show => must be set o when run inside the Pepper robot
     */

    void displayimage(bool flag);


    /**
     * @brief saveimage show navigation images
     * @param flag 0 don't save
     */

    void saveimage(bool flag);


    /**
     * @brief showfeat draw features used for ibvs in displayed image if image is displayed
     * @param flag 0 don't draw
     */

    void showfeat(bool flag);


    /**
     * @brief closedisp
     */

    void closedisp();


    /**
     * @brief setKeyImages set current reference images
     * @param pkim previous reference image
     * @param nkim next reference image
     * @param nnkim second next reference image
     */

    void setKeyImages(std::string pkim,std::string nkim,std::string nnkim);


    /**
     * @brief dispNavigation
     * @param Ic Current Image
     */

    void dispNavigation(cv::Mat &Ic);


    /**
     * @brief dispNavigation
     * @param Ic  Current Image
     * @param cLines lines in current image
     * @param nLines lines in next reference image
     * @param nnLines lines in second next reference image
     * @param op matchedlines index
     */

    void dispNavigation(cv::Mat &Ic,ScaleLines &cLines,ScaleLines &nLines,ScaleLines &nnLines,std::vector< std::vector< int> > &op);


    /**
     * @brief dispImages display images in 2x2 grid window
     * @param title title of window
     * @param IP cv::Mat image
     * @param IC cv::Mat image
     * @param IN cv::Mat image
     * @param INN cv::Mat image
     */

    void dispImages(std::string title, cv::Mat &IP,cv::Mat &IC,cv::Mat &IN,cv::Mat &INN);


    /**
     * @brief dispImages dispaly two images side by side in window
     * @param title title of window
     * @param IC cv::Mat image
     * @param IK cv::Mat image
     */

    void dispImages(std::string title, cv::Mat &IC,cv::Mat &IK);


    /**
     * @brief dispImages dispaly two images side by side in window with matched line segemnts
     * @param title title of window
     * @param IC cv::Mat image
     * @param linesInLeft lines in IC
     * @param IK cv::Mat image
     * @param linesInRight lines in IK
     * @param matchResult matched line indices
     */

    void dispImages(std::string title, cv::Mat &IC,ScaleLines linesInLeft,cv::Mat &IK,ScaleLines linesInRight,std::vector<unsigned int> matchResult);


    /**
     * @brief setpseudocolour gebnerate color for lines segents dispaly
     * @param maxlines maximum number of lines
     */

    void setpseudocolour(int maxlines);


    /**
     * @brief dispImages display images in 2x2 grid window
     * @param title title title of window
     * @param IP cv::Mat image
     * @param IC cv::Mat image
     * @param IN cv::Mat image
     * @param INN cv::Mat image
     * @param cLines lines in IC
     * @param nLines lines in IN
     * @param nnLines lines in INN
     * @param op matcjed line indices among IC, IN and INN
     */

    void dispImages(std::string title, cv::Mat &IP,cv::Mat &IC,cv::Mat &IN,cv::Mat &INN,ScaleLines &cLines,ScaleLines &nLines,ScaleLines &nnLines,std::vector< std::vector< int> > &op);


};

#endif // DISPNAV_H
