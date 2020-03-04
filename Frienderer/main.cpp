#include <Windows.h>
#include <Windowsx.h>

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <string>
#include <chrono>

#include "Math.h"
#include "Camera.h"
#include "Render.h"

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

static Mesh testMesh;
static Mesh testMesh2;
BITMAP testTexture;

static Buffer buffer;
Camera cam;

bool constrainMouse = false;

auto start = std::chrono::steady_clock::now();

void setup() {
	
	HBITMAP hTestTexture = (HBITMAP)LoadImage(NULL, "Images/testPic.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (!GetObject(hTestTexture, sizeof(BITMAP), &testTexture)) std::cout << "Could not get bitmap object.\n";

	uint8_t pixels[100];
	uint8_t* pixel = (uint8_t*)testTexture.bmBits;

	for (int i = 0; i < 100; i++) {
		pixels[i] = *pixel++;
	}


	testMesh.texture = testTexture;
	std::cout << "Bitmap height: " << testMesh.texture.bmHeight << std::endl;
	std::cout << "Bits per pixel: " << testMesh.texture.bmBitsPixel << std::endl;
	std::cout << "Bytes per width: " << testMesh.texture.bmWidthBytes << std::endl;

	testMesh.tris.push_back(Tri(Vec3f(-5.0f, 5.0f, 5.0f), Vec3f(-5.0f, -5.0f, 5.0f), Vec3f(5.0f, -5.0f, 5.0f), Vec2f(1, 0), Vec2f(1, 1), Vec2f(0, 1), Colour(255, 0, 0), Colour(255, 0, 0), Colour(255, 0, 0)));
	testMesh.tris.push_back(Tri(Vec3f(-5.0f, 5.0f, 5.0f), Vec3f(5.0f, -5.0f, 5.0f), Vec3f(5.0f, 5.0f, 5.0f), Vec2f(1, 0), Vec2f(0, 1), Vec2f(0, 0), Colour(255, 0, 0), Colour(255, 0, 0), Colour(255, 0, 0)));
	
	testMesh.tris.push_back(Tri(Vec3f(5.0f, 5.0f, -5.0f), Vec3f(-5.0f, 5.0f, 5.0f), Vec3f(5.0f, 5.0f, 5.0f), Vec2f(1, 1), Vec2f(0, 0), Vec2f(1, 0), Colour(0, 255, 0), Colour(0, 255, 0), Colour(0, 255, 0)));
	testMesh.tris.push_back(Tri(Vec3f(-5.0f, 5.0f, -5.0f), Vec3f(-5.0f, 5.0f, 5.0f), Vec3f(5.0f, 5.0f, -5.0f), Vec2f(0, 1), Vec2f(0, 0), Vec2f(1, 1), Colour(0, 255, 0), Colour(0, 255, 0), Colour(0, 255, 0)));
	
	testMesh.tris.push_back(Tri(Vec3f(5.0f, -5.0f, -5.0f), Vec3f(5.0f, 5.0f, -5.0f), Vec3f(5.0f, 5.0f, 5.0f), Vec2f(0, 1), Vec2f(0, 0), Vec2f(1, 0), Colour(0, 0, 255), Colour(0, 0, 255), Colour(0, 0, 255)));
	testMesh.tris.push_back(Tri(Vec3f(5.0f, 5.0f, 5.0f), Vec3f(5.0f, -5.0f, 5.0f), Vec3f(5.0f, -5.0f, -5.0f), Vec2f(1, 0), Vec2f(1, 1), Vec2f(0, 1), Colour(0, 0, 255), Colour(0, 0, 255), Colour(0, 0, 255)));
	
	testMesh.tris.push_back(Tri(Vec3f(-5.0f, 5.0f, 5.0f), Vec3f(-5.0f, 5.0f, -5.0f), Vec3f(-5.0f, -5.0f, -5.0f), Vec2f(0, 0), Vec2f(1, 0), Vec2f(1, 1), Colour(255, 255, 0), Colour(255, 255, 0), Colour(255, 255, 0)));
	testMesh.tris.push_back(Tri(Vec3f(-5.0f,-5.0f, 5.0f), Vec3f(-5.0f, 5.0f, 5.0f), Vec3f(-5.0f, -5.0f, -5.0f), Vec2f(0, 1), Vec2f(0, 0), Vec2f(1, 1), Colour(255, 255, 0), Colour(255, 255, 0), Colour(255, 255, 0)));
	
	testMesh.tris.push_back(Tri(Vec3f(5.0f, -5.0f, 5.0f), Vec3f(-5.0f, -5.0f, 5.0f), Vec3f(5.0f, -5.0f, -5.0f), Vec2f(1, 1), Vec2f(0, 1), Vec2f(1, 0), Colour(255, 0, 255), Colour(255, 0, 255), Colour(255, 0, 255)));
	testMesh.tris.push_back(Tri(Vec3f(5.0f, -5.0f, -5.0f), Vec3f(-5.0f, -5.0f, 5.0f), Vec3f(-5.0f, -5.0f, -5.0f), Vec2f(1, 0), Vec2f(0, 1), Vec2f(0, 0), Colour(255, 0, 255), Colour(255, 0, 255), Colour(255, 0, 255)));
	
	testMesh.tris.push_back(Tri(Vec3f(-5.0f, 5.0f, -5.0f), Vec3f(5.0f, -5.0f, -5.0f), Vec3f(-5.0f, -5.0f, -5.0f), Vec2f(0, 0), Vec2f(1, 1), Vec2f(0, 1), Colour(0, 255, 255), Colour(0, 255, 255), Colour(0, 255, 255)));
	testMesh.tris.push_back(Tri(Vec3f(5.0f, -5.0f, -5.0f), Vec3f(-5.0f, 5.0f, -5.0f), Vec3f(5.0f, 5.0f, -5.0f), Vec2f(1, 1), Vec2f(0, 0), Vec2f(1, 0), Colour(0, 255, 255), Colour(0, 255, 255), Colour(0, 255, 255)));
	


}

void drawShit(Buffer* buffer) {
	

	buffer->clearBuffer(Colour(64, 127, 191));

	static auto end = std::chrono::steady_clock::now();

	float t = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / 1000000000.f; // lol

	testMesh.rotateAroundAxis(0.001, Vec3f(1, 0, 0));
	testMesh.rotateAroundAxis(0.0005, Vec3f(0, 1, 0));
	testMesh.rotateAroundAxis(0.00025, Vec3f(0, 0, 1));

	cam.update();

	buffer->drawMesh(testMesh, cam);

	Vec3<float> move(0, 0, 0);

	float rotateH = 0;
	float rotateV = 0;

	if (GetKeyState('W') & 0x8000) {
		move = move + cam.dir;
	}
	if (GetKeyState('S') & 0x8000) {
		move = move - cam.dir;
	}
	if (GetKeyState('A') & 0x8000) {
		move = move + Vec3f::crossProduct(cam.dir, Vec3f(0, 1, 0));
	}
	if (GetKeyState('D') & 0x8000) {
		move = move - Vec3f::crossProduct(cam.dir, Vec3f(0, 1, 0));
	}
	if (GetKeyState(VK_CONTROL) & 0x8000) {
		move = move + Vec3<float>(0, 1, 0);
	}
	if (GetKeyState(VK_SPACE) & 0x8000) {
		move = move - Vec3<float>(0, 1, 0);
	}

	if (GetKeyState(VK_LEFT) & 0x8000) {
		rotateH = 0.001;
	}
	if (GetKeyState(VK_RIGHT) & 0x8000) {
		rotateH = -0.001;
	}
	if (GetKeyState(VK_UP) & 0x8000) {
		rotateV = -0.001;
	}
	if (GetKeyState(VK_DOWN) & 0x8000) {
		rotateV = 0.001;
	}

	move = Vec3f::normalize(move);
	move *= 0.2;

	cam.dir = cam.dir.rotateAroundY(rotateH);
	cam.dir = cam.dir.rotateAroundX(rotateV);

	cam.dir = Vec3f::normalize(cam.dir);

	
	cam.pos = cam.pos + move;
	
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		buffer.drawToScreen(hdc);
		EndPaint(hwnd, &ps);

		return 0;
	}

	case WM_KEYDOWN:
		break;

	case WM_KEYUP:
		switch (wParam) {
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			return 0;
		}

		break;

	case WM_INPUT:
	{
		UINT dwSize = 40;
		static BYTE lpb[40];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
			lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			int xPosRelative = raw->data.mouse.lLastX;
			int yPosRelative = raw->data.mouse.lLastY;
			
			if (constrainMouse) cam.rotateCamFromMouseMovement(xPosRelative, yPosRelative);
		}
		break;
	}

	case WM_SETFOCUS:
		ShowCursor(false);
		constrainMouse = true;
		break;

	case WM_KILLFOCUS:
		ShowCursor(true);
		constrainMouse = false;
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

