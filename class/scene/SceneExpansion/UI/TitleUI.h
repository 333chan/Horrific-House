#pragma once
#include "UI.h"
#include "../../../common/Geometry.h"
#include "../../../tmx/TmxObj.h"
#include "../../../input/Controller.h"


enum class TitleColUI
{
	Non,
	Title,
	Play,
	Option,
	Exit,
	Mouse,
	Operation,
	Max
};
//�M�~�b�N�I�u�W�F�N�g���
struct UIobj
{
	std::string objName_;   //�I�u�W�F�N�g��
	Vector3 pos_;           //���W
	Vector3 angle_;	//��]
	Vector3 scale_;
	UIobj(std::string objName, const Vector3& pos, const Vector3& angle, const Vector3& scale) :
		objName_(objName),
		pos_(pos),
		angle_(angle),
	    scale_(scale) {}
};
class OriginalShader;
class Controller;
enum class ControllerType;
class TitleUI :
	public UI
{
public:
	TitleUI();
	TitleUI(std::unique_ptr<Controller> controller);
	~TitleUI();
	void Init()override;
	void Update()override;
	void Draw()override;
	//�N���b�N���ē��������R���W������enum��Ԃ�
	TitleColUI GetHitUICol();
	//�R���g���[���[�^�C�v���擾
	ControllerType GetControllerType();
	//���݂̃R���g���[�����擾
	std::unique_ptr<Controller> GetController();

	//�v���^�C�g�����I����Ă�����
	bool GetPreEndFlag();
private:
	//UIMap�̏�����
	void InitUIMap();
	//�N���b�N���ē���������
	void PutClickHit();	
	void LoadBinData(const char* bData,std::string pName);
	std::vector<UIobj> uiObjList_;
	//�^�C�g��UI�̃}�b�v
	std::map<TitleColUI, UIObjWithModel<TitleColUI>> titleUIMap_;
	//���蓖������UI
	TitleColUI decideHitCol_;
	
	//���f���p�X
	std::string modelPath_;
	//�V�F�[�_�[�n���h��
	int psLight_;
	int vsLight_;
protected:

	//�R���g���[�����
	std::unique_ptr<Controller> controller_;
	//�I���W�i���V�F�[�_�[
	std::unique_ptr<OriginalShader> origShader_;
	std::unique_ptr<OriginalShader> originalShader_;
	//�v���^�C�g���I����Ă邩�t���O
	bool PreEndFlag_;
};