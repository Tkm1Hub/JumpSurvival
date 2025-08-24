#include "Game.h"

// �R���X�g���N�^
Game::Game()
    : currentState(TITLE), isDebugMode(false), inputF1(false) , frameCount(0), StartTimeCount(GetNowCount()),
    ply1(VGet(-24,0,0)),camera(player.getPlayerPos())// ������Ԃ̓^�C�g�����
{
    // �K�v�ȏ������������s��
    Title_Start = LoadGraph("data/picture/Title_Start.png");
    Title_Start_Decision = LoadGraph("data/picture/Title_Start_Decision.png");
    Title_Tutorial = LoadGraph("data/picture/Title_Tutorial.png");
    Title_Tutorial_Decision = LoadGraph("data/picture/Title_Tutorial_Decision.png");
    Score_UI = LoadGraph("data/picture/Score_UI.png");
    Score_Effect = LoadGraph("data/picture/+1.png");
    HP_UI = LoadGraph("data/picture/HP.png");
    HighScore_UI = LoadGraph("data/picture/HighScore_UI.png");
    GameOver_Die = LoadGraph("data/picture/GameOver_Die.png");
    GameOver_Fall = LoadGraph("data/picture/GameOver_Fall.png");
    Results_UI = LoadGraph("data/picture/results.png");
    Results_backGround = LoadGraph("data/picture/results_background.png");
    Tutorial_mov = LoadGraph("data/picture/Tutorial_2.mp4");
    Tutorial_Decision = LoadGraph("data/picture/Tutorial_decision.png");

    CountDown_3 = LoadGraph("data/picture/CountDown_3.png");
    CountDown_2 = LoadGraph("data/picture/CountDown_2.png");
    CountDown_1 = LoadGraph("data/picture/CountDown_1.png");
    CountDown_Start = LoadGraph("data/picture/CountDown_Start.png");

    titleSound = LoadSoundMem("data/sound/title.mp3");
    tutorialSound = LoadSoundMem("data/sound/tutorial.mp3");
    gameSound = LoadSoundMem("data/sound/playing.wav");
    resultSound = LoadSoundMem("data/sound/result.mp3");

    titleSE_choose = LoadSoundMem("data/sound/choose.mp3");
    titleSE_push = LoadSoundMem("data/sound/decision.mp3");
    gameSE_heart = LoadSoundMem("data/sound/heart.mp3");
    gameSE_gameover = LoadSoundMem("data/sound/gameover.mp3");
    gameSE_score = LoadSoundMem("data/sound/score.mp3");


    Title_isStart = false;
    Title_isInputBottun = false;
    isInputUpDown = false;
    isInit = false;
    isDrawingScoreEffect = false;
    changeState = false;
    isStartGame = false;


    LPCSTR ScoreFontpath = "ZenMaruGothic-Black.ttf";
    ChangeFont("Zen Maru Gothic Black", DX_CHARSET_DEFAULT);
    FontHandle = CreateFontToHandle("Zen Maru Gothic Black", 70, 4, DX_FONTTYPE_EDGE);

    fps = 0;
    joyInputX = 0;
    joyInputY = 0;
    saveTime = 0;
    ChangeStateBoxAlpha = 0;
    enemyNum = 1;
    currentTime = 0;
    ObjCode = 0;
    inputA = false;
    hitStop = false;
    img_Y = -700.0f;
    HighScore = rand() % 30 + 90;

    for (int i = 0; i < maxGroundNum; i++)
    {
        enemy[i] = new Enemy(background.getS_groundPos(i));
        addScore[i] = false;
        effectPos[i] = VGet(0, 0, 0);
    }

    TitleEnemy = new Enemy(background.getTitle_groundPos());
    enemyAwakenTime = new Timer(7000);  //7000�~���b���ƂɓG���N����
    hitStopTimer = new Timer(200);      //�_���[�W��300�~���b�̃q�b�g�X�g�b�v
    waitChangeTimer = new Timer(1000);  //�V�[���؂�ւ��ҋ@����(1�b)
    gameOverWaitTimer = new Timer(2000);
    gameCountUpTimer = new Timer(100000);
    startTimer = new Timer(4000);
}

