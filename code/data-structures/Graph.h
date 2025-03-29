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
  Vertex(T in);
  bool operator<(Vertex<T> &vertex) const; // required by MutablePriorityQueue

  T getInfo() const;
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
  Edge<T> *addEdge(Vertex<T> *dest, double dw, double ww);
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
  Edge(Vertex<T> *orig, Vertex<T> *dest, double dw, double ww);

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
  Vertex<T> *findVertex(const T &in) const;
  Vertex<T> *findVertexByCode(const std::string code) const;
  Vertex<T> *findVertexById(const int id) const;

  bool addVertex(const T &in);
  bool removeVertex(const T &in);

  bool addEdge(const T &sourc, const T &dest, double dw, double ww);
  bool removeEdge(const T &source, const T &dest);
  bool addBidirectionalEdge(const T &sourc, const T &dest, double dw,
                            double ww);
  Edge<T> *findEdge(int sourceId, int destId);

  int getNumVertex() const;

  std::vector<Vertex<T> *> getVertexSet() const;

protected:
  std::vector<Vertex<T> *> vertexSet; // vertex set

  double **distMatrix = nullptr; // dist matrix for Floyd-Warshall
  int **pathMatrix = nullptr;    // path matrix for Floyd-Warshall

  int findVertexIdx(const T &in) const;
};

void deleteMatrix(int **m, int n);
void deleteMatrix(double **m, int n);

/************************* Vertex  **************************/

/**
 * @brief Constructor for the Vertex class.
 * @param in Information to store in the vertex.
 */
template <class T> Vertex<T>::Vertex(T in) : info(in) {}

/**
 * @brief Adds an edge from this vertex to the destination vertex.
 * This operation has constant time complexity.
 * @param d Destination vertex.
 * @param dw Driving weight of the edge.
 * @param ww Walking weight of the edge.
 * @return Pointer to the created edge.
 */
template <class T>
Edge<T> *Vertex<T>::addEdge(Vertex<T> *d, double dw, double ww) {
  auto newEdge = new Edge<T>(this, d, dw, ww);
  adj.push_back(newEdge);
  d->incoming.push_back(newEdge);
  return newEdge;
}

/**
 * @brief Removes an edge from this vertex to a vertex with the given info.
 * This operation has a time complexity of O(E), where E is the number of edges
 * in the adjacency list.
 * @param in Info of the destination vertex.
 * @return True if the edge was removed, false otherwise.
 */
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

/**
 * @brief Compares two vertices based on their driving distance.
 * @param vertex Vertex to compare with.
 * @return True if this vertex has a smaller driving distance, false otherwise.
 * @complexity O(1)
 */
template <class T> bool Vertex<T>::operator<(Vertex<T> &vertex) const {
  return this->drivingDist < vertex.drivingDist;
}

/**
 * @brief Removes all outgoing edges from this vertex.
 * This operation has a time complexity of O(E), where E is the number of
 * outgoing edges.
 */
template <class T> void Vertex<T>::removeOutgoingEdges() {
  auto it = adj.begin();
  while (it != adj.end()) {
    Edge<T> *edge = *it;
    it = adj.erase(it);
    deleteEdge(edge);
  }
}

/**
 * @brief Gets the information stored in the vertex.
 * @return Information stored in the vertex.
 */
template <class T> T Vertex<T>::getInfo() const { return this->info; }

/**
 * @brief Gets the low value of the vertex.
 * @return Low value of the vertex.
 */
template <class T> int Vertex<T>::getLow() const { return this->low; }

/**
 * @brief Sets the low value of the vertex.
 * @param value Low value to set.
 */
template <class T> void Vertex<T>::setLow(int value) { this->low = value; }

/**
 * @brief Gets the num value of the vertex.
 * @return Num value of the vertex.
 */
template <class T> int Vertex<T>::getNum() const { return this->num; }

/**
 * @brief Sets the num value of the vertex.
 * @param value Num value to set.
 */
