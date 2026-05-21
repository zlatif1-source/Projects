// UMBC - CMSC 341 - Spring 2026 - Proj1
#ifndef FUEL_H
#define FUEL_H
#include <iostream>
using namespace std;
// default values
const int MINCAP = 2000; // Minimum Capacity in Kg
const int DEFCAP = 5000; // Default capacity in Kg
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class Pump;  //forward declaration
class FuelSys;//forward declaration
class Tank{
 public:
  friend class Tester;
  friend class Grader;
  friend class FuelSys;
  Tank();
  Tank(int ID, int tankCap, int tankFuel = 0, 
       Pump* pumpList = nullptr, Tank* nextTank = nullptr)
    {
      m_tankID = ID; m_tankCapacity = tankCap; m_tankFuel = tankFuel;
      m_pumps = pumpList; m_next = nextTank;
    }
 private:
  int m_tankID;       // unique identifier
  int m_tankCapacity;
  int m_tankFuel;     // current amount of fuel in the tank
  Pump * m_pumps;
  Tank * m_next;
};
class Pump{
 public:
  friend class Tester;
  friend class Grader;
  friend class FuelSys;
  Pump();
  Pump(int ID, int target, Pump* nextPump = nullptr){
    m_pumpID = ID; m_target = target;
    m_next = nextPump;
  }
 private:
  int m_pumpID;   // unique identifier
  int m_target;   // ID of the target tank for transfer
  Pump * m_next;
};
class FuelSys{
 public:
  friend class Tester;
  friend class Grader;
  FuelSys();
  ~FuelSys();
  const FuelSys & operator=(const FuelSys & rhs);
  bool addTank(int tankID, int capacity, int tankFuel);
  bool removeTank(int tankID);
  bool findTank(int tankID); // if tank is found, it becomes the next of current
  bool addPump(int tankID, int pumpID, int targetTank);
  bool removePump(int tankID, int pumpID);
  bool fill(int tankID, int fuel);    // fill the tank with fuel
  // transfer fuel from a tank through a pump
  bool drain(int tankID, int pumpID, int fuel);
  int totalFuel() const;  // return the total fuel of airplane
  // the dump function is provided to facilitate debugging
  // using dump function for test cases is not accepted
  void dumpSys() const;
  void dumpPumps(Pump* pumps) const;
 private:
  Tank * m_current;
  
  // ***************************************************
  // Any private helper functions must be delared here!
  // ***************************************************
  
};
#endif
