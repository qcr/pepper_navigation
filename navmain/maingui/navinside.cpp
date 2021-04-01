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
 * Topological Naviagtion GUI version
 */

#include "opencv2/highgui/highgui_c.h"
#include <opencv2/core/types_c.h>
#include "navinside.h"


/**
 * @brief NavInside::NavInside
 * @param parent
 */

NavInside::NavInside() 
{
    
     clickflag = 0;
     clickdisableflag = 1;


   

    
    start = 0;
    goal = 0;
   
    nNodes = -1;
    nEdges = -1;
    inode.first = -1;
    inode.second = -1;

    runcount =0;
    kf = nullptr;
    dirinit = 0;
    obsavoid = 0;
    kk = 0;

}








NavInside::~NavInside()
{
  
}

void NavInside::setGraph(int N, astar::node *node_array, int num_edges, astar::edge *edge_array,astar::cost *wts)
{


//Add some nodes

 

  name = new astar::node[N];
  ve = new int*[N];

  for(int i=0;i<N;i++)
  {
     // std::cout<<node_array[i]<<std::endl;
      ve[i] = new int[N]();
      name[i] = node_array[i];
  }

//Add Edges
   for(int i=0;i<num_edges;i++){
      ve[edge_array[i].first][edge_array[i].second] = i;
      ve[edge_array[i].second][edge_array[i].first] = i;
   }

/*for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
        std::cout<<ve[i][j]<<"\t";}
     std::cout<<std::endl;}
     */
//Layout scene
   nNodes = N;
   nEdges = num_edges;


//Fit in view


std::cout<<std::endl;

}

void NavInside::pathplaning()
{

 std::vector<int> optpath;
 optpath.clear();
 //int ct = AS->searchingraphrandom(start, goal,optpath);
 start = inode.second;
 int ct = AS->searchingraph(start,goal,optpath);

 if(optpath.at(1)==inode.first){
  std::cout<<"need to reverse"<<std::endl;
  start = inode.second;
 
   pepper->rotate180();
 }
 else
 {
   optpath.insert(optpath.begin(),inode.first);
   start = inode.first;
   if(ct)  ct=ct+1;
 }
  std::cout<<"ct = "<<ct<<std::endl;
 selectKeyImages(ct, optpath);


}

void NavInside::selectKeyImages(int ct, std::vector<int> &optpath){



 int pl = optpath.size();


  std::cout << "Start vertex: " << name[start] << std::endl;
  std::cout << "Goal vertex: " << name[goal] << std::endl;


  if(ct){
      std::cout << "\n Shortest path from " << name[start] << " to "
                      << name[goal] << ": ";




     std::cout << name[optpath.at(0)];

     for(int i=1;i<pl-1;i++)
         std::cout << " -> " << name[optpath.at(i)];

         std::cout << std::endl << "Total travel time: " << optpath.at(pl-1) << std::endl;
  }

  else{
     std::cout << "Can't find a path from " << name[start] << " to " << name[goal] << "!" << std::endl;
     }


 if(ct){
     std::cout<<"\nTotal nodes to be visited = "<<ct<<std::endl;

     std::cout<<"Nodes : \t";
       for(int i=0;i<pl-1;i++){
         std::cout<<optpath.at(i)<<"\t";
      
       }
       std::cout<<std::endl;



        kf = tp->setpathkim(optpath);

       showkim(/*kf*/);

      

       int dirf = tp->getdirflag();

 }



 clickflag = 0;

   

}

void NavInside::setPointers(astar::astar &as,tgraph::topmapprocessor &TP)
{
AS = &as;
tp = &TP;
obsavoid = tp->getobsavoiodflag();
if(obsavoid){
    cv::Mat Kd = pepper->getKd();
    pn->setfreespacenav(Kd);
}
}

void NavInside::setPepperRobotPointer(pepperInterface* pr, dispNav* d){
    pepper = pr;
    cv::Mat K = pepper->getK();
    pn = new pepperNavigation(K,d);

}

