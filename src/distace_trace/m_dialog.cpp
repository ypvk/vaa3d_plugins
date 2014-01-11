#include "m_dialog.h"
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "tracing_function.h"
#include "parameterdialog.h"
#include <iostream>
#include <QMessageBox>
#include <QDebug>

using namespace std;

Dialog::Dialog(V3DPluginCallback2* callback, QWidget *parent) : QDialog(parent), m_callback(callback)
{
  this->setupUI();
  this->setupConnection();
  this->updateImageList();
}
Dialog::~Dialog()
{
}

void Dialog::setupUI()
{
  this->m_okButton = new QPushButton(tr("Run"), this);
  this->m_cancelButton = new QPushButton(tr("Cancel"), this);
  this->m_saveButton = new QPushButton(tr("Save"), this);
  this->m_imageLabel = new QLabel(tr("Image"), this);
  this->m_imageList = new QComboBox(this);
  this->m_logEdit = new QTextEdit(this);
  
  QVBoxLayout* mainLayout = new QVBoxLayout();
  QHBoxLayout* imageLayout = new QHBoxLayout();
  QHBoxLayout* buttonLayout = new QHBoxLayout();
  
  imageLayout->addWidget(this->m_imageLabel);
  imageLayout->addWidget(this->m_imageList);
  
  buttonLayout->addWidget(this->m_okButton);
  buttonLayout->addWidget(this->m_cancelButton);
  buttonLayout->addStretch(0.5);
  buttonLayout->addWidget(this->m_saveButton);

  mainLayout->addLayout(imageLayout);
  mainLayout->addLayout(buttonLayout);
  mainLayout->addWidget(m_logEdit);

  this->setLayout(mainLayout); 
  this->setWindowTitle("Neuron Trace Plugin");
  this->resize(600, 400);
}

void Dialog::setupConnection()
{
  connect(this->m_okButton, SIGNAL(clicked()), this, SLOT(onOkButtonClicked()));
  connect(this->m_cancelButton, SIGNAL(clicked()), this, SLOT(onCancelButtonClicked()));
  connect(this->m_saveButton, SIGNAL(clicked()), this, SLOT(onSaveButtonClicked()));
}

void Dialog::setCallback(V3DPluginCallback2* callback)
{
  this->m_callback = callback; 
}

void Dialog::updateImageList()
{
  //get imagelist
  //update the combobox
  if(!m_callback) return;
  v3dhandleList imageWindowList = this->m_callback->getImageWindowList();
  qDebug() << imageWindowList.size();
  qDebug() << m_callback->getImageName(imageWindowList[0]);
  int i;
  for(i = 0; i < imageWindowList.count(); i++)
  {
    this->m_imageList->addItem(m_callback->getImageName(imageWindowList[i]));
  }
}