HWND createWindow(HINSTANCE hInstance, int nCmdShow, const char* windowClassName, const char* windowText) {
	WNDCLASS wClass = {};

	wClass.lpfnWndProc = WindowProc;
	wClass.lpszClassName = windowClassName;
	wClass.hInstance = hInstance;

	RegisterClass(&wClass);

	HWND windowHandle = CreateWindowEx(NULL, windowClassName, windowText, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 500, 500, NULL, NULL, hInstance, NULL);

	if (windowHandle == NULL) {
		return NULL;
	}

	ShowWindow(windowHandle, nCmdShow);
	return windowHandle;
}

int CALLBACK WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, LPSTR CommandLine, int ShowCode) {

#ifdef _DEBUG	
	AllocConsole();
	freopen_s((FILE * *)stdout, "CONOUT$", "w", stdout);
#endif
	
	HWND window = createWindow(Instance, ShowCode, "Frienderer", "Frienderer");
	
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = window;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	buffer.width = 500;
	buffer.height = 500;
	buffer.bytesPerPixel = 4;

	buffer.info.bmiHeader.biSize = sizeof(buffer.info.bmiHeader);
	buffer.info.bmiHeader.biWidth = buffer.width;
	buffer.info.bmiHeader.biHeight = -buffer.height; // Negative to flip y-axis
	buffer.info.bmiHeader.biPlanes = 1;
	buffer.info.bmiHeader.biBitCount = 8 * 4;
	buffer.info.bmiHeader.biCompression = BI_RGB;

	int BitmapMemorySize = (buffer.width * buffer.height) * buffer.bytesPerPixel;

	buffer.memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	buffer.pitch = buffer.width * buffer.bytesPerPixel;

	MSG msg = {};

	setup();

	bool running = true;
	while (running) {
		
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				running = false;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		drawShit(&buffer);

		HDC hdc = GetDC(window);


		buffer.drawToScreen(hdc);
		ReleaseDC(window, hdc);
		
		if (constrainMouse) {
			POINT pt;
			pt.x = 250;
			pt.y = 250;
			ClientToScreen(window, &pt);
			SetCursorPos(pt.x, pt.y);

		}
		

	}

	return 1;
}