// UMBC - CMSC 341 - Spring 2026 - Proj2
#include "streak.h"
#include <algorithm> 

Streak::Streak(){
  m_root = nullptr;
  m_gridID = 0;
  m_left = nullptr;
  m_right = nullptr;
    
}
Streak::Streak(int grid, Tiger tigers[], int population){
  m_root = nullptr;
  m_gridID = grid;
  m_left = nullptr;
  m_right = nullptr;

  for (int i = 0; i < population; i++){
    insert(tigers[i]);

  }
}
    

Streak::~Streak(){
  clear();
    
}
void Streak::insert(const Tiger& tiger){
  if (tiger.getID() < MINID || tiger.getID() > MAXID)
    return;

  m_root = insertHelper(m_root, tiger);
    
}

void Streak::clear(){
  clearHelper(m_root);
  m_root = nullptr;
    
}

void Streak::remove(int id){
  m_root = removeHelper(m_root, id);
    
}

void Streak::listTigers() const {
  listHelper(m_root);
    
}

bool Streak::setState(int id, STATE state){
  Tiger* current = m_root;

  while (current != nullptr){
    if (id == current->getID()){
      current->setState(state);
      return true;
    }
    else if (id < current->getID())
      current = current->getLeft();
    else
      current = current->getRight();
  }

  return false;
    
}

void Streak::removeDead(){
   removeDeadHelper(m_root);
    
}

bool Streak::findTiger(int id) const {
  return findHelper(m_root, id);
    
}

int Streak::height(Tiger* node){
  if (node == nullptr)
    return -1;
  return node->getHeight();


}
int Streak::getBalance(Tiger* node){
  if (node == nullptr)
    return 0;
  return height(node->getLeft()) - height(node->getRight());


}
Tiger* Streak::minValueNode(Tiger* node){
  Tiger* current = node;
  
  while (current->getLeft() != nullptr)
    current = current->getLeft();
  
  return current;
}
Tiger* Streak::removeHelper(Tiger* node, int id){
  if (node == nullptr)
    return node;
  
  //  BST delete
  if (id < node->getID())
    node->setLeft(removeHelper(node->getLeft(), id));
  else if (id > node->getID())
    node->setRight(removeHelper(node->getRight(), id));
  else {
    // FOUND NODE
    
    // Case 1 & 2: one or no child
    if (node->getLeft() == nullptr || node->getRight() == nullptr){
      Tiger* temp;
      
      if (node->getLeft() != nullptr)
	temp = node->getLeft();
      else
	temp = node->getRight();
      
      // No child
      if (temp == nullptr){
	delete node;
	return nullptr;
      }
      else{
	// One child
	Tiger* old = node;
	node = temp;
	delete old;
      }
        }
    else{
      // Case 3: two children
      Tiger* temp = minValueNode(node->getRight());
      
      // copy successor data
      node->setID(temp->getID());
      node->setAge(temp->getAge());
      node->setGender(temp->getGender());
      node->setState(temp->getState());

      // delete successor
      node->setRight(removeHelper(node->getRight(), temp->getID()));
    }
  }
  if (node == nullptr)
    return node;
  
  // update height
  node->setHeight(1 + max(height(node->getLeft()), height(node->getRight())));

  //  balance
  int balance = getBalance(node);
  
  // Left left
  if (balance > 1 && getBalance(node->getLeft()) >= 0)
    return rotateRight(node);
  
  // Left right
  if (balance > 1 && getBalance(node->getLeft()) < 0){
    node->setLeft(rotateLeft(node->getLeft()));
    return rotateRight(node);
  }

  // Right right
  if (balance < -1 && getBalance(node->getRight()) <= 0)
    return rotateLeft(node);
  
  // Right left
  if (balance < -1 && getBalance(node->getRight()) > 0){
    node->setRight(rotateRight(node->getRight()));
    return rotateLeft(node);
  }
    
  return node;
}
void Streak::listHelper(Tiger* node) const{
  if (node == nullptr)
    return;

  listHelper(node->getLeft());
  
  cout << node->getID() << " "
       << node->getAgeStr() << " "
       << node->getGenderStr() << " "
       << node->getStateStr() << endl;
  
  listHelper(node->getRight());
}
void Streak::removeDeadHelper(Tiger* node){
  if (node == nullptr)
    return;
  
  removeDeadHelper(node->getLeft());
  removeDeadHelper(node->getRight());

  if (node->getState() == DEAD){
    remove(node->getID());
  }
}

