#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <math.h>
#include "src/m_matrix.cpp"
#include "src/m_bp_neural_network.cpp"

using namespace std;

struct IdxImageInfo
{
  int count;
  int width;
  int height;
};

int ReverseInt(int i)
{
  unsigned char ch1, ch2, ch3, ch4;
  ch1 = i & 255;
  ch2 = (i >> 8) & 255;
  ch3 = (i >> 16) & 255;
  ch4 = (i >> 24) & 255;
  return ((int)ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}

void read_Mnist_Label(string filename, vector<float> &labels)
{
  ifstream file(filename, ios::binary);
  if (file.is_open())
  {
    int magic_number = 0;
    int number_of_images = 0;
    file.read((char *)&magic_number, sizeof(magic_number));
    file.read((char *)&number_of_images, sizeof(number_of_images));
    magic_number = ReverseInt(magic_number);
    number_of_images = ReverseInt(number_of_images);
    cout << "magic number = " << magic_number << endl;
    cout << "number of images = " << number_of_images << endl;
    for (int i = 0; i < number_of_images; i++)
    {
      unsigned char label = 0;
      file.read((char *)&label, sizeof(label));
      labels.push_back((float)label);
    }
  }
}

void read_Mnist_Images(string filename, IdxImageInfo &info, vector<vector<float>> &images)
{
  ifstream file(filename, ios::binary);
  if (file.is_open())
  {
    int magic_number = 0;
    int number_of_images = 0;
    int n_rows = 0;
    int n_cols = 0;

    file.read((char *)&magic_number, sizeof(magic_number));
    file.read((char *)&number_of_images, sizeof(number_of_images));
    file.read((char *)&n_rows, sizeof(n_rows));
    file.read((char *)&n_cols, sizeof(n_cols));
    magic_number = ReverseInt(magic_number);
    number_of_images = ReverseInt(number_of_images);
    n_rows = ReverseInt(n_rows);
    n_cols = ReverseInt(n_cols);
    info.height = n_rows;
    info.width = n_cols;
    info.count = number_of_images;
    cout << "magic number = " << magic_number << endl;
    cout << "number of images = " << number_of_images << endl;
    cout << "rows = " << n_rows << endl;
    cout << "cols = " << n_cols << endl;
    for (int i = 0; i < number_of_images; i++)
    {
      vector<float> tp;
      for (int r = 0; r < n_rows; r++)
      {
        for (int c = 0; c < n_cols; c++)
        {
          unsigned char image = 0;
          file.read((char *)&image, sizeof(image));
          float v = (float)((float)((int)image)/256.0f);
          tp.push_back(v);
        }
      }
      images.push_back(tp);
    }
  }
}


float sigmoid_tanh(float x)
{
  float v = tanhf(x);
  return v;
}

float dsigmoid_tanh(float x)
{
  float d = sigmoid_tanh(x);
  return 1 - d*d;
}

float sigmoid(float x)
{
  float v = 1 / (1 + expf(-x*1.0f));
  return v;
}

float dsigmoid(float x)
{
  float d = sigmoid(x);
  return d * (1 - d);
}



void sin_fitting()
{
  m_matrix inputData(1, 1);
  m_matrix resultData(1, 1);
  m_bp_neural_network bp_network(1, {5}, 1,0.4f,0.0f,0.000f);
  bp_network.init();
  vector<m_matrix> trainInput;
  vector<m_matrix> trainOutput;
  for(int i=0;i<=20;i++)
  {
    //inputData.random();
    inputData.set(0,0,((float)(i)/20));
    //inputData.set(0,0,0.1f);
    resultData.set(0, 0, (1+sinf(inputData.get(0,0)*M_PI))/2);
    //cout<<inputData.get(0,0)<<"=>"<<resultData.get(0,0)<<endl;
    trainInput.push_back(inputData);
    trainOutput.push_back(resultData);
  }
  for (int i = 0; i <10; i++)
  {
    //cout<<"Train "<<i<<"//////////"<<endl;
    for(int j=0;j<trainInput.size();j++)
    {
      //cout<<"Data "<<trainInput[j].get(0,0)<<"//"<<trainOutput[j].get(0,0)<<"////////"<<endl;
      m_matrix genr = bp_network.gene(trainInput[j], sigmoid);
      //cout<<"GeneOut ///////////"<<endl;
      //genr.print();
      //bp_network.print();
      bp_network.back_propagation(trainOutput[j], dsigmoid);
      //bp_network.print();
      //return;
      /*if(j>0 && j%5==0){
        m_matrix err = bp_network.error();
        err.print();
        bp_network.update();
      }*/
    }
    //if(i%100==0)
    {
      m_matrix err = bp_network.error();
      err.print();
    }
    bp_network.update();
    //break;
  }
  //return;
  //bp_network.print();
  
  cout<<"TEST ///////////"<<endl;
  for (int i = 0; i < 10; i++)
  {
    inputData.random();
    resultData.set(0, 0, (1+sinf(inputData.get(0,0)*M_PI))/2);
    bp_network.gene(inputData, sigmoid);
    m_matrix err = bp_network.error(resultData);
    //err.filter(abs);
    err.print();
  }
  
  //bp_network.print();
}

void image_fitting()
{
  vector<float>labels; 
  read_Mnist_Label("data/train-labels.idx1-ubyte", labels);
  vector<vector<float>> images;
  IdxImageInfo imageInfo;
  read_Mnist_Images("data/train-images.idx3-ubyte",imageInfo, images);


  m_bp_neural_network bp_network(784,{32,16},10,0.08f,0.0f,0.001f);
  m_matrix inputData(784, 1);
  m_matrix resultData(10, 1);
  bp_network.init();
  for(int i=0;i<1000;i++)
  {
    for(int j=0;j<1000;j++)
    {
      resultData.mul(0);
      inputData.set_vector(images[j]);
      int index = labels[j];
      resultData.set(index,0,1);
      //resultData.mul(0.1f);
      m_matrix result = bp_network.gene(inputData,sigmoid_tanh);
      bp_network.back_propagation(resultData, dsigmoid_tanh);
      
    }
    //if(i%10==0)
    {
      m_matrix err = bp_network.error();
      err.print();
    }
    bp_network.update();
  }
  cout<<"TEST////////////////////////////////"<<endl;
  int all = 100;
  int correct =0;
  for(int j=10000;j<10000+all;j++)
  {
    resultData.mul(0);
    inputData.set_vector(images[j]);
    //inputData.print();
    int index = labels[j];
    resultData.set(index,0,1);
    m_matrix result = bp_network.gene(inputData,sigmoid);
    m_matrix_index_info info = result.max();
    if(info.x==index)correct++;
    cout<<index<<endl;
    result.print();
    cout<<"Error:"<<bp_network.error_f(resultData)<<endl;
  }
  cout<<"Correct:"<<(float)(correct)/(float)(all)*100.0f<<'%'<<endl;
}


int main()
{
  sin_fitting();
  //image_fitting();
  return 0;
}