Capaware RC1 0.1 version

Before you start some remarks: 

1 .- In order to compile correctly this Capaware version, you should have installed the 2005 version of Microsoft Visual Studio Professional with the Service Pack 1. 

2 .- The initial version of Capaware works perfectly with NVIDIA video cards. We have detected some display problems with some models of ATI cards. 


Instructions: 

*** Before compiling: 

- Install, preferably in c:\wxWidgets-2.8.4 wxWidgets libraries to be found in the repository where you downloaded Capaware. We have not tested Capaware with newer versions of wxWidgets,   so we can not ensure that everything works fine with them. Compile the libraries following the instructions of MSW INSTALL_MSW of changing the value of the variable WXUSE_GLCANVAS to 1 in the archives setup.h located at:
- C:\wxWidgets-2.8.4\include\wx\MSW
- C:\wxWidgets-2.8.4\lib\vc_lib\MSW\wx
- C:\wxWidgets-2.8.4\lib\vc_lib\mswd\wx

- Create the system environment variable WXWIDGET_ROOT giving it the value of the installation directory of the wxWidgets libraries.

- Download and unzip, preferably, in C:\Capaware the compressed file Capaware_src_rc1_0.1.rar. 
- Download and unzip in <installation_folder>\extern the external file dependencies Capaware_dep_bin_rc1_0.1.rar 
- Download and unzip in <installation_folder>\extern the external file dependencies Capaware_dep_include_rc1_0.1.rar 
- Download and unzip in <installation_folder>\extern the external file dependencies Capaware_dep_lib_rc1_0.1.rar 
- Include in your system path <installation_folder>\extern\bin 
- Include in your system path <installation_folder>\extern\bin\osg 


*** Compilation MVS2005 automatically (recommended): 

1 .- Open <installation_folder>\apps\capaware\capaware.sln and compile the solution in the desired mode (Release or Debug). 

As a demonstration, in <installation_folder>\src\plugins you will find some very simple examples of plugins on Capaware. Use them as examples to any other plugin you need to develop. 

*** Notes: 

* In <installation_folder>\doc\user_guide you will find a brief initial paper on Capaware. (only in Spanish) 

Thanks for your interest in Capaware. 

Greetings, 

Capaware Team 



For additional information, contact: 

  Instituto Tecnológico de Canarias (ITC) 
  Playa de Pozo Izquierdo s/n 
  Santa Lucia - Las Palmas 
  35119 Gran Canaria 
  Spain 
  
  +34 928727503 - Fax +34 928727517 
  software@itccanarias.org 
  www.itccanarias.org 

      or 

  University of Las Palmas de Gran Canaria (ULPGC) 
  Dept. of Computer Science and Systems 
  Campus Tafira 
  35017 Las Palmas 
  Spain 
  
  +34 928458710/708 - Fax +34 9288711 
  atrujillo@dis.ulpgc.es 
  www.dis.ulpgc.es