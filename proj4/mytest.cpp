// CMSC 341 - Spring 2026 - Project 4
// mytest.cpp

#include "vdetect.h"
#include <random>

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL};

class Random {
public:
  Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20)
    : m_min(min), m_max(max), m_type(type) {
    if (type == NORMAL) {
      m_generator = std::mt19937(m_device());
      m_normdist = std::normal_distribution<>(mean, stdev);
    }
    else if (type == UNIFORMINT) {
      m_generator = std::mt19937(10);
      m_unidist = std::uniform_int_distribution<>(min, max);
    }
    else {
      m_generator = std::mt19937(10);
      m_uniReal = std::uniform_real_distribution<double>((double)min, (double)max);
    }
  }

  void setSeed(int seedNum) {
    m_generator = std::mt19937(seedNum);
  }
  
  int getRandNum() {
    int result = 0;
    
    if (m_type == NORMAL) {
      result = m_min - 1;
      while (result < m_min || result > m_max) {
	result = m_normdist(m_generator);
      }
    }
    else if (m_type == UNIFORMINT) {
      result = m_unidist(m_generator);
    }
    
    return result;
  }
  
  double getRealRandNum() {
    double result = m_uniReal(m_generator);
    result = std::floor(result * 100.0) / 100.0;
    return result;
  }
  
private:
  int m_min;
  int m_max;
  RANDOM m_type;
  std::random_device m_device;
  std::mt19937 m_generator;
  std::normal_distribution<> m_normdist;
  std::uniform_int_distribution<> m_unidist;
  std::uniform_real_distribution<double> m_uniReal;
};

unsigned int hashCode(const string str);
unsigned int constantHash(const string str);
string sequencer(int size, int seedNum);
string makeKey(int num);

class Tester {
public:
  void report(string name, bool result);
  
  bool testConstructorSizing();
  bool testInsertNonColliding();
  bool testInsertColliding();
  bool testFindMissing();
  bool testFindNonColliding();
  bool testFindColliding();
  bool testRemoveNonColliding();
  bool testRemoveColliding();
  bool testDuplicateInsert();
  bool testInvalidIDInsert();
  bool testUpdateID();
  bool testChangePolicyUsedAfterRehash();
  bool testRehashTriggeredByLoad();
  bool testRehashCompleteByLoad();
  bool testRehashTriggeredByDeleteRatio();
  bool testRehashCompleteByDeleteRatio();
  bool testDeletedSlotReusable();
  bool testRemoveFromOldTable();
  bool testFindDuringRehash();
  bool testDestructorStress();
};

int main() {
  Tester tester;
  
  tester.report("testConstructorSizing", tester.testConstructorSizing());
  tester.report("testInsertNonColliding", tester.testInsertNonColliding());
  tester.report("testInsertColliding", tester.testInsertColliding());
  tester.report("testFindMissing", tester.testFindMissing());
  tester.report("testFindNonColliding", tester.testFindNonColliding());
  tester.report("testFindColliding", tester.testFindColliding());
  tester.report("testRemoveNonColliding", tester.testRemoveNonColliding());
  tester.report("testRemoveColliding", tester.testRemoveColliding());
  tester.report("testDuplicateInsert", tester.testDuplicateInsert());
  tester.report("testInvalidIDInsert", tester.testInvalidIDInsert());
  tester.report("testUpdateID", tester.testUpdateID());
  tester.report("testChangePolicyUsedAfterRehash", tester.testChangePolicyUsedAfterRehash());
  tester.report("testRehashTriggeredByLoad", tester.testRehashTriggeredByLoad());
  tester.report("testRehashCompleteByLoad", tester.testRehashCompleteByLoad());
  tester.report("testRehashTriggeredByDeleteRatio", tester.testRehashTriggeredByDeleteRatio());
  tester.report("testRehashCompleteByDeleteRatio", tester.testRehashCompleteByDeleteRatio());
  tester.report("testDeletedSlotReusable", tester.testDeletedSlotReusable());
  tester.report("testRemoveFromOldTable", tester.testRemoveFromOldTable());
  tester.report("testFindDuringRehash", tester.testFindDuringRehash());
  tester.report("testDestructorStress", tester.testDestructorStress());
  
  return 0;
}

