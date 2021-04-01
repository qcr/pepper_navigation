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


#include "kimread.h"
#include <iostream>

kimRead::kimRead()
{
    keyimlist.clear();
    keylineslist.clear();
    index = 0;
    currindx = 0;
    initlocl = false;
    kimfold = "multiple";
    snode = -1;
    enode = -1;

}
void kimRead::setKimbasefold(std::string &fold){
    if(kimfold=="multiple")
        kimfold = fold;
}

kimRead::kimRead(char *kimf)
{
   /* // read key images
        QString fold(kimf);
        QStringList filters;
        filters <<"*.txt";

        QStringList imgfilt;
        imgfilt <<"*.png"<<"*.pgm";

        QDir dir(fold);
        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);
        QDir imgs(dir);

        dir.setNameFilters(filters);
        keylineslist = dir.entryList();

        imgs.setNameFilters(imgfilt);
        keyimlist = imgs.entryList();

        kimfold.assign(kimf);
        kimfold += "/";

        */


    std::string imdir(kimf);
    imdir += "/";

    cv::glob(imdir + "*.png",keyimlist);
    cv::glob(imdir + "*.txt",keylineslist);

        index = 0;
        currindx = 0;
        initlocl = false;
        kimfold = imdir;
        snode = -1;
        enode = -1;
}

void kimRead::setnodes(int sn,int en){
    snode = sn;
    enode = en;
}

kimRead::kimRead(std::string &kimf)
{
   /* // read key images
        QString fold(kimf);
        QStringList filters;
        filters <<"*.txt";

        QStringList imgfilt;
        imgfilt <<"*.png"<<"*.pgm";

        QDir dir(fold);
        dir.setFilter(QDir::Files);
        dir.setSorting(QDir::Name);
        QDir imgs(dir);

        dir.setNameFilters(filters);
        keylineslist = dir.entryList();

        imgs.setNameFilters(imgfilt);
        keyimlist = imgs.entryList();

        kimfold.assign(kimf);
        kimfold += "/";

        */


    std::string imdir(kimf);
   // kimfold = imdir;

  if(imdir.back()!= '/')
    imdir += "/";

    cv::glob(imdir + "*.png",keyimlist);
    cv::glob(imdir + "*.txt",keylineslist);

        index = 0;
        currindx = 0;
        initlocl = false;

 kimfold = imdir;


}


 kimRead::kimRead(std::string &kimf,int loc){
     std::string imdir(kimf);
    // kimfold = imdir;

   if(imdir.back()!= '/')
     imdir += "/";

     cv::glob(imdir + "*.png",keyimlist);
     cv::glob(imdir + "*.txt",keylineslist);

  if(loc>1){
     keyimlist.erase(keyimlist.begin(),keyimlist.begin()+loc-1);
     keylineslist.erase(keylineslist.begin(),keylineslist.begin()+loc-1);
  }
         index = 0;
         currindx = 0;
         initlocl = false;

  kimfold = imdir;



 }

kimRead::kimRead(std::string &kimf,int sloc, int eloc){
    std::string imdir(kimf);
   // kimfold = imdir;

  if(imdir.back()!= '/')
    imdir += "/";

    cv::glob(imdir + "*.png",keyimlist);
    cv::glob(imdir + "*.txt",keylineslist);

    if(eloc==-1){eloc = keyimlist.size();}

    if(eloc<keyimlist.size()){
        keyimlist.erase(keyimlist.begin()+eloc,keyimlist.end());
        keylineslist.erase(keylineslist.begin()+eloc,keylineslist.end());
    }

 if(sloc>1){
    keyimlist.erase(keyimlist.begin(),keyimlist.begin()+sloc);
    keylineslist.erase(keylineslist.begin(),keylineslist.begin()+sloc);
 }
        index = 0;
        currindx = 0;
        initlocl = false;

 kimfold = imdir;

 }

void kimRead::resetIndex(){
    index = 0;
    currindx = 0;
    initlocl = false;
}

void kimRead::setKeyImageIndex(int n)
{
  if(currindx==0) {
   index = n;
   currindx = index;

  }
}

void kimRead::getnextKeyImage(std::string &kim)
{
   // kim.assign(kimfold+keyimlist.at(currindx).toStdString());
    kim.assign(keyimlist.at(currindx));
    currindx++;
}

