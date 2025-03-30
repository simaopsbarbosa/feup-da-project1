// Original code by Gonçalo Leão
// Updated by DA 2024/2025 Team
// Modified for DA2025_PRJ1_T13_G07

#ifndef DA_TP_CLASSES_GRAPH
#define DA_TP_CLASSES_GRAPH

#include "../data-structures/MutablePriorityQueue.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>

template <class T> class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T> class Vertex {
public:
  /**
   * @brief Constructor for the Vertex class.
   * @param in Information to store in the vertex.
   */
  Vertex(T in);

  /**
   * @brief Compares two vertices based on their driving distance.
   * @param vertex Vertex to compare with.
   * @return True if this vertex has a smaller driving distance, false
   * otherwise.
   * @complexity O(1)
   */
  bool operator<(Vertex<T> &vertex) const; // required by MutablePriorityQueue

  /**
   * @brief Gets the information stored in the vertex.
   * @return Information stored in the vertex.
   */
  T getInfo() const;

  /**
   * @brief Gets the adjacency list of the vertex.
   * @return Adjacency list of the vertex.
   */
  std::vector<Edge<T> *> getAdj() const;

  bool isVisited() const;
  bool isProcessing() const;
  unsigned int getIndegree() const;
  double getDrivingDist() const;
  double getWalkingDist() const;
  Edge<T> *getPath() const;
  std::vector<Edge<T> *> getIncoming() const;

  void setInfo(T info);
  void setVisited(bool visited);
  void setProcessing(bool processing);

  int getLow() const;
  void setLow(int value);
  int getNum() const;
  void setNum(int value);

  void setIndegree(unsigned int indegree);
  void setDrivingDist(double dd);
  void setWalkingDist(double wd);
  void setPath(Edge<T> *path);
  void setQueueIndex(int idx);

  /**
   * @brief Adds an edge from this vertex to the destination vertex.
   * This operation has constant time complexity.
   * @param dest Destination vertex.
   * @param dw Driving weight of the edge.
   * @param ww Walking weight of the edge.
   * @return Pointer to the created edge.
   */
  Edge<T> *addEdge(Vertex<T> *dest, double dw, double ww);

  /**
   * @brief Removes an edge from this vertex to a vertex with the given info.
   * This operation has a time complexity of O(E), where E is the number of
   * edges in the adjacency list.
   * @param in Info of the destination vertex.
   * @return True if the edge was removed, false otherwise.
   */
  bool removeEdge(T in);

  void removeOutgoingEdges();
  int getQueueIndex() const;

  friend class MutablePriorityQueue<Vertex>;

protected:
  T info;                     // info node
  std::vector<Edge<T> *> adj; // outgoing edges

  // auxiliary fields
  bool visited = false; // used by DFS, BFS, Prim ...
  bool processing =
      false; // used by isDAG (in addition to the visited attribute)
  int low = -1, num = -1; // used by SCC Tarjan
  unsigned int indegree;  // used by topsort
  double walkingDist = 0;
  double drivingDist = 0;
  Edge<T> *path = nullptr;

  std::vector<Edge<T> *> incoming; // incoming edges

  int queueIndex = 0; // required by MutablePriorityQueue and UFDS

  void deleteEdge(Edge<T> *edge);
};

/********************** Edge  ****************************/

template <class T> class Edge {
public:
  /**
   * @brief Constructor for the Edge class.
   * @param orig Origin vertex of the edge.
   * @param dest Destination vertex of the edge.
   * @param dw Driving weight of the edge.
   * @param ww Walking weight of the edge.
   */
  Edge(Vertex<T> *orig, Vertex<T> *dest, double dw, double ww);

  /**
   * @brief Gets the destination vertex of the edge.
   * @return Destination vertex of the edge.
   */
  Vertex<T> *getDest() const;

  double getWalkingWeight() const;
  double getDrivingWeight() const;
  bool isSelected() const;
  Vertex<T> *getOrig() const;
  Edge<T> *getReverse() const;
  double getFlow() const;

  void setSelected(bool selected);
  void setReverse(Edge<T> *reverse);
  void setFlow(double flow);

protected:
  Vertex<T> *dest;      // destination vertex
  double drivingWeight; // driving edge weight (time)
  double walkingWeight; // walking edge weight (time)

  // auxiliary fields
  bool selected = false;

  // used for bidirectional edges
  Vertex<T> *orig;
  Edge<T> *reverse = nullptr;

  double flow; // for flow-related problems
};

/********************** Graph  ****************************/

