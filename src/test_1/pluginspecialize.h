#ifndef PLUGIN_SPECIALIZE_H
#define PLUGIN_SPECIALIZE_H
template<class TpixelType>
class PluginSpecialize
{
  public:
    PluginSpecialize(unsigned char* data_org, V3DLONG size_x, V3DLONG size_y, V3DLONG size_z);
    ~PluginSpecialize();
    V3DLONG getXDim() const;
    V3DLONG getYDim() const;
    V3DLONG getZDim() const;
    TPixelType* getData() const;
    bool run();
  private:
  //private function
  private:
    TPixelType* data;
    V3DLONG size_x;
    V3DLONG size_y;
    V3DLONG size_z;
};
#include "pluginspecialize.cpp"
#endif
