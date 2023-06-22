#include<DxLib.h>
#include "InventoryScene.h"
#include "SceneManager.h"
#include "../common/manager/ModelMng.h"
#include "../common/manager/ImageMng.h"
#include "../common/manager/CBufferMng.h"
#include "../common/manager/SoundMng.h"

#include "../object/Item/Item.h"

#include "../input/PadInput.h"
#include "../input/KeyInput.h"

#include "../tmx/TmxObj.h"
#include "../common/CheckHit.h"
#include "../common/Geometry.h"
#include "../common/MouseIcon.h"

#include"../common/OriginalShader.h"
#include "../common/SocketServer.h"

#include "../../_debug/_DebugConOut.h"

//�ڂ����̓x����
constexpr float BLUR_SIGMA = 8.0f;

//�����̃I�t�Z�b�g��������
constexpr float OFFSET = 50.0f;

//�ʐ^�Ƀ}�E�X�����킹���Ƃ��ɒǉ�����l
constexpr float PHOTO_ADD_POS = 5.0f;

//�g�厞�̈ʒu�A�傫��
constexpr float PHOTO_EXPANSION_SIZE_X = 1200.0f;
constexpr float PHOTO_EXPANSION_SIZE_Y = PHOTO_EXPANSION_SIZE_X * 0.5655;
constexpr float PHOTO_EXPANSION_POS_X = (1920.0f- PHOTO_EXPANSION_SIZE_X) / 2.0f;
constexpr float PHOTO_EXPANSION_POS_Y = (1080.0f - PHOTO_EXPANSION_SIZE_Y) / 2.0f;

//�t���[���̐F(��)
constexpr int   FRAME_COLOR = 125;

InventoryScene::InventoryScene(UniqueScene beforeScene,int bgHandle,ControllerType type, const std::vector<InventoryObj>& inventoryList, const std::vector<int>& photo)
{
    beforeScene_ = std::move(beforeScene);

    if (type == ControllerType::Pad)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }

    //�w�i
    backGroundHandle_ = bgHandle;
    controller_->Update();

    inventoryList_ = inventoryList;

    //�ʐ^������
    std::string name;
    for (int i = 0; i < photo.size(); i++)
    {
        name = "photo" + std::to_string(i + 1);
        photoTbl_.try_emplace(name, photo[i]);
    }

    Init();
    test_ = 0;
    //������h�~
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);
}

InventoryScene::~InventoryScene()
{
    Release();
}

void InventoryScene::Init(void)
{
    lpMouseIcon.Init();
    //�e�[�u��������
    InitTable();

    //�u���[�̃s�N�Z���V�F�[�_�n���h��
    psBlurXandYHandle_ = LoadPixelShader("./resource/shader/ps/blur_ps.pso");

    //�����_�����O�^�[�Q�b�g�쐬
    mainRenderTarget_ = MakeScreen(screenSizeX_, screenSizeY_);
    font_ = CreateFontToHandle("HG���ȏ���", 17, 3, DX_FONTTYPE_NORMAL);

    //���_������
    InitVerts();

    //�d�݌v�Z
    CalcBlurWeights();

    //�萔�o�b�t�@�������A���������Ɉ�x�X�V
    for (int n = 0; n < WEIGHTS_NUM; n++)
    {
        //lpSceneMng.SetWriterData(n, weights_[n]);
        lpCBufferMng.SetPixelCbuffer(9, n, weights_[n]);
    }
    lpCBufferMng.Update();

    InitBlur();

    if (lpSceneMng.GetInventoryCnt() == 1)
    {
        isDrawTips_ = true;
    }

    //�ʐ^�֌W
    psPhotoHandle_  = LoadPixelShader("./resource/shader/ps/Memo_ps.pso");
    photoOrigShader_ = std::make_unique<OriginalShader>(psPhotoHandle_);
    int VertexShader = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
    int PixelShader = LoadPixelShader("resource/shader/ps/TexOnlyModel_ps.pso");
    origShader_ = std::make_unique<OriginalShader>(VertexShader, PixelShader);
    InitInventory();
}

