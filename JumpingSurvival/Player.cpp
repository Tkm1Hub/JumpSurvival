#include"Player.h"

Player::Player()
{
	playerHandle = MV1LoadModel("data/model/Character/Rogue.mv1");
	JumpUI_Back = LoadGraph("data/picture/JumpUI_back.png");
	JumpUI = LoadGraph("data/picture/JumpUI.png");

	se_jump = LoadSoundMem("data/sound/se_jump_005.wav");
	se_damage = LoadSoundMem("data/sound/damage.mp3");

	playerState = STOP;
	lastState = STOP;
	playerPos = VGet(0, 0, 0);
	shadowPos = VGet(playerPos.x, 0.1, playerPos.z);
	playerWidth = 1.5f;
	shadowWidth = (-0.05 * playerPos.y) + 1.5f;
	playerVelocity = VGet(0, 0, 0);
	playerRotate = VGet(0, 0, 0);
	playerScale = VGet(3, 3, 3);
	HP = 3;
	Score = initialScore;
	JumpCombo = 0;
	JumpPower = 0.8f;
	isJumpCombo = false;
	isRunningTimer = false;
	alpha = 1.0f;
	hitXdiff = 0.5;
	isJumping = false;
	onGround = true;
	isTakeDamage = false;
	isHit = false;
	isHitting = false;
	isInvincible = false;
	isGameOver = false;
	isInit_Result = false;

	moveSpeed = 0.0001f;
	fallSpeed = 0.0f;

	invincibleTimer = new Timer(3000);
	LandingTimer = new Timer(500);
	alpha = 1.0;
	alphaTime = 0;
	padInputX = 0;
	padInputY = 0;
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	isInputB = false;


	animNum = MV1GetAnimNum(playerHandle);
	MV1DetachAnim(playerHandle, 0);
	animIndex = 1;
	attachAnimIndex = MV1AttachAnim(playerHandle, animIndex, -1, FALSE);
	animTimer = 0;
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);

	MV1SetScale(playerHandle, VGet(0.01, 0.01, 0.01));
}

Player::~Player()
{
	MV1DeleteModel(playerHandle);

}

void Player::Init()
{
	playerState = STOP;
	lastState = STOP;
	playerPos = VGet(0, 0, 0);
	playerWidth = 1.5f;
	shadowWidth = (-0.05 * playerPos.y) + 1.5f;
	shadowPos = VGet(playerPos.x, 0.1, playerPos.z);
	playerVelocity = VGet(0, 0, 0);
	playerRotate = VGet(0, 0, 0);
	playerScale = VGet(3, 3, 3);
	HP = 3;
	Score = initialScore;
	JumpCombo = 0;
	JumpPower = 0.8f;
	isJumpCombo = false;
	isRunningTimer = false;
	alpha = 1.0f;
	isJumping = false;
	onGround = true;
	isTakeDamage = false;
	isHit = false;
	isHitting = false;
	isInvincible = false;
	isGameOver = false;

	moveSpeed = 0.0001f;
	fallSpeed = 0.0f;

	invincibleTimer = new Timer(3000);
	alpha = 1.0;
	alphaTime = 0;
	padInputX = 0;
	padInputY = 0;
	padState = GetJoypadInputState(DX_INPUT_PAD1);
	isInputB = false;


	animNum = MV1GetAnimNum(playerHandle);
	MV1DetachAnim(playerHandle, 0);
	animIndex = 1;
	attachAnimIndex = MV1AttachAnim(playerHandle, animIndex, -1, FALSE);
	animTimer = 0;
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);

	MV1SetScale(playerHandle, VGet(0.01, 0.01, 0.01));
}

