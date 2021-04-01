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


#include "linenavigation.h"
#include "linematch.h"
#include <fstream>

linenavigation::linenavigation()
{
    lm = new linematch;

    count = 0;
    sx = 1.0;
    sy = 1.0;
    w = 0.0;
    h = 0.0;
    difn2nn = 0;

}

void linenavigation::setDisplay(dispNav *d){
      disp = d;
}

void linenavigation::setK(cv::Mat &Kc){
    K = Kc.clone();
    sx = 1/K.at<double>(0,0);
    sy =1/ K.at<double>(1,1);
    w = K.at<double>(0,2);
    h = K.at<double>(1,2);
}

linenavigation::linenavigation(dispNav *d,cv::Mat &Kc)
{
    lm = new linematch;
    disp = d;
    count = 0;
    K = Kc.clone();
    sx = 1/K.at<double>(0,0);
    sy = 1/K.at<double>(1,1);
    w = K.at<double>(0,2);
    h = K.at<double>(1,2);
    difn2nn = 0;

}


void linenavigation::SetKeyImages(std::string pim, std::string nim,std::string nnim)
{
    pLines.clear();
    nLines.clear();
    nnLines.clear();
    lm->readlinedesc(pim,pLines);
    lm->readlinedesc(nim,nLines);
    lm->readlinedesc(nnim,nnLines);

}

void linenavigation::SwitchKeyImages(std::string nnim)
{
  pLines.clear();
  pLines = nLines;
  nLines.clear();
  nLines = nnLines;
  lm->readlinedesc(nnim,nnLines);
}

int linenavigation::SwitchtoNewKeyImages(kimRead &kf)
{
      if(kf.isLast())
          return 1;
      else{
         std::cout<<"\n Key Image Switching lINES..."<<std::endl;
        std::string nnim;
        previmg.assign(nextimg);
        nextimg.assign(nextnextimg);

      kf.getnextKeyImage(nextnextimg,nnim);
      disp->setKeyImages(previmg,nextimg,nextnextimg);
      SwitchKeyImages(nnim);
     // std::cout<<"\n"<<nLines.size()<<"\t"<<nnLines.size()<<std::endl;
      try{
      lm->matchlines(pLines,nnLines,mrPNN);

      difn2nn=calturningkeyim();
      }
      catch(int exception)
      {
        difn2nn = -1;
      }

      lm->matchlines(nLines,nnLines,mrNNN);
      disp->setpseudocolour(nLines.size());
      std::cout<<"\n"<<nextnextimg<<std::endl;

      return 0;
      }

}

double linenavigation::getdiffN2NN(){
    return(difn2nn);
}

int linenavigation::step(cv::Mat& Ic)
{

    linesnoN = 0;
    linesnoNN = 0;

    lm->getlinedesc(Ic,cLines);
    std::cout<<"nCurrLines = "<<cLines.size()<<std::endl;
    if(cLines.size()<10)
        return -2;

 try{lm->matchlines(cLines,pLines,mrPC);}
 catch(int exception){ std::cout<<"excpPC"<<std::endl; return -1;}

    nCP = mrPC.size()/2;

 try{lm->matchlines(cLines,nLines,mrCN);}
 catch(int exception){ std::cout<<"excpCN"<<std::endl; return -1;}

    nCN = mrCN.size()/2;

    if(!nCP || !nCN) return -1;



  try{lm->matchlines(cLines,nnLines,mrCNN);}
  catch(int exception){std::cout<<"excpCNN"<<std::endl; return -1;}

    nCNN = mrCNN.size()/2;


  lm->findCommonIndex(mrCN,mrPC,opcn);
  lm->findCommonIndex(mrCN,mrCNN,ocnnn);
  lm->findCommonIndex2(mrCN,mrNNN,onnnc);

  rPCN = nCN/nCP;
  rPCNN = nCNN/nCP;
 // std::cout<<"********************"<<onnnc.size()<<std::endl;
 disp->dispNavigation(Ic,cLines,nLines,nnLines,onnnc);

  nlc = ocnnn.size();
  nlp = opcn.size();
  nln = onnnc.size();

  linesnoN = nCN;
  linesnoNN = nCNN;

  std::cout<<"nLines = "<<nCP<<"\t"<<nCN<<"\t"<<nCNN<<std::endl;

   if((nlc > nlp) ||( ((rPCN<rPCNN) && (rPCNN >1)) || (nCP <8 && nCN <8 && nCNN <8)) )
       return 1;
   else
       return 0;
}

int linenavigation::initlocalisation(cv::Mat &Ic, kimRead &kf)
{

    lm->getlinedesc(Ic,cLines);

    int nkims = kf.kimsize();

    std::cout<<nkims<<std::endl;
    std::string title("Iloc");
    linect.clear();

    cv::Mat Ik;

    for(int i = kf.getStartIndex(); i<nkims; i++)
    {
        kf.getnextKeyImage(kim,kil);
                std::cout<<kim<<std::endl;
        Ik= cv::imread(kim);
        //std::cout<<kim<<"\n";
        lm->readlinedesc(kil,nLines);
        //cv::Mat Ik = cv::imread(kim);

        matchResult.clear();

    try{
      //   std::cout<<linesInLeft.size()<<"\t"<<linesInRight.size()<<std::endl;

        lm->matchlines(cLines,nLines,matchResult);
//        disp->dispImages(title,Ic,cLines,Ik,nLines,matchResult);
    }
    catch(int exception)
    {
       std::cout<<"exexppppppppppppp";
            return 0;
    }

        try{
          //   std::cout<<linesInLeft.size()<<"\t"<<linesInRight.size()<<std::endl;

            lm->matchlines(cLines,nLines,matchResult);
            disp->dispImages("Iloc",Ic,cLines,Ik,nLines,matchResult);
        }
        catch(int exception)
        {
           std::cout<<"exexppppppppppppp";
                return 0;
        }


    linect.push_back(matchResult.size()*0.5);
    std::cout<<i<<"\t matchedlines="<<matchResult.size()/2<<std::endl;
    // matchlist.push_back(matchResult);

    //displaymatchedline(leftImage,linesInLeft,keyim.str(),linesInRight,matchResult);


 }
  cv::destroyWindow("Iloc");

 std::vector<int>::iterator result = std::max_element(linect.begin(),linect.end());

 int maxvalue = *result;
 int ind = std::distance(linect.begin(), result);
 std::cout<<std::endl<<"Maximum matches = "<<maxvalue<<"\t Index = "<<ind<<std::endl;

 int flagx = -1;


 if(ind == 0)
     flagx = 0;
 else if ((ind == linect.size()-1) || (linect[ind-1] > linect[ind+1]))
     flagx = 1;
 else if (linect[ind-1] <=linect[ind+1])
     flagx = 0;

 if(flagx==1) kf.setCurrIndex(ind-1);
 else if(flagx==0) kf.setCurrIndex(ind);

 std::string pim;
 std::string nim;
 std::string nnim;

 kf.getnextKeyImage(previmg,pim);
 kf.getnextKeyImage(nextimg,nim);
 kf.getnextKeyImage(nextnextimg,nnim);


 disp->setKeyImages(previmg,nextimg,nextnextimg);
 SetKeyImages(pim,nim,nnim);
 disp->setpseudocolour(nLines.size());

  // std::cout<<"\n"<<nLines.size()<<"\t"<<nnLines.size()<<std::endl;
   mrNNN.clear();
   try{
  lm->matchlines(nLines,nnLines,mrNNN);
   }

   catch(int exception)
   {
      std::cout<<"exexppppppppppppp";
           return 0;
   }
 //  cv::imwrite("img_00000.png",Ic);
   count++;


   return 1;
}


