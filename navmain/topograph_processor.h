/* Top level interface for processing topological map
 * reads topological graph, reference images from disk for navigation
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
 */
#ifndef TOPOGRAPH_PROCESSOR_H
#define TOPOGRAPH_PROCESSOR_H

#include <ctime>
#include "linenav/kimread.h"
#include "navigation.h"
#include "linenav/dispnav.h"
#include <array>



namespace tgraph{

/**
 * @brief The node struct each node has nodeid starting from 0.
 * nodename : higher level id for node
 */

struct node{
   int nodeid;
   std::string nodename;
};

/**
 *   Edges of topological graph
 *   sid: start id
 *   eid: end id
 *   fold : folder where the reference images of the edge lies
 *   edges and brances both represent the edge
 *    branches are used to simplify imags retrival for ibvs
 */

struct edge{
    int sid;
    int eid;
    std::string fold;
};

struct branch{
    int sid;
    int eid;
    std::string fold;
    int indx;
    char diflag;

};

    /**
     * @brief The topograph struct
     */

struct topograph
{
    unsigned long nbranch;
    std::string directionname;
    unsigned int tag;
    std::vector<branch> node;
};

/**
 * @brief The topmapprocessor class
 */

class topmapprocessor{

private:
    std::vector<tgraph::node> nodes;
    std::vector<tgraph::edge> edges;
    std::vector<tgraph::branch> branches;
    int nodesize;
    int edgesize;
    int branchsize;
    int edbrsize;
    std::string mapfold;
    std::vector<kimRead*> edbr;
    navigation *nav;
    int pos;
    int wt;
    int dirfint, dirfnav;
    bool useobsvaoid;


    /**
     * @brief setKimforinitnav extract all reference images from topological graph
     * for global localisation in the topological map
     */

    void setKimforinitnav(){
       std::string kimfold;
       edbr.reserve(edbrsize);

       for(int i=0;i<edgesize;i++){
           kimfold = mapfold + edges.at(i).fold;
           edbr[i] = new kimRead(kimfold);
           edbr[i]->setnodes(edges.at(i).sid,edges.at(i).eid);

        }

       for(int i=0;i<branchsize;i++){
           kimfold = mapfold+branches.at(i).fold;
           edbr[i+edgesize] = new kimRead(kimfold);
           edbr[i+edgesize]->setnodes(branches.at(i).sid,branches.at(i).eid);
       }
    }


    /**
     * @brief readconf readconfiguration file (conf.txt)
     * @param cfile  path of conf file
     */

    void readconf(std::string &cfile){
        mapfold = cfile + "/";
        std::string confile = cfile + "/conf.txt";
        std::cout<<confile<<std::endl;
        std::ifstream ifile(confile.c_str());
        nodes.clear();
        edges.clear();
        branches.clear();
        std::string s1;
        tgraph::node nd;
        tgraph::edge ed;
        tgraph::branch br;
        while(ifile) {
            std::string str;
            std::getline(ifile,str,'\n');
            if(str.length()==0) continue;
            if((str==std::string("Nodes"))==0){
                std::cout<<"no matches found"<<std::endl;
                break;
             }
            else{
                do{
                    std::getline(ifile,str,'\n');
                    if(str.length()==0) continue;
                    std::stringstream in(str);
                     in>>s1;
                    if(s1!="Edges"){
                        in>>nd.nodeid;
                         nd.nodename = s1;
                         nodes.push_back(nd);
                    }
                }while((s1==std::string("Edges"))==0);

                do{
                    std::getline(ifile,str,'\n');
                    if(str.length()==0) continue;
                    std::stringstream in(str);
                    in>>s1;

                    if(s1!="Branches"){
                        ed.fold = s1;
                        in>>ed.sid>>ed.eid;
                        edges.push_back(ed);
                       }
                    }while((s1==std::string("Branches"))==0);

                while(ifile){
                    std::getline(ifile,str,'\n');
                    if(str.length()==0) continue;
                    std::stringstream in(str);
                    in>>br.fold>>br.sid>>br.eid>>br.indx;
                    br.diflag=br.fold.back();
                    branches.push_back(br);
                    }
            }
        }
        nodesize = nodes.size();
        edgesize = edges.size();
        branchsize = branches.size();
        edbrsize = edgesize + branchsize;
        //showneb();
         setKimforinitnav();
    }