void InventoryScene::InitBlur(void)
{
    //���u���[������
    blurX_.Init({ screenSizeX_ / 2, screenSizeY_ }, "./resource/shader/vs/blurX_vs.vso");
    blurX_.origShader = std::make_unique<OriginalShader>(blurX_.vsHandle, psBlurXandYHandle_);

    //�c�u���[������
    blurY_.Init({ screenSizeX_, screenSizeY_ / 2 }, "./resource/shader/vs/blurY_vs.vso");
    blurY_.origShader = std::make_unique<OriginalShader>(blurY_.vsHandle, psBlurXandYHandle_);
}

void InventoryScene::InitVerts(void)
{
    for (auto& v : totalBlurVerts_)
    {
        v.rhw = 1.0f;
        v.dif = GetColorU8(255, 255, 255, 255);
        v.pos.z = 0.0f;
    }

    totalBlurVerts_[0].pos.x = 0.0f;
    totalBlurVerts_[0].pos.y = 0.0f;

    totalBlurVerts_[1].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[1].pos.y = 0.0f;

    totalBlurVerts_[2].pos.x = 0.0f;
    totalBlurVerts_[2].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[3].pos.x = static_cast<float>(screenSizeX_);
    totalBlurVerts_[3].pos.y = static_cast<float>(screenSizeY_);

    totalBlurVerts_[0].u = 0.0f;
    totalBlurVerts_[0].v = 0.0f;

    totalBlurVerts_[1].u = 1.0f;
    totalBlurVerts_[1].v = 0.0f;

    totalBlurVerts_[2].u = 0.0f;
    totalBlurVerts_[2].v = 1.0f;

    totalBlurVerts_[3].u = 1.0f;
    totalBlurVerts_[3].v = 1.0f;

}

UniqueScene InventoryScene::Update(UniqueScene scene)
{
    controller_->Update();
    
    if (controller_->CheckInputKey(KeyID::Inventory))
    {
        //�}�E�X�J�[�\����^�񒆂ɖ߂��ăJ������]�����Ȃ��悤�ɂ���
        //�^�񒆂ɖ߂��Ȃ������ꍇ�̓C���x���g������߂����ۂɓ�����������]���Ă��܂�
        SetMousePoint(lpSceneMng.GetScreenSize().x / 2, lpSceneMng.GetScreenSize().y / 2);
        lpSoundMng.PlayingSound("resource/sound/InventoryCloseSE.mp3");
        return std::move(beforeScene_);
    }

    UpdateInventory();
    
    //�f�o�b�O�p
    if (CheckHitKey(KEY_INPUT_6))
    {
        isDrawTips_ = false;
    }

    DrawScreen();
    return scene;
}

void InventoryScene::DrawScreen(void)
{
    //���C�������_�����O�^�[�Q�b�g�ɕ`��
    SetDrawScreen(mainRenderTarget_);
    ClsDrawScreen();

    //�w�i
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, backGroundHandle_, false);

    //���u���[��������
    SetDrawScreen(blurX_.renderTarget);
    ClsDrawScreen();
    blurX_.origShader->DrawPrimitive3D({ 0.0f,0.0f }, blurX_.imageSize, mainRenderTarget_);

    //�c�u���[��������
    SetDrawScreen(blurY_.renderTarget);
    ClsDrawScreen();
    blurY_.origShader->DrawPrimitive3D({ 0.0f,0.0f },blurY_.imageSize, blurX_.renderTarget);

    //�ŏI�`��
    SetDrawScreen(screenID_);
    ClsDrawScreen();
    
    //�c�u���[�̂��̂�`��
    DrawPrimitive2D(totalBlurVerts_.data(), totalBlurVerts_.size(), DX_PRIMTYPE_TRIANGLESTRIP, blurY_.renderTarget, false);

    DrawInventory();
}

