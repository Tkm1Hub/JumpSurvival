//-----------------------------------------------------------------------------
// 10/18 ����J�n
// �W�����v�Q�[��3D
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include"Game.h"

//-----------------------------------------------------------------------------
// @brief  ���C���֐�.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// ��ʃ��[�h�̃Z�b�g
	SetGraphMode(1600, 900, 32);
	ChangeWindowMode(FALSE);


	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		return -1;	// �G���[���N�����璼���ɏI��
	}

	Game game;

	// �G�X�P�[�v�L�[��������邩�E�C���h�E��������܂Ń��[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		game.Update();

		// ��ʂ�����������
		ClearDrawScreen();

		game.Draw();

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}