void NavInside::showkim(/*kimRead *kf*/)
{
   

    std::string Kfn;
    cv::Mat imgK;
    kf->resetIndex();
    int foldlen =0;
 
    int nkim = kf->kimsize();
    if(nkim){
        
        std::cout<<"\n Total number of Key Images = "<<kf->kimsize()<<std::endl;
        kf->getkimfold(Kfn);
        foldlen = Kfn.size();
        

    while(!kf->isLast()){
         kf->getnextKeyImage(Kfn);
         std::cout<<Kfn<<std::endl;
         
    }
  //  cv::destroyWindow("kim");
    std::cout<<"Path Found"<<std::endl;
    
}
 else{
     std::cout<<"\n Destination same as Source"<<std::endl;
     
    }

  
}


void NavInside::startRobotNavigation(){

    inode.first = 0;
    inode.second = 0;
   

   // srand(time(NULL));
   // start = rand()%(nNodes-1);*/
  // int kk = rand()%(imlist.size()-1);

   //  std::cout<<kk<<"\t"<<imlist.size()<<std::endl;
    cv::Mat cim;// = cv::imread(imlist.at(kk),cv::IMREAD_GRAYSCALE);
    pepper->getCurrImage(cim);
    inilocltime = 0;
       clock_t tStartxx = clock();
      goal = pepper->getid();
    dirinit = tp->localizeintopomap(cim,inode);
    inilocltime = ((double(clock() - tStartxx))/CLOCKS_PER_SEC);
    start = inode.second;
   setKimforNavigation();
     runcount++;

     std::cout<<"\n \n OBSAVOID = "<<obsavoid<<"\n"<<std::endl;

    performNavigation();

}

void NavInside::navigateoffline(){

          clickdisableflag = 1;
          goal = pepper->getid();
          clickflag = 0;
   
         // setKimforNavigation();
          if(kf != nullptr)
              delete kf;
          kf = new kimRead(kimfold);

         
           obsavoid = 0;

          performNavigation();

}


void NavInside::setKimforNavigation(){

    std::vector<int> optpath_sn;
    optpath_sn.clear();
    std::vector<int> optpath_en;
    optpath_en.clear();
    int start_sn, start_en;
    //int ct = AS->searchingraphrandom(start, goal,optpath);
    start = inode.first;
    int cts = AS->searchingraph(start,goal,optpath_sn);
    start = inode.second;
    int cte = AS->searchingraph(start,goal,optpath_en);
    int revflags = 0;
    int revflage = 0;

    int cost_sn = optpath_sn.at(cts);
    int cost_en = optpath_en.at(cte);
     std::cout<<"\n cost = "<<cost_sn<<"\t"<<cost_en<<std::endl;
    if(optpath_en.at(1)==inode.first){
     // std::cout<<"need to reverse"<<std::endl;
      start_en = inode.second;
  
      revflage = 1;

     }
     else
     {
       optpath_en.insert(optpath_en.begin(),inode.first);
       start_en = inode.first;
       cost_en += tp->getweightofcurredge();//AS->getweight(ve[inode.first][inode.second]);
       revflage = 0;
       if(cte)  cte=cte+1;
     }

    /* std::cout<<"\n"<<"From End Node = \n";
       for(int i=0;i<=cte;i++)
         std::cout<<optpath_en.at(i)<<"\t";
       std::cout<<"\n"<<std::endl;*/

    // start = start_en;
     //selectKeyImages(cte, optpath_en);



    // myscene->applyLayout();

    if(optpath_sn.at(1)==inode.second){

     start_sn = inode.first;
     revflags = 0;
    }
    else
    {
      revflags = 1;
      start_sn = inode.second;
      optpath_sn.insert(optpath_sn.begin(),inode.second);
      cost_sn += tp->getindexofcurredge();//AS->getweight(ve[inode.first][inode.second]);
      if(cts)cts=cts+1;
    }


  /*  std::cout<<"\n"<<"From Start Node = \n";
    for(int i=0;i<=cts;i++)
      std::cout<<optpath_sn.at(i)<<"\t";
        std::cout<<std::endl;

    start = start_sn;
    selectKeyImages(cts, optpath_sn);
    */
 std::cout<<"\n cost = "<<cost_sn<<"\t"<<cost_en<<std::endl;
    if(cost_sn<cost_en){
        start = start_sn;
        if(revflags){
            std::cout<<"need to reverse"<<std::endl;
           
            pepper->rotate180();
        }
        selectKeyImages(cts, optpath_sn);
    }
    else{
        if(revflage){
            std::cout<<"need to reverse"<<std::endl;
         
             pepper->rotate180();
        }
        start = start_en;
        selectKeyImages(cte, optpath_en);
    }



}




