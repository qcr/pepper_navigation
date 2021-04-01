#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QGVScene.h"
#include "QGVNode.h"
#include "QGVEdge.h"
#include "QGVSubGraph.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     clickflag = 0;
     clickdisableflag = 0;
   // scene = new QGraphicsScene(this);
   // ui->graphicsView->setScene(scene);

    myscene = new QGVScene(QString("TopNavPepper"),this);
    //myscene = new QGVScene("TopNavPepper", std::string("/home/suman/soft/pepper_ws/topmanav/build/S11.dot"),this);
    ui->graphicsView->setScene(myscene);
    ui->pushButton->hide();

    ui->pushButtonclear->hide();
    connect(myscene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
    imdisp = new cvqt::CvWindow(QString("cvWindow"), CV_WINDOW_NORMAL|CV_GUI_NORMAL|CV_WINDOW_FREERATIO);
    imdisp->setGeometry(0,0,320,240);
    ui->frame->setGeometry(this->width()-5-320,10,320,240);
    ui->textBrowser->setHidden(true);
    imdisp->setParent(ui->frame);
    ui->frame->setHidden(true);
    start = 0;
    goal = 0;
    ui->pathinfo->setHidden(true);
}

void MainWindow::drawGraphTest(){
    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(2);

    rectangle = scene->addRect(100, 0, 80, 100, outlinePen, blueBrush);

    // addEllipse(x,y,w,h,pen,brush)
    ellipse = scene->addEllipse(0, -100, 300, 60, outlinePen, greenBrush);

    text = scene->addText("bogotobogo.com", QFont("Arial", 20) );
    // movable text
    text->setFlag(QGraphicsItem::ItemIsMovable);

}
void MainWindow::showGraph()
{


    //Layout sceneQString\\

   QString fname("S11.dot");
   myscene->readLayout(fname);




    //Fit in view
    ui->graphicsView->fitInView(myscene->sceneRect(), Qt::KeepAspectRatio);
    ui->graphicsView->rotate(90);
}
void MainWindow::drawGraph()
{
//myscene->loadLayout()

    myscene->setGraphAttribute("label", "PEPPER");

    myscene->setGraphAttribute("splines", "curved");
    myscene->setGraphAttribute("rankdir", "LR");
   /// myscene->setGraphAttribute("concentrate", "true"); //Error !
    myscene->setGraphAttribute("nodesep", "1");
  myscene->setGraphAttribute("rank","same");
   myscene->setGraphAttribute("ranksep","equal");

    myscene->setNodeAttribute("shape", "circle");
    myscene->setNodeAttribute("style", "filled");
    myscene->setNodeAttribute("fillcolor", "white");
    myscene->setNodeAttribute("height", "1.25");
    myscene->setEdgeAttribute("minlen", "3");
    myscene->setEdgeAttribute("dir", "both");


    enum nodes {
              PepperHome, Harvey, Rob1,
                Peter,
                ROb2, Cartman,
                Belinda,
                Manipulation, Robotics,
               // Cor,Suman,
                N
            };
            const char *name[] = {
              "PepperHome", "Harvey", "Rob1", "Peter",
              "ROb2", "Cartman", "Belinda", "Manipulation", "Robotics"
              // ,"Cor","Suman"
            };


           typedef std::pair<int, int> edge;

           edge edge_array[] = {
               edge(PepperHome,Harvey), edge(Harvey,Rob1),
               edge(Rob1,Peter), edge(Peter,ROb2),
               edge(ROb2,Cartman), edge(Cartman,Belinda),
              // edge(Cartman,Cor),
               edge(Belinda,PepperHome),
               edge(Harvey,Manipulation), edge(Manipulation,Cartman),
               edge(Rob1,Robotics), edge(Robotics,ROb2),
              // edge(Cor,Suman), edge(Suman,Belinda)

            };

      unsigned int num_edges = sizeof(edge_array) / sizeof(edge);

        //Add some nodes
          std::vector<QGVNode*> nodes;
          nodes.reserve(N);

          for(int i=0;i<N;i++)
          {
              nodes[i] = myscene->addNode(name[i]);
          }


       for(int i=0;i<num_edges;i++){
           myscene->addEdge(nodes[edge_array[i].first],nodes[edge_array[i].second]);

       }





    //Layout scene
    myscene->applyLayout();

    //Fit in view
  ui->graphicsView->fitInView(myscene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::setGraphAttributes()
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


void MainWindow::drawGraph(int N, astar::node *node_array, int num_edges, astar::edge *edge_array,astar::cost *wts)
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
myscene->applyLayout();

//Fit in view
ui->graphicsView->fitInView(myscene->sceneRect(), Qt::KeepAspectRatio);

std::cout<<std::endl;

}



void MainWindow::nodeDoubleClick(QGVNode *node)
{

  if(!clickdisableflag){

    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
     std::cout<<"Node "<<node->id().toInt()<<" with label "<<node->label().toStdString()<<" is selected"<<std::endl;

    if(clickflag){

        ui->pushButton->show();

        ui->pushButtonclear->show();
        clickdisableflag = 1;
        goal = node->id().toInt();
        node->setAttribute("fillcolor","Blue");
        ui->pathinfo->setHidden(false);

    }
    else{
        ui->pushButton->hide();

        ui->pushButtonclear->hide();
        start = node->id().toInt();
        node->setAttribute("fillcolor","Green");

    }

    clickflag = !clickflag;
    myscene->applyLayout();

  }


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
 ui->pushButtonclear->hide();
std::cout<<std::endl;
ui->pathinfo->setHidden(true);
ui->pushButton->hide();

ui->nodeinfo->setHidden(true);
 std::vector<int> optpath;
 //int ct = AS->searchingraphrandom(start, goal,optpath);
 int ct = AS->searchingraph(start,goal,optpath);
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
       std::cout<<std::endl;
       if(i<ct-1)
        edges[ve[optpath.at(i)][optpath.at(i+1)]]->setAttribute("color","red");
       if(i>0 && i<ct-1)
           nodes[optpath.at(i)]->setAttribute("color","red");
       }
       myscene->applyLayout();


      kimRead *kf = tp->setpathkim(optpath);

       showkim(kf);

       for(int i=0;i<pl-1;i++){
           if(i<ct-1)
            edges[ve[optpath.at(i)][optpath.at(i+1)]]->setAttribute("color","");
           if(i>0 && i<ct-1)
               nodes[optpath.at(i)]->setAttribute("color","");
       }

 }



 ui->frame->setHidden(true);
 ui->pushButton->setHidden(true);

 clickflag = 0;
 clickdisableflag = 0;
   nodes[start]->setAttribute("fillcolor", "white");
   nodes[goal]->setAttribute("fillcolor", "white");
