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
 */


#include "dispnav.h"

#include <iostream>


dispNav::dispNav()
{
    waittime = 1;
  // cl.reserve(3);
    showdisp = true;
    savedispimg = true;
    featflag = true;


    count = 0;
   // displaymode = 0;
}

void dispNav::setDisptime(int delay){
      waittime = delay;
}



void dispNav::showfeat(bool flag){
    featflag = flag;
}

void dispNav::displayimage(bool flag){
    showdisp = flag;

}

void dispNav::saveimage(bool flag){
     savedispimg = flag;
}

void dispNav::setKeyImages(std::string pkim,std::string nkim,std::string nnkim)
{

    Ip = cv::imread(pkim);
    In = cv::imread(nkim);
    Inn = cv::imread(nnkim);

}


void dispNav::dispNavigation(cv::Mat &Ic,ScaleLines &cLines,ScaleLines &nLines,ScaleLines &nnLines,std::vector< std::vector< int> > &op)
{
   if(featflag)
      dispImages("Navigation",Ip,Ic,In,Inn,cLines,nLines,nnLines,op);
   else
      dispImages("Navigation",Ip,Ic,In,Inn);

}




void dispNav::dispNavigation(cv::Mat &Ic)
{
    dispImages("Navigation",Ip,Ic,In,Inn);
}


void dispNav::dispImages(std::string title, cv::Mat &IP,cv::Mat &IC,cv::Mat &IN,cv::Mat &INN) {

     int size,sz;
     int m,n;
     int w, h;

     h =2; w=2;

     size = 320;
     sz = 240;

     int wdh= 0;
     int offsets = 0;

     cv::Mat DispImage(cv::Size(size*w,int(sz*h+wdh*h)+offsets),CV_8UC3);
     cv::Mat temp;

     cv::cvtColor(IC,Ia,cv::COLOR_GRAY2BGR);

     INt = IN.clone();
     INNt = INN.clone();

     IPt = IP.clone();


     m= 0; n=wdh+offsets/2;
    //cv::resize(IPt,temp,cv::Size(size,sz));
     IPt.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m = size; wdh+offsets/2;
    // cv::resize(IC,temp,cv::Size(size,sz));
     Ia.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m = 0, n=sz+wdh*2+offsets/2;
     cv::resize(INt,temp,cv::Size(size,sz));
     INt.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m =size; n=sz+wdh*2+offsets/2;
     cv::resize(INNt,temp,cv::Size(size,sz));
     INNt.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     temp.release();

   if(showdisp){
      cv::namedWindow( title);
      cv::imshow( title,DispImage);
      cv::waitKey(waittime);
      }

   std::stringstream aa;
 if(count<10)
   aa<<"img000"<<count<<".png";
 else if (count<100)
      aa<<"img00"<<count<<".png";
 else if (count<1000)
         aa<<"img0"<<count<<".png";
 else
     aa<<"img"<<count<<".png";

 if(savedispimg)
     cv::imwrite(aa.str().c_str(),DispImage);
 count++;

}



void dispNav::dispImages(std::string title, cv::Mat &IP,cv::Mat &IC,cv::Mat &IN,cv::Mat &INN,ScaleLines &cLines,ScaleLines &nLines,ScaleLines &nnLines,std::vector< std::vector< int> > &op) {

     int size,sz;
     int m,n;
     int w, h;

     h =2; w=2;

     size = 320;
     sz = 240;

     int wdh= 0;
     int offsets = 0;

     cv::Mat DispImage(cv::Size(size*w,int(sz*h+wdh*h)+offsets),CV_8UC3);
     cv::Mat temp;

     cv::cvtColor(IC,Ia,cv::COLOR_GRAY2BGR);

     INt = IN.clone();
     INNt = INN.clone();

     IPt = IP.clone();


     int r,g,b,ln;
     int nmatch = op.size()/2;
     int lineID;

     // std::cout<<pcol;

      for(int pair=0; pair<nmatch;pair++){

         ln =op[pair][1];

         r = pcol[ln][0];
         g = pcol[ln][1];
         b = pcol[ln][2];

          lineID= op[pair][0];
          cv::line(Ia,cv::Point(int(cLines[lineID][0].startPointX),int(cLines[lineID][0].startPointY))
                      ,cv::Point(int(cLines[lineID][0].endPointX),int(cLines[lineID][0].endPointY)),
                       CV_RGB(r,g,b),2,cv::LINE_AA);

          lineID= op[pair][1];
          cv::line(INt,cv::Point(int(nLines[lineID][0].startPointX),int(nLines[lineID][0].startPointY)),
                    cv::Point(int(nLines[lineID][0].endPointX), int(nLines[lineID][0].endPointY)),
                     CV_RGB(r,g,b),2, cv::LINE_AA);

          lineID= op[pair][2];

          cv::line(INNt,cv::Point(int(nnLines[lineID][0].startPointX),int(nnLines[lineID][0].startPointY)),
                    cv::Point(int(nnLines[lineID][0].endPointX), int(nnLines[lineID][0].endPointY)),
                     CV_RGB(r,g,b),2, cv::LINE_AA);



      }




     m= 0; n=wdh+offsets/2;
     //cv::resize(IP,temp,cv::Size(size,sz));
     IPt.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m = size; wdh+offsets/2;
     //cv::resize(Ia,temp,cv::Size(size,sz));
     Ia.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m = 0, n=sz+wdh*2+offsets/2;
    // cv::resize(INt,temp,cv::Size(size,sz));
     INt.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m =size; n=sz+wdh*2+offsets/2;
     //cv::resize(INNt,temp,cv::Size(size,sz));
     INNt.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     //temp.release();
   if(showdisp){

       cv::imshow( title,DispImage);

      cv::waitKey(waittime);
   }
    
     std::stringstream aa;
   if(count<10)
     aa<<"img000"<<count<<".png";
   else if (count<100)
        aa<<"img00"<<count<<".png";
   else if (count<1000)
           aa<<"img0"<<count<<".png";
   else
       aa<<"img"<<count<<".png";

   if(savedispimg)
       cv::imwrite(aa.str().c_str(),DispImage);
   count++;
}



