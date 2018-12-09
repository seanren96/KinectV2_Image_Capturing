#include <Kinect.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <windows.h>
#include <thread>
#include <pthread.h>
#include <ctime>
#include <cstdio>
#include <time.h>

using namespace std;
using namespace cv;
char file_name[20];

IKinectSensor *m_pKinectSensor = nullptr;
WaveFile audioFile;
BOOLEAN audio_initialized = false;
vector<BYTE> audioBuffer;
//void setAudio() { updateAudioFrame(); }
//void openAudio(string fname) { audioFile.Open(fname); }
//void closeAudio() { audioFile.Close(); }
bool isOpenedAudio() { return audioFile.audioFile.is_open(); }
string now()
{
    char s[1024];
    time_t t = time(NULL);
    struct tm lnow;
    localtime_s(&lnow, &t);
    sprintf_s(s, "%04d-%02d-%02d_%02d-%02d-%02d", lnow.tm_year + 1900, lnow.tm_mon + 1, lnow.tm_mday,
              lnow.tm_hour, lnow.tm_min, lnow.tm_sec);
    return string(s);
}

void initializeAudio()
{
    IAudioSource *m_pAudioSource = nullptr;
    auto ret1 = m_pKinectSensor->get_AudioSource(&m_pAudioSource);
   /* if (ret1 != S_OK)
    {
        cout << "error : m_pKinectSensor->get_AudioSource(&m_pAudioSource);" << endl;
        return;
    }*/
    IAudioBeamFrameReader *m_pAudioBeamFrameReader = nullptr;
    auto ret2 = m_pAudioSource->OpenReader(&m_pAudioBeamFrameReader);
    /*if (ret2 != S_OK)
    {
        cout << "error : m_pAudioSource->OpenReader(&m_pAudioBeamFrameReader);" << endl;
        return;
    }*/
    UINT subFrameLength = 0;
    auto ret100 = m_pAudioSource->get_SubFrameLengthInBytes(&subFrameLength);
    if (ret100 != S_OK)
    {
        cout << "error : m_pAudioSource->get_SubFrameLengthInBytes(&subFrameLength);" << endl;
    }
    else
    {
        cout << subFrameLength << endl;
        audioBuffer.resize(subFrameLength);
        audio_initialized = true;
    }
    
}
//void updateAudioFrame()
//{
//    //if (!audio_initialized)
//      //  initializeAudio();
//	
//	IAudioBeamFrameReader *m_pAudioBeamFrameReader = nullptr;
//	IAudioBeamFrameList *m_pAudioFrameBeamList = nullptr;
//   
//    UINT beamCount = 0;
//	auto ret = m_pAudioBeamFrameReader->AcquireLatestBeamFrames(&m_pAudioFrameBeamList);
//	if (ret != S_OK)
//	{
//		cout << "error : m_pAudioBeamFrameReader->AcquireLatestBeamFrames(&m_pAudioFrameBeamList);" << endl;
//	}
//	else
//	{
//		cout << "successful 155" << endl;
//	}
//    auto ret3 = m_pAudioFrameBeamList->get_BeamCount(&beamCount);
//    if (ret3 != S_OK)
//    {
//        cout << "error : m_pAudioFrameBeamList->get_BeamCount(&beamCount);" << endl;
//    }
//    for (int i = 0; i < (int)beamCount; i++)
//    {
//        IAudioBeamFrame *m_pAudioBeamFrame;
//        m_pAudioFrameBeamList->OpenAudioBeamFrame(i, &m_pAudioBeamFrame);
//        UINT subFrameCount = 0;
//        m_pAudioBeamFrame->get_SubFrameCount(&subFrameCount);
//        for (int j = 0; j < (int)subFrameCount; j++)
//        {
//            IAudioBeamSubFrame *m_pAudioBeamSubFrame;
//            m_pAudioBeamFrame->GetSubFrame(j, &m_pAudioBeamSubFrame);
//            m_pAudioBeamSubFrame->CopyFrameDataToArray((UINT)audioBuffer.size(), &audioBuffer[0]);
//            cout << (UINT)audioBuffer.size() << endl;
//            audioFile.Write(&audioBuffer[0], (UINT)audioBuffer.size());
//        }
//    }
//}
void depthImage()
{
    while (1)
    {
        //初始化深度流
        IDepthFrameSource *m_pDepthSource = nullptr;
        m_pKinectSensor->get_DepthFrameSource(&m_pDepthSource);
        //取得深度尺寸
        int depth_height = 0, depth_width = 0;
        IFrameDescription *m_pDepthDescription = nullptr;
        m_pDepthSource->get_FrameDescription(&m_pDepthDescription);
        m_pDepthDescription->get_Height(&depth_height);
        m_pDepthDescription->get_Width(&depth_width);
        m_pDepthDescription->Release();
        //打开数据阅读器
        IDepthFrameReader *m_pDepthReader = nullptr;
        m_pDepthSource->OpenReader(&m_pDepthReader);
        //创建depth image buffer
        IDepthFrame *m_pDepthFrame = nullptr;
        UINT16 *depthArray = new UINT16[depth_height * depth_width];
        Mat depthImg(depth_height, depth_width, CV_8UC1);
        Mat depthImgSave(depth_height, depth_width, CV_16UC1);
        string savefilename;
        string saveReadfilename;
        if (m_pDepthReader->AcquireLatestFrame(&m_pDepthFrame) == S_OK)
        {
            m_pDepthFrame->CopyFrameDataToArray(depth_height * depth_width, depthArray);
            m_pDepthFrame->CopyFrameDataToArray(depth_width * depth_height, reinterpret_cast<UINT16 *>(depthImgSave.data)); //强制转换数据类型
            for (int i = 0; i < depth_height; i++)                                                                          //遍历图像
            {
                for (int j = 0; j < depth_width; j++)
                {
                    depthImg.at<uchar>(i, j) = static_cast<uchar>(depthArray[i * depth_width + j]);
                }
            }
            imshow("depthImg", depthImg);
            string tmp;
            tmp = now();
            //sprintf_s(tmp, "%ddepthSave.png", a++); //也可以是其他类型的图片文件。比如.bmp
            // char tmpRead[64];
            //sprintf_s(tmpRead, "%ddepthRead.png", b++);
            savefilename = tmp + "depth16Bit.png";
            saveReadfilename = tmp + "depth8Bit.png";
            imwrite(saveReadfilename, depthImg); //存储转换后的8位深度图
            imwrite(savefilename, depthImgSave); //存储每一帧的深度图片
            waitKey(1);
            m_pDepthFrame->Release();
        }
        if (waitKey(30) == VK_ESCAPE)
        {
            m_pDepthReader->Release();
            m_pDepthSource->Release();
            break;
        }
    }
}
void colorImage()
{
    while (1)
    { //初始化，彩色图像
        IColorFrameSource *m_pColorSource = nullptr;
        m_pKinectSensor->get_ColorFrameSource(&m_pColorSource);
        //cout << &m_pColorSource << endl;
        //获取彩色图像的尺寸
        int color_height = 0, color_width = 0;
        IFrameDescription *m_pColorDescription = nullptr;
        m_pColorSource->get_FrameDescription(&m_pColorDescription);
        m_pColorDescription->get_Height(&color_height);
        m_pColorDescription->get_Width(&color_width);
        m_pColorDescription->Release();
        //打开数据阅读器
        IColorFrameReader *m_pColorReader = nullptr;
        m_pColorSource->OpenReader(&m_pColorReader);
        //创建color image buffer
        string savefilenamecolor;
        IColorFrame *m_pColorFrame = nullptr;
        Mat ColorImg(color_height, color_width, CV_8UC4);
        if (m_pColorReader->AcquireLatestFrame(&m_pColorFrame) == S_OK)
        {
            m_pColorFrame->CopyConvertedFrameDataToArray(color_width * color_height * 4, (BYTE *)ColorImg.data, ColorImageFormat_Bgra);
            imshow("ColorImg", ColorImg);
            string tmp;
            tmp = now();
            //sprintf_s(tmpcolor, "%dColorSave.jpg", c++);
            savefilenamecolor = tmp + "Color.jpg";
            imwrite(savefilenamecolor, ColorImg);
            waitKey(1);
            m_pColorFrame->Release();
        }
        if (waitKey(30) == VK_ESCAPE)
        {
            m_pColorSource->Release();
            m_pColorReader->Release();
            break;
        }
    }
}
int main(void)
{

    GetDefaultKinectSensor(&m_pKinectSensor);
    m_pKinectSensor->Open();
	
	audioFile.Open(now() + ".wav");
    thread thread1(depthImage);
    thread thread2(colorImage);
    thread thread3(initializeAudio);
   // thread thread4(updateAudioFrame);
    thread1.join();
    thread2.join();
    thread3.join();
  //  thread4.join();

    return 0;
}