// �X�V����
void Game::Update()
{
    int frameStartTime = GetNowCount(); // �t���[���̊J�n�������擾
    padState = GetJoypadInputState(DX_INPUT_PAD1);

    // �f�o�b�O���[�h�̐؂�ւ�
    ToggleDebugMode();
    if (isDebugMode)
    {
        SelectDebugObject();

        switch (ObjCode)
        {
        case 0:
            // �I�u�W�F�N�g�̈ړ������i�f�o�b�O���[�h���̂݁j
            DebugMove(player.getPlayerPos(), player.getPlayerRotate(), player.getPlayerScale());
            break;
        case 1:
            DebugMove(background.getGroundPos(), background.getGroundRotate(), background.getGroundScale());
            break;
        case 2:
            DebugMove(enemy[0]->getEnemyPos(), enemy[0]->getEnemyRotate(), enemy[0]->getEnemyScale());
            break;
        case 3:
            camera.DebugMode();
            break;
        }
    }


    switch (currentState)
    {
    case TITLE:
        UpdateTitle();
        break;
    case TUTORIAL:
        UpdateTutorial();
        break;
    case PLAYING:
        UpdatePlaying();
        break;
    case RESULT:
        UpdateResult();
        break;
    }

    
    // FPS��60�ɌŒ肷�邽�߂̑ҋ@����
    int frameEndTime = GetNowCount(); // �t���[���̏I���������擾
    int elapsedTime = frameEndTime - frameStartTime; // �o�ߎ��Ԃ��v�Z

    int waitTime = 1000 / 60 - elapsedTime; // 1�t���[��������̎��Ԃ���o�ߎ��Ԃ�����
    if (waitTime > 0) {
        WaitTimer(waitTime); // �c��̎��Ԃ����ҋ@
    }
}

// �`�揈��
void Game::Draw()
{
    switch (currentState)
    {
    case TITLE:
        DrawTitle();
        break;
    case TUTORIAL:
        DrawTutorial();
        break;
    case PLAYING:
        DrawPlaying();
        break;
    case RESULT:
        DrawResult();
        break;
    }

    // �f�o�b�O���̕`��
    if (isDebugMode) {
        DisplayDebugInfo();
        DisplayFPS();
    }

    // �����x��ݒ�i��������255�����S�ɕs�����A0�����S�ɓ����j
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, ChangeStateBoxAlpha);  // ������

    // �������ȐԂ��{�b�N�X��`�� (x1, y1, x2, y2, �F)
    DrawBox(0, 0, 1600, 900, GetColor(255, 255, 255), TRUE);

    // �`�惂�[�h�����ɖ߂��i�ʏ탂�[�h�ɖ߂��j
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

// �^�C�g����ʂ̍X�V����
void Game::UpdateTitle()
{
    GetJoypadAnalogInput(&joyInputX, &joyInputY, DX_INPUT_PAD1);
    fadeOut();
    if (!isPlayingSound)
    {
        PlaySoundMem(titleSound, DX_PLAYTYPE_LOOP);
        isPlayingSound = true;
    }


    if (!isInit)
    {
        enemyNum = 1;
        enemyAwakenTime->reset();
        camera.Init(player.getPlayerPos());
        background.Init();
        player.Init();
        item.Init();
        Title_isInputBottun = false;   //����{�^���������ꂽ��
        isStartGame = false;
        for (int i = 0; i < maxGroundNum; i++)
        {
            enemy[i]->Init(background.getS_groundPos(i));
        }
        isInit = true;
        CountDownNum = 3;
        isCountDown = false;
        isStartGame = false;
    }

    camera.Title_Update();
    background.Title_Update();
    player.Title(TitleEnemy->getWaveScale());
    TitleEnemy->Title_Update();

    if (!Title_isInputBottun)
    {
        if (joyInputY!=0) //�㉺�{�^�������ꂽ��
        {
            if (!isInputUpDown)
            {
                if(joyInputY>0)
                {
                    Title_isStart = true; //�I��؂�ւ�
                    PlaySoundMem(titleSE_choose, DX_PLAYTYPE_BACK);
                }
                else
                {
                    Title_isStart = false; //�I��؂�ւ�
                    PlaySoundMem(titleSE_choose, DX_PLAYTYPE_BACK);
                }
            }
            isInputUpDown = true;
        }
        else
        {
            isInputUpDown = false;
        }
    }

    if ((padState&PAD_INPUT_A)!=0)
    {
        Title_isInputBottun = true;   //����{�^���������ꂽ��
        PlaySoundMem(titleSE_push, DX_PLAYTYPE_BACK);
        if (!inputA)
        {
            changeState = true;
            isInit = false;
        }
        inputA = true;
    }
    else
    {
        inputA = false;
    }

    if (Title_isStart)
    {
        changeGameState(PLAYING,titleSound);//�Q�[���J�n
    }
    else
    {
        changeGameState(TUTORIAL,titleSound);//�`���[�g���A���Đ�
    }
}

