#pragma once
#include"DxLib.h"

class Timer
{
public:
    Timer(int durationMillis);
    void start();
    bool isActive();
    bool hasElapsed();
    bool hasElapsedResetnt();
    void reset();
    // �^�C�}�[�J�n����̌o�ߎ��Ԃ�b�P�ʂŎ擾����
    double getElapsedTime() const;
    // �^�C�}�[�̎������Ԃ�ݒ肷��
    void setDuration(int newDuration);

private:
    bool isRunning; //�^�C�}�[���쓮����
    int duration;  //�^�C�}�[�̎�������
    int startTime;  //�^�C�}�[�̊J�n����
};