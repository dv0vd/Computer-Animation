#include <Windows.h>
#include <gdiplus.h>
#include <tchar.h>
#pragma comment(lib, "GdiPlus.lib")
using namespace Gdiplus;

bool affine = true;
Bitmap *steam;
Image * locomotive;
int frameIndex=0;
int frameCount;
float step = 0, stepp=0;
PointF raysStart[3] = {
	 PointF(470, 350),PointF(400, 300),PointF(400, 400)
};
PointF raysEnd[3] = {
	 PointF(-450, 350),PointF(-520, 300),PointF(-520, 400)
};
PointF raysResult[3];
PointF frontStart[3] = {
	 PointF(500,300), PointF(470,350), PointF(500,400) 
};
PointF frontEnd[3] = {
	 PointF(-420,300), PointF(-450,350), PointF(-420,400)
};
PointF frontResult[3];
PointF lightStart[1] = {
	PointF(465, 340)
};
PointF lightEnd[1] = {
	PointF(-455, 340)
};
PointF lightResult[1];
PointF unknownStart[1] = {
	PointF(650, 287)
};
PointF unknownEnd[1] = {
	PointF(-270, 287)
};
PointF unknownResult[1];
PointF cabineStart[1] = {
	PointF(800, 200)
};
PointF cabineEnd[1] = {
	PointF(-120, 200)
};
PointF cabineResult[1];
PointF windowStart[4] = {
		PointF(810, 210), PointF(910, 210),PointF(910, 270), PointF(810, 270)
};
PointF windowEnd[4] = {
	PointF(-110, 210), PointF(-10, 210),PointF(-10, 270), PointF(-110, 270)
};
PointF windowResult[4];
PointF frontBottomStart[4] = {
		PointF(500,398), PointF(500,440), PointF(450,440), PointF(490,398)
};
PointF frontBottomEnd[4] = {
	PointF(-420,398), PointF(-420,440), PointF(-470,440), PointF(-430,398)
};
PointF frontBottomResult[4];
PointF bodyStart[1] = {
		PointF(500, 300)
};
PointF bodyEnd[1] = {
	PointF(-420, 300)
};
PointF bodyResult[1];
PointF pipeStart[1] = {
	PointF(520, 250)
};
PointF pipeEnd[1] = {
	PointF(-400, 250)
};
PointF pipeResult[1];
PointF bigWheelsStart[4] = {
		PointF(830, 370),PointF(745, 370),PointF(865, 405),PointF(780, 405)
};
PointF bigWheelsEnd[4] = {
	PointF(-90, 370),PointF(-175, 370),PointF(-55, 405),PointF(-140, 405)
};
PointF bigWheelsResult[4];
PointF smallWheelsStart[4] = {
		PointF(510, 390),PointF(575, 390),PointF(535, 415),PointF(600, 415)
};
PointF smallWheelsEnd[4] = {
	PointF(-410, 390),PointF(-345, 390),PointF(-375, 415),PointF(-320, 415)
};
PointF smallWheelsResult[4];

PointF Tween(const PointF &A, const PointF &B, float t)
{
	return PointF(A.X*(1.f - t) + B.X*t, A.Y*(1.f - t) + B.Y*t);
}

void Tween(const PointF *A, const PointF *B, PointF *P, int count, float t)
{
	for (int i = 0; i < count; ++i)
		P[i] = Tween(A[i], B[i], t);
}

