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

#ifndef KIMREAD_H
#define KIMREAD_H

#include <iostream>
//#include <cv.hpp>
//#include <QString>
//#include <QDir>
//#include <QStringList>
#include <opencv2/opencv.hpp>


class kimRead
{    std::string kimfold;
   //  QStringList keylineslist;
   //  QStringList keyimlist ;
     std::vector<cv::String> keylineslist;
     std::vector<cv::String> keyimlist;

     int index;
     int currindx;
     int nKIm;
     bool initlocl;
     int snode;
     int enode;
public:

      /**
      * @brief kimRead
      */

    kimRead();


     /**
      * @brief kimRead
      * @param kimf folder containing reference images
      */
    kimRead(char *kimf);


     /**
      * @brief kimRead Read reference images
      * @param kimf folder containing reference images
      */

    kimRead(std::string &kimf);


     /**
      * @brief kimRead Read reference images
      * @param kimf folder containing reference images
      * @param loc start index to insert
      */

    kimRead(std::string &kimf,int loc);


     /**
      * @brief kimRead  Read reference images
      * @param kimf folder containing reference images
      * @param sloc start index
      * @param eloc end index
      */

    kimRead(std::string &kimf,int sloc, int eloc);


     /**
       * @brief setnodes
       * @param sn start node
       * @param en end node
       */

     void setnodes(int sn,int en);


      /**
       * @brief setKimbasefold
       * @param fold folder location
       */

     void setKimbasefold(std::string &fold);


      /**
       * @brief resetIndex
       */

     void resetIndex();


      /**
      * @brief setKeyImageIndex
      * @param n index
      */

    void setKeyImageIndex(int n);


     /**
      * @brief getStartIndex
      * @return
      */

    int getStartIndex();


     /**
      * @brief setCurrIndex
      * @param indx current index
      */

    void setCurrIndex(int indx);


     /**
      * @brief getnextKeyImage
      * @param kim read next reference image name
      */

    void getnextKeyImage(std::string &kim);


     /**
      * @brief getnextKeyImage
      * @param kim next reference image name
      * @param kil file conatining line segment of the corresponding reference image
      */

    void getnextKeyImage(std::string &kim,std::string &kil);


     /**
      * @brief resetCurrentIndex
      */

    void resetCurrentIndex();


     /**
      * @brief currindex
      * @return current index
      */

    int currindex();


     /**
      * @brief kimsize
      * @return
      */

    int kimsize();

     /**
       * @brief isLast
       * @return true if the index is at the end of the list
       */

     int isLast();


      /**
        * @brief addKeyImages add reference images to the current list
        * @param kfptr  pointer of the reference image list
        * @param stindx start index
        * @param endindx end index
        */

      void addKeyImages(kimRead *kfptr, int stindx, int endindx);


       /**
        * @brief removeBKeyImages remove reference image from the back of the list upto
        * @param idx index
        */

      void removeBKeyImages(int idx);


       /**
       * @brief insetKeyImage
       * @param kim r
       * @param kil
       */

     void insetKeyImage(std::string &kim, std::string &kil);


      /**
       * @brief removeKeyImages
       * @param idx
       */

     void removeKeyImages(int idx);


      /**
        * @brief insertKeyImages
        * @param kfptr pointer to reference image list
        * @param desloc
        * @param indx
        */

      void insertKeyImages(kimRead *kfptr, int desloc, int indx);


       /**
          * @brief insertKeyImages
          * @param kfptr
          * @param indx
          */

        void insertKeyImages(kimRead *kfptr, int indx);


         /**
         * @brief insertKeyImages instert reference image list
         * @param kfptr pointer to the reference image list that is to be inserted
         */

       void insertKeyImages(kimRead *kfptr);


        /**
        * @brief getkimfold get referene image folder
        * @param kif path
        */

      void getkimfold(std::string &kif);


       /**
         * @brief showkim display key images
         */

       void showkim();

    ~kimRead();
};

#endif // KIMREAD_H
