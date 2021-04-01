/* Header file for performing A* search in the graph
 * /
//=======================================================================
// Copyright (c) 2004 Kristopher Beevers
//
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt)
//=======================================================================
//
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
 *  Suman Raj Bista
 * code modified from https://www.boost.org/doc/libs/1_54_0/libs/graph/example/astar-cities.cpp
 */


#ifndef ASTAR_H
#define ASTAR_H

#include <boost/graph/astar_search.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random.hpp>
#include <boost/graph/graphviz.hpp>
#include <ctime>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <math.h>    // for sqrt

namespace astar{
// auxiliary types
struct location
{
  float y, x; // lat, long
};
typedef float cost;
typedef const char* node;
template <class Name, class LocMap>
class graph_writer {
public:
  graph_writer(Name n, LocMap l, float _minx, float _maxx, float _miny, float _maxy, unsigned int _ptx, unsigned int _pty)
    : name(n), loc(l), minx(_minx), maxx(_maxx), miny(_miny), maxy(_maxy), ptx(_ptx), pty(_pty) {}

  template <class Vertex>
  void operator()(std::ostream& out, const Vertex& v) const {
    float px = 1 - (loc[v].x - minx) / (maxx - minx);
    float py = (loc[v].y - miny) / (maxy - miny);
    out << "[label=\"" << name[v] << "\", pos=\""
        << static_cast<unsigned int>(ptx * px) << ","
        << static_cast<unsigned int>(pty * py)
        << "\", fontsize=\"11\"]";
  }
private:
  Name name;
  LocMap loc;
  float minx, maxx, miny, maxy;
  unsigned int ptx, pty;
};

template <class WeightMap>
class weight_writer {
public:
  weight_writer(WeightMap w) : wm(w) {}
  template <class Edge>
  void operator()(std::ostream &out, const Edge& e) const {
    out << "[label=\"" << wm[e] << "\", fontsize=\"11\"]";
  }
private:
  WeightMap wm;
};

// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class distance_heuristic : public boost::astar_heuristic<Graph, CostType>
{
public:
  typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
  distance_heuristic(LocMap l, Vertex goal)
    : m_location(l), m_goal(goal) {}
  CostType operator()(Vertex u)
  {
    CostType dx = m_location[m_goal].x - m_location[u].x;
    CostType dy = m_location[m_goal].y - m_location[u].y;
    return ::sqrt(dx * dx + dy * dy);
  }
private:
  LocMap m_location;
  Vertex m_goal;
};

// euclidean distance heuristic
template <class Graph, class CostType, class LocMap>
class heuristic : public boost::astar_heuristic<Graph, CostType>
{
public:
  typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
  heuristic(LocMap l, Vertex goal)
    : m_location(l), m_goal(goal) {}
  CostType operator()(Vertex u)
  {
    CostType dx = m_location[m_goal].x - m_location[u].x;
    CostType dy = m_location[m_goal].y - m_location[u].y;
    //return ::sqrt(dx * dx + dy * dy);
    return(abs(dx)+abs(dy));
  }
private:
  LocMap m_location;
  Vertex m_goal;
};



struct found_goal {}; // exception for termination

// visitor that terminates when we find the goal
template <class Vertex>
class astar_goal_visitor : public boost::default_astar_visitor
{
public:
  astar_goal_visitor(Vertex goal) : m_goal(goal) {}
  template <class Graph>
  void examine_vertex(Vertex u, Graph& g) {
    if(u == m_goal)
      throw found_goal();
  }
private:
  Vertex m_goal;
};



typedef boost::adjacency_list<boost::listS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, cost> > mygraph_t;
typedef boost::property_map<mygraph_t, boost::edge_weight_t>::type WeightMap;
typedef mygraph_t::vertex_descriptor vertex;
typedef mygraph_t::edge_descriptor edge_descriptor;
typedef std::pair<int, int> edge;


/**
 * @brief The astar class
 */
class astar{

private:
     mygraph_t g;
     WeightMap weightmap;
     location *locations;
      edge *edge_array;
      unsigned int num_edges ;
      cost *weights;
      vertex start ;
      vertex goal;
      node *node_array;
       unsigned int N;


/**
 * @brief astarsearch perofrm A* search
 * @param optpath list of nodes in optimum path
 * @return cost of the optimum path
 */
int astarsearch(std::vector<int> &optpath ) {
    std::vector<mygraph_t::vertex_descriptor> p(num_vertices(g));
    std::vector<cost> d(num_vertices(g));
    try {
      // call astar named parameter interface

      boost::astar_search_tree
        (g, start,
         heuristic<mygraph_t, cost, location*>
          (locations, goal),
         boost::predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).
         distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))).
         visitor(astar_goal_visitor<vertex>(goal)));


    } catch(found_goal fg) { // found a path to the goal
      std::list<vertex> shortest_path;
      for(vertex v = goal;; v = p[v]) {
        shortest_path.push_front(v);
        if(p[v] == v)
          break;
      }


      std::list<vertex>::iterator spi;
      int ct = 0;
      for( spi = shortest_path.begin(); spi != shortest_path.end(); ++spi){
          optpath.push_back(*spi);
          ct++;

      }

      optpath.push_back(d[goal]*100);

      return ct;
    }


    return 0;


}
public:

