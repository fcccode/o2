#include "Window.h"

#include "Application/Application.h"
#include "Render/Render.h"
#include "Render/Sprite.h"
#include "Render/Text.h"
#include "UI/Button.h"
#include "UI/UIManager.h"
#include "UI/WidgetLayer.h"
#include "UI/WidgetLayout.h"
#include "Utils/Debug.h"
#include "WidgetState.h"

namespace o2
{
	UIWindow::UIWindow():
		UIScrollArea(), DrawableCursorEventsListener(this)
	{
		layout->anchorMin = Vec2F(0.5f, 0.5f);
		layout->anchorMax = Vec2F(0.5f, 0.5f);
		layout->offsetMin = Vec2F(-100, -100);
		layout->offsetMax = Vec2F(100, 100);

		InitializeHandles();
		InitializeProperties();
	}

	UIWindow::UIWindow(const UIWindow& other):
		UIScrollArea(other), DrawableCursorEventsListener(this), mHeadDragAreaLayout(other.mHeadDragAreaLayout),
		mTopDragAreaLayout(other.mTopDragAreaLayout), mBottomDragAreaLayout(other.mBottomDragAreaLayout),
		mLeftDragAreaLayout(other.mLeftDragAreaLayout), mRightDragAreaLayout(other.mRightDragAreaLayout),
		mLeftTopDragAreaLayout(other.mLeftTopDragAreaLayout), mRightTopDragAreaLayout(other.mRightTopDragAreaLayout),
		mLeftBottomDragAreaLayout(other.mLeftBottomDragAreaLayout),
		mRightBottomDragAreaLayout(other.mRightBottomDragAreaLayout)
	{
		for (auto elem : other.mWindowElements)
		{
			auto newElem = dynamic_cast<UIWidget*>(elem->Clone());
			newElem->mParent = this;
			mWindowElements.Add(newElem);
		}

		UIButton* closeBtn = (UIButton*)mWindowElements.FindMatch(
			[](UIWidget* x) { return x->GetName() == "closeButton" && x->GetType() == TypeOf(UIButton); });

		if (closeBtn)
			closeBtn->onClick += [&]() { Hide(); };

		RetargetStatesAnimations();
		UpdateLayout();

		InitializeHandles();
		InitializeProperties();
	}

	UIWindow::~UIWindow()
	{
		for (auto elem : mWindowElements)
			delete elem;
	}

	UIWindow& UIWindow::operator=(const UIWindow& other)
	{
		for (auto elem : mWindowElements)
			delete elem;

		UIScrollArea::operator=(other);

		for (auto elem : other.mWindowElements)
		{
			auto newElem = dynamic_cast<UIWidget*>(elem->Clone());
			newElem->mParent = this;
			mWindowElements.Add(newElem);
		}

		UIButton* closeBtn = (UIButton*)mWindowElements.FindMatch(
			[](UIWidget* x) { return x->GetName() == "closeButton" && x->GetType() == TypeOf(UIButton); });

		if (closeBtn)
			closeBtn->onClick += [&]() { Hide(); };

		mHeadDragAreaLayout        = other.mHeadDragAreaLayout;
		mTopDragAreaLayout         = other.mTopDragAreaLayout;
		mBottomDragAreaLayout      = other.mBottomDragAreaLayout;
		mLeftDragAreaLayout        = other.mLeftDragAreaLayout;
		mRightDragAreaLayout       = other.mRightDragAreaLayout;
		mLeftTopDragAreaLayout     = other.mLeftTopDragAreaLayout;
		mRightTopDragAreaLayout    = other.mRightTopDragAreaLayout;
		mLeftBottomDragAreaLayout  = other.mLeftBottomDragAreaLayout;
		mRightBottomDragAreaLayout = other.mRightBottomDragAreaLayout;

		RetargetStatesAnimations();
		BindHandlesInteractableToVisibility();
		UpdateLayout();

		return *this;
	}

	void UIWindow::Update(float dt)
	{
		if (mFullyDisabled)
			return;

		UIScrollArea::Update(dt);

		for (auto elem : mWindowElements)
			elem->Update(dt);
	}