void InventoryScene::Release(void)
{
    DeleteGraph(mainRenderTarget_);
    DeleteGraph(blurX_.renderTarget);
    DeleteGraph(blurY_.renderTarget);

    DeleteShader(psBlurXandYHandle_);
    DeleteShader(blurX_.vsHandle);
    DeleteShader(blurY_.vsHandle);
    DeleteShader(psPhotoHandle_);
}

void InventoryScene::CalcBlurWeights(void)
{
    //�d�݂̍��v
    float totalWeight = 0.0f;

    //x�͊�e�N�Z������̋���
    for (int x = 0; x < WEIGHTS_NUM; x++)
    {
        weights_[x] = expf(-0.5f * static_cast<float>(x * x) / BLUR_SIGMA);
        totalWeight += 2.0f * weights_[x];
    }

    //���Z���A���v��1�ɂ���
    for (int i = 0; i < WEIGHTS_NUM; i++)
    {
        weights_[i] /= totalWeight;
    }
}

void InventoryScene::CollMousePosAndPhoto(void)
{
    //�}�E�X�̈ʒu�擾
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (const auto& coll : tmxObj_.GetInventoryCollList())
    {
        std::string collName = coll.first;
        //�Ή������ʒu�łȂ����continue
        if (!photoTbl_.count(collName))
        {
            continue;
        }

        //�}�E�X�̈ʒu�Ǝʐ^�̈ʒu���������ǂ�������
        if (CheckCollMouse()(mPos, coll.second))
        {
            //�g��t���O�𗧂Ă�
            photoTbl_[collName].addFlag = true;

            if (controller_->CheckInputKey(KeyID::Decide))
            {
                photoTbl_[collName].expansionFlag = true;
                photoTbl_[collName].stepLerpTime = PHOTO_LERP_TIME;
            }
        }
        else
        {
            //���̎ʐ^�̓J�[�\���������Ă��Ȃ��̂�false�ɂ���
            photoTbl_[collName].addFlag = false;
        }
    }
}

void InventoryScene::DrawPhoto(std::string name, Vector2 pos, Vector2 imageSize)
{
    if (!photoTbl_.count(name))
    {
        return;
    }

    //�ŏI�ʒu�v�Z(�����_)
    auto totalPos = [](Vector2 initPos,Vector2 addPos) {
        Vector2 vec2Pos = initPos + addPos;
        Vector3 vec3Pos = { vec2Pos.x,vec2Pos.y,0.0f };
        return vec3Pos;
    };

    //�}�E�X�J�[�\���Əd�Ȃ��Ă����ꍇ�͏����g�傷��
    Vector2 addPos = photoTbl_[name].addFlag ? Vector2(PHOTO_ADD_POS, PHOTO_ADD_POS) : Vector2(0.0f, 0.0f);

    //���_���
    std::array<Vector3, 4> vertPos;

    vertPos[0] = totalPos({ pos.x,pos.y },                             { -addPos.x,-addPos.y });
    vertPos[1] = totalPos({ pos.x + imageSize.x,pos.y },               {  addPos.x,-addPos.y });
    vertPos[2] = totalPos({ pos.x,pos.y + imageSize.y },               { -addPos.x, addPos.y });
    vertPos[3] = totalPos({ pos.x + imageSize.x,pos.y + imageSize.y }, {  addPos.x, addPos.y });
    //�g��t���O������ꍇ�͕�Ԃ�����
    if (photoTbl_[name].expansionFlag)
    {
        float lerpTime = 1.0f - (photoTbl_[name].stepLerpTime / PHOTO_LERP_TIME);
        vertPos[0] = Lerp(vertPos[0], { PHOTO_EXPANSION_POS_X,PHOTO_EXPANSION_POS_Y,1.0f }, lerpTime);
        vertPos[1] = Lerp(vertPos[1], { PHOTO_EXPANSION_POS_X + PHOTO_EXPANSION_SIZE_X,PHOTO_EXPANSION_POS_Y,1.0f }, lerpTime);
        vertPos[2] = Lerp(vertPos[2], { PHOTO_EXPANSION_POS_X,PHOTO_EXPANSION_POS_Y + PHOTO_EXPANSION_SIZE_Y,1.0f }, lerpTime);
        vertPos[3] = Lerp(vertPos[3], { PHOTO_EXPANSION_POS_X + PHOTO_EXPANSION_SIZE_X,PHOTO_EXPANSION_POS_Y + PHOTO_EXPANSION_SIZE_Y,1.0f }, lerpTime);
    }

    //�z��ɐ��l�����킹�ĕ`��
    photoOrigShader_->DrawOnlyPixel(vertPos, photoTbl_[name].handle);
}

