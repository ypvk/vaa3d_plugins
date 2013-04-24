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
    bool run();
  private:
    bool mathesAsTemplate(bool n[3][3][3]);
    bool transformNeighorhood(bool n[3][3][3], char direction, bool USn[3][3][3]);
    void markBoundaryInDirection(char direction);
    void initNeiborhood();
    void copyNeiborhood(V3DLONG idx, bool n[3][3][3]);
  private:
    T* data;
    V3DLONG size_x;
    V3DLONG size_y;
    V3DLONG size_z;
    V3DLONG *nb; // 18 elems
    V3DLONG *dataNeighbors; //27 elems
};
#include "thin.cpp"
#endif
