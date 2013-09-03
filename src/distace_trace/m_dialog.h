#ifndef M_DIALOG_H
#define M_DIALOG_H

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
    void onOkButtonClicked();
    void onCancelButtonClicked();
  public:
    void setCallBack(const V3DPluginCallback2* callback);
    void updateImageList();
  private:
    void setupUI();
    void setupConnection();
  private:
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
    QLabel* m_imageLabel;
    QComboBox* m_imageList;
    V3DPluginCallback2* m_callback;
};
#endif
