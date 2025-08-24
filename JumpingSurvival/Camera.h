#pragma once
#include"DxLib.h"
#include"Player.h"
class Camera
{
public:

	Camera(VECTOR playerPos);
	void Init(VECTOR playerPos);
	void Update(VECTOR playerPos);
	void DebugMode();
	void Title_Update();
	void Results_Update();


	VECTOR getCameraPos() const {
		return cameraPosDiff; // ���݂̃J�����ʒu��Ԃ��i��Ƃ��ČŒ�j
	}
private:
	VECTOR cameraPosDiff;
	VECTOR cameraPos;
	VECTOR targetPos;

};