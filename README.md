# Creating a nodejs native .Net extension
This short writeup will guide you through creating a simple node.js addon

## Before we begin
1. Read [this](http://coderesearchlabs.com/articles/BNWCA.pdf) short description on how to build a simple nodejs addon on windows.
2. Download node.js [sources](http://nodejs.org/#download).
3. Make sure you have [Python 2.x](http://www.python.org/download/) installed (not 3.x).
4. Build node sources by running `vcbuild.bat'.
5. Verify that node.lib is located under the Debug/ directory.

## Creating a .Net node addon
1. Fire up VS2010 and create a new C++ Empty CLR project.
![new project](https://github.com/saary/node.net/raw/master/images/CreateClrVCProject.png) 
2. Open the project properties (Click Alt+Enter while the project file is selected) 
3. Set the project type and extension project type should be *dll* and the extension should be *.node*
![new project](https://github.com/saary/node.net/raw/master/images/SetProjectType.png) 
4. Set the include directories as follows (replace paths with the local nodejs source path).
![new project](https://github.com/saary/node.net/raw/master/images/SetIncludes.png) 
5. Set the libraries directories as follows (replace paths with the local nodejs source path).
![new project](https://github.com/saary/node.net/raw/master/images/SetLibrariesDirs.png) 
6. Add a new CPP file and call it SOME_PREFIX_Addon.cpp.
This file will hold the actual addon definitions and dll entry point.
7. Open the property pages of the **new CPP file** you created.
8. Change the "Common Language Runtime Support" option to **No Common Language RunTime Support**                           
![new project](https://github.com/saary/node.net/raw/master/images/AddonProperties.png) 
