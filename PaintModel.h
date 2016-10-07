#pragma once
#include <memory>
#include <vector>
#include "Shape.h"
#include "Command.h"
#include <wx/bitmap.h>
#include <stack>

class PaintModel : public std::enable_shared_from_this<PaintModel>
{
public:
	PaintModel();
	
	// Draws any shapes in the model to the provided DC (draw context)
	void DrawShapes(wxDC& dc, bool showSelection = true);

	// Clear the current paint model and start fresh
	void New();

	// Add a shape to the paint model
	void AddShape(std::shared_ptr<Shape> shape);
	// Remove a shape from the paint model
	void RemoveShape(std::shared_ptr<Shape> shape);
    
    bool HasActiveCommand();
    
    void CreateCommand(CommandType commandType, const wxPoint& start);
    
    void UpdateCommand(wxPoint point);
    
    void FinalizeCommand();
    
    bool CanUndo() { return !mUndo.empty(); }
    
    bool CanRedo() { return !mRedo.empty(); }
    // Undo command
    void Undo();
    // Redo command
    void Redo();
    
    void SetPenWidth(int width) { mPen.SetWidth(width); }
    
    int GetPenWidth() { return mPen.GetWidth(); }
    
    void SetPenColor(wxColour color) { mPen.SetColour(color); }
    
    wxColour GetPenColor() { return mPen.GetColour(); }
    
    void SetBrushColor(wxColour color) { mBrush.SetColour(color); }
    
    wxColour GetBrushColor() { return mBrush.GetColour(); }
    
    wxPen GetPen() { return mPen; }
    wxPen GetOldPen() { return mOldPen; }
    
    wxBrush GetBrush() { return mBrush; }
    wxBrush GetOldBrush() { return mOldBrush; }
    
    void SelectShape(wxPoint point);
    
    std::shared_ptr<Shape> GetSelectedShape() { return mSelectedShape; }
    
    void SetPenCommand();
    
    void SetBrushCommand();

private:
	// Vector of all the shapes in the model
	std::vector<std::shared_ptr<Shape>> mShapes;
    //Shared pointer to active commands
    std::shared_ptr<Command> mActiveCommand;
    // Undo stack
    std::stack<std::shared_ptr<Command>> mUndo;
    // Redo stack
    std::stack<std::shared_ptr<Command>> mRedo;
    // Pen
    wxPen mPen;
    wxPen mOldPen;
    // Brush
    wxBrush mBrush;
    wxBrush mOldBrush;
    // Selected shape
    std::shared_ptr<Shape> mSelectedShape;
};
