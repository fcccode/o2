#include "StringProperty.h"

#include "UI/UIManager.h"
#include "UI/EditBox.h"

namespace Editor
{
	StringProperty::StringProperty()
	{
		InitializeWidget();
	}

	StringProperty::StringProperty(const Vector<void*>& targets, bool isProperty)
	{
		InitializeWidget();
		Setup(targets, isProperty);
	}

	void StringProperty::InitializeWidget()
	{
		mEditBox = o2UI.CreateWidget<UIEditBox>("singleline");
		mEditBox->layout.minHeight = 10;
		mEditBox->onChangeCompleted = Function<void(const WString&)>(this, &StringProperty::OnEdited);
	}

	StringProperty::~StringProperty()
	{
		delete mEditBox;
	}

	void StringProperty::Setup(const Vector<void*>& targets, bool isProperty)
	{
		if (isProperty)
		{
			mAssignFunc = [](void* ptr, const String& value) { *((Property<String>*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return ((Property<String>*)(ptr))->Get(); };
		}
		else
		{
			mAssignFunc = [](void* ptr, const String& value) { *((String*)(ptr)) = value; };
			mGetFunc = [](void* ptr) { return *((String*)(ptr)); };
		}

		mValuesPointers = targets;

		Update();
	}

	void StringProperty::Update()
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
			mEditBox->text = "--";
			mCommonValue = "";
		}
		else mEditBox->text = (WString)mCommonValue;
	}

	UIWidget* StringProperty::GetWidget() const
	{
		return mEditBox;
	}

	String StringProperty::GetCommonValue() const
	{
		return mCommonValue;
	}

	bool StringProperty::IsValuesDifferent() const
	{
		return mValuesDifferent;
	}

	const Type* StringProperty::GetFieldType() const
	{
		return &TypeOf(String);
	}

	void StringProperty::OnEdited(const WString& data)
	{
		if (mValuesDifferent && data == "--")
			return;

		mCommonValue = data;
		mValuesDifferent = false;

		for (auto ptr : mValuesPointers)
			mAssignFunc(ptr, mCommonValue);
	}
}
 
CLASS_META(Editor::StringProperty)
{
	BASE_CLASS(Editor::IPropertyField);

	PROTECTED_FIELD(mAssignFunc);
	PROTECTED_FIELD(mGetFunc);
	PROTECTED_FIELD(mValuesPointers);
	PROTECTED_FIELD(mCommonValue);
	PROTECTED_FIELD(mValuesDifferent);
	PROTECTED_FIELD(mEditBox);

	PUBLIC_FUNCTION(void, Setup, const Vector<void*>&, bool);
	PUBLIC_FUNCTION(void, Update);
	PUBLIC_FUNCTION(UIWidget*, GetWidget);
	PUBLIC_FUNCTION(String, GetCommonValue);
	PUBLIC_FUNCTION(bool, IsValuesDifferent);
	PUBLIC_FUNCTION(const Type*, GetFieldType);
	PROTECTED_FUNCTION(void, InitializeWidget);
	PROTECTED_FUNCTION(void, OnEdited, const WString&);
}
END_META;
 