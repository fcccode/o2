#include "BooleanProperty.h"

#include "UI/UIManager.h"
#include "UI/Toggle.h"

namespace Editor
{
	BooleanProperty::BooleanProperty()
	{
		InitializeWidget();
	}

	BooleanProperty::BooleanProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	void BooleanProperty::InitializeWidget()
	{
		mToggle = o2UI.CreateWidget<UIToggle>("without caption");
		mToggle->layout.minHeight = 10;
		mToggle->onToggleByUser = Function<void(bool)>(this, &BooleanProperty::OnEdited);
	}

	BooleanProperty::~BooleanProperty()
	{
		delete mToggle;
	}

	void BooleanProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const bool& value) { *((Property<bool>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<bool>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const bool& value) { *((bool*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((bool*)(ptr)); };
		}

		mValuesPointers = targets;

		Update();
	}

	void BooleanProperty::Update()
	{
		if (mValuesPointers.IsEmpty())
			return;

		mCommonValue = mGetFunc(mValuesPointers[0]);
		mValuesDifferent = false;

		for (int i = 1; i < mValuesPointers.Count(); i++)
		{
			if (mCommonValue != mGetFunc(mValuesPointers[i]))
			{
				mValuesDifferent = true;
				break;
			}
		}

		if (mValuesDifferent)
		{
			mToggle->value = false;
			mToggle->SetValueUnknown();
			mCommonValue = false;
		}
		else mToggle->value = mCommonValue;
	}

	UIWidget* BooleanProperty::GetWidget() const
	{
		return mToggle;
	}

	bool BooleanProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool BooleanProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* BooleanProperty::GetFieldType() const
	{
		return &TypeOf(bool);
	}

	void BooleanProperty::OnEdited(bool value)
	{
		mCommonValue = value;
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);
	}
}
 
CLASS_META(Editor::BooleanProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mToggle);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Update);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(bool, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, InitializeWidget);
	PROTECTED_FUNCTION(void, OnEdited, bool);
}
END_META;
 