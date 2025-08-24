#pragma once
#include "DxLib.h"
#include "Fishing.h"
#include "BackGround.h"
#include "Camera.h"
#include"Player.h"
#include"Enemy.h"
#include"Item.h"
#include"HitCheck.h"

enum GameState
{
    TITLE,      // �^�C�g�����
    TUTORIAL,   //�`���[�g���A��
    PLAYING,    // �Q�[����
    RESULT      // ���U���g���
};

class Game
{
public:
    Game();
    void Update();
    void Draw();

private:
    GameState currentState;
    bool Title_isStart;     // �X�^�[�g�I������Ă��邩
    bool Title_isInputBottun;   //����{�^���������ꂽ��
    bool isInputUpDown;     //�\���L�[��������Ă��邩
    int Title_Start;        // �X�^�[�g�I���摜
    int Title_Start_Decision;
    int Title_Tutorial;     // �����т����I���摜
    int Title_Tutorial_Decision;     // �����т����I���摜
    int Tutorial_mov;       //�`���[�g���A������n���h��
    int Tutorial_Decision;
    int ChangeStateBoxAlpha;
    int joyInputX;
    int joyInputY;
    int saveTime;
    int HighScore;
    bool isPlayingMovie = false;
    bool changeState;
    bool a = false;
    int Score_UI;           // �X�R�A��UI
    int HighScore_UI;       // �Q�[�����n�C�X�R�AUI
    int Score_Effect;       // "+1"
    int HP_UI;
    int GameOver_Die;
    int GameOver_Fall;
    int Results_UI;
    int Results_backGround;
    float alpha = 1.0f;	//�摜�����x
    float img_Y = -700.0f;	//�摜Y���W
    void DrawScoreEffect(int i); // "+1"�\��
    VECTOR effectPos[maxGroundNum];
    int FontHandle;     //�t�H���g�n���h��
    bool isDrawingScoreEffect;  //"+1"�\������

    int CountDown_3;
    int CountDown_2;
    int CountDown_1;
    int CountDown_Start;
    int CountDownNum = 3; // �����J�E���g�_�E������ (3�b)
    float CountDownTime;

    int titleSound;     // �^�C�g��BGM
    int tutorialSound;  // �`���[�g���A��BGM
    int gameSound;      // �Q�[��BGM
    int resultSound;    // ���U���gBGM
    int gameSE_score;     // �X�R�A�l�����ʉ�
    bool isPlayingSound = false;
    int titleSE_choose; // �J�[�\���ړ����ʉ�
    int titleSE_push;   // ����{�^�����ʉ�
    int gameSE_heart;	// �n�[�g�l�����ʉ�
    int gameSE_gameover;    //�Q�[���I�[�o�[���ʉ�
    bool isInit;
    bool isDebugMode;    // �f�o�b�O���[�h�̃t���O
    bool inputF1;        // F1�L�[�������ꂽ��
    bool inputF2;        // F2�L�[�������ꂽ��
    bool inputF3;        // F3�������ꂽ��
    bool enemyStop;      //�@�G��~��
    bool inputA;         //A�L�[�������ꂽ��
    bool hitStop;        // �q�b�g�X�g�b�v
    bool addScore[maxGroundNum];       //�X�R�A�����Z���ꂽ��
    bool isStartGame;   // �Q�[�����J�n������
    bool isCountDown;
    int ObjCode;         // �f�o�b�O�����삷�镨�̔ԍ�
    int enemyNum;        // �G�ԍ�
    int frameCount;      // �t���[���J�E���g
    int StartTimeCount;       // FPS�v�Z�p�̊J�n����
    int currentTime;     // FPS�v�Z�p�̎���
    int fps;             // FPS
    int padState;           //�R���g���[��[�̏��



    BackGround background;
    Camera camera;
    Fishing ply1;
    Player player;
    Enemy* enemy[maxGroundNum];
    Enemy* TitleEnemy;
    Item item;
    Timer* enemyAwakenTime;
    Timer* hitStopTimer;
    Timer* waitChangeTimer;
    Timer* gameOverWaitTimer;
    Timer* gameCountUpTimer;
    HitChecker hitChecker;
    Timer* startTimer;

    void UpdateTitle();  // �^�C�g����ʂ̍X�V����
    void UpdateTutorial();//�`���[�g���A����ʂ̍X�V
    void UpdatePlaying(); // �Q�[�����̍X�V����
    void UpdateResult();  // ���U���g��ʂ̍X�V����

    void DrawTitle();   // �^�C�g����ʂ̕`�揈��
    void DrawTutorial();//�`���[�g���A����ʂ̕`��
    void DrawPlaying();  // �Q�[�����̕`�揈��
    void DrawResult();   // ���U���g��ʂ̕`�揈��

    void enemyAwaken();

    void ToggleDebugMode();  // �f�o�b�O���[�h�̃I��/�I�t�؂�ւ�
    void DisplayDebugInfo(); // �f�o�b�O���̕\��
    void DebugMove(VECTOR& pos, VECTOR& rotate, VECTOR& scale);    //�@�f�o�b�O�ł̈ړ�
    void SelectDebugObject();// �f�o�b�O�ňړ�������̂�I��
    void DisplayFPS();       // FPS�\��
    void HitStopManager();
    void DrawHP(int HP);
    void changeGameState(GameState gameState,int stopSound);
    void fadeOut();
    void CountDown();
};
