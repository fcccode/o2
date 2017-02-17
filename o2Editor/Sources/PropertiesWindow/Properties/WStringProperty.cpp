#include "WStringProperty.h"

#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/EditBox.h"
#include "UI/UIManager.h"

namespace Editor
{
	WStringProperty::WStringProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("singleline edit property");

		mEditBox = mPropertyWidget->FindChild<UIEditBox>();
		if (!mEditBox)
			mEditBox = dynamic_cast<UIEditBox*>(mPropertyWidget);

		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &WStringProperty::OnEdited);

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();

		if (auto state = mPropertyWidget->state["revert"])
			*state = true;
	}

	WStringProperty::~WStringProperty()
	{
		delete mPropertyWidget;
	}

	void WStringProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const WString& value) { *((Property<WString>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<WString>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const WString& value) { *((WString*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((WString*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void WStringProperty::Refresh()
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
				SetUnknownValue();
		}
		else if (lastCommonValue != newCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	UIWidget* WStringProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	WString WStringProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool WStringProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void WStringProperty::SetValue(const WString& value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void WStringProperty::SetUnknownValue()
	{
		mCommonValue = "";
		mValuesDifferent = true;
		mEditBox->text = "--";

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	const Type* WStringProperty::GetFieldType() const
	{
		return &TypeOf(WString);
	}

	void WStringProperty::SetCommonValue(const WString& value)
	{
		mCommonValue = value;
		mValuesDifferent = false;
		mEditBox->text = value;

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void WStringProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		SetValue(data);
	}
}

CLASS_META(Editor::WStringProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mEditBox);
	PROTECTED_FIELD(mRevertBtn);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(WString, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, const WString&);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, SetCommonValue, const WString&);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
}
END_META;
 