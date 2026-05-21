// UMBC - CMSC 341 - Spring 2026 - Proj2
#include "streak.h"
#include <iostream>
#include <cmath>
using namespace std;

// ---------------- RANDOM CLASS ----------------
class Random {
public:
  Random(int min, int max) {
    m_min = min;
    m_max = max;
    m_seed = 10;
  }

  int getRandNum(){
    m_seed = (m_seed * 1103515245 + 12345) % 2147483647;
    return m_min + (m_seed % (m_max - m_min + 1));
  }

private:
  int m_min, m_max, m_seed;
};

// ---------------- Tester Class ----------------
class Tester {
public:

  // ---------- Avl Helpers ----------
  int checkHeight(Tiger* node){
    if (!node) return -1;
    
    int lh = checkHeight(node->getLeft());
    int rh = checkHeight(node->getRight());
    
    if (node->getHeight() != 1 + max(lh, rh))
      return -1000;
    
    return 1 + max(lh, rh);
  }

  bool isBalanced(Tiger* node) {
    if (!node) return true;
    int lh = node->getLeft() ? node->getLeft()->getHeight() : -1;
    int rh = node->getRight() ? node->getRight()->getHeight() : -1;
    
    if (abs(lh - rh) > 1) return false;
    
    return isBalanced(node->getLeft()) && isBalanced(node->getRight());
  }
  
  bool isBST(Tiger* node, int min, int max) {
    if (!node) return true;
    
    if (node->getID() <= min || node->getID() >= max)
      return false;
    
    return isBST(node->getLeft(), min, node->getID()) &&
      isBST(node->getRight(), node->getID(), max);
  }

  // ---------- splay helpers ----------
  bool isSplayBST(Streak* node, int min, int max) {
    if (!node) return true;

    if (node->m_gridID <= min || node->m_gridID >= max)
      return false;
    
    return isSplayBST(node->m_left, min, node->m_gridID) &&
      isSplayBST(node->m_right, node->m_gridID, max);
  }

  // ---------- tests ----------
  
  bool testAVLInsertBalance() {
    Streak s;
    Random r(MINID, MAXID);
    
    for (int i = 0; i < 100; i++)
      s.insert(Tiger(r.getRandNum()));

    return isBalanced(s.m_root) && checkHeight(s.m_root) != -1000;
  }
  
  bool testBSTProperty() {
    Streak s;
    Random r(MINID, MAXID);
    
    for (int i = 0; i < 100; i++)
      s.insert(Tiger(r.getRandNum()));
    
    return isBST(s.m_root, MINID - 1, MAXID + 1);
  }
  
  bool testAVLRemoveBalance() {
    Streak s;
    
    for (int i = 0; i < 50; i++)
      s.insert(Tiger(10000 + i));
    
    for (int i = 0; i < 25; i++)
      s.remove(10000 + i);

    return isBalanced(s.m_root);
  }

  bool testBSTAfterRemovals() {
    Streak s;
    
    for (int i = 0; i < 50; i++)
      s.insert(Tiger(10000 + i));
    
    for (int i = 0; i < 25; i++)
      s.remove(10000 + i);
    
    return isBST(s.m_root, MINID - 1, MAXID + 1);
  }
  
  bool testMultipleRemovals() {
    Streak s;
    
    for (int i = 0; i < 50; i++)
      s.insert(Tiger(10000 + i));
    
    for (int i = 0; i < 50; i++)
      s.remove(10000 + i);
    
    return s.m_root == nullptr;
  }
  
  bool testFindTigerMultiple() {
    Streak s;
    
    for (int i = 0; i < 50; i++)
      s.insert(Tiger(10000 + i));
    
    for (int i = 0; i < 50; i++)
      if (!s.findTiger(10000 + i))
        return false;
    
    return true;
  }
  
