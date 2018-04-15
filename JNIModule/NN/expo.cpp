#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <math.h>
#include <armadillo>
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

mat build_mat_by_image(vector<double> &image_data)
{
  mat re = mat(1,image_data.size());
  for(int i=0;i<image_data.size();i++)
  {
    re(0,i)=image_data[i];
  }
  return re;
}

void read_Mnist_Label(string filename, vector<double> &labels)
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
      labels.push_back((double)label);
    }
  }
}

void read_Mnist_Images(string filename, IdxImageInfo &info, vector<vector<double>> &images)
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
      vector<double> tp;
      for (int r = 0; r < n_rows; r++)
      {
        for (int c = 0; c < n_cols; c++)
        {
          unsigned char image = 0;
          file.read((char *)&image, sizeof(image));
          double v = (double)((double)((int)image) / 256.0f);
          tp.push_back(v);
        }
      }
      images.push_back(tp);
    }
  }
}

void sin_fitting()
{
  mat inputData(1, 1);
  mat resultData(1, 2);
  m_bp_neural_network bp_network(1, {5, 5, 2}, {"logsig", "logsig", "logsig"});
  bp_network.init();
  vector<mat> trainInput;
  vector<mat> trainOutput;
  vector<mat> testInput;
  vector<mat> testOutput;
  int sample_num = 20;
  int test_num = 20;
  for (int i = 0; i <= sample_num - 1; i++)
  {
    inputData(0, 0) = (((double)(i) / (sample_num - 1)) - 0.5) * 2;
    resultData(0, 0) = (1 + sinf(inputData(0, 0) * M_PI)) / 2;
    resultData(0, 1) = resultData(0, 0);
    trainInput.push_back(inputData);
    trainOutput.push_back(resultData);
  }
  for (int i = 0; i <= test_num - 1; i++)
  {
    inputData(0, 0) = (((double)(i) / (test_num - 1)) - 0.5) * 2;
    resultData(0, 0) = (1 + sinf(inputData(0, 0) * M_PI)) / 2;
    resultData(0, 1) = resultData(0, 0);

    testInput.push_back(inputData);
    testOutput.push_back(resultData);
  }
  bp_network.train("traingd", trainInput, trainOutput, 80000, 0.5);
  for (int i = 0; i < testInput.size(); i++)
  {
    mat re = bp_network.sim(testInput[i]);
    re.print("Sim");
    testOutput[i].print();
  }
  //bp_network.print();
}

void image_fitting()
{
  vector<double>labels; 
  read_Mnist_Label("data/train-labels.idx1-ubyte", labels);
  vector<vector<double>> images;
  IdxImageInfo imageInfo;
  read_Mnist_Images("data/train-images.idx3-ubyte",imageInfo, images);


  m_bp_neural_network bp_network(784,{32,32,10}, {"logsig", "logsig", "logsig"});
  mat inputData(1, 784);
  mat resultData(1, 10);
  bp_network.init();
  vector<mat> trainInput;
  vector<mat> trainOutput;
  vector<mat> testInput;
  vector<mat> testOutput;
  vector<int> testLabel;
  int sample_num = 5000;
  int test_num = 1000;
  for (int i = 0; i <= sample_num - 1; i++)
  {
    int index = labels[i];
    inputData = build_mat_by_image(images[i]);
    resultData.fill(0);
    resultData(0,index) = 1;
    trainInput.push_back(inputData);
    trainOutput.push_back(resultData);
  } 
  for (int i = 20000; i <= 20000 + test_num - 1; i++)
  {
    int index = labels[i];
    inputData = build_mat_by_image(images[i]);
    resultData.fill(0);
    resultData(0,index) = 1;
    testInput.push_back(inputData);
    testOutput.push_back(resultData);
    testLabel.push_back(index);
  }
  bp_network.train("traingd", trainInput, trainOutput, 10000, 0.5);
  int correct = 0;
  for (int i = 0; i < testInput.size(); i++)
  {
    mat re = bp_network.sim(testInput[i]);
    double max1 = re(0,testLabel[i]);
    double max2 = max(max(re));
    //cout<<max1<<"/"<<max2<<endl;
    //re.print("Sim");
    //testOutput[i].print();
    if(max1==max2)correct++;
  }
  cout<<"Correct:"<<(double)correct/(double)testInput.size()*100<<"%"<<endl;
}

int main()
{
  //sin_fitting();
  image_fitting();
  return 0;
}