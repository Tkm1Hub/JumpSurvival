#include"timer.h"

Timer::Timer(int durationMillis)
{
    startTime = 0;
    duration = durationMillis;
    isRunning = false;
}

//�^�C�}�[���J�n
void Timer::start()
{
    startTime = GetNowCount();
    isRunning = true;
}

//�^�C�}�[�����쒆���m�F
bool Timer::isActive()
{
    return isRunning;
}

//�^�C�}�[���w�莞�Ԍo�߂������m�F
bool Timer::hasElapsed()
{
    if (duration > 0)
    {
        int currentTime;
        currentTime = GetNowCount();
        if (isRunning && (currentTime - startTime >= duration)) {
            isRunning = false;  // �^�C�}�[���~
            return true;
        }
        return false;
    }
}
bool Timer::hasElapsedResetnt()
{
    int currentTime;
    currentTime = GetNowCount();
    if (isRunning && (currentTime - startTime >= duration)) {
        return true;
    }
    return false;
}

// �^�C�}�[�J�n����̌o�ߎ��Ԃ�b�P�ʂŎ擾
double Timer::getElapsedTime() const {
    if (!isRunning) {
        return 0.0;  // �^�C�}�[�����삵�Ă��Ȃ��ꍇ
    }
    int currentTime = GetNowCount();
    return (currentTime - startTime) / 1000.0;  // �o�ߎ��Ԃ�b�ɕϊ�
}

//�^�C�}�[�̃��Z�b�g
void Timer::reset()
{
    isRunning = false;
}

//�������Ԃ�ύX
void Timer::setDuration(int newDuration) {
    duration = newDuration;
}