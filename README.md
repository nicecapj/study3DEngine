# study3DEngine

using DirectX9
		case WM_CREATE:
		{			
			gEngine = new S3DEngine(S3DEngine::DIRECTX);
![screenshot of Directx renderer](dxScreenshot.png?raw=true)		
		
		
using OPENGL 2.0
		case WM_CREATE:
		{			
			gEngine = new S3DEngine(S3DEngine::OPENGL);
![screenshot of opengl Renderer](openglScreenshot.png?raw=true)
			

[Build]
1. download directx sdk(9.0c) and install
https://www.microsoft.com/en-us/download/details.aspx?id=6812


2. download opengl sdk
https://www.opengl.org/resources/libraries/glut/

3.....


reference
opengl
https://www.opengl.org/resources/libraries/glut/
https://www.khronos.org/opengl/wiki/Getting_Started
https://www.opengl.org/sdk/