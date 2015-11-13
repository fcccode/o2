#include "UI/HorizontalProgress.h"

namespace o2
{
	IOBJECT_CPP(UIHorizontalProgress);

	UIHorizontalProgress::UIHorizontalProgress():
		mValue(0), mMinValue(0), mMaxValue(1), mOrientation(Orientation::Right), mScrollSense(1.0f)
	{
		InitializeProperties();
	}

	UIHorizontalProgress::UIHorizontalProgress(const UIHorizontalProgress& other):
		UIWidget(other), mValue(other.mValue), mMinValue(other.mMinValue), mMaxValue(other.mMaxValue),
		mOrientation(other.mOrientation), mScrollSense(other.mScrollSense)
	{
		mBarLayer = GetLayer("bar");
		mBackLayer = GetLayer("back");

		InitializeProperties();
	}

	UIHorizontalProgress::~UIHorizontalProgress()
	{}

	UIHorizontalProgress& UIHorizontalProgress::operator=(const UIHorizontalProgress& other)
	{
		UIWidget::operator=(other);
		mValue = other.mValue;
		mMinValue = other.mMinValue;
		mMaxValue = other.mMaxValue;
		mOrientation = other.mOrientation;
		mScrollSense = other.mScrollSense;

		UpdateLayout();

		return *this;
	}

	void UIHorizontalProgress::Update(float dt)
	{
		UIWidget::Update(dt);

		const float threshold = 0.01f;
		const float smoothCoef = 30.0f;

		if (!Math::Equals(mValue, mSmoothValue, threshold))
		{
			mSmoothValue = Math::Clamp(Math::Lerp(mSmoothValue, mValue, dt*smoothCoef), mMinValue, mMaxValue);

			if (Math::Abs(mValue - mSmoothValue) < threshold)
				mSmoothValue = mValue;

			UpdateProgressLayersLayouts();
		}
	}

	void UIHorizontalProgress::SetValue(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void UIHorizontalProgress::SetValueForcible(float value)
	{
		mValue = Math::Clamp(value, mMinValue, mMaxValue);
		mSmoothValue = mValue;
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIHorizontalProgress::GetValue() const
	{
		return mValue;
	}

	void UIHorizontalProgress::SetMinValue(float minValue)
	{
		mMinValue = minValue;
		mValue = Math::Max(mMinValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIHorizontalProgress::GetMinValue() const
	{
		return mMinValue;
	}

	void UIHorizontalProgress::SetMaxValue(float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Min(mMaxValue, mValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	float UIHorizontalProgress::GetMaxValue() const
	{
		return mMaxValue;
	}

	void UIHorizontalProgress::SetValueRange(float minValue, float maxValue)
	{
		mMaxValue = maxValue;
		mValue = Math::Clamp(mValue, mMinValue, mMaxValue);
		UpdateProgressLayersLayouts();

		onChange(value);
	}

	void UIHorizontalProgress::SetScrollSense(float coef)
	{
		mScrollSense = coef;
	}

	float UIHorizontalProgress::GetScrollSense() const
	{
		return mScrollSense;
	}

	void UIHorizontalProgress::SetOrientation(Orientation orientation)
	{
		mOrientation = orientation;
		UpdateLayersLayouts();
	}

	UIHorizontalProgress::Orientation UIHorizontalProgress::GetOrientation() const
	{
		return mOrientation;
	}

	bool UIHorizontalProgress::IsUnderPoint(const Vec2F& point)
	{
		if (mBackLayer)
		{
			return mBackLayer->IsUnderPoint(point);
		}

		return false;
	}

	float UIHorizontalProgress::Depth()
	{
		return GetMaxDrawingDepth();
	}

	void UIHorizontalProgress::OnCursorPressed(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = true;

		GetValueFromCursor(cursor);
	}

	void UIHorizontalProgress::OnCursorReleased(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UIHorizontalProgress::OnCursorPressBreak(const Input::Cursor& cursor)
	{
		auto pressedState = state["pressed"];
		if (pressedState)
			*pressedState = false;
	}

	void UIHorizontalProgress::OnCursorStillDown(const Input::Cursor& cursor)
	{
		GetValueFromCursor(cursor);
	}

	void UIHorizontalProgress::GetValueFromCursor(const Input::Cursor &cursor)
	{
		float width = layout.mAbsoluteRect.Width();
		float d = mMaxValue - mMinValue;
		if (mOrientation == Orientation::Right)
			SetValue((cursor.mPosition.x - layout.mAbsoluteRect.left)/width*d + mMinValue);
		else
			SetValue((width - (cursor.mPosition.x - layout.mAbsoluteRect.left))/width*d + mMinValue);
	}

	void UIHorizontalProgress::OnCursorEnter(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = true;
	}

	void UIHorizontalProgress::OnCursorExit(const Input::Cursor& cursor)
	{
		auto selectState = state["select"];
		if (selectState)
			*selectState = false;
	}

	void UIHorizontalProgress::OnScrolled(float scroll)
	{
		SetValue(mValue + scroll*mScrollSense);
	}

	void UIHorizontalProgress::UpdateProgressLayersLayouts()
	{
		if (mBarLayer)
		{
			mBarLayer->layout.offsetMin = Vec2F();
			mBarLayer->layout.offsetMax = Vec2F();

			if (mOrientation == Orientation::Right)
			{
				mBarLayer->layout.anchorMin = Vec2F(0, 0);
				mBarLayer->layout.anchorMax = Vec2F((mSmoothValue - mMinValue)/(mMaxValue - mMinValue), 1);
			}
			else if (mOrientation == Orientation::Left)
			{
				mBarLayer->layout.anchorMin = Vec2F(1.0f - (mSmoothValue - mMinValue)/(mMaxValue - mMinValue), 0.0f);
				mBarLayer->layout.anchorMax = Vec2F(1, 1);
			}
		}

		if (mBackLayer)
			mBackLayer->layout = Layout::Both();

		UpdateLayersLayouts();
	}

	void UIHorizontalProgress::OnLayerAdded(Ptr<UIWidgetLayer> layer)
	{
		if (layer->name == "back")
			mBackLayer = layer;
		else if (layer->name == "bar")
			mBarLayer = layer;

		UpdateProgressLayersLayouts();
	}

	void UIHorizontalProgress::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIHorizontalProgress, value, SetValue, GetValue);
		INITIALIZE_PROPERTY(UIHorizontalProgress, minValue, SetMinValue, GetMinValue);
		INITIALIZE_PROPERTY(UIHorizontalProgress, maxValue, SetMaxValue, GetMaxValue);
		INITIALIZE_PROPERTY(UIHorizontalProgress, scrollSense, SetScrollSense, GetScrollSense);
	}
}