#pragma once
#include <wx/dc.h>

// Abstract base class for all Shapes
class Shape
{
public:
	Shape(const wxPoint& start);
	// Tests whether the provided point intersects
	// with this shape
	bool Intersects(const wxPoint& point) const;
	// Update shape with new provided point
	virtual void Update(const wxPoint& newPoint);
	// Finalize the shape -- when the user has finished drawing the shape
	virtual void Finalize();
	// Returns the top left/bottom right points of the shape
	void GetBounds(wxPoint& topLeft, wxPoint& botRight) const;
	// Draw the shape
	virtual void Draw(wxDC& dc) const = 0;
	virtual ~Shape() { }
    
    void SetPen(wxPen pen) { mPen = pen; }
    
    wxPen GetPen() { return mPen; }
    
    void SetBrush(wxBrush brush) { mBrush = brush; }
    
    wxBrush GetBrush() { return mBrush; }
    
    wxRect GetSelectionRectangle() { return mSelectionRectangle; }
    
    void DrawSelection(wxDC &dc);
    
    void SetOffset(wxPoint offset) { mOffset = offset; }
protected:
	// Starting point of shape
	wxPoint mStartPoint;
	// Ending point of shape
	wxPoint mEndPoint;
	// Top left point of shape
	wxPoint mTopLeft;
	// Bottom right point of shape
	wxPoint mBotRight;
    // Pen
    wxPen mPen;
    // Brush
    wxBrush mBrush;
    // Rect for selection
    wxRect mSelectionRectangle;
    // Offset point
    wxPoint mOffset;
};

class RectShape : public Shape
{
public:
    RectShape(const wxPoint& start);
    
    //Draw the shape
    void Draw(wxDC& dc) const override;
};

class EllipseShape : public Shape
{
public:
    EllipseShape(const wxPoint& start);
    
    //Draw the shape
    void Draw(wxDC& dc) const override;
};

class LineShape : public Shape
{
public:
    LineShape(const wxPoint& start);
    
    //Draw the line
    void Draw(wxDC& dc) const override;
};

class PencilShape : public Shape
{
public:
    PencilShape(const wxPoint& point);
    
    void Update(const wxPoint& newPoint) override;
    
    void Finalize() override;
    
    void Draw(wxDC& dc) const override;
private:
    std::vector<wxPoint> mPoints;
};
