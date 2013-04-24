// implement thining templates given by
// Palagyi and Kuda in "A Parallel 3D 12-Subiteration Thinning Algorithm"
#ifndef THIN_CPP
#define THIN_CPP

#include "thin.h"
#include <cstdio>
#include <cstring>

#define OBJECT 255
#define D_BORDER 100
#define SIMPLE 200

enum Direction {
  UP_SOUTH = 0,
  NORTH_EAST,
  WEST_DOWN,

  EAST_SOUTH,
  UP_WEST,
  NORTH_DOWN,

  SOUTH_WEST,
  UP_NORTH,
  EAST_DOWN,

  NORTH_WEST,
  UP_EAST,
  SOUTH_DOWN,
  
  UP,
  DOWN,
  EAST,
  WEST,
  NORTH,
  SOUTH
};
  

template <class T>
Thin<T>::Thin(unsigned char* data_org, V3DLONG size_x, V3DLONG size_y, V3DLONG size_z)
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
            this->data[i + j*size_x + k*size_x*size_y] = OBJECT;
          }
      }
  this->initNeiborhood();
}
template <class T>
Thin<T>::~Thin()
{
  delete[] this->nb;
  delete[] this->dataNeighbors;
}

template <class T>
void Thin<T>::setXDim(V3DLONG size_x)
{
  this->size_x = size_x;
}

template <class T>
void Thin<T>::setYDim(V3DLONG size_y)
{
  this->size_y = size_y;
}

template <class T>
void Thin<T>::setZDim(V3DLONG size_z)
{
  this->size_z = size_z;
}

