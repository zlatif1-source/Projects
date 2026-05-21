// CMSC 341 - Spring 2026 - Project 3
#include "shop.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <math.h>
#include <vector>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};

class Random {
public:
  Random() {}
  Random(int min, int max, RANDOM type = UNIFORMINT, int mean = 50, int stdev = 20)
    : m_min(min), m_max(max), m_type(type)
  {
    if (type == NORMAL) {
      m_generator = mt19937(m_device());
      m_normdist = normal_distribution<>(mean, stdev);
    }
    else if (type == UNIFORMINT) {
      m_generator = mt19937(10);
      m_unidist = uniform_int_distribution<>(min, max);
    }
    else if (type == UNIFORMREAL) {
      m_generator = mt19937(10);
      m_uniReal = uniform_real_distribution<double>((double)min, (double)max);
    }
    else {
      m_generator = mt19937(m_device());
    }
  }

  void setSeed(int seedNum) {
    m_generator = mt19937(seedNum);
  }
  
  void init(int min, int max) {
    m_min = min;
    m_max = max;
    m_type = UNIFORMINT;
    m_generator = mt19937(10);
    m_unidist = uniform_int_distribution<>(min, max);
  }
  
  void getShuffle(vector<int> & array) {
    int i;
    for (i = m_min; i <= m_max; i++) {
      array.push_back(i);
    }
    shuffle(array.begin(), array.end(), m_generator);
  }

  void getShuffle(int array[]) {
    vector<int> temp;
    int i;
    for (i = m_min; i <= m_max; i++) {
      temp.push_back(i);
    }
    shuffle(temp.begin(), temp.end(), m_generator);
    vector<int>::iterator it;
    i = 0;
    for (it = temp.begin(); it != temp.end(); it++) {
      array[i] = *it;
      i++;
    }
  }
  
  int getRandNum() {
    int result = 0;
    if (m_type == NORMAL) {
      result = m_min - 1;
      while (result < m_min || result > m_max)
	result = m_normdist(m_generator);
    }
    else if (m_type == UNIFORMINT) {
      result = m_unidist(m_generator);
    }
    return result;
  }
  
  double getRealRandNum() {
    double result = m_uniReal(m_generator);
    result = floor(result * 100.0) / 100.0;
    return result;
  }
  
  string getRandString(int size) {
    string output = "";
    int i;
    for (i = 0; i < size; i++) {
      output = output + (char)getRandNum();
    }
    return output;
  }
  
  int getMin() { return m_min; }
  int getMax() { return m_max; }
  
private:
  int m_min;
  int m_max;
  RANDOM m_type;
  random_device m_device;
  mt19937 m_generator;
  normal_distribution<> m_normdist;
  uniform_int_distribution<> m_unidist;
  uniform_real_distribution<double> m_uniReal;
};

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);

class Tester {
public:
  bool testInsertMinHeap();
  bool testInsertMaxHeap();
  bool testRemoveMinHeap();
  bool testRemoveMaxHeap();
  bool testLeftistNPL();
  bool testLeftistProperty();
  bool sanityCheck();
  bool testSetPriorityFn();
  bool testSetPriorityFnDataPreserved();
  bool testSetStructureToLeftist();
  bool testSetStructureToSkew();
  bool testMergeEmptyWithNormal();
  bool testMergeNormalWithNormal();
  bool testCopyConstructorNormal();
  bool testCopyConstructorEdge();
  bool testCopyConstructorSingleNode();
  bool testAssignmentOperatorNormal();
  bool testAssignmentOperatorEdge();
  bool testSelfAssignment();
  bool testDequeueEmptyException();
  bool testMergeDifferentPriorityException();
  bool testMergeDifferentHeapTypeException();
  bool testMergeDifferentStructureException();
  bool testRegionAddShop();
  bool testRegionGetShopOrder();
  bool testRegionGetNthShop();
  bool testRegionGetOrder();
  bool testRegionEmptyCases();
  bool testRegionSetPriorityFn();
  bool testRegionSetStructure();
  
private:
  void makeOrders(Shop& shop, int n);
  bool checkMinHeap(Order* node, prifn_t priFn);
  bool checkMaxHeap(Order* node, prifn_t priFn);
  int computeNPL(Order* node);
  bool checkLeftistNPLValues(Order* node);
  bool checkLeftistPropertyHelper(Order* node);
  int countNodes(Order* node);
  void collectOrderIDs(Order* node, int arr[], int& index);
  bool sameOrderIDSet(Order* root1, Order* root2);
  bool isBSTByOrderID(Order* node, int minVal, int maxVal);
  bool checkRegionMinHeap(Shop heapArray[], int size);
};