void Tester::report(string name, bool result) {
  if (result) {
    cout << name << ": PASS" << endl;
  }
  else {
    cout << name << ": FAIL" << endl;
  }
}

// Function: constructor
// Case: small, large, and non-prime table sizes.
// Expected: capacity is adjusted to valid prime bounds.
bool Tester::testConstructorSizing() {
  VDetect small(10, hashCode, QUADRATIC);
  VDetect large(200000, hashCode, LINEAR);
  VDetect nonPrime(200, hashCode, DOUBLEHASH);
  
  bool result = true;
  
  result = result && small.m_currentCap == MINPRIME;
  result = result && large.m_currentCap == MAXPRIME;
  result = result && nonPrime.m_currentCap > 200;
  result = result && nonPrime.isPrime(nonPrime.m_currentCap);
  result = result && small.m_currentTable != nullptr;
  result = result && small.m_oldTable == nullptr;
  result = result && small.m_currentSize == 0;
  result = result && small.m_currNumDeleted == 0;
  
  return result;
}

// Function: insert
// Case: 50 mostly non-colliding values.
// Expected: values are inserted and size updates correctly.
bool Tester::testInsertNonColliding() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    Virus virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(virus);
    result = result && table.getVirus(virus.getKey(), virus.getID()) == virus;
  }
  
  result = result && table.m_currentSize == 50;
  result = result && table.m_currNumDeleted == 0;
  result = result && table.m_oldTable == nullptr;
  
  return result;
}

// Function: insert
// Case: 50 values with the same hash value.
// Expected: collision handling places all values correctly.
bool Tester::testInsertColliding() {
  VDetect table(MINPRIME, constantHash, DOUBLEHASH);
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    Virus virus(makeKey(i), idGen.getRandNum(), true);
    int expected = (7 + (i * 4)) % MINPRIME;
    
    result = result && table.insert(virus);
    result = result && table.m_currentTable[expected] != nullptr;
    result = result && table.m_currentTable[expected]->getKey() == virus.getKey();
    result = result && table.m_currentTable[expected]->getID() == virus.getID();
  }
  
  result = result && table.m_currentSize == 50;

  return result;
}

// Function: getVirus
// Case: object does not exist.
// Expected: EMPTY object is returned.
bool Tester::testFindMissing() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    Virus virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(virus);
  }
  
  Virus missing = table.getVirus("AAAAA", 111111);
  
  result = result && missing.getKey() == "";
  result = result && missing.getID() == 0;
  
  return result;
}

// Function: getVirus
// Case: finding 50 inserted normal values.
// Expected: every inserted value can be found.
bool Tester::testFindNonColliding() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus data[50];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  for (int i = 0; i < 50; i++) {
    Virus found = table.getVirus(data[i].getKey(), data[i].getID());
    result = result && found == data[i];
  }
  
  return result;
}

