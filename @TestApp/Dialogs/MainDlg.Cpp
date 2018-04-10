#ifndef _MAINDLG_CPP
#define _MAINDLG_CPP
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Windows.H>
#include <Stdio.H>
#include <Stdlib.H>
#include <Math.H>

#include "../Source/Entry.H"
#include "../Dialogs/MainDlg.H"
#include "../../CGraph.H"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace NSWFL::Windows;
using namespace NSWFL::Conversion;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool GraphTextFormatCallback(CGraph *pGraph, GRAPHSERIES *pSeries, GRAPHTEXTATTRIBUTES *pTextAttributes, HDC hDC)
{
	/* //Uncomment for some fun formatting/
	if(pGraph->Settings.Direction == GRAPH_LEFT)
	{
		pTextAttributes->x = pGraph->Width();
	}
	pTextAttributes->y = pSeries->Index * 35;

	pTextAttributes->BGMode = TRANSPARENT;
	pTextAttributes->FGColor = pSeries->LineColor;

	pTextAttributes->FontSize = 30;
	strcpy_s(pTextAttributes->FontName, sizeof(pTextAttributes->FontName), "Comic Sans MS");
	*/

	FormatDouble(pTextAttributes->Text, sizeof(pTextAttributes->Text), pSeries->TopValue, 0);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI GraphPlotter(LPVOID pVoid)
{
	CGraph graph(GetDlgItem((HWND)pVoid, IDC_GRAPH1),
		RGB(0, 0, 0), RGB(0, 100, 0), 15, GRAPH_LEFT, true);

	graph.AddSeries(RGB(255, 0, 0), 1, GraphTextFormatCallback);
	graph.AddSeries(RGB(0, 0, 255), 1, GraphTextFormatCallback);
	graph.AddSeries(RGB(100, 100, 100), 1, GraphTextFormatCallback);

	double fValues[3];
	double x = 90;
	double fScale = 1;
	bool bReverse = false;
	int iSleep = 200;

	srand(GetTickCount());

	while(IsWindow((HWND)pVoid))
	{
		fValues[0] = (sin(x++/10.0f) * 75.0f) + 100 * fScale;
		fValues[1] = (cos(x++/50.0f) * 75.0f) + 100 * fScale;
		fValues[2] = ((double)((rand() + 500) % 550));
		graph.Update(fValues);

		if(x > 1000)
		{
			if(x > 2380)
			{
				bReverse = true;
				iSleep = 100;
			}
			else if(x > 1940)
			{
				bReverse = false;
			}
			else if(x > 1100)
			{
				bReverse = true;
			}

			if(((int)x % 11) == 0)
			{
				if(bReverse)
				{
					if(fScale != 1)
					{
						fScale -= 0.5;
					}
				}
				else {
					fScale += 0.5;
				}
			}
		}

		if(x > 1700)
		{
			graph.Invalidate();
			Sleep(iSleep);
		}
	}

	graph.Destroy();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CALLBACK MainDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //--------------------------------------------------------------------------

	static CGraph graph;

	//--------------------------------------------------------------------------

    if(uMsg == WM_INITDIALOG) // Received an Initilize Dialog Message
    {
        SendMessage(hWnd, (UINT)WM_SETTEXT, (WPARAM)0, (LPARAM)"Graph Test");
        SendMessage(hWnd, WM_SETICON, TRUE, (LPARAM) LoadIcon(ghAppInstance, MAKEINTRESOURCE(IDI_MAIN)));

		CreateThread(NULL, NULL, GraphPlotter, hWnd, 0, NULL);

		graph.Initialize(GetDlgItem(hWnd, IDC_GRAPH2), RGB(0, 0, 0), RGB(0, 100, 0), 15, GRAPH_RIGHT, false);

		graph.AddSeries(RGB(255, 0, 0), 200, GraphTextFormatCallback);

        CenterWindow(hWnd);

		SetTimer(hWnd, 0, 10, NULL);

        return TRUE; // Return TRUE to set the keyboard focus, Otherwise return FALSE.
    }

	//--------------------------------------------------------------------------

	else if(uMsg == WM_TIMER)
    {
		static float x = 0;
		graph.Update((sin(x++/40.0f) * 75.0f) + 100);
		graph.Invalidate();
	}

    //--------------------------------------------------------------------------

    else if(uMsg == WM_CLOSE) //- Received a close message.
    {
		graph.Destroy();

        EndDialog(hWnd, 0);
        DestroyWindow(hWnd);

        return TRUE;
    }

    //--------------------------------------------------------------------------

    return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif

