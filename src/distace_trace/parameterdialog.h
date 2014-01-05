#ifndef PARAMETOR_DIALOG_H
#define PARAMETOR_DIALOG_H

#incldue <v3d_interface.h>
#include <QDialog>

class QPushButton;
class QLabel;
class QComboBox;
class Parameters;
class QCheckBox;
class QSpinBox;

class ParameterDialog : public QDialog
{
  Q_OBJECT
  public:
    ParameterDialog(QWidget* parent = NULL, int landMarkSize = 0, int channelSize = 0);
    ~ParameterDialog();
    void getData(Parameters& parameters);
    int getStartNodeIndex() const;
    int getEndNodeIndex() const;
    int getChannelIndex() const;
  private slots:
    void onOkButtonClicked();
    void onCancelButtonClicked();
    void onCheckBoxStateChanged(int state);
  private:
    void setupUi();
    void setupConnection();
  private:
    QLabel* m_start_node_label;
    QLabel* m_end_node_label;
    //QLabel* m_background_node_label;
    QLabel* m_node_step_label;
    QLabel* m_outsample_step_label;
    QLabel* m_smooth_winsize_label;
    QLabel* m_edge_select_label;
    QLabel* m_channel_label;
    QLabel* m_background_select_label;

    QCheckBox* m_select_all_the_left_nodes;

    QPushButton* m_ok_button;
    QPushButton* m_cancel_button;

    QSpinBox* m_node_step_edit;
    QSpinBox* m_outsample_step_edit;
    QSpinBox* m_smooth_winsize_edit;
    QComboBox* m_edge_select;
    QComboBox* m_background_select;
    QComboBox* m_start_node;
    QComboBox* m_end_node;
    QComboBox* m_channel;
    //QComboBox* m_background_node;

    int m_landMarkSize;
    int m_channelSize;

    int m_start_node_index;
    int m_end_node_index;
    int m_channel_index;
    bool m_select_all_the_nodes;
};
#endif/*PARAMETOR_DIALOG_H*/
