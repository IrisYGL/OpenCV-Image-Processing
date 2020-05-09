#include <highgui/highgui.hpp>
#include <imgproc/imgproc.hpp>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <string>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#define INI_FILE _T("cpg.ini")

using namespace cv;
using namespace std;


//int main(int argc, char *argv[])


double calculate(string fileName)
{
    string newfileName, grayFile;
    stringstream meanValueStream;
    string  meanValueString;
    
    Mat imageSource = imread(fileName);
    Mat imageGrey;
    Mat meanValueImage, meanStdValueImage;
    
    cvtColor(imageSource, imageGrey, CV_RGB2GRAY);
    Mat imageSobel;
    Laplacian(imageGrey, imageSobel, CV_16U, 1, 1);
    //Sobel(imageGrey, imageSobel, CV_16U, 1, 1);
    //求灰度图像的标准差
    /*meanStdDev(imageGrey, meanValueImage, meanStdValueImage);
     double meanValue = 0.0;
     meanValue = meanStdValueImage.at<double>(0, 0);
     */
    //图像的平均灰度
    double meanValue = 0.0;
    meanValue = mean(imageSobel)[0];
    
    meanValueStream << meanValue;
    meanValueStream >> meanValueString;
    
    meanValueString = fileName + ": Articulation(Laplacian):" + meanValueString;
    
    ofstream outfile("路径/ArticulationLaplacianresult.txt", ios::app);
    
    outfile << meanValueString;
    outfile << endl;
    outfile.close();
    
    return meanValue;
}
//double to string
void opt(string fileName, string imgname, double meanValue)
{
    char ThresholdValue[260];
    double fThresholdValue;
    string newfileName;
    WIN32_FIND_DATAA winFindData;
    
    GetPrivateProfileStringA("Threshold", "ThresholdValue", "", ThresholdValue, 260, "D:/AItensor/blurdetector/cpg.ini");
    
    fThresholdValue = atof(ThresholdValue);
    
    
    string str1 = winFindData.cFileName;
    newfileName = "路径/" + imgname;
    
    if (meanValue <= fThresholdValue)
    {
        rename(fileName.c_str(), newfileName.c_str());
        
    }
    
}

void judge(const string &strPath)
{
    string fileName, filePath;
    double meanValue;
    
    string strRawPath = strPath;//传地址
    strRawPath.append("\\");//加分隔符
    
    string strFindPath = strRawPath;//在新设一个
    strFindPath.append("*.*");//在最外面的目录下查找文件
    
    WIN32_FIND_DATAA winFindData;
    
    HANDLE hTemp = FindFirstFileA(strFindPath.c_str(), &winFindData);//c.str是因为这个函数需要cha类型，返回找到的数据
    
    //if (INVALID_HANDLE_VALUE == hTemp)
    //	return 0;
    while (FindNextFileA(hTemp, &winFindData))
    {
        string strOldName = winFindData.cFileName;
        if ("." == strOldName || ".." == strOldName)
        continue;
        //如果是目录，则递归继续操作
        if (winFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            string strAgain = strPath;
            strAgain.append("\\");
            strAgain.append(winFindData.cFileName);
            judge(strAgain);
            continue;
        }
        //获得绝对路径
        strOldName = strRawPath;
        //strOldName.append(winFindData.cFileName);
        filePath = strRawPath;
        string fileName = strOldName + winFindData.cFileName;
        //string strNewName = strOldName;
        string imgname = winFindData.cFileName;
        meanValue = calculate(fileName);
        opt(fileName, imgname, meanValue);
        //return fileName, filePath, imgname;
    }
    FindClose(hTemp);
}

void rename(const string &strPath, const string &strPathroot)
{
    string fileName, filePath;
    double meanValue;
    
    string strRawPath = strPath;//传地址
    strRawPath.append("\\");//加分隔符
    
    string strFindPath = strRawPath;//在新设一个
    strFindPath.append("*.*");//在最外面的目录下查找文件
    
    WIN32_FIND_DATAA winFindData;
    
    HANDLE hTemp = FindFirstFileA(strFindPath.c_str(), &winFindData);//c.str是因为这个函数需要cha类型，返回找到的数据
    
    //if (INVALID_HANDLE_VALUE == hTemp)
    //	return 0;
    while (FindNextFileA(hTemp, &winFindData))
    {
        string imgname = winFindData.cFileName;
        //newfilePath = strRawPath;
        string fileName = strRawPath + winFindData.cFileName;
        //string strNewName = strOldName;
        string newfileName = strPathroot + winFindData.cFileName;
        rename(fileName.c_str(), newfileName.c_str());
        //return fileName, filePath, imgname;
    }
    FindClose(hTemp);
}

int main(int argc, char *argv[])
{
    string thresholdName = "路径/threshold1";
    string newthresholdName = "路径/";
    rename(thresholdName, newthresholdName);
    string path = "路径";
    judge(path);
}