// Function: getVirus
// Case: finding 50 colliding values.
// Expected: probing continues through collisions and finds all live values.
bool Tester::testFindColliding() {
  VDetect table(MINPRIME, constantHash, DOUBLEHASH);
  Virus data[50];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    data[i] = Virus(makeKey(i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  for (int i = 0; i < 50; i++) {
    Virus found = table.getVirus(data[i].getKey(), data[i].getID());
    result = result && found == data[i];
  }
  
  return result;
}

// Function: remove
// Case: remove several normal values.
// Expected: removed values are not found, size includes deleted buckets.
bool Tester::testRemoveNonColliding() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus data[50];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  result = result && table.remove(data[5]);
  result = result && table.remove(data[15]);
  result = result && table.remove(data[25]);
  
  result = result && table.getVirus(data[5].getKey(), data[5].getID()) == EMPTY;
  result = result && table.getVirus(data[15].getKey(), data[15].getID()) == EMPTY;
  result = result && table.getVirus(data[25].getKey(), data[25].getID()) == EMPTY;
  
  result = result && table.m_currentSize == 50;
  result = result && table.m_currNumDeleted == 3;
  
  return result;
}

// Function: remove
// Case: remove colliding values.
// Expected: deleted buckets do not break search for later colliding values.
bool Tester::testRemoveColliding() {
  VDetect table(MINPRIME, constantHash, DOUBLEHASH);
  Virus data[50];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    data[i] = Virus(makeKey(i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  result = result && table.remove(data[10]);
  result = result && table.remove(data[20]);
  result = result && table.remove(data[30]);
  
  result = result && table.getVirus(data[10].getKey(), data[10].getID()) == EMPTY;
  result = result && table.getVirus(data[20].getKey(), data[20].getID()) == EMPTY;
  result = result && table.getVirus(data[30].getKey(), data[30].getID()) == EMPTY;
  
  result = result && table.getVirus(data[11].getKey(), data[11].getID()) == data[11];
  result = result && table.getVirus(data[21].getKey(), data[21].getID()) == data[21];
  result = result && table.getVirus(data[31].getKey(), data[31].getID()) == data[31];
  
  return result;
}

// Function: insert
// Case: duplicate object.
// Expected: second insert fails and size does not increase.
bool Tester::testDuplicateInsert() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus virus("AACGT", 123456, true);
  bool result = true;
  
  result = result && table.insert(virus);
  result = result && !table.insert(virus);
  result = result && table.m_currentSize == 1;
  
  return result;
}

// Function: insert
// Case: invalid low and high IDs.
// Expected: insert fails and table remains empty.
bool Tester::testInvalidIDInsert() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus low("AACGT", MINID - 1, true);
  Virus high("AACGA", MAXID + 1, true);
  bool result = true;
  
  result = result && !table.insert(low);
  result = result && !table.insert(high);
  result = result && table.m_currentSize == 0;
  
  return result;
}

// Function: updateID
// Case: update existing object to valid ID.
// Expected: old ID is gone, new ID is searchable.
bool Tester::testUpdateID() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus virus("AACGT", 123456, true);
  bool result = true;
  
  result = result && table.insert(virus);
  result = result && table.updateID(virus, 654321);
  
  Virus oldVirus = table.getVirus("AACGT", 123456);
  Virus newVirus = table.getVirus("AACGT", 654321);
  
  result = result && oldVirus == EMPTY;
  result = result && newVirus.getKey() == "AACGT";
  result = result && newVirus.getID() == 654321;
  result = result && !table.updateID(newVirus, MAXID + 1);
  
  return result;
}

// Function: changeProbPolicy
// Case: policy changed before rehash.
// Expected: old table keeps old policy and new table uses requested policy.
bool Tester::testChangePolicyUsedAfterRehash() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Random idGen(MINID, MAXID);
  bool result = true;
  
  table.changeProbPolicy(LINEAR);
  
  for (int i = 0; i < 51; i++) {
    Virus virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(virus);
  }
  
  result = result && table.m_oldTable != nullptr;
  result = result && table.m_oldProbing == QUADRATIC;
  result = result && table.m_currProbing == LINEAR;
  
  return result;
}

// Function: rehash
// Case: load factor exceeds 50 percent.
// Expected: rehash begins and old table exists temporarily.
bool Tester::testRehashTriggeredByLoad() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 51; i++) {
    Virus virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(virus);
  }
  
  result = result && table.m_oldTable != nullptr;
  result = result && table.m_oldCap == MINPRIME;
  result = result && table.m_currentCap > MINPRIME;
  result = result && table.m_transferIndex > 0;
  
  return result;
}

// Function: rehash
// Case: continue operations after load-triggered rehash.
// Expected: all live data transfers and old table is removed.
bool Tester::testRehashCompleteByLoad() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus data[60];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 60; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  result = result && table.m_oldTable == nullptr;
  
  for (int i = 0; i < 60; i++) {
    result = result && table.getVirus(data[i].getKey(), data[i].getID()) == data[i];
  }
  
  result = result && table.m_currNumDeleted == 0;
  
  return result;
}

// Function: rehash
// Case: deleted ratio exceeds 80 percent.
// Expected: rehash begins because deleted ratio is too high.
bool Tester::testRehashTriggeredByDeleteRatio() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus data[50];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  for (int i = 0; i < 41; i++) {
    result = result && table.remove(data[i]);
  }
  
  result = result && table.m_oldTable != nullptr;
  result = result && table.m_oldCap == MINPRIME;
  result = result && table.m_transferIndex > 0;
  
  return result;
}