template <class T>
V3DLONG Thin<T>::getXDim() const 
{
  return this->size_x;
}
template <class T>
V3DLONG Thin<T>::getYDim() const
{
  return this->size_y;
}
template <class T>
V3DLONG Thin<T>::getZDim() const
{
  return this->size_z;
}
template <class T>
T* Thin<T>::getData() const
{
  return this->data;
}
template <class T>
bool Thin<T>::mathesAsTemplate(bool n[3][3][3])
{
  // T1
  if((n[1][1][1] && n[1][0][1]) 
      &&
      (!n[0][2][0] && !n[1][2][0] && !n[2][2][0] &&
       !n[0][2][1] && !n[1][2][1] && !n[2][2][1] &&
       !n[0][2][2] && !n[1][2][2] && !n[2][2][2])
      &&
      (n[0][0][0] || n[1][0][0] || n[2][0][0] ||
       n[0][0][1]               || n[2][0][1] ||
       n[0][0][2] || n[1][0][2] || n[2][0][2] ||
       n[0][1][0] || n[1][1][0] || n[2][1][0] ||
       n[0][1][1]               || n[2][1][1] ||
       n[0][1][2] || n[1][1][2] || n[2][1][2])
    )
  {
    return true;
  }

  // T2
  if((n[1][1][1] && n[1][1][2]) 
      &&
      (!n[0][0][0] && !n[1][0][0]  && !n[2][0][0] &&
       !n[0][1][0] && !n[1][1][0]  && !n[2][1][0] &&
       !n[0][2][0] && !n[1][2][0]  && !n[2][2][0])
      &&
      (n[0][0][1] || n[1][0][1] || n[2][0][1] ||
       n[0][1][1]               || n[2][1][1] ||
       n[0][2][1] || n[1][2][1] || n[2][2][1] ||
       n[0][0][2] || n[1][0][2] || n[2][0][2] ||
       n[0][1][2]               || n[2][1][2] ||
       n[0][2][2] || n[1][2][2] || n[2][2][2])
    )
  {
    return true;
  }

  // T3
  if((n[1][1][1] && n[1][0][2]) 
      &&
      (!n[0][0][0] && !n[1][0][0] && !n[2][0][0] &&
       !n[0][1][0] && !n[1][1][0] && !n[2][1][0] &&
       !n[0][2][0] && !n[1][2][0] && !n[2][2][0] &&
       !n[0][2][1] && !n[1][2][1] && !n[2][2][1] &&
       !n[0][2][2] && !n[1][2][2] && !n[2][2][2])
      &&
      (n[0][0][1] || n[2][0][1] || n[0][0][2] || n[2][0][2] ||
       n[0][1][1] || n[2][1][1] || n[0][1][2] || n[2][1][2])
    )
  {
    return true;
  }

  // T4
  if((n[1][1][1] && n[1][0][1] && n[1][1][2])
      &&
      (!n[1][1][0] && !n[0][2][0] && !n[1][2][0] && !n[2][2][0] && !n[1][2][1])
      &&
      !(n[0][1][0] && n[0][2][1])
      &&
      !(n[2][1][0] && n[2][2][1])
    )
  {
    return true;
  }

  // T5
  if((n[1][1][1] && n[1][0][1] && n[1][1][2] && n[2][2][0])
      &&
      (!n[1][1][0] && !n[0][2][0] && !n[1][2][0] && !n[1][2][1])
      &&
      !(n[0][1][0] && n[0][2][1]) 
      &&
      ((!n[2][1][0] && n[2][2][1]) || (n[2][1][0] && !n[2][2][1]))
    )
  {
    return true;
  }

  // T6
  if((n[1][1][1] && n[1][0][1] && n[1][1][2] && n[0][2][0]) 
      &&
      (!n[1][1][0] && !n[1][2][0] && !n[2][2][0] && !n[1][2][1])
      && 
      ((!n[0][1][0] && n[0][2][1]) || (n[0][1][0] && !n[0][2][1]))
      &&
      !(n[2][1][0] && n[2][2][1])
    )
  {
    return true;
  }

  // T7
  if((n[1][1][1] && n[1][0][1] && n[2][1][1] && n[1][1][2])
      &&
      (!n[1][1][0] && !n[0][2][0] && !n[1][2][0] && !n[1][2][1])
      &&
      !(n[0][1][0] && n[0][2][1])
    )
  {
    return true;
  }

  // T8
  if((n[1][1][1] && n[1][0][1] && n[0][1][1] && n[1][1][2])
      &&
      (!n[1][1][0] && !n[1][2][0] && !n[2][2][0] && !n[1][2][1])
      &&
      !(n[2][1][0] && n[2][2][1])
    )
  {
    return true;
  }

  // T9
  if((n[1][1][1] && n[1][0][1] && n[2][1][1] && n[1][1][2] && n[0][2][0])
      &&
      (!n[1][1][0] && !n[1][2][0] && !n[1][2][1])
      &&
      ((n[0][1][0] && !n[0][2][1]) || (!n[0][1][0] && n[0][2][1]))
    )
  {
    return true;
  }

  // T10
  if((n[1][1][1] && n[1][0][1] && n[0][1][1] && n[1][1][2] && n[2][2][0])
      &&
      (!n[1][1][0] && !n[1][2][0] && !n[1][2][1])
      &&
      ((n[2][1][0] && !n[2][2][1]) || (!n[2][1][0] && n[2][2][1]))
    )
  {
    return true;
  }

  // T11
  if((n[1][1][1] && n[2][0][1] && n[1][0][2])
      &&
      (!n[0][0][0] && !n[1][0][0] && 
       !n[0][1][0] && !n[1][1][0] && 
       !n[0][2][0] && !n[1][2][0] && !n[2][2][0] &&
       !n[0][2][1] && !n[1][2][1] && !n[2][2][1] &&
       !n[0][2][2] && !n[1][2][2] && !n[2][2][2])
    )
  {
    return true;
  }

  // T12
  if((n[1][1][1] && n[0][0][1] && n[1][0][2])
      &&
      (!n[1][0][0] && !n[2][0][0] && 
       !n[1][1][0] && !n[2][1][0] &&
       !n[0][2][0] && !n[1][2][0] && !n[2][2][0] &&
       !n[0][2][1] && !n[1][2][1] && !n[2][2][1] &&
       !n[0][2][2] && !n[1][2][2] && !n[2][2][2])
    )
  {
    return true;
  }

  // T13
  if((n[1][1][1] && n[1][0][2] && n[2][1][2])
      &&
      (!n[0][0][0] && !n[1][0][0] && !n[2][0][0] &&
       !n[0][1][0] && !n[1][1][0] && !n[2][1][0] &&
       !n[0][2][0] && !n[1][2][0] && !n[2][2][0] &&
       !n[0][2][1] && !n[1][2][1] && 
       !n[0][2][2] && !n[1][2][2])
    )
  {
    return true;
  }

  // T14
  if((n[1][1][1] && n[1][0][2] && n[0][1][2])
      &&
      (!n[0][0][0] && !n[1][0][0] && !n[2][0][0] &&
       !n[0][1][0] && !n[1][1][0] && !n[2][1][0] &&
       !n[0][2][0] && !n[1][2][0] && !n[2][2][0] &&
       !n[1][2][1] && !n[2][2][1] &&
       !n[1][2][2] && !n[2][2][2])
    )
  {
    return true;
  }

  return false;
}
//transfrom a neiborhood from a diffrent direction
template <class T>
bool Thin<T>::transformNeighorhood(bool n[3][3][3], char direction, bool USn[3][3][3])
{
  char i, j, k;
  bool tmp[3][3][3];

  switch(direction) {
    case 0:  //UP_SOUTH = 0, 
      // just copy
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[i][j][k] = n[i][j][k];
          }
        }
      }
      break;

      // The following cases are clearly rotations only
    case 3: //   EAST_SOUTH,

      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[2-j][i][k] = n[i][j][k];
          }
        }
      }
      break;

    case 6: //  SOUTH_WEST,

      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[j][2-i][k] = n[i][j][k];
          }
        }
      }
      break;

    case 10: //   UP_EAST, 

      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[k][j][2-i] = n[i][j][k];
          }
        }
      }
      break;
    case 4: //  UP_WEST, 

      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[2-k][j][i] = n[i][j][k];
          }
        }
      }
      break;  


      // the next 3 are opposite sides - we can do either 2 or 3 rotations 
      // or a single reflection
