// UMBC - CSEE - CMSC 341 - Spring 2026 - Proj5
// Do not write your name or campus ID in files
#include "graph.h"

Graph::Graph(){
  m_numNodes = 0;
  m_head = nullptr;
  m_dataFile = "";
}

Graph::Graph(string dataFile){
  m_numNodes = 0;
  m_head = nullptr;
  m_dataFile = dataFile;
  loadData();
}

Graph::~Graph(){
  clearGraph();
}

void Graph::loadData(){
  int numNodes;
  int node, n, e, s, w; 
  ifstream dataFile;
  dataFile.open(m_dataFile);
  if (dataFile.is_open()) {
    dataFile >> numNodes;
    m_numNodes = numNodes;
    for (int i = 0; i < numNodes; i++) {
      dataFile >> node >> n >> e >> s >> w;
      insert(node, n, e, s, w);
    }
  }
  else
    //the following statement provides the reason if a file doesn't open
    //please note: not all platforms are providing the same message
    cerr << "Error: " << strerror(errno) << endl;
}

void Graph::insert(int node, int n, int e, int s, int w){
  Node *current = findNode(node);
  
  if (current == nullptr){
    current = new Node(node);
    insertAtHead(current);
  }
  
  Node *neighbor = nullptr;
  
  if (n == -1){
    current->setNorth(nullptr);
  }
  else{
    neighbor = findNode(n);
    if (neighbor == nullptr){
      neighbor = new Node(n);
      insertAtHead(neighbor);
    }
    current->setNorth(neighbor);
  }
  
  if (e == -1){
    current->setEast(nullptr);
  }
  else{
    neighbor = findNode(e);
    if (neighbor == nullptr){
      neighbor = new Node(e);
      insertAtHead(neighbor);
    }
    current->setEast(neighbor);
  }
  
  if (s == -1){
    current->setSouth(nullptr);
  }
  else{
    neighbor = findNode(s);
    if (neighbor == nullptr){
      neighbor = new Node(s);
      insertAtHead(neighbor);
    }
    current->setSouth(neighbor);
  }
  
  if (w == -1){
    current->setWest(nullptr);
  }
  else{
    neighbor = findNode(w);
    if (neighbor == nullptr){
      neighbor = new Node(w);
      insertAtHead(neighbor);
    }
    current->setWest(neighbor);
  }
}

void Graph::insertAtHead(Node * aNode){
  if (aNode != nullptr){
    aNode->setNext(m_head);
    m_head = aNode;
  }
}

Node * Graph::findNode(int nodeValue){
  Node *current = m_head;
  
  while (current != nullptr){
    if (current->getValue() == nodeValue){
      return current;
    }
    current = current->getNext();
  }
  
  return nullptr;
}

bool Graph::findPath(int start, int end){
  clearResult();
  clearVisited();
  
  Node *startNode = findNode(start);
  Node *endNode = findNode(end);
  
  if (startNode == nullptr || endNode == nullptr){
    return false;
  }

  bool result = findPath(startNode, end);
  
  if (result == false){
    clearResult();
  }
  
  clearVisited();
  
  return result;
}

bool Graph::findPath(Node* aNode, int end){
  if (aNode == nullptr){
    return false;
  }

  if (aNode->getVisited()){
    return false;
  }
  
  aNode->setVisited(true);
  
  if (aNode->getValue() == end){
    m_path.push(aNode->getValue());
    return true;
  }

  if (findPath(aNode->getNorth(), end)){
    m_path.push(aNode->getValue());
    return true;
  }
  
  if (findPath(aNode->getEast(), end)){
    m_path.push(aNode->getValue());
    return true;
  }
  
  if (findPath(aNode->getSouth(), end)){
    m_path.push(aNode->getValue());
    return true;
  }

  if (findPath(aNode->getWest(), end)){
    m_path.push(aNode->getValue());
    return true;
  }
  
  return false;
}

void Graph::dump(){
  stack<int> tempPath = m_path;
  
  while (!tempPath.empty()){
    cout << tempPath.top() << " => ";
    tempPath.pop();
  }
  
  cout << "END" << endl;
}

void Graph::clearResult(){

  while (!m_path.empty()){
    m_path.pop();
  }
}

void Graph::clearVisited(){
  Node *current = m_head;
  
  while (current != nullptr){
    current->setVisited(false);
    current = current->getNext();
  }
}

void Graph::buildGraph(string file){
  clearGraph();
  m_dataFile = file;
  loadData();
}

void Graph::clearGraph(){
  Node *current = m_head;
  Node *temp = nullptr;
  
  while (current != nullptr){
    temp = current->getNext();
    delete current;
    current = temp;
  }
  
  m_head = nullptr;
  m_numNodes = 0;
  m_dataFile = "";
  clearResult();
}

const Graph & Graph::operator=(const Graph & rhs){
  if (this != &rhs){
    clearGraph();
    
    m_numNodes = rhs.m_numNodes;
    m_dataFile = rhs.m_dataFile;
    m_path = rhs.m_path;
    
    Node *rhsCurrent = rhs.m_head;
    Node *last = nullptr;
    
    while (rhsCurrent != nullptr){
      Node *newNode = new Node(rhsCurrent->getValue());
      
      if (m_head == nullptr){
	m_head = newNode;
      }
      else{
	last->setNext(newNode);
      }
      
      last = newNode;
      rhsCurrent = rhsCurrent->getNext();
    }
    
    rhsCurrent = rhs.m_head;
    Node *current = m_head;
    
    while (rhsCurrent != nullptr && current != nullptr){
      if (rhsCurrent->getNorth() != nullptr){
	current->setNorth(findNode(rhsCurrent->getNorth()->getValue()));
      }
      else{
	current->setNorth(nullptr);
      }
      
      if (rhsCurrent->getEast() != nullptr){
	current->setEast(findNode(rhsCurrent->getEast()->getValue()));
      }
      else{
	current->setEast(nullptr);
      }
      
      if (rhsCurrent->getSouth() != nullptr){
	current->setSouth(findNode(rhsCurrent->getSouth()->getValue()));
      }
      else{
	current->setSouth(nullptr);
      }
      
      if (rhsCurrent->getWest() != nullptr){
	current->setWest(findNode(rhsCurrent->getWest()->getValue()));
      }
      else{
	current->setWest(nullptr);
      }
      
      current->setVisited(rhsCurrent->getVisited());
      
      rhsCurrent = rhsCurrent->getNext();
      current = current->getNext();
    }
  }
  
  return *this;
}

bool Graph::empty() const        // is the list empty?
{ return m_head == nullptr; }
