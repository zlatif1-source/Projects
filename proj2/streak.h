// UMBC - CMSC 341 - Spring 2026 - Proj2
#ifndef STREAK_H
#define STREAK_H
#include <iostream>
using namespace std;
class Grader; // this class is for grading purposes, no need to do anything
class Tester; // this is your tester class, you add your test functions in this class
class Streak; // this is the class that you implement
class Grid;   // this is the class that you implement
enum STATE {ALIVE, DEAD};
enum AGE {CUB, YOUNG, OLD};
enum GENDER {MALE, FEMALE, UNKNOWN};
const int MINID = 10000;
const int MAXID = 99999;
#define DEFAULT_HEIGHT 0
#define DEFAULT_ID 0 // this is for empty object
#define DEFAULT_STATE ALIVE
#define DEFAULT_AGE CUB
#define DEFAULT_GENDER UNKNOWN
class Tiger{
public:
  friend class Grader;
  friend class Tester;
  friend class Streak;
  friend class Grid;
  Tiger(int id, AGE age = DEFAULT_AGE, GENDER gender = DEFAULT_GENDER, STATE state = DEFAULT_STATE)
    :m_id(id),m_age(age),m_gender(gender),m_state(state) {
    m_left = nullptr;
    m_right = nullptr;
    m_height = DEFAULT_HEIGHT;
  }
  Tiger(){
    m_id = DEFAULT_ID;
    m_state = DEFAULT_STATE;
    m_age = DEFAULT_AGE;
    m_gender = DEFAULT_GENDER;
    m_left = nullptr;
    m_right = nullptr;
    m_height = DEFAULT_HEIGHT;
  }
  int getID() const {return m_id;}
  STATE getState() const {return m_state;}
  string getStateStr() const {
    string text = "";
    switch (m_state)
      {
      case DEAD:text = "DEAD";break;
      case ALIVE:text = "ALIVE";break;
      default:text = "UNKNOWN";break;
      }
    return text;
  }
  AGE getAge() const {return m_age;}
  string getAgeStr() const {
    string text = "";
    switch (m_age)
      {
      case CUB:text = "CUB";break;
      case YOUNG:text = "YOUNG";break;
      case OLD:text = "OLD";break;
      default:text = "UNKNOWN";break;
      }
    return text;
    }
  GENDER getGender() const {return m_gender;}
  string getGenderStr() const {
    string text = "";
    switch (m_gender)
      {
      case MALE:text = "MALE";break;
      case FEMALE:text = "FEMALE";break;
      case UNKNOWN:text = "UNKNOWN";break; 
      default:text = "UNKNOWN";break;
      }
    return text
      ;
  }
  int getHeight() const {return m_height;}
  Tiger* getLeft() const {return m_left;}
  Tiger* getRight() const {return m_right;}
  void setID(const int id){m_id=id;}
  void setState(STATE state){m_state=state;}
  void setAge(AGE age){m_age=age;}
  void setGender(GENDER gender){m_gender=gender;}
  void setHeight(int height){m_height=height;}
  void setLeft(Tiger* left){m_left=left;}
  void setRight(Tiger* right){m_right=right;}
 private:
  int m_id; // unique ID transmitted from a tiger wearing a transmitter
  AGE m_age;
  GENDER m_gender;
  STATE m_state;
  Tiger* m_left;  // the pointer to the left child in the AVL tree
  Tiger* m_right; // the pointer to the right child in the AVL tree
  int m_height;   // the height of node in the AVL tree
};
class Streak{
public:
  friend class Grader;
  friend class Tester;
  friend class Grid;
  Streak();
  Streak(int grid, Tiger tigers[], int population);
  ~Streak();
  void insert(const Tiger& tiger);
  void clear();
  void remove(int id);
  void dumpTree() const;
  void listTigers() const;
  bool setState(int id, STATE state);
  void removeDead();//remove all dead tigers from the tree
  bool findTiger(int id) const;//returns true if the tiger is in tree
  int count(STATE state) const;
  int count(AGE age) const;
private:
  Tiger* m_root;  // the root of the AVL tree
  int m_gridID;   // unique ID of the grid
  Streak* m_left;
  Streak* m_right;
  
  void dump(Tiger* aTiger) const;// helper for recursive traversal
  Tiger* insertHelper(Tiger* node, const Tiger& tiger);
  
  int height(Tiger* node);
  int getBalance(Tiger* node);
  
  Tiger* rotateLeft(Tiger* x);
  Tiger* rotateRight(Tiger* y);
  
  void clearHelper(Tiger* node);
  
  Tiger* removeHelper(Tiger* node, int id);
  Tiger* minValueNode(Tiger* node);
  bool findHelper(Tiger* node, int id) const;
  int countStateHelper(Tiger* node, STATE state) const;
  void listHelper(Tiger* node) const;
  void removeDeadHelper(Tiger* node);
  int countAgeHelper(Tiger* node, AGE age) const;
  
  // ***************************************************
  // Any private helper functions must be delared here!
  // ***************************************************

};

class Grid{
public:
  friend class Grader;
  friend class Tester;
  Grid();
  ~Grid();
  void dump(bool verbose=false) const;
  bool insert(int grid, Tiger tigers[], int population);
  int count(int grid, STATE state);
  int count(int grid, AGE age);
  bool removeTiger(int grid, int tiger, bool all);
  bool setState(int grid, int tiger, STATE state);
  int getGridHeight(int grid);
  
private:
  Streak* m_root;// the root of the Splay tree
  
  void dumpHelper(Streak* root, bool verbose=false) const;
  
  Streak* splay(Streak* root, int key);
  Streak* rotateLeft(Streak* x);
  Streak* rotateRight(Streak* x);
  Streak* insertHelper(Streak* root, Streak* node);
  Streak* findHelper(Streak* root, int key);
  void clearHelper(Streak* node);
  
  // ***************************************************
  // Any private helper functions must be delared here!
  // ***************************************************

};
#endif
