// CMSC 341 - Spring 2026 - Project 3
#include "shop.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
  Random(){}
  Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
  {
    if (type == NORMAL){
      //the case of NORMAL to generate integer numbers with normal distribution
      m_generator = mt19937(m_device());
      //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
      //the mean and standard deviation can change by passing new values to constructor 
      m_normdist = normal_distribution<>(mean,stdev);
    }
    else if (type == UNIFORMINT) {
      //the case of UNIFORMINT to generate integer numbers
      // Using a fixed seed value generates always the same sequence
      // of pseudorandom numbers, e.g. reproducing scientific experiments
      // here it helps us with testing since the same sequence repeats
      m_generator = mt19937(10);// 10 is the fixed seed value
      m_unidist = uniform_int_distribution<>(min,max);
    }
    else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
      m_generator = mt19937(10);// 10 is the fixed seed value
      m_uniReal = uniform_real_distribution<double>((double)min,(double)max);
    }
    else { //the case of SHUFFLE to generate every number only once
      m_generator = mt19937(m_device());
    }
  }
  void setSeed(int seedNum){
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = mt19937(seedNum);
  }
  void init(int min, int max){
    m_min = min;
    m_max = max;
    m_type = UNIFORMINT;
    m_generator = mt19937(10);// 10 is the fixed seed value
    m_unidist = uniform_int_distribution<>(min,max);
  }
  void getShuffle(vector<int> & array){
    // this function provides a list of all values between min and max
    // in a random order, this function guarantees the uniqueness
    // of every value in the list
    // the user program creates the vector param and passes here
    // here we populate the vector using m_min and m_max
    for (int i = m_min; i<=m_max; i++){
      array.push_back(i);
    }
    shuffle(array.begin(),array.end(),m_generator);
  }
  
  void getShuffle(int array[]){
    // this function provides a list of all values between min and max
    // in a random order, this function guarantees the uniqueness
    // of every value in the list
    // the param array must be of the size (m_max-m_min+1)
    // the user program creates the array and pass it here
    vector<int> temp;
    for (int i = m_min; i<=m_max; i++){
      temp.push_back(i);
    }
    shuffle(temp.begin(), temp.end(), m_generator);
    vector<int>::iterator it;
    int i = 0;
    for (it=temp.begin(); it != temp.end(); it++){
      array[i] = *it;
      i++;
    }
  }
  
  int getRandNum(){
    // this function returns integer numbers
    // the object must have been initialized to generate integers
    int result = 0;
    if(m_type == NORMAL){
      //returns a random number in a set with normal distribution
      //we limit random numbers by the min and max values
      result = m_min - 1;
      while(result < m_min || result > m_max)
	result = m_normdist(m_generator);
    }
    else if (m_type == UNIFORMINT){
      //this will generate a random number between min and max values
      result = m_unidist(m_generator);
    }
    return result;
  }
  
  double getRealRandNum(){
    // this function returns real numbers
    // the object must have been initialized to generate real numbers
    double result = m_uniReal(m_generator);
    // a trick to return numbers only with two deciaml points
    // for example if result is 15.0378, function returns 15.03
    // to round up we can use ceil function instead of floor
    result = floor(result*100.0)/100.0;
    return result;
  }
  
  string getRandString(int size){
    // the parameter size specifies the length of string we ask for
    // to use ASCII char the number range in constructor must be set to 97 - 122
    // and the Random type must be UNIFORMINT (it is default in constructor)
    string output = "";
    for (int i=0;i<size;i++){
      output = output + (char)getRandNum();
    }
    return output;
  }
  
  int getMin(){return m_min;}
  int getMax(){return m_max;}
private:
  int m_min;
  int m_max;
  RANDOM m_type;
  random_device m_device;
  mt19937 m_generator;
  normal_distribution<> m_normdist;//normal distribution
  uniform_int_distribution<> m_unidist;//integer uniform distribution
  uniform_real_distribution<double> m_uniReal;//real uniform distribution
  
};

