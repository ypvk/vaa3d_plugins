#ifndef NEURON_PLUGIN_H
#define NEURON_PLUGIN_H

#include <QtGui>
#include <v3d_interface.h>

class NeuronPluin : public QObject, public V3DPluginInterface2_1
{
  Q_OBJECT
  Q_INTERFACES(V3DPluginInterface2_1)
  public:
    float getPluginVersion()const {return 1.1f;}
    QStringList menulist() const;
    void domenu(const QString & menu_name. V3DPluginCallback2 &callback, QWdiget* parent);
    QStringList funclist() const;
    bool dofunc(const QString & func_name, const V3DPluginArgList & input, V3DPluginArgList & output, V3DPluginCallback2 & v3d, QWidget * parent);
};
#endif