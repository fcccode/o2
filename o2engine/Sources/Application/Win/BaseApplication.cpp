#include "BaseApplication.h"

#include "Application/Input.h"
#include "Render/Render.h"
#include "Utils/Debug.h"
#include "Utils/Log/LogStream.h"
#include "Utils/Time.h"

namespace o2
{
	BaseApplication::BaseApplication():
		IApplication(), mHWnd(0), mWndStyle(0), mWindowed(true), mWindowedSize(800, 600), mWindowedPos(0, 0),
		mWindowResizible(true), mActive(false)
	{
		mApplication = this;
		InitializeWindow();

		mRender = mnew Render();
		//mRenderSystem = mnew RenderSystem();
	}

	BaseApplication::~BaseApplication()
	{
		mRender.Release();
		//SafeRelease(mUIController);
		//SafeRelease(mRenderSystem);
		//SafeRelease(mTimer);
	}

	void BaseApplication::InitializeWindow()
	{
		mLog->Out("Initializing window..");

		mWndStyle = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

		WNDCLASSEX wndClass;
		wndClass.cbSize         = sizeof(WNDCLASSEX);
		wndClass.style			= mWndStyle;
		wndClass.lpfnWndProc	= (WNDPROC)WndProc;
		wndClass.cbClsExtra		= 0;
		wndClass.cbWndExtra		= 0;
		wndClass.hInstance		= NULL;
		wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground	= (HBRUSH)GetStockObject(GRAY_BRUSH);
		wndClass.lpszMenuName	= NULL;
		wndClass.lpszClassName	= "o2App";
		wndClass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wndClass))
		{
			mLog->Error("Can't register class");
			return;
		}

		if (!(mHWnd = CreateWindowEx(NULL, wndClass.lpszClassName, "App test",
			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			mWindowedPos.x, mWindowedPos.y, mWindowedSize.x, mWindowedSize.y,
			NULL, NULL, NULL, NULL)))
		{

			mLog->Error("Can't create window (CreateWindowEx)");
			return;
		}

		mLog->Out("Window initialized!");
	}

	void BaseApplication::Launch()
	{
		ShowWindow(mHWnd, SW_SHOW);

		mLog->Out("Application launched!");

		OnStarted();
		onStartedEvent.Invoke();

		MSG msg;
		memset(&msg, 0, sizeof(msg));

		//mTimer->Reset();

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				ProcessFrame();
			}
		}

		OnClosing();
		onClosingEvent.Invoke();
	}

	LRESULT BaseApplication::WndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		POINT pt;
		RECT rt;
		int key = 0;
		Vec2I size, pos;
		GetCursorPos(&pt);
		ScreenToClient(wnd, &pt);
		Vec2F cursorPos = Vec2F((float)pt.x, (float)-pt.y);

		if (mApplication->mRender)
			cursorPos -= Vec2F(mApplication->mRender->mResolution.x*0.5f, -mApplication->mRender->mResolution.y*0.5f);

		float wheelDelta;

		switch (uMsg)
		{
			case WM_LBUTTONDOWN:
			mApplication->mInput->CursorPressed(cursorPos);
			break;

			case WM_LBUTTONUP:
			mApplication->mInput->CursorReleased();
			break;

			case WM_RBUTTONDOWN:
			mApplication->mInput->AltCursorPressed(cursorPos);
			break;

			case WM_RBUTTONUP:
			mApplication->mInput->AltCursorReleased();
			break;

			case WM_MBUTTONDOWN:
			mApplication->mInput->Alt2CursorPressed(cursorPos);
			break;

			case WM_MBUTTONUP:
			mApplication->mInput->Alt2CursorReleased();
			break;

			case WM_KEYDOWN:
			key = (int)wParam;
			mApplication->mInput->KeyPressed(key);
			break;

			case WM_KEYUP:
			mApplication->mInput->KeyReleased((int)wParam);
			break;

			case WM_MOUSEMOVE:
			mApplication->mInput->SetCursorPos(cursorPos);
			break;

			case WM_MOUSEWHEEL:
			wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			mApplication->mInput->SetMouseWheelDelta(wheelDelta);
			break;

			case WM_ACTIVATE:
			if ((HWND)lParam == mApplication->mHWnd || true)
			{
				if (LOWORD(wParam) == WA_ACTIVE || LOWORD(wParam) == WA_CLICKACTIVE)
				{
					mApplication->mActive = true;
					mApplication->OnActivated();
					mApplication->onActivatedEvent.Invoke();
				}
				else
				{
					mApplication->mActive = false;
					mApplication->OnDeactivated();
					mApplication->onDeactivatedEvent.Invoke();
				}
			}
			break;

			case WM_SIZE:
			GetWindowRect(mApplication->mHWnd, &rt);
			size.x = rt.right - rt.left; size.y = rt.bottom - rt.top;

			if (size.x > 0 && size.y > 0 && size != mApplication->mWindowedSize)
			{
				mApplication->mWindowedSize = size;
				mApplication->OnResizing();
				mApplication->mRender->mResolution = size;
				mApplication->onResizingEvent.Invoke();
			}
			//mApplication->ProcessFrame();

			break;

			case WM_MOVE:
			GetWindowRect(mApplication->mHWnd, &rt);
			pos.x = rt.left; pos.y = rt.top;

			if (pos.x < 10000 && pos.y < 10000 && pos != mApplication->mWindowedPos)
			{
				mApplication->mWindowedPos = pos;
				mApplication->OnMoved();
				mApplication->onMovingEvent.Invoke();
			}
			break;

			case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
		}
		return DefWindowProc(wnd, uMsg, wParam, lParam);
	}

	void BaseApplication::ResetWnd()
	{
		SetFullscreen(!mWindowed);
	}

	void BaseApplication::SetFullscreen(bool fullscreen /*= true*/)
	{
		if (fullscreen)
		{
			//mRenderSystem->FrameResized();
			mLog->Out("Setting full screen");
		}
		else
		{
			mLog->Out("Setting windowed..");

			mWindowed = true;

			RECT rt = { mWindowedPos.x, mWindowedPos.y, mWindowedPos.x + mWindowedSize.x, mWindowedPos.y + mWindowedSize.y };
			AdjustWindowRect(&rt, mWndStyle, false);
			SetWindowPos(mHWnd, HWND_NOTOPMOST, mWindowedPos.x, mWindowedPos.y,
						 mWindowedSize.x, mWindowedSize.y, SWP_SHOWWINDOW);

			//mRenderSystem->FrameResized();
			mLog->Out("Complete");
		}
	}

	void BaseApplication::OnUpdate(float dt)
	{
		o2Debug.Log("Time: %f", o2Time.GetLocalTime());
	}

	void BaseApplication::OnDraw()
	{
	}

	bool BaseApplication::IsFullScreen() const
	{
		return !mWindowed;
	}

	void BaseApplication::SetResizible(bool resizible)
	{
		if (resizible == mWindowResizible)
			return;

		mWindowResizible = resizible;

		if (mWindowResizible)
			mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
		else
			mWndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX;

		mLog->Out("set resizible: %s ", (mWindowResizible ? "true":"false"));

		SetWindowLong(mHWnd, GWL_STYLE, mWndStyle);
	}

	bool BaseApplication::IsResizible() const
	{
		return mWindowResizible;
	}

	void BaseApplication::SetWindowSize(const Vec2I& size)
	{
		mWindowedSize = size;
		mLog->Out("setWindowSize: %ix%i", mWindowedSize.x, mWindowedSize.y);
		ResetWnd();
	}

	Vec2I BaseApplication::GetWindowSize() const
	{
		return mWindowedSize;
	}

	void BaseApplication::SetWindowPosition(const Vec2I& position)
	{
		mWindowedPos = position;
		mLog->Out("set Window Position: %i, %i", mWindowedPos.x, mWindowedPos.y);
		ResetWnd();
	}

	Vec2I BaseApplication::GetWindowPosition() const
	{
		return mWindowedPos;
	}

	void BaseApplication::SetWindowCaption(const String& caption)
	{
		mWndCaption = caption;
		SetWindowText(mHWnd, mWndCaption.Data());
	}

	String BaseApplication::GetWindowCaption() const
	{
		return mWndCaption;
	}

	void BaseApplication::SetContentSize(const Vec2I& size)
	{
		Vec2I clientRectSize = size;

		RECT clientRect;
		GetClientRect(mHWnd, &clientRect);
		clientRect.right = clientRect.left + size.x;
		clientRect.bottom = clientRect.top + size.y;

		AdjustWindowRect(&clientRect, mWndStyle, false);

		mWindowedPos = Vec2I(clientRect.left, clientRect.top);
		mWindowedSize = Vec2I(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

		mLog->Out("set Content Size: %ix%i", size.x, size.y);

		ResetWnd();
	}

	Vec2I BaseApplication::GetContentSize() const
	{
		RECT clientRect;
		GetClientRect(mHWnd, &clientRect);
		return Vec2I(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}

	BaseApplication* BaseApplication::mApplication = nullptr;
}