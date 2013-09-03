#include "m_dialog.h"
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
  this->setupUI();
  this->setupConnection();
}
Dialog::~Dialog()
{
}

void Dialog::setupUI()
{
  this->m_okButton = new QPushButton(tr("Run"), this);
  this->m_cancelButton = new QPushButton(tr("Cancel"), this);
  this->m_imageLabel = new QLabel(tr("Image"), this);
  this->m_imageList = new QComboBox(this);
  
  QVBoxLayout* mainLayout = new QVBoxLayout();
  QHBoxLayout* imageLayout = new QHBoxLayout();
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  
  imageLayout->addWidget(this->m_imageLabel);
  imageLayout->addWidget(this->m_imageList);
  
  buttonLayout->addWidget(this->m_okButton);
  buttonLayout->addWidget(this->m_cancelButton);
  
  mainLayout->addLayout(imageLayout);
  buttonLayout->addLayout(buttonLayout);

  this->setLayout(mainLayout); 
  this->setWindowTitle("Neuron Trace Plugin");
}

void Dialog::setupConnecttion()
{
  connect(this->m_okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
  connect(this->m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelButtonClicked());
}

void Dialog::setCallBack(const V3DPluginCallback2* callback)
{
  this->m_callback = callback; 
}

void Dialog::updateImageList()
{
  //get imagelist
  //update the combobox
  v3dhandlelist imageWindowList = this->m_callback->getImageWindowList();
  int i;
  for(i = 0; i < imageWindowList.count(); i++)
  {
    this->m_imageList->addItem(getImageName(imageWindowList[i]));
  }
}

void Dialog::onOkButtonClicked()
{
  int index = this->m_imageList->currentIndex();
  Image4DSimple* image = this->m_callback->getImage(this->m_callback->getImageWindowList()[index]);
  //run the function
}
