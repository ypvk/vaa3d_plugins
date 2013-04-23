#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <v3d_interface.h>

class QPushButton;
class QComboBox;
class QLabel;

class Dialog : public QDialog
{
  Q_OBJECT
  public:
    Dialog(QWidget* parent = 0);
    ~Dialog();
  private slots:
    void onPushButtonClicked();
  private:
    void setupUI();
    void setupConnection();
  public:
    void setCallback(V3DPluginCallback2* callback);
  private:
    V3DPluginCallback2 *m_callback;
    QPushButton *m_button;
    QComboBox *m_methodList; 
    QComboBox *m_channelList;
    QLabel *m_method;
    QLabel *m_channel;
};
#endif
