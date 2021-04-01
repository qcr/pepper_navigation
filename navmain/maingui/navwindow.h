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


#ifndef NAVWINDOW_H
#define NAVWINDOW_H

#include "pepperInterface.h"
#include "peppernavigation.h"
#include <QMainWindow>
#include "topograph_astar.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "QGVScene.h"
#include "topograph_processor.h"
#include "window_QT.h"


namespace Ui {
class NavWindow;
}

class NavWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief NavWindow
     * @param parent
     */
        explicit NavWindow(QWidget *parent = nullptr);
    ~NavWindow();



    /**
     * @brief drawGraph  create ad draw topological graph
     * @param N numebr of nodes
     * @param node_array nodes list
     * @param num_edges number of edges
     * @param edge_array edge list
     * @param wts edge weights
     */

    void drawGraph(int N, astar::node *node_array, int num_edges, astar::edge *edge_array, astar::cost *wts);

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


private slots:
    void nodeDoubleClick(QGVNode* node);
     void on_pushButton_clicked();

     void on_pushButtonclear_clicked();

     void on_startButton_clicked();

     void on_pushButtonx_clicked();
     void on_Navigate_clicked();

private:
    Ui::NavWindow *ui;

    QGraphicsScene *scene;
    QGraphicsEllipseItem *ellipse;
    QGraphicsRectItem *rectangle;
    QGraphicsTextItem *text;
     std::vector<QGVNode*> nodes;
     std::vector<QGVEdge*> edges;
    QGVScene *myscene;
    bool clickflag;
    bool clickdisableflag;
    astar::astar *AS;
    astar::node *name;
    tgraph::topmapprocessor *tp;
    cvqt::CvWindow *imdisp;
    void showkim(/*kimRead *kf*/);
    void setGraphAttributes();
    void performNavigation();
    void setKimforNavigation();
    void navigateoffline();
    void selectKeyImages(int ct, std::vector<int> &optpath);

    QString nlab;
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

#endif // NAVWINDOW_H