bool Streak::findHelper(Tiger* node, int id) const{
  if (node == nullptr)
    return false;
  
  if (id == node->getID())
    return true;
  else if (id < node->getID())
    return findHelper(node->getLeft(), id);
  else
    return findHelper(node->getRight(), id);
}

int Streak::countStateHelper(Tiger* node, STATE state) const{
  if (node == nullptr)
    return 0;
  
  int count = 0;
  
  if (node->getState() == state)
    count = 1;

  return count + countStateHelper(node->getLeft(), state)
    + countStateHelper(node->getRight(), state);
}
int Streak::countAgeHelper(Tiger* node, AGE age) const {
  if (node == nullptr)
    return 0;
  int count = (node->getAge() == age) ? 1 : 0;
  return count + countAgeHelper(node->getLeft(), age) + countAgeHelper(node->getRight(), age);
}





Tiger* Streak::rotateRight(Tiger* y){
  Tiger* x = y->getLeft();
  if (x == nullptr)
    return y;

  Tiger* T2 = x->getRight();
  
  x->setRight(y);
  y->setLeft(T2);
  
  y->setHeight(1 + max(height(y->getLeft()), height(y->getRight())));
  x->setHeight(1 + max(height(x->getLeft()), height(x->getRight())));

  return x;

}

Tiger* Streak::rotateLeft(Tiger* x){
  Tiger* y = x->getRight();
  if (y == nullptr)
    return x;

  Tiger* T2 = y->getLeft();
  
  y->setLeft(x);
  x->setRight(T2);

  x->setHeight(1 + max(height(x->getLeft()), height(x->getRight())));
  y->setHeight(1 + max(height(y->getLeft()), height(y->getRight())));
  
  return y;
}
Tiger* Streak::insertHelper(Tiger* node, const Tiger& tiger){
  if (node == nullptr){
    Tiger* newNode = new Tiger(tiger.getID(), tiger.getAge(), tiger.getGender(), tiger.getState());
    return newNode;
  }

  if (tiger.getID() < node->getID())
    node->setLeft(insertHelper(node->getLeft(), tiger));
  else if (tiger.getID() > node->getID())
    node->setRight(insertHelper(node->getRight(), tiger));
  else
    return node; // no duplicates
  
  // update height
  node->setHeight(1 + max(height(node->getLeft()), height(node->getRight())));
  
  int balance = getBalance(node);
  
  // left left
  if (balance > 1 && tiger.getID() < node->getLeft()->getID())
    return rotateRight(node);
  
  // right right
  if (balance < -1 && tiger.getID() > node->getRight()->getID())
    return rotateLeft(node);
  
  // Left right
  if (balance > 1 && tiger.getID() > node->getLeft()->getID()){
    node->setLeft(rotateLeft(node->getLeft()));
    return rotateRight(node);
  }

  // right left
  if (balance < -1 && tiger.getID() < node->getRight()->getID()){
    node->setRight(rotateRight(node->getRight()));
    return rotateLeft(node);
  }

  return node;
}
void Streak::clearHelper(Tiger* node){
  if (node == nullptr)
    return;

  clearHelper(node->getLeft());
  clearHelper(node->getRight());


  delete node;



}

int Streak::count(AGE age) const{
  return countAgeHelper(m_root, age);
    
}

int Streak::count(STATE state) const{
  return countStateHelper(m_root, state);
    
}

void Streak::dumpTree() const {
  dump(m_root);
}

void Streak::dump(Tiger* aTiger) const {
  if (aTiger != nullptr){
    cout << "(";
    dump(aTiger->m_left);                    // visit left child
    cout << aTiger->m_id << ":" << aTiger->m_height; // print node info
    dump(aTiger->m_right);                   // visit right child
    cout << ")";
  }
}

//////////////////////////////////////////////////////////////////////
Grid::Grid(){
  m_root = nullptr;
    
}

Grid::~Grid(){
  clearHelper(m_root);
  m_root = nullptr;
    
}

