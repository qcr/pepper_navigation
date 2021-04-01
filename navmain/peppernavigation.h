/* Top level interface for Pepper Navigation
 * functions to interfcae
 *
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
 * Initial version @Inria Rennes Breatgne Atlantique
 * Modified for navigation of the Pepper Robot @ACRV QUT
 */


#ifndef PEPPERNAVIGATION_H
#define PEPPERNAVIGATION_H

#include "linenav/kimread.h"
#include "navigation.h"
#include "linenav/dispnav.h"
#include "depthnav/freespacenavigation.h"


/**
 * @brief The pepperNavigation class
 */

class pepperNavigation{

private:
    dispNav *d;
    navigation *nav;

    kimRead *kf;
    int flag;
    int count;
    float rv;
    int navflag;
    freespacenavigation *fsn;
    bool useobsavoid;


public:

    /**
     * @brief pepperNavigation
     * @param K instrinsic matrix of RGB camera
     */

    pepperNavigation(cv::Mat &K, dispNav *dn){

        d=dn;
        nav = new navigation(d,K);
        d->setDisptime(100);
        flag = 0;
        count = 0;
        rv = 0.0;
        useobsavoid = 0;
    }

    /**
     * @brief setfreespacenav enable free space navigation
     * @param Kd instrinsic matrix of depth camera
     */

    void setfreespacenav(cv::Mat &Kd){

        fsn = new freespacenavigation(Kd);
        useobsavoid = 1;
    }

    /**
    * @brief setPath
    * @param KF pointer of reference image list
    */

   void setPath(kimRead *KF){

        kf = KF;
    }


   /**
     * @brief localise perform global localization in the path defined by reference image list
     * @param I current RGB image
     */

    void localise(cv::Mat &I){

        nav->initlocalisation(I,*kf);
        rv  = 0.0;
    }

    /**
     * @brief navigate Perform Succesive Image-Based Navigation
     * @param cim Current Image
     * @return Rottaional Velocity
     */

    float navigate(cv::Mat &cim){

        flag = nav->step(cim);
        std::cout<<"Nav flag = "<<flag<<std::endl;
        if(flag==0) count = 0;

        else if(flag == 1) count++;
        else if(flag <0) { return NAN;}

        rv = float(nav->getRotVel());
        std::cout<<"Rot Vel = "<<rv<<std::endl;
        return rv;
    }


    /**
   * @brief continueNav check if reference images need to be switched
   * @param turnval turning value between refernce images
   * @return 0 => end of navigation 2=> switch reference images 1=> just continue
   */

  int continueNav(float &turnval){

      navflag = 1;
        if(count>2){
           if(nav->SwitchtoNewKeyImages(*kf)){
               navflag = 0;

              }
           else{
              navflag = 2;
              turnval = float(nav->getturninginkim());
           }
           count = 0;
        }
      return navflag;
    }


  /**
   * @brief closedisp
   */

  void closedisp(){

      d->closedisp();
  }


  /**
   * @brief getturninginkim
   * @return turnval
   */

  float getturninginkim(){

      return(float(nav->getturninginkim()));
  }

  /**
   * @brief getinitdisplacemt get lateral displacement with refernce images
   * @param cim curr images
   * @return dispalcement
   */
  float getinitdisplacemt(cv::Mat &cim){

      nav->step(cim);
      return(float(nav->getinitdisp()));
  }


  /**
   * @brief setofileid filenmaes for debuging
   * @param id
   */
  void setofileid(int id){

    stringstream tmp;

    tmp.str(std::string());
    nav->velfile.close();
    tmp<<"velfile_"<<std::to_string(id)<<".txt";
    nav->velfile.open(tmp.str().c_str());

    tmp<<flush;
    tmp.str(std::string());
    tmp<<"featfile_"<<std::to_string(id)<<".txt";
    nav->featfile.close();
    nav->featfile.open(tmp.str().c_str());

    tmp<<flush;
    tmp.str(std::string());
    tmp<<"errfile_"<<std::to_string(id)<<".txt";
    nav->errfile.close();
    nav->errfile.open(tmp.str().c_str());

    tmp<<flush;
    tmp.str(std::string());
    tmp<<"jacfile_"<<std::to_string(id)<<".txt";
    nav->jacfile.close();
    nav->jacfile.open(tmp.str().c_str());
    tmp<<flush;
  }


  /**
   * @brief setinitialpose set odometry pose at the start of navigation
   * @param pose ododmetry reading
   */

  void setinitialpose(std::vector<float> &pose){

      fsn->setinitialpose(pose);
  }


  /**
   * @brief setcurrentpose
   * @param pose current ododmetry reading
   */

  void setcurrentpose(std::vector<float> &pose){

      fsn->setcurrentpose(pose);
  }


  /**
   * @brief getvelfreespa current depth imagece free-space navigation
   * @param Id
   * @return rotational velocity to drive into free space
   */

  float getvelfreespace(cv::Mat &Id){

      return(float(fsn->getvelfreespace(Id)));
  }


  /**
   * @brief getH fusion of control ref. Paper
   * @return H
   */

  float getH(){

      return(float(fsn->getH()));
  }


  /**
   *   set base forward velocity
   */

  void setbasevel(double vt){

      fsn->setbasevel(vt);
  }

};
#endif // PEPPERNAVIGATION_H
#
