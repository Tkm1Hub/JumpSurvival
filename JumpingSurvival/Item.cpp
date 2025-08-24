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
    //プレイヤーの体力が3以下の場合のみ動作
    if (playerHP < 3)
    {
        HeartRotate.y += RotateSpeed;   //ハートを回転させる

        //ハートが出現中でない場合
        if (!IsActiveHeart)
        {
            if (!HeartSpawnTimer->isActive())
            {
                HeartSpawnTimer->start();
            }
            ArrowPos = VGet(0, 1.5f, 0);
        }
        //出現中の場合
        else
        {
            if (!HeartDisappearTimer->isActive())
            {
                HeartDisappearTimer->start();
            }
            
            //プレイヤーとハートの角度を求める
            float deltaX = HeartPos.x - playerPos.x;
            float deltaZ = HeartPos.z - playerPos.z;

            ArrowAngle = atan2(-deltaX,-deltaZ);

            ArrowRotate.y = ArrowAngle;

            //矢印の座標を求める
            float ArrowX = playerPos.x + ArrowR * -sinf(ArrowAngle);
            float ArrowZ = playerPos.z + ArrowR * -cosf(ArrowAngle);
            ArrowPos = VGet(ArrowX, 1.5f, ArrowZ);

            //プレイヤーとハートが近くなったら矢印を非表示
            float distance = sqrtf(powf(HeartPos.x - playerPos.x, 2) + powf(HeartPos.z - playerPos.z, 2));
            if (distance < 8.0f)    //距離が8.0f以下だと非表示
            {
                isActiveArrow = false;  //非表示
            }
            else
            {
                isActiveArrow = true;   //表示
            }
        }

        if (HeartSpawnTimer->hasElapsed())  //スポーンまでの時間が経過
        {
            IsActiveHeart = true;
            HeartSpawnTimer->reset();
        }

        if (HeartDisappearTimer->hasElapsed())  //消えるまでの時間が経過
        {
            IsActiveHeart = false;
            HeartPos = GenerateRandomPosition();    //ハートが消えたら座標を再設定
            HeartDisappearTimer->reset();
        }

        //ハートをセット
        MV1SetPosition(HeartHandle, HeartPos);
        MV1SetRotationXYZ(HeartHandle, HeartRotate);
        //矢印をセット
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

    // x, z の絶対値の合計が 35 以下となるようにランダムな値を生成
    do {
        x = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f の範囲でランダムなx座標
        z = static_cast<float>(rand() % 71) - 35.0f;  // -35.0f ~ +35.0f の範囲でランダムなz座標
    } while (fabs(x) + fabs(z) > 35.0f);  // xとzの絶対値の合計が35を超えないように再生成

    // y座標は常に2.0f
    float y = 2.0f;

    // VECTORに座標を格納して返す
    return VGet(x, y, z);
}