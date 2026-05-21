// CMSC 341 - Spring 2026 - Project 3
#include "shop.h"
Shop::Shop(){ // empty object
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = nullptr;
  m_heapType = MINHEAP;
  m_structure = SKEW;
  m_regPrior = 0;
  m_shopID = 0;
  
}
Shop::Shop(prifn_t priFn, HEAPTYPE heapType, 
	   STRUCTURE structure, int regPrior,
	   int id)
{
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
  m_heapType = heapType;
  m_structure = structure;
  m_regPrior = regPrior;
  m_shopID = id;
}
Shop::~Shop()
{
  clear();
  
}
void Shop::clear() {
  clearHelper(m_heap);
  m_heap = nullptr;
  m_size = 0;
    
}
Shop::Shop(const Shop& rhs)
{
  m_shopID = rhs.m_shopID;
  m_heap = copyHelper(rhs.m_heap);
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
  m_regPrior = rhs.m_regPrior;
}

Shop& Shop::operator=(const Shop& rhs) {
  if (this != &rhs) {
    clear();
    m_shopID = rhs.m_shopID;
    m_heap = copyHelper(rhs.m_heap);
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    m_regPrior = rhs.m_regPrior;
  }
  return *this;
  
}
void Shop::mergeWithQueue(Shop& rhs) {
  if (this == &rhs)
    return;

  if (m_priorFunc != rhs.m_priorFunc ||
      m_heapType != rhs.m_heapType ||
      m_structure != rhs.m_structure)
    throw domain_error("Incompatible heaps");
  
  m_heap = mergeHelper(m_heap, rhs.m_heap);
  m_size = m_size + rhs.m_size;
  
  rhs.m_heap = nullptr;
  rhs.m_size = 0;
  
}

bool Shop::insertOrder(const Order& order) {
  Order* newNode = new Order(order.getItem(),
			     order.getCount(),
			     order.getMemebership(),
			     order.getPoints(),
			     order.getCustomerID(),
			     order.getOrderID());
  
  m_heap = mergeHelper(m_heap, newNode);
  m_size++;
  return true;    
}

int Shop::numOrders() const
{
  return m_size;
}
prifn_t Shop::getPriorityFn() const {
  return m_priorFunc;
  
}
Order Shop::getNextOrder() {
  if (m_heap == nullptr)
    throw out_of_range("Heap is empty");

  Order nextOrder = *m_heap;
  Order* leftChild = m_heap->m_left;
  Order* rightChild = m_heap->m_right;
  
  delete m_heap;
  m_heap = mergeHelper(leftChild, rightChild);
  m_size--;
  
  return nextOrder;
  
}
void Shop::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  if (!m_heap) {
    m_priorFunc = priFn;
    m_heapType = heapType;
    return;
  }

  Order** arr = new Order*[m_size];
  int index = 0;
  
  collectOrders(m_heap, arr, index);
  
  clear();

  m_priorFunc = priFn;
  m_heapType = heapType;
  
  for (int i = 0; i < index; i++) {
    insertOrder(*arr[i]);
    delete arr[i];
  }
  
  delete [] arr;
}
                      

void Shop::setStructure(STRUCTURE structure){
  if (m_size == 0) {
    m_structure = structure;
    return;
  }

  int oldSize = m_size;
  Order** arr = new Order*[oldSize];
  int index = 0;

  collectOrders(m_heap, arr, index);

  clear();

  m_structure = structure;
  
  for (int i = 0; i < index; i++) {
    insertOrder(*arr[i]);
    delete arr[i];
  }
  
  delete [] arr;
  
}
HEAPTYPE Shop::getHeapType() const {
  return m_heapType;
  
}
STRUCTURE Shop::getStructure() const {
  return m_structure;
  
}
void Shop::printOrdersQueue() const {
  cout << "Contents of the queue:" << endl;
  int index = 1;
  printOrdersHelper(m_heap, index);
  
}

void Shop::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    cout << "Shop " << m_regPrior << ": => ";
    dump(m_heap);
  }
  cout << endl;
}
void Shop::dump(Order *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(*pos) << ":" << pos->m_orderID;
    else
        cout << m_priorFunc(*pos) << ":" << pos->m_orderID << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}

ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order ID: " << order.getOrderID() 
        << ", item: " << order.getItem()
        << ", count: " << order.getCount();
  return sout;
}
Order* Shop::mergeHelper(Order* h1, Order* h2){
  if (h1 == nullptr)
    return h2;
  if (h2 == nullptr)
    return h1;

  int p1 = m_priorFunc(*h1);
  int p2 = m_priorFunc(*h2);

  bool h1First;
  if (m_heapType == MINHEAP)
    h1First = (p1 <= p2);
  else
    h1First = (p1 >= p2);

  if (!h1First) {
    Order* temp = h1;
    h1 = h2;
    h2 = temp;
  }

  if (m_structure == SKEW)
    return skewMerge(h1, h2);
  else
    return leftistMerge(h1, h2);
  
  

}


Order* Shop::skewMerge(Order* h1, Order* h2){
  h1->m_right = mergeHelper(h1->m_right, h2);
  swapChildren(h1);
  return h1;
}

Order* Shop::leftistMerge(Order* h1, Order* h2){
  h1->m_right = mergeHelper(h1->m_right, h2);

  if (getNPL(h1->m_left) < getNPL(h1->m_right))
    swapChildren(h1);
  
  h1->m_npl = 1 + ((getNPL(h1->m_left) < getNPL(h1->m_right)) ?
		   getNPL(h1->m_left) : getNPL(h1->m_right));
  
  return h1;

}


