#include "parameterdialog.h"
#include "tracing_function.h"
#include <QPushButton>
//#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
//#include <QVBoxLayout>
#include <QComboBox>
#include <QGridLayout>

ParameterDialog::ParameterDialog(QWidget* parent) : QDialog(parent)
{
  this->setupUi();
  this->setupConnection();
}
ParameterDialog::~ParameterDialog()
{
}

void ParameterDialog::setupUi()
{
  
}

void ParameterDialog::setupConnection()
{
  //label init
  m_start_node_label = new QLabel(tr("start node lmark index"), this); 
  m_end_node_label = new QLabel(tr("end node lmark index"), this);
  m_node_step_label = new QLabel(tr("node step"), this);
  m_outsample_step_label = new QLabel(tr("outsample step"), this);
  m_smooth_winsize_label = new QLabel(tr("smooth win_size"), this);
  m_edge_select_label = new QLabel(tr("edge select"), this);
  m_background_select_label = new QLabel(tr("select background"), this);
//button init
  m_ok_button = new QPushButton(tr("OK"), this);
  m_cancel_button = new QPushButton(tr("Cancel"), this);

  //LineEdit and ComboBox init
  m_node_step_edit = new QLineEdit(this);
  m_outsample_step_edit  = new QLineEdit(this);
  m_smooth_winsize_edit = new QLineEdit(this);
  m_edge_select = new QComboBox(this);
  m_background_select = new QComboBox(this);
  m_start_node = new QComboBox(this);
  m_end_node = new QComboBox(this);

  //add layout
  //QVBoxLayout* mainLayout = new QVBoxLayout();

  //QHBoxLayout* layout1 = new QHBoxLayout();
  //layout1->addWidget(m_node_step_label);
  //layout1->addWidget(m_node_step_edit); 
  //mainLayout->addLayout(layout1);

  //QHBoxLayout* layout2 = new QHBoxLayout();
  //layout2->addWidget(m_outsample_step_label);
  //layout2->addWidget(m_outsample_step_edit);
  //mainLayout->addLayout(layout2);

  QGridLayout* mainLayout = new QGridLayout();
  
  mainLayout->addWidget(m_start_node_label, 0, 0);
  mainLayout->addWidget(m_start_node, 0, 1);
  mainLayout->addWidget(m_end_node_label, 1, 0);
  mainLayout->addWidget(m_end_node, 1, 1);
  mainLayout->addWidget(m_node_step_label, 2, 0);
  mainLayout->addWidget(m_node_step_edit, 2, 1);
  mainLayout->addWidget(m_outsample_step_label, 3, 0);
  mainLayout->addWidget(m_outsample_step_edit, 3, 1);
  mainLayout->addWidget(m_smooth_winsize_label, 4, 0);
  mainLayout->addWidget(m_smooth_winsize_edit, 4, 1);
  mainLayout->addWidget(m_edge_select_label, 5, 0);
  mainLayout->addWidget(m_edge_select, 5, 1);
  mainLayout->addWidget(m_background_select_label, 6, 0);
  mainLayout->addWidget(m_background_select, 6, 1);
  
  this->setLayout(mainLayout);

  //init values
  Parameters para;  
  m_node_step_edit->setText(QString(para.node_step));
  m_outsample_step_edit->setText(QString(para.outsample_step));
  m_smooth_winsize_edit->setText(QString(para.smooth_winsize));  
  m_edge_select->addItem(tr("6 edge"));
  m_edge_select->addItem(tr("26 edge"));
  m_edge_select->setCurrentIndex(para.edge_select);
  m_background_select->addItem(tr("false"));
  m_background_select->addItem(tr("true"));
  m_background_select->setCurrentIndex(para.background_select);
}