#ifdef ROTATIONS_ONLY
    case 11: //    SOUTH_DOWN  
      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[2-j][i][k] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[2-j][i][k] = tmp[i][j][k];
          }
        }
      }
      break;

    case 7: //  UP_NORTH, 
      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[k][j][2-i] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[k][j][2-i] = tmp[i][j][k];
          }
        }
      }
      break;


    case 5: //   NORTH_DOWN, 

      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[k][j][2-i] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            n[i][2-k][j] = tmp[i][j][k];
          }
        }
      }
      // 3
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[2-j][i][k] = n[i][j][k];
          }
        }
      }
      break;

#else // not ROTATIONS_ONLY
      // use reflection
    case 11: //    SOUTH_DOWN  
      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[i][2-j][k] = n[i][j][k];
          }
        }
      }
      break;

    case 7: //  UP_NORTH, 

      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[i][j][2-k] = n[i][j][k];
          }
        }
      }
      break;

    case 5: //   NORTH_DOWN, 
      /*
      // 1 reflection
      for(k=0; k < 3; k++) {
      for(j=0; j < 3; j++) {
      for(i=0; i < 3; i++) {
      USn[i][k][j] = n[i][j][k];
      }
      }
      }
      break;
      */

      // OR - two reflections
      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[i][j][2-k] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[i][2-j][k] = tmp[i][j][k];
          }
        }
      }
      break;

#endif // ROTATIONS_ONLY




      // The following 4 cases need
      // either 2 rotations or one reflection and one rotation
#ifdef ROTATIONS_ONLY
    case 8: //   EAST_DOWN, 
      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[i][2-k][j] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[2-j][i][k] = tmp[i][j][k];
          }
        }
      }
      break;

    case 2: // WEST_DOWN, 
      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[i][2-k][j] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[j][2-i][k] = tmp[i][j][k];
          }
        }
      }
      break;

    case 1:  //NORTH_EAST,

      // 1 - rotation
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[i][k][2-j] = n[i][j][k];
          }
        }
      }
      // 2 - rotation
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[k][j][2-i] = tmp[i][j][k];
          }
        }
      }
      break;

    case 9: //   NORTH_WEST,

      // 1 - rotation
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[i][k][2-j] = n[i][j][k];
          }
        }
      }
      // 2 - rotation
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[2-k][j][i] = tmp[i][j][k];
          }
        }
      }
      break;

#else // not ROTATIONS_ONLY
    case 8: //   EAST_DOWN, 

      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[k][j][2-i] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[i][2-j][k] = tmp[i][j][k];
          }
        }
      }
      break;

    case 2: // WEST_DOWN, 
      // 1
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[2-k][j][i] = n[i][j][k];
          }
        }
      }
      // 2
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[i][2-j][k] = tmp[i][j][k];
          }
        }
      }
      break;

    case 1:  //NORTH_EAST,

      // 1 - reflection
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[i][j][2-k] = n[i][j][k];
          }
        }
      }
      // 2 - rotation
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[2-j][i][k] = tmp[i][j][k];
          }
        }
      }
      break;

    case 9: //   NORTH_WEST,

      // 1 - reflection
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            tmp[i][j][2-k] = n[i][j][k];
          }
        }
      }
      // 2 - rotation
      for(k=0; k < 3; k++) {
        for(j=0; j < 3; j++) {
          for(i=0; i < 3; i++) {
            USn[j][2-i][k] = tmp[i][j][k];
          }
        }
      }
      break;