int linenavigation::initiallocalisationLines(cv::Mat &Ic, kimRead &kf)
{

    lm->getlinedesc(Ic,cLines);

    int nkims = kf.kimsize();

    std::cout<<nkims<<std::endl;

     linect.clear();
     std::string title("Iloc");
    cv::Mat Ik;

    for(int i = kf.getStartIndex(); i<nkims; i++)
    {
        kf.getnextKeyImage(kim,kil);
        Ik= cv::imread(kim);
        //std::cout<<kim<<"\n";
        lm->readlinedesc(kil,nLines);
        //cv::Mat Ik = cv::imread(kim);

        matchResult.clear();

    try{
         std::cout<<cLines.size()<<"\t"<<nLines.size()<<std::endl;

        lm->matchlines(cLines,nLines,matchResult);
     //   disp->dispImages(title,Ic,cLines,Ik,nLines,matchResult);
    }
    catch(int exception)
    {
       std::cout<<"exexppppppppppppp";
            //return 0;
    }

    linect.push_back(matchResult.size()*0.5);
    std::cout<<"matchedlines="<<matchResult.size()/2<<std::endl;
    // matchlist.push_back(matchResult);

    //displaymatchedline(leftImage,linesInLeft,keyim.str(),linesInRight,matchResult);


 }
  //cv::destroyWindow("Iloc");
  cLines.clear();
  nLines.clear();

 std::vector<int>::iterator result = std::max_element(linect.begin(),linect.end());

 int maxvalue = *result;
 int ind = std::distance(linect.begin(), result);
 std::cout<<std::endl<<"Maximum matches = "<<maxvalue<<"\t Index = "<<ind<<std::endl;

 int flagx = -1;


 if(ind == 0)
     flagx = 0;
 else if ((ind == linect.size()-1) || (linect[ind-1] > linect[ind+1]))
     flagx = 1;
 else if (linect[ind-1] <=linect[ind+1])
     flagx = 0;

 if(flagx==1) kf.setCurrIndex(ind-1);
 else if(flagx==0) kf.setCurrIndex(ind);

  return kf.currindex();

}

void linenavigation::initiallocalisationLines(cv::Mat &Ic, kimRead &kf, int &idx, int &nlm)
{

    cv::Mat Ik;
    if(Ic.channels() == 1)
       lm->getlinedesc(Ic,cLines);
    else {
        cv::cvtColor(Ic,Ik,cv::COLOR_BGR2GRAY);
        lm->getlinedesc(Ik,cLines);

    }
    idx =-1;
    nlm = -1;
    int nkims = kf.kimsize();

   // std::cout<<nkims<<std::endl;

linect.clear();
   // std::string title("Iloc");


    for(int i = kf.getStartIndex(); i<nkims; i++)
    {
        kf.getnextKeyImage(kim,kil);
        //Ik= cv::imread(kim);
        //std::cout<<kim<<"\n";
        lm->readlinedesc(kil,nLines);
        //cv::Mat Ik = cv::imread(kim);

        matchResult.clear();

    try{
       //  std::cout<<cLines.size()<<"\t"<<nLines.size()<<std::endl;
       lm->matchlines(cLines,nLines,matchResult);
      // lm->matchlinesbinary(cLines,nLines,matchResult);
      //  disp->dispImages(title,Ic,cLines,Ik,nLines,matchResult);
    }
    catch(int exception)
    {
      //std::cout<<"exexppppppppppppp";
            //return 0;
    }

    linect.push_back(matchResult.size()*0.5);
    std::cout<<"matchedlines="<<matchResult.size()/2<<std::endl;
    // matchlist.push_back(matchResult);

    //displaymatchedline(leftImage,linesInLeft,keyim.str(),linesInRight,matchResult);


 }
 // cv::destroyWindow("Iloc");
  cLines.clear();
  nLines.clear();

 std::vector<int>::iterator result = std::max_element(linect.begin(),linect.end());

 int maxvalue = *result;
 int ind = std::distance(linect.begin(), result);
//std::cout<<std::endl<<"Maximum matches = "<<maxvalue<<"\t Index = "<<ind<<std::endl;

 int flagx = -1;
/*

 if(ind == 0)
     flagx = 0;
 else if ((ind == linect.size()-1) || (linect[ind-1] > linect[ind+1]))
     flagx = 1;
 else if (linect[ind-1] <=linect[ind+1])
     flagx = 0;

 if(flagx==1) kf.setCurrIndex(ind-1);
 else if(flagx==0) kf.setCurrIndex(ind);
*/
idx = ind;
nlm = maxvalue;

}



int linenavigation::setInitialKeyImages(cv::Mat &Ic, kimRead &kf, int index){

 std::string pim;
 std::string nim;
 std::string nnim;

 if(index>=0)
    kf.setCurrIndex(index);

 kf.getnextKeyImage(previmg,pim);
 kf.getnextKeyImage(nextimg,nim);
 kf.getnextKeyImage(nextnextimg,nnim);


 disp->setKeyImages(previmg,nextimg,nextnextimg);
 SetKeyImages(pim,nim,nnim);
 disp->setpseudocolour(nLines.size());

  // std::cout<<"\n"<<nLines.size()<<"\t"<<nnLines.size()<<std::endl;
   mrPNN.clear();
   try{ //
  lm->matchlines(pLines,nnLines,mrPNN);
  difn2nn=calturningkeyim();
 }

 catch(int exception)
 {
   difn2nn = -1;
 }

  // }
  mrNNN.clear();
  try{
   lm->matchlines(nLines,nnLines,mrNNN);
}
   catch(int exception)
   {
      std::cout<<"exexppppppppppppp";
           return 0;
   }
   cv::imwrite("img_00000.png",Ic);
   count++;


   return 1;
}



