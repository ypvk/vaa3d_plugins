#ifndef PARAMETOR_DIALOG_H
#define PARAMETOR_DIALOG_H

#include <QDialog>

class QPushButton;
class QLabel;
class QComboBox;
class Parameters;

class ParameterDialog : public QDialog
{
  Q_OBJECT
  public:
    ParameterDialog(QWidget* parent = NULL);
    ~ParameterDialog();
    void get_data(Parameters& parameters);
  private slots:
    void onOkButtonClicked();
    void onCancelButtonClicked();  
  private:
    void setupUi();
    void setupConnection();
  private:
    QLabel* m_start_node_label;
    QLabel* m_end_node_label;
    QLabel* m_node_step_label;
    QLabel* m_outsample_step_label;
    QLabel* m_smooth_winsize_label;
    QLabel* m_edge_select_label;
    QLabel* m_background_select_label;
    
    QPushButton* m_ok_button;
    QPushButton* m_cancel_button;
    
    QLineEdit* m_node_step_edit;
    QLineEdit* m_outsample_step_edit;
    QLineEdit* m_smooth_winsize_edit;
    QComboBox* m_edge_select;
    QComboBox* m_background_select;
    QComboBox* m_start_node;
    QComboBox* m_end_node;
};
#endif/*PARAMETOR_DIALOG_H*/
