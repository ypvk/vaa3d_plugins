#include "testplugin.h"
#include <iostream>

Q_EXPORT_PLUGIN2(testPlugin, TestPlugin)

QStringList TestPlugin::menulist() const
{
  return QStringList() << tr("test1") << tr("test2");
}

QStringList TestPlugin::funclist() const
{
  return QStringList();
}

void TestPlugin::domenu(const QString & menu_name, V3DPluginCallback2 & callback, QWidget* parent)
{
  if (menu_name == tr("test1"))
  {
    //v3d_msg(tr("in test1"));
    std::cout << "1111" << endl;
  }
  else if (menu_name == tr("test2"))
  {
    //v3d_msg(tr("in test2"));
    std::cout << "222" << endl;
  }
}
bool TestPlugin::dofunc(const QString & func_name, const V3DPluginArgList & input, V3DPluginArgList & output, V3DPluginCallback2 & v3d, QWidget* parent)
{
  return false;
}