//�`���[�g���A����ʂ̍X�V����
void Game:: UpdateTutorial()
{
    fadeOut();
    if (!isPlayingSound)
    {
        PlaySoundMem(tutorialSound, DX_PLAYTYPE_LOOP);
        isPlayingSound = true;
    }

    if (padState&PAD_INPUT_A||padState&PAD_INPUT_B)
    {
        Title_isInputBottun = true;   //����{�^���������ꂽ��
        PlaySoundMem(titleSE_push, DX_PLAYTYPE_BACK);
        if (!inputA)
        {
            changeState = true;
            inputA = true;
        }
    }
    else
    {
        inputA = false;
    }

    changeGameState(TITLE,tutorialSound);//�����Ƃ�ɂ��ǂ�
}

// �Q�[�����̍X�V����
void Game::UpdatePlaying()
{
    fadeOut();
    if (!isPlayingSound)
    {
        PlaySoundMem(gameSound, DX_PLAYTYPE_LOOP);
        isPlayingSound = true;
    }


    if (!player.getIsGameOver())
    {
            if (!gameCountUpTimer->isActive())
            {
                gameCountUpTimer->start();
            }
            if (player.getPlayerPos().y > -0.5)
            {
                HitStopManager();
            }
            camera.Update(player.getPlayerPos());
            background.Update();
            item.Update(player.getPlayerHP(), player.getPlayerPos());
            if (!hitStop)
            {
                player.Update();
            }

            if (!enemy[0]->setIsRunning())
            {
                enemy[0]->setIsRunning() = true;
            }
            enemyAwaken();

            if (CheckHitKey(KEY_INPUT_F3))
            {
                if (!inputF3)
                {
                    enemyStop = !enemyStop;
                }
                inputF3 = true;
            }
            else
            {
                inputF3 = false;
            }

            if (!enemyStop && !hitStop)
            {
                for (int i = 0; i < maxGroundNum; i++)
                {
                    enemy[i]->Update(player.getPlayerPos(), player.setScore());
                }
            }

            if (player.getPlayerPos().y >= -0.5)
            {
                for (int i = 0; i < maxGroundNum; i++)
                {
                    if (!player.setIsHit())
                    {
                        player.setIsHit() = hitChecker.hitCheck(player.getPlayerPos(), enemy[i]->getEnemyPos(), player.getHitRadius(), enemy[i]->getHitRadius(), enemy[i]->getSafeRadius(), player.setScore(), addScore[i], player.getIsHitting(), effectPos[i]);
                    }
                }
            }

            if (item.getIsActiveHeart())
            {
                if (!item.getIsTakeHeart())
                {
                    if (hitChecker.hitCheckItem(player.getPlayerPos(), item.getHeartPos(), player.getHitRadius(), item.getHeartRadius()))
                    {
                        player.getPlayerHP()++;
                        PlaySoundMem(gameSE_heart, DX_PLAYTYPE_BACK);
                        item.getIsActiveHeart() = false;
                        item.getIsTakeHeart() = true;
                    }
                }
            }
            else
            {
                item.getIsTakeHeart() = false;
            }
        



        if (isDebugMode)
        {
            if (CheckHitKey(KEY_INPUT_RETURN))
            {
                currentState = TITLE;       //�f�o�b�O���ɃG���^�[�Ń^�C�g��
            }
        }
    }

    // �v���C���I�������烊�U���g��ʂɑJ�ڂ��鏈��
    else
    {

        if (!gameOverWaitTimer->isActive() && a == false)
        {
            gameOverWaitTimer->start();
            a = true;
            PlaySoundMem(gameSE_gameover, DX_PLAYTYPE_BACK);
            saveTime = gameCountUpTimer->getElapsedTime();
            gameCountUpTimer->reset();
        }

        if (gameOverWaitTimer->hasElapsed())
        {
            changeState = true;
            StopSoundMem(gameSound);


        }
        changeGameState(RESULT, gameSound);
    }
}

