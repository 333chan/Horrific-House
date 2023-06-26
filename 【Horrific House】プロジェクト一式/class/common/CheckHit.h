#pragma once
#include <string>
#include <iostream>
#include <string>
#include <algorithm>
#include "Geometry.h"
#include "../tmx/TmxObj.h"
#include "../../_debug/_DebugConOut.h"
#include "../../_debug/_DebugDispOut.h"


struct CollisionRay
{

	MV1_COLL_RESULT_POLY operator()(Line& line, const rapidxml::xml_node<>* node,int id)
	{
		// 相手にIDがない場合の関数
		// Unitのコリジョンの更新はされてない
		// 更新したい場合下の関数を読んで
		// 磁石のオブジェクトと判定を取る

		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//レイの長さ

		auto clampX = atof(node->first_attribute("clampX")->value());//中心からのX
		auto clampY = atof(node->first_attribute("clampY")->value());//中心からのY
		Line posLine;
		//ラインに変更
		posLine = { line.p,line.end };
		//プレイヤーの中心座標まで固定値で引き上げてる
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//中心からのX
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//中心からのY
		//返す用
		MV1_COLL_RESULT_POLY hitData{};
		
			auto hit = MV1CollCheck_Line(id, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitData = hit;
			}
			//MV1DeleteModel(id);
		return hitData;
	}

	//MV1_COLL_RESULT_POLY operator()(Line& line, const rapidxml::xml_node<>* node,std::string path,int num)
	//{
	//	// 相手にIDがない場合の関数
	//	// Unitのコリジョンの更新はされてない
	//	// 更新したい場合下の関数を読んで
	//	// 磁石のオブジェクトと判定を取る

	//	std::string name = node->first_attribute("name")->value();
	//	auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//レイの長さ

	//	auto clampX = atof(node->first_attribute("clampX")->value());//中心からのX
	//	auto clampY = atof(node->first_attribute("clampY")->value());//中心からのY
	//	Line posLine;
	//	//ラインに変更
	//	posLine = { line.p,line.end };
	//	//プレイヤーの中心座標まで固定値で引き上げてる
	//	auto centerX = atof(node->first_attribute("Checkcenterx")->value());//中心からのX
	//	auto centerY = atof(node->first_attribute("Checkcentery")->value());//中心からのY
	//	//返す用
	//	MV1_COLL_RESULT_POLY hitData{};
	//	
	//		auto hit = MV1CollCheck_Line(lpModelMng.GetID(path)[num], -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
	//		if (hit.HitFlag > 0)
	//		{
	//			hitData = hit;
	//		}
	//		//MV1DeleteModel(id);
	//	return hitData;
	//}