void NavInside::performNavigation(){

    float tv = 0.15f;

    float rv = 0.0;
    float rvL = 0.0;
    float rvfs = 0.0;
    float H = 0.0;
    int navflag;
    float sv;
    float fv = 0.0;
    float turnval;
    float dispal=0.0;
   double lnt = 0.0;
        double dnt = 0.0;
       double nt  = 0.0;
        double lntt = 0.0;
       double dntt = 0.0;
       double ntt  = 0.0;
    if(kf->kimsize()>0){
        pn->setPath(kf);
        cv::Mat cim;
        cv::Mat Id;
        pepper->getCurrImage(cim);
        if(obsavoid) pepper->getCurrDepthImage(Id);
        cv::cvtColor(cim,cim,cv::COLOR_BGR2GRAY);
        //pepper->adjusthead();

        pn->localise(cim);
        dispal = pn->getinitdisplacemt(cim);
        std::cout<<"Init err = \t"<<dispal<<std::endl;
       /* do{
          rv = 0.0f;
          if(dispal>0.01f) rv = 0.05f;
          else if(dispal<-0.01f) rv = -0.05f;

           pepper->setBaseVelocities(0.0,rv,0.0);
           pepper->getCurrImage(cim);
           cv::cvtColor(cim,cim,cv::COLOR_BGR2GRAY);
            dispal = pn->getinitdisplacemt(cim);
             std::cout<<"disp err = \t"<<dispal<<std::endl;
        }while(fabs(dispal)>0.05f);
*/
        rv = 0.0;
        rvL = 0.0;
        rvfs = 0.0;
        H = 0.0;

        pepper->setBaseVelocities(0.0,0.0,0.0);

        turnval = pn->getturninginkim();
        pn->setofileid(runcount);



        std::stringstream fn;
        fn<<"poseinfo_"<<std::to_string(runcount)<<".txt";
        std::ofstream ofilepose(fn.str().c_str());

        std::vector<float> tt;
        tt = pepper->getOdometryReading();
        if(obsavoid)
            { pn->setinitialpose(tt);
              pn->setbasevel(tv);
        }

        ofilepose<<setw(5)<<kk<<"\t"<<setw(10)<<tt[0]<<"\t"<<setw(10)<<tt[1]<<"\t"<<setw(10)<<tt[2]<<"\t"<<setw(10)<<rv<<"\t"<<setw(10)<<fv<<"\t"<<setw(10)<<turnval<<"\t"<<setw(10)<<H<<"\t"<<setw(10)<<rvL<<"\t"<<setw(10)<<rvfs<<std::endl;

        int excpflag = 0;
     



        while(1){
            kk++;

            std::cout<<"------------step------------------"<<std::endl;
             clock_t tStart = clock();
            pepper->getCurrImage(cim);
            if(obsavoid) pepper->getCurrDepthImage(Id);
            
            tt = pepper->getOdometryReading();

            pepper->adjusthead();

            clock_t tStartln = clock();
            cv::cvtColor(cim,cim,cv::COLOR_BGR2GRAY);

            rvL = pn->navigate(cim);
            lnt= ((double(clock() - tStartln))/CLOCKS_PER_SEC);

             sv = 1.0f;

            if(std::isnan(rvL)){/*fv = 0.05;  //lost robot
                if(turnval<0) rv = -0.05;
                else rv = 0.05;
                if(fabs(turnval)>0.9) {*/ fv = 0.0 ; rv = 0.0;
                excpflag++;

            }
           // }

            else{
                    rvfs = 0.0;
                    H = 0.0;
                    if(obsavoid) {
                        clock_t tStartdn = clock();
                        pn->setcurrentpose(tt);
                         rvfs = pn->getvelfreespace(Id);
                         H = pn->getH();

                        if(H<0) {sv = 0.0; H=0.0;}
                        if(fabs(rvfs)<0.0001f ) H = 0.0;
                         dnt= ((double(clock() - tStartdn))/CLOCKS_PER_SEC);
                    }

                    if(fabs(rv)>0.15f){ H = 0.0;}
                    if(fabs(turnval)>0.08f){ H = 0.0; sv = 0.5;}

                 if(sv>0)
                    rv = H*rvfs + (1-H)*rvL;
                 else
                     rv = 0.0;


                if(fabs(rv)>0.08f)  sv = 0.3f;

                fv = tv*sv;

                excpflag = 0;
            }
            fv =0; rv=0;

           pepper->setBaseVelocities(fv,rv);

            navflag = pn->continueNav(turnval);
            std::stringstream aa;
            if(kk<10)
              aa<<"cimg0000"<<kk<<".png";
            else if (kk<100)
                 aa<<"cimg000"<<kk<<".png";
            else if (kk<1000)
                    aa<<"cimg00"<<kk<<".png";
            else if (kk<10000)
                    aa<<"cimg0"<<kk<<".png";
            else
                aa<<"cimg"<<kk<<".png";

            cv::imwrite(aa.str().c_str(),cim);

            if(excpflag>10){
                std::cout<<"\n I am trying to reloclize myself"<<std::endl;
                navflag = 1;
                excpflag = 0;
                pn->localise(cim);
                pepper->setBaseVelocities(0.0,0.0);
                turnval = pn->getturninginkim();

            }

            if(navflag==2){

                pepper->adjusthead();}
            else if(navflag == 0){
              pepper->setBaseVelocities(0.0,0.0);
               std::cout<<"end of topological navigation"<<std::endl;
              // pn->closedisp();
              break;
            }
            nt = ((double(clock() - tStart))/CLOCKS_PER_SEC); ofilepose<<setw(5)<<kk<<"\t"<<setw(10)<<tt[0]<<"\t"<<setw(10)<<tt[1]<<"\t"<<setw(10)<<tt[2]<<"\t"<<setw(10)<<rv<<"\t"<<setw(10)<<fv<<"\t"<<setw(10)<<turnval<<"\t"<<setw(10)<<H<<"\t"<<setw(10)<<rvL<<"\t"<<setw(10)<<rvfs<<"\t"<<lnt<<"\t"<<dnt<<"\t"<<nt<<std::endl;
             std::cout<<"---------------------------------------------"<<std::endl;
            std::cout<<"\n exec time = "<<lnt<<"\t"<<dnt<<"\t"<<nt<<std::endl;
            lntt+= lnt;
            dntt+= dnt;
            ntt+= nt;

        }

         pepper->setBaseVelocities(0.0,0.0);
          ofilepose<<setw(5)<<kk<<"\t"<<lnt<<"\t"<<dnt<<"\t"<<nt<<"\t"<<lnt/kk<<"\t"<<dnt/kk<<"\t"<<nt/kk<<std::endl;
          ofilepose<<inilocltime<<std::endl;
          ofilepose.close();


    }

   std::cout<<inilocltime<<"\t"<<kk<<"\t"<<lnt<<"\t"<<dnt<<"\t"<<nt<<std::endl;

}


void NavInside::setKimfoldoffline(std::string kif){
    kimfold = kif;
    std::cout<<"Kimfold = "<<kimfold<<std::endl;

}