// ���U���g��ʂ̍X�V����
void Game::UpdateResult()
{
    fadeOut();
    if (!isPlayingSound)
    {
        PlaySoundMem(resultSound, DX_PLAYTYPE_LOOP);
        isPlayingSound = true;
        if (HighScore < player.setScore())
        {
            HighScore = player.setScore();
        }
    }

    a = false;

    padState = GetJoypadInputState(DX_INPUT_PAD1);

    player.Results_Update();
    camera.Results_Update();
    //if (!isInit)
    //{
    //    enemyNum = 1;
    //    enemyAwakenTime->reset();
    //    player.Init();
    //    camera.Init(player.getPlayerPos());
    //    background.Init();
    //    for (int i = 0; i < maxGroundNum; i++)
    //    {
    //        enemy[i]->Init(background.getS_groundPos(i));
    //    }

    //    isInit = true;
    //}



    if ((padState & PAD_INPUT_A) != 0)
    {
        if (!inputA)
        {
            changeState = true;
        }
        inputA = true;
        isInit = false;
    }
    else
    {
        inputA = false;
    }
    changeGameState(TITLE,resultSound);
}

// �^�C�g����ʂ̕`�揈��
void Game::DrawTitle()
{
    background.Title_Draw();
    player.Draw();
    TitleEnemy->Draw();
    if (Title_isStart)
    {
        DrawGraph(0, 0, Title_Start, TRUE);
        if (Title_isInputBottun)
        {
            DrawGraph(0, 0, Title_Start_Decision, TRUE);
        }
    }
    else
    {
        DrawGraph(0, 0, Title_Tutorial, TRUE);
        if (Title_isInputBottun)
        {
            DrawGraph(0, 0, Title_Tutorial_Decision, TRUE);
        }
    }
}

//�`���[�g���A����ʂ̕`��
void Game::DrawTutorial()
{
    //int movieState= GetMovieStateToGraph(Tutorial_mov);
    //if (movieState == 0)
    //{
    DrawExtendGraph(0,0,1600,900,Tutorial_mov, FALSE);

    if (GetMovieStateToGraph(Tutorial_mov)==0)
    {
        PlayMovieToGraph(Tutorial_mov);
        SeekMovieToGraph(Tutorial_mov, 0);
        if (Title_isInputBottun)
        {
            DrawGraph(0, 0, Tutorial_Decision, TRUE);
        }
    }



}

// �Q�[�����̕`�揈��
void Game::DrawPlaying()
{
    background.Draw();
    for (int i = 0; i < maxGroundNum; i++)
    {
        enemy[i]->Draw();
    }
    //enemy2->Draw();
    if (!hitStop)
    {
        player.Draw();
    }
    else
    {
        player.DrawHit();
    }

    item.Draw();



    //if (!isStartGame)
    //{
    //    CountDown();
    //}

    if (isDebugMode)
    {
        player.DrawDebug();
        for (int i = 0; i < maxGroundNum; i++)
        {
            enemy[i]->DrawDebug();
        }
    }
    for (int i = 0; i < maxGroundNum; i++)
    {
        isDrawingScoreEffect = addScore[i];
        if (addScore[i])
        {
            DrawScoreEffect(i);
        }
    }

    if (!player.getIsGameOver())
    {
        DrawGraph(0, 0, Score_UI, TRUE);
        DrawGraph(0, 0, HighScore_UI, TRUE);
        player.JumpUImanager();
        DrawFormatStringToHandle(1300, 700, GetColor(255, 255, 255), FontHandle, "%d", player.setScore());
        DrawFormatStringToHandle(1400, 0, GetColor(255, 255, 255), FontHandle, "%d", HighScore);
        DrawHP(player.getPlayerHP());
        img_Y = -700;
    }
    else
    {
        if (player.getPlayerPos().y > -0.5f)
        {
            if (img_Y >= 0)
            {
                img_Y = 0;
            }
            else
            {
                img_Y += 100;
            }
            DrawGraph(0, img_Y, GameOver_Die, TRUE);
        }
        else
        {
            if (img_Y >= 0)
            {
                img_Y = 0;
            }
            else
            {
                img_Y += 100;
            }
            DrawGraph(0, img_Y, GameOver_Fall, TRUE);
        }
    }

}

