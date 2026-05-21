// UMBC - CMSC 341 - Spring 2026 - Proj0
#ifndef ART_H
#define ART_H
#include <iostream>
#include <random>
#include <string>
using namespace std;
const std::string RESET = "\x1b[0m";
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};
class Random {
 public:
 Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
      if (type == NORMAL){
	//the case of NORMAL to generate integer numbers with normal distribution
	m_generator = std::mt19937(m_device());
	//the data set will have the mean of 50 (default) and standard deviation of 20 (default)
	//the mean and standard deviation can change by passing new values to constructor 
	m_normdist = std::normal_distribution<>(mean,stdev);
      }
      else if (type == UNIFORMINT) {
	//the case of UNIFORMINT to generate integer numbers
	// Using a fixed seed value generates always the same sequence
	// of pseudorandom numbers, e.g. reproducing scientific experiments
	// here it helps us with testing since the same sequence repeats
	m_generator = std::mt19937(10);// 10 is the fixed seed value
	m_unidist = std::uniform_int_distribution<>(min,max);
      }
      else{ //the case of UNIFORMREAL to generate real numbers
	m_generator = std::mt19937(10);// 10 is the fixed seed value
	m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
      }
    }
  void setSeed(int seedNum){
    // we have set a default value for seed in constructor
    // we can change the seed by calling this function after constructor call
    // this gives us more randomness
    m_generator = std::mt19937(seedNum);
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
    result = std::floor(result*100.0)/100.0;
    return result;
  }
    
 private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;//normal distribution
  std::uniform_int_distribution<> m_unidist;//integer uniform distribution
  std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Art{
 public:
  friend class Grader;
  friend class Tester;
  Art();
  Art(int height, int width);
  ~Art();
  Art(const Art& rhs);
  const Art& operator=(const Art& rhs);
  void clear();
  void create(int initiate);//creates random colors in the cells
  void dumpColors(string pixel) const;//for debugging purposes
  void dumpValues() const;//for debugging purposes
  bool left2Right(const Art& rhs);
  bool top2Bottom(const Art& bottom);
  bool reverse();
  bool rotate();

 private:
  int m_height;
  int m_width;
  int ** m_painting;// the 2d structure to store color cells

  /******************************************
   * Private function declarations go here! *
   ******************************************/
};

#endif
