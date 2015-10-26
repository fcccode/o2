#include "Camera.h"

#include "Render/Render.h"

namespace o2
{
	Camera::Camera(const Vec2F& position /*= Vec2F()*/, const Vec2F& size /*= o2Render.GetResolution()*/, 
				   float angle /*= 0.0f*/):
		Transform(size, position, angle)
	{
		if (size == Vec2F())
			SetSize(o2Render.GetResolution());
	}

	Camera Camera::Default()
	{
		return Camera();
	}

}