int priorityFn1(const Order &order) {
  int priority = static_cast<int>(order.getCount()) + order.getPoints();
  if (priority == 0)
    priority = 1;
  return priority;
}

int priorityFn2(const Order &order) {
  int priority = static_cast<int>(order.getItem()) + static_cast<int>(order.getMemebership());
  if (priority == 0)
    priority = 1;
  return priority;
}

void Tester::makeOrders(Shop& shop, int n) {
  Random orderIDGen(MINORDERID, MAXORDERID);
  Random countGen(ONE, DOZEN);
  Random customerIDGen(MINCUSTID, MAXCUSTID);
  Random membershipGen(TIER1, TIER6);
  Random pointsGen(MINPOINTS, MAXPOINTS);
  Random itemGen(COFFEE, ICEDTEA);
  
  int i;
  for (i = 0; i < n; i++) {
    Order anOrder(static_cast<ITEM>(itemGen.getRandNum()),
		  static_cast<COUNT>(countGen.getRandNum()),
		  static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
		  pointsGen.getRandNum(),
		  customerIDGen.getRandNum(),
		  orderIDGen.getRandNum());
    shop.insertOrder(anOrder);
  }
}

bool Tester::checkMinHeap(Order* node, prifn_t priFn) {
  if (node == nullptr)
    return true;
  
  if (node->m_left != nullptr) {
    if (priFn(*node) > priFn(*(node->m_left)))
      return false;
  }
  if (node->m_right != nullptr) {
    if (priFn(*node) > priFn(*(node->m_right)))
      return false;
  }
  
  return checkMinHeap(node->m_left, priFn) &&
    checkMinHeap(node->m_right, priFn);
}

bool Tester::checkMaxHeap(Order* node, prifn_t priFn) {
  if (node == nullptr)
    return true;
  
  if (node->m_left != nullptr) {
    if (priFn(*node) < priFn(*(node->m_left)))
      return false;
  }
  if (node->m_right != nullptr) {
    if (priFn(*node) < priFn(*(node->m_right)))
      return false;
  }
  
  return checkMaxHeap(node->m_left, priFn) &&
    checkMaxHeap(node->m_right, priFn);
}

int Tester::computeNPL(Order* node) {
  int leftNPL;
  int rightNPL;
  
  if (node == nullptr)
    return -1;
  
  leftNPL = computeNPL(node->m_left);
  rightNPL = computeNPL(node->m_right);
  
  if (leftNPL < rightNPL)
    return 1 + leftNPL;
  else
    return 1 + rightNPL;
}

bool Tester::checkLeftistNPLValues(Order* node) {
  int expected;
  
  if (node == nullptr)
    return true;
  
  expected = computeNPL(node);
  if (node->m_npl != expected)
    return false;
  
  return checkLeftistNPLValues(node->m_left) &&
    checkLeftistNPLValues(node->m_right);
}

bool Tester::checkLeftistPropertyHelper(Order* node) {
  int leftNPL;
  int rightNPL;
  
  if (node == nullptr)
    return true;
  
  leftNPL = computeNPL(node->m_left);
  rightNPL = computeNPL(node->m_right);
  
  if (leftNPL < rightNPL)
    return false;
  
  return checkLeftistPropertyHelper(node->m_left) &&
    checkLeftistPropertyHelper(node->m_right);
}

int Tester::countNodes(Order* node) {
  if (node == nullptr)
    return 0;
  return 1 + countNodes(node->m_left) + countNodes(node->m_right);
}

