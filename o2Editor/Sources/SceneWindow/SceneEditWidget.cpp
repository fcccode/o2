#include "SceneEditWidget.h"

#include "SceneWindow/SceneEditScreen.h"

namespace Editor
{
	SceneEditWidget::SceneEditWidget():
		UIWidget()
	{
		mSceneEditScreen = mnew SceneEditScreen();
		RetargetStatesAnimations();
	}

	SceneEditWidget::SceneEditWidget(const SceneEditWidget& other):
		UIWidget(other)
	{
		mSceneEditScreen = mnew SceneEditScreen();
		RetargetStatesAnimations();
	}

	SceneEditWidget::~SceneEditWidget()
	{
		delete mSceneEditScreen;
	}

	SceneEditWidget& SceneEditWidget::operator=(const SceneEditWidget& other)
	{
		UIWidget::operator=(other);
		UpdateLayout();
		return *this;
	}

	void SceneEditWidget::Draw()
	{
		if (mFullyDisabled)
			return;

		UIWidget::Draw();
		mSceneEditScreen->Draw();
	}

	void SceneEditWidget::Update(float dt)
	{
		UIWidget::Update(dt);
		mSceneEditScreen->Update(dt);
	}

	void SceneEditWidget::UpdateLayout(bool forcible /*= false*/, bool withChildren /*= true*/)
	{
		UIWidget::UpdateLayout(forcible, withChildren);
		mSceneEditScreen->SetRect(layout.GetAbsoluteRect());
	}

}
 
CLASS_META(Editor::SceneEditWidget)
{
	BASE_CLASS(o2::UIWidget);

	PROTECTED_FIELD(mSceneEditScreen);

	PUBLIC_FUNCTION(void, Draw);
	PUBLIC_FUNCTION(void, Update, float);
	PROTECTED_FUNCTION(void, UpdateLayout, bool, bool);
}
END_META;
 