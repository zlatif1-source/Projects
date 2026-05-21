// UMBC - CMSC 341 - Spring 2026 - Proj0
#include "art.h"
//initalizes all variables (empty object)
Art::Art(){
  m_height = 0;
  m_width = 0;
  m_painting = nullptr;
  
}
//This is the constructor. It initializes all member variables and it allocates required memory if necessary. If the user passes invalid parameters such a a negative size the constructor creates an empty object.
Art::Art(int height, int width){
  if (height <= 0 || width <= 0) {
    m_height = 0;
    m_width = 0;
    m_painting = nullptr;
    return;
  }
  

  m_height = height;
  m_width = width;

  m_painting = new int*[m_height];

  for(int i = 0; i < m_height; i++) {
    m_painting[i] = new int[m_width];
  }
}
  
    
//This is the destructor and it deallocates the memory.
Art::~Art(){
  if (m_painting != nullptr) {
    for (int i = 0; i < m_height; i++) {
      delete[] m_painting[i];
    }
    delete[] m_painting;
  }

}
//This function deallocates all memory and reinitializes all member variables to default values. It clears the current object to an empty object.
void Art::clear(){
  if (m_painting != nullptr) {
    for (int i = 0; i < m_height; i++) {
      delete[] m_painting[i];
    }
    delete[] m_painting;
  }
  m_painting = nullptr;
  m_height = 0;
  m_width = 0;

}

    
//generates random color code and inserts it into m_painting.
void Art::create(int initiate){
  if (m_painting == nullptr)
    return;

  Random generator(10,99,UNIFORMINT);

  generator.setSeed(initiate);

  for(int i = 0; i < m_height; i++) {
    for(int j = 0; j < m_width; j++) {
      m_painting[i][j] = generator.getRandNum();
    }
  }
}
void Art::dumpColors(string pixel) const{
  if (m_height > 0 && m_width > 0 && m_painting != nullptr){
    for (int i=1;i <= m_height;i++){
      for (int j=1;j<=m_width;j++){
	std::cout << "\x1b[38;5;" << to_string(m_painting[i-1][j-1]) << "m" << pixel << pixel << RESET;
      }
      cout << endl;
    }
    cout << endl;
  }
}
void Art::dumpValues() const{
  if (m_height > 0 && m_width > 0 && m_painting != nullptr){
    for (int i=1;i <= m_height;i++){
      for (int j=1;j<=m_width;j++){
	cout << m_painting[i-1][j-1] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
}
Art::Art(const Art& rhs){
  m_height = rhs.m_height;
  m_width = rhs.m_width;

  if (rhs.m_painting == nullptr){
    m_painting = nullptr;
    return;
  }
  m_painting = new int*[m_height];
  for (int i = 0; i < m_height; i++){
    m_painting[i] = new int[m_width];
  }

  for(int i = 0; i < m_height; i++){
    for(int j = 0; j < m_width; j++){
      m_painting[i][j] = rhs.m_painting[i][j];
    }
  }
	
}
const Art& Art::operator=(const Art& rhs){
  if (this == &rhs)
    return *this;

  
  clear();

    
  m_height = rhs.m_height;
  m_width = rhs.m_width;

   
  if (rhs.m_painting == nullptr) {
    m_painting = nullptr;
    return *this;
  }

   
  m_painting = new int*[m_height];
  for (int i = 0; i < m_height; i++) {
    m_painting[i] = new int[m_width];
  }

   
  for (int i = 0; i < m_height; i++) {
    for (int j = 0; j < m_width; j++) {
      m_painting[i][j] = rhs.m_painting[i][j];
    }
  }

  return *this;
}

bool Art::left2Right(const Art& rhs){
  //case 1
  if (rhs.m_painting == nullptr || rhs.m_width == 0) {
    return true;
  }
  
  //case 2
  if (m_painting == nullptr || m_width == 0) {
    *this = rhs;
    return true;
  }
  
  //heights match
  if (m_height != rhs.m_height) {
    return false;
  }
  
  //save a copy of the current object 
  Art temp(*this);
  
  //new dimensions
  int newWidth = m_width + rhs.m_width;
  
  //clear current memory
  clear();
  
  //allocate new memory
  m_height = temp.m_height;
  m_width = newWidth;
  
  m_painting = new int*[m_height];
  for (int i = 0; i < m_height; i++) {
    m_painting[i] = new int[m_width];
  }
  
  //copy left side (original object)
  for (int i = 0; i < m_height; i++) {
    for (int j = 0; j < temp.m_width; j++) {
      m_painting[i][j] = temp.m_painting[i][j];
    }
  }
  
  //copy right side (rhs)
  for (int i = 0; i < m_height; i++) {
    for (int j = 0; j < rhs.m_width; j++) {
      m_painting[i][j + temp.m_width] = rhs.m_painting[i][j];
    }
  }

  return true;
}    

bool Art::top2Bottom(const Art& bottom){
  //case 1
  if (bottom.m_painting == nullptr || bottom.m_height == 0) {
    return true;
  }
  
  //case 2
  if (m_painting == nullptr || m_height == 0) {
    *this = bottom;
    return true;
  }

  //widths match
  if (m_width != bottom.m_width) {
    return false;
  }

  //save a copy of the current object
  Art temp(*this);

  //new dimensions
  int newHeight = m_height + bottom.m_height;

  //clear current memory
  clear();

  //allocate new memory
  m_height = newHeight;
  m_width = temp.m_width;

  m_painting = new int*[m_height];
  for (int i = 0; i < m_height; i++) {
    m_painting[i] = new int[m_width];
  }

  //copy top part (original object)
  for (int i = 0; i < temp.m_height; i++) {
    for (int j = 0; j < m_width; j++) {
      m_painting[i][j] = temp.m_painting[i][j];
    }
  }

  //copy bottom part (bottom object)
  for (int i = 0; i < bottom.m_height; i++) {
    for (int j = 0; j < m_width; j++) {
      m_painting[i + temp.m_height][j] = bottom.m_painting[i][j];
    }
  }

  return true;
}
    

bool Art::reverse(){
  if (m_painting == nullptr || m_width == 0 || m_height == 0) {
    return false;
  }

    //reverse each row
  for (int i = 0; i < m_height; i++) {
    for (int j = 0; j < m_width / 2; j++) {
      int temp = m_painting[i][j];
      m_painting[i][j] = m_painting[i][m_width - 1 - j];
      m_painting[i][m_width - 1 - j] = temp;
    }
  }

  return true;
  
}
bool Art::rotate(){
  //cannot rotate empty object
  if (m_painting == nullptr || m_height == 0 || m_width == 0) {
    return false;
  }
  
  //height and width must both be even
  if (m_height % 2 != 0 || m_width % 2 != 0) {
    return false;
  }
  
  //save a copy of current object
  Art temp(*this);
  
  //new dimensions after rotation
  int newHeight = m_width;
  int newWidth = m_height;
  
  //clear current memory
  clear();
  
  //allocate new rotated structure
  m_height = newHeight;
  m_width = newWidth;
  
  m_painting = new int*[m_height];
  for (int i = 0; i < m_height; i++) {
    m_painting[i] = new int[m_width];
  }

  //perform rotation 
  for (int i = 0; i < temp.m_height; i++) {
    for (int j = 0; j < temp.m_width; j++) {
      m_painting[j][temp.m_height - 1 - i] = temp.m_painting[i][j];
    }
  }

  return true;
  
}
