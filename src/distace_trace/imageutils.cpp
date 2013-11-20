#include <math.h>
#include "imageutils.h"


double ImageUtils::getBlockMaxValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, vV3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep)
{
  if(!data || dimx <= 0 || dimy <= 0 || dimz <= 0 || x0 < 0 || x0 > dimx || y0 < 0 \
      || y0 > dimy || z0 < 0 || z0 > dimz)
    return 0;
  double xstep_h = xstep/2.0;
  double ystep_h = ystep/2.0;
  double zstep_h = zstep/2.0;
  V3DLONG xstart = x0 - xstep_h, xend = x0 + xstep_h,
          ystart = y0 - ystep_h, yend = y0 + ystep_h,
          zstart = z0 - zstep_h, zend = z0 + zstep_h;
  if (xstart < 0) xstart = 0;
  if (xend >= dimx) xend = dimx - 1;
  if (ystart < 0) ystart = 0;
  if (yend >= dimy) yend = dimy - 1;
  if (zstart < 0) zstart = 0;
  if (zend >= dimz) zend = dimz - 1; 

  V3DLONG i, j, k;
  double v = 0;
  double mv = 0;
  for(i = xstart; i <= xend; ++i)
    for(j = ystart; j <= yend; ++j)
      for(k = zstart; k <= zend; ++z)
      {
        v = double(data[i + j * dimx + z * dimx * dimy]);
        if (mv < v)
          mv = v;
      }
  return mv;
}

double ImageUtils::getBlockAveValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep)
{
  if(!data || dimx <= 0 || dimy <= 0 || dimz <= 0 || x0 < 0 || x0 > dimx || y0 < 0 \
      || y0 > dimy || z0 < 0 || z0 > dimz)
    return 0;
  double xstep_h = xstep/2.0;
  double ystep_h = ystep/2.0;
  double zstep_h = zstep/2.0;
  V3DLONG xstart = x0 - xstep_h, xend = x0 + xstep_h,
          ystart = y0 - ystep_h, yend = y0 + ystep_h,
          zstart = z0 - zstep_h, zend = z0 + zstep_h;
  if (xstart < 0) xstart = 0;
  if (xend >= dimx) xend = dimx - 1;
  if (ystart < 0) ystart = 0;
  if (yend >= dimy) yend = dimy - 1;
  if (zstart < 0) zstart = 0;
  if (zend >= dimz) zend = dimz - 1; 
  V3DLONG i;
  V3DLONG j;
  V3DLONG k;
  V3DLONG n;  
  double value = 0;
  n = 0;
  for(i = xstart; i <= xend; ++i)
    for(j = ystart; j <= yend; ++j)
      for(k = zstart; k <= zend; ++z)
      {
        value += double(double(data[i + j * dimx + z * dimx * dimy]);
        ++n;
      }
  return n == 0 ? 0 : v/n;
}

bool ImageUtils::setBlockAveValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep, unsigned char target_value)
{
  if(!data || dimx <= 0 || dimy <= 0 || dimz <= 0 || x0 < 0 || x0 > dimx || y0 < 0 \
      || y0 > dimy || z0 < 0 || z0 > dimz)
    return false;
  double xstep_h = xstep/2.0;
  double ystep_h = ystep/2.0;
  double zstep_h = zstep/2.0;
  V3DLONG xstart = x0 - xstep_h, xend = x0 + xstep_h,
          ystart = y0 - ystep_h, yend = y0 + ystep_h,
          zstart = z0 - zstep_h, zend = z0 + zstep_h;
  if (xstart < 0) xstart = 0;
  if (xend >= dimx) xend = dimx - 1;
  if (ystart < 0) ystart = 0;
  if (yend >= dimy) yend = dimy - 1;
  if (zstart < 0) zstart = 0;
  if (zend >= dimz) zend = dimz - 1; 
  V3DLONG i;
  V3DLONG j;
  V3DLONG k;
  for(i = xstart; i <= xend; ++i)
    for(j = ystart; j <= yend; ++j)
      for(k = zstart; k <= zend; ++z)
      {
        data[i + j * dimx + k * dimx * dimy] = target_value;
      }
  return true; 
}

double ImageUtils::getBlockStdValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz, V3DLONG x0, V3DLONG y0, V3DLONG z0, int xstep, int ystep, int zstep)
{
  if(!data || dimx <= 0 || dimy <= 0 || dimz <= 0 || x0 < 0 || x0 > dimx || y0 < 0 \
      || y0 > dimy || z0 < 0 || z0 > dimz)
    return 0;
  
  double ave_value = getBlockAveValue(data, dimx, dimy, dimz, x0, y0, z0, xstep, ystep, zstep);
  double xstep_h = xstep/2.0;
  double ystep_h = ystep/2.0;
  double zstep_h = zstep/2.0;
  V3DLONG xstart = x0 - xstep_h, xend = x0 + xstep_h,
          ystart = y0 - ystep_h, yend = y0 + ystep_h,
          zstart = z0 - zstep_h, zend = z0 + zstep_h;
  if (xstart < 0) xstart = 0;
  if (xend >= dimx) xend = dimx - 1;
  if (ystart < 0) ystart = 0;
  if (yend >= dimy) yend = dimy - 1;
  if (zstart < 0) zstart = 0;
  if (zend >= dimz) zend = dimz - 1; 
  V3DLONG i;
  V3DLONG j;
  V3DLONG k;
  V3DLONG n = 0;
  double value = 0;
  for(i = xstart; i <= xend; ++i)
    for(j = ystart; j <= yend; ++j)
      for(k = zstart; k <= zend; ++z)
      {
        double v = data[i + j * dimx + k * dimx * dimy] - ave_value;
        value += v*v;
        ++n;
      }
  return n==0 ? 0 : sqrt(value/n); 
}

double ImageUtils::getImageMaxValue(unsigned char* data, V3DLONG dimx, V3DLONG dimy, V3DLONG dimz)
{
  V3DLONG x0 = dimx/2;
  V3DLONG y0 = dimy/2;
  V3DLONG z0 = dimz/2;
  V3DLONG xstep = dimx;
  V3DLONG ystep = dimy;
  V3DLONG zstep = dimz;
  return getBlockMaxValue(data, dimx, dimy, dimz, x0, y0, z0, xstep, ystep, zstep); 
}

double ImageUtils::getImageAveValue(unsigned char* data, V3DLONG dimx, V3DLONG dimz, V3DLONG dimz)
{
  V3DLONG x0 = dimx/2;
  V3DLONG y0 = dimy/2;
  V3DLONG z0 = dimz/2;
  V3DLONG xstep = dimx;
  V3DLONG ystep = dimy;
  V3DLONG zstep = dimz;
  return getBlockAveValue(data, dimx, dimy, dimz, x0, y0, z0, xstep, ystep, zstep); 
}
double ImageUtils::getImageStdValue(unsigned char* data, V3DLONG dimx, V3DLONG dimz, V3DLONG dimz)
{
  V3DLONG x0 = dimx/2;
  V3DLONG y0 = dimy/2;
  V3DLONG z0 = dimz/2;
  V3DLONG xstep = dimx;
  V3DLONG ystep = dimy;
  V3DLONG zstep = dimz;
  return getBlockStdValue(data, dimx, dimy, dimz, x0, y0, z0, xstep, ystep, zstep); 
}

