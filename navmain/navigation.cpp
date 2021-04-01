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


#include "navigation.h"

navigation::navigation(dispNav *d, cv::Mat &Kc)
{
    ln.setDisplay(d);
    ln.setK(Kc);
    K = Kc.clone();



    lne.reserve(4);
    lnj.reserve(4);
    lne2.reserve(4);
    lnj2.reserve(4);

    velfile.open("velfile.txt");
    featfile.open("featfile.txt");
    errfile.open("errfile.txt");
    jacfile.open("jacfile.txt");

    rvL = 0; rvL2 = 0;
    rv = 0;

    nl = 0;
    nl2 = 0;
    fetno = 0;


    useobsavoid = 0;

}

void navigation::usecollisionavoidance(bool flag){
    useobsavoid = flag;
}

bool navigation::usecollisionavoidance(){
    return(useobsavoid);
}
//dispImages
void navigation::initlocalisation(cv::Mat &Ic, kimRead &kf)
{
   kf.resetIndex();
   int LL = ln.initiallocalisationLines(Ic,kf);
   ln.setInitialKeyImages(Ic,kf,LL);
}

void navigation::initlocalisation(cv::Mat &Ic, kimRead &kf, int &idx, int &nlm)
{
    kf.resetIndex();
    ln.initiallocalisationLines(Ic,kf,idx,nlm);
}

int navigation::step(cv::Mat& Ic)
{
      return(ln.step(Ic));

}

int navigation::SwitchtoNewKeyImages(kimRead &kf)
{
    return(ln.SwitchtoNewKeyImages(kf));
 }

 double navigation::getRotVel()
 {

    rvL  = ln.getRotVel(lne,lnj);
    nl = ln.linesno;
    fetno = nl;
    if(fabs(rvL)>0.3) rvL = rvL/fabs(rvL) *0.3;

    rvL2 = ln.getRotVel2(lne2,lnj2);
    nl2 = ln.linesno;
    fetno = nl2;
    if(fabs(rvL2)>0.3) rvL2 = rvL2/fabs(rvL2) *0.3;


    std::cout<<"lines = "<<nl<<"\t lines2 = "<<nl2<<std::endl;



   rv = rvL;
    if(fabs(rv)<0.0001) rv = rvL2;

   velfile<<setw(10)<<rvL<<"\t"<<setw(10)<<rvL<<"\t"<<setw(10)<<rvL2<<"\t"<<setw(10)<<rvL2<<"\t"<<setw(10)<<rv<<std::endl;
   featfile<<setw(10)<<nl<<"\t"<<setw(10)<<nl<<"\t"<<setw(10)<<nl2<<"\t"<<setw(10)<<nl2<<std::endl;
   jacfile<<setw(10)<<lnj[0]<<"\t"<<setw(10)<<lnj[1]<<"\t"<<setw(10)<<lnj2[0]<<"\t"<<setw(10)<<lnj2[1]<<std::endl;
   errfile<<setw(10)<<lne[0]<<"\t"<<setw(10)<<lne[1]<<"\t"<<setw(10)<<lne2[0]<<"\t"<<setw(10)<<lne2[1]<<std::endl;

   return rv;
}

 double navigation::getinitdisp(){
     ln.getRotVel(lne,lnj);
     ln.getRotVel2(lne2,lnj2);
     return (0.25*(lne[0]+lne[1]+lne2[0]+lne2[1]));

 }

 double navigation::getturninginkim(){
     return(ln.getdiffN2NN());
 }
