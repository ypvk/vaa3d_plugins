/**************************************************
  *basic function for image
  *************************************************/
#ifndef IMAGE_UTILS
#define IMAGE_UTILS
#include <v3d_interface.h>
class ImageUtils
{
  private:
    ImageUtils(){}
  public:
    static double getImageMaxValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz);
    static double getImageAveValue(unsigned char* data, V3DLONG dimx, V3DLONG dimz, V3DLONG dimz);
    static double getImageStdValue(unsigned char* data, V3DLONG dimx, V3DLONG dimz, V3DLONG dimz);
    static double fitRadiusePercent(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz);
    static double fitPosition(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLOMG dimz); 
    //get special block value
    static double getBlockMaxValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, vV3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep);
    static double getBlockAveValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep);
    static double getBlockStdValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep);
    static bool setBlockAveValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep, unsigned char target_value);
    //get intensity
    static inline double getIntensity(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, int ix, int iy, int iz);
};
#endif /*IMAGE_UTILS*/