	void UIWindow::Draw()
	{
		if (mFullyDisabled)
			return;

		mBackCursorArea.OnDrawn();

		UIScrollArea::Draw();

		mHeadDragHandle.OnDrawn();
		mTopDragHandle.OnDrawn();
		mBottomDragHandle.OnDrawn();
		mLeftDragHandle.OnDrawn();
		mRightDragHandle.OnDrawn();
		mLeftTopDragHandle.OnDrawn();
		mRightTopDragHandle.OnDrawn();
		mLeftBottomDragHandle.OnDrawn();
		mRightBottomDragHandle.OnDrawn();

		for (auto elem : mWindowElements)
			elem->Draw();

//  		int clr = 0;
//  		o2Render.DrawRectFrame(mHeadDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mTopDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mBottomDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mLeftDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mRightDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mLeftTopDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mRightTopDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mLeftBottomDragAreaRect, Color4::SomeColor(clr++));
//  		o2Render.DrawRectFrame(mRightBottomDragAreaRect, Color4::SomeColor(clr++));
	}

	void UIWindow::ShowModal()
	{
		Show();
		SetModal(true);
	}

	UIWidget* UIWindow::AddWindowElement(UIWidget* widget)
	{
		widget->mParent = this;
		mWindowElements.Add(widget);
		UpdateLayout();

		return widget;
	}

	void UIWindow::RemoveWindowElement(UIWidget* widget)
	{
		mWindowElements.Remove(widget);
		delete widget;
		UpdateLayout();
	}

	void UIWindow::RemoveAllWindowElements()
	{
		for (auto elem : mWindowElements)
			delete elem;

		UpdateLayout();
	}