void kimRead::getnextKeyImage(std::string &kim,std::string &kil)
{
    // kil.assign(kimfold+keylineslist.at(currindx).toStdString());
    // kim.assign(kimfold+keyimlist.at(currindx).toStdString());
     kil.assign(keylineslist.at(currindx));
     kim.assign(keyimlist.at(currindx));
     std::cout<<kil<<std::endl;

     currindx++;

}

int kimRead::getStartIndex()
{
    return index;
}

void kimRead::setCurrIndex(int indx){
   if(!initlocl)
    currindx = indx+index;
    initlocl = true;
}

int kimRead::currindex()
{
    return currindx;
}

void kimRead::resetCurrentIndex()
{
    currindx--;
}

int kimRead::kimsize()
{
  //  return(keyimlist.length()-index);
    return(keyimlist.size()-index);
}

int kimRead::isLast()
{
  if(currindx>=kimsize())
      return 1;
  else
      return 0;
}

 void kimRead::insetKeyImage(std::string &kim,std::string &kil)
 {
   keyimlist.push_back(kim);
   keylineslist.push_back(kil);
 }

 void kimRead::getkimfold(std::string &kif)
 {
     kif = kimfold;
 }

 void kimRead::removeKeyImages(int idx)
 {
     int len =  keyimlist.size()-1;
     for(int ct = idx; ct<len; ct++)
       {
        keyimlist.pop_back();
        keylineslist.pop_back();
     }
 }

 void kimRead::removeBKeyImages(int idx)
 {
     int len =  idx;
    while(len>=0)
       {
        keyimlist.pop_back();
        keylineslist.pop_back();
        len--;
     }
 }


 void kimRead::insertKeyImages(kimRead *kfptr, int desloc, int indx)
 {
     kfptr->resetIndex();
     kfptr->setKeyImageIndex(indx);
     std::string kim,kil;
   // std::cout<<indx<<"\t"<<desloc<<std::endl;
   //  int loctr = abs(desloc-indx);
    // std::cout<<"jjjjj "<<loctr<<std::endl;

     for(int loctr =indx;loctr<=desloc;loctr++){
         kfptr->getnextKeyImage(kim,kil);
         insetKeyImage(kim,kil);
        // loctr--;
     }

 }

 void kimRead::addKeyImages(kimRead *kfptr, int stindx, int endindx)
 {
     if(stindx<0) stindx = 0;
  
     if(endindx<0) endindx = kfptr->kimsize()-1;
     std::cout<<stindx<<"\t"<<endindx<<std::endl;
     kfptr->resetIndex();
     kfptr->setKeyImageIndex(stindx);
     std::string kim,kil;
   // std::cout<<indx<<"\t"<<desloc<<std::endl;
   //  int loctr = abs(desloc-indx);
    // std::cout<<"jjjjj "<<loctr<<std::endl;

     for(int loctr = stindx; loctr<=endindx;loctr++){
         kfptr->getnextKeyImage(kim,kil);
         insetKeyImage(kim,kil);
        // loctr--;
     }

 }

 void kimRead::insertKeyImages(kimRead *kfptr, int indx)
 {
     kfptr->resetIndex();
     kfptr->setKeyImageIndex(0);
     std::string kim,kil;
    // std::cout<<indx<<"\t"<<desloc<<std::endl;
   //  int len = kfptr->kimsize();
   //for (int loctr = indx;loctr<len;loctr++){
   /*  while(!kfptr->isLast()){
         kfptr->getnextKeyImage(kim,kil);
         insetKeyImage(kim,kil);
     }*/
     int idxx = indx;
       int loctr = kfptr->kimsize();
       while(loctr>0){
           kfptr->getnextKeyImage(kim,kil);
           if(idxx<1)
              insetKeyImage(kim,kil);
           idxx--;
           loctr--;
       }

 }


 void kimRead::insertKeyImages(kimRead *kfptr)
 {
     kfptr->resetIndex();
     kfptr->setKeyImageIndex(0);
     std::string kim,kil;
    // std::cout<<indx<<"\t"<<desloc<<std::endl;
     int loctr = kfptr->kimsize();
     while(loctr>0){
         kfptr->getnextKeyImage(kim,kil);
         insetKeyImage(kim,kil);
         loctr--;
     }

 }


 void kimRead::showkim(){
     std::cout<<"\n Total number of Key Images = "<<keyimlist.size()<<std::endl;
     for(int i = 0; i<keyimlist.size();i++)
     {
         std::cout<<keyimlist.at(i)<<std::endl;
         std::cout<<keylineslist.at(i)<<std::endl;
     }
 }

kimRead::~kimRead()
{

}