#endif // ROTATIONS_ONLY

  }

  return true; 
}
template <class T>
void Thin<T>::initNeiborhood()
{
  this->nb = new V3DLONG[18];
  this->dataNeighbors = new V3DLONG[18];
  V3DLONG slsz = this->size_x * this->size_y;

  this->nb[0] = this->size_x - slsz;  // UP_SOUTH,   0
  this->nb[1] = +slsz + 1;// NORTH_EAST     1
  this->nb[2] = -1 - this->size_x;     // WEST_DOWN,  2 
  this->nb[3] = +1 -slsz; // EAST_SOUTH    3
  this->nb[4] = +this->size_x - 1;     // UP_WEST,    4
  this->nb[5] =  +slsz - this->size_x;  // NORTH_DOWN, 5 
  this->nb[6] = -slsz - 1;// SOUTH_WEST    6
  this->nb[7] = this->size_x + slsz;  // UP_NORTH,   7
  this->nb[8] = +1 - this->size_x;     // EAST_DOWN,  8
  this->nb[9] = +slsz - 1;// NORTH_WEST     9
  this->nb[10] = this->size_x + 1;     // UP_EAST,   10
  this->nb[11] = -slsz - this->size_x;  // SOUTH_DOWN 11
  this->nb[12] = this->size_x; // UP                 12
  this->nb[13] =  -this->size_x; // DOWN               13
  this->nb[14] =  +1;  // EAST,             14
  this->nb[15] = -1;  // WEST,             15
  this->nb[17] = +slsz;  // NORTH,         16
  this->nb[18] = -slsz;  // SOUTH,          17

    // initialize global neighbors array

    // lower plane

  this->dataNeighbors[0] = (-slsz -this->size_x -1);
  this->dataNeighbors[1] = (-slsz -this->size_x +0);
  this->dataNeighbors[2] = (-slsz -this->size_x +1);
  this->dataNeighbors[3] = (-slsz +0 -1);
  this->dataNeighbors[4] = (-slsz +0 +0);
  this->dataNeighbors[5] = (-slsz +0 +1);
  this->dataNeighbors[6] = (-slsz +this->size_x -1);
  this->dataNeighbors[7] = (-slsz +this->size_x +0);
  this->dataNeighbors[8] = (-slsz +this->size_x +1);

  // same plane

  this->dataNeighbors[9]  = (+0 -this->size_x -1);
  this->dataNeighbors[10] = (+0 -this->size_x +0);
  this->dataNeighbors[11] = (+0 -this->size_x +1);
  this->dataNeighbors[12] = (+0 +0 -1);
  this->dataNeighbors[13] = (+0 +0 +0);
  this->dataNeighbors[14] = (+0 +0 +1);
  this->dataNeighbors[15] = (+0 +this->size_x -1);
  this->dataNeighbors[16] = (+0 +this->size_x +0);
  this->dataNeighbors[17] = (+0 +this->size_x +1);

  // upper plane

  this->dataNeighbors[18] = (+slsz -this->size_x -1);
  this->dataNeighbors[19] = (+slsz -this->size_x +0);
  this->dataNeighbors[20] = (+slsz -this->size_x +1);
  this->dataNeighbors[21] = (+slsz +0 -1);
  this->dataNeighbors[22] = (+slsz +0 +0);
  this->dataNeighbors[23] = (+slsz +0 +1);
  this->dataNeighbors[24] = (+slsz +this->size_x -1);
  this->dataNeighbors[25] = (+slsz +this->size_x +0);
  this->dataNeighbors[26] = (+slsz +this->size_x +1);
}