void Player::Update()
{
	//コントローラー入力
	GetJoypadAnalogInput(&padInputX, &padInputY, DX_INPUT_PAD1);
	padState = GetJoypadInputState(DX_INPUT_PAD1);

	playerVelocity = VGet(0, 0, 0);
	onGround = CheckOnGround();

	if (isHit)
	{
		if (!isHitting)
		{
			isTakeDamage = true;
		}
	}

	//ダメージを受けていなかったら操作可能
	if (!isTakeDamage)
	{
		if (HP > 0)
		{
			playerMove();	// 移動処理
			playerJump();	// ジャンプ処理
			playerAlpha();	// ダメージで透明切り替え
		}
	}
	else
	{
		takeDamage();
	}


	if (animTime == totalAnimTime&&playerState==DIE||playerPos.y<-100.0f)
	{
		isGameOver = true;
	}


	if (invincibleTimer->hasElapsed())
	{
		isHit = false;
		isHitting = false;
		isInvincible = false;
	}

	Animation();	// アニメ処理

	//座標、回転、サイズなどをセット

	playerPos = VAdd(playerPos, playerVelocity);
	MV1SetPosition(playerHandle, playerPos);
	MV1SetRotationXYZ(playerHandle, playerRotate);
	MV1SetScale(playerHandle, playerScale);
	MV1SetOpacityRate(playerHandle, alpha);

	//プレイヤーの状態を保存
	lastState = playerState;
}

void Player::Draw()
{
	//丸影の描画
	if (onGround)
	{
		shadowPos = VGet(playerPos.x, 0.1, playerPos.z);
		shadowWidth = (-0.1 * playerPos.y) + 1.5f;
		DrawCone3D(shadowPos, VGet(shadowPos.x, shadowPos.y - 0.3, shadowPos.z), shadowWidth, 5, 0xAA000000, 0xAA000000, TRUE);
	}

	MV1DrawModel(playerHandle);
	//DrawBox(700, 500 - 50, 700 + 20, 500 + 50, GetColor(255, 255, 255), TRUE);
	//JumpUImanager();

	//if (isJumpCombo)
	//{
	//	DrawBox(300, 300, 350, 350, GetColor(255, 0, 0), TRUE);
	//}
}

void Player::playerMove()
{

	if (!onGround && !isJumping)
	{
		fallSpeed -= Gravity;
	}

	if (playerPos.y > -0.5f)
	{

		//左スティックが入力されている場合
		if ((padInputX != 0 || padInputY != 0))
		{
			// Aボタンが押されている場合
			if ((padState & PAD_INPUT_A) != 0)
			{
				if (!isJumping)
				{
					if (onGround)
					{
						playerState = RUNNING;  // 走り
					}
					else
					{
						playerState = JUMP_IDLE;
					}
				}
				moveSpeed = 0.0004f;
			}
			else
			{
				if (!isJumping)
				{
					if (onGround)
					{
						playerState = WALK;  // 走り
					}
					else
					{
						playerState = JUMP_IDLE;
					}
				}
				moveSpeed = 0.0002f;
			}

			// 左スティックの入力に応じて移動量を追加
			playerVelocity.x += padInputX * moveSpeed;
			playerVelocity.z -= padInputY * moveSpeed;
			// スティックの入力があれば、キャラクターの向きを変更する
			playerRotate.y = atan2(padInputX * -1, padInputY); // 進行方向に回転
		}
		else
		{
			if (!isJumping)
			{
				if (onGround)
				{
					playerState = STOP;  // IDLE
				}
				else
				{
					playerState = JUMP_IDLE;
				}
			}
		}
	}
}



void Player::playerJump()
{
	//ジャンプ中のみ重力を適応
	if (isJumping)
	{
		fallSpeed -= Gravity;
	}

	//着地中にAボタンが押されたらジャンプ
	if (!isJumping)
	{
		if (!LandingTimer->isActive())
		{
			LandingTimer->start();	//着地後何秒経過したか計測
			isRunningTimer = true;
		}

		if (LandingTimer->hasElapsedResetnt())//連続ジャンプじゃない場合
		{
			isJumpCombo = false;
			JumpCombo = 0;		//連続ジャンプ回数リセット
			JumpPower = 0.8f;	//ジャンプ力リセット
		}
		else
		{
			isJumpCombo = true;
		}

		if (padState & PAD_INPUT_B)
		{
			LandingTimer->reset();
			if (isJumpCombo)//連続ジャンプの場合
			{
				JumpCombo++;
				if (JumpCombo > 3)
				{
					JumpPower -= 0.1f;
					if (JumpPower < 0.4f)
					{
						JumpPower = 0.4f;
					}
					if (JumpCombo > 7)
					{
						JumpCombo = 7;
					}
				}
			}


			fallSpeed += JumpPower;
			isJumping = true;
			playerState = JUMP_START;
			isInputB = true;
			isRunningTimer = false;
			PlaySoundMem(se_jump, DX_PLAYTYPE_BACK);
		}

	}


	if (playerState == JUMP_START && animTimer == totalAnimTime)
	{
		playerState = JUMP_IDLE;
	}

	//Y座標が０を下回ったら０に戻す
	if (playerPos.y < 0)
	{
		if (onGround)
		{
			fallSpeed = 0;
			playerPos.y = 0;
			animTimer = 0;
			playerState = JUMP_END;
			isJumping = false;

		}
	}

	//落下速度を移動量に加える
	playerVelocity.y += fallSpeed;
}

