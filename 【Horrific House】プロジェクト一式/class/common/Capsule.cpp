#include <DxLib.h>
#include "Capsule.h"

constexpr int COLOR = 0xffffff;

void Capsule::Draw(void)
{
	// ã‚Ì‹…‘Ì
	VECTOR pos1 = topPos;
	DrawSphere3D(pos1, radius, 5, COLOR, COLOR, false);

	// ‰º‚Ì‹…‘Ì
	VECTOR pos2 = downPos;
	DrawSphere3D(pos2, radius, 5, COLOR, COLOR, false);

	VECTOR dir;
	VECTOR s;
	VECTOR e;

	// ‹…‘Ì‚ğŒq‚®ü(X+)
	dir = { 1.0f, 0.0f, 0.0f };
	s = VAdd(pos1, VScale(dir, radius));
	e = VAdd(pos2, VScale(dir, radius));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(X-)
	dir = { -1.0f, 0.0f, 0.0f };
	s = VAdd(pos1, VScale(dir, radius));
	e = VAdd(pos2, VScale(dir, radius));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z+)
	dir = { 0.0f, 0.0f, 1.0f };
	s = VAdd(pos1, VScale(dir, radius));
	e = VAdd(pos2, VScale(dir, radius));
	DrawLine3D(s, e, COLOR);

	// ‹…‘Ì‚ğŒq‚®ü(Z-)
	dir = { 0.0f, 0.0f, -1.0f };
	s = VAdd(pos1, VScale(dir, radius));
	e = VAdd(pos2, VScale(dir, radius));
	DrawLine3D(s, e, COLOR);

	// ƒJƒvƒZƒ‹‚Ì’†S
	DrawSphere3D(GetCenter(), 5.0f, 10, COLOR, COLOR, true);
}

float Capsule::GetHeight(void)
{
	return topPos.y;
}

VECTOR Capsule::GetCenter(void)
{
	VECTOR top = topPos;
	VECTOR down = downPos;

	VECTOR diff = VSub(top, down);
	return VAdd(down, VScale(diff, 0.5f));
}
