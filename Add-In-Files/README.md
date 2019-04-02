Setting up the add-in enviornment
1. Install following list of sdk's and other shit from VS installer
	1. Desktop dev with c++
	2. .NET Framework 4.6.1 sdk
	3. VC++ 2017 version 15.7 v14.14 toolset
	4. VC++ 2017 version 15.9 v14.16
	5. Visual C++ 2017 redistibutable update
	6. Visual ++ compilers and libraries for ARM64
	7. Visual C++ ATL for x86 and x64
	8. Visual C++ MFC for x86 and x64
	9. Windows 10 sdk(10.0.14393.0)
	10. Windows 8.1 sdk
2. Configure Paths in the project settings
	1. Under VC++ Directories
		1. Include Directories
			1.$(VC_IncludePath)
			2.$(WindowsSDK_IncludePath)
			3.C:\Users\Public\Documents\Autodesk\Inventor 2019\SDK\DeveloperTools\Include
	2. Under Reference Paths
		1. Include Directories
			1. $(VC_ReferencesPath_x64)
			2. C:\Users\Public\Documents\Audodesk\Inventor 2019\SDK\DeveloperTools\Include
	3. Under Library Directories
		1. Include Directories
			1. $(VC_LibraryPath_x64)
			2. $(WindowsSDK_LibraryPath_x64)
			3. $(NETFXKitsDir)Lib\um\x64
			4. C:\Program Files\Autodesk\Inventor 2019\Bin
	4. Under Linker/General
		1. Under Output File 
			1. $(outdir)/CommandWhisperer.dll
3. Placing .addin extension in the following directories
