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
![new project](raw/master/images/CreateClrVCProject.png)          
