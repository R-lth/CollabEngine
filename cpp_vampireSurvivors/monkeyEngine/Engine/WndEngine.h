#pragma once

//---------------------------------------------------------------------------------------------------------
// Author       : 고미소
// Date         : 2026-06-04
// Copyright    : 게임인재원 8기
// Description  : Wnd Engine 클래스
//
//              - Win32 API 기반의 엔진 클래스
// 
//              - 1. 객체 생성 불가
//---------------------------------------------------------------------------------------------------------

#include "../Core/Core.h"
#include "Inc_Windows.h"

namespace Monkey
{
    LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam); // 주 창의 메세지 처리

	class Monkey_API WndEngine
	{
        struct EngineSetting
        {
            float targetFrame = 0.f;
            float fixedFrame = 0.f;

            int width = 0;
            int height = 0;
        };

    public:
        WndEngine();
        virtual ~WndEngine() = default;

    protected:
        bool Create(const wchar_t* className, const wchar_t* windowName, int width, int height);
        void Destroy();

    protected:
        virtual void StartUp() = 0;
        virtual void ShutDown() = 0;

    private:
        void LoadEngineSetting();

    protected:
        EngineSetting setting;
        float targetFrameTime = 1.f;
        float fixedFrameTime = 1.f;

        HWND m_hWnd = HWND();

    private:
        friend LRESULT CALLBACK NzWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}