#include"Enemy.h"

Enemy::Enemy(VECTOR groundPos)
{
	wave_Normal = MV1LoadModel("data/model/Wave/ShockWave_Normal_Ver2.1.mv1");
	wave_Slow = MV1LoadModel("data/model/Wave/ShockWave_Slow_Ver2.1.mv1");
	wave_High = MV1LoadModel("data/model/Wave/ShockWave_High_Ver2.1.mv1");
	enemyHandle = MV1LoadModel("data/model/Character/Skeleton_Mage.mv1");

	waveSpeed = 0.0015f;

	hitRadius = 5;
	//hitRadiusWidth = 1.5;
	hitRadiusWidth = 0.1;
	safeRadius = (waveScale.x * hitRadiusScale) - hitRadiusWidth;

	enemyPos = groundPos;
	enemyRotate = VGet(0, 0, 0);
	enemyScale = VGet(3, 3, 3);
	shadowPos = VGet(enemyPos.x, 0.1, enemyPos.z);
	shadowWidth = 1.5f;
	wavePos = VGet(enemyPos.x, 0.5, enemyPos.z);
	waveRotate = VGet(0, 0, 0);
	waveScale = VGet(0.01, 0.01, 0.01);

	runningEnemyNum = 1;
	enemyState = INACTIVE;
	lastState = INACTIVE;
	waveState = NORMAL;
	isAttack = false;
	waveRunning = false;
	isRunning = false;
	titleInit = false;
	waitTimer = new Timer(4000);	//�ҋ@���Ԃ�5000�~���b�ɐݒ�
	waveSE = LoadSoundMem("data/sound/wave_SE.mp3");
	ChangeVolumeSoundMem(255 * 40 / 100, waveSE);

	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);
	MV1SetScale(wave_Normal, waveScale);
}

Enemy::~Enemy()
{
	MV1DeleteModel(wave_Normal);
	MV1DeleteModel(enemyHandle);
}

void Enemy::Init(VECTOR groundPos)
{
	hitRadius = 5;
	//hitRadiusWidth = 1.5;
	hitRadiusWidth = 0.1;
	safeRadius = (waveScale.x * hitRadiusScale) - hitRadiusWidth-3.0;

	waveSpeed = 0.0015f;

	enemyPos = groundPos;
	enemyRotate = VGet(0, 0, 0);
	enemyScale = VGet(3, 3, 3);
	shadowPos = VGet(enemyPos.x, 0.1, enemyPos.z);
	shadowWidth = 1.5f;
	wavePos = VGet(enemyPos.x, 0.5, enemyPos.z);
	waveRotate = VGet(0, 0, 0);
	waveScale = VGet(0.01, 0.01, 0.01);

	runningEnemyNum = 1;
	enemyState = INACTIVE;
	lastState = INACTIVE;
	waveState = NORMAL;
	isAttack = false;
	waveRunning = false;
	isRunning = false;
	titleInit = false;

	wavePos = VGet(enemyPos.x, 0.5, enemyPos.z);
	waveRotate = VGet(0, 0, 0);
	waveScale = VGet(0.001, 0.01, 0.001);
	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);
	MV1SetScale(wave_Normal, waveScale);
}

void Enemy::Update(VECTOR playerPos,int score)
{
	if (isRunning)
	{
		if (enemyState == INACTIVE)
		{
			enemyState = AWAKEN;
			animTime = 0;
		}
		if (enemyState==AWAKEN&&animTime == totalAnimTime)
		{
			enemyState = IDLE;
		}
		//�N����
		if (enemyState == IDLE || enemyState == ATTACK)
		{
			calcRotateY(playerPos);
			AttackManager(score);
		}
	}
	else
	{
		enemyState = INACTIVE;
	}

	Animation();	// �A�j������

	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);

	MV1SetPosition(wave_Slow, wavePos);
	MV1SetRotationXYZ(wave_Slow, waveRotate);

	MV1SetPosition(wave_High, wavePos);
	MV1SetRotationXYZ(wave_High, waveRotate);


	switch (waveState)
	{
	case NORMAL:	
		MV1SetPosition(wave_Normal, wavePos);
		MV1SetRotationXYZ(wave_Normal, waveRotate);
		MV1SetScale(wave_Normal, waveScale);
		break;
	case SLOW:
		MV1SetPosition(wave_Slow, wavePos);
		MV1SetRotationXYZ(wave_Slow, waveRotate);
		MV1SetScale(wave_Slow, waveScale);
		break;
	case HIGH:
		MV1SetPosition(wave_High, wavePos);
		MV1SetRotationXYZ(wave_High, waveRotate);
		MV1SetScale(wave_High, waveScale);
		break;
	}


	lastState = enemyState;
}

