#ifndef __IVIEW
#define __IVIEW
#include "Vector2.h"

enum color { white, red, transparent, green, yellow, blue, brown };

class IView
{
private:

public:
	virtual void DrawLine(const Vector2&, const Vector2&) = 0;
	virtual void SetColor(color) = 0;
	virtual void Run() = 0;
	virtual void Clear() = 0;
};

#endif // !__IVIEW


