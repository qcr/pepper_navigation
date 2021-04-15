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
 * Topological Naviagtion non GUI version
 * This version is able to run onboard
 */


#ifndef NAVINSIDE_H
#define NAVINSIDE_H
#include <utility>
#include "pepperInterface.h"
#include "peppernavigation.h"
#include "topograph_astar.h"
#include "topograph_processor.h"



class NavInside{

public:
     NavInside();
    ~NavInside();



    /**
     * @brief setPointers set pointers rekared to graph processing and searching
     * @param as Pointer to astar search
     * @param TP Pointer to topological garph processor
     */
    void setPointers(astar::astar &as,tgraph::topmapprocessor &TP);

    /**
     * @brief setPepperRobotPointer
     * @param pr
     */
    void setPepperRobotPointer(pepperInterface* pr, dispNav* d);

    /**
     * @brief setKimfoldoffline
     * @param kif
     */
    void setKimfoldoffline(std::string kif);

  void startRobotNavigation();
void setGraph(int N, astar::node *node_array, int num_edges, astar::edge *edge_array,astar::cost *wts);
private:

    bool clickflag;
    bool clickdisableflag;
    astar::astar *AS;
    astar::node *name;
    tgraph::topmapprocessor *tp;
   double inilocltime;
    void performNavigation();
    void setKimforNavigation();
    void navigateoffline();
    void selectKeyImages(int ct, std::vector<int> &optpath);
    void pathplaning();
    void showkim(/*kimRead *kf*/);
    int start;
    int goal;
    int **ve;
    int nNodes;
    int nEdges;
    std::pair<int,int> inode;
    std::vector<cv::String> imlist;
  
    kimRead *kf;
    pepperInterface *pepper;
    pepperNavigation *pn;
    std::string kimfold;
    int runcount;
    int dirinit;
    bool obsavoid;
   int kk;

};

#endif // NAVWINDOW_