void Enemy::Draw()
{
	shadowPos = VGet(enemyPos.x, 0.1, enemyPos.z);
	DrawCone3D(shadowPos, VGet(shadowPos.x, shadowPos.y - 0.3, shadowPos.z), shadowWidth, 5, 0xAA000000, 0xAA000000, TRUE);
	MV1DrawModel(enemyHandle);
	if (waveRunning)
	{
		switch (waveState)
		{
		case NORMAL:	MV1DrawModel(wave_Normal);	break;
		case SLOW:		MV1DrawModel(wave_Slow);	break;
		case HIGH:		MV1DrawModel(wave_High);	break;
		}
	}
}

void Enemy::DrawDebug()
{
	// �Ռ��g�̂����蔻��.
	DrawSphere3D(wavePos, (waveScale.x * hitRadiusScale), 5, 0x00ffff, 0x00ffff, false);//�O��
	DrawSphere3D(wavePos, (waveScale.x * hitRadiusScale) - hitRadiusWidth-3.0 , 5, 0x00ffff, 0x00ffff, false);//����
}

void Enemy::calcRotateY(VECTOR playerPos)
{
	float deltaX = playerPos.x - enemyPos.x;
	float deltaZ = playerPos.z - enemyPos.z;

	// atan2���g�p���āAY���̉�]�p�x�i���W�A���j���v�Z
	float angle = atan2(-deltaX, -deltaZ);

	// currentPos �� Y����]�icurrentRotate.y�j���^�[�Q�b�g�����̊p�x�ɐݒ�
	enemyRotate.y = angle;
}

void Enemy::AttackManager(int score)
{
	if (!isAttack)
	{
		wavePos = VGet(enemyPos.x, 0.5, enemyPos.z);
		if (!waitTimer->isActive())
		{
			//�ҋ@���Ԃ�4�`6�Ń����_���ɐݒ�
			switch (waveState)
			{
			case NORMAL:	waitTimer->setDuration((GetRand(2000) + 3000));		break;
			case SLOW:		waitTimer->setDuration((GetRand(2000) + 8000));		break;
			case HIGH:		waitTimer->setDuration((GetRand(2000) + 2000));		break;
			}
			waitTimer->start();	//�^�C�}�[�X�^�[�g

		}
		enemyState = ATTACK;	//�U�����ɕύX
		isAttack = true;
	}
	else
	{
		if (enemyState==ATTACK&&animTime == 18)
		{
			waveRunning = true;
			PlaySoundMem(waveSE, DX_PLAYTYPE_BACK);
		}
		if ((waveScale.x < maxWaveScale)&& waveRunning )//�Ռ��g��傫��
		{
			waveScale.x += waveSpeed;
			waveScale.z += waveSpeed;
			//hitRadiusWidth += 0.05;
			waveRotate.y += 0.03;
		}
		else if(waveScale.x > maxWaveScale)//���Z�b�g
		{
			StopSoundMem(waveSE);
			//hitRadiusWidth = 1.5;
			waveScale.x = 0.001;
			waveScale.z = 0.001;
			waveRotate.y = 0;
			waveRunning = false;

			//���̍U���I��
			int random = rand() % 100;

			if (score < 50) {
				waveState = NORMAL;  // �X�R�A��50�����Ȃ�m�[�}���U��
			}
			else if (score < 100) {
				waveState = (random <= 70) ? NORMAL : SLOW;  // �X�R�A��50�ȏ�100�����Ȃ�A70%�Ńm�[�}���A30%�ŃX���[�U��
			}
			else if (score >100) {
				if (random <= 70) {
					waveState = NORMAL;  // 70%�Ńm�[�}��
				}
				else if (random <= 85) {
					waveState = SLOW;    // 15%�ŃX���[
				}
				else {
					waveState = HIGH;    // 15%�Ńn�C
				}
			}

			switch (waveState)	//�g���x�ύX
			{
			case NORMAL:	waveSpeed = 0.0015;	break;
			case SLOW:		waveSpeed = 0.0005; break;
			case HIGH:		waveSpeed = 0.002;	break;
			}

		}

		if (animTime == totalAnimTime)
		{
			enemyState = IDLE;	//IDLE�ɕύX
		}

		if (waitTimer->hasElapsed())
		{
			isAttack = false;
		}
	}
}

