#pragma once
#include<chrono>
#include<map>
#include <DxLib.h>
#include"EffekseerForDXLib.h"
#include"../common/Geometry.h"
#include "BaseScene.h"

#define lpSceneMng SceneManager::GetInstance()

class MouseIcon;
class Fps;

class SceneManager
{
public:

	static SceneManager& GetInstance()
	{
		static SceneManager s_Instance;
		return s_Instance;
	}

	//������
	void Init(void);
	void Init3D(void);
	//�X�V
	void Run(void);

	//��ʃT�C�Y�擾
	Vector2Int GetScreenSize(void);

	//�f���^�^�C���擾
	double GetDeltaTime(void);

	//�萔�o�b�t�@�Ƀf�[�^�����鏈��
	void SetBrightnessScrren(int brightnessScrren);
	int GetBrightnessScrren();
	void AddInventoryCnt();
	int GetInventoryCnt();
private:
	//�V�X�e������������
	bool SysInit(void);
	SceneManager();
	~SceneManager();

	void Update(void);
	void Draw(void);

	void UpdateMouse();
	//�V�[�����
	UniqueScene scene_;
	int sceneGraphFilter_;
	//�o�ߎ���
	std::chrono::system_clock::time_point tickCount_;

	//�f���^�^�C��
	double deltaTime_;

	int brightnessScrren_;
	//std::unique_ptr<MouseIcon> mouseIcon_;
	int inventoryCnt_;

	std::unique_ptr<Fps> fps_;
};