template <class T> void Vertex<T>::setNum(int value) { this->num = value; }

/**
 * @brief Gets the adjacency list of the vertex.
 * @return Adjacency list of the vertex.
 */
template <class T> std::vector<Edge<T> *> Vertex<T>::getAdj() const {
  return this->adj;
}

/**
 * @brief Checks if the vertex has been visited.
 * @return True if the vertex has been visited, false otherwise.
 */
template <class T> bool Vertex<T>::isVisited() const { return this->visited; }

/**
 * @brief Checks if the vertex is being processed.
 * @return True if the vertex is being processed, false otherwise.
 */
template <class T> bool Vertex<T>::isProcessing() const {
  return this->processing;
}

/**
 * @brief Gets the indegree of the vertex.
 * @return Indegree of the vertex.
 */
template <class T> unsigned int Vertex<T>::getIndegree() const {
  return this->indegree;
}

/**
 * @brief Gets the walking distance of the vertex.
 * @return Walking distance of the vertex.
 */
template <class T> double Vertex<T>::getWalkingDist() const {
  return this->walkingDist;
}

/**
 * @brief Gets the driving distance of the vertex.
 * @return Driving distance of the vertex.
 */
template <class T> double Vertex<T>::getDrivingDist() const {
  return this->drivingDist;
}

/**
 * @brief Gets the path edge of the vertex.
 * @return Path edge of the vertex.
 */
template <class T> Edge<T> *Vertex<T>::getPath() const { return this->path; }

/**
 * @brief Gets the incoming edges of the vertex.
 * @return Incoming edges of the vertex.
 */
template <class T> std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
  return this->incoming;
}

/**
 * @brief Sets the information stored in the vertex.
 * @param in Information to store in the vertex.
 */
template <class T> void Vertex<T>::setInfo(T in) { this->info = in; }

/**
 * @brief Sets the visited status of the vertex.
 * @param visited Visited status to set.
 */
template <class T> void Vertex<T>::setVisited(bool visited) {
  this->visited = visited;
}

/**
 * @brief Sets the processing status of the vertex.
 * @param processing Processing status to set.
 */
template <class T> void Vertex<T>::setProcessing(bool processing) {
  this->processing = processing;
}

/**
 * @brief Sets the indegree of the vertex.
 * @param indegree Indegree to set.
 */
template <class T> void Vertex<T>::setIndegree(unsigned int indegree) {
  this->indegree = indegree;
}

/**
 * @brief Sets the driving distance of the vertex.
 * @param dd Driving distance to set.
 */
template <class T> void Vertex<T>::setDrivingDist(double dd) {
  this->drivingDist = dd;
}

/**
 * @brief Sets the walking distance of the vertex.
 * @param wd Walking distance to set.
 */
template <class T> void Vertex<T>::setWalkingDist(double wd) {
  this->walkingDist = wd;
}

/**
 * @brief Sets the path edge of the vertex.
 * @param path Path edge to set.
 */
template <class T> void Vertex<T>::setPath(Edge<T> *path) { this->path = path; }

/**
 * @brief Deletes an edge from the vertex.
 * This operation has a time complexity of O(E), where E is the number of
 * incoming edges of the destination vertex.
 * @param edge Edge to delete.
 */
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

/**
 * @brief Gets the queue index of the vertex.
 * @return Queue index of the vertex.
 */
template <class T> int Vertex<T>::getQueueIndex() const {
  return this->queueIndex;
}

/**
 * @brief Sets the queue index of the vertex.
 * @param idx Queue index to set.
 */
template <class T> void Vertex<T>::setQueueIndex(int idx) {
  this->queueIndex = idx;
}

/********************** Edge  ****************************/

/**
 * @brief Constructor for the Edge class.
 * @param orig Origin vertex of the edge.
 * @param dest Destination vertex of the edge.
 * @param dw Driving weight of the edge.
 * @param ww Walking weight of the edge.
 */
