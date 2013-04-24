#include "dialog.h"
#include "thin.h"
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

  this->m_methodList->addItem(tr("Thin"));
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
  //QMessageBox::information(this, tr("Messsage"), QString("%1:%2").arg(this->m_methodList->currentText()).arg(this->m_channelList->currentIndex()));  
  int channel = this->m_channelList->currentIndex();
  QString method = this->m_methodList->currentText();
  Image4DSimple* image = m_callback->getImage(m_callback->getImageWindowList()[0]);
  unsigned char* raw_data = image->getRawDataAtChannel(channel);
  if (method == "thin")
  {
    switch(image->getDatatype())
    {
      case V3D_UINT8:
        Thin<unsigned char>* thin = new Thin<unsigned char>(
            raw_data, 
            image->getXDim(),
            image->getYDim(),
            image->getZDim(),
            );
        Image4DSimple * p4DImage = new Image4DSimple();
        p4DImage->setData(
            (unsigned char*)thin->getData,
            thin->getXDim(),
            thin->getYDim(),
            thin->getZDim(),
            1,
            image->getDatatype()
            );
        v3dhandle newwin;
        newwin = this->m_callback->newImageWindow();
        this->m_callback->setImage(newwin, p4DImage);
        this->m_callback->setImageName(newwin, tr("Thin output"));
        this->m_callback->updateImageWindow(newwin);
        delete thin;
        break;
      case V3D_UINT16:
        Thin<unsigned short>* thin = new Thin<unsigned short>(
            raw_data, 
            image->getXDim(),
            image->getYDim(),
            image->getZDim(),
            );
        Image4DSimple * p4DImage = new Image4DSimple();
        p4DImage->setData(
            (unsigned char*)thin->getData,
            thin->getXDim(),
            thin->getYDim(),
            thin->getZDim(),
            1,
            image->getDatatype()
            );
        v3dhandle newwin;
        newwin = this->m_callback->newImageWindow();
        this->m_callback->setImage(newwin, p4DImage);
        this->m_callback->setImageName(newwin, tr("Thin output"));
        this->m_callback->updateImageWindow(newwin);
        delete thin;
        break;

      case V3D_FLOAT32:
        Thin<float>* thin = new Thin<float>(
            raw_data, 
            image->getXDim(),
            image->getYDim(),
            image->getZDim(),
            );
        Image4DSimple * p4DImage = new Image4DSimple();
        p4DImage->setData(
            (unsigned char*)thin->getData,
            thin->getXDim(),
            thin->getYDim(),
            thin->getZDim(),
            1,
            image->getDatatype()
            );
        v3dhandle newwin;
        newwin = this->m_callback->newImageWindow();
        this->m_callback->setImage(newwin, p4DImage);
        this->m_callback->setImageName(newwin, tr("Thin output"));
        this->m_callback->updateImageWindow(newwin);
        delete thin;
        break;

      default:
        QMessageBox::information(this, tr("Error"), tr("Error deal with the image type"));
    }
  }

}
void Dialog::setCallback(V3DPluginCallback2* callback)
{
  this->m_callback = callback;
}

