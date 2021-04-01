/* Main file
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
 */

#include "pepperInterface.h"
#include "pepperRobot.h"
#include "pepperOffline.h"
#include "topograph_astar.h"
#include "linenav/kimread.h"
#include "topograph_processor.h"
#include "maingui/navinside.h"




void usage(int argc, char** argv){
    std::cout<<"Usage: "<<argv[0]<<" /path/to/topologicalgraph/folder pepper_ip_address"<<std::endl;
     std::cout<<"Usage: "<<argv[0]<<" /path/to/topologicalgraph/folder /path/to/offline/images"<<std::endl;
}

int main(int argc, char** argv)
{

    if(argc<2){
        usage(argc,argv);
        return -1;
    }

    tgraph::topmapprocessor tp;
    tp.process(argv[1]);


    /**
     * @brief Define Nodes
     */

    enum nodes {
      PepperHome, Harvey, Rob1,
        Peter,
        ROb2, Cartman,
        Belinda,
        Manipulation, Robotics,
        N
    };


    astar::node name[]{
        "PepperHome", "Harvey", "Rob1", "Peter",
        "ROb2", "Cartman", "Belinda", "Manipulation", "Robotics"
      };

    astar::location locations[] = {
      {0, 0}, {1, 0}, {2, 0},
      {3, 1},
      {2, 1}, {1, 1},
      {0, 1},
      {1, 0.5}, {2, 0.5}
    };


    /**
      * @brief Define Edges
      */



   astar::edge edge_array[] = {
       astar::edge(PepperHome,Harvey), astar::edge(Harvey,Rob1),
       astar::edge(Rob1,Peter), astar::edge(Peter,ROb2),
       astar::edge(ROb2,Cartman), astar::edge(Cartman,Belinda),
       astar::edge(Belinda,PepperHome),
       astar::edge(Harvey,Manipulation), astar::edge(Manipulation,Cartman),
       astar::edge(Rob1,Robotics), astar::edge(Robotics,ROb2)
    };
   int num_edges = sizeof(edge_array) / sizeof(astar::edge);

  std::cout<<"No of Nodes = "<<N<<std::endl;
  std::cout<<"No of edges = "<<num_edges<<std::endl;




  astar::cost weights[] = {
      17.5, 17.0,
      29.0, 21.5,
      17.5, 28.5,
      6,
      10.5, 11.25,
      9.0,  7.25
    };



    astar::cost *edlen = new astar::cost[num_edges];

    auto minval = min_element(weights,weights+num_edges);
    auto maxval = max_element(weights,weights+num_edges);
    float ll = 4;
    float ul = 6;
    float m = (ul-ll)/((*maxval)-(*minval));
    for(int i=0;i<num_edges;i++){
     // edlen[i] =  1.5*weights[i]/(*minval);
         edlen[i] =  m*(weights[i]-(*minval))+ll;
         std::cout<<edlen[i]<<"\t";
       }

    // Graph initialisation
     astar::astar AS(N);
     AS.setlocationherustics(name,locations);
     AS.setedges(num_edges,edge_array,weights);
     AS.creategraph();
     std::string dfile("S11.dot");
     AS.writegraph2file(dfile);

    // dispaly initialisation
    dispNav *d;
    d=new dispNav;

    // USer settings
    d->saveimage(true);         // save nav images
    d->displayimage(true);      // dispaly nav images
    d->showfeat(false);         // drwaw features in dispaly window
    d->setDisptime(90);       // display delay

    /* use obstacle aviodance module or not */
    bool useobsavoid = false;

    // set mdoe
    // 0 => online on robot
    // 1 => offline

    int mode = 1;


    // Initialisation navigation parameters
    int id  = 0;

    cv::Mat K = (cv::Mat_<double>(3, 3) <<296.3437428191,0,158.2733787287, 0, 296.6441607237,118.0622191474 , 0, 0, 1);
    navigation nav(d,K);
    nav.usecollisionavoidance(useobsavoid);
    tp.setNavigationPtr(&nav);





   
    NavInside w;
     std::string opt_ip;


 try{
      pepperInterface *pr;


    // online mode

     if(mode==0){
        if(argc>2)
             opt_ip = std::string(argv[2]);
         else
             opt_ip = "172.19.226.200";
          
         std::cout<<"\n Pepper_IP = "<<opt_ip<<std::endl;
          pr = new pepperRobot(opt_ip);
          id =0;   //camera id
      }

     // offline mode
    else{
         std::stringstream datfold;
         datfold<<argv[2]<<"/";

       // path of the navigation image sequence
        opt_ip = datfold.str() + "imgs";
        pr = new pepperOffline(opt_ip);

      //set path of correpsonding reference image folder
       //w.setKimfoldoffline(datfold.str() + "kfls");

      //in offline mode => id = destinatin node
      id = Cartman;
      }

    // start Pepper
    pr->startPepper(id);
    if(useobsavoid){
        pr->openDepthCamera();
    }


    w.setPepperRobotPointer(pr, d);
     w.setPointers(AS,tp);
    w.setGraph(N,name,num_edges,edge_array,edlen);
    w.startRobotNavigation();


  }
    catch(int a){ std::cerr << "Caught exception: " << std::endl; }

   // return a.exec();

}