template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double dw, double ww)
    : orig(orig), dest(dest), drivingWeight(dw), walkingWeight(ww) {}

/**
 * @brief Gets the destination vertex of the edge.
 * @return Destination vertex of the edge.
 */
template <class T> Vertex<T> *Edge<T>::getDest() const { return this->dest; }

/**
 * @brief Gets the walking weight of the edge.
 * @return Walking weight of the edge.
 */
template <class T> double Edge<T>::getWalkingWeight() const {
  return this->walkingWeight;
}

/**
 * @brief Gets the driving weight of the edge.
 * @return Driving weight of the edge.
 */
template <class T> double Edge<T>::getDrivingWeight() const {
  return this->drivingWeight;
}

/**
 * @brief Gets the origin vertex of the edge.
 * @return Origin vertex of the edge.
 */
template <class T> Vertex<T> *Edge<T>::getOrig() const { return this->orig; }

/**
 * @brief Gets the reverse edge of the edge.
 * @return Reverse edge of the edge.
 */
template <class T> Edge<T> *Edge<T>::getReverse() const {
  return this->reverse;
}

/**
 * @brief Checks if the edge is selected.
 * @return True if the edge is selected, false otherwise.
 */
template <class T> bool Edge<T>::isSelected() const { return this->selected; }

/**
 * @brief Gets the flow of the edge.
 * @return Flow of the edge.
 */
template <class T> double Edge<T>::getFlow() const { return flow; }

/**
 * @brief Sets the selected status of the edge.
 * @param selected Selected status to set.
 */
template <class T> void Edge<T>::setSelected(bool selected) {
  this->selected = selected;
}

/**
 * @brief Sets the reverse edge of the edge.
 * @param reverse Reverse edge to set.
 */
template <class T> void Edge<T>::setReverse(Edge<T> *reverse) {
  this->reverse = reverse;
}

/**
 * @brief Sets the flow of the edge.
 * @param flow Flow to set.
 */
template <class T> void Edge<T>::setFlow(double flow) { this->flow = flow; }

/********************** Graph  ****************************/

/**
 * @brief Gets the number of vertices in the graph.
 * @return Number of vertices in the graph.
 */
template <class T> int Graph<T>::getNumVertex() const {
  return vertexSet.size();
}

/**
 * @brief Gets the vertex set of the graph.
 * @return Vertex set of the graph.
 */
template <class T> std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
  return vertexSet;
}

/**
 * @brief Finds a vertex in the graph by its info.
 * This operation has a time complexity of O(V), where V is the number of
 * vertices in the graph.
 * @param in Info of the vertex to find.
 * @return Pointer to the vertex if found, nullptr otherwise.
 */
template <class T> Vertex<T> *Graph<T>::findVertex(const T &in) const {
  for (auto v : vertexSet)
    if (v->getInfo() == in)
      return v;
  return nullptr;
}

/**
 * @brief Finds a vertex in the graph by its code.
 * This operation has a time complexity of O(V), where V is the number of
 * vertices in the graph.
 * @param code Code of the vertex to find.
 * @return Pointer to the vertex if found, nullptr otherwise.
 */
template <class T>
Vertex<T> *Graph<T>::findVertexByCode(const std::string code) const {
  for (auto v : vertexSet)
    if (v->getInfo().code == code)
      return v;
  return nullptr;
}

/**
 * @brief Finds a vertex in the graph by its ID.
 * This operation has a time complexity of O(V), where V is the number of
 * vertices in the graph.
 * @param id ID of the vertex to find.
 * @return Pointer to the vertex if found, nullptr otherwise.
 */
template <class T> Vertex<T> *Graph<T>::findVertexById(const int id) const {
  for (auto v : vertexSet)
    if (v->getInfo().id == id)
      return v;
  return nullptr;
}

