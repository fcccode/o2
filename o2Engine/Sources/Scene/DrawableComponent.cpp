#include "DrawableComponent.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/SceneLayer.h"

namespace o2
{
	DrawableComponent::DrawableComponent():
		Component(), SceneDrawable()
	{}

	DrawableComponent::DrawableComponent(const DrawableComponent& other) :
		Component(other), SceneDrawable(other)
	{}

	DrawableComponent& DrawableComponent::operator=(const DrawableComponent& other)
	{
		Component::operator=(other);
		SceneDrawable::operator=(other);

		return *this;
	}

	void DrawableComponent::SetDrawingDepth(float depth)
	{
		SceneDrawable::SetDrawingDepth(depth);

		if (mOwner)
			mOwner->OnChanged();
	}

	void DrawableComponent::UpdateEnabled()
	{
		bool lastResEnabled = mResEnabled;

		if (mOwner)
			mResEnabled = mEnabled && mOwner->mResEnabled;
		else
			mResEnabled = mEnabled;

		if (lastResEnabled != mResEnabled && mLayer)
		{
			if (mResEnabled)
				mLayer->DrawableEnabled(this);
			else
				mLayer->DrawableDisabled(this);
		}
	}

	void DrawableComponent::SetOwnerActor(Actor* actor)
	{
		SceneDrawable::SetLayer(actor->mLayer);
		Component::SetOwnerActor(actor);

		if (mOwner)
		{
			mOwner->mComponents.Add(this);
			OnTransformUpdated();
		}
	}
}

CLASS_META(o2::DrawableComponent)
{
	BASE_CLASS(o2::Component);
	BASE_CLASS(o2::SceneDrawable);


	PUBLIC_FUNCTION(void, SetDrawingDepth, float);
	PROTECTED_FUNCTION(void, UpdateEnabled);
	PROTECTED_FUNCTION(void, SetOwnerActor, Actor*);
}
END_META;
