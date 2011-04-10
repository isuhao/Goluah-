#pragma once

#include <windows.h>
#include <stdio.h>
#include <d3d8.h>
#include <d3dx8.h>
#include "gobjbase.h"

#define NUM_JIMEN_Z 15
#define NUM_JIMEN_X 15

/*!
*	@brief �X�e�[�W�N���X
*/
class CStage : public CStageBase
{
public:
	CStage(SDI_STAGEINFO2 *info);
	~CStage();
	void InitializeStage();
	DWORD Action();
protected:
	DWORD DrawBack();
	DWORD Draw();
	
	void InitVrtx();
	void LoadTextures();

	void DrawWater();
	void DrawJimen();
	void DrawSky();
	void DrawJisaku1();

	//����
	LPDIRECT3DTEXTURE8 ptex_water;
	MYVERTEX3D vb_water[4];
	MYVERTEX3D vb_maewater[4];

	//�n��
	LPDIRECT3DTEXTURE8 ptex_jimen;
	LPDIRECT3DVERTEXBUFFER8 vb_jimen[NUM_JIMEN_Z];

	//��
	LPDIRECT3DTEXTURE8 ptex_sky;
	MYVERTEX3D vb_sky[4];

	//���쎩���̈��
	LPDIRECT3DTEXTURE8 ptex_jisaku1;
	MYVERTEX3D vb_jisaku1[4];

	//�X
	LPDIRECT3DTEXTURE8 ptex_forest;
	MYVERTEX3D vb_forest[4];
};

#ifndef RELEASE
#define RELEASE(a) if(a!=NULL){a->Release();a=NULL;}
#endif