/**
 * @brief Finds the index of a vertex in the graph by its info.
 * This operation has a time complexity of O(V), where V is the number of
 * vertices in the graph.
 * @param in Info of the vertex to find.
 * @return Index of the vertex if found, -1 otherwise.
 */
template <class T> int Graph<T>::findVertexIdx(const T &in) const {
  for (unsigned i = 0; i < vertexSet.size(); i++)
    if (vertexSet[i]->getInfo() == in)
      return i;
  return -1;
}

/**
 * @brief Adds a vertex to the graph.
 * This operation has a time complexity of O(V), where V is the number of
 * vertices in the graph.
 * @param in Info of the vertex to add.
 * @return True if the vertex was added, false if it already exists.
 */
template <class T> bool Graph<T>::addVertex(const T &in) {
  if (findVertex(in) != nullptr)
    return false;
  vertexSet.push_back(new Vertex<T>(in));
  return true;
}

/**
 * @brief Removes a vertex from the graph.
 * This operation has a time complexity of O(V + E), where V is the number of
 * vertices and E is the number of edges.
 * @param in Info of the vertex to remove.
 * @return True if the vertex was removed, false otherwise.
 */
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
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double dw, double ww) {
  auto v1 = findVertex(sourc);
  auto v2 = findVertex(dest);
  if (v1 == nullptr || v2 == nullptr)
    return false;
  v1->addEdge(v2, dw, ww);
  return true;
}

/**
 * @brief Removes a directed edge between two vertices.
 * This operation has a time complexity of O(E), where E is the number of edges
 * in the adjacency list of the source vertex.
 * @param sourc Info of the source vertex.
 * @param dest Info of the destination vertex.
 * @return True if the edge was removed, false otherwise.
 */
template <class T> bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
  Vertex<T> *srcVertex = findVertex(sourc);
  if (srcVertex == nullptr) {
    return false;
  }
  return srcVertex->removeEdge(dest);
}

/**
 * @brief Finds an edge between two vertices by their IDs.
 * This operation has a time complexity of O(E), where E is the number of edges
 * in the adjacency list of the source vertex.
 * @param sourceId ID of the source vertex.
 * @param destId ID of the destination vertex.
 * @return Pointer to the edge if found, nullptr otherwise.
 */
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

/**
 * @brief Adds a bidirectional edge between two vertices.
 * This operation has a time complexity of O(V), where V is the number of
 * vertices in the graph.
 * @param sourc Info of the source vertex.
 * @param dest Info of the destination vertex.
 * @param dw Driving weight of the edge.
 * @param ww Walking weight of the edge.
 * @return True if the edge was added, false otherwise.
 */
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

/**
 * @brief Deletes a dynamically allocated matrix.
 * This operation has a time complexity of O(n), where n is the number of rows
 * in the matrix.
 * @param m Pointer to the matrix.
 * @param n Number of rows in the matrix.
 */
inline void deleteMatrix(int **m, int n) {
  if (m != nullptr) {
    for (int i = 0; i < n; i++)
      if (m[i] != nullptr)
        delete[] m[i];
    delete[] m;
  }
}

/**
 * @brief Deletes a dynamically allocated matrix.
 * This operation has a time complexity of O(n), where n is the number of rows
 * in the matrix.
 * @param m Pointer to the matrix.
 * @param n Number of rows in the matrix.
 */
inline void deleteMatrix(double **m, int n) {
  if (m != nullptr) {
    for (int i = 0; i < n; i++)
      if (m[i] != nullptr)
        delete[] m[i];
    delete[] m;
  }
}

/**
 * @brief Destructor for the Graph class. Frees allocated memory.
 * This operation has a time complexity of O(V + n), where V is the number of
 * vertices and n is the size of the matrices.
 */
template <class T> Graph<T>::~Graph() {
  deleteMatrix(distMatrix, vertexSet.size());
  deleteMatrix(pathMatrix, vertexSet.size());
}

#endif /* DA_TP_CLASSES_GRAPH */