    /**
     * @brief showneb display nodes and edges
     */

    void showneb(){
        std::cout<<std::endl;

        for(int i=0;i<nodesize;i++){
            std::cout<<nodes.at(i).nodeid<<"\t"<<nodes.at(i).nodename<<std::endl;
        }
        std::cout<<std::endl<<std::left;
        for(int i=0;i<edgesize;i++){
            std::cout<<std::setw(10)<<edges.at(i).fold<<"\t"<<std::setw(10)<<nodes.at(edges.at(i).sid).nodename<<"\t-->\t"<<std::setw(10)<<nodes.at(edges.at(i).eid).nodename<<std::endl;
        }
            std::cout<<std::endl;
        for(int i=0;i<branchsize;i++){
            std::cout<<std::setw(10)<<branches.at(i).fold<<"\t"<<std::setw(10)<<nodes.at(branches.at(i).sid).nodename<<"\t-->\t"<<std::setw(10)<<nodes.at(branches.at(i).eid).nodename<<"\t"<<branches.at(i).indx<<"\t"<<branches.at(i).diflag<<std::endl;
        }
        std::cout<<std::endl;
    }

public:

    /**
     * @brief topmapprocessor
     */

    topmapprocessor(){
        nodes.clear();
        edges.clear();
        branches.clear();
        nodesize = 0;
        edgesize = 0;
        branchsize = 0;
        edbrsize = 0;
        pos = -1;
        wt = -1;
        dirfint = 0;
        dirfnav = 0;
        useobsvaoid = 0;
    }

    /**
     * @brief setNavigationPtr Set pointers for navgation
     * @param NAV
     */

    void setNavigationPtr(navigation *NAV){
        nav = NAV;
        useobsvaoid = nav->usecollisionavoidance();

    }

    /**
     * @brief getweightofcurredge
     * @return scaled weight of current edge
     */

    int getweightofcurredge(){
        return wt*100;
    }

    /**
     * @brief getobsavoiodflag
     * @return true if obs avoid is used
     */

    bool getobsavoiodflag(){
        return useobsvaoid;
    }

    /**
     * @brief getindexofcurredge location of the reference image in the edge that matches best with the current view
     * @return scaled position (to maintain same scale as edge weight(
     */

    int getindexofcurredge(){
        return pos*100;
    }

    /**
     * @brief localizeintopomap find the edge in the topological map correpoding to the current view of the robot
     * @param cim current image
     * @param nod current edge <startnode, endnode>
     * @return
     */

    int localizeintopomap(cv::Mat &cim, std::pair<int,int> &nod)
    {
        dirfint = 0;
        int *indx = new int[edbrsize]();
        int *lnno = new int[edbrsize]();
        int idx = -1; int mlin = -1;  pos = -1; wt = -1;
        for(int i=0;i<edbrsize;i++){
            nav->initlocalisation(cim,*edbr[i],indx[i],lnno[i]);
            if(lnno[i]>mlin){
                mlin = lnno[i];
                idx = i;
                wt = edbr[i]->kimsize()-indx[i];
                pos = indx[i];
            }
        }

        std::cout<<"ED = "<<edgesize<<"\t BR = "<<branchsize<<"\t EDBR = "<<edbrsize<<std::endl;
        std::cout<<"Init locl = pos "<<pos<<"\t idx ";

        if(idx<edgesize){
            nod.first = edges.at(idx).sid;
            nod.second = edges.at(idx).eid;
            dirfint = 0;
            std::cout<<idx<<"\t at \t"<<edges.at(idx).fold<<" with nmatch ="<<mlin<<std::endl;
        }
        else{
             std::cout<<idx-edgesize<<"\t at \t"<<branches.at(idx-edgesize).fold<<" with nmatch ="<<mlin<<std::endl;
             nod.first = branches.at(idx-edgesize).sid;
             nod.second = branches.at(idx-edgesize).eid;
             if(std::tolower(branches.at(idx-edgesize).diflag)=='c') dirfint = 1;
             else if(std::tolower(branches.at(idx-edgesize).diflag)=='a') dirfint = -1;
             else dirfint = 0;
        }
           std::cout<<nod.first<<"\t"<<nod.second<<std::endl;

        delete[] indx;
        delete[] lnno;

      return(dirfint);

    }