void Player::Animation()
{
	animTimer ++;
	animTime = animTimer;
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);
	// アニメーションをループさせる
	if (animTime > totalAnimTime) {
		animTimer = 0.0;  // アニメーションの進行時間をリセット
	}


	switch (playerState)
	{
	case STOP:			ChangeAnim(Anim_IDLE);			break;
	case WALK:			ChangeAnim(Anim_WALK);			break;
	case RUNNING:		ChangeAnim(Anim_RUNNING);		break;
	case JUMPING:		ChangeAnim(Anim_JUMPING);		break;
	case JUMP_START:	ChangeAnim(Anim_JUMP_START);	break;
	case JUMP_IDLE:		ChangeAnim(Anim_JUMP_IDLE);		break;
	case JUMP_END:		ChangeAnim(Anim_JUMP_END);		break;
	case DAMAGE:		ChangeAnim(Anim_DAMAGE);		break;
	case DIE:			ChangeAnim(Anim_DIE);			break;
	case RESULTS:		ChangeAnim(Anim_RESULTS);		break;

	}

	// 再生時間をセットする
	MV1SetAttachAnimTime(playerHandle, attachAnimIndex, animTime);
}

void Player::ChangeAnim(int motionNum)
{
	MV1DetachAnim(playerHandle,attachAnimIndex);

	if (lastState != playerState)
	{
		animTimer = 0;
	}

	// ３Ｄモデルの０番目のアニメーションをアタッチする
	attachAnimIndex = MV1AttachAnim(playerHandle, motionNum, -1, FALSE);
	// アタッチしたアニメーションの総再生時間を取得する
	totalAnimTime = MV1GetAttachAnimTotalTime(playerHandle, attachAnimIndex);
}

void Player::takeDamage()
{

	if (isTakeDamage)
	{
		fallSpeed -= Gravity;
		// プレイヤーの向きに対して反対方向へノックバックするため、回転角から移動量を計算
		playerVelocity.x += sin(playerRotate.y) * KnockBackPower;
		playerVelocity.z += cos(playerRotate.y) * KnockBackPower;
	}

	//ダメージを受けた場合
	if (isTakeDamage)
	{
		//ヒット中じゃなければ
		if (!isHitting)
		{
			playerState = DAMAGE;
			fallSpeed = 0;
			fallSpeed += JumpPower / 1.5;
			isHitting = true;
			HP--;
			PlaySoundMem(se_damage, DX_PLAYTYPE_BACK);
			if (!invincibleTimer->isActive())
			{
				invincibleTimer->start();
				isInvincible = true;
			}
		}
	}
	

	if (playerState == DAMAGE && animTime == totalAnimTime)
	{
		playerState = JUMP_IDLE;
	}

	if (playerPos.y < 0)
	{
		if (onGround)
		{
			fallSpeed = 0;
			playerPos.y = 0;
			isJumping = false;
			isTakeDamage = false;
		}

		if (HP <= 0)
		{
			playerState = DIE;
		}
		//isHitting = false;

		//if (!invincibleTimer->isActive())
		//{
		//	invincibleTimer->start();
		//	isInvincible = true;
		//}
	}


	//落下速度を移動量に加える
	playerVelocity.y += fallSpeed;
}

