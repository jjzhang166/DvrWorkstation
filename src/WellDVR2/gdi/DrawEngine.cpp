#include "stdafx.h"
#include "DrawEngine.h"

#define PI 3.1415926

CDrawEngine::CDrawEngine(void)
{
}


CDrawEngine::~CDrawEngine(void)
{
}

CDrawEngine& CDrawEngine::Instance()
{
	static CDrawEngine de;
	return de;
}

//////////////////////////////////////////////////////////////////////////
//Function:	���Ʊ�ͼ
//Params:
//		hWnd [IN]		--		���ھ�����ڴ˴���������
//		rcDraw [IN]	--		�����޶�������������)
//		pieInfo [IN]	--		�����������,��ɫ������, ����ֵ������ҪΪ1
//		size [IN]		--		pecent�Ĵ�С
//Return:
//Date: [2014/11/15 yjt]
//History:
//////////////////////////////////////////////////////////////////////////
bool CDrawEngine::DrawPie(HWND hWnd, RECT rcDraw, PPieInfo pieInfo, int size)
{
	if(hWnd == NULL)
	{
		return false;
	}
	
	int angle = 0;
	//���pecent�ĺ��Ƿ�Ϊ1
	if(pieInfo && size > 1)
	{
		for(int i = 0; i < size; ++i)
		{
			angle += pieInfo[i].angle;
		}
		if(angle != 360)
		{
			return false;
		}
	}

	DrawPieChart * pie = new DrawPieChart(hWnd, rcDraw);
	for (int j = 0; j < size; ++j)
	{
		pie->AddPiece(pieInfo[j].backColor, pieInfo[j].textColor, pieInfo[j].angle, pieInfo[j].title);
	}
	delete pie;
	return true;
}