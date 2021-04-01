#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "topograph_astar.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "QGVScene.h"
#include "topograph_processor.h"
#include "window_QT.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void drawGraphTest();
    void drawGraph();
    void showGraph();
    void drawGraph(int N, astar::node *node_array, int num_edges, astar::edge *edge_array, astar::cost *wts);
    void setPointers(astar::astar &as,tgraph::topmapprocessor &TP);
private slots:
    void nodeDoubleClick(QGVNode* node);
     void on_pushButton_clicked();

     void on_pushButtonclear_clicked();


private:
    Ui::MainWindow *ui;

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
    void showkim(kimRead *kf);
    void setGraphAttributes();
    QString nlab;
    int start;
    int goal;
    int **ve;
};

#endif // MAINWINDOW_H