void linenavigation::calcLineParameters()
{
     xcn = 0.0;
     xcnn = 0.0;
     jcnx = 0.0;
     jcnnx = 0.0;
     jcny = 0.0;
     jcnny = 0.0;
     ycn = 0.0;
     ycnn = 0.0;
     vel = 0.0;
     ha = 0.0;
     linesno = 0;

     int indx;
     double cr,ct,nr,nt,nnr,nnt;
     double x1,x2,y1,y2,xd,yd,d;
     double sgn;



     if(nlc>0){

         for(int i =0;i<nlc;i++){

                indx = ocnnn[i][0];
                x1 =  sx*(cLines[indx][0].startPointX-w);
                x2 =  sx*(cLines[indx][0].endPointX-w);
                y1 =  sy*(cLines[indx][0].startPointY-h);
                y2 =  sy*(cLines[indx][0].endPointY-h);

                yd = y2 - y1;
                xd = x1 - x2;

                d =  sqrt(yd*yd+xd*xd);

                cr = (x1*yd+y1*xd)/d;
                sgn = (cr<0)?-1:1;
                ct = atan2(xd*sgn,yd*sgn);
                cr*=sgn;


                indx = ocnnn[i][1];
                x1 =  sx*(nLines[indx][0].startPointX-w);
                x2 =  sx*(nLines[indx][0].endPointX-w);
                y1 =  sy*(nLines[indx][0].startPointY-h);
                y2 =  sy*(nLines[indx][0].endPointY-h);


                yd = y2 - y1;
                xd = x1 - x2;
                d =  sqrt(yd*yd+xd*xd);

                nr = (x1*yd+y1*xd)/d;
                sgn = (nr<0)?-1:1;
                nt = atan2(xd*sgn,yd*sgn);
                nr*=sgn;


                indx = ocnnn[i][2];
                x1 =  sx*(nnLines[indx][0].startPointX-w);
                x2 =  sx*(nnLines[indx][0].endPointX-w);
                y1 =  sy*(nnLines[indx][0].startPointY-h);
                y2 =  sy*(nnLines[indx][0].endPointY-h);


                yd = y2 - y1;
                xd = x1 - x2;
                d =  sqrt(yd*yd+xd*xd);

                nnr = (x1*yd+y1*xd)/d;
                sgn = (nnr<0)?-1:1;
                nnt = atan2(xd*sgn,yd*sgn);
                nnr*=sgn;



                xcn += nr*cos(nt)-cr*cos(ct);
                ycn += nr*sin(nt)-cr*sin(ct);

                jcnx += -cr*cr*sin(ct)*sin(ct)+cr*cr*cos(ct)*cos(ct)+cos(ct)*cos(ct);
                jcny +=  (1+2*cr*cr)*sin(ct)*cos(ct);

                xcnn += nnr*cos(nnt)-cr*cos(ct);
                ycnn += nnr*sin(nnt)-cr*sin(ct);
         }

         xcn/= nlc;
         ycn/= nlc;
         jcnx/= nlc;
         jcny/= nlc;
         xcnn/= nlc;
         ycnn/= nlc;



    }
     else{
         jcnx = 1.0;
         jcny = 1.0;

     }


     jcnnx = jcnx;
     jcnny = jcny;

}

double linenavigation::getRotVel()
{

    calcLineParameters();

    linesno = ocnnn.size();


     ha = 0.67*xcn+0.33*xcnn;

     jacx = jcnx;

     if(fabs(jacx)<0.01)
        jacx= jacx/fabs(jacx)*0.01;


      ha /= jacx;


      if(fabs(ha)>0.3)
         ha = ha/fabs(ha) *0.3;


      vel = 0.8*ha;

     return vel;

}


