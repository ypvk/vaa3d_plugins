#include "dialog.h"
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

Dialog::Dialog(QWidget* parent) : QDialog(parent)
{
  this->setupUI();
  this->setupConnection();
}
Dialog::~Dialog()
{
}

void Dialog::setupUI()
{ 
  this->m_button = new QPushButton(tr("Run"), this);
  this->m_methodList = new QComboBox(this);   
  this->m_channelList = new QComboBox(this);
  this->m_method = new QLabel(tr("Method:"), this);
  this->m_channel = new QLabel(tr("Channel:"), this);

  this->m_methodList->addItem(tr("thin"));
  //TODO more

  this->m_channelList->addItem(tr("Red"));
  this->m_channelList->addItem(tr("Green"));
  this->m_channelList->addItem(tr("Blue"));

  QVBoxLayout *mainLayout = new QVBoxLayout();
  QHBoxLayout *layout1 = new QHBoxLayout();
  QHBoxLayout *layout2 = new QHBoxLayout();
  
  layout1->addWidget(this->m_method);
  layout1->addWidget(this->m_methodList);

  layout2->addWidget(this->m_channel);
  layout2->addWidget(this->m_channelList);

  mainLayout->addLayout(layout1);
  mainLayout->addLayout(layout2);
  mainLayout->addWidget(m_button);

  this->setLayout(mainLayout);
  this->setWindowTitle("Neuron Plugin");
}

void Dialog::setupConnection()
{
  connect(this->m_button, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));
}

void Dialog::onPushButtonClicked()
{
  QMessageBox::information(this, tr("Messsage"), QString("%1:%2").arg(this->m_methodList->currentText()).arg(this->m_channelList->currentIndex()));  
}
void Dialog::setCallback(V3DPluginCallback2* callback)
{
  this->m_callback = callback;
}

