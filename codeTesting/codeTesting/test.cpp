#include "globalHeader.h"
#include "faceProcessor.h"
#include "trainImage.h"
#include "opencv2/core/core_c.h"
#include "opencv2/ml/ml.hpp"
#include <iostream>
//#include <sstream>
//#include <fstream>
//#include <vector>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

//����ѵ������


int genderTest(Mat testImg);
Mat IIPconvertMat(IplImage *inputImg, bool flag);
IplImage  MatconvertIIP(Mat inputImg);
Mat gaFi(Mat img);


/*void read_csv(String& csvPath,vector<String>& trainPath,vector<int>& label,char separator = ';')
{
    string line,path,classLabel;
    ifstream file(csvPath.c_str(),ifstream::in);
    while (getline(file,line))
    {
        stringstream lines(line);
        getline(lines,path,separator);
        getline(lines,classLabel);
        if (!path.empty()&&!classLabel.empty())
        {
            trainPath.push_back(path);
            label.push_back(atoi(classLabel.c_str()));
        }
    }
}*/

int main()
{    
	/**********��ʼ�������������������**********/
	initializeFaceProcessor();
	/**********����ͼƬ**********/
	IplImage *inputImg;
	trainImage sample; 
	Mat img = imread("6.jpg");
	inputImg =  &MatconvertIIP(img);
	/**********�������**********/
    processFace(inputImg, sample.FaceImage, sample.MouthImage, sample.NoseImage, sample.EyeImage, 0);
	//sample.FaceImage = filterGabor(sample.FaceImage);
    img = IIPconvertMat(sample.FaceImage,true);
	//resize(img, img, Size(64,64));
	//imshow("",img);
	//waitKey();
	
	/**********�Ա���**********/

	int result = genderTest(img);
	if (result == 0)
	{
		printf("female!!!!\n");
	}else if(result == 1)
	{
		printf("male!!!\n");
	}else{
		printf("error!!!");
	}


	return 0;	
}



int genderTest(Mat testImg)
{
	//��ʼ��svmѵ����
	/*string trainCsvPath = "at.txt";
    vector<String> vecTrainPath;
    vector<int> vecTrainLabel;
    read_csv(trainCsvPath,vecTrainPath,vecTrainLabel);*/

    /**********��ʼ��ѵ�����ݾ���**********/
    /*int iNumTrain = 446;
    Mat trainData;
    Mat trainLabel = Mat::zeros(iNumTrain,1,CV_32FC1);*/
	//int numPixels = testImg.cols * testImg.rows * testImg.channels();

    /**********��ȡHOG����������ѵ�����ݾ�����**********/
   /* Mat imageSrc;
    for (int i = 0; i < iNumTrain; i++)
    {
        imageSrc = imread(vecTrainPath[i].c_str(),1);
		imageSrc = gaFi(imageSrc);

        resize(imageSrc,imageSrc,Size(64,64));
        HOGDescriptor *hog = new HOGDescriptor(cvSize(64,64),cvSize(16,16),
           cvSize(8,8),cvSize(8,8),9); 
        vector<float> descriptor;
        hog->compute(imageSrc,descriptor,Size(1,1),Size(0,0));

        if (i == 0)
        {
            trainData = Mat::zeros(iNumTrain,descriptor.size(),CV_32FC1);
        }

        int n = 0;
        for (vector<float>::iterator iter = descriptor.begin();iter != descriptor.end();iter++)
        {
            trainData.at<float>(i,n) = *iter;
            n++;
        }
        trainLabel.at<float>(i,0) = vecTrainLabel[i];
    }*/

    /**********��ʼ��SVM������**********/
    CvSVM svm;  
    CvSVMParams param;    
    CvTermCriteria criteria;      
    criteria = cvTermCriteria( CV_TERMCRIT_EPS, 1000, FLT_EPSILON );      
    param = CvSVMParams(CvSVM::C_SVC, CvSVM::RBF, 
        10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria );    

    /**********ѵ��������SVM**********/
    //svm.train(trainData,trainLabel,Mat(),Mat(),param);
   // svm.save("SVM_SEX_Model.txt");
	svm.load("SVM_SEX_Model.txt");

    /**********����SVM��������**********/
    
	testImg = gaFi(testImg);
    resize(testImg,testImg,Size(64,64));
    //IplImage tempTestImg = MatconvertIIP(testImg);
	//testImg = filterGabor(&tempTestImg);

    HOGDescriptor *hog = new HOGDescriptor(cvSize(64,64),cvSize(16,16),
        cvSize(8,8),cvSize(8,8),9); 
    vector<float> descriptor;
    hog->compute(testImg,descriptor,Size(1,1),Size(0,0));

    Mat testHog = Mat::zeros(1,descriptor.size(), CV_32FC1);

    int n = 0;
    for (vector<float>::iterator iter = descriptor.begin();iter != descriptor.end();iter++)
    {
        testHog.at<float>(0,n) = *iter;
        n++;
    }
    int predictResult = svm.predict(testHog);

	return predictResult;
	
}





//ͼ���ʽ֮���ת��

Mat IIPconvertMat(IplImage *inputImg, bool flag)
{

	cv::Mat image(inputImg,flag);
	return image;
}


IplImage  MatconvertIIP(Mat inputImg)
{

	IplImage img = inputImg;
	return img;
}


Mat gaFi(Mat img)
{
	IplImage tempImg = MatconvertIIP(img);
    IplImage *matImg;
	matImg = filterGabor(&tempImg);
	img = IIPconvertMat(matImg,false);
	return img;

}