double linenavigation::getRotVel2()
{
    calcLineParameters2();

    linesno = int(0.67*nCN+0.33*nCNN);

    jacx = jcn2x;
    jacx2 = jcnn2x;

     if(fabs(jacx)<0.01)
         jacx = jacx/fabs(jacx)*0.01;

     if(fabs(jacx)<0.01)
              jacx= jacx/fabs(jacx)*0.01;


     ha = (0.67*xcn2/jacx + 0.33*xcnn2/jacx2);


    if(fabs(ha)>0.3)
       ha = ha/fabs(ha) *0.3;


      vel = 0.8*ha;

     return vel;

}


void linenavigation::calcLineParameters2()
{
    xcn2 = 0.0;
    xcnn2 = 0.0;
    jcn2x = 0.0;
    jcnn2x = 0.0;
    ycn2 = 0.0;
    ycnn2 = 0.0;
    jcn2y = 0.0;
    jcnn2y = 0.0;
    vel = 0.0;

     int indx;
     double cr,ct,nr,nt,nnr,nnt;
     double x1,x2,y1,y2,xd,yd,d;
     double sgn;

     if(nCN>0){
             for(int i =0;i<nCN;i++){

                indx = mrCN[2*i];
                x1 =  sx*(cLines[indx][0].startPointX-w);
                x2 =  sx*(cLines[indx][0].endPointX-w);
                y1 =  sy*(cLines[indx][0].startPointY-h);
                y2 =  sy*(cLines[indx][0].endPointY-h);

                yd = y2 - y1;
                xd = x1 - x2;

                d =  sqrt(yd*yd+xd*xd);

                cr = (x1*yd+y1*xd)/d;
                sgn = (cr<0)?-1:1;
                ct = atan2(xd*sgn,yd*sgn);
                cr*=sgn;


                indx = mrCN[2*i+1];
                x1 =  sx*(nLines[indx][0].startPointX-w);
                x2 =  sx*(nLines[indx][0].endPointX-w);
                y1 =  sy*(nLines[indx][0].startPointY-h);
                y2 =  sy*(nLines[indx][0].endPointY-h);


                yd = y2 - y1;
                xd = x1 - x2;
                d =  sqrt(yd*yd+xd*xd);

                nr = (x1*yd+y1*xd)/d;
                sgn = (nr<0)?-1:1;
                nt = atan2(xd*sgn,yd*sgn);
                nr*=sgn;

                xcn2 += nr*cos(nt)-cr*cos(ct);
                ycn2 += nr*sin(nt)-cr*sin(ct);

                jcn2x += -cr*cr*sin(ct)*sin(ct)+cr*cr*cos(ct)*cos(ct)+cos(ct)*cos(ct);
                jcn2y +=  (1+2*cr*cr)*sin(ct)*cos(ct);
             }

         xcn2/= nCN;
         jcn2x/= nCN;
         ycn2/= nCN;
         jcn2y/= nCN;
     }
     else{
         jcn2x = 1.0;
         jcn2y = 1.0;
     }


     if(nCNN>0){
             for(int i =0;i<nCNN;i++){


                 indx = mrCNN[2*i];
                 x1 =  sx*(cLines[indx][0].startPointX-w);
                 x2 =  sx*(cLines[indx][0].endPointX-w);
                 y1 =  sy*(cLines[indx][0].startPointY-h);
                 y2 =  sy*(cLines[indx][0].endPointY-h);

                 yd = y2 - y1;
                 xd = x1 - x2;

                 d =  sqrt(yd*yd+xd*xd);

                 cr = (x1*yd+y1*xd)/d;
                 sgn = (cr<0)?-1:1;
                 ct = atan2(xd*sgn,yd*sgn);
                 cr*=sgn;


                    indx = mrCNN[2*i+1];
                    x1 =  sx*(nnLines[indx][0].startPointX-w);
                    x2 =  sx*(nnLines[indx][0].endPointX-w);
                    y1 =  sy*(nnLines[indx][0].startPointY-h);
                    y2 =  sy*(nnLines[indx][0].endPointY-h);


                    yd = y2 - y1;
                    xd = x1 - x2;
                    d =  sqrt(yd*yd+xd*xd);

                    nnr = (x1*yd+y1*xd)/d;
                    sgn = (nnr<0)?-1:1;
                    nnt = atan2(xd*sgn,yd*sgn);
                    nnr*=sgn;


                jcnn2x += -cr*cr*sin(ct)*sin(ct)+cr*cr*cos(ct)*cos(ct)+cos(ct)*cos(ct);
                jcnn2y +=  (1+2*cr*cr)*sin(ct)*cos(ct);

                xcnn2 += nnr*cos(nnt)-cr*cos(ct);
                ycnn2 += nnr*sin(nnt)-cr*sin(ct);
             }



         xcnn2/= nCNN;
         jcnn2x/= nCNN;
         ycnn2/= nCNN;
         jcnn2y/= nCNN;
     }
     else{
         jcnn2x = 1.0;
         jcnn2y = 1.0;
     }
}