    /**
     * @brief kimshow show reference images in the list
     * @param pointer to the reference image list
     */

    void kimshow(kimRead *kf)
    {
        std::string Kfn;
        cv::Mat imgK;
        kf->resetIndex();
        int flag =0;
        while(!kf->isLast()){
             kf->getnextKeyImage(Kfn);
             imgK= cv::imread(Kfn,cv::IMREAD_COLOR);
             cv::imshow("kim",imgK);
             cv::waitKey(500*flag);
             flag = 1;
        }
        cv::destroyWindow("kim");
    }

    /**
     * @brief process process topological garp
     * @param imfold folder where toplogical garph lies
     */

    void process(char *imfold){

        std::string mapfold(imfold);
        readconf(mapfold);
        showneb();
    }

    /**
     * @brief getbranchindx read edge
     * @param s  start node
     * @param e  end node
     * @param d  dir
     * @return
     */

    int getbranchindx(int s, int e, char d){
    int ind =-1;

    for(int j=0;j<branchsize;j++){
        if(branches.at(j).diflag==d && branches.at(j).sid==s && branches.at(j).eid==e){
                 ind = branches.at(j).indx;
                 break;
               }
    }
    return ind;
    }

    /**
     * @brief setpathkim get reference image list of optimal path
     * @param optpath list of nodes in optimal path
     * @param kf list of reference images
     */

    void setpathkim(std::vector<int> optpath, kimRead *kf)
    {

        int len = optpath.size()-1;
        int ps = optpath.at(0);
        int ne = optpath.at(1);
        std::vector<std::string> path;
        std::vector<int> indx;
        path.clear();
        indx.clear();
        for(int i=0;i<len-1;i++)
        {
          int s = optpath.at(i);
          int e = ne;
          ne = optpath.at(i+2);
          std::stringstream ss;
          int ind;
          char d;
          if(s<7 && e<7){
             ss<<"Edge_"<<s<<e;
             ind = -1;}
          else{
              ss<<"Branch_"<<s<<e;
              std::cout<<"calc = \t"<<s<<"\t"<<ps<<"\t"<<e<<"\t"<<ne<<"\t"<<s-ps<<"\t"<<e-ne<<"\t";
              if((s<7 && ((ps-s)<0)) || ((e<7) && ((e-ne)<0)))
                d = 'c';
              else {
                d = 'a';
              }
              std::cout<<d<<std::endl;
              ss<<d;
              ind = getbranchindx(s,e,d);

          }

           ps = s;
           path.push_back(ss.str());
           indx.push_back(ind);
        }



        int elen = path.size();


        std::vector<int> pathidxs(elen,0);
        std::vector<int> pathidxe(elen,-1);

        for(int j =0;j<elen;j++){

            std::cout<<path.at(j)<<" ";
            if(j!=elen-1){
                std::cout<<"--> ";

                if(indx.at(j)<0 && indx.at(j+1)>0){
                    pathidxe.at(j)=indx.at(j+1);

                }
                else if(indx.at(j)>0 && indx.at(j+1)<0){
                    pathidxs.at(j+1)=indx.at(j);

                }

        }
        }

    std::cout<<std::endl;
    for(int j=0;j<elen;j++)
     std::cout<<indx.at(j)<<"\t"<<pathidxs.at(j)<<"\t"<<pathidxe.at(j)<<std::endl;

     std::string kfold = mapfold + path.at(0);
     if(kf!=NULL)
         delete kf;
    kf = new kimRead();


    for(int j=0;j<elen;j++){
        std::string tfold = mapfold + path.at(j);
         kimRead tmpkf(tfold);
         kf->addKeyImages(&tmpkf,pathidxs.at(j),pathidxe.at(j));

    }
    //for(int j=0;j<elen;j++)
     //std::cout<<indx.at(j)<<"\t"<<pathidxs.at(j)<<"\t"<<pathidxe.at(j)<<std::endl;

    kimshow(kf);
    kf->showkim();

    }

