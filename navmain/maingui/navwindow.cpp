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


#include <opencv2/core/types_c.h>

#include "navwindow.h"
#include "ui_navwindow.h"
#include "QGVScene.h"
#include "QGVNode.h"
#include "QGVEdge.h"
#include "QGVSubGraph.h"
#include <QMessageBox>

/**
 * @brief NavWindow::NavWindow
 * @param parent
 */

NavWindow::NavWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NavWindow)
{
    ui->setupUi(this);
    ui->nodeinfo->setHidden(true);
     clickflag = 0;
     clickdisableflag = 1;
   // scene = new QGraphicsScene(this);
   // ui->graphicsView->setScene(scene);

    myscene = new QGVScene(QString("TopNavPepper"),this);
    //myscene = new QGVScene("TopNavPepper", std::string("/home/suman/soft/pepper_ws/topmanav/build/S11.dot"),this);
    ui->graphicsView->setScene(myscene);
    ui->pushButton->hide();
    ui->pushButtonx->hide();

    ui->pushButtonclear->hide();
    connect(myscene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
    imdisp = new cvqt::CvWindow(QString("cvWindow"), CV_WINDOW_NORMAL|CV_GUI_NORMAL|CV_WINDOW_FREERATIO);
    imdisp->setGeometry(0,0,320,240);
    ui->frame->setGeometry(this->width()-5-320,10,320,240);
    ui->textBrowser->setHidden(true);
    imdisp->setParent(ui->frame);
    ui->frame->setHidden(true);
    ui->Navigate->hide();
    start = 0;
    goal = 0;
    ui->startButton->show();
    ui->pathinfo->setHidden(true);
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


void NavWindow::setGraphAttributes()
{

  //  myscene->setGraphAttribute("label", "Select START and GOAL Nodes");

    myscene->setGraphAttribute("splines", "curved");
    myscene->setGraphAttribute("rankdir", "LR");

    myscene->setGraphAttribute("nodesep", "1");
    myscene->setGraphAttribute("rank","same");
    myscene->setGraphAttribute("ranksep","equal");

    myscene->setNodeAttribute("shape", "circle");
    myscene->setNodeAttribute("style", "filled");
    myscene->setNodeAttribute("fillcolor", "white");
    myscene->setNodeAttribute("height", "1.25");
    myscene->setEdgeAttribute("minlen", "3");
    myscene->setEdgeAttribute("dir", "both");
    myscene->setGraphAttribute("layout","neato");
}


void NavWindow::drawGraph(int N, astar::node *node_array, int num_edges, astar::edge *edge_array,astar::cost *wts)
{
 setGraphAttributes();

//Add some nodes

  nodes.reserve(N);
  edges.reserve(num_edges);

  name = new astar::node[N];
  ve = new int*[N];

  for(int i=0;i<N;i++)
  {
     // std::cout<<node_array[i]<<std::endl;
      ve[i] = new int[N]();
      QString qs = QString::number(i);
      nodes[i] = myscene->addNode(node_array[i],qs);
      name[i] = node_array[i];
  }

//Add Edges
   for(int i=0;i<num_edges;i++){

      edges[i] = myscene->addEdge(nodes[edge_array[i].first],nodes[edge_array[i].second],wts[i]);
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
myscene->applyLayout();

//Fit in view
ui->graphicsView->fitInView(myscene->sceneRect(), Qt::KeepAspectRatio);

std::cout<<std::endl;

}



void NavWindow::nodeDoubleClick(QGVNode *node)
{

  if(!clickdisableflag){

    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
     std::cout<<"Node "<<node->id().toInt()<<" with label "<<node->label().toStdString()<<" is selected"<<std::endl;

    if(clickflag){

        ui->pushButton->show();
        //ui->pushButtonx->show();
        ui->pushButtonclear->show();
        clickdisableflag = 1;
        goal = node->id().toInt();
        node->setAttribute("fillcolor","Blue");
        ui->pathinfo->setHidden(false);
        myscene->applyLayout();

    }



    clickflag = 0;


  }


}

NavWindow::~NavWindow()
{
    delete ui;
}

void NavWindow::on_pushButton_clicked()
{

 clickdisableflag = 1;
 ui->pushButtonclear->hide();
 ui->nodeinfox->setHidden(true);
std::cout<<std::endl;
ui->pathinfo->setHidden(true);
ui->pushButton->hide();
ui->pushButtonx->hide();
ui->nodeinfo->setHidden(true);


 std::vector<int> optpath;
 optpath.clear();
 //int ct = AS->searchingraphrandom(start, goal,optpath);
 start = inode.second;
 int ct = AS->searchingraph(start,goal,optpath);

 if(optpath.at(1)==inode.first){
  std::cout<<"need to reverse"<<std::endl;
  start = inode.second;
  ui->textBrowser->setText(QString("Turning 180"));
   pepper->rotate180();
 }
 else
 {
   optpath.insert(optpath.begin(),inode.first);
   start = inode.first;
   if(ct)  ct=ct+1;
 }

 selectKeyImages(ct, optpath);
 ui->Navigate->show();

}

void NavWindow::selectKeyImages(int ct, std::vector<int> &optpath){

    ui->startButton->hide();
    ui->nodeinfox->setHidden(true);

 int pl = optpath.size();


  std::cout << "Start vertex: " << name[start] << std::endl;
  std::cout << "Goal vertex: " << name[goal] << std::endl;

  nodes[start]->setAttribute("fillcolor","Green");

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
       if(i<ct-1)
        edges[ve[optpath.at(i)][optpath.at(i+1)]]->setAttribute("color","red");
       if(i>0 && i<ct-1)
           nodes[optpath.at(i)]->setAttribute("color","red");
       }
       std::cout<<std::endl;
       myscene->applyLayout();


        kf = tp->setpathkim(optpath);

       showkim(/*kf*/);

       for(int i=0;i<pl-1;i++){
           if(i<ct-1)
            edges[ve[optpath.at(i)][optpath.at(i+1)]]->setAttribute("color","");
           if(i>0 && i<ct-1)
               nodes[optpath.at(i)]->setAttribute("color","");
       }

       int dirf = tp->getdirflag();

 }



 ui->frame->setHidden(true);
 ui->pushButton->setHidden(true);
 clickflag = 0;

   nodes[start]->setAttribute("fillcolor", "white");
   nodes[goal]->setAttribute("fillcolor", "white");
   nodes[inode.second]->setAttribute("color","");
   nodes[inode.first]->setAttribute("color","");
   edges[ve[inode.first][inode.second]]->setAttribute("color","");

   ui->nodeinfox->setHidden(false);
   ui->startButton->setText("Reset");
   ui->startButton->show();
   myscene->applyLayout();

}

void NavWindow::setPointers(astar::astar &as,tgraph::topmapprocessor &TP)
{
AS = &as;
tp = &TP;
obsavoid = tp->getobsavoiodflag();
if(obsavoid){
    cv::Mat Kd = pepper->getKd();
    pn->setfreespacenav(Kd);
}
}

void NavWindow::setPepperRobotPointer(pepperInterface* pr, dispNav* d){
    pepper = pr;
    cv::Mat K = pepper->getK();
    pn = new pepperNavigation(K,d);

}

void NavWindow::showkim(/*kimRead *kf*/)
{
    ui->frame->setHidden(false);

    std::string Kfn;
    cv::Mat imgK;
    kf->resetIndex();
    int foldlen =0;
    CvMat c_img;
    int nkim = kf->kimsize();
    if(nkim){
         ui->textBrowser->setHidden(false);
        std::cout<<"\n Total number of Key Images = "<<kf->kimsize()<<std::endl;
        kf->getkimfold(Kfn);
        foldlen = Kfn.size();
        ui->textBrowser->append(QString("Total Key Images = " + QString::number(nkim)));

    while(!kf->isLast()){
         kf->getnextKeyImage(Kfn);
        // std::cout<<Kfn<<std::endl;
         imgK= cv::imread(Kfn,cv::IMREAD_COLOR);
         c_img = cvMat(imgK);//c_img = imgK;
         imdisp->updateImage(&c_img);

         ui->textBrowser->append(Kfn.substr(foldlen).c_str());
         //cv::imshow("aaa",imgK);
         //cv::waitKey(500*flag);
         QApplication::processEvents();
         QThread::msleep(100);

    }
  //  cv::destroyWindow("kim");
    std::cout<<"Path Found"<<std::endl;
     ui->textBrowser->setHidden(true);
}
 else{
     std::cout<<"\n Destination same as Source"<<std::endl;
      ui->textBrowser->setText(QString("Destination same as Source "));
    }

    ui->textBrowser->setText(QString(" "));
}



void NavWindow::on_pushButtonclear_clicked()
{
     clickflag = 1;
     clickdisableflag = 0;
     ui->pathinfo->setHidden(true);

      nodes[goal]->setAttribute("fillcolor", "white");
     // ui->frame->setHidden(true);
      ui->pushButton->setHidden(true);
      ui->pushButtonx->setHidden(true);
      ui->pushButtonclear->hide();

      myscene->applyLayout();

}

void NavWindow::on_startButton_clicked()
{
    ui->Navigate->hide();
    ui->pushButton->hide();
    ui->pushButtonx->hide();
    ui->pushButtonclear->hide();
    inode.first = 0;
    inode.second = 0;
    ui->frame->setHidden(false);

   // srand(time(NULL));
   // start = rand()%(nNodes-1);*/
  // int kk = rand()%(imlist.size()-1);

   //  std::cout<<kk<<"\t"<<imlist.size()<<std::endl;
    cv::Mat cim;// = cv::imread(imlist.at(kk),cv::IMREAD_GRAYSCALE);
    pepper->getCurrImage(cim);
     CvMat c_img;
     c_img = cvMat(cim); //c_img = cim;
     imdisp->updateImage(&c_img);
     QApplication::processEvents();
     QThread::msleep(5);

    dirinit = tp->localizeintopomap(cim,inode);
    start = inode.second;
   // if(inode.first <0)
        nodes[inode.second]->setAttribute("color","Cyan");
   // else {
        nodes[inode.first]->setAttribute("color","Magenta");
        edges[ve[inode.first][inode.second]]->setAttribute("color","Cyan");

   // }

    myscene->applyLayout();
   //  ui->frame->setHidden(true);
    ui->startButton->hide();
    ui->nodeinfo->setHidden(false);
     clickflag = 1;
      clickdisableflag = 0;

    runcount++;

     std::cout<<"\n \n OBSAVOID = "<<obsavoid<<"\n"<<std::endl;

    if(pepper->getmode()==1)
        navigateoffline();

}

void NavWindow::navigateoffline(){



          clickdisableflag = 1;
          goal = pepper->getid();
          nodes[goal]->setAttribute("fillcolor","Blue");
          myscene->applyLayout();
          clickflag = 0;
          ui->textBrowser->hide();
          ui->pathinfo->hide();
          ui->pushButton->hide();
          ui->pushButtonx->hide();
          ui->nodeinfo->hide();
          ui->nodeinfox->hide();

         // setKimforNavigation();
          if(kf != nullptr)
              delete kf;
          kf = new kimRead(kimfold);

          ui->nodeinfo->hide();
          ui->nodeinfox->hide();
          ui->pathinfo->hide();
           obsavoid = 0;

          performNavigation();

}

void NavWindow::on_pushButtonx_clicked()
{
    clickdisableflag = 1;
    ui->pushButtonclear->hide();
    ui->nodeinfox->setHidden(true);
   std::cout<<std::endl;
   ui->pathinfo->setHidden(true);
   ui->pushButton->hide();
   ui->pushButtonx->hide();
   ui->nodeinfo->setHidden(true);

   setKimforNavigation();

   ui->nodeinfox->setHidden(false);

  /*  nodes[goal]->setAttribute("fillcolor","Blue");
    nodes[inode.second]->setAttribute("color","Cyan");

    nodes[inode.first]->setAttribute("color","Magenta");
    edges[ve[inode.first][inode.second]]->setAttribute("color","Cyan");

// }

myscene->applyLayout();

    ui->pushButton->show();
    ui->startButton->hide();
    ui->nodeinfox->hide();*/
    ui->Navigate->show();
}

void NavWindow::setKimforNavigation(){

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
     // ui->textBrowser->setText(QString("Turning 180"));
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


     //nodes[goal]->setAttribute("fillcolor", "blue");
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
            ui->textBrowser->setText(QString("Turning 180"));
            pepper->rotate180();
        }
        selectKeyImages(cts, optpath_sn);
    }
    else{
        if(revflage){
            std::cout<<"need to reverse"<<std::endl;
            ui->textBrowser->setText(QString("Turning 180"));
             pepper->rotate180();
        }
        start = start_en;
        selectKeyImages(cte, optpath_en);
    }



}


