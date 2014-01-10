#ifndef M_DIALOG_H
#define M_DIALOG_H

#include <QDialog>
#include <QList>
#include <v3d_interface.h>

class QPushButton;
class QComboBox;
class QLabel;

class Dialog : public QDialog
{
  Q_OBJECT
  public:
    Dialog(V3DPluginCallback2* callback, QWidget* parent = 0);
    ~Dialog();
  private slots:
    void onOkButtonClicked();
    void onCancelButtonClicked();
    void onSaveButtonClicked();
  public:
    void setCallback(V3DPluginCallback2* callback);
    void updateImageList();
  private:
    void setupUI();
    void setupConnection();
    void saveNeuronSWCData();
  protected:
    void closeEvent(QCloseEvent* event);
  private:
    QPushButton* m_okButton;
    QPushButton* m_cancelButton;
    QPushButton* m_saveButton;
    QLabel* m_imageLabel;
    QComboBox* m_imageList;
    V3DPluginCallback2* m_callback;

    QList<NeuronTree> m_traced_neurons;
};
#endif