// Function: rehash
// Case: finish delete-ratio-triggered rehash.
// Expected: deleted nodes are removed and live nodes remain searchable.
bool Tester::testRehashCompleteByDeleteRatio() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus data[60];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 50; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  for (int i = 0; i < 41; i++) {
    result = result && table.remove(data[i]);
  }
  
  for (int i = 50; i < 60; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  result = result && table.m_oldTable == nullptr;
  
  for (int i = 0; i < 41; i++) {
    result = result && table.getVirus(data[i].getKey(), data[i].getID()) == EMPTY;
  }
  
  for (int i = 41; i < 60; i++) {
    result = result && table.getVirus(data[i].getKey(), data[i].getID()) == data[i];
  }
  
  return result;
}

// Function: insert/remove
// Case: remove one item and insert another colliding item.
// Expected: deleted slot can be reused.
bool Tester::testDeletedSlotReusable() {
  VDetect table(MINPRIME, constantHash, DOUBLEHASH);
  Random idGen(MINID, MAXID);
  Virus data[10];
  bool result = true;
  
  for (int i = 0; i < 10; i++) {
    data[i] = Virus(makeKey(i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  int oldSize = table.m_currentSize;
  
  result = result && table.remove(data[3]);
  
  Virus replacement("REPLACEMENT", idGen.getRandNum(), true);
  result = result && table.insert(replacement);
  
  result = result && table.getVirus(data[3].getKey(), data[3].getID()) == EMPTY;
  result = result && table.getVirus(replacement.getKey(), replacement.getID()) == replacement;
  result = result && table.m_currentSize == oldSize;
  result = result && table.m_currNumDeleted == 0;
  
  return result;
}

// Function: remove
// Case: remove an item that is still in old table during incremental rehash.
// Expected: remove succeeds from old table.
bool Tester::testRemoveFromOldTable() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus data[55];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 51; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  result = result && table.m_oldTable != nullptr;
  
  bool removedOne = false;
  
  for (int i = 0; i < 51 && removedOne == false; i++) {
    int index = table.findIndex(table.m_oldTable,
				table.m_oldCap,
				table.m_oldProbing,
				data[i].getKey(),
				data[i].getID());
    
    if (index != -1) {
      result = result && table.remove(data[i]);
      result = result && table.getVirus(data[i].getKey(), data[i].getID()) == EMPTY;
      removedOne = true;
    }
  }
  
  result = result && removedOne;
  
  return result;
}

// Function: getVirus
// Case: finding values while incremental rehash is active.
// Expected: search checks both current and old tables.
bool Tester::testFindDuringRehash() {
  VDetect table(MINPRIME, hashCode, QUADRATIC);
  Virus data[51];
  Random idGen(MINID, MAXID);
  bool result = true;
  
  for (int i = 0; i < 51; i++) {
    data[i] = Virus(sequencer(5, i), idGen.getRandNum(), true);
    result = result && table.insert(data[i]);
  }
  
  result = result && table.m_oldTable != nullptr;
  
  for (int i = 0; i < 51; i++) {
    result = result && table.getVirus(data[i].getKey(), data[i].getID()) == data[i];
  }
  
  return result;
}

// Function: destructor
// Case: create and destroy many tables.
// Expected: program finishes; valgrind should report no leaks/errors.
bool Tester::testDestructorStress() {
  bool result = true;
  
  for (int round = 0; round < 50; round++) {
    VDetect table(MINPRIME, hashCode, QUADRATIC);
    Random idGen(MINID, MAXID);
    
    for (int i = 0; i < 60; i++) {
      Virus virus(sequencer(5, i + round), idGen.getRandNum(), true);
      result = result && table.insert(virus);
    }
  }
  
  return result;
}

unsigned int hashCode(const string str) {
  unsigned int val = 0;
  const unsigned int thirtyThree = 33;
  int i = 0;
  
  while (i < (int)str.length()) {
    val = val * thirtyThree + str[i];
    i++;
  }
  
  return val;
}

unsigned int constantHash(const string str) {
  return 7;
}

string sequencer(int size, int seedNum) {
  string sequence = "";
  Random rndObject(0, 3);
  rndObject.setSeed(seedNum);
  
  for (int i = 0; i < size; i++) {
    sequence = sequence + ALPHA[rndObject.getRandNum()];
  }
  
  return sequence;
}

string makeKey(int num) {
  string key = "KEY";
  key = key + std::to_string(num);
  return key;
}