bool Grid::insert(int grid, Tiger tigers[], int population){
  if (m_root == nullptr){
    m_root = new Streak(grid, tigers, population);
    return true;
  }

  // splay closest node to root
  m_root = splay(m_root, grid);

  // duplicate
  if (m_root->m_gridID == grid)
    return false;

  Streak* newNode = new Streak(grid, tigers, population);

  if (grid < m_root->m_gridID){
    newNode->m_right = m_root;
    newNode->m_left = m_root->m_left;
    m_root->m_left = nullptr;
  }
  else{
    newNode->m_left = m_root;
    newNode->m_right = m_root->m_right;
    m_root->m_right = nullptr;
  }

  m_root = newNode;
  return true;
  
}

int Grid::count(int grid, STATE state){
  if (m_root == nullptr)
    return 0;
  
  m_root = splay(m_root, grid);
  
  if (m_root->m_gridID != grid)
    return 0;
  
  return m_root->count(state);
  
}
int Grid::count(int grid, AGE age){
  if (m_root == nullptr)
    return 0;

  m_root = splay(m_root, grid);
  
  if (m_root->m_gridID != grid)
    return 0;

  return m_root->count(age);
    
}
bool Grid::removeTiger(int grid, int tiger, bool all){
  if (m_root == nullptr)
    return false;

  m_root = splay(m_root, grid);

  if (m_root->m_gridID != grid)
    return false;

  if (all)
    m_root->removeDead();
  else
    m_root->remove(tiger);
  
  return true;
    
}
int Grid::getGridHeight(int grid){
  if (m_root == nullptr)
    return -1;

  m_root = splay(m_root, grid);
  
  if (m_root->m_gridID != grid)
    return -1;
  
  if (m_root->m_root == nullptr)
    return -1;

  return m_root->m_root->getHeight();
  
}
bool Grid::setState(int grid, int tiger, STATE state){
   if (m_root == nullptr)
        return false;

    m_root = splay(m_root, grid);

    if (m_root->m_gridID != grid)
        return false;

    return m_root->setState(tiger, state);
    
}
void Grid::clearHelper(Streak* node){
  if (node == nullptr)
    return;

  clearHelper(node->m_left);
  clearHelper(node->m_right);

  delete node; // calls Streak destructor automatically
}

Streak* Grid::findHelper(Streak* root, int key){
  return splay(root, key);
}
Streak* Grid::rotateRight(Streak* x){
  Streak* y = x->m_left;
  x->m_left = y->m_right;
  y->m_right = x;
  return y;
}
Streak* Grid::rotateLeft(Streak* x){
    Streak* y = x->m_right;
    x->m_right = y->m_left;
    y->m_left = x;
    return y;
}
Streak* Grid::splay(Streak* root, int key){
  if (root == nullptr || root->m_gridID == key)
    return root;
  
  // left side
  if (key < root->m_gridID){
    if (root->m_left == nullptr)
      return root;
    
    // Zig-Zig
    if (key < root->m_left->m_gridID){
      root->m_left->m_left = splay(root->m_left->m_left, key);
      root = rotateRight(root);
    }
    // Zig-Zag
    else if (key > root->m_left->m_gridID){
      root->m_left->m_right = splay(root->m_left->m_right, key);
      if (root->m_left->m_right != nullptr)
	root->m_left = rotateLeft(root->m_left);
    }
    
    return (root->m_left == nullptr) ? root : rotateRight(root);
  }

  // right side
  else{
    if (root->m_right == nullptr)
      return root;
    
    // Zig-Zig
    if (key > root->m_right->m_gridID){
      root->m_right->m_right = splay(root->m_right->m_right, key);
      root = rotateLeft(root);
    }
    // Zig-Zag
    else if (key < root->m_right->m_gridID){
      root->m_right->m_left = splay(root->m_right->m_left, key);
      if (root->m_right->m_left != nullptr)
	root->m_right = rotateRight(root->m_right);
    }
    
    return (root->m_right == nullptr) ? root : rotateLeft(root);
  }
}

void Grid::dump(bool verbose) const {
  dumpHelper(m_root, verbose);
  cout << endl;
}

void Grid::dumpHelper(Streak* root, bool verbose) const {
  if (root != nullptr) {
    cout << "(";
    dumpHelper(root->m_left, verbose);
    if (verbose) {
      // Print grid ID and the root tiger's ID of that streak
      if (root->m_root != nullptr)
	cout << root->m_gridID << ":" << root->m_root->m_id;
      else
	cout << root->m_gridID << ":null";
    } else {
      cout << root->m_gridID;
    }
    dumpHelper(root->m_right, verbose);
    cout << ")";
  }
}