ui->nodeinfo->setHidden(false);

   myscene->applyLayout();

}

void MainWindow::setPointers(astar::astar &as,tgraph::topmapprocessor &TP)
{
AS = &as;
tp = &TP;
}

void MainWindow::showkim(kimRead *kf)
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
        ui->textBrowser->setText(QString("Total Key Images = " + QString::number(nkim)));

    while(!kf->isLast()){
         kf->getnextKeyImage(Kfn);
         std::cout<<Kfn<<std::endl;
         imgK= cv::imread(Kfn,cv::IMREAD_COLOR);
         c_img = imgK;
         imdisp->updateImage(&c_img);

         ui->textBrowser->append(Kfn.substr(foldlen).c_str());
         //cv::imshow("aaa",imgK);
         //cv::waitKey(500*flag);
         QApplication::processEvents();
         QThread::msleep(500);

    }
  //  cv::destroyWindow("kim");
 std::cout<<"Path Found"<<std::endl;
     ui->textBrowser->setHidden(true);
}
 else{
     std::cout<<"\n Destination same as Source"<<std::endl;
    }
}



void MainWindow::on_pushButtonclear_clicked()
{
     clickflag = 0;
     clickdisableflag = 0;
     ui->pathinfo->setHidden(true);
      nodes[start]->setAttribute("fillcolor", "white");
      nodes[goal]->setAttribute("fillcolor", "white");
      ui->frame->setHidden(true);
      ui->pushButton->setHidden(true);
      ui->pushButtonclear->hide();

      myscene->applyLayout();

}