template <class T> class Graph {
public:
  ~Graph();

  /**
   * @brief Finds a vertex in the graph by its info.
   * This operation has a time complexity of O(V), where V is the number of
   * vertices in the graph.
   * @param in Info of the vertex to find.
   * @return Pointer to the vertex if found, nullptr otherwise.
   */
  Vertex<T> *findVertex(const T &in) const;

  Vertex<T> *findVertexByCode(const std::string code) const;
  Vertex<T> *findVertexById(const int id) const;

  /**
   * @brief Adds a vertex to the graph.
   * This operation has a time complexity of O(V), where V is the number of
   * vertices in the graph.
   * @param in Info of the vertex to add.
   * @return True if the vertex was added, false if it already exists.
   */
  bool addVertex(const T &in);

  bool removeVertex(const T &in);

  /**
   * @brief Adds a directed edge between two vertices.
   * This operation has a time complexity of O(V), where V is the number of
   * vertices in the graph.
   * @param sourc Info of the source vertex.
   * @param dest Info of the destination vertex.
   * @param dw Driving weight of the edge.
   * @param ww Walking weight of the edge.
   * @return True if the edge was added, false otherwise.
   */
  bool addEdge(const T &sourc, const T &dest, double dw, double ww);

  bool removeEdge(const T &source, const T &dest);
  bool addBidirectionalEdge(const T &sourc, const T &dest, double dw,
                            double ww);
  Edge<T> *findEdge(int sourceId, int destId);

  /**
   * @brief Gets the number of vertices in the graph.
   * @return Number of vertices in the graph.
   */
  int getNumVertex() const;

  /**
   * @brief Gets the vertex set of the graph.
   * @return Vertex set of the graph.
   */
  std::vector<Vertex<T> *> getVertexSet() const;

protected:
  std::vector<Vertex<T> *> vertexSet; // vertex set

  double **distMatrix = nullptr; // dist matrix for Floyd-Warshall
  int **pathMatrix = nullptr;    // path matrix for Floyd-Warshall

  int findVertexIdx(const T &in) const;
};

/**
 * @brief Deletes a dynamically allocated matrix.
 * This operation has a time complexity of O(n), where n is the number of rows
 * in the matrix.
 * @param m Pointer to the matrix.
 * @param n Number of rows in the matrix.
 */
void deleteMatrix(int **m, int n);

/**
 * @brief Deletes a dynamically allocated matrix.
 * This operation has a time complexity of O(n), where n is the number of rows
 * in the matrix.
 * @param m Pointer to the matrix.
 * @param n Number of rows in the matrix.
 */
void deleteMatrix(double **m, int n);

/************************* Vertex  **************************/

template <class T> Vertex<T>::Vertex(T in) : info(in) {}

template <class T> bool Vertex<T>::operator<(Vertex<T> &vertex) const {
  return this->drivingDist < vertex.drivingDist;
}

template <class T> T Vertex<T>::getInfo() const { return this->info; }

template <class T> std::vector<Edge<T> *> Vertex<T>::getAdj() const {
  return this->adj;
}

template <class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *d, double dw, double ww) {
  auto newEdge = new Edge<T>(this, d, dw, ww);
  adj.push_back(newEdge);
  d->incoming.push_back(newEdge);
  return newEdge;
}

template <class T> bool Vertex<T>::removeEdge(T in) {
  bool removedEdge = false;
  auto it = adj.begin();
  while (it != adj.end()) {
    Edge<T> *edge = *it;
    Vertex<T> *dest = edge->getDest();
    if (dest->getInfo() == in) {
      it = adj.erase(it);
      deleteEdge(edge);
      removedEdge = true; // allows for multiple edges to connect the same pair
                          // of vertices (multigraph)
    } else {
      it++;
    }
  }
  return removedEdge;
}

template <class T> void Vertex<T>::removeOutgoingEdges() {
  auto it = adj.begin();
  while (it != adj.end()) {
    Edge<T> *edge = *it;
    it = adj.erase(it);
    deleteEdge(edge);
  }
}

template <class T> bool Vertex<T>::isVisited() const { return this->visited; }

template <class T> bool Vertex<T>::isProcessing() const {
  return this->processing;
}

template <class T> unsigned int Vertex<T>::getIndegree() const {
  return this->indegree;
}

template <class T> double Vertex<T>::getWalkingDist() const {
  return this->walkingDist;
}

template <class T> double Vertex<T>::getDrivingDist() const {
  return this->drivingDist;
}

template <class T> Edge<T> *Vertex<T>::getPath() const { return this->path; }

template <class T> std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
  return this->incoming;
}

template <class T> void Vertex<T>::setInfo(T in) { this->info = in; }

template <class T> void Vertex<T>::setVisited(bool visited) {
  this->visited = visited;
}

template <class T> void Vertex<T>::setProcessing(bool processing) {
  this->processing = processing;
}

template <class T> void Vertex<T>::setIndegree(unsigned int indegree) {
  this->indegree = indegree;
}

template <class T> void Vertex<T>::setDrivingDist(double dd) {
  this->drivingDist = dd;
}

template <class T> void Vertex<T>::setWalkingDist(double wd) {
  this->walkingDist = wd;
}

template <class T> void Vertex<T>::setPath(Edge<T> *path) { this->path = path; }

