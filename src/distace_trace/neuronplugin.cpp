#include "neuronplugin.h"
#include "m_dialog.h"

Q_EXPORT_PLUGIN2(neuronPlugin, NeuronPlugin)

QStringList NeuronPlugin::menulist() const
{
  return QStringList() << tr("Skeleton trace")
                       << tr("about this plugin");
}
QStringList NeuronPlugin::funclist() const
{
  return QStringList();
}

void NeuronPlugin::domenu(const QString & menu_name, V3DPluginCallback2 & callback, QWidget* parent)
{
  if (menu_name == tr("Skeleton trace"))
  {
    //v3d_msg(tr("on going!!"));
    Dialog* m_dialog = new Dialog(); 
    m_dialog->setCallback(&callback);
    m_dialog->exec();
    delete m_dialog;
  }
  else if (menu_name == tr("about this plugin"))
  {
    v3d_msg(tr("Plugin to get the skeleton of neuron by ping yu 2013-8-20"));
  }
}

bool NeuronPlugin::dofunc(const QString & func_name, const V3DPluginArgList & input, V3DPluginArgList & output, V3DPluginCallback2 & v3d, QWidget* parent)
{
  return false;
}
