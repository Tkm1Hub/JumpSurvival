#include"Item.h"

Item::Item()
{
    HeartHandle = MV1LoadModel("data/model/Item/Heart.mv1");
    HeartPos = GenerateRandomPosition();
    HeartRotate = VGet(0, 0, 0);
    HeartScale = VGet(3.0, 3.0, 3.0);
    MV1SetScale(HeartHandle, HeartScale);
    ArrowHandle = MV1LoadModel("data/model/Item/Arrow.mv1");
    ArrowPos = VGet(0, 1.5f, 0);
    ArrowRotate = VGet(0, 0, 0);
    ArrowScale = VGet(0.02, 0.02, 0.02);
    MV1SetScale(ArrowHandle, ArrowScale);
    IsActiveHeart = false;
    IsTakeHeart = false;
    isActiveArrow = false;
    HeartRadius = 1.5f;
    ArrowAngle = 0;

    HeartSpawnTimer = new Timer(20000);
    HeartDisappearTimer = new Timer(10000);
}

Item::~Item()
{
    MV1DeleteModel(HeartHandle);
}

void Item::Init()
{
    HeartPos = GenerateRandomPosition();
    HeartRotate = VGet(0, 0, 0);
    HeartScale = VGet(3.0, 3.0, 3.0);
    MV1SetScale(HeartHandle, HeartScale);
    ArrowPos = VGet(0, 1.5f, 0);
    ArrowRotate = VGet(0, 0, 0);
    ArrowScale = VGet(0.02, 0.02, 0.02);
    MV1SetScale(ArrowHandle, ArrowScale);

    IsActiveHeart = false;
    IsTakeHeart = false;
    isActiveArrow = false;
    HeartRadius = 1.5f;
    ArrowAngle = 0;

}

void Item::Update(int playerHP,VECTOR playerPos)
{
    //�v���C���[�̗̑͂�3�ȉ��̏ꍇ�̂ݓ���
    if (playerHP < 3)
    {
        HeartRotate.y += RotateSpeed;   //�n�[�g����]������

        //�n�[�g���o�����łȂ��ꍇ
        if (!IsActiveHeart)
        {
            if (!HeartSpawnTimer->isActive())
            {
                HeartSpawnTimer->start();
            }
            ArrowPos = VGet(0, 1.5f, 0);
        }
        //�o�����̏ꍇ
        else
        {
            if (!HeartDisappearTimer->isActive())
            {
                HeartDisappearTimer->start();
            }
            
            //�v���C���[�ƃn�[�g�̊p�x�����߂�
            float deltaX = HeartPos.x - playerPos.x;
            float deltaZ = HeartPos.z - playerPos.z;

            ArrowAngle = atan2(-deltaX,-deltaZ);

            ArrowRotate.y = ArrowAngle;

            //���̍��W�����߂�
            float ArrowX = playerPos.x + ArrowR * -sinf(ArrowAngle);
            float ArrowZ = playerPos.z + ArrowR * -cosf(ArrowAngle);
            ArrowPos = VGet(ArrowX, 1.5f, ArrowZ);

            //�v���C���[�ƃn�[�g���߂��Ȃ���������\��
            float distance = sqrtf(powf(HeartPos.x - playerPos.x, 2) + powf(HeartPos.z - playerPos.z, 2));
            if (distance < 8.0f)    //������8.0f�ȉ����Ɣ�\��
            {
                isActiveArrow = false;  //��\��
            }
            else
            {
                isActiveArrow = true;   //�\��
            }
        }

        if (HeartSpawnTimer->hasElapsed())  //�X�|�[���܂ł̎��Ԃ��o��
        {
            IsActiveHeart = true;
            HeartSpawnTimer->reset();
        }

        if (HeartDisappearTimer->hasElapsed())  //������܂ł̎��Ԃ��o��
        {
            IsActiveHeart = false;
            HeartPos = GenerateRandomPosition();    //�n�[�g������������W���Đݒ�
            HeartDisappearTimer->reset();
        }

        //�n�[�g���Z�b�g
        MV1SetPosition(HeartHandle, HeartPos);
        MV1SetRotationXYZ(HeartHandle, HeartRotate);
        //�����Z�b�g
        MV1SetPosition(ArrowHandle, ArrowPos);
        MV1SetRotationXYZ(ArrowHandle, ArrowRotate);
    }
}

void Item::Draw()
{
    if (IsActiveHeart)
    {
        MV1DrawModel(HeartHandle);
        if (isActiveArrow)
        {
            MV1DrawModel(ArrowHandle);
        }
    }
}


VECTOR Item::GenerateRandomPosition()
{
    float x, z;

    // x, z �̐�Βl�̍��v�� 35 �ȉ��ƂȂ�悤�Ƀ����_���Ȓl�𐶐�
    do {
        x = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f �͈̔͂Ń����_����x���W
        z = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f �͈̔͂Ń����_����z���W
    } while (fabs(x) + fabs(z) > 35.0f);  // x��z�̐�Βl�̍��v��35�𒴂��Ȃ��悤�ɍĐ���

    // y���W�͏��2.0f
    float y = 2.0f;

    // VECTOR�ɍ��W���i�[���ĕԂ�
    return VGet(x, y, z);
}