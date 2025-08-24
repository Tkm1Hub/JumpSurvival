#include"HitCheck.h"

bool HitChecker::hitCheck(VECTOR playerPos, VECTOR enemyPos, float playerRadius, float hitRadius, float safeRadius, int& Score,bool&addScore,bool isHitting,VECTOR& effectPos)
{
	// 2�̃v���C���[�Ԃ̋������v�Z
	VECTOR diff = VSub(playerPos, enemyPos);
	float distance = VSize(diff);

	// ���a�̍��v
	float totalHitRadius = playerRadius + hitRadius - 0.1f;//�O��
	float totalSafeRadius = playerRadius + safeRadius - 0.1f;//����

	bool hitDamageRadius=false;		//�Ռ��g�ɓ���������
	bool hitSafeRadius=false;			//�Ռ��g�̓����ɓ�������


	//�Ռ��g�ɂ���������
	if (distance < totalHitRadius)
	{
		if (playerPos.y < shockWaveHeight)
		{
			hitDamageRadius = true;
		}
		else
		{
			hitDamageRadius = false;
		}
	}
	else
	{
		hitDamageRadius = false;
		addScore = false;
	}

	//���S�n�тɓ�������
	if (distance < totalSafeRadius)
	{
		hitSafeRadius = true;
	}
	else
	{
		hitSafeRadius = false;
	}

	//�v���C���[��Y��3�ȉ��ŁA�Ռ��g�ɂ�����A���S�n�тɂ͂����Ă��Ȃ��ꍇ
	if (hitDamageRadius && !hitSafeRadius)
	{

		return true;		//�_���[�W���󂯂�
	}
	else if (hitDamageRadius && hitSafeRadius)
	{
		if (!addScore&&!isHitting)
		{
			Score++;
			PlaySoundMem(scoreSE, DX_PLAYTYPE_BACK);
			effectPos = playerPos;

			effectPos.y += 5.0f;
			addScore = true;
		}
		return false;		//�_���[�W���󂯂Ȃ�
	}
	//�Ռ��g�ɂ����炸�A���S�n�тɂ͂����Ă��Ȃ��ꍇ
	else if ( !hitDamageRadius && !hitSafeRadius)
	{
		return false;
	}
	else if (!hitDamageRadius && hitSafeRadius)
	{
		return false;
	}
}


bool HitChecker::hitCheckItem(VECTOR playerPos, VECTOR itemPos, float playerRadius, float itemRadius)
{
	// 2�̃v���C���[�Ԃ̋������v�Z
	VECTOR diff = VSub(playerPos, itemPos);
	float distance = VSize(diff);

	// ���a�̍��v
	float totalRadius = playerRadius + itemRadius - 0.1f;//�O��

	if (distance < totalRadius)
	{
		return true;
	}
	else
	{
		return false;
	}
}