// the followings are sample priority functions to be used by Shop class
// Priority functions compute an integer priority for a order.
int priorityFn1(const Order &order);// works with a MAXHEAP
int priorityFn2(const Order &order);// works with a MINHEAP

class Tester{
public:
private:
};

int main(){
  Random shopGen(10,30); // this generates same priority numbers too
  Random orderIDGen(MINORDERID,MAXORDERID);
  Random countGen(ONE, DOZEN);
  Random customerIDGen(MINCUSTID,MAXCUSTID);
  Random membershipGen(TIER1,TIER6);
  Random pointsGen(MINPOINTS,MAXPOINTS);
  Random itemGen(COFFEE,ICEDTEA);
  Random rndShopID(SHOPMINID, SHOPMAXID);
  // create a queue of multiple shops
  Region shopsQueue(20);
  
  cout << "Creating 10 shops and adding them to the system:" << endl;
  for (int j=0;j<10;j++){
    int rndShop = shopGen.getRandNum();
        int shopID = rndShopID.getRandNum();
        // create a Shop object
        Shop aShop(priorityFn2, MINHEAP, SKEW, rndShop, shopID);
        
        for (int i=0;i<5;i++){
	  // create multiple Order objects
	  Order anOrder(static_cast<ITEM>(itemGen.getRandNum()), 
                        static_cast<COUNT>(countGen.getRandNum()), 
                        static_cast<MEMBERSHIP>(membershipGen.getRandNum()),
                        pointsGen.getRandNum(), 
                        customerIDGen.getRandNum(),
                        orderIDGen.getRandNum());
	  // insert orders into shop
	  aShop.insertOrder(anOrder);
        }
        cout << "Shop Priority: " << rndShop << endl;
        aShop.dump();
        // insert shop into shops queue
        shopsQueue.addShop(aShop);
  }
  
  cout << endl;
  cout << "Checking the tree of shops:" << endl;
  shopsQueue.dump();
  
  Shop highPrioShop;
  
  cout << endl << "Get the highest priority shop:" << endl;
  shopsQueue.getShop(highPrioShop);
  cout << endl; highPrioShop.dump();
  
  cout << endl << "Get the 3rd highest priority shop:" << endl;
  shopsQueue.getNthShop(highPrioShop, 3);
  cout << endl; highPrioShop.dump();
  
  cout << endl << "Checking the tree of shops:" << endl;
  cout << endl; shopsQueue.dump();
  
  cout << "Changing the priority function in the highest priority shop:" << endl;
  shopsQueue.setPriorityFn(priorityFn1, MAXHEAP, 1);
  
  cout << endl << "Get the highest priority shop, it must be a max heap now:" << endl;
  shopsQueue.getShop(highPrioShop);
  cout << endl; highPrioShop.dump();
  
  cout << endl << "Get the highest priority shop, this one be a min heap:" << endl;
  shopsQueue.getShop(highPrioShop); 
  cout << endl; highPrioShop.dump();
  
  cout << "\nPreorder traversal of the nodes in the highest priority shop:\n";
  highPrioShop.printOrdersQueue();
  
  cout << endl;
  return 0;
}

int priorityFn1(const Order &order) {
  //this function works with a MAXHEAP
  //priority value is determined based on some criteria
  //priority value falls in the range [0-5003]
  //the highest priority would be 3+5000 = 5003
  //the lowest priority would be 0+0 = 0 => 1 
  //the larger value means the higher priority
  int priority = static_cast<int>(order.getCount()) + order.getPoints();
  if (priority == 0 ) priority = 1;
  return priority;
}

int priorityFn2(const Order &order) {
  //this function works with a MINHEAP
  //priority value is determined based on some criteria
  //priority value falls in the range [0-10]
  //the highest priority would be 0+0 = 0 => 1
  //the lowest priority would be 5+5 =10
  //the smaller value means the higher priority
  int priority = static_cast<int>(order.getItem()) + static_cast<int>(order.getMemebership());
  if (priority == 0 ) priority = 1;
  return priority;
}
