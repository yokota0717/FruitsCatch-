#pragma once
#include "MyPG.h"

enum TaskFlag {
	Task_Non,      //�^�X�N���I��
	Task_Title,    //�^�C�g��
	Task_HowTofor1,    //�V�ѕ���l�p
	Task_HowTofor2,    //�V�ѕ���l�p
	Task_Gamefor1, //��l�v���C�Q�[���{��
	Task_Gamefor2, //��l�v���C�p 
	Task_Endingfor1,   //�G���f�B���O
	Task_Endingfor2,   //�G���f�B���O
};

extern  void  MyGameMain_Finalize( );
extern  void  MyGameMain_Initialize( );
extern  void  MyGameMain_UpDate( );
extern  void  MyGameMain_Render2D( );

