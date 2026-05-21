#include "art.h"
#include <iostream>

using namespace std;

class Tester {
public:
  bool testCreateNormal();
  bool testConstructorError();
  bool testConstructorEdge();
  bool testCopyConstructorNormal();
  bool testCopyConstructorEdge();
  bool testAssignmentNormal();
  bool testAssignmentEdge();
  bool testAssignmentSelf();
  bool testLeft2RightNormal();
  bool testLeft2RightError();
  bool testLeft2RightEmptyLeft();
  bool testLeft2RightEmptyRight();
  bool testLeft2RightSelf();
};

// TEST FUNCTIONS

// create() normal case
bool Tester::testCreateNormal() {
  Art a(5, 5);
  a.create(10);
  
  if (a.m_painting == nullptr)
    return false;
  
  for (int i = 0; i < a.m_height; i++) {
    for (int j = 0; j < a.m_width; j++) {
      if (a.m_painting[i][j] < 10 || a.m_painting[i][j] > 99)
	return false;
    }
  }
  return true;
}

// constructor error case
bool Tester::testConstructorError() {
  Art a(-5, -10);
  return (a.m_height == 0 &&
	  a.m_width == 0 &&
	  a.m_painting == nullptr);
}

// constructor edge case
bool Tester::testConstructorEdge() {
  Art a(1, 1);
  return (a.m_height == 1 &&
	  a.m_width == 1 &&
	  a.m_painting != nullptr);
}

// copy constructor normal
bool Tester::testCopyConstructorNormal() {
  Art a(3, 3);
  a.create(7);
  
  Art b(a);
  
  if (a.m_painting == b.m_painting)
    return false;
  
  for (int i = 0; i < a.m_height; i++) {
    for (int j = 0; j < a.m_width; j++) {
      if (a.m_painting[i][j] != b.m_painting[i][j])
	return false;
    }
  }
  return true;
}

// copy constructor edge case
bool Tester::testCopyConstructorEdge() {
  Art a;
  Art b(a);
  return (b.m_height == 0 &&
	  b.m_width == 0 &&
	  b.m_painting == nullptr);
}

// assignment operator normal
bool Tester::testAssignmentNormal() {
  Art a(4, 4);
  a.create(5);
  
  Art b;
  b = a;
  
  if (a.m_painting == b.m_painting)
    return false;

  for (int i = 0; i < a.m_height; i++) {
    for (int j = 0; j < a.m_width; j++) {
      if (a.m_painting[i][j] != b.m_painting[i][j])
	return false;
    }
  }
  return true;
}

// assignment operator edge case
bool Tester::testAssignmentEdge() {
  Art a;
  Art b(2, 2);
  b = a;
  return (b.m_height == 0 &&
	  b.m_width == 0 &&
	  b.m_painting == nullptr);
}

// assignment self-assignment
bool Tester::testAssignmentSelf() {
  Art a(2, 2);
  a.create(3);
  a = a;
  return (a.m_painting != nullptr);
}

// left2Right normal
bool Tester::testLeft2RightNormal() {
  Art a(2, 2);
  Art b(2, 3);
  a.create(1);
  b.create(2);
  return a.left2Right(b);
}

// left2Right error case
bool Tester::testLeft2RightError() {
  Art a(2, 2);
  Art b(3, 2);
  return !a.left2Right(b);
}

// left2Right empty left
bool Tester::testLeft2RightEmptyLeft() {
  Art a;
  Art b(2, 2);
  b.create(4);
  return a.left2Right(b);
}

// left2Right empty right
bool Tester::testLeft2RightEmptyRight() {
  Art a(2, 2);
  Art b;
  a.create(4);
  return a.left2Right(b);
}

// left2Right self append
bool Tester::testLeft2RightSelf() {
  Art a(2, 2);
  a.create(3);
  return a.left2Right(a);
}

// MAIN 

int main() {
    Tester t;

    cout << "testCreateNormal: " << (t.testCreateNormal() ? "pass" : "fail") << endl;
    cout << "testConstructorError: " << (t.testConstructorError() ? "pass" : "fail") << endl;
    cout << "testConstructorEdge: " << (t.testConstructorEdge() ? "pass" : "fail") << endl;
    cout << "testCopyConstructorNormal: " << (t.testCopyConstructorNormal() ? "pass" : "fail") << endl;
    cout << "testCopyConstructorEdge: " << (t.testCopyConstructorEdge() ? "pass" : "fail") << endl;
    cout << "testAssignmentNormal: " << (t.testAssignmentNormal() ? "pass" : "fail") << endl;
    cout << "testAssignmentEdge: " << (t.testAssignmentEdge() ? "pass" : "fail") << endl;
    cout << "testAssignmentSelf: " << (t.testAssignmentSelf() ? "pass" : "fail") << endl;
    cout << "testLeft2RightNormal: " << (t.testLeft2RightNormal() ? "pass" : "fail") << endl;
    cout << "testLeft2RightError: " << (t.testLeft2RightError() ? "pass" : "fail") << endl;
    cout << "testLeft2RightEmptyLeft: " << (t.testLeft2RightEmptyLeft() ? "pass" : "fail") << endl;
    cout << "testLeft2RightEmptyRight: " << (t.testLeft2RightEmptyRight() ? "pass" : "fail") << endl;
    cout << "testLeft2RightSelf: " << (t.testLeft2RightSelf() ? "pass" : "fail") << endl;

    return 0;
}
