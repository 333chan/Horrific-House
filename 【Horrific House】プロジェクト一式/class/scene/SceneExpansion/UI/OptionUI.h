#pragma once
#include "UI.h"

enum class OptionColUI
{
    Non,
    Title,
    Control,
    Audio,
    Graphic,
    TitleReture,
    Reture,
    Mouse,
    Operation,
    Max
};

class Controller;
class OriginalShader;
enum class ControllerType;


class OptionUI:
    public UI
{
public:
    OptionUI(SceneID beforeSceneID);
    ~OptionUI();
    void Init()override;
    void Update()override;
    void Draw()override;
    //��������UI
    OptionColUI GetAfterSceneCol();
private:

    //�Փ˔���
    void HitCollAndAssignProcess();

    /// <summary>
    /// �N���b�N�����Ƃ�����������
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    void HitCollClickAndAssignProcess(std::pair<std::string, Collision> coll);

    /// <summary>
    /// �}�E�X���������Ă��邩
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    std::pair<std::string, std::string> HitCollMouse(std::pair<std::string, Collision> coll);


    //�֐��|�C���^
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    void (OptionUI::* UpdateFuncPtr_)();

    //�X�V�֐��|�C���^�ɑ������֐�
    void UpdateControl();         //�L�[����
    void UpdateAudio();           //����
    void UpdateGraphic();         //���邳����
    void UpdateToTitle();     // �^�C�g���ɍs��
    void UpdateReturnScreen();    //�߂�

    //�`��֐��|�C���^
    void (OptionUI::* DrawFuncPtr_)();

    //�`��֐��|�C���^�ɑ������֐�
    void DrawControl();         //�L�[����
    void DrawAudio();           //����
    void DrawGraphic();         //���邳����
    void DrawToTitle();     // �^�C�g���ɍs��
    void DrawReturnScreen();    //�߂�
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

    //�܂Ƃ߂��R���W������draw
    void DrawColl();
    /// <summary>
    /// �R���W������Y���𒆐S�ɕ`��
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    /// <param name="imageStr">�摜�̃n���h���p������</param>
    void DrawCollCenterY(std::pair<std::string, Collision> coll, std::string imageStr);

    /// <summary>
    /// �R���W�����𒆐S�ɕ`��
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    /// <param name="imageStr">�摜�̃n���h���p������</param>
    void DrawCollCenter(std::pair<std::string, Collision> coll, std::string imageStr);


    /// <summary>
    /// SelectList��p�`��
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    void DrawSelectList(std::pair<std::string, Collision> coll);

    /// <summary>
    /// ���X�g�̕`��
    /// </summary>
    /// <param name="coll">�l�p�`�Փ˔���</param>
    /// <param name="maplist">�`��Ɏg���}�b�v���X�g</param>
    void DrawList(std::pair<std::string, Collision> coll, std::map<std::string, std::string> maplist);


    SceneID beforeSceneID_;      //�O�̃V�[��ID
    UniqueScene returningScene_; //�V�[����؂�ւ��鎞�p
    OptionColUI afterSceneID_;
    std::unique_ptr<Controller> controller_;//�R���g���[�����

    //�t�@�[�X�g��tmx�̕�����Z�J���h�͏����̃|�C���^�[
    std::map<std::string, void (OptionUI::*)()> selectMap_;//�����܂Ƃ߃��X�g

    //std::map<std::string, std::string>* listToDraw_;        //�`�悷�郊�X�g�̃|�C���^

    //�t�@�[�X�g��tmx�̕�����Z�J���h�͉摜�̕�����
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
    std::map<std::string, std::string> displayMap_;        //�`�悾�����X�g
    std::map<std::string, std::string> controlMap_;        //���샊�X�g
    std::map<std::string, std::string> audioMap_;          //�����X�g
    std::map<std::string, std::string> graphicMap_;        //�O���t�B�b�N���X�g
    std::map<std::string, std::string> titleRetureMap_;    //�^�C�g���ɍs�����X�g
    std::map<std::string, std::string> retureMap_;         //�߂郊�X�g

    std::map<std::string, std::string> supportMap_;
    //�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[

    std::pair<std::string, std::string> OldhitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ
    std::pair<std::string, std::string> hitMouse_;       //���}�E�X���������Ă�����̂��i�[����ꏊ


    std::string selection_; //���I�����Ă�����̂��i�[����ꏊ


    int Volume_;    //����
    float brightness_;//���邳

    std::unique_ptr<OriginalShader> origShader_;

};