template <class T>
void Thin<T>::markBoundaryInDirection(char direction)
{
  V3DLONG slsz = this->size_x * this->size_y;
  V3DLONG idx;
  V3DLONG i, j, k;
  //neigborhood index in 18 directions
  for(k=1; k < (this->size_z-1); k++) {
    for(j=1; j < (this->size_y-1); j++) {
      for(i=1; i < (this->size_x-1); i++) {
        idx = k*slsz + j*this->size_x + i;
        if((this->data[idx] == OBJECT) && (this->data[idx + this->nb[direction]] == 0)) {
          this->data[idx] = D_BORDER;
        }
      }
    }
  }
}
template<class T>
void Thin<T>::copyNeiborhood(V3DLONG idx, bool n[3][3][3])
{
  V3DLONG nidx;
  int i, j, k, ii;
    
  ii = 0;
  for(k = 0; k < 3; k++)
    for(j = 0; j < 3; j++)
      for(i = 0; i < 3; i++) {
        nidx = idx + this->dataNeighbors[ii];
        if(this->data[nidx] != 0)
        {
         n[i][j][k] = true;
        }
        else {
         n[i][j][k] = false;
        }
       ii++;
      }
  return;
} 

// main func to run
template <class T>
bool Thin<T>::run()
{
  char dir;
  bool n[3][3][3];
  bool USn[3][3][3];

  int nrDel;
  int nrPasses;

  V3DLONG idx;
  V3DLONG i, j, k;
  V3DLONG sz = this->size_x * this->size_y * this->size_z;

  nrDel = 1;
  nrPasses = 1;
  while(nrDel > 0)
  {
    printf("Pass %d\n", nrPasses);
    nrDel = 0;
    for(dir = 0; dir < 12; dir++)
    {
      printf("Dir %d\n", dir);
      switch(dir) {
        case UP_SOUTH:
          // UP
          this->markBoundaryInDirection(UP);
          // SOUTH
          this->markBoundaryInDirection(SOUTH);
          break;
        case NORTH_EAST:
          // NOTH
          this->markBoundaryInDirection(NORTH);
          // EAST
          this->markBoundaryInDirection(EAST);
          break;
        case WEST_DOWN:
          // WEST
          this->markBoundaryInDirection(WEST);
          // DOWN
          this->markBoundaryInDirection(DOWN);
          break;
        case EAST_SOUTH:
          // EAST
          this->markBoundaryInDirection(EAST);
          // SOUTH
          this->markBoundaryInDirection(SOUTH);
          break;
        case UP_WEST:
          // UP
          this->markBoundaryInDirection(UP);
          // WEST
          this->markBoundaryInDirection(WEST);
          break;
        case NORTH_DOWN:
          // NORTH
          this->markBoundaryInDirection(NORTH);
          // DOWN
          this->markBoundaryInDirection(DOWN);
          break;
        case SOUTH_WEST:
          // SOUTH
          this->markBoundaryInDirection(SOUTH);
          // WEST
          this->markBoundaryInDirection(WEST);
          break;
        case UP_NORTH:
          // UP
          this->markBoundaryInDirection(UP);
          // NORTH
          this->markBoundaryInDirection(NORTH);
          break;
        case EAST_DOWN:
          // EAST
          this->markBoundaryInDirection(EAST);
          // DOWN
          this->markBoundaryInDirection(DOWN);  
          break;
        case NORTH_WEST:
          // NORTH
          this->markBoundaryInDirection(NORTH);
          // WEST
          this->markBoundaryInDirection(WEST);
          break;
        case UP_EAST:
          // UP
          this->markBoundaryInDirection(UP);
          // EAST
          this->markBoundaryInDirection(EAST);
          break;
        case SOUTH_DOWN:
          // SOUTH
          this->markBoundaryInDirection(SOUTH);
          // DOWN
          this->markBoundaryInDirection(DOWN);  
          break;
      }
      for(k = 1; k < (this->size_z -1); k++)
        for(j = 1; j < (this->size_y - 1); j++)
          for(i = 1; i < (this->size_x - 1); i++) {
            idx = k * this->size_x * this->size_y + j * this->size_x + i;
            if(this->data[idx] == D_BORDER)
            {
              //copy to bool array
              this->copyNeiborhood(idx, n);
              this->transformNeighorhood(n, dir, USn);
              if(this->mathesAsTemplate(USn))
              {
                //can remove
                this->data[idx] = SIMPLE;
                nrDel++;
              }
            }
          }
      //reset all object voxels to OBJECT
      //delete SIMPLE points
      for(idx = 0; idx < sz; idx ++)
      {
        if(this->data[idx] == SIMPLE) this->data[idx] = 0;
        if(this->data[idx] != 0) this->data[idx] = OBJECT;
      } 
      printf("done\n");
    }
    printf("Number of voxels delelted in pass %d : %d\n", nrPasses, nrDel);
    nrPasses++;
  }
}

#endif
