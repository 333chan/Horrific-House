#pragma once
class SceneExpansion
{
public :

	SceneExpansion();
	~SceneExpansion();
	virtual void Init(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
};

