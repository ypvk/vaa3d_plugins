#include "parameterdialog.h"
#include "tracing_function.h"
#include <QPushButton>
//#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
//#include <QVBoxLayout>
#include <QComboBox>
#include <QGridLayout>
//#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>

ParameterDialog::ParameterDialog(QWidget* parent, int landMarkSize, int channelSize) : QDialog(parent), m_landMarkSize(landMarkSize), m_channelSize(channelSize)
{
  this->setupUi();
  this->setupConnection();
}
ParameterDialog::~ParameterDialog()
{
}

void ParameterDialog::setupUi()
{
  //label init
  m_start_node_label = new QLabel(tr("start node lmark index"), this);
  m_end_node_label = new QLabel(tr("end node lmark index"), this);
  m_channel_label = new QLabel(tr("Channel to work"), this);
  //m_background_node_label = new QLabel(tr("background node lmark index", this);
  m_node_step_label = new QLabel(tr("node step"), this);
  m_outsample_step_label = new QLabel(tr("outsample step"), this);
  m_smooth_winsize_label = new QLabel(tr("smooth win_size"), this);
  m_edge_select_label = new QLabel(tr("edge select"), this);
  m_background_select_label = new QLabel(tr("select background"), this);
  //init checkbox
  m_select_all_the_left_nodes = new QCheckBox(tr("select all the left nodes"), this);
//button init
  m_ok_button = new QPushButton(tr("OK"), this);
  m_cancel_button = new QPushButton(tr("Cancel"), this);

  //LineEdit and ComboBox SpinBox init
//  m_node_step_edit = new QLineEdit(this);
//  m_outsample_step_edit  = new QLineEdit(this);
//  m_smooth_winsize_edit = new QLineEdit(this);
  m_node_step_edit = new QSpinBox(this);
  m_outsample_step_edit = new QSpinBox(this);
  m_smooth_winsize_edit = new QSpinBox(this);
  m_edge_select = new QComboBox(this);
  m_background_select = new QComboBox(this);
  m_start_node = new QComboBox(this);
  m_end_node = new QComboBox(this);
  m_channel = new QComboBox(this);
  //m_background_node = new QComboBox(this);
  //m_background_node->setEnabled(false)//basically the backgroun node is not select
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
  mainLayout->addWidget(m_select_all_the_left_nodes, 2, 1);
  mainLayout->addWidget(m_channel_label, 3, 0);
  mainLayout->addWidget(m_channel, 3, 1);
  mainLayout->addWidget(m_node_step_label, 4, 0);
  mainLayout->addWidget(m_node_step_edit, 4, 1);
  mainLayout->addWidget(m_outsample_step_label, 5, 0);
  mainLayout->addWidget(m_outsample_step_edit, 5, 1);
  mainLayout->addWidget(m_smooth_winsize_label, 6, 0);
  mainLayout->addWidget(m_smooth_winsize_edit, 6, 1);
  mainLayout->addWidget(m_edge_select_label, 7, 0);
  mainLayout->addWidget(m_edge_select, 7, 1);
  mainLayout->addWidget(m_background_select_label, 8, 0);
  mainLayout->addWidget(m_background_select, 8, 1);
  mainLayout->addWidget(m_ok_button, 9, 0);
  mainLayout->addWidget(m_cancel_button, 9, 1);

  this->setLayout(mainLayout);

  //init values
  Parameters para;
//  m_node_step_edit->setText(QString(para.node_step));
//  m_outsample_step_edit->setText(QString(para.outsample_step));
//  m_smooth_winsize_edit->setText(QString(para.smooth_winsize));
  m_node_step_edit->setValue(para.node_step);
  m_outsample_step_edit->setValue(para.outsample_step);
  m_smooth_winsize_edit->setValue(para.smooth_winsize);

  m_edge_select->addItem(tr("6 edge"));
  m_edge_select->addItem(tr("26 edge"));
  m_edge_select->setCurrentIndex(para.edge_select);
  m_background_select->addItem(tr("false"));
  m_background_select->addItem(tr("true"));
  m_background_select->setCurrentIndex(para.background_select);
  //add item for the landmark init for the comboBox
  int i;
  for(i = 0; i < m_landMarkSize; ++i)
  {
    m_start_node->addItem(QString::number(i+1));
    m_end_node->addItem(QString::number(i+1));
    //m_background_node->addItem(QString::number(i+1));
  }
  for(i = 0; i < m_channelSize; ++i)
  {
    m_channel->addItem(QString::number(i+1));
  }
}

void ParameterDialog::setupConnection()
{
  connect(this->m_ok_button, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
  connect(this->m_cancel_button, SIGNAL(clicked()), this, SLOT(onCancelButtonClicked()));
  connect(this->m_select_all_the_left_nodes, SIGNAL(stateChanged(int)), this, SLOT(onCheckBoxStateChanged(int)));
}

void ParameterDialog::onOkButtonClicked()
{
  this->accept();
}

void ParameterDialog::onCancelButtonClicked()
{
  this->reject();
}

void ParameterDialog::getData(Parameters& parameter)
{
  parameter.node_step = m_node_step_edit->value();
  parameter.outsample_step = m_outsample_step_edit->value();
  parameter.smooth_winsize = m_smooth_winsize_edit->value();
  parameter.edge_select = m_edge_select->currentIndex();
  parameter.background_select = m_background_select->currentIndex();
  //init the node index
  m_start_node_index = m_start_node->currentIndex();
  m_end_node_index = m_end_node->currentIndex();
  m_channel_index = m_channel->currentIndex();
  m_select_all_the_nodes = m_select_all_the_left_nodes->checkState() == Qt::Unchecked ? false : true;
}
int ParameterDialog::getStartNodeIndex() const
{
  return this->m_start_node_index;
}

int ParameterDialog::getEndNodeIndex() const
{
  return this->m_end_node_index;
}

int ParameterDialog::getChannelIndex() const
{
  return this->m_channel_index;
}

void ParameterDialog::onCheckBoxStateChanged(int state)
{
    if(state == Qt::Unchecked)
    {
        m_select_all_the_nodes = false;
        m_end_node->setEnabled(true);
    }
    else
    {
        m_select_all_the_nodes = true;
        m_end_node->setEnabled(false);
    }
}