void Dialog::onOkButtonClicked()
{
  selfLog("===============================================");
  int index = this->m_imageList->currentIndex();
  v3dhandle image_window = this->m_callback->getImageWindowList()[index];
  Image4DSimple* image = this->m_callback->getImage(image_window);
  //run the function
  LandmarkList landmarkList = this->m_callback->getLandmark(image_window);
  Parameters para; 
  selfLog("setup parameterdialog");
  ParameterDialog parameterDialog(this, landmarkList.size(), image->getCDim());
  if(parameterDialog.exec() != QDialog::Accepted)
  {
    cout << "reject the dialog return" << endl;
    return;    
  }  
  //now simplly add two node, then add multinode function
  selfLog("Get the parameter");
  parameterDialog.getData(para);
  const LocationSimple& startLandMark = landmarkList.at(parameterDialog.getStartNodeIndex());
  int start_node_index = parameterDialog.getStartNodeIndex();
  int n_end_nodes;
  vector<int> end_node_indexs;
  end_node_indexs.clear();
  if (parameterDialog.isAllLeftNodesSelected())
  {
    n_end_nodes = landmarkList.size() - 1;
    int i;
    for(i = 0; i < landmarkList.size(); ++i)
    {
      if(i != start_node_index)
      {
        end_node_indexs.push_back(i);
      }
    } 
  }
  else
  {
    n_end_nodes = 1;
    end_node_indexs.push_back(parameterDialog.getEndNodeIndex());
  }
  int channelIndex =  parameterDialog.getChannelIndex();
  //prepare the basic para fro the nueron_tracing 
  unsigned char* data = image->getRawDataAtChannel(channelIndex);
  //get all the end node coordinate
  float x[n_end_nodes];
  float y[n_end_nodes];
  float z[n_end_nodes];
  int i;
  for(i = 0; i < end_node_indexs.size(); ++i)
  {
    const LocationSimple& endLandMark = landmarkList.at(end_node_indexs[i]);
    x[i] = endLandMark.x;
    y[i] = endLandMark.y;
    z[i] = endLandMark.z;
  } 
  //creating the NeuronTracing object
  selfLog("Begion create the Algorithm Object");
  NeuronTracing neuronTracing(data, 
      image->getXDim(), image->getYDim(), image->getZDim(),
      1.0, 0, 0, 0, image->getXDim() - 1, image->getYDim() - 1, image->getZDim() - 1, 
      startLandMark.x, startLandMark.y, startLandMark.z,
      n_end_nodes, x, y, z,
      para);
  const char* error = neuronTracing.find_shortest_path(); 
  if (error != NULL)
  {
    //cout << "run the find_shortest_path function error" << endl;
    selfLog("run the find_shortest_path function error");
    selfLog(error);
    return;
  }
  if (n_end_nodes >= 2)
  {
    error = neuronTracing.merge_traced_path();
    if (error != NULL)
    {
      selfLog("run the merge function error");
      selfLog(error);
      return;
    }
  }
  else
  {
    //TODO add some additional action
    //do nothing
  }
  //downsample the curve
  selfLog("downsample the curve");
  neuronTracing.downsample_curve();  
  //rearrange the index 
  selfLog("rearrange_curve_index");
  neuronTracing.rearrange_curve_index();
  //refit postion and radius and smooth the radius     
  selfLog("refit the position and smooth the radius");
  neuronTracing.refit_pos_r_and_smooth_r(true,
      m_callback->getGlobalSetting().b_3dcurve_width_from_xyonly, 
      false);
 //get all the neuronTree data 
  selfLog("get the data");
  neuronTracing.fetch_data_for_neurontree(m_traced_neurons); 
  //update the 3d Image window to see the result
  for(i = 0; i < m_traced_neurons.size(); ++i)
  {
    m_callback->setSWC(image_window, m_traced_neurons[i]);
  } 
  selfLog("update on 3D view");
  m_callback->open3DWindow(image_window);  
  m_callback->pushObjectIn3DWindow(image_window);
  selfLog("===============================================");
}
void Dialog::onCancelButtonClicked()
{
  m_traced_neurons.clear();
  this->reject();
}

void Dialog::closeEvent(QCloseEvent* event)
{
  if(m_traced_neurons.size() == 0)
  {
    //without data to save
    event->accept();
  } 
  else
  {
    QMessageBox::StandardButton ret_button = QMessageBox::question(this, 
        tr("Question"),
        tr("You have traced data, will you save them?"),
        (QMessageBox::Ok | QMessageBox::Cancel)
      );    
    if(ret_button == QMessageBox::Ok)
    {
      saveNeuronSWCData();
      event->ignore();
    }
    else
    {
      m_traced_neurons.clear();
      event->accept();
    }
  }
}

void Dialog::onSaveButtonClicked()
{  
  if(m_traced_neurons.size() == 0)
  {
    QMessageBox::information(this, tr("No data"), tr("No data to save."));
  }
  else
  {
    saveNeuronSWCData();
  }
}

void Dialog::saveNeuronSWCData()
{

  int i;
  for(i = 0; i < m_traced_neurons.size(); ++i)
  {
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
        QString("%1_%2").arg(m_traced_neurons[i].file).arg(i),
        tr("SWC (*.swc)"));
    writeSWC_file(filename, m_traced_neurons[i]);   
    selfLog(QString("save file %1").arg(filename));
  }
  m_traced_neurons.clear();
}
void Dialog::selfLog(const QString& text)
{
  m_logEdit->append(text); 
}
