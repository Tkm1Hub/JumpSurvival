#pragma once
#include"DxLib.h"
#include"Timer.h"
#include"math.h"

const float RotateSpeed = 0.1f;  //��]���x

class Item
{
public:
    Item();
    ~Item();
    void Init();
    void Update(int playerHP,VECTOR playerPos);
    void Draw();

    VECTOR getHeartPos() { return HeartPos; }
    float getHeartRadius() { return HeartRadius; }
    bool& getIsActiveHeart() { return IsActiveHeart; }
    bool& getIsTakeHeart() { return IsTakeHeart; }
private:
    int HeartHandle;    //�n�[�g�̃��f���n���h��
    VECTOR HeartPos;    //�n�[�g�̍��W
    VECTOR HeartRotate; //�n�[�g�̉�]
    VECTOR HeartScale;  //�n�[�g�̃X�P�[��

    int ArrowHandle;    //���̃��f���n���h��
    VECTOR ArrowPos;    //���̍��W
    VECTOR ArrowRotate; //���̉�]
    VECTOR ArrowScale;  //���̃X�P�[��
    static constexpr float ArrowR = 5.0f;   //���̃v���C���[����̋���
    float ArrowAngle;   //���̌���
    bool isActiveArrow; //����\�����邩

    VECTOR GenerateRandomPosition();    //�����_�����W���擾

    Timer* HeartSpawnTimer; //�n�[�g���o������܂ł̊Ԋu
    Timer* HeartDisappearTimer;  //�n�[�g��������܂ł̊Ԋu

    bool IsActiveHeart;      //�n�[�g���o������
    bool IsTakeHeart;        //�n�[�g���擾������
    float HeartRadius;      //�A�C�e���̓����蔻��̔��a

};