#pragma once
#include <list>
#include <vector>
#include <memory>
#include <string>

struct Collider
{
	int modelHandle;
};

class Obj
{
public:
	Obj();
	~Obj();

	virtual void Init(void) = 0;
	virtual void Release(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
private:
protected:
};