  // ---------- remove dead ----------
  bool testRemoveDeadBasic() {
    Streak s;
    
    s.insert(Tiger(10001, CUB, MALE, ALIVE));
    s.insert(Tiger(10002, CUB, MALE, DEAD));
    s.insert(Tiger(10003, OLD, MALE, DEAD));
    s.insert(Tiger(10004, OLD, MALE, ALIVE));
    
    s.removeDead();
    
    return (!s.findTiger(10002) &&
            !s.findTiger(10003) &&
	    s.findTiger(10001) &&
	    s.findTiger(10004));
  }
  
  bool testRemoveDeadAll() {
    Streak s;
    
    s.insert(Tiger(10001, CUB, MALE, DEAD));
    s.insert(Tiger(10002, OLD, MALE, DEAD));
    
    s.removeDead();
    
    return s.m_root == nullptr;
  }
  
  bool testRemoveDeadBalanced() {
    Streak s;

    for (int i = 0; i < 50; i++) {
      STATE st = (i % 2 == 0) ? DEAD : ALIVE;
      s.insert(Tiger(10000 + i, CUB, MALE, st));
    }

    s.removeDead();

    return isBalanced(s.m_root);
  }
  
  // ---------- splay tests ----------
  bool testSplayInsertRoot() {
    Grid g;
    Tiger t[1];
    
    for (int i = 0; i < 10; i++) {
      t[0] = Tiger(10000 + i);
      g.insert(100 + i, t, 1);
      
      if (g.m_root->m_gridID != 100 + i)
        return false;
    }
    
    return true;
  }
  
  bool testGridCountSplay() {
    Grid g;
    Tiger t[1] = {Tiger(10001)};
    
    g.insert(200, t, 1);
    g.insert(300, t, 1);

    g.count(200, ALIVE);
    
    return g.m_root->m_gridID == 200;
  }
  
  bool testGridSetStateSplay() {
    Grid g;
    Tiger t[1] = {Tiger(12345, CUB, MALE, ALIVE)};
    
    g.insert(300, t, 1);
    g.insert(400, t, 1);
    
    g.setState(300, 12345, DEAD);
    
    return g.m_root->m_gridID == 300;
  }
  
  bool testSplayBSTProperty() {
    Grid g;
    Tiger t[1] = {Tiger(10001)};
    
    for (int i = 0; i < 20; i++)
      g.insert(100 + i, t, 1);

    for (int i = 0; i < 20; i++)
      g.count(100 + i, ALIVE);
    
    return isSplayBST(g.m_root, -1, 100000);
  }
  
  // ---------- run all ----------
  void runAll() {
    cout << "AVL Insert Balance: " << (testAVLInsertBalance() ? "PASS" : "FAIL") << endl;
    cout << "BST Property: " << (testBSTProperty() ? "PASS" : "FAIL") << endl;
    cout << "AVL Remove Balance: " << (testAVLRemoveBalance() ? "PASS" : "FAIL") << endl;
    cout << "BST After Removals: " << (testBSTAfterRemovals() ? "PASS" : "FAIL") << endl;
    cout << "Multiple Removals: " << (testMultipleRemovals() ? "PASS" : "FAIL") << endl;
    cout << "Find Tiger Multiple: " << (testFindTigerMultiple() ? "PASS" : "FAIL") << endl;
    
    cout << "RemoveDead Basic: " << (testRemoveDeadBasic() ? "PASS" : "FAIL") << endl;
    cout << "RemoveDead All: " << (testRemoveDeadAll() ? "PASS" : "FAIL") << endl;
    cout << "RemoveDead Balanced: " << (testRemoveDeadBalanced() ? "PASS" : "FAIL") << endl;

    cout << "Splay Insert Root: " << (testSplayInsertRoot() ? "PASS" : "FAIL") << endl;
    cout << "Grid Count Splay: " << (testGridCountSplay() ? "PASS" : "FAIL") << endl;
    cout << "Grid SetState Splay: " << (testGridSetStateSplay() ? "PASS" : "FAIL") << endl;
    cout << "Splay BST Property: " << (testSplayBSTProperty() ? "PASS" : "FAIL") << endl;
  }
};

// ---------------- main ----------------
int main() {
  Tester t;
  t.runAll();
  return 0;
}