void Player::DrawDebug()
{
	// デバッグあたり判定.
	DrawSphere3D(playerPos, playerWidth, 5, 0x00ffff, 0x00ffff, false);

	if (CheckHitKey(KEY_INPUT_D))
	{
		isTakeDamage = true;
	}

}

void Player::playerAlpha()
{
	if (isHitting)
	{
		alphaTime++;
		if (alphaTime == 5)
		{
			alphaTime = 0;
			if (alpha == 0.0f)
			{
				alpha = 1.0;
			}
			else
			{
				alpha = 0.0;
			}
		}
	}
	else
	{
		alpha = 1.0;
	}
}

bool Player::CheckOnGround()
{
	float absX = fabs(playerPos.x);
	float absZ = fabs(playerPos.z);

	// ひし形の条件
	if (absX + absZ <= 40) {
		return true; // ひし形の内部
	}
	else
	{
		return false;
	}
}

void Player::Title(float waveScale)
{
	playerVelocity = VGet(0, 0, 0);
	playerRotate = VGet(0, -135 * DX_PI_F / 180.0f, 0);

	//ジャンプ中のみ重力を適応
	if (isJumping)
	{
		fallSpeed -= Gravity;
	}

	if (!isJumping)
	{
		playerState = STOP;
	}

	//着地中にAボタンが押されたらジャンプ
	if (!isJumping)
	{
		if (waveScale>=0.04f&&waveScale<=0.045f)
		{
			fallSpeed += JumpPower;
			isJumping = true;
			playerState = JUMP_START;
		}
	}

	if (playerState == JUMP_START && animTimer == totalAnimTime)
	{
		playerState = JUMP_IDLE;
	}

	//Y座標が０を下回ったら０に戻す
	if (playerPos.y < 0)
	{
		if (onGround)
		{
			fallSpeed = 0;
			playerPos.y = 0;
			animTimer = 0;
			playerState = JUMP_END;
			isJumping = false;
		}
	}

	//落下速度を移動量に加える
	playerVelocity.y += fallSpeed;


	Animation();	// アニメ処理

	//座標、回転、サイズなどをセット
	playerPos = VAdd(playerPos, playerVelocity);
	MV1SetPosition(playerHandle, playerPos);
	MV1SetRotationXYZ(playerHandle, playerRotate);
	MV1SetScale(playerHandle, playerScale);
	MV1SetOpacityRate(playerHandle, alpha);

	//プレイヤーの状態を保存
	lastState = playerState;

}

void Player::Results_Update()
{
	if (!isInit_Result)
	{
		playerPos = VGet(-5, -3, 0);
		playerRotate = VGet(0, -25 * DX_PI_F / 180.0f, 0);
		playerState = RESULTS;
	}


	MV1SetPosition(playerHandle, playerPos);
	MV1SetRotationXYZ(playerHandle, playerRotate);
	Animation();	// アニメ処理

	//プレイヤーの状態を保存
	lastState = playerState;
}

void Player::DrawHit()
{
	MV1SetPosition(playerHandle, VGet(playerPos.x + hitXdiff, playerPos.y, playerPos.z));
	MV1DrawModel(playerHandle);

	if (hitstop<0)
	{
		if (hitXdiff < 0)
		{
			hitXdiff += 0.7;
		}
		else if(hitXdiff<0.5)
		{
			hitXdiff = 0.5;
		}
		else
		{
			hitXdiff *= -1.0;
		}
	}
	hitstop -= 1;
}

void Player::JumpUImanager()
{

	int red = GetColor(255, 0, 0);
	int green = GetColor(0, 255, 0);
	int yellow = GetColor(255, 255, 0);
	int Color;

	y1 = yTop + 20 * JumpCombo;

		if (y1 <= 490)
		{
			Color = green;
		}
		else if (y1 >= 491 && y1 <= 530)
		{
			Color = yellow;
		}
		else if (y1 >= 531)
		{
			Color = red;
		}

	DrawGraph(0, 0, JumpUI_Back, TRUE);
	DrawBox(x1, y1, x2, y2, Color, TRUE);
	DrawGraph(0, 0, JumpUI, TRUE);

}