	void UIWindow::SetIcon(Sprite* icon)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
		{
			if (iconLayer->drawable)
				delete iconLayer->drawable;

			iconLayer->drawable = icon;
		}
	}

	Sprite* UIWindow::GetIcon() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return dynamic_cast<Sprite*>(iconLayer->drawable);
		
		return nullptr;
	}

	void UIWindow::SetIconLayout(const Layout& layout)
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			iconLayer->layout = layout;
	}

	Layout UIWindow::GetIconLayout() const
	{
		auto iconLayer = GetLayer(mIconLayerPath);
		if (iconLayer)
			return iconLayer->layout;

		return Layout();
	}

	void UIWindow::SetCaption(const WString& caption)
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->drawable))
				textDrawable->SetText(caption);
		}
	}

	WString UIWindow::GetCaption() const
	{
		auto captionLayer = GetLayer(mCaptionLayerPath);
		if (captionLayer)
		{
			if (auto textDrawable = dynamic_cast<Text*>(captionLayer->drawable))
				return textDrawable->GetText();
		}

		return WString();
	}

	void UIWindow::SetDragAreaLayouts(const Layout& head, const Layout& top, const Layout&bottom, const Layout&left,
									  const Layout& right, const Layout& leftTop, const Layout& rightTop,
									  const Layout& leftBottom, const Layout& rightBottom)
	{
		mHeadDragAreaLayout = head;
		mTopDragAreaLayout = top;
		mBottomDragAreaLayout = bottom;
		mLeftDragAreaLayout = left;
		mRightDragAreaLayout = right;
		mLeftTopDragAreaLayout = leftTop;
		mRightTopDragAreaLayout = rightTop;
		mLeftBottomDragAreaLayout = leftBottom;
		mRightBottomDragAreaLayout = rightBottom;
	}

	bool UIWindow::IsFocusable() const
	{
		return true;
	}

	void UIWindow::SetModal(bool isModal)
	{
		mBackCursorArea.interactable = isModal;
	}

	bool UIWindow::IsModal() const
	{
		return mBackCursorArea.IsInteractable();
	}

	void UIWindow::UpdateLayout(bool withChildren /*= true*/)
	{
		UIScrollArea::UpdateLayout(withChildren);

		RectF _mChildrenAbsRect = mChildrenWorldRect;
		mChildrenWorldRect = layout->mData->worldRectangle;

		for (auto elem : mWindowElements)
			elem->UpdateLayout(true);

		mChildrenWorldRect = _mChildrenAbsRect;

		mHeadDragAreaRect        = mHeadDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mTopDragAreaRect         = mTopDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mBottomDragAreaRect      = mBottomDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mLeftDragAreaRect        = mLeftDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mRightDragAreaRect       = mRightDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mLeftTopDragAreaRect     = mLeftTopDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mRightTopDragAreaRect    = mRightTopDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mLeftBottomDragAreaRect  = mLeftBottomDragAreaLayout.Calculate(layout->mData->worldRectangle);
		mRightBottomDragAreaRect = mRightBottomDragAreaLayout.Calculate(layout->mData->worldRectangle);
	}

	CursorEventsArea& UIWindow::GetBackCursorListener()
	{
		return mBackCursorArea;
	}

	void UIWindow::UpdateTransparency()
	{
		UIScrollArea::UpdateTransparency();

		for (auto elem : mWindowElements)
			elem->UpdateTransparency();
	}

	void UIWindow::InitializeHandles()
	{
		mBackCursorArea.isUnderPoint = [&](const Vec2F& point) { return true; };
		mBackCursorArea.interactable = false;

		mHeadDragHandle.isUnderPoint = [&](const Vec2F& point) { return mHeadDragAreaRect.IsInside(point); };
		mHeadDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->position += cursor.delta; };
		mHeadDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };

		mTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mTopDragAreaRect.IsInside(point); };
		mTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->worldTop += cursor.delta.y; };
		mTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mTopDragHandle.cursorType = CursorType::SizeNS;

		mBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mBottomDragAreaRect.IsInside(point); };
		mBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->worldBottom += cursor.delta.y; };
		mBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mBottomDragHandle.cursorType = CursorType::SizeNS;

		mLeftDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftDragAreaRect.IsInside(point); };
		mLeftDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->left += cursor.delta.x; };
		mLeftDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftDragHandle.cursorType = CursorType::SizeWE;

		mRightDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightDragAreaRect.IsInside(point); };
		mRightDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->right += cursor.delta.x; };
		mRightDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightDragHandle.cursorType = CursorType::SizeWE;

		mLeftTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftTopDragAreaRect.IsInside(point); };
		mLeftTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->worldLeftTop += cursor.delta; };
		mLeftTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftTopDragHandle.cursorType = CursorType::SizeNwSe;

		mLeftBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mLeftBottomDragAreaRect.IsInside(point); };
		mLeftBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->leftBottom += cursor.delta; };
		mLeftBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mLeftBottomDragHandle.cursorType = CursorType::SizeNeSw;

		mRightTopDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightTopDragAreaRect.IsInside(point); };
		mRightTopDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->rightTop += cursor.delta; };
		mRightTopDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightTopDragHandle.cursorType = CursorType::SizeNeSw;

		mRightBottomDragHandle.isUnderPoint = [&](const Vec2F& point) { return mRightBottomDragAreaRect.IsInside(point); };
		mRightBottomDragHandle.onMoved = [&](const Input::Cursor& cursor) { layout->rightBottom += cursor.delta; };
		mRightBottomDragHandle.onCursorPressed = [&](const Input::Cursor& cursor) { OnFocused(); };
		mRightBottomDragHandle.cursorType = CursorType::SizeNwSe;

		BindHandlesInteractableToVisibility();
	}

	void UIWindow::SetHandlesInteractable(bool interactable)
	{
		mHeadDragHandle.interactable        = interactable;
		mTopDragHandle.interactable         = interactable;
		mBottomDragHandle.interactable      = interactable;
		mLeftDragHandle.interactable        = interactable;
		mRightDragHandle.interactable       = interactable;
		mLeftTopDragHandle.interactable     = interactable;
		mRightTopDragHandle.interactable    = interactable;
		mLeftBottomDragHandle.interactable  = interactable;
		mRightBottomDragHandle.interactable = interactable;
	}

	void UIWindow::BindHandlesInteractableToVisibility()
	{
		if (mVisibleState)
		{
			mVisibleState->onStateBecomesTrue += [&]() { SetHandlesInteractable(true); };
			mVisibleState->onStateFullyFalse += [&]() { SetHandlesInteractable(false); };
		}
	}

	void UIWindow::OnFocused()
	{
		if (mParent)
			mParent->SetPositionIndexInParent(mParent->GetChildren().Count() - 1);

		onFocused();
	}

	void UIWindow::OnChildFocused(UIWidget* child)
	{
		OnFocused();
	}

	void UIWindow::OnCursorPressed(const Input::Cursor& cursor)
	{
		o2UI.FocusWidget(this);
	}

	void UIWindow::OnStateAdded(UIWidgetState* state)
	{
		BindHandlesInteractableToVisibility();
	}

	void UIWindow::OnVisibleChanged()
	{
		interactable = mResVisible;
		
		if (mResVisible)
			Focus();
	}

	void UIWindow::InitializeProperties()
	{
		INITIALIZE_PROPERTY(UIWindow, icon, SetIcon, GetIcon);
		INITIALIZE_PROPERTY(UIWindow, caption, SetCaption, GetCaption);
	}
}

DECLARE_CLASS(o2::UIWindow);