void Tester::collectOrderIDs(Order* node, int arr[], int& index) {
  if (node != nullptr) {
    arr[index] = node->m_orderID;
    index++;
    collectOrderIDs(node->m_left, arr, index);
    collectOrderIDs(node->m_right, arr, index);
  }
}

bool Tester::sameOrderIDSet(Order* root1, Order* root2) {
  int n1;
  int n2;
  int* arr1;
  int* arr2;
  int i;
  int index1;
  int index2;
  
  n1 = countNodes(root1);
  n2 = countNodes(root2);
  
  if (n1 != n2)
    return false;
  
  arr1 = new int[n1];
  arr2 = new int[n2];
  
  index1 = 0;
  index2 = 0;
  collectOrderIDs(root1, arr1, index1);
  collectOrderIDs(root2, arr2, index2);
  
  sort(arr1, arr1 + n1);
  sort(arr2, arr2 + n2);
  
  for (i = 0; i < n1; i++) {
    if (arr1[i] != arr2[i]) {
      delete[] arr1;
      delete[] arr2;
      return false;
    }
  }
  
  delete[] arr1;
  delete[] arr2;
  return true;
}

bool Tester::isBSTByOrderID(Order* node, int minVal, int maxVal) {
  if (node == nullptr)
    return true;
  
  if (node->m_orderID <= minVal || node->m_orderID >= maxVal)
    return false;
  
  return isBSTByOrderID(node->m_left, minVal, node->m_orderID) &&
    isBSTByOrderID(node->m_right, node->m_orderID, maxVal);
}

bool Tester::checkRegionMinHeap(Shop heapArray[], int size) {
  int i;
  for (i = 1; i <= size; i++) {
    int left = i * 2;
    int right = i * 2 + 1;
    
    if (left <= size && heapArray[i].m_regPrior > heapArray[left].m_regPrior)
      return false;
    if (right <= size && heapArray[i].m_regPrior > heapArray[right].m_regPrior)
      return false;
  }
  return true;
}

bool Tester::testInsertMinHeap() {
  Shop shop(priorityFn2, MINHEAP, SKEW, 10, 100001);
  makeOrders(shop, 300);
  return checkMinHeap(shop.m_heap, priorityFn2) && shop.numOrders() == 300;
}

bool Tester::testInsertMaxHeap() {
  Shop shop(priorityFn1, MAXHEAP, SKEW, 10, 100002);
  makeOrders(shop, 300);
  return checkMaxHeap(shop.m_heap, priorityFn1) && shop.numOrders() == 300;
}

bool Tester::testRemoveMinHeap() {
  Shop shop(priorityFn2, MINHEAP, SKEW, 10, 100003);
  int lastPriority;
  int currentPriority;
  int i;
  
  makeOrders(shop, 300);
  lastPriority = -1;
  
  for (i = 0; i < 300; i++) {
    Order temp = shop.getNextOrder();
    currentPriority = priorityFn2(temp);
    if (i > 0 && currentPriority < lastPriority)
      return false;
    lastPriority = currentPriority;
  }

  return shop.numOrders() == 0;
}

bool Tester::testRemoveMaxHeap() {
  Shop shop(priorityFn1, MAXHEAP, SKEW, 10, 100004);
  int lastPriority;
  int currentPriority;
  int i;
  
  makeOrders(shop, 300);
  lastPriority = 1000000000;
  
  for (i = 0; i < 300; i++) {
    Order temp = shop.getNextOrder();
    currentPriority = priorityFn1(temp);
    if (i > 0 && currentPriority > lastPriority)
      return false;
    lastPriority = currentPriority;
  }
  
  return shop.numOrders() == 0;
}

bool Tester::testLeftistNPL() {
  Shop shop(priorityFn2, MINHEAP, LEFTIST, 10, 100005);
  makeOrders(shop, 300);
  return checkLeftistNPLValues(shop.m_heap);
}

bool Tester::testLeftistProperty() {
  Shop shop(priorityFn2, MINHEAP, LEFTIST, 10, 100006);
  makeOrders(shop, 300);
  
  if (!checkMinHeap(shop.m_heap, priorityFn2))
    return false;
  
  return checkLeftistPropertyHelper(shop.m_heap);
}