double linenavigation::getRotVel(std::vector<double>& err, std::vector<double>& jac)
{
    calcLineParameters();

    linesno = ocnnn.size();


     err[0]=xcn;
     err[1]=xcnn;
     err[2]=ycn;
     err[3]=ycnn;

     jac[0]=jcnx;
     jac[1]=jcnnx;
     jac[2]=jcny;
     jac[3]=jcnny;

     jacx = jcnx;


     if(fabs(jacx)<0.05)
        jacx= 1;

    if(int(nCN+nCNN)>0)
       ha = (nCN*xcn+ nCNN*xcnn)/(nCN+nCNN);
    else
        ha = 0.0;



     jacx2 = jacx;

      ha /= jacx;

      vel = ha;

     return vel;

}


double linenavigation::getRotVel2(std::vector<double>& err, std::vector<double>& jac)
{

    calcLineParameters2();



    err[0]=xcn2;
    err[1]=xcnn2;
    err[2]=ycn2;
    err[3]=ycnn2;

    jac[0]=jcn2x;
    jac[1]=jcnn2x;
    jac[2]=jcn2y;
    jac[3]=jcnn2y;


    jacx = jcn2x;
    jacx2 = jcnn2x;

    if(fabs(jacx)<0.01)
        jacx = 1;

     if(fabs(jacx)<0.01)
              jacx= 1;

     linesno = int((nCN*nCN+nCNN*nCNN));
     ha = (nCN*xcn2/jacx + nCNN*xcnn2/jacx2);
     if(linesno>0){
        linesno = int(linesno/(nCN+nCNN));
        ha /= (nCN+nCNN);
     }


    //if(fabs(ha)>0.3)
    //   ha = ha/fabs(ha) *0.3;


      vel = ha;

     return vel;

}


double linenavigation::calturningkeyim()
{


     int indx,nNNN;
     double cr,ct,nr,nt,nnr,nnt;
     double x1,x2,y1,y2,xd,yd,d;
     double sgn;
     double xnn,ynn,jnnx,jnny;
     xnn = 0.0;
     ynn = 0.0;
     jnnx = 0.0;
     jnny = 0.0;
     nNNN = mrPNN.size()/2;

             for(int i =0;i<nNNN;i++){

                indx = mrPNN[2*i];
                x1 =  sx*(pLines[indx][0].startPointX-w);
                x2 =  sx*(pLines[indx][0].endPointX-w);
                y1 =  sy*(pLines[indx][0].startPointY-h);
                y2 =  sy*(pLines[indx][0].endPointY-h);

                yd = y2 - y1;
                xd = x1 - x2;

                d =  sqrt(yd*yd+xd*xd);

                cr = (x1*yd+y1*xd)/d;
                sgn = (cr<0)?-1:1;
                ct = atan2(xd*sgn,yd*sgn);
                cr*=sgn;


                indx = mrPNN[2*i+1];
                x1 =  sx*(nnLines[indx][0].startPointX-w);
                x2 =  sx*(nnLines[indx][0].endPointX-w);
                y1 =  sy*(nnLines[indx][0].startPointY-h);
                y2 =  sy*(nnLines[indx][0].endPointY-h);


                yd = y2 - y1;
                xd = x1 - x2;
                d =  sqrt(yd*yd+xd*xd);

                nr = (x1*yd+y1*xd)/d;
                sgn = (nr<0)?-1:1;
                nt = atan2(xd*sgn,yd*sgn);
                nr*=sgn;

                xnn += nr*cos(nt)-cr*cos(ct);
                ynn += nr*sin(nt)-cr*sin(ct);

                jnnx += -cr*cr*sin(ct)*sin(ct)+cr*cr*cos(ct)*cos(ct)+cos(ct)*cos(ct);
                jnny +=  (1+2*cr*cr)*sin(ct)*cos(ct);



             }





         xnn/= nNNN;
         ynn/= nNNN;
         return(xnn);

}