void NavWindow::on_Navigate_clicked()
{
 ui->Navigate->hide();
 ui->startButton->setText("START");
 ui->startButton->hide();
 performNavigation();
 ui->Navigate->hide();
 ui->startButton->setText("START");
 ui->startButton->show();
}

void NavWindow::performNavigation(){

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


            cv::cvtColor(cim,cim,cv::COLOR_BGR2GRAY);

            rvL = pn->navigate(cim);


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
                        pn->setcurrentpose(tt);
                         rvfs = pn->getvelfreespace(Id);
                         H = pn->getH();

                        if(H<0) {sv = 0.0; H=0.0;}
                        if(fabs(rvfs)<0.0001f ) H = 0.0;
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
           // fv =0; rv=0;

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
               pn->closedisp();
              break;
            }
             ofilepose<<setw(5)<<kk<<"\t"<<setw(10)<<tt[0]<<"\t"<<setw(10)<<tt[1]<<"\t"<<setw(10)<<tt[2]<<"\t"<<setw(10)<<rv<<"\t"<<setw(10)<<fv<<"\t"<<setw(10)<<turnval<<"\t"<<setw(10)<<H<<"\t"<<setw(10)<<rvL<<"\t"<<setw(10)<<rvfs<<std::endl;
             std::cout<<"---------------------------------------------"<<std::endl;
            std::cout<<"\n exec time = "<<((double(clock() - tStart))/CLOCKS_PER_SEC)<<std::endl;

        }

         pepper->setBaseVelocities(0.0,0.0);
          ofilepose.close();


    }


}


void NavWindow::setKimfoldoffline(std::string kif){
    kimfold = kif;
    std::cout<<"Kimfold = "<<kimfold<<std::endl;

}