bool Tester::sanityCheck() {
  Shop shop(priorityFn2, MINHEAP, LEFTIST, 10, 100007);
  makeOrders(shop, 300);
  
  if (!checkLeftistPropertyHelper(shop.m_heap))
    return false;
  
  return !isBSTByOrderID(shop.m_heap, MINORDERID - 1, MAXORDERID + 1);
}

bool Tester::testSetPriorityFn() {
  Shop shop(priorityFn2, MINHEAP, SKEW, 10, 100008);
  makeOrders(shop, 300);
  shop.setPriorityFn(priorityFn1, MAXHEAP);
  
  if (!checkMaxHeap(shop.m_heap, priorityFn1))
    return false;
  
  return shop.numOrders() == 300;
}

bool Tester::testSetPriorityFnDataPreserved() {
  Shop shop(priorityFn2, MINHEAP, LEFTIST, 10, 100009);
  Shop copyShop;
  makeOrders(shop, 300);
  copyShop = shop;
  
  shop.setPriorityFn(priorityFn1, MAXHEAP);
  
  if (!sameOrderIDSet(shop.m_heap, copyShop.m_heap))
    return false;
  
  return shop.numOrders() == copyShop.numOrders();
}

bool Tester::testSetStructureToLeftist() {
  Shop shop(priorityFn2, MINHEAP, SKEW, 10, 100010);
  makeOrders(shop, 300);
  
  shop.setStructure(LEFTIST);
  
  if (shop.getStructure() != LEFTIST)
    return false;
  if (!checkMinHeap(shop.m_heap, priorityFn2))
    return false;
  return checkLeftistPropertyHelper(shop.m_heap) &&
    checkLeftistNPLValues(shop.m_heap);
}

bool Tester::testSetStructureToSkew() {
  Shop shop(priorityFn2, MINHEAP, LEFTIST, 10, 100011);
  makeOrders(shop, 300);
  
  shop.setStructure(SKEW);
  
  if (shop.getStructure() != SKEW)
    return false;
  if (!checkMinHeap(shop.m_heap, priorityFn2))
    return false;
  return shop.numOrders() == 300;
}

bool Tester::testMergeEmptyWithNormal() {
  Shop emptyShop(priorityFn2, MINHEAP, SKEW, 10, 100012);
  Shop normalShop(priorityFn2, MINHEAP, SKEW, 10, 100013);
  int before;
  
  makeOrders(normalShop, 300);
  before = normalShop.numOrders();
  
  emptyShop.mergeWithQueue(normalShop);
  
  if (emptyShop.numOrders() != before)
    return false;
  if (normalShop.numOrders() != 0)
    return false;
  return checkMinHeap(emptyShop.m_heap, priorityFn2);
}

bool Tester::testMergeNormalWithNormal() {
  Shop shop1(priorityFn2, MINHEAP, SKEW, 10, 100014);
  Shop shop2(priorityFn2, MINHEAP, SKEW, 10, 100015);
  
  makeOrders(shop1, 200);
  makeOrders(shop2, 150);
  
  shop1.mergeWithQueue(shop2);
  
  if (shop1.numOrders() != 350)
    return false;
  if (shop2.numOrders() != 0)
    return false;
  return checkMinHeap(shop1.m_heap, priorityFn2);
}

bool Tester::testCopyConstructorNormal() {
  Shop original(priorityFn2, MINHEAP, LEFTIST, 10, 100016);
  int copiedRoot;
  
  makeOrders(original, 300);
  
  Shop copied(original);
  
  if (copied.m_heap == original.m_heap)
    return false;
  if (copied.numOrders() != original.numOrders())
    return false;
  if (!sameOrderIDSet(copied.m_heap, original.m_heap))
    return false;
  
  copiedRoot = copied.m_heap->m_orderID;
  original.getNextOrder();
  
  if (copied.m_heap == nullptr)
    return false;
  if (copied.m_heap->m_orderID != copiedRoot)
    return false;
  
  return true;
}

bool Tester::testCopyConstructorEdge() {
  Shop original(priorityFn2, MINHEAP, LEFTIST, 10, 100017);
  Shop copied(original);
  
  return copied.numOrders() == 0 && copied.m_heap == nullptr;
}