void Enemy::Animation()
{
	animTimer++;
	animTime = animTimer;
	totalAnimTime = MV1GetAttachAnimTotalTime(enemyHandle, attachAnimIndex);
	// �A�j���[�V���������[�v������
	if (animTime > totalAnimTime) {
		animTimer = 0.0;  // �A�j���[�V�����̐i�s���Ԃ����Z�b�g
	}


	switch (enemyState)
	{
	case IDLE:		ChangeAnim(Enemy_IDLE);		break;
	case ATTACK:	ChangeAnim(Enemy_Attack);	break;
	case INACTIVE:	ChangeAnim(Enemy_Inective);	break;
	case AWAKEN:	ChangeAnim(Enemy_Awaken);	break;

	}

	// �Đ����Ԃ��Z�b�g����
	MV1SetAttachAnimTime(enemyHandle, attachAnimIndex, animTime);
}

void Enemy::ChangeAnim(int motionNum)
{
	MV1DetachAnim(enemyHandle, attachAnimIndex);

	if (lastState != enemyState)
	{
		animTimer = 0;
	}

	// �R�c���f���̂O�Ԗڂ̃A�j���[�V�������A�^�b�`����
	attachAnimIndex = MV1AttachAnim(enemyHandle, motionNum, -1, FALSE);
	// �A�^�b�`�����A�j���[�V�����̑��Đ����Ԃ��擾����
	totalAnimTime = MV1GetAttachAnimTotalTime(enemyHandle, attachAnimIndex);
}

void Enemy::Title_Update()
{
	if (!titleInit)
	{
		enemyState = IDLE;
		waitTimer->setDuration(3000);
		titleInit = true;
	}

	calcRotateY(VGet(0, 0, 0));

	if (!isAttack)
	{
		wavePos = VGet(enemyPos.x, 0.5, enemyPos.z);
		if (!waitTimer->isActive())
		{
			waitTimer->start();	//�^�C�}�[�X�^�[�g
		}
		enemyState = ATTACK;	//�U�����ɕύX
		isAttack = true;
	}
	else
	{
		if (enemyState == ATTACK && animTime == 18)
		{
			waveRunning = true;
		}
		if ((waveScale.x < 0.1f) && waveRunning)//�Ռ��g��傫��
		{
			waveScale.x += waveSpeed;
			waveScale.z += waveSpeed;
			waveRotate.y += 0.03;
		}
		else if (waveScale.x > 0.1f)//���Z�b�g
		{
			waveScale.x = 0.001;
			waveScale.z = 0.001;
			waveRotate.y = 0;
			waveRunning = false;
		}

		if (animTime == totalAnimTime)
		{
			enemyState = IDLE;	//IDLE�ɕύX
		}

		if (waitTimer->hasElapsed())
		{
			isAttack = false;
		}
	}
	Animation();	// �A�j������

	MV1SetPosition(enemyHandle, enemyPos);
	MV1SetRotationXYZ(enemyHandle, enemyRotate);
	MV1SetScale(enemyHandle, enemyScale);

	MV1SetPosition(wave_Normal, wavePos);
	MV1SetRotationXYZ(wave_Normal, waveRotate);
	MV1SetScale(wave_Normal, waveScale);

	lastState = enemyState;

}