	MV1_COLL_RESULT_POLY operator()(Line& line, int id)
	{
		MV1_COLL_RESULT_POLY hitData{};

		auto hit = MV1CollCheck_Line(id, -1, line.p.toVECTOR(), line.end.toVECTOR());
		if (hit.HitFlag > 0)
		{
			hitData = hit;
		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Line& line, const rapidxml::xml_node<>* node, std::vector<int>list)
	{
		// 相手にIDがない場合の関数
		// Unitのコリジョンの更新はされてない
		// 更新したい場合下の関数を読んで
		// 磁石のオブジェクトと判定を取る
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//レイの長さ

		auto clampX = atof(node->first_attribute("clampX")->value());//中心からのX
		auto clampY = atof(node->first_attribute("clampY")->value());//中心からのY
		Line posLine;
		//ラインに変更
		posLine = { line.p,line.end };
		//プレイヤーの中心座標まで固定値で引き上げてる
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//中心からのX
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//中心からのY
		//返す用
		MV1_COLL_RESULT_POLY hitData{};
		for (auto StageCollId : list)
		{
			auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitData = hit;
			}
		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec ,Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<std::vector<int>>list)
	{
		//マップ全てに当たり判定をする関数
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//レイの長さ

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//中心からのX
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//中心からのY

		if (clampX != 0 && clampY != 0)
		{
			VecPow.x = std::clamp(VecPow.x, -clampX, clampX);
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//ラインに変更
		posLine = { pos,pos + (VecPow * w) };
		//プレイヤーの中心座標まで固定値で引き上げてる
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//中心からのX
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//中心からのY
		posLine.p.y += static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//返す用
		MV1_COLL_RESULT_POLY hitData{};
		for (auto& Stagelist : list)
		{
			for (auto StageCollId : Stagelist)
			{
				auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
				if (hit.HitFlag > 0)
				{
					hitData = hit;
				}
			}

		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec, Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<std::vector<int>>list,bool flag)
	{
		//マップ全てに当たり判定をする関数
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//レイの長さ

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//中心からのX
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//中心からのY

		if (clampX != 0 && clampY != 0)
		{
			VecPow.x = std::clamp(VecPow.x, -clampX, clampX);
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//ラインに変更
		posLine = { pos,pos + (VecPow * w) };
		//プレイヤーの中心座標まで固定値で引き上げてる
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//中心からのX
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//中心からのY
		posLine.p.y +=static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//返す用
		MV1_COLL_RESULT_POLY hitData{};
		for (auto& Stagelist : list)
		{
			for (auto StageCollId : Stagelist)
			{
				auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
				if (hit.HitFlag > 0)
				{
					hitData = hit;
				}
			}

		}
		return hitData;
	}
	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec, Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<int>list)
	{
		//磁石など、特定の物と当たり判定する関数
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//レイの長さ

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//中心からのX
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//中心からのY

		if (clampX != 0 && clampY != 0)
		{
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//ラインに変更
		posLine = { pos,pos + (VecPow * (w)) };
		//プレイヤーの中心座標まで固定値で引き上げてる
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//中心からのX
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//中心からのY
		/*posLine.p.x += centerX;
		posLine.end.x += centerX;*/
		posLine.p.y += static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//返す用
		MV1_COLL_RESULT_POLY hitData{};
		for (auto StageCollId : list)
		{
			auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitData = hit;
			}
		}
		return hitData;
	}

	MV1_COLL_RESULT_POLY operator()(Vector3& pos, Vector3 moveVec, Vector3 movePower, const rapidxml::xml_node<>* node, std::vector<int>list,int& hitid)
	{
		//磁石など、特定の物と当たり判定する関数
		auto VecPow = moveVec * movePower;
		//MV1RefreshCollInfo(thisId);
		std::string name = node->first_attribute("name")->value();
		auto w = static_cast<float>(atof(node->first_attribute("w")->value()));//レイの長さ

		auto clampX = static_cast<float>(atof(node->first_attribute("clampX")->value()));//中心からのX
		auto clampY = static_cast<float>(atof(node->first_attribute("clampY")->value()));//中心からのY

		if (clampX != 0 && clampY != 0)
		{
			VecPow.y = std::clamp(VecPow.y, -clampY, clampY);
		}
		Line posLine;
		//ラインに変更
		posLine = { pos,pos + (VecPow * (w)) };
		//プレイヤーの中心座標まで固定値で引き上げてる
		auto centerX = atof(node->first_attribute("Checkcenterx")->value());//中心からのX
		auto centerY = atof(node->first_attribute("Checkcentery")->value());//中心からのY
		/*posLine.p.x += centerX;
		posLine.end.x += centerX;*/
		posLine.p.y += static_cast<float>(centerY);
		posLine.end.y += static_cast<float>(centerY);
		//返す用
		MV1_COLL_RESULT_POLY hitData{};
		for (auto StageCollId : list)
		{
			auto hit = MV1CollCheck_Line(StageCollId, -1, posLine.p.toVECTOR(), posLine.end.toVECTOR());
			if (hit.HitFlag > 0)
			{
				hitid = StageCollId;
				hitData = hit;
				return hitData;
			}
		}
		return hitData;
	}
};

struct CheckCollMouse
{
	/// <summary>
	/// マウス位置とUIの当たり判定
	/// </summary>
	/// <param name="pos">マウス座標</param>
	/// <param name="coll">オブジェクト座標</param>
	/// <returns>true:範囲内/false:範囲外</returns>
	bool operator()(Vector2Int& mPos, const Collision& coll)
	{
		//範囲内にマウスがあるかどうかを判定
		if (mPos.x > coll.first.x &&
			mPos.y > coll.first.y &&
			mPos.x < coll.first.x + coll.second.x &&
			mPos.y < coll.first.y + coll.second.y
			)
		{
			return true;
		}
		return false;
	}
};

struct CollisionSphere
{
	/// <summary>
	/// 弾と敵の当たり判定
	/// </summary>
	/// <param name="spherepos">球の位置</param>
	/// <param name="radius">球の半径</param>
	/// <param name="path">モデルパス</param>
	/// <param name="num">モデルナンバー</param>
	/// <returns>true:当たり/false:当たっていない</returns>
	//bool operator()(Vector3 spherepos, float radius, std::string path, int num)
	//{
	//	bool hitF = false;
	//	MV1_COLL_RESULT_POLY_DIM colls{};

	//	colls = MV1CollCheck_Sphere(lpModelMng.GetID(path)[num], -1, spherepos.toVECTOR(), radius);

	//	if (colls.HitNum > 0)
	//	{
	//		hitF = true;
	//	}

	//	//当たり判定の後始末をする
	//	MV1CollResultPolyDimTerminate(colls);

	//	return hitF;
	//}

	bool operator()(Vector3 spherepos, float radius,std::vector<int> stage)
	{
		bool hitF = false;
		MV1_COLL_RESULT_POLY_DIM colls{};
		for (auto& s : stage)
		{
			colls = MV1CollCheck_Sphere(s, -1, spherepos.toVECTOR(), radius);

			if (colls.HitNum > 0)
			{
				hitF = true;
			}
		}

		//当たり判定の後始末をする
		MV1CollResultPolyDimTerminate(colls);

		return hitF;
	}

};