void InventoryScene::InitInventory(void)
{
    //�C���x���g���̃f�[�^�ǂݍ���
    tmxObj_.LoadTmx("resource/tmx/inventory.tmx", false);
    for (auto& inventory : inventoryList_)
    {
        //�l�̏������ƃT�C�Y�̒���
        inventory.pos_ = { 0.0f,0.0f,0.0f };
        MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(inventory.scale_.toVECTOR(), 0.1f));
    }

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                for (auto& inventory : inventoryList_)
                {
                    //�l�̃Z�b�g
                    if (inventory.pos_ == Vector3{ 0.0f,0.0f,0.0f })
                    {
                        //�l�ƃC���x���g���̔Ԓn�̃Z�b�g
                        inventory.pos_ = { coll.second.first.x + OFFSET,lpSceneMng.GetScreenSize().y - coll.second.first.y - OFFSET,0 };
                        inventory.inventoryNum_ = itemTable.first;
                        break;
                    }
                }
            }
        }
    }
}

void InventoryScene::UpdateInventory(void)
{
    for (auto& photo : photoTbl_)
    {
        //�g�債�Ă���ꍇ�͑��̏��������Ȃ�
        if (photo.second.expansionFlag)
        {
            //������������ꍇ�͊g�����߂�
            if (controller_->CheckInputKey(KeyID::Decide))
            {
                photo.second.stepLerpTime = PHOTO_LERP_TIME;
                photo.second.expansionFlag = false;
            }
            photo.second.stepLerpTime -= lpSceneMng.GetDeltaTime();
            return;
        }

    }

    hitItemID_ = ItemID::Non;
    Vector2Int mPos;
    GetMousePoint(&mPos.x, &mPos.y);

    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryList_)
        {
            if (coll.first != inventory.inventoryNum_)
            {
                continue;
            }
            for (auto& itemTable : itemTable_)
            {
                if (itemTable.first != inventory.inventoryNum_)
                {
                    continue;
                }
                //�}�E�X�̓����������̐F��ς���
                //�ǂ�ID���i�[
                if (CheckCollMouse()(mPos, coll.second))
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                    if (inventory.usedItemID_ != ItemID::Non)
                    {
                        itemTable.second = { 75,75,75 };
                    }
                }
                else
                {
                    itemTable.second = { 125,125,125 };
                    if (inventory.usedItemID_ != ItemID::Non)
                    {
                        itemTable.second = { 75,75,75 };
                    }
                }

                if (inventory.inventoryNum_ == lpSocketServer.GetinventorySelect())
                {
                    hitItemID_ = inventory.itemID_;
                    itemTable.second = { 255,255,255 };
                }
            }

        }
    }

    //�ʐ^�ƃ}�E�X�̓����蔻��
    CollMousePosAndPhoto();

    for (auto& inventory : inventoryList_)
    {
        //�}�E�X���������Ă���ԁ@���W�A�傫���A��]�@��ς���
        if (hitItemID_ == inventory.itemID_)
        {
            MV1SetPosition(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VGet(950,
                650, 0));
            Vector3 dd = inventory.scale_ * 1;
            //�I�����čۂ̉�]��ݒ肵�Đ��ʂɌ�������
            switch (inventory.itemID_)
            {
            case ItemID::Entrance_Key:
            case ItemID::Kitchen_Key:
            case ItemID::MusicRoom_Key:
            case ItemID::FoodBank_Key:
            case ItemID::ChildRoom_Key:
            case ItemID::Celler_Key:
                //���������ꍇ�̉�]���Z�b�g����
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(0, 0, 0).toVECTOR());
                break;
            case ItemID::Phot:
               
                //�ʐ^��������ꍇ�A�傫�����g�債�Ẳ�]���Z�b�g����
                dd = inventory.scale_ * 1.5f;
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(0, Deg2RadF(180), 0).toVECTOR());
                break;
            case ItemID::Mapitem:
                dd = inventory.scale_ * 1.8f;
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(Deg2RadF(90), 0, Deg2RadF(180)).toVECTOR());
                break;
            default:
                //��L�ȊO�̃A�C�e����������-90�x������Ēu��
                MV1SetRotationXYZ(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], Vector3(Deg2RadF(-90), 0, 0).toVECTOR());
                break;
            }

            MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(dd.toVECTOR(), 1));

        }
        else
        {
            //���W�A�傫�������Ƃɖ߂�
            MV1SetPosition(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], inventory.pos_.toVECTOR());
            MV1SetScale(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_], VScale(inventory.scale_.toVECTOR(), 0.1f));
        }
    }
}