// ���U���g��ʂ̕`�揈��
void Game::DrawResult()
{
    //background.Draw();
    DrawGraph(0, 0, Results_backGround, TRUE);
    player.Draw();
    DrawGraph(0, 0,Results_UI, TRUE);
    int saveM = saveTime / 60;
    int saveS = saveTime % 60;

    DrawFormatStringToHandle(1200, 426, GetColor(255, 255, 255), FontHandle, "%d", player.setScore());
    //DrawFormatStringToHandle(1200, 290, GetColor(255, 255, 255), FontHandle, "%d : %d", saveM,saveS);
    if (saveS < 10 && saveM < 10)
    {
        DrawFormatStringToHandle(1200, 295, GetColor(255, 255, 255), FontHandle, "0%d : 0%d", saveM, saveS);
    }
    else if (saveM < 10)
    {
        DrawFormatStringToHandle(1200, 295, GetColor(255, 255, 255), FontHandle, "0%d : %d", saveM, saveS);
    }
    else if(saveS<10)
    {
        DrawFormatStringToHandle(1200, 295, GetColor(255, 255, 255), FontHandle, "%d : 0%d", saveM, saveS);
    }
    else
    {
        DrawFormatStringToHandle(1200, 295, GetColor(255, 255, 255), FontHandle, "%d : %d", saveM, saveS);
    }

}

// �f�o�b�O���[�h�̃I��/�I�t�؂�ւ�
void Game::ToggleDebugMode()
{
    if (CheckHitKey(KEY_INPUT_F1))
    {
        if (!inputF1)
        {
            isDebugMode = !isDebugMode;
        }
        inputF1 = true;
    }
    else
    {
        inputF1 = false;
    }
}

void Game::SelectDebugObject()
{
    if (CheckHitKey(KEY_INPUT_F2))
    {
        if (!inputF2)
        {
            ObjCode++;
            if (ObjCode > 3)
            {
                ObjCode = 0;
            }
            inputF2 = true;
        }
    }
    else
    {
        inputF2 = false;
    }
}

