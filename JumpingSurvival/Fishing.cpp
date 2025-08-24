#include"Fishing.h"

Fishing::Fishing(VECTOR pos)
{
	floatPos = pos;
	floatRotate = VGet(0, 0, 0);
	floatSize = VGet(0.05, 0.05, 0.05);
	fishPos = VGet(0, 0, 0);
	fishPos = VGet(0, 0, 0);
	fishSize = VGet(0, 0, 0);

	floatHandle = MV1LoadModel("data/model/float.mv1");
	MV1SetScale(floatHandle, floatSize);
}

Fishing::~Fishing()
{
	MV1DeleteModel(floatHandle);
	MV1DeleteModel(sharkHandle);
}

void Fishing::Init(VECTOR pos)
{
	floatPos = pos;
	floatRotate = VGet(0, 0, 0);
	fishPos = VGet(0, 0, 0);
	fishPos = VGet(0, 0, 0);

}

void Fishing::Update()
{
	MV1SetPosition(floatHandle, floatPos);
	MV1SetScale(floatHandle, floatSize);
}

void Fishing::Draw()
{
	MV1DrawModel(floatHandle);
}

void floatMove()
{

}

void Fishing::DebugMode()
{
	//����L�[�ō��W��O�Ɉړ�
	if (CheckHitKey(KEY_INPUT_UP))
	{
		floatPos.z++;
	}
	//�����L�[�ō��W�����Ɉړ�
	else if (CheckHitKey(KEY_INPUT_DOWN))
	{
		floatPos.z--;
	}
	//�E���L�[�ō��W���E�Ɉړ�
	else if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		floatPos.x++;
	}
	//�����L�[�ō��W�����Ɉړ�
	else if (CheckHitKey(KEY_INPUT_LEFT))
	{
		floatPos.x--;
	}

	//���V�t�g�L�[�����Ȃ���㉺����Y���W�ύX
	if (CheckHitKey(KEY_INPUT_LSHIFT) && CheckHitKey(KEY_INPUT_UP))
	{
		floatPos.y++;
	}
	else if (CheckHitKey(KEY_INPUT_LSHIFT) && CheckHitKey(KEY_INPUT_DOWN))
	{
		floatPos.y--;
	}

	//S�L�[�����Ȃ���㉺���ő傫���ύX
	if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_UP))
	{
		floatSize.x += 0.01;
		floatSize.y += 0.01;
		floatSize.z += 0.01;
	}
	else if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_DOWN))
	{
		floatSize.x -= 0.01;
		floatSize.y -= 0.01;
		floatSize.z -= 0.01;
	}
}