    /**
     * @brief setpathkim get reference image list of optimal path
     * @param optpath list of nodes in optimal path
     * @param kf list of reference images
     */

    kimRead* setpathkim(std::vector<int> optpath)
    {
        dirfnav = 0;
        int len = optpath.size()-1;
        int ps = optpath.at(0);
        int ne = optpath.at(1);
        std::vector<std::string> path;
        std::vector<int> indx;
        path.clear();
        indx.clear();
        std::cout<<std::endl;
        for(int i=0;i<len-1;i++)
        {
          int s = optpath.at(i);
          int e = ne;
          ne = optpath.at(i+2);
          std::stringstream ss;
          int ind;
          char d;
          if(s<7 && e<7){
             ss<<"Edge_"<<s<<e;
             ind = -1;}
          else{
              ss<<"Branch_"<<s<<e;
             // std::cout<<"calc = \t"<<s<<"\t"<<ps<<"\t"<<e<<"\t"<<ne<<"\t"<<s-ps<<"\t"<<e-ne<<"\t";
              if((s<7 && ((ps-s)<0)) || ((e<7) && ((e-ne)<0)))
                d = 'c';
              else {
                d = 'a';
              }
              //td::cout<<d<<std::endl;
              ss<<d;
              ind = getbranchindx(s,e,d);

          }

           ps = s;
           path.push_back(ss.str());
           indx.push_back(ind);
        }



        int elen = path.size();


        std::vector<int> pathidxs(elen,0);
        std::vector<int> pathidxe(elen,-1);

        for(int j =0;j<elen;j++){

            std::cout<<path.at(j)<<" ";
            if(j!=elen-1){
                std::cout<<"--> ";

                if(indx.at(j)<0 && indx.at(j+1)>0){
                    pathidxe.at(j)=indx.at(j+1);

                }
                else if(indx.at(j)>0 && indx.at(j+1)<0){
                    pathidxs.at(j+1)=indx.at(j);

                }

        }
        }

    std::cout<<std::endl;
    //for(int j=0;j<elen;j++)
     //std::cout<<indx.at(j)<<"\t"<<pathidxs.at(j)<<"\t"<<pathidxe.at(j)<<std::endl;
    std::cout<<"Start Path \t "<<path.at(0)<<std::endl;

    if(std::tolower(path.at(0).back())=='c') dirfnav = 1;
    else if(std::tolower(path.at(0).back())=='a') dirfnav = -1;
    else dirfnav = 0;

    int b2bflag = 0;
    if(elen>1){
    if(std::tolower(path.at(elen-1).front())=='b' && std::tolower(path.at(elen-2).front())=='b')
        b2bflag = 1;
    }
    std::cout<<"num of Edges = "<<elen<<"\t b2bflag = "<<b2bflag<<std::endl;

    kimRead *kf = new kimRead();

    kf->setKimbasefold(mapfold);
    for(int j=0;j<elen;j++){
        std::string tfold = mapfold + path.at(j);
         kimRead tmpkf(tfold);
         if(j==elen-1 && b2bflag==1){
              std::cout<<"ttf = "<<pathidxs.at(j)<<"\t"<<pathidxe.at(j)<<std::endl;
              kf->addKeyImages(&tmpkf,pathidxs.at(j),int(0.5*tmpkf.kimsize()));
         }
         else{
              std::cout<<pathidxs.at(j)<<"\t"<<pathidxe.at(j)<<std::endl;
              kf->addKeyImages(&tmpkf,pathidxs.at(j),pathidxe.at(j));
         }

    }
    //for(int j=0;j<elen;j++)
     //std::cout<<indx.at(j)<<"\t"<<pathidxs.at(j)<<"\t"<<pathidxe.at(j)<<std::endl;

    //kimshow(kf);
    //kf->showkim();
    return kf;
    }

    /**
     * @brief getdirflag
     * @return
     */

    int getdirflag(){
        return dirfnav;
    }

 };
}
#endif
