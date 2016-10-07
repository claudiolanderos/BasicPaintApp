#pragma once
#include <wx/gdicmn.h>
#include <wx/pen.h>
#include <wx/brush.h>
#include <memory>

enum CommandType
{
	CM_DrawLine,
	CM_DrawEllipse,
	CM_DrawRect,
	CM_DrawPencil,
	CM_Move,
	CM_Delete,
	CM_SetPen,
	CM_SetBrush,
};

// Forward declarations
class PaintModel;
class Shape;

// Abstract Base Command class
// All actions that change the drawing (drawing, deleting, etc., are commands)
class Command
{
public:
	Command(const wxPoint& start, std::shared_ptr<Shape> shape);
	// Called when the command is still updating (such as in the process of drawing)
	virtual void Update(const wxPoint& newPoint);
	// Called when the command is completed
	virtual void Finalize(std::shared_ptr<PaintModel> model) = 0;
	// Used to "undo" the command
	virtual void Undo(std::shared_ptr<PaintModel> model) = 0;
	// Used to "redo" the command
	virtual void Redo(std::shared_ptr<PaintModel> model) = 0;
    
    std::shared_ptr<Shape> GetShape() { return mShape; }
    
    void SetShape(std::shared_ptr<Shape> shape) { mShape = shape; }
    
	virtual ~Command() { }
protected:
	wxPoint mStartPoint;
	wxPoint mEndPoint;
	std::shared_ptr<Shape> mShape;
};

// Factory method to help create a particular command
struct CommandFactory
{
	static std::shared_ptr<Command> Create(std::shared_ptr<PaintModel> model,
		CommandType type, const wxPoint& start);
};

class DrawCommand : public Command
{
public:
    DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
    
    void Finalize(std::shared_ptr<PaintModel> model) override;
    
    void Undo(std::shared_ptr<PaintModel> model) override;
    
    void Redo(std::shared_ptr<PaintModel> model) override;
    
    void Update(const wxPoint& newPoint) override;
};

class PenBrushCommand : public Command
{
public:
    PenBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
    
    void Finalize(std::shared_ptr<PaintModel> model) override;
    
    void Undo(std::shared_ptr<PaintModel> model) override;
    
    void Redo(std::shared_ptr<PaintModel> model) override;
    
    void SetNewPen(wxPen pen) { mNewPen = pen; }
    
    void SetOldPen(wxPen pen) { mOldPen = pen; }
    
    void SetNewBrush(wxBrush brush) { mNewBrush = brush; }
    
    void SetOldBrush(wxBrush brush) { mOldBrush = brush; }
private:
    wxPen mOldPen;
    wxPen mNewPen;
    wxBrush mOldBrush;
    wxBrush mNewBrush;
};

class DeleteCommand : public Command
{
public:
    DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
    
    void Finalize(std::shared_ptr<PaintModel> model) override;
    
    void Undo(std::shared_ptr<PaintModel> model) override;
    
    void Redo(std::shared_ptr<PaintModel> model) override;
};

class MoveCommand : public Command
{
public:
    MoveCommand(const wxPoint& start, std::shared_ptr<Shape> shape);
    
    void Update(const wxPoint& newPoint) override;
    
    void Finalize(std::shared_ptr<PaintModel> model) override;
    
    void Undo(std::shared_ptr<PaintModel> model) override;
    
    void Redo(std::shared_ptr<PaintModel> model) override;
};

