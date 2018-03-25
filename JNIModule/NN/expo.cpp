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
          float v = (float)((int)image)/255;
          tp.push_back(v);
        }
      }
      images.push_back(tp);
    }
  }
}

float sigmoid(float x)
{
  return 1 / (1 + expf(-x));
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
  m_bp_neural_network bp_network(1, {6,6,6}, 1);
  bp_network.init();
  vector<m_matrix> trainInput;
  vector<m_matrix> trainOutput;
  for(int i=0;i<400;i++)
  {
    inputData.random();
    inputData.mul(M_PI);
    resultData.set(0, 0, 1+sinf(inputData.get(0,0)));
    trainInput.push_back(inputData);
    trainOutput.push_back(resultData);
  }
  for (int i = 0; i < 1000; i++)
  {
    cout<<"Train "<<i<<"//////////"<<endl;
    for(int j=0;j<trainInput.size();j++)
    {
      bp_network.gene(trainInput[j], sigmoid);
      //bp_network.print();
      bp_network.back_propagation(trainOutput[j],0.05f,0.0f, dsigmoid);
      m_matrix err = bp_network.error(trainOutput[j]);
      err.print();
    }
    
  }
  cout<<"TEST ///////////"<<endl;
  for (int i = 0; i < 10; i++)
  {
    inputData.random();
    inputData.mul(M_PI);
    resultData.set(0, 0, 1+sinf(inputData.get(0,0)));
    bp_network.gene(inputData, sigmoid);
    m_matrix err = bp_network.error(resultData);
    err.print();
  }
  bp_network.print();
}

void image_fitting()
{
  vector<float>labels; 
  read_Mnist_Label("data/train-labels.idx1-ubyte", labels);
  vector<vector<float>> images;
  IdxImageInfo imageInfo;
  read_Mnist_Images("data/train-images.idx3-ubyte",imageInfo, images);

  m_bp_neural_network bp_network(784,{16,16},1);
  m_matrix inputData(784, 1);
  m_matrix resultData(1, 1);
  bp_network.init();
  for(int i=0;i<10000;i++)
  {
    for(int j=0;j<30;j++)
    {
      resultData.mul(0);
      inputData.set_vector(images[j]);
      int index = labels[j];
      resultData.set(0,0,1+index);
      m_matrix result = bp_network.gene(inputData,sigmoid);
      bp_network.back_propagation(resultData,0.0001f,0.0f, dsigmoid);
      m_matrix err = bp_network.error(resultData);
      cout<<index<<endl;
      result.print();
      err.print();
    }
  }
  for(int j=3000;j<3100;j++)
  {
    resultData.mul(0);
    inputData.set_vector(images[j]);
    int index = labels[j];
    resultData.set(0,0,1+index);
    m_matrix result = bp_network.gene(inputData,sigmoid);
    //m_matrix err = bp_network.error(resultData);
    cout<<index<<endl;
    result.print();
    //err.print();
  }
}


int main()
{
  sin_fitting();
  //image_fitting();
  return 0;
}