// �f�o�b�O���̕\��
void Game::DisplayDebugInfo()
{
    //box
    // �`�惂�[�h��ݒ�
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(5, 5, 450, 600, GetColor(0, 0, 0), TRUE);
    // �u�����h���[�h�����ɖ߂�
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    // ���W����\��
    DrawFormatString(10, 70, GetColor(255, 255, 255), "Camera Pos: (%f, %f, %f)", camera.getCameraPos().x, camera.getCameraPos().y, camera.getCameraPos().z);
    DrawFormatString(10, 90, GetColor(255, 255, 255), "PlayerPos: (%f,%f,%f)", player.getPlayerPos().x, player.getPlayerPos().y, player.getPlayerPos().z);
    DrawFormatString(10, 110, GetColor(255, 255, 255), "PlayerRotate: (%f,%f,%f)", player.getPlayerRotate().x, player.getPlayerRotate().y, player.getPlayerRotate().z);
    DrawFormatString(10, 130, GetColor(255, 255, 255), "PlayerScale: (%f,%f,%f)", player.getPlayerScale().x, player.getPlayerScale().y, player.getPlayerScale().z);
    DrawFormatString(10, 150, GetColor(255, 255, 255), "animTime : %f", player.getAnimTime());
    DrawFormatString(10, 170, GetColor(255, 255, 255), "groundPos: (%f,%f,%f)", background.getGroundPos().x, background.getGroundPos().y, background.getGroundPos().z);
    DrawFormatString(10, 190, GetColor(255, 255, 255), "groundRotate: (%f,%f,%f)", background.getGroundRotate().x, background.getGroundRotate().y, background.getGroundRotate().z);
    DrawFormatString(10, 210, GetColor(255, 255, 255), "groundScale: (%f,%f,%f)", background.getGroundScale().x, background.getGroundScale().y, background.getGroundScale().z);
    DrawFormatString(10, 230, GetColor(255, 255, 255), "EnemyPos: (%f,%f,%f)", enemy[0]->getEnemyPos().x, enemy[0]->getEnemyPos().y, enemy[0]->getEnemyPos().z);
    DrawFormatString(10, 250, GetColor(255, 255, 255), "EnemyRotate: (%f,%f,%f)", enemy[0]->getEnemyRotate().x, enemy[0]->getEnemyRotate().y, enemy[0]->getEnemyRotate().z);
    DrawFormatString(10, 270, GetColor(255, 255, 255), "EnemyScale: (%f,%f,%f)", enemy[0]->getEnemyScale().x, enemy[0]->getEnemyScale().y, enemy[0]->getEnemyScale().z);
    DrawFormatString(10, 290, GetColor(255, 0, 0), "Score : %d", player.setScore());
    DrawFormatString(10, 410, GetColor(255, 255, 255), "enemyAwakenTimer : %f", enemyAwakenTime->getElapsedTime());


    DrawFormatString(10, 310, GetColor(255, 0, 0), "Selected Object : 0%d", ObjCode);
    DrawString(10, 330, "LShift:Y  S:size  R:Rotate", GetColor(255, 0, 0));
    if (player.setIsTakeDamage())
    {
        DrawString(10, 350, "isTakeDamage : true", GetColor(255, 0, 0));
    }
    else
    {
        DrawString(10, 350, "isTakeDamage : false", GetColor(255, 0, 0));
    }
    if (player.getIsHitting())
    {
        DrawString(10, 370, "isHitting : true", GetColor(255, 0, 0));
    }
    else
    {
        DrawString(10, 370, "isHittng : false", GetColor(255, 0, 0));
    }

}

void Game::DebugMove(VECTOR& pos, VECTOR& rotate, VECTOR& scale)
{
    // S�L�[�����Ȃ���㉺���ő傫���ύX
    if (CheckHitKey(KEY_INPUT_S))
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            scale.x += 0.1;
            scale.y += 0.1;
            scale.z += 0.1;
        }
        else if (CheckHitKey(KEY_INPUT_DOWN))
        {
            scale.x -= 0.1;
            scale.y -= 0.1;
            scale.z -= 0.1;
        }
        return; // �X�P�[���ύX���D�悳�ꂽ�ꍇ�A���̏��������s���Ȃ�
    }

    // R�L�[�����Ȃ��獶�E����Y����]�ύX
    if (CheckHitKey(KEY_INPUT_R))
    {
        if (CheckHitKey(KEY_INPUT_RIGHT))
        {
            rotate.y += 0.1;
        }
        else if (CheckHitKey(KEY_INPUT_LEFT))
        {
            rotate.y -= 0.1;
        }
        return; // ��]�ύX���D�悳�ꂽ�ꍇ�A���̏��������s���Ȃ�
    }

    // ���V�t�g�L�[�����Ȃ���㉺����Y���W�ύX
    if (CheckHitKey(KEY_INPUT_LSHIFT))
    {
        if (CheckHitKey(KEY_INPUT_UP))
        {
            pos.y++;
        }
        else if (CheckHitKey(KEY_INPUT_DOWN))
        {
            pos.y--;
        }
        return; // Y���̍��W�ύX���D�悳�ꂽ�ꍇ�A���̏��������s���Ȃ�
    }

    // ���W�ύX�i�ʏ�̈ړ��j
    if (CheckHitKey(KEY_INPUT_UP))
    {
        pos.z++;
    }
    else if (CheckHitKey(KEY_INPUT_DOWN))
    {
        pos.z--;
    }
    else if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        pos.x++;
    }
    else if (CheckHitKey(KEY_INPUT_LEFT))
    {
        pos.x--;
    }
}