void Shop::swapChildren(Order* node){
  Order* temp = node->m_left;
  node->m_left = node->m_right;
  node->m_right = temp;

}


int Shop::getNPL(Order* node) const{
  return (node == nullptr) ? -1 : node->m_npl;
}


void Shop::clearHelper(Order* node){
  if (!node) return;

  clearHelper(node->m_left);
  clearHelper(node->m_right);
  
  delete node;
  
}

Order* Shop::copyHelper(Order* node){
  if (node == nullptr)
    return nullptr;
  
  Order* newNode = new Order(node->getItem(),
			     node->getCount(),
			     node->getMemebership(),
			     node->getPoints(),
			     node->getCustomerID(),
			     node->getOrderID());
  
  newNode->m_npl = node->m_npl;
  newNode->m_left = copyHelper(node->m_left);
  newNode->m_right = copyHelper(node->m_right);
  
  return newNode;
  
}


void Shop::collectOrders(Order* node, Order* arr[], int &index){
  if (node != nullptr) {
    arr[index] = new Order(node->getItem(),
			   node->getCount(),
			   node->getMemebership(),
			   node->getPoints(),
			   node->getCustomerID(),
			   node->getOrderID());
    index++;
    collectOrders(node->m_left, arr, index);
    collectOrders(node->m_right, arr, index);
  }
}


void Shop::printOrdersHelper(Order* node, int& index) const{
  if (node != nullptr) {
    cout << "[" << index << "] Order ID: " << node->getOrderID()
	 << ", item: " << node->getItemString()
	 << ", count: " << node->getCountString() << endl;
    index++;
    printOrdersHelper(node->m_left, index);
    printOrdersHelper(node->m_right, index);
  }
}


//////////////////////////////////////////////////////////////
Region::Region(int size){
  m_capacity = size;
  m_size = 0;
  m_heap = new Shop[m_capacity + 1];
  
}
Region::~Region(){
  delete[] m_heap;
  
}
bool Region::addShop(Shop & aShop){
  if (m_size == m_capacity)
    return false;

  m_size++;
  m_heap[m_size] = aShop;
  heapifyUp(m_size);
  return true;
  
}

bool Region::getShop(Shop & aShop){
  if (m_size == 0)
    return false;

  aShop = m_heap[ROOTINDEX];
  m_heap[ROOTINDEX] = m_heap[m_size];
  m_size--;
  if (m_size > 0)
    heapifyDown(ROOTINDEX);
  
  return true;
  
}

bool Region::getNthShop(Shop & aShop, int n){
  if (n < 1 || n > m_size)
    return false;
  
  Shop* temp = new Shop[n];
  int i;
  
  for (i = 0; i < n; i++) {
    getShop(temp[i]);
  }
  
  aShop = temp[n - 1];
  
  for (i = 0; i < n; i++) {
    addShop(temp[i]);
  }
  
  delete[] temp;
  return true;
  
}

bool Region::setPriorityFn(prifn_t priFn, HEAPTYPE heapType, int n){
  if (n < 1 || n > m_size)
    return false;
  
  Shop* temp = new Shop[n];
  int i;
  
  for (i = 0; i < n; i++) {
    getShop(temp[i]);
  }
  
  temp[n - 1].setPriorityFn(priFn, heapType);
  
  for (i = 0; i < n; i++) {
    addShop(temp[i]);
  }
  
  delete[] temp;
  return true;
  
}

bool Region::setStructure(STRUCTURE structure, int n){
  if (n < 1 || n > m_size)
        return false;

  Shop* temp = new Shop[n];
  int i;
  
  for (i = 0; i < n; i++) {
    getShop(temp[i]);
  }
  
  temp[n - 1].setStructure(structure);
  
  for (i = 0; i < n; i++) {
    addShop(temp[i]);
  }
  
  delete[] temp;
  return true;
  
}
bool Region::getOrder(Order & order){
  if (m_size == 0)
    return false;
  
  Shop topShop;
  getShop(topShop);
  
  if (topShop.numOrders() == 0)
    return false;
  
  order = topShop.getNextOrder();
  
  if (topShop.numOrders() > 0)
    addShop(topShop);

  return true;
  
}
void Region::dump(){
    dump(ROOTINDEX);
    cout << endl;
}
void Region::dump(int index){
  if (index <= m_size){
    cout << "(";
    dump(index*2);
    cout << m_heap[index].m_regPrior;
    dump(index*2 + 1);
    cout << ")";
  }
}

void Region::heapifyUp(int index) {
  while (index > 1 && m_heap[index].m_regPrior < m_heap[index / 2].m_regPrior) {
    Shop temp = m_heap[index];
    m_heap[index] = m_heap[index / 2];
    m_heap[index / 2] = temp;
    index = index / 2;
  }
}


void Region::heapifyDown(int index) {
  int done = 0;

  while (!done) {
    int left = index * 2;
    int right = index * 2 + 1;
    int smallest = index;
    
    if (left <= m_size && m_heap[left].m_regPrior < m_heap[smallest].m_regPrior)
      smallest = left;
    if (right <= m_size && m_heap[right].m_regPrior < m_heap[smallest].m_regPrior)
      smallest = right;
    
    if (smallest == index)
      done = 1;
    else {
      Shop temp = m_heap[index];
      m_heap[index] = m_heap[smallest];
      m_heap[smallest] = temp;
      index = smallest;
    }
  }
}