bool Tester::testCopyConstructorSingleNode() {
  Shop original(priorityFn2, MINHEAP, SKEW, 10, 100018);
  Order o(COFFEE, ONE, TIER1, 100, 100001, 100101);
  
  original.insertOrder(o);
  Shop copied(original);
  
  if (copied.numOrders() != 1)
    return false;
  if (copied.m_heap == original.m_heap)
    return false;
  if (copied.m_heap == nullptr)
    return false;
  return copied.m_heap->m_orderID == 100101;
}

bool Tester::testAssignmentOperatorNormal() {
  Shop original(priorityFn1, MAXHEAP, SKEW, 10, 100019);
  Shop assigned(priorityFn2, MINHEAP, LEFTIST, 20, 100020);
  int assignedRoot;
  
  makeOrders(original, 300);
  makeOrders(assigned, 50);
  
  assigned = original;
  
  if (assigned.m_heap == original.m_heap)
    return false;
  if (assigned.numOrders() != original.numOrders())
    return false;
  if (!sameOrderIDSet(assigned.m_heap, original.m_heap))
    return false;
  
  assignedRoot = assigned.m_heap->m_orderID;
  original.getNextOrder();
  
  if (assigned.m_heap == nullptr)
    return false;
  if (assigned.m_heap->m_orderID != assignedRoot)
    return false;
  
  return true;
}

bool Tester::testAssignmentOperatorEdge() {
  Shop original(priorityFn2, MINHEAP, SKEW, 10, 100021);
  Shop assigned(priorityFn1, MAXHEAP, LEFTIST, 20, 100022);
  
  makeOrders(assigned, 40);
  assigned = original;
  
  return assigned.numOrders() == 0 && assigned.m_heap == nullptr;
}

bool Tester::testSelfAssignment() {
  Shop shop(priorityFn2, MINHEAP, LEFTIST, 10, 100023);
  int beforeCount;
  
  makeOrders(shop, 300);
  beforeCount = shop.numOrders();
  
  shop = shop;
  
  if (shop.numOrders() != beforeCount)
    return false;
  
  return checkMinHeap(shop.m_heap, priorityFn2);
}

bool Tester::testDequeueEmptyException() {
  Shop shop(priorityFn2, MINHEAP, SKEW, 10, 100024);
  
  try {
    shop.getNextOrder();
  }
  catch (out_of_range&) {
    return true;
  }
  catch (...) {
    return false;
  }
  
  return false;
}

bool Tester::testMergeDifferentPriorityException() {
  Shop shop1(priorityFn1, MAXHEAP, SKEW, 10, 100025);
  Shop shop2(priorityFn2, MINHEAP, SKEW, 10, 100026);
  
  makeOrders(shop1, 50);
  makeOrders(shop2, 50);
  
  try {
    shop1.mergeWithQueue(shop2);
  }
  catch (domain_error&) {
    return true;
  }
  catch (...) {
    return false;
  }
  
  return false;
}

bool Tester::testMergeDifferentHeapTypeException() {
  Shop shop1(priorityFn2, MINHEAP, SKEW, 10, 100027);
  Shop shop2(priorityFn2, MAXHEAP, SKEW, 10, 100028);
  
  makeOrders(shop1, 50);
  makeOrders(shop2, 50);
  
  try {
    shop1.mergeWithQueue(shop2);
  }
  catch (domain_error&) {
    return true;
  }
  catch (...) {
    return false;
  }
  
  return false;
}

bool Tester::testMergeDifferentStructureException() {
  Shop shop1(priorityFn2, MINHEAP, SKEW, 10, 100029);
  Shop shop2(priorityFn2, MINHEAP, LEFTIST, 10, 100030);
  
  makeOrders(shop1, 50);
  makeOrders(shop2, 50);
  
  try {
    shop1.mergeWithQueue(shop2);
  }
  catch (domain_error&) {
    return true;
  }
  catch (...) {
    return false;
  }
  
  return false;
}

