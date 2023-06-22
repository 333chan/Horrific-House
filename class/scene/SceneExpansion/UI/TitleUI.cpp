#include "../../SceneManager.h"
#include "../../../common/manager/ImageMng.h"
#include "../../../common/manager/ModelMng.h"
#include "../../../input/KeyInput.h"
#include "../../../input/PadInput.h"
#include "../../../common/CheckHit.h"
#include "../../../common/OriginalShader.h"
#include "../../../tmx/TmxObj.h"
#include "TitleUI.h"

TitleUI::TitleUI()
{
    //�p�b�h�̐����P�ȏ�̏ꍇ�̓p�b�h����ɂ���
    if (GetJoypadNum() > 0)
    {
        controller_ = std::make_unique<PadInput>();
    }
    else
    {
        controller_ = std::make_unique<KeyInput>();
    }
    //�R���g���[���̍X�V
    controller_->Update();
    Init();
}
TitleUI::TitleUI(std::unique_ptr<Controller> controller)
{
    //�R���g���[���[�̏�����
    controller_ = std::move(controller);
    //�R���g���[���̍X�V
    controller_->Update();
    //������
    Init();
}

TitleUI::~TitleUI()
{
}

void TitleUI::Init()
{
    //tmx
    tmxObj_.LoadTmx("resource/tmx/title.tmx", false);
    //�o�C�i���f�[�^�ǂݍ���
    LoadBinData("resource/data/UI/TitleUIData.dat", "UIObj");
    modelPath_ = "resource/model/UI/";
    //������
    for (auto& uiObj : uiObjList_)
    {
        //�z�u
        MV1SetPosition(lpModelMng.GetID(modelPath_ + uiObj.objName_ + ".mv1")[0], uiObj.pos_.toVECTOR());
        //�x�����烉�W�A���ɕϊ�
        uiObj.angle_ = { Deg2RadF(uiObj.angle_.x),Deg2RadF(uiObj.angle_.y),Deg2RadF(uiObj.angle_.z) };
        //��]
        MV1SetRotationXYZ(lpModelMng.GetID(modelPath_ + uiObj.objName_ + ".mv1")[0], uiObj.angle_.toVECTOR());
        //�g��l
        MV1SetScale(lpModelMng.GetID(modelPath_ + uiObj.objName_ + ".mv1")[0], uiObj.scale_.toVECTOR());
    }

    InitUIMap();
    //�A�E�g���C���`��̃V�F�[�_�[������
    int outL_vs = LoadVertexShader("./resource/shader/vs/OutLine_vs.vso");
    int outL_ps = LoadPixelShader("./resource/shader/ps/OutLine_ps.pso");
    origShader_ = std::make_unique<OriginalShader>(outL_vs, outL_ps);
    //�A�E�g���C���Ȃ��̕��ʂ̕`��p�V�F�[�_�[
    psLight_ = LoadPixelShader("resource/shader/ps/TitleModel_ps.pso");
    vsLight_ = LoadVertexShader("resource/shader/vs/NormalModel_vs.vso");
}

void TitleUI::Update()
{
    //�}�E�X�擾
    GetMousePoint(&mousePos_.x, &mousePos_.y);
    //�R���g���[���̍X�V
   controller_->Update();
   //�N���b�N���ē�������enum�i�[
    PutClickHit();
    //����������������󂯎��
    mouseHitCol_ = PutMouseHit(tmxObj_.GetTitleCollList());
    FadeInOutCnt_ += lpSceneMng.GetDeltaTime();
}

void TitleUI::Draw()
{
    FadeInOutCnt_ += 2;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(FadeInOutCnt_));
    for (auto& uiObj : titleUIMap_)
    {
        if (uiObj.first == TitleColUI::Mouse)
        {
            //�E�����̃}�E�X�`��
            CollDrawCenterX(uiObj.second.coll, uiObj.second.drawImage);
        }
        //�RD���f���̕����`��
        origShader_->Draw(lpModelMng.GetID(uiObj.second.modelStr)[0], mouseHitCol_ == uiObj.second.drawImage,psLight_,vsLight_);
    }
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleUI::InitUIMap()
{
    //�^�C�g���i�[
    titleUIMap_ =
    {
        //enum                               �摜,        �摜�̍��W ,tips�̉摜,         �����������ɕԂ�enum
        {TitleColUI::Play,      UIObjWithModel<TitleColUI>("play",       {{0,0},{0,0}},"gamePlay",         TitleColUI::Play,    "resource/model/UI/Play.mv1")},
        {TitleColUI::Option,    UIObjWithModel<TitleColUI>("option",     {{0,0},{0,0}},"optionOpen",       TitleColUI::Option,  "resource/model/UI/Option.mv1")},
        {TitleColUI::Exit,      UIObjWithModel<TitleColUI>("exit",       {{0,0},{0,0}},"returnToDesktop",  TitleColUI::Exit,    "resource/model/UI/Exit.mv1")},
        {TitleColUI::Mouse,     UIObjWithModel<TitleColUI>("mouse",      {{0,0},{0,0}},"",                 TitleColUI::Non,     "")},
        {TitleColUI::Operation, UIObjWithModel<TitleColUI>("operation",  {{0,0},{0,0}},"",                 TitleColUI::Non,     "")}
    };
    //tmx�������Ă����l��UIMap�ɑ������
    for (auto& coll : tmxObj_.GetTitleCollList())
    {
        for (auto& UI : titleUIMap_)
        {
            if (coll.first == UI.second.drawImage)
            {
                //�摜�̍��W��tmx�������Ă������W������
                UI.second.coll = { coll.second.first,coll.second.second };
            }
        }
        if (coll.first == "operation")
        {
            //�q���g�悤�ɍ��W������Ă���
            tipsColl_ = coll.second;
        }
    }
}

void TitleUI::PutClickHit()
{
    decideHitCol_ = TitleColUI::Non;
    if (controller_->CheckInputKey(KeyID::Decide))
    {
        for (auto& coll : titleUIMap_)
        {
            if (CheckCollMouse()(mousePos_, coll.second.coll))
            {
                //�N���b�N��������������enum�������
                decideHitCol_ = coll.second.CanHitUI;
            }
        }
    }
}

void TitleUI::LoadBinData(const char* bData, std::string pName)
{
    int file = FileRead_open(bData);
    int version;
    FileRead_read(&version, sizeof(version), file);
    if (version == 6)
    {
        while (!FileRead_eof(file))
        {
            std::string name;	//���O
            Vector3 pos;	//���W
            Vector3 rot;	//��]
            Vector3 scale;	//��]
            char nameSize;
            //�n���h���A���O�A���W�A��]���ǂݎ��
            FileRead_read(&nameSize, sizeof(nameSize), file);
            name.resize(static_cast<int>(nameSize));
            FileRead_read(&name[0], nameSize, file);
            FileRead_read(&pos, sizeof(pos), file);
            FileRead_read(&rot, sizeof(rot), file);
            FileRead_read(&scale, sizeof(scale), file);
            if (pName == name)
            {
            	continue;
            }
            //���O�A���W�A��]�����i�[
            uiObjList_.emplace_back(name, pos, rot,scale);
        }
    }
    //�t�@�C�������
    FileRead_close(file);
}

TitleColUI TitleUI::GetHitUICol()
{
    return decideHitCol_;
}

ControllerType TitleUI::GetControllerType()
{
    return controller_->GetControllerType();
}

std::unique_ptr<Controller> TitleUI::GetController()
{
    return std::move(controller_);
}


bool TitleUI::GetPreEndFlag()
{
    return PreEndFlag_;
}


