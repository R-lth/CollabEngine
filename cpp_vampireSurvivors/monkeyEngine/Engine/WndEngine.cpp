#include "WndEngine.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include "Engine.h"

namespace Monkey
{
	// 전역 함수
	LRESULT NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		switch(msg) 
		{
		case WM_CLOSE:
			{
				WndEngine* pWnd = reinterpret_cast<WndEngine*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
				if (pWnd) { pWnd->ShutDown(); }
				PostQuitMessage(0);
			}
			break;
		default:
			return::DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return NULL;
	}

	// WndEngine 클래스
	WndEngine::WndEngine()
	{
		LoadEngineSetting();

		assert(setting.targetFrame != 0.f);
		assert(setting.fixedFrame != 0.f);

		targetFrameTime /= setting.targetFrame;
		fixedFrameTime /= setting.fixedFrame;
	}

	bool WndEngine::Create(const wchar_t* className, const wchar_t* windowName, int width, int height)
	{
		// 윈도우 클래스 정의 및 등록
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = className;
		wc.lpfnWndProc = NzWndProc; // (중요) 윈도우 프로시저(함수) 포인터 등록 

		ATOM classId = 0;
		if (!GetClassInfoEx(HINSTANCE(), className, &wc))
		{
			classId = RegisterClassEx(&wc);

			if (0 == classId) return false;
		}

		setting.width = width;
		setting.height = height;

		RECT rc = { 0, 0, width, height };
		DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
		AdjustWindowRect(&rc, dwStyle, FALSE);

		// 윈도우 생성
		m_hWnd = ::CreateWindowEx(
			NULL, 
			MAKEINTATOM(classId), 
			L"vampire survivor", 
			dwStyle,
			CW_USEDEFAULT, 
			CW_USEDEFAULT,
			rc.right - rc.left, rc.bottom - rc.top, 
			HWND(), 
			HMENU(), 
			HINSTANCE(), 
			NULL);

		if (m_hWnd == NULL) { return false; }

		::SetWindowText((HWND)m_hWnd, windowName);
		::SetWindowLongPtr((HWND)m_hWnd, GWLP_USERDATA, (LONG_PTR)this);

		ShowWindow((HWND)m_hWnd, SW_SHOW);
		UpdateWindow((HWND)m_hWnd);

		return true;
	}

	void WndEngine::Destroy()
	{
		if (m_hWnd != NULL) 
		{
			DestroyWindow((HWND)m_hWnd);
			m_hWnd = NULL;
		}
	}

	void WndEngine::LoadEngineSetting()
	{
		std::ifstream file("../config/setting.txt");
		assert(file.is_open());

		std::string line;
		while (std::getline(file, line))
		{
			if (line.empty() || line[0] == '#') { continue; }

			const size_t equalPos = line.find('=');
			if (equalPos == std::string::npos) { continue; }

			std::string key = line.substr(0, equalPos);
			std::string value = line.substr(equalPos + 1);

			auto trim = [](std::string& str)
				{
					const char* whiteSpace = " \t\r\n";
					const size_t begin = str.find_first_not_of(whiteSpace); 
					if (begin == std::string::npos) { str.clear(); return; }
					const size_t end = str.find_last_not_of(whiteSpace); 
					
					str = str.substr(begin, end - begin /*count*/ + 1);
				};

			trim(key);
			trim(value);

			if (key == "targetFps") { setting.targetFrame = static_cast<float>(atof(value.c_str()/*char* 배열*/)/*문자열을 실수로 변환*/); }
			if (key == "fixedFps") { setting.fixedFrame = static_cast<float>(atof(value.c_str())); }
			if (key == "width")		{ setting.width = static_cast<int>(atof(value.c_str())); }
			if (key == "height")    { setting.height = static_cast<int>(atof(value.c_str())); }
		}
	}
}