bool Tester::testRegionAddShop() {
  Region region(20);
  Shop s1(priorityFn2, MINHEAP, SKEW, 20, 100031);
  Shop s2(priorityFn2, MINHEAP, SKEW, 10, 100032);
  Shop s3(priorityFn2, MINHEAP, SKEW, 30, 100033);
  
  region.addShop(s1);
  region.addShop(s2);
  region.addShop(s3);
  
  if (region.m_size != 3)
    return false;
  
  return checkRegionMinHeap(region.m_heap, region.m_size);
}

bool Tester::testRegionGetShopOrder() {
  Region region(20);
  Shop s1(priorityFn2, MINHEAP, SKEW, 20, 100034);
  Shop s2(priorityFn2, MINHEAP, SKEW, 10, 100035);
  Shop s3(priorityFn2, MINHEAP, SKEW, 30, 100036);
  Shop out;
  
  region.addShop(s1);
  region.addShop(s2);
  region.addShop(s3);
  
  if (!region.getShop(out))
    return false;
  
  if (out.m_regPrior != 10)
    return false;
  
  return checkRegionMinHeap(region.m_heap, region.m_size);
}

bool Tester::testRegionGetNthShop() {
  Region region(20);
  Shop s1(priorityFn2, MINHEAP, SKEW, 40, 100037);
  Shop s2(priorityFn2, MINHEAP, SKEW, 10, 100038);
  Shop s3(priorityFn2, MINHEAP, SKEW, 30, 100039);
  Shop s4(priorityFn2, MINHEAP, SKEW, 20, 100040);
  Shop out;
  
  region.addShop(s1);
  region.addShop(s2);
  region.addShop(s3);
  region.addShop(s4);
  
  if (!region.getNthShop(out, 3))
    return false;
  
  if (out.m_regPrior != 30)
    return false;
  
  return region.m_size == 4 && checkRegionMinHeap(region.m_heap, region.m_size);
}

bool Tester::testRegionGetOrder() {
  Region region(20);
  Shop s1(priorityFn2, MINHEAP, SKEW, 20, 100041);
  Shop s2(priorityFn2, MINHEAP, SKEW, 10, 100042);
  Order out;
  
  makeOrders(s1, 30);
  makeOrders(s2, 30);
  
  region.addShop(s1);
  region.addShop(s2);
  
  if (!region.getOrder(out))
    return false;
  
  if (region.m_size != 2)
    return false;
  
  return checkRegionMinHeap(region.m_heap, region.m_size);
}

bool Tester::testRegionEmptyCases() {
  Region region(10);
  Shop shopOut;
  Order orderOut;
  
  if (region.getShop(shopOut))
    return false;
  if (region.getNthShop(shopOut, 1))
    return false;
  if (region.getOrder(orderOut))
    return false;
  
  return true;
}

bool Tester::testRegionSetPriorityFn() {
  Region region(10);
  Shop s1(priorityFn2, MINHEAP, SKEW, 10, 100043);
  Shop s2(priorityFn2, MINHEAP, SKEW, 20, 100044);
  Shop out;
  
  makeOrders(s1, 50);
  makeOrders(s2, 50);
  
  region.addShop(s1);
  region.addShop(s2);
  
  if (!region.setPriorityFn(priorityFn1, MAXHEAP, 1))
    return false;
  if (!region.getShop(out))
    return false;
  
  if (out.getHeapType() != MAXHEAP)
    return false;
  
  return checkMaxHeap(out.m_heap, priorityFn1);
}

bool Tester::testRegionSetStructure() {
  Region region(10);
  Shop s1(priorityFn2, MINHEAP, SKEW, 10, 100045);
  Shop s2(priorityFn2, MINHEAP, SKEW, 20, 100046);
  Shop out;
  
  makeOrders(s1, 50);
  makeOrders(s2, 50);
  
  region.addShop(s1);
  region.addShop(s2);
  
  if (!region.setStructure(LEFTIST, 1))
    return false;
  if (!region.getShop(out))
    return false;
  
  if (out.getStructure() != LEFTIST)
    return false;
  
  return checkLeftistPropertyHelper(out.m_heap) &&
    checkLeftistNPLValues(out.m_heap);
}

int main() {
  Tester tester;
  
  cout << "testInsertMinHeap: " << (tester.testInsertMinHeap() ? "PASS" : "FAIL") << endl;
  cout << "testInsertMaxHeap: " << (tester.testInsertMaxHeap() ? "PASS" : "FAIL") << endl;
  cout << "testRemoveMinHeap: " << (tester.testRemoveMinHeap() ? "PASS" : "FAIL") << endl;
  cout << "testRemoveMaxHeap: " << (tester.testRemoveMaxHeap() ? "PASS" : "FAIL") << endl;
  cout << "testLeftistNPL: " << (tester.testLeftistNPL() ? "PASS" : "FAIL") << endl;
  cout << "testLeftistProperty: " << (tester.testLeftistProperty() ? "PASS" : "FAIL") << endl;
  cout << "sanityCheck: " << (tester.sanityCheck() ? "PASS" : "FAIL") << endl;
  cout << "testSetPriorityFn: " << (tester.testSetPriorityFn() ? "PASS" : "FAIL") << endl;
  cout << "testSetPriorityFnDataPreserved: " << (tester.testSetPriorityFnDataPreserved() ? "PASS" : "FAIL") << endl;
  cout << "testSetStructureToLeftist: " << (tester.testSetStructureToLeftist() ? "PASS" : "FAIL") << endl;
  cout << "testSetStructureToSkew: " << (tester.testSetStructureToSkew() ? "PASS" : "FAIL") << endl;
  cout << "testMergeEmptyWithNormal: " << (tester.testMergeEmptyWithNormal() ? "PASS" : "FAIL") << endl;
  cout << "testMergeNormalWithNormal: " << (tester.testMergeNormalWithNormal() ? "PASS" : "FAIL") << endl;
  cout << "testCopyConstructorNormal: " << (tester.testCopyConstructorNormal() ? "PASS" : "FAIL") << endl;
  cout << "testCopyConstructorEdge: " << (tester.testCopyConstructorEdge() ? "PASS" : "FAIL") << endl;
  cout << "testCopyConstructorSingleNode: " << (tester.testCopyConstructorSingleNode() ? "PASS" : "FAIL") << endl;
  cout << "testAssignmentOperatorNormal: " << (tester.testAssignmentOperatorNormal() ? "PASS" : "FAIL") << endl;
  cout << "testAssignmentOperatorEdge: " << (tester.testAssignmentOperatorEdge() ? "PASS" : "FAIL") << endl;
  cout << "testSelfAssignment: " << (tester.testSelfAssignment() ? "PASS" : "FAIL") << endl;
  cout << "testDequeueEmptyException: " << (tester.testDequeueEmptyException() ? "PASS" : "FAIL") << endl;
  cout << "testMergeDifferentPriorityException: " << (tester.testMergeDifferentPriorityException() ? "PASS" : "FAIL") << endl;
  cout << "testMergeDifferentHeapTypeException: " << (tester.testMergeDifferentHeapTypeException() ? "PASS" : "FAIL") << endl;
  cout << "testMergeDifferentStructureException: " << (tester.testMergeDifferentStructureException() ? "PASS" : "FAIL") << endl;
  cout << "testRegionAddShop: " << (tester.testRegionAddShop() ? "PASS" : "FAIL") << endl;
  cout << "testRegionGetShopOrder: " << (tester.testRegionGetShopOrder() ? "PASS" : "FAIL") << endl;
  cout << "testRegionGetNthShop: " << (tester.testRegionGetNthShop() ? "PASS" : "FAIL") << endl;
  cout << "testRegionGetOrder: " << (tester.testRegionGetOrder() ? "PASS" : "FAIL") << endl;
  cout << "testRegionEmptyCases: " << (tester.testRegionEmptyCases() ? "PASS" : "FAIL") << endl;
  cout << "testRegionSetPriorityFn: " << (tester.testRegionSetPriorityFn() ? "PASS" : "FAIL") << endl;
  cout << "testRegionSetStructure: " << (tester.testRegionSetStructure() ? "PASS" : "FAIL") << endl;
  
  return 0;
}