void dispNav::dispImages(std::string title, cv::Mat &I1,cv::Mat &I2) {

     int size,sz;
     int m,n;
     int w, h;

     h =1; w=2;

     size = 320;
     sz = 240;

     int wdh= 0;
     int offsets = 0;

     cv::Mat DispImage(cv::Size(size*w,int(sz*h+wdh*h)+offsets),CV_8U);
     cv::Mat temp;

     m= 0; n=wdh+offsets/2;
     cv::resize(I1,temp,cv::Size(size,sz));
     temp.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m = size; wdh+offsets/2;
     cv::resize(I2,temp,cv::Size(size,sz));
     temp.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     temp.release();

     if(showdisp){

    cv::namedWindow( title);
    cv::imshow( title,DispImage);

  cv::waitKey(waittime);
  }
}





void dispNav::dispImages(std::string title, cv::Mat &IC,ScaleLines linesInLeft,cv::Mat &IK,ScaleLines linesInRight,std::vector<unsigned int> matchResult) {


     int size,sz;
     int m,n;
     int w, h;

     h =1; w=2;

     size = 320;
     sz = 240;

     int wdh= 0;
     int offsets = 0;

     cv::cvtColor(IC,Ia,cv::COLOR_GRAY2BGR);


     cv::Mat DispImage(cv::Size(size*w,int(sz*h+wdh*h)+offsets),CV_8UC3);

    int r,g,b;
    int nmatch = matchResult.size()/2;
    int lineIDLeft;
    int lineIDRight;


     for(int pair=0; pair<nmatch;pair++){
         r = int(rand()%256);
         g = int(rand()%256);
         b = 255 - r;

         lineIDLeft = matchResult[2*pair];
         lineIDRight= matchResult[2*pair+1];

         cv::line(Ia,cv::Point(int(linesInLeft[lineIDLeft][0].startPointX),int(linesInLeft[lineIDLeft][0].startPointY))
                     ,cv::Point(int(linesInLeft[lineIDLeft][0].endPointX),int(linesInLeft[lineIDLeft][0].endPointY)),
                      CV_RGB(r,g,b),2,cv::LINE_AA);

         cv::line(IK,cv::Point(int(linesInRight[lineIDRight][0].startPointX),int(linesInRight[lineIDRight][0].startPointY)),
                   cv::Point(int(linesInRight[lineIDRight][0].endPointX), int(linesInRight[lineIDRight][0].endPointY)),
                    CV_RGB(r,g,b),2, cv::LINE_AA);
     }



     cv::Mat temp;

     m= 0; n=wdh+offsets/2;
     cv::resize(Ia,temp,cv::Size(size,sz));
     temp.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     m = size; n=wdh+offsets/2;
     cv::resize(IK,temp,cv::Size(size,sz));
     temp.copyTo(DispImage(cv::Rect(m, n, size, sz)));

     temp.release();

    cv::namedWindow( title );
    cv::imshow( title,DispImage);

  cv::waitKey(waittime);



}

void dispNav::setpseudocolour(int maxcolor){

     int bpc = 8;
     int tb = 3*bpc;
     int tc = maxcolor;

     int r = 0;
     int g = 0;
     int b = 0;
     pcol.clear();
     for(long int i=0;i<tc;i++)
     {
       r = 0;
       g = 0;
       b = 0;
       for(int k= 0; k<tb;){
           b = (b << 1) + ((i >> k++) & 1);
           g = (g << 1) + ((i >> k++) & 1);
           r = (r << 1) + ((i >> k++) & 1);
       }
       r = r << (8 - bpc);
       g = g << (8 - bpc);
       b = b << (8 - bpc);

       std::vector<int> cl;

       cl.push_back(r);
       cl.push_back(g);
       cl.push_back(b);
      /* cl[0] = r;
       cl[1] = g;
       cl[2] = 3;*/
      //pcol.push_back({r,g,b});
       pcol.push_back(cl);
     }

 }


void dispNav::closedisp()
{
  cv::destroyWindow("Navigation");
}
