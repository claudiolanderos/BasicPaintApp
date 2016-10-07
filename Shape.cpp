#include "Shape.h"

Shape::Shape(const wxPoint& start)
	:mStartPoint(start)
	,mEndPoint(start)
	,mTopLeft(start)
	,mBotRight(start)
{
    mOffset.x = 0;
    mOffset.y = 0;
}

// Tests whether the provided point intersects
// with this shape
bool Shape::Intersects(const wxPoint& point) const
{
	wxPoint topleft;
	wxPoint botright;
	GetBounds(topleft, botright);
	if (point.x >= topleft.x && point.x <= botright.x &&
		point.y >= topleft.y && point.y <= botright.y)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Update shape with new provided point
void Shape::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;

	// For most shapes, we only have two points - start and end
	// So we can figure out the top left/bottom right bounds
	// based on this.
	mTopLeft.x = std::min(mStartPoint.x, mEndPoint.x);
	mTopLeft.y = std::min(mStartPoint.y, mEndPoint.y);
	mBotRight.x = std::max(mStartPoint.x, mEndPoint.x);
	mBotRight.y = std::max(mStartPoint.y, mEndPoint.y);
}

void Shape::Finalize()
{
	// Default finalize doesn't do anything
}

void Shape::GetBounds(wxPoint& topLeft, wxPoint& botRight) const
{
	topLeft = mTopLeft + mOffset;
	botRight = mBotRight + mOffset;
}

void Shape::DrawSelection(wxDC &dc)
{
    wxPoint topLeft = mTopLeft + mOffset;
    wxPoint botRight = mBotRight + mOffset;
    topLeft.x -= 2;
    topLeft.y -= 2;
    botRight.x += 2;
    botRight.y += 2;
    
    dc.SetPen(*wxBLACK_DASHED_PEN);
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    mSelectionRectangle = wxRect(topLeft, botRight);
    dc.DrawRectangle(mSelectionRectangle);
}
RectShape::RectShape(const wxPoint& start)
: Shape(start)
{
    
}

void RectShape::Draw(wxDC &dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    dc.DrawRectangle(wxRect(mTopLeft + mOffset, mBotRight + mOffset));
}

EllipseShape::EllipseShape(const wxPoint& start)
: Shape(start)
{

}

void EllipseShape::Draw(wxDC &dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    dc.DrawEllipse(wxRect(mTopLeft + mOffset, mBotRight + mOffset));
}

LineShape::LineShape(const wxPoint& start)
: Shape(start)
{
    
}

void LineShape::Draw(wxDC &dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    dc.DrawLine(mStartPoint + mOffset, mEndPoint + mOffset);
}

PencilShape::PencilShape(const wxPoint& point)
:Shape(point)
{
    mPoints.push_back(point);
}

void PencilShape::Update(const wxPoint &newPoint)
{
    Shape::Update(newPoint);
    mPoints.push_back(newPoint);
}

void PencilShape::Finalize()
{
    wxPoint topLeft = mPoints.front();
    wxPoint botRight = mPoints.front();
    for(auto& iter : mPoints)
    {
        if(iter.x < topLeft.x)
        {
            topLeft.x = iter.x;
        }
        if(iter.y < topLeft.y)
        {
            topLeft.y = iter.y;
        }
        if(iter.x > botRight.x)
        {
            botRight.x = iter.x;
        }
        if(iter.y > botRight.y)
        {
            botRight.y = iter.y;
        }
    }
    mTopLeft = topLeft;
    mBotRight = botRight;
}

void PencilShape::Draw(wxDC &dc) const
{
    dc.SetPen(mPen);
    dc.SetBrush(mBrush);
    if(mPoints.size() == 1)
    {
        dc.DrawPoint(mPoints[0]);
    }
    else {
        dc.DrawLines(static_cast<int>(mPoints.size()), mPoints.data(), mOffset.x, mOffset.y);
    }
}