/**
* @brief astar initilaise graph
* @param num_nodes number of nodes
*/

 astar(int num_nodes){
   N = num_nodes;
   g =  mygraph_t(N);
   weightmap = boost::get(boost::edge_weight, g);
   locations = new location[N];
   node_array = new node[N];

    }

 /**
 * @brief getweight return weight of the edge
 * @param indx index of edge
 * @return weight
 */

float getweight(int indx){
    return weights[indx];
    }

/**
  * @brief setlocationherustics
  */
 void setlocationherustics(node *nodename,location *locat){
     for(int i=0;i<N;i++){
       locations[i].x = locat[i].x;
       locations[i].y = locat[i].y;
       node_array[i]= nodename[i];
     }
     }

 /**
  * @brief setedges set edges of the graph
  * @param numedges number of the edges
  * @param edgearray egdes information
  * @param wts weights of edges
  */

 void setedges(int numedges, edge *edgearray, cost *wts){
    num_edges = numedges;
    weights = new cost[num_edges];
    edge_array = new edge[num_edges];
     for(std::size_t j = 0; j < num_edges; ++j) {
        weights[j] = wts[j];
        edge_array[j] = edgearray[j];

     }
    }

 /**
  * @brief creategraph creates toplogical graph from edges
  */

 void creategraph(){
     for(std::size_t j = 0; j < num_edges; ++j) {
       edge_descriptor e; bool inserted;
       boost::tie(e, inserted) = boost::add_edge(edge_array[j].first,
                                          edge_array[j].second, g);

        weightmap[e] = weights[j];
     }
    }


 /**
 * @brief searchingraph A* search betwwn start node and destination node
 * @param st index of start node
 * @param gl index of destination node
 * @param optpath node list of the optimum path
 * @return cost of the optimum path
 */

int searchingraph(int st, int gl, std::vector<int> &optpath ){

    optpath.clear();
    typename boost::graph_traits<mygraph_t>::vertex_iterator i = boost::vertices(g).first;

   start = *(boost::next(i, st));
    goal = *(boost::next(i, gl));
     return astarsearch(optpath);
    }


/**
 * @brief searchingraphrandom A* search with random start and random destination
 * @param st start node index => generated randomly
 * @param gl end node index => generated randomly
 * @param optpath node list of the optimum path
 * @return cost of the optimum path
 */

int searchingraphrandom(int &st, int &gl, std::vector<int> &optpath){
    optpath.clear();
    do{
       boost::mt19937 gen(std::time(0));
       start = boost::random_vertex(g, gen);
       goal = boost::random_vertex(g, gen);
       st = start;
       gl = goal;
    }while(st==gl);

    return astarsearch(optpath);
    }


/**
 * @brief writegraph2file wirte graph in file in .dot format
 * @param fname file name
 */

void writegraph2file(std::string &fname){
    std::ofstream dotfile;
    dotfile.open(fname);
    boost::write_graphviz(dotfile,g,
                      graph_writer<node*, location*>
                      (node_array, locations, -1, 3, -1, 3,
                       1000, 750),
                       weight_writer<WeightMap>(weightmap));
    }

};
}
#endif // ASTAR_H