// FPS�̕\��
void Game::DisplayFPS()
{
    frameCount++;
    if (frameCount % 60 == 0) {
        currentTime = GetNowCount();
        fps = 60000 / (currentTime - StartTimeCount);  // FPS���v�Z
        StartTimeCount = currentTime;
    }
    DrawFormatString(10, 10, GetColor(255, 255, 255), "FPS: %d", fps);
}

void Game::enemyAwaken()
{
    if (enemyNum < maxGroundNum)
    {
        if (!enemyAwakenTime->isActive())
        {
            enemyAwakenTime->start();
        }

        if (enemyAwakenTime->hasElapsed())
        {
            enemy[enemyNum]->setIsRunning() = true;
            enemyNum++;
        }
    }
}

void Game::HitStopManager()
{
    for (int i = 0; i < maxGroundNum; i++)
    {
        if (hitChecker.hitCheck(player.getPlayerPos(), enemy[i]->getEnemyPos(), player.getHitRadius(), enemy[i]->getHitRadius(), enemy[i]->getSafeRadius(),player.setScore(),addScore[i],player.getIsHitting(),effectPos[i]))
        {
            if (!player.getIsInvincible())
            {
                hitStop = true;
                if (!hitStopTimer->isActive())
                {
                    hitStopTimer->start();
                }
            }
        }
    }

    if (hitStopTimer->hasElapsed())
    {
        hitStop = false;
    }
}

void Game::DrawScoreEffect(int i)
{
    if (addScore[i - 1])
    {
        effectPos[i].y = effectPos[i-1].y + 0.3f;
    }
    DrawBillboard3D(effectPos[i], 0.5f, 0.5f, 5.0f, 0.0f, Score_Effect, TRUE);
    effectPos[i].y += 0.1f;
}

void Game::DrawHP(int HP)
{
    for (int i = 0; i < HP; i++)
    {
        int x1 = 10 + (100 * i);
        int y1 = 5;
        int x2 = x1 + 100;
        int y2 = y1 + 90;

        //DrawGraph(20 + (150 * i), 20, HP_UI, TRUE);
        DrawExtendGraph(x1, y1, x2, y2, HP_UI, TRUE);
    }
}

void Game::changeGameState(GameState gameState,int stopSound)
{
    if (changeState)
    {
        if (!waitChangeTimer->isActive())
        {
            waitChangeTimer->start();
        }

        ChangeStateBoxAlpha += 10;
        if (ChangeStateBoxAlpha > 255)
        {
            ChangeStateBoxAlpha = 255;
        }

        if (waitChangeTimer->hasElapsed())
        {
            currentState = gameState;
            changeState = false;
            Title_isInputBottun = false;   //����{�^���������ꂽ��
            StopSoundMem(stopSound);
            isPlayingSound = false;
        }
    }
}
void Game::fadeOut()
{
    if (!waitChangeTimer->isActive() && ChangeStateBoxAlpha > 0)
    {
        ChangeStateBoxAlpha -= 10;
        if (ChangeStateBoxAlpha < 0)
        {
            ChangeStateBoxAlpha = 0;
        }
    }
}

void Game::CountDown()
{
    if (!isCountDown)
    {
        CountDownTime = GetNowCount(); // �J�E���g�_�E���J�n����
        isCountDown = true;
    }

    while (CountDownNum >= 0)
    {
        // �J�E���g�_�E���̎��Ԃ��X�V
        if (GetNowCount() - CountDownTime >= 1000) // 1�b�o�߂�����J�E���g�_�E�������炷
        {
            CountDownTime = GetNowCount(); // �V�����J�n���Ԃ�ݒ�
            CountDownNum--; // �J�E���g�_�E����1�b���炷
        }
        // 0�b�ɂȂ�����X�^�[�g���b�Z�[�W��\��
        if (CountDownNum == 0)
        {
            DrawGraph(0, 0, CountDown_Start, TRUE);
        }
        else if (CountDownNum == 1)
        {
            DrawGraph(0, 0, CountDown_1, TRUE);
        }
        else if (CountDownNum == 2)
        {
            DrawGraph(0, 0, CountDown_2, TRUE);
        }
        else if (CountDownNum == 3)
        {
            DrawGraph(0, 0, CountDown_3, TRUE);
        }

    }
        if (CountDownNum < 0)
        {
            isStartGame = true;
        }
}