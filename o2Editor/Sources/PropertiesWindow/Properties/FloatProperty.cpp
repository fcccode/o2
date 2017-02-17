#include "FloatProperty.h"

#include "Application/Application.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/UIManager.h"

namespace Editor
{
	FloatProperty::FloatProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("singleline edit property");

		mEditBox = mPropertyWidget->FindChild<UIEditBox>();
		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &FloatProperty::OnEdited);
		mEditBox->text = "--";
		mEditBox->SetFilterFloat();

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();

		if (auto state = mPropertyWidget->state["revert"])
			*state = true;

		auto handleLayer = mEditBox->GetLayer("arrows");

		if (handleLayer)
		{
			mEditBox->onDraw += [&]() { mDragHangle.OnDrawn(); };

			mDragHangle.cursorType = CursorType::SizeNS;
			mDragHangle.isUnderPoint = [=](const Vec2F& point) { return handleLayer->IsUnderPoint(point); };
			mDragHangle.onMoved = Function<void(const Input::Cursor&)>(this, &FloatProperty::OnDragHandleMoved);
			mDragHangle.onCursorPressed = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(true); };
			mDragHangle.onCursorReleased = [&](const Input::Cursor&) { o2Application.SetCursorInfiniteMode(false); };
		}
	}

	FloatProperty::~FloatProperty()
	{
		delete mPropertyWidget;
	}

	void FloatProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const float& value) { *((Property<float>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<float>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const float& value) { *((float*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((float*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void FloatProperty::Refresh()
	{
		if (mValuesPointers.IsEmpty())
			return;

		auto lastCommonValue = mCommonValue;
		auto lastDifferent = mValuesDifferent;

		auto newCommonValue = mGetFunc(mValuesPointers[0].first);
		auto newDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (newCommonValue != mGetFunc(mValuesPointers[i].first))
			{
				newDifferent = true;
				break;
			}
		}

		if (newDifferent)
		{
			if (!lastDifferent)
				SetUnknownValue(newCommonValue);
		}
		else if (!Math::Equals(lastCommonValue, newCommonValue) || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	UIWidget* FloatProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	float FloatProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool FloatProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void FloatProperty::SetValue(float value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void FloatProperty::SetUnknownValue(float defaultValue /*= 0.0f*/)
	{
		mCommonValue = defaultValue;
		mValuesDifferent = true;
		mEditBox->text = "--";

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	const Type* FloatProperty::GetFieldType() const
	{
		return &TypeOf(float);
	}

	void FloatProperty::SetCommonValue(float value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mEditBox->text = (WString)mCommonValue;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void FloatProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValue((const float)data);
	}

	void FloatProperty::OnDragHandleMoved(const Input::Cursor& cursor)
	{
		float multiplier = Math::Abs(cursor.delta.y) < 20 ? 0.01f : 0.05f;
		SetValue(mCommonValue + cursor.delta.y*multiplier);
	}

	void FloatProperty::OnKeyReleased(const Input::Key& key)
	{
		if (!mEditBox->IsFocused())
			return;

		if (key == VK_UP)
		{
			SetValue(Math::Ceil(mCommonValue + 0.01f));
			mEditBox->SelectAll();
		}

		if (key == VK_DOWN)
		{
			SetValue(Math::Floor(mCommonValue - 0.01f));
			mEditBox->SelectAll();
		}
	}

}

CLASS_META(Editor::FloatProperty)
{
	BASE_CLASS(Editor::IPropertyField);
	BASE_CLASS(o2::KeyboardEventsListener);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mDragHangle);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(float, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, float);
	PUBLIC_FUNCTION(void, SetUnknownValue, float);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, float);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
	PROTECTED_FUNCTION(void, OnDragHandleMoved, const Input::Cursor&);
	PROTECTED_FUNCTION(void, OnKeyReleased, const Input::Key&);
}
END_META;
 