void InventoryScene::DrawInventory(void)
{
    int color = FRAME_COLOR;
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& inventory : inventoryTable_)
        {
            if (coll.first == inventory.first)
            {
                //�X�g�����O�ɉ��������ĂȂ������畁�ʂ̎l�p������
                if (inventory.second == "")
                {
                    DrawBox(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y),
                        GetColor(color, color, color), false);
                    color += 1;
                }
                else
                {
                    //�����Ă����炻���`�悷��
                    DrawExtendGraph(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y), lpImageMng.GetID(inventory.second)[0], true);
                    break;
                }
            }

        }
        //�A�C�e���̘g�̕`��
        for (auto& itemTable : itemTable_)
        {
            if (coll.first == itemTable.first)
            {
                DrawBox(static_cast<int>(coll.second.first.x),
                    static_cast<int>(coll.second.first.y),
                    static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                    static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y),
                    GetColor(static_cast<int>(itemTable.second.x), static_cast<int>(itemTable.second.y), static_cast<int>(itemTable.second.z)), false);
            }
        }
        /*if (coll.first == "target")
        {
            DrawFormatStringToHandle(static_cast<int>(coll.second.first.x + OFFSET), static_cast<int>(coll.second.first.y + OFFSET), 0x000000, font_, "���ւ̌��������ĒE�o���悤");
            DrawFormatString(static_cast<int>(coll.second.first.x + OFFSET), static_cast<int>(coll.second.first.y + OFFSET * 2), 0x000000, "�T���Ă镨");
            DrawGraph(static_cast<int>(coll.second.first.x + OFFSET * 5), static_cast<int>(coll.second.first.y + OFFSET * 1.5), lpImageMng.GetID("resource/image/marker/marker.png")[0], true);
        }*/
        //�ʐ^�`��
        DrawPhoto(coll.first, { coll.second.first }, { coll.second.second });
    }
    //���f���`��
    for (auto& inventory : inventoryList_)
    {
        origShader_->Draw(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_]);
        //MV1DrawModel(lpModelMng.GetID(inventory.modelString_)[inventory.modelNum_]);
    }
    //�����̕`��i���̓X�g�����O�ŏI�I�ɉ摜�j
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        if (coll.first == "explanation")
        {
            for (auto& explanation : explanationTable_)
            {
                //ID�m�F
                if (hitItemID_ == explanation.first)
                {
                    //�����Ă����炻���`�悷��
                    DrawExtendGraph(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y), lpImageMng.GetID(explanation.second)[0], true);
                }
            }
        }
    }
    for (auto& coll : tmxObj_.GetInventoryCollList())
    {
        for (auto& option : optionTable_)
        {
            if (coll.first == option.first)
            {
                DrawFormatStringToHandle(static_cast<int>(coll.second.first.x + OFFSET), static_cast<int>(coll.second.first.y + OFFSET / 2), 0xffffff, font_, "%s", option.second.c_str());
                if (isDrawTips_)
                {
                    if(coll.first == "invtyTips")
                    DrawBox(static_cast<int>(coll.second.first.x),
                        static_cast<int>(coll.second.first.y),
                        static_cast<int>(coll.second.first.x) + static_cast<int>(coll.second.second.x),
                        static_cast<int>(coll.second.first.y) + static_cast<int>(coll.second.second.y),
                        GetColor(144, 144, 144), false);
                }
            }
        }
    }
}

void InventoryScene::InitTable(void)
{
    inventoryTable_ = {
            {"belongings",	"resource/image/inventory/Inventory.png"},
            //{"target",	"resource/image/inventory/memo.png"},
            {"explanation",""}
    };
    Vector3 Zero = { 125,125,125 };
    itemTable_ = {
        {"1",Zero},
        {"2",Zero},
        {"3",Zero},
        {"4",Zero},
        {"5",Zero},
        {"6",Zero},
        {"7",Zero},
        {"8",Zero},
        {"9",Zero},
        {"10",Zero},
        {"11",Zero},
        {"12",Zero},
        {"13",Zero},
        {"14",Zero},
        {"15",Zero},
        {"16",Zero},
        {"17",Zero},
        {"18",Zero},
        {"19",Zero},
        {"20",Zero}
    };

    explanationTable_ = {
        {ItemID::Entrance_Key,"Entrancekeydescribe"},   //���ւ̌��̐����摜
        {ItemID::Kitchen_Key,"kitchenkeydescribe"},     //�L�b�`���̌��̐����摜
        {ItemID::MusicRoom_Key,"musicroomkeydescribe"}, //���y���̌��̐����摜
        {ItemID::FoodBank_Key,"Pantrykeydescribe"},     //�H�ƌɂ̌��̐����摜
        {ItemID::ChildRoom_Key,"childRoomkeydescribe"}, //�q�������̐����摜
        {ItemID::Celler_Key,"basementkeydescribe"},     //�n�����̌��̐����摜

        {ItemID::Enemy_Memo,"enemymemo"},               //�G�̎�_�̐����摜

        {ItemID::Story_Book_A,"storymemo_a"},           //�X�g�[���[����a�̐����摜
        {ItemID::Story_Memo_B,"storymemo_b"},           //�X�g�[���[����b�̐����摜
        {ItemID::Story_Book_C,"storymemo_c"},           //�X�g�[���[����c���ǂ̐����摜1

        {ItemID::Wall_Memo,"hint_d"},                   //�ǂ̐����摜2
        {ItemID::Picture_Memo,"hint_q"},                //���ڊԂ̊G��̐����摜
        {ItemID::CookWare_Memo,"hint_f"},               //�������̐����摜
        {ItemID::Score_Memo,"hint_g"},                  //�y���̐����摜
        {ItemID::Living_Memo,"hint_h"},                 //�Q�[���\�t�g�̐����摜
        {ItemID::Clock_Book,"hint_j"},                  //�L�̎��v�ƊG��̐����摜
        {ItemID::Celler_Memo,"hint_t"},                 //�x���ł̐����摜

        {ItemID::Phot,"Photdescide"},                   //�ʐ^�̐����摜
        {ItemID::Mapitem,"mapItemdescide"}              //�}�b�v�A�C�e���̐����摜


    };
    if (controller_->GetControllerType() == ControllerType::Key)
    {
        optionTable_ = {
                {"tab",	"tab�ŃQ�[���ɖ߂�"},
                {"mouse","�J�[�\�������킹�ăA�C�e���̊m�F"},
                {"invtyTips",""}
        };
    }
    else
    {
        optionTable_ = {
                {"tab",	"Y�ŃQ�[���ɖ߂�"},
                {"mouse","�J�[�\�������킹�ăA�C�e���̊m�F"},
                {"invtyTips",""}
        };
    }
}
