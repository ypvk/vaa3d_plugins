#ifndef THIN_H
#define THIN_H
#include <v3d_interface.h>

template <class T> /*T for image data type*/
class Thin
{
  public:
    Thin(unsigned char* data_org, V3DLONG size_x, V3DLONG size_y, V3DLONG size_z);
    ~Thin();
    void setXDim(V3DLONG size_x);
    void setYDim(V3DLONG size_y);
    void setZDim(V3DLONG size_z); 
    V3DLONG getXDim() const;
    V3DLONG getYDim() const;
    V3DLONG getZDim() const;
    T* getData() const;
  private:
    T* data;
    V3DLONG size_x;
    V3DLONG size_y;
    V3DLoNG size_z;
}

#endif
