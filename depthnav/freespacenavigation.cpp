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


#include "freespacenavigation.h"

freespacenavigation::freespacenavigation(cv::Mat &kd){
    dls = new depthimagescanner::DepthImageScanner(kd);
    dls->setdepthasUC();
    dls->set_scan_height(75);
    od.reserve(3);
    od[0]=0; od[1]=0; od[2]=0;
     flno = 0;

      wr.reserve(4);
      wrd.reserve(4);
      wrx.reserve(4);
      dis[0]=1.0; dis[1]=2.0; dis[2]=3.25; dis[3]=0.9;
      fs1 = 0.0;
      fs2 = 0.0;
      fs3 = 0.0;
      fsx = 0.0;
      flagx = 0;
}


freespacenavigation::freespacenavigation(cv::Mat &kd, int scan_height)
{
    dls = new depthimagescanner::DepthImageScanner(kd);
    dls->setdepthasUC();
    dls->set_scan_height(scan_height);
    od.reserve(3);
    od[0]=0; od[1]=0; od[2]=0;
     flno = 0;

      wr.reserve(4);
      wrd.reserve(4);
      wrx.reserve(4);
      dis[0]=1.0; dis[1]=2.0; dis[2]=3.25; dis[3]=0.9;
      fs1 = 0.0;
      fs2 = 0.0;
      fs3 = 0.0;
      fsx = 0.0;
      flagx = 0;

}


void freespacenavigation::setbasevel(double vt=0.18){
    v =vt;
}

void freespacenavigation::setinitialpose(const std::vector<float> &pose){
    initialPose.x       =   pose[0];
    initialPose.y       =   pose[1];
    initialPose.theta   =   pose[2];
    std::cout<<"\n Init Pose = \t"<<initialPose.x<<"\t"<<initialPose.y<<"\t"<<initialPose.theta<<std::endl;

}
void freespacenavigation::setcurrentpose(const std::vector<float> &pose){
    currPose.x       =   pose[0];
    currPose.y       =   pose[1];
    currPose.theta   =   pose[2];
    std::cout<<"\n curr Pose = \t"<<currPose.x<<"\t"<<currPose.y<<"\t"<<currPose.theta<<std::endl;
    diffPose =  AL::Math::pose2DInverse(currPose)*initialPose;
     std::cout<<"\n diff Pose = \t"<<diffPose.x<<"\t"<<diffPose.y<<"\t"<<diffPose.theta<<std::endl;


    od[0] = double(-diffPose.x);
    od[1] = double(diffPose.y);
    od[2] = double(diffPose.theta);

    std::cout<<"\n od = \t"<<od[0]<<"\t"<<od[1]<<"\t"<<od[2]<<std::endl;


    initialPose = currPose;

}

double freespacenavigation::getvelfreespace(cv::Mat &Id){
  flno++;
  dls->setImname(flno);
 std::cout<<"\n od = \t"<<od[0]<<"\t"<<od[1]<<"\t"<<od[2]<<std::endl;
  dls->convert_msg(Id,Igrid, od,wrd,wrx);

    fs1 = wrx[0]; fs2 = wrx[1]; fs3 =  wrx[2]; fsx = wrx[4];

    wr[0] =  ((fs1-0)+fs1*v/1)/(1+fs1*fs1);
    wr[1] =  ((fs2-0)+fs2*v/2)/(1+fs2*fs2);
    wr[2] =  ((fs3-0)+fs3*v/3.25)/(1+fs3*fs3);
    wr[3] =  ((fsx-0)+fsx*v/0.9)/(1+fsx*fsx);

    double fs = (0.6*wr[1] + 0.4*wr[2]);
    flagx =3;
  // if((wr[1]>0 &wr[2]<0) | (wr[1]<0 & wr[2]>0)){ fs = (0.5*wr[1] + 0.5*wr[0]);}
    if(std::isnan(wr[2])) { fs = (0.5*wr[1] + 0.5*wr[0]); flagx =2;}
    if(std::isnan(wr[1])) { fs = (0.4*wr[3] + 0.6*wr[0]); flagx = 1;}
    if(std::isnan(wr[0]) ) { fs = std::numeric_limits<float>::quiet_NaN(); flagx = 0; }

    if(fabs(fs)>0.3)
       fs = 0.3*fs/fabs(fs);

    H = 1.0;

            if(flagx==3) H = 0.13;
            else if(flagx ==2)  H = 0.2;
            else if(flagx ==1) H = 0.29;
            else  H = -1.0; //obstacle in front



   return fs;
}

double freespacenavigation::getH(){

return H;

}


