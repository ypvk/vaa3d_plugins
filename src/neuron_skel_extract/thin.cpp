#include "thin.h"
#include <cstdio>
#include <cstring>

Thin::Thin(unsigned char* data_org, V3DLONG size_x, V3DLONG size_y, V3DLONG size_z)
{
  T* data_tmp = (T*)data_org;
  this->size_x = size_x;
  this->size_y = size_y;
  this->size_z = size_z;
  try {
    this->data = new T[size_x*size_y*size_z];
  }catch(std::bad_alloc& exc)
  {
    printf("error to alloca memory");
    return;
  }
  memset(this->data, 0, sizeof(this->data));
  //init the value just for test
  int i, j, k;
  for(i = 0; i < size_x; i++)
    for(j = 0; j < size_y; j++)
      for(k = 0; k < size_z; k++)
      {
          if(data_org[i + j*size_x + k*size_x*size_y] > 0)
          {
            this->data[i + j*size_x + k*size_x*size_y] = 255
          }
      }
}
Thin::~Thin()
{
}

void Thin::setXDim(V3DLONG size_x)
{
  this->size_x = size_x;
}

void Thin::setYDim(V3DLONG size_y)
{
  this->size_y = size_y;
}

void Thin::setZDim(V3DLONG size_z)
{
  this->size_z = size_z;
}

V3DLONG Thin::getXDim() const 
{
  return this->size_x;
}
V3DLONG Thin::getYDim() const
{
  return this->size_y;
}
V3DLONG Thin::getZDim() const
{
  return this->size_z;
}
T* Thin::getData() const
{
  return this->data;
}
