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

#include "linematch.h"
#include <fstream>


linematch::linematch()
{
    lm = new PairwiseLineMatching;
    ld = new LineDescriptor;
   // tmp.reserve(3);
}

/*int linematch::getlinedesc(BIAS::Image<unsigned char> &leftImage,ScaleLines &linesInLeft)
{
    return(ld->GetLineDescriptor(leftImage,linesInLeft));
    /*if(!ld->OctaveKeyLines(leftImage,linesInLeft)){
        std::cout<<"No lines detected"<<std::endl;
        return -1;
    }

    ld->findLineDesc(linesInLeft);
    return 0;*/
//}

int linematch::getlinedesc(cv::Mat &leftImage,ScaleLines &linesInLeft)
{
    return(ld->GetLineDescriptor(leftImage,linesInLeft));
    /*if(!ld->OctaveKeyLines(leftImage,linesInLeft)){
        std::cout<<"No lines detected"<<std::endl;
        return -1;
    }

    ld->findLineDesc(linesInLeft);
    return 0;*/
}


/*int linematch::getlinedesc(BIAS::Image<unsigned char> &leftImage,ScaleLines &linesInLeft,std::string linefile)
{
    if(!ld->OctaveKeyLines(leftImage,linesInLeft)){
        std::cout<<"No lines detected"<<std::endl;
        return -1;
    }

    ld->findLineDesc(linesInLeft);


    std::ofstream ofile1(linefile.c_str());

    for (int i = 0;i < linesInLeft.size();i++){
        ofile1<<linesInLeft[i][0].startPointX<<" "<<linesInLeft[i][0].startPointY<<" "<<linesInLeft[i][0].endPointX<<" "<<linesInLeft[i][0].endPointY<<" "<<linesInLeft[i][0].lineLength<<" "<<linesInLeft[i][0].direction<<" "<<linesInLeft[i][0].salience<<" "<<linesInLeft[i][0].numOfPixels;
      //  cout<<linesInLeft[i][0].startPointX<<" "<<linesInLeft[i][0].startPointY<<" "<<linesInLeft[i][0].endPointX<<" "<<linesInLeft[i][0].endPointY<<" "<<linesInLeft[i][0].lineLength<<" "<<linesInLeft[i][0].direction<<" "<<linesInLeft[i][0].salience<<" "<<linesInLeft[i][0].numOfPixels;
     for(int j=0;j<linesInLeft[i][0].descriptor.size();j++){
        ofile1<<" "<<linesInLeft[i][0].descriptor[j];
      //  cout<<" "<<linesInLeft[i][0].descriptor[j];
     }
    ofile1<<std::endl;
  //  std::cout<<linesInLeft[i][0].descriptor.size()<<endl;

    }
ofile1.close();
return 0;

}

*/
void linematch::readlinedesc(std::string keyln, ScaleLines &linesInRight){

            std::ifstream ifile2(keyln.c_str());
           // std::cout<<keyln;
             linesInRight.clear();
        while(ifile2) {
            std::string str;
            std::getline(ifile2,str,'\n');
            if(str.length()==0) continue;

            double x0,y0,x1,y1,len,grad,sal,npix;
            std::vector<float> descriptor;
        //	unsigned int npix;
            std::stringstream in(str);
             descriptor.clear();
            in>>x0>>y0>>x1>>y1>>len>>grad>>sal>>npix;
             int ct = 0;
                while(in) {
                float xx;
                in>>xx;
                descriptor.push_back(xx);
            //	std::cout<<ct++<<" "<<xx<<std::endl;
                ct++;
                if(ct==72)
                    break;
            }

                //for(int kkk=0;kkk<descriptor.size();kkk++)
                //	std::cout<<kkk+1<<" "<<descriptor.at(kkk)<<std::endl;

                //std::cout<<std::endl;

                OctaveSingleLine singleLine;
                singleLine.numOfPixels = (unsigned int)(npix);
                singleLine.startPointX = x0;
                singleLine.startPointY = y0;
                singleLine.endPointX = x1;
                singleLine.endPointY = y1;
                singleLine.lineLength = len;
                singleLine.direction = grad;
                singleLine.ePointInOctaveX = x1;
                singleLine.ePointInOctaveY = y1;
                singleLine.sPointInOctaveX = x0;
                singleLine.sPointInOctaveY = y0;
                singleLine.salience = sal;
                singleLine.octaveCount = 0;
                singleLine.descriptor = descriptor;
                LinesVec lv;
                lv.push_back(singleLine);
                linesInRight.push_back(lv);
            //	std::cout<<std::endl<<descriptor.size()<<std::endl;
            }
         ld->computeBinaryLineDesc(linesInRight);
        ifile2.close();
        //cout<<"readdone"<<endl;

}

void linematch::matchlines(ScaleLines &linesInLeft,ScaleLines &linesInRight,std::vector<unsigned int> &matchResult)
{
  lm->LineMatching(linesInLeft,linesInRight,matchResult);
}

void linematch::matchlinesbinary(ScaleLines &linesInLeft,ScaleLines &linesInRight,std::vector<unsigned int> &matchResult)
{

  ld->LineMatchingBinary(linesInLeft,linesInRight,matchResult);
}

void linematch::findCommonIndex(std::vector<unsigned int> v1,std::vector<unsigned int> v2, std::vector< std::vector< int> >&op){

    int sv1 = v1.size()/2;
    int sv2 = v2.size()/2;
    op.clear();

    for(int i=0;i<sv1;i++)
    {
        int ev1 = v1.at(2*i);
        for(int j=0;j<sv2;j++)
        {
            int ev2 = v2.at(2*j);
            if(ev1==ev2){
              //  ct++;
                std::vector<int> tmp;
               /* tmp[0]=ev1;
                tmp[1]=v1.at(2*i+1);
                tmp[2]=v2.at(2*j+1);*/
                tmp.push_back(ev1);
                tmp.push_back(v1.at(2*i+1));
                tmp.push_back(v2.at(2*j+1));
                op.push_back(tmp);
                break;
            }
        }
    }
//std::cout<<"<n ml="<<ct<<std::endl;

}

void linematch::findCommonIndex2(std::vector<unsigned int> v1,std::vector<unsigned int> v2, std::vector< std::vector< int> >&op){

    int sv1 = v1.size()/2;
    int sv2 = v2.size()/2;
    op.clear();

    for(int i=0;i<sv1;i++){
        int ev1 = v1.at(2*i+1);
        for(int j=0;j<sv2;j++){
            int ev2 = v2.at(2*j);
            if(ev1==ev2){
              //  ct++;
                std::vector<int> tmp;
                tmp.push_back(v1.at(2*i));
                tmp.push_back(ev1);
                tmp.push_back(v2.at(2*j+1));
                op.push_back(tmp);
                break;
            }
        }
    }
//std::cout<<"<n ml="<<ct<<std::endl;

}

