# KinectV2_Image_Capturing
This is a demo code for capturing depth image utilising Kinect V2 on windows. 

Requirement: 
Software:
    Kinect Windows v2 SDK: https://www.microsoft.com/en-au/download/details.aspx?id=44561 
        **Kinect v2 == Kinect for Windows v2
        
   OpenCV: https://opencv.org/opencv-4-0-0.html 
        In the development period, OpenCV4.0.0 has not been released, so, we encourage OpenCV 3.*.*. 
        
   Visual Studio 2017 has been using whole development period, so, it is better downloading  VS2017. However, any other version should not be a problem.

Hardware:
    USB 3.0

Tips:
 Project1.sln is the file that instructs the VS2017 to build the project. After you configure all the properties and linker stuffs VS2017 will save as this Project1.sln.
 Official SDK Documentation: https://docs.microsoft.com/en-us/previous-versions/windows/kinect/dn791365(v%3dieb.10)

To run this project and get the results properly, it is nessecary to adjust the properties in the property manager. That is because the property manager gives the directory to the
 VS2017 to get it find the function written in the cpp file. 
1. Go to View > Property Manager > Add New Project Property Sheet. 
2. In the Property Manager, right click on the project name and select properties. 
3. Under C/C++ > Additional Include Directories, add "$(KINECTSDK20_DIR)/include".

4. Under Linker > Additional Library Directories, add "$(KINECTSDK20_DIR)/lib/x64" (or /x86 for 32-bit systems)
                        .
5. Under Linker > Input > Additional Dependencies, add "kinect20.lib".

6. Under cv++ Directories > include Path add "$(OpenCV_DIR)/build/include/opencv2"
    
7. Under Linker > input > additional Dependencies add :opencv_world330d. 

    **this file name 'opencv_world330d' maybe vary depends on your opencv version, it could be different from mine. The post-fix "d" stands for debug, so if you are under configuration, choose "d".\
    ** $(KINECTSDK20_DIR) is a variable not a exact value, it presents the directory of the sdk you installed. 
    **If you are not sure about the number in the file, just use google with your opencv Version.
 Still stuck at some point, check this out https://www.youtube.com/watch?v=M-VHaLHC4XI for complete opencv configuration in vs2017 windows. 

The recorded file will be stored under KinectStream > Project1.  