template <class T> void Vertex<T>::deleteEdge(Edge<T> *edge) {
  Vertex<T> *dest = edge->getDest();
  auto it = dest->incoming.begin();
  while (it != dest->incoming.end()) {
    if ((*it)->getOrig()->getInfo() == info) {
      it = dest->incoming.erase(it);
    } else {
      it++;
    }
  }
  delete edge;
}

template <class T> int Vertex<T>::getQueueIndex() const {
  return this->queueIndex;
}

template <class T> void Vertex<T>::setQueueIndex(int idx) {
  this->queueIndex = idx;
}

/********************** Edge  ****************************/

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double dw, double ww)
    : orig(orig), dest(dest), drivingWeight(dw), walkingWeight(ww) {}

template <class T> Vertex<T> *Edge<T>::getDest() const { return this->dest; }

template <class T> double Edge<T>::getWalkingWeight() const {
  return this->walkingWeight;
}

template <class T> double Edge<T>::getDrivingWeight() const {
  return this->drivingWeight;
}

template <class T> Vertex<T> *Edge<T>::getOrig() const { return this->orig; }

template <class T> Edge<T> *Edge<T>::getReverse() const {
  return this->reverse;
}

template <class T> bool Edge<T>::isSelected() const { return this->selected; }

template <class T> double Edge<T>::getFlow() const { return flow; }

template <class T> void Edge<T>::setSelected(bool selected) {
  this->selected = selected;
}

template <class T> void Edge<T>::setReverse(Edge<T> *reverse) {
  this->reverse = reverse;
}

template <class T> void Edge<T>::setFlow(double flow) { this->flow = flow; }

/********************** Graph  ****************************/

template <class T> int Graph<T>::getNumVertex() const {
  return vertexSet.size();
}

template <class T> std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
  return vertexSet;
}

template <class T> Vertex<T> *Graph<T>::findVertex(const T &in) const {
  for (auto v : vertexSet)
    if (v->getInfo() == in)
      return v;
  return nullptr;
}

template <class T>
Vertex<T> *Graph<T>::findVertexByCode(const std::string code) const {
  for (auto v : vertexSet)
    if (v->getInfo().code == code)
      return v;
  return nullptr;
}

template <class T> Vertex<T> *Graph<T>::findVertexById(const int id) const {
  for (auto v : vertexSet)
    if (v->getInfo().id == id)
      return v;
  return nullptr;
}

template <class T> int Graph<T>::findVertexIdx(const T &in) const {
  for (unsigned i = 0; i < vertexSet.size(); i++)
    if (vertexSet[i]->getInfo() == in)
      return i;
  return -1;
}

template <class T> bool Graph<T>::addVertex(const T &in) {
  if (findVertex(in) != nullptr)
    return false;
  vertexSet.push_back(new Vertex<T>(in));
  return true;
}

template <class T> bool Graph<T>::removeVertex(const T &in) {
  for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
    if ((*it)->getInfo() == in) {
      auto v = *it;
      v->removeOutgoingEdges();
      for (auto u : vertexSet) {
        u->removeEdge(v->getInfo());
      }
      vertexSet.erase(it);
      delete v;
      return true;
    }
  }
  return false;
}

template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double dw, double ww) {
  auto v1 = findVertex(sourc);
  auto v2 = findVertex(dest);
  if (v1 == nullptr || v2 == nullptr)
    return false;
  v1->addEdge(v2, dw, ww);
  return true;
}

template <class T> bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
  Vertex<T> *srcVertex = findVertex(sourc);
  if (srcVertex == nullptr) {
    return false;
  }
  return srcVertex->removeEdge(dest);
}

template <class T> Edge<T> *Graph<T>::findEdge(int sourceId, int destId) {
  auto sourceVertex = findVertexById(sourceId);
  if (!sourceVertex)
    return nullptr;

  for (auto &edge : sourceVertex->getAdj()) {
    if (edge->getDest()->getInfo().id == destId) {
      return edge;
    }
  }
  return nullptr;
}

template <class T>
bool Graph<T>::addBidirectionalEdge(const T &sourc, const T &dest, double dw,
                                    double ww) {
  auto v1 = findVertex(sourc);
  auto v2 = findVertex(dest);
  if (v1 == nullptr || v2 == nullptr)
    return false;
  auto e1 = v1->addEdge(v2, dw, ww);
  auto e2 = v2->addEdge(v1, dw, ww);
  e1->setReverse(e2);
  e2->setReverse(e1);
  return true;
}

inline void deleteMatrix(int **m, int n) {
  if (m != nullptr) {
    for (int i = 0; i < n; i++)
      if (m[i] != nullptr)
        delete[] m[i];
    delete[] m;
  }
}

inline void deleteMatrix(double **m, int n) {
  if (m != nullptr) {
    for (int i = 0; i < n; i++)
      if (m[i] != nullptr)
        delete[] m[i];
    delete[] m;
  }
}

template <class T> Graph<T>::~Graph() {
  deleteMatrix(distMatrix, vertexSet.size());
  deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */