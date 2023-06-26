#pragma once
#include "../../../common/manager/ImageMng.h"
#include "../../../tmx/TmxObj.h"
#include "../../../common/CheckHit.h"
enum class StatusUI
{
	Stop,
	MoveIn,
	MoveOut,
	Max
};

enum class CntType
{
	Non,
	Key,
	Pad,
	Smh,
	Max
};

template <typename T>
struct UIObj
{
	std::string drawImage;
	std::pair<Vector2, Vector2> coll;
	std::string tipsImage;
	T CanHitUI;
};

template <typename T>
struct UISelectObj
{
	std::string drawImage;
	std::pair<Vector2, Vector2> coll;
	std::string tipsImage;
	std::string deviceImage;
	CntType type;
	T CanHitUI;
};

template <typename T>
struct UIObjWithModel
{
	std::string drawImage;
	std::pair<Vector2, Vector2> coll;
	std::string tipsImage;
	T CanHitUI;
	std::string modelStr;
};

struct MoveUIObj
{
	std::string imageName;
	Vector2 Destination;
	Vector2 Pos;
	StatusUI IsMove;
	int StopCnt;
	int popCnt;
	bool endflag;
};



class UI
{
public:
	UI();
	~UI();
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
private:

protected:
	/*template<typename T>
	T PutClickHit(CollListName& collList,std::map<std::string,T> hitlist);*/
	
	
	/// <summary>
	/// �}�E�X�����������������Ԃ�
	/// </summary>
	/// <param name="colllist">�R���W�����̃��X�g</param>
	/// <returns>���������������Ԃ�</returns>
	std::string PutMouseHit(CollListName& colllist);
	/// <summary>
	/// X�𒆐S�ɍ��킹�ĕ`��
	/// </summary>
	/// <param name="coll">�R���W����</param>
	/// <param name="image">�摜</param>
	void CollDrawCenterX(Collision coll, std::string image);
	/// <summary>
	/// ���S�ŕ`��
	/// </summary>
	/// <param name="coll">�R���W����</param>
	/// <param name="image">�摜</param>
	void CollDrawCenter(Collision coll, std::string image);
	//�}�E�X���W
	Vector2Int mousePos_;

	//tmx
	TmxObj tmxObj_;

	//�}�E�X��������UI
	std::string mouseHitCol_;
	//�q���g�pcoll
	std::pair<Vector2, Vector2> tipsColl_;
	//�t�F�[�h�C���A�E�g�֐�
	double FadeInOutCnt_;
};