VOID MessageProcessing() {
	MSG msg;
	BOOL bRet;
	for (;;)
	{
		bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet == -1)
		{
			break;
		}
		else if (FALSE == bRet)
		{
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

Status MeasureImage(Image* image, const RectF& layout, StringAlignment align, RectF* result)
{
	if (NULL == image || layout.IsEmptyArea() || NULL == result)
	{
		return InvalidParameter;
	}
	RectF rect;
	float fRatio = (float)image->GetWidth() / (float)image->GetHeight();
	if (fRatio > (layout.Width / layout.Height))
	{
		rect.Width = layout.Width;
		rect.Height = layout.Width / fRatio;
		rect.X = layout.GetLeft();
		switch (align)
		{
		case StringAlignmentNear:
			rect.Y = layout.GetTop();
			break;
		case StringAlignmentCenter:
			rect.Y = layout.Y + (layout.Height - rect.Height) / 2.f;
			break;
		case StringAlignmentFar:
			rect.Y = layout.GetBottom() - rect.Height;
			break;
		}
	}
	else
	{
		rect.Width = fRatio * layout.Height;
		rect.Height = layout.Height;
		rect.Y = layout.GetTop();
		switch (align)
		{
		case StringAlignmentNear:
			rect.X = layout.GetLeft();
			break;
		case StringAlignmentCenter:
			rect.X = layout.X + (layout.Width - rect.Width) / 2.f;
			break;
		case StringAlignmentFar:
			rect.X = layout.GetRight() - rect.Width;
			break;
		}
	}
	rect.GetBounds(result);
	return Ok;
}

VOID Display(HDC hdc)
{
	Graphics main(hdc);
	main.SetSmoothingMode(SmoothingModeHighSpeed);
	Rect rectt;
	main.GetVisibleClipBounds(&rectt);
	Bitmap backBuffer(rectt.Width, rectt.Height, &main);
	Graphics g(&backBuffer);
	PointF cabineAffine[4] = {
		PointF(50, 0.6),PointF(62, 0.6),PointF(62, 20.6),PointF(50, 20.6)
	};
	PointF roofAffine[4] = {
		PointF(48, 0),PointF(63,0),PointF(63, 0.6),PointF(48, 0.6)
	};
	PointF bodyAffine[4] = {
		PointF(20, 10.6),PointF(50,10.6),PointF(50, 20.6),PointF(20, 20.6)
	};
	PointF frontAffine[3] = {
		PointF(20,10.6), PointF(17,15.9), PointF(20, 20.6)
	};
	PointF pipeAffine[4] = {
		PointF(22,5.6), PointF(25, 5.6), PointF(25, 10.6), PointF(22, 10.6)
	};
	PointF windowAffine[4] = {
		PointF(51,1.5), PointF(61 ,1.5), PointF(61, 7.5), PointF(51, 7.5)
	};
	PointF unknownAffine[1] = {
		PointF(34.8, 9.3)
	};
	PointF lightAffine[1] = {
		PointF(16.5, 14.6)
	};
	PointF bigWheelsAffine[4] = {
		PointF(53, 17.5),PointF(44.4, 17.5),PointF(56.5, 21.1),PointF(48, 21.1)
	};
	PointF smallWheelsAffine[4] = {
		PointF(21, 19.5),PointF(27.5, 19.5),PointF(23.5, 22),PointF(30, 22)
	};
	PointF frontBottomAffine[4] = {
		PointF(20,20.5), PointF(20,24.6), PointF(15,24.6), PointF(19,20.5)
	};
	if (affine) {
		g.Clear(Color::White);
		Matrix mx;
		stepp += 0.1;
		mx.Scale(stepp, stepp);
		mx.Translate(3 * stepp, 1.94*stepp);
		mx.TransformPoints(bodyAffine, 4);
		mx.TransformPoints(cabineAffine, 4);
		mx.TransformPoints(windowAffine, 4);
		mx.TransformPoints(bigWheelsAffine, 4);
		mx.TransformPoints(smallWheelsAffine, 4);
		mx.TransformPoints(roofAffine, 4);
		mx.TransformPoints(unknownAffine, 1);
		mx.TransformPoints(lightAffine, 1);
		mx.TransformPoints(pipeAffine, 4);
		mx.TransformPoints(frontAffine, 3);
		mx.TransformPoints(frontBottomAffine, 4);
		g.FillPolygon(&SolidBrush(Color::Magenta), cabineAffine, 4);
		g.FillPolygon(&SolidBrush(Color::Red), pipeAffine, 4);
		g.FillPolygon(&SolidBrush(Color::Blue), windowAffine, 4);
		g.FillPolygon(&SolidBrush(Color::Green), bodyAffine, 4);
		g.FillPolygon(&SolidBrush(Color::Green), frontAffine, 3);
		g.FillPolygon(&SolidBrush(Color::Gray), roofAffine, 4);
		g.FillPie(&SolidBrush(Color::Yellow), (int)unknownAffine[0].X, (int)unknownAffine[0].Y, (int)(6*stepp), (int)(2.5*stepp), 180, 180);
		PathGradientBrush pthGrBrush(windowAffine, 4);
		pthGrBrush.SetCenterColor(Color::White);
		Color color = Color::Blue;
		INT n = 1;
		pthGrBrush.SetSurroundColors(&color, &n);
		pthGrBrush.SetCenterPoint(PointF((float)(windowAffine[2].X - 3*stepp),(float)( windowAffine[2].Y - 3*stepp)));
		g.FillPolygon(&pthGrBrush, windowAffine, 4);
		HatchBrush hatchBrush(HatchStyleSmallGrid, Color::Black, Color::Gray);
		g.FillEllipse(&hatchBrush, (int)bigWheelsAffine[0].X, (int)bigWheelsAffine[0].Y, (int)(8*stepp), (int)(8*stepp));
		g.FillEllipse(&hatchBrush, (int)bigWheelsAffine[1].X, (int)bigWheelsAffine[1].Y, (int)(8 * stepp), (int)(8 * stepp));
		g.FillEllipse(&SolidBrush(Color::Red), (int)bigWheelsAffine[2].X, (int)bigWheelsAffine[2].Y, (int)stepp, (int)stepp);
		g.FillEllipse(&SolidBrush(Color::Red), (int)bigWheelsAffine[3].X, (int)bigWheelsAffine[3].Y, (int)stepp, (int)stepp);
		g.FillEllipse(&hatchBrush, (int)smallWheelsAffine[0].X, (int)smallWheelsAffine[0].Y, (int)(6 * stepp), (int)(6 * stepp));
		g.FillEllipse(&hatchBrush, (int)smallWheelsAffine[1].X, (int)smallWheelsAffine[1].Y, (int)(6*stepp), (int)(6 * stepp));
		g.FillEllipse(&SolidBrush(Color::Red), (int)smallWheelsAffine[2].X, (int)smallWheelsAffine[2].Y, (int)stepp, (int)stepp);
		g.FillEllipse(&SolidBrush(Color::Red), (int)smallWheelsAffine[3].X, (int)smallWheelsAffine[3].Y, (int)stepp, (int)stepp);
		g.FillEllipse(&SolidBrush(Color::Yellow), (int)lightAffine[0].X, (int)lightAffine[0].Y, (int)stepp, (int)(2* stepp));
		g.FillPolygon(&HatchBrush(HatchStyleLightUpwardDiagonal, Color::Orange, Color::Blue), frontBottomAffine, 4);
		g.FillEllipse(&SolidBrush(Color::White), (int)frontBottomAffine[0].X - (int)(10*stepp), (int)frontBottomAffine[0].Y - (int)(2* stepp), (int)(9*stepp), (int)(6*stepp));
	}
	else {
		HatchBrush hatchBrush(HatchStyleSmallGrid, Color::Black, Color::Gray);
		//очистка
		g.FillPolygon(&SolidBrush(Color::White), frontBottomResult, 4);
		g.FillPie(&SolidBrush(Color::White), (int)unknownResult[0].X, (int)unknownResult[0].Y, 60, 25, 180, 180);
		g.FillPolygon(&SolidBrush(Color::White), frontResult, 3);
		g.FillRectangle(&SolidBrush(Color::White), (int)cabineResult[0].X - 20, (int)cabineResult[0].Y - 6, 150, 6);
		g.FillEllipse(&SolidBrush(Color::White), (int)smallWheelsResult[0].X, (int)smallWheelsResult[0].Y, 60, 60);
		g.FillEllipse(&SolidBrush(Color::White), (int)smallWheelsResult[1].X, (int)smallWheelsResult[1].Y, 60, 60);
		g.FillRectangle(&SolidBrush(Color::White), (int)bodyResult[0].X, (int)bodyResult[0].Y, 300, 100);
		g.FillRectangle(&SolidBrush(Color::White), (int)cabineResult[0].X, (int)cabineResult[0].Y, 120, 200);
		g.FillRectangle(&SolidBrush(Color::White), (int)pipeResult[0].X, (int)pipeResult[0].Y, 30, 50);
		g.FillEllipse(&SolidBrush(Color::White), (int)bigWheelsResult[0].X, (int)bigWheelsResult[0].Y, 80, 80);
		g.FillEllipse(&SolidBrush(Color::White), (int)bigWheelsResult[1].X, (int)bigWheelsResult[1].Y, 80, 80);
		g.FillEllipse(&SolidBrush(Color::White), (int)smallWheelsResult[2].X, (int)smallWheelsResult[2].Y, 10, 10);
		g.FillEllipse(&SolidBrush(Color::White), (int)smallWheelsResult[3].X, (int)smallWheelsResult[3].Y, 10, 10);
		g.FillEllipse(&SolidBrush(Color::White), (int)bigWheelsResult[2].X, (int)bigWheelsResult[2].Y, 10, 10);
		g.FillEllipse(&SolidBrush(Color::White), (int)bigWheelsResult[3].X, (int)bigWheelsResult[3].Y, 10, 10);
		//кабина
		Tween(cabineStart, cabineEnd, cabineResult, 1, step);
		g.FillRectangle(&SolidBrush(Color::Magenta), (int)cabineResult[0].X, (int)cabineResult[0].Y, 120, 200);
		//крыша
		g.FillRectangle(&SolidBrush(Color::Gray), (int)cabineResult[0].X - 20, (int)cabineResult[0].Y - 6, 150, 6);
		//окно
		Tween(windowStart, windowEnd, windowResult, 4, step);
		PathGradientBrush pthGrBrush(windowResult, 4);
		pthGrBrush.SetCenterColor(Color::White);
		Color color = Color::Blue;
		INT n = 1;
		pthGrBrush.SetSurroundColors(&color, &n);
		pthGrBrush.SetCenterPoint(Point(windowResult[2].X -30, windowResult[2].Y-30));
		g.FillPolygon(&pthGrBrush, windowResult, 4);
		//корпус
		Tween(bodyStart, bodyEnd, bodyResult, 1, step);
		g.FillRectangle(&SolidBrush(Color::Green), (int)bodyResult[0].X, (int)bodyResult[0].Y, 300, 100);
		//труба
		Tween(pipeStart, pipeEnd, pipeResult, 4, step);
		g.FillRectangle(&SolidBrush(Color::Red), (int)pipeResult[0].X, (int)pipeResult[0].Y, 30, 50);
		//маленькие колЄса
		Tween(smallWheelsStart, smallWheelsEnd, smallWheelsResult, 4, step);
		g.FillEllipse(&hatchBrush, (int)smallWheelsResult[0].X, (int)smallWheelsResult[0].Y, 60, 60);
		g.FillEllipse(&hatchBrush, (int)smallWheelsResult[1].X, (int)smallWheelsResult[1].Y, 60, 60);
		g.FillEllipse(&SolidBrush(Color::Red), (int)smallWheelsResult[2].X, (int)smallWheelsResult[2].Y, 10, 10);
		g.FillEllipse(&SolidBrush(Color::Red), (int)smallWheelsResult[3].X, (int)smallWheelsResult[3].Y, 10, 10);
		//большие колЄса
		Tween(bigWheelsStart, bigWheelsEnd, bigWheelsResult, 4, step);
		g.FillEllipse(&hatchBrush, (int)bigWheelsResult[0].X, (int)bigWheelsResult[0].Y, 80, 80);
		g.FillEllipse(&hatchBrush, (int)bigWheelsResult[1].X, (int)bigWheelsResult[1].Y, 80, 80);
		g.FillEllipse(&SolidBrush(Color::Red), (int)bigWheelsResult[2].X, (int)bigWheelsResult[2].Y, 10, 10);
		g.FillEllipse(&SolidBrush(Color::Red), (int)bigWheelsResult[3].X, (int)bigWheelsResult[3].Y, 10, 10);
		//передн€€ часть
		Tween(frontStart, frontEnd, frontResult, 3, step);
		g.FillPolygon(&SolidBrush(Color::Green), frontResult, 3);
		//непон€тна€ штука на корпусе
		Tween(unknownStart, unknownEnd, unknownResult, 1, step);
		g.FillPie(&SolidBrush(Color::Yellow), (int)unknownResult[0].X, (int)unknownResult[0].Y, 60, 25, 180, 180);
		//нос
		Tween(frontBottomStart, frontBottomEnd, frontBottomResult, 4, step);
		g.FillPolygon(&HatchBrush(HatchStyleLightUpwardDiagonal, Color::Orange, Color::Blue), frontBottomResult, 4);
		g.FillEllipse(&SolidBrush(Color::White), (int)frontBottomResult[0].X-100, 380, 90, 60);
		//фонарь
		Tween(lightStart, lightEnd, lightResult, 1, step);
		g.FillEllipse(&SolidBrush(Color::Yellow), (int)lightResult[0].X, (int)lightResult[0].Y, 10, 20);
		step += 0.02;
		if (step > 1) {
			step = 0;
			stepp = 0;
			g.Clear(Color::White);
			affine = true;
		}
	}
	if (stepp <= 10) {
		affine = true;
	}
	else {
		affine = false;
	}
	RectF coordinates(700, 15, 300, 150);
	MeasureImage(locomotive, coordinates, StringAlignmentNear, &coordinates);
	g.DrawImage(locomotive, coordinates);
	main.DrawImage(&backBuffer, rectt);
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ERASEBKGND:
		return 1;
	case WM_TIMER:
		frameIndex = (frameIndex + 1) % frameCount;
		locomotive->SelectActiveFrame(&FrameDimensionTime, frameIndex);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		Display(hdc);
		EndPaint(hWnd, &ps);
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

WNDCLASSEX RegisterWindowClass(HINSTANCE hInstance) {
	WNDCLASSEX window = { sizeof(WNDCLASSEX) }; 
	window.style = CS_HREDRAW | CS_VREDRAW; 
	window.lpfnWndProc = WindowProcedure; 
	window.hInstance = hInstance; 
	window.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window.hCursor = LoadCursor(NULL, IDC_ARROW); 
	window.hbrBackground = WHITE_BRUSH;
	window.lpszMenuName = NULL; 
	window.lpszClassName = TEXT("WindowClass");
	window.hIconSm = LoadIcon(NULL, IDI_APPLICATION); 
	return window; 
}

int WINAPI  _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR lpszCmdLine, int nCmdShow)
{
	if (!RegisterClassEx(&RegisterWindowClass(hInstance)))
		return EXIT_FAILURE;
	HWND hWnd = CreateWindowEx(0, TEXT("WindowClass"), TEXT("Ћабораторна€ работа є3"), WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 100, 50, 1000, 600, NULL, NULL, hInstance, NULL);
	if (NULL == hWnd)
		return EXIT_FAILURE;
	ShowWindow(hWnd, nCmdShow);
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	if (Ok == GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL))
	{
		steam = new Bitmap(TEXT("steam.png"));
		locomotive = Image::FromFile(TEXT("1FNl.gif"));
		frameCount = locomotive->GetFrameCount(&FrameDimensionTime);
		SetTimer(hWnd, 1, USER_TIMER_MINIMUM, NULL);
		MessageProcessing();
		delete locomotive;
		delete steam;
		GdiplusShutdown(gdiplusToken);
	}
}
