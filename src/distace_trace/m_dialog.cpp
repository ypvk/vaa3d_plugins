#include "m_dialog.h"
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "tracing_function.h"
#include "parameterdialog.h"
#include <iostream>
using namespace std;

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
  v3dhandle image_window = this->m_callback->getImageWindowList()[index];
  Image4DSimple* image = this->m_callback->getImage(image_window);
  //run the function
  LandmarkList landmarkList = this->m_callback->getLandMark(image_window);
  Parameters para; 
  ParameterDialog parameterDialog(this, landmarkList.size(), image->getCDim());
  if(parameterDialog.exec() != QDialog::Accepted)
  {
    cout << "reject the dialog return" << endl;
    return;    
  }  
  //now simplly add two node, then add multinode function
  parameterDialog.getData(para);
  const LocationSimple& startLandMark = landmarkList.get(parameterDialog.getStartNodeIndex());
  const LocationSimple& endLandMark = landmarkList.get(parameterDialog.getEndNodeIndex());
  int channelIndex =  parameterDialog.getChannelIndex();
  //prepare the basic para fro the nueron_tracing 
  unsigned char* data = image->getRawDataAtChannel(channelIndex);
  float x[1] = {endLandMark.x};
  float y[1] = {endLandMark.y};
  float z[1] = {endLandMark.z};
  NeuronTracing neuronTracing(data, 
      image->getXDim(), image->getYDim(), image->getZDim(),
      1.0, 0, 0, 0, image->getXDim() - 1, image->getYDim() - 1, image->getZDim() - 1, 
      startLandMark.x, startLandMark.y, startLandMark.z,
      x, y, z, 1,
      para);
  const char* error = neuronTracing.find_shortest_path(); 
  if (error != NULL)
  {
    cout << "run the find_shortest_path function error" << endl;
    return;
  }
  error = neuronTracing.merge_traced_path();
  if (error != NULL)
  {
    cout << "run the merge function error" << endl;
    return;
  }
  //downsample the curve
     
}
void Dialog::onCancelButtonClicked()
{
  this->reject();
}

void Dialog::closeEvent(QCloseEvent* event)
{
  if(...)
  {
    //TODO save the result
        
    event->accept();
  } 
  else
  {
    event->ignore();
  }
}
