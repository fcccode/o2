#include "Toggle.h"

#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/UIManager.h"

namespace o2
{
	UIToggle::UIToggle():
		mValue(false), mToggleGroup(nullptr), mCaptionText(nullptr), mBackLayer(nullptr)
	{
		InitializeProperties();
	}

	UIToggle::UIToggle(const UIToggle& other):
		UIWidget(other), mToggleGroup(nullptr)
	{
		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = GetLayer("back");

		RetargetStatesAnimations();
		SetValue(other.mValue);
		InitializeProperties();
	}

	UIToggle& UIToggle::operator=(const UIToggle& other)
	{
		UIWidget::operator=(other);
		mCaptionText = GetLayerDrawable<Text>("caption");
		mBackLayer = GetLayer("back");
		SetValue(other.mValue);
		RetargetStatesAnimations();
		return *this;
	}

	UIToggle::~UIToggle()
	{
		if (mToggleGroup && mToggleGroup->mOwner == this)
			delete mToggleGroup;
	}

	void UIToggle::Update(float dt)
	{
		if (mFullyDisabled)
			return;

		UIWidget::Update(dt);

		if (mToggleGroup && mToggleGroup->mPressed && mToggleGroup->mPressedValue != mValue &&
			(mToggleGroup->mType == UIToggleGroup::Type::VerOneClick || mToggleGroup->mType == UIToggleGroup::Type::HorOneClick))
		{
			float cursory = o2Input.GetCursorPos().y;
			bool underPoint = false;

			if (mToggleGroup->mType == UIToggleGroup::Type::VerOneClick)
				underPoint = cursory > layout.mAbsoluteRect.bottom && cursory < layout.mAbsoluteRect.top;
			else
				underPoint = cursory > layout.mAbsoluteRect.bottom && cursory < layout.mAbsoluteRect.top;
		
			if (underPoint)
			{
				SetValue(!mValue);
				onClick();
			}
		}
	}

	void UIToggle::SetCaption(const WString& text)
	{
		if (mCaptionText)
			mCaptionText->SetText(text);
	}

	WString UIToggle::GetCaption() const
	{
		if (mCaptionText)
			return mCaptionText->GetText();

		return WString();
	}

	void UIToggle::SetValue(bool value)
	{
		mValue = value;

		auto valueState = state["value"];
		if (valueState)
			*valueState = mValue;

		onToggle(mValue);

		if (mToggleGroup)
			mToggleGroup->OnToggled(this);
	}

	bool UIToggle::GetValue() const
	{
		return mValue;
	}

	bool UIToggle::IsUnderPoint(const Vec2F& point)
	{
		if (mBackLayer && mBackLayer->IsUnderPoint(point))
			return true;

		if (mCaptionText && mCaptionText->IsPointInside(point))
			return true;

		return layout.GetAbsoluteRect().IsInside(point);
	}

	float UIToggle::Depth()
	{
		return GetMaxDrawingDepth();
	}

	bool UIToggle::IsSelectable() const
	{
		return true;
	}

	void UIToggle::SetToggleGroup(UIToggleGroup* toggleGroup)
	{
		if (mToggleGroup)
		{
			if (mToggleGroup->mOwner == this)
			{
				if (mToggleGroup->mToggles.Count() == 1)
				{
					mToggleGroup->mToggles.Clear();
					delete mToggleGroup;
				}
				else
				{
					mToggleGroup->mToggles.Remove(this);
					mToggleGroup->mOwner = mToggleGroup->mToggles[0];
				}
			}
			else mToggleGroup->mToggles.Remove(this);
		}

		mToggleGroup = toggleGroup;
		mToggleGroup->mToggles.Add(this);

		if (!mToggleGroup->mOwner)
		{
			mToggleGroup->mOwner = this;

			if (mToggleGroup->mType == UIToggleGroup::Type::OnlySingleTrue)
				SetValue(true);
		}
	}

	UIToggleGroup* UIToggle::GetToggleGroup() const
	{
		return mToggleGroup;
	}

	void UIToggle::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		o2UI.SelectWidget(this);

		if (mToggleGroup)
		{
			SetValue(!mValue);
			onClick();

			mToggleGroup->mPressed = true;
			mToggleGroup->mPressedValue = mValue;
		}
	}

	void UIToggle::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (IsUnderPoint(cursor.mPosition) && 
			!(mToggleGroup && (mToggleGroup->mType == UIToggleGroup::Type::VerOneClick || 
							   mToggleGroup->mType == UIToggleGroup::Type::HorOneClick) && 
			  mToggleGroup->mPressed))
		{
			SetValue(!mValue);
			onClick();
		}

		if (mToggleGroup && (mToggleGroup->mType == UIToggleGroup::Type::VerOneClick ||
							 mToggleGroup->mType == UIToggleGroup::Type::HorOneClick))
		{
			mToggleGroup->mPressed = false;
		}
	}

	void UIToggle::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;

		if (mToggleGroup && (mToggleGroup->mType == UIToggleGroup::Type::VerOneClick ||
							 mToggleGroup->mType == UIToggleGroup::Type::HorOneClick))
		{
			mToggleGroup->mPressed = false;
		}
	}

	void UIToggle::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void UIToggle::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

	void UIToggle::OnKeyPressed(const Input::Key& key)
	{
		if (mIsSelected && (key.mKey == VK_SPACE || key.mKey == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = true;
		}

		if (shortcut.IsPressed())
		{
			SetValue(!mValue);
			onClick();
			onToggleByUser(mValue);
		}
	}

	void UIToggle::OnKeyReleased(const Input::Key& key)
	{
		if (mIsSelected && (key.mKey == VK_SPACE || key.mKey == VK_RETURN))
		{
			auto pressedState = state["pressed"];
			if (pressedState)
				*pressedState = false;

			SetValue(!mValue);
			onClick();
			onToggleByUser(mValue);
		}

	}

	void UIToggle::OnLayerAdded(UIWidgetLayer* layer)
	{
		if (layer->name == "caption" && layer->drawable && layer->drawable->GetType() == Text::type)
			mCaptionText = (Text*)layer->drawable;

		if (layer->name == "back")
			mBackLayer = layer;
	}

	void UIToggle::OnVisibleChanged()
	{
		interactable = mResVisible;
	}

	void UIToggle::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIToggle, caption, SetCaption, GetCaption);
		INITIALIZE_PROPERTY(UIToggle, value, SetValue, GetValue);
		INITIALIZE_PROPERTY(UIToggle, toggleGroup, SetToggleGroup, GetToggleGroup);
	}

	UIToggleGroup::UIToggleGroup(Type type):
		mType(type), mPressed(false)
	{}

	UIToggleGroup::~UIToggleGroup()
	{
		for (auto toggle : mToggles)
			toggle->mToggleGroup = nullptr;
	}

	void UIToggleGroup::AddToggle(UIToggle* toggle)
	{
		mToggles.Add(toggle);
		toggle->mToggleGroup = this;
		toggle->SetValue(true);
	}

	void UIToggleGroup::RemoveToggle(UIToggle* toggle)
	{
		mToggles.Remove(toggle);
		toggle->mToggleGroup = nullptr;
	}

	const UIToggleGroup::TogglesVec& UIToggleGroup::GetToggles() const
	{
		return mToggles;
	}

	void UIToggleGroup::OnToggled(UIToggle* toggle)
	{
		if (mType == Type::OnlySingleTrue)
		{
			if (toggle->GetValue())
			{
				for (auto ctoggle : mToggles)
				{
					if (ctoggle == toggle)
						continue;

					ctoggle->SetValue(false);
				}
			}
			else
			{
				if (!mToggles.Any([&](auto x) { return x->GetValue(); }))
					toggle->SetValue(true);
			}
		}
	}
}