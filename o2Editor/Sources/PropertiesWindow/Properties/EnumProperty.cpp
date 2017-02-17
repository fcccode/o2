#include "EnumProperty.h"

#include "Animation/AnimatedFloat.h"
#include "Animation/AnimatedVector.h"
#include "Animation/Animation.h"
#include "Render/Sprite.h"
#include "SceneWindow/SceneEditScreen.h"
#include "UI/Button.h"
#include "UI/DropDown.h"
#include "UI/UIManager.h"

namespace Editor
{
	EnumProperty::EnumProperty(UIWidget* widget /*= nullptr*/)
	{
		if (widget)
			mPropertyWidget = widget;
		else
			mPropertyWidget = o2UI.CreateWidget<UIWidget>("enum property");

		mDropDown = mPropertyWidget->FindChild<UIDropDown>();
		mDropDown->onSelectedText = Function<void(const WString&)>(this, &EnumProperty::OnSelectedItem);

		mRevertBtn = mPropertyWidget->FindChild<UIButton>();

		if (auto state = mPropertyWidget->state["revert"])
			*state = true;
	}

	EnumProperty::~EnumProperty()
	{
		delete mPropertyWidget;
	}

	void EnumProperty::SetValueAndPrototypePtr(const TargetsVec& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, int value) { *((Property<int>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<int>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, int value) { *((int*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((int*)(ptr)); };
		}

		mValuesPointers = targets;

		Refresh();
	}

	void EnumProperty::Refresh()
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
		else if (newCommonValue != lastCommonValue || lastDifferent)
			SetCommonValue(newCommonValue);
	}

	UIWidget* EnumProperty::GetWidget() const
	{
		return mPropertyWidget;
	}

	int EnumProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool EnumProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	void EnumProperty::SetValue(int value)
	{
		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr.first, value);

		SetCommonValue(value);
	}

	void EnumProperty::SetUnknownValue()
	{
		mCommonValue = 0;
		mValuesDifferent = true;

		mUpdatingValue = true;
		mDropDown->SelectItemAt(-1);
		mUpdatingValue = false;

		mDropDown->SetState("undefined", true);

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	const Type* EnumProperty::GetFieldType() const
	{
		return &TypeOf(void);
	}

	void EnumProperty::SpecializeType(const Type* type)
	{
		if (type->GetUsage() == Type::Usage::Property)
			mEnumType = dynamic_cast<const EnumType*>(((const PropertyType*)type)->GetValueType());
		else
			mEnumType = dynamic_cast<const EnumType*>(type);

		if (mEnumType)
		{
			mEntries = &mEnumType->GetEntries();

			for (auto kv : *mEntries)
				mDropDown->AddItem(kv.Value());
		}
	}

	void EnumProperty::SetCommonValue(int value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		mUpdatingValue = true;
		mDropDown->value = (*mEntries)[mCommonValue];
		mUpdatingValue = false;

		mDropDown->SetState("undefined", false);

		onChanged();
		o2EditorSceneScreen.OnSceneChanged();
	}

	void EnumProperty::OnSelectedItem(const WString& name)
	{
		if (mUpdatingValue)
			return;

		SetValue(mEntries->FindValue(name).Key());
	}
}

CLASS_META(Editor::EnumProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mEnumType);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mEntries);
	PROTECTED_FIELD(mPropertyWidget);
	PROTECTED_FIELD(mRevertBtn);
	PROTECTED_FIELD(mDropDown);
	PROTECTED_FIELD(mUpdatingValue);

	PUBLIC_FUNCTION(void, SetValueAndPrototypePtr, const TargetsVec&, bool);
	PUBLIC_FUNCTION(void, Refresh);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(int, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(void, SetValue, int);
	PUBLIC_FUNCTION(void, SetUnknownValue);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PUBLIC_FUNCTION(void, SpecializeType, const Type*);
	PROTECTED_FUNCTION(void, SetCommonValue, int);
	PROTECTED_FUNCTION(void, OnSelectedItem, const WString&);
}
END_META;
 