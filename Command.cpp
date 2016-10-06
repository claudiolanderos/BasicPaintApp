#include "Command.h"
#include "Shape.h"
#include "PaintModel.h"

Command::Command(const wxPoint& start, std::shared_ptr<Shape> shape)
	:mStartPoint(start)
	,mEndPoint(start)
	,mShape(shape)
{

}

// Called when the command is still updating (such as in the process of drawing)
void Command::Update(const wxPoint& newPoint)
{
	mEndPoint = newPoint;
}

std::shared_ptr<Command> CommandFactory::Create(std::shared_ptr<PaintModel> model,
	CommandType type, const wxPoint& start)
{
	std::shared_ptr<Command> retVal = nullptr;
    std::shared_ptr<Shape> shape = nullptr;
    
    switch (type)
    {
        case CM_DrawLine:
            shape = std::make_shared<LineShape>(start);
            retVal = std::make_shared<DrawCommand>(start, shape);
            break;
            
        case CM_DrawEllipse:
            shape = std::make_shared<EllipseShape>(start);
            retVal = std::make_shared<DrawCommand>(start, shape);
            break;
            
        case CM_DrawRect:
            shape = std::make_shared<RectShape>(start);
            retVal = std::make_shared<DrawCommand>(start, shape);
            break;
            
        case CM_DrawPencil:
            shape = std::make_shared<PencilShape>(start);
            retVal = std::make_shared<DrawCommand>(start, shape);
            break;
            
        case CM_Move:
            break;
            
        case CM_Delete:
            break;
            
        case CM_SetPen:
            shape = model->GetSelectedShape();
            shape->SetOldPen(shape->GetPen());
            retVal = std::make_shared<PenBrushCommand>(start, shape);
            break;
            
        case CM_SetBrush:
            shape = model->GetSelectedShape();
            shape->SetOldBrush(shape->GetBrush());
            retVal = std::make_shared<PenBrushCommand>(start, shape);
            break;
    }
    
    shape->SetBrush(model->GetBrush());
    model->AddShape(shape);
	return retVal;
}

DrawCommand::DrawCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
    : Command(start, shape)
{
    
}

void DrawCommand::Update(const wxPoint &newPoint)
{
    Command::Update(newPoint);
    mShape->Update(newPoint);
}

void DrawCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->RemoveShape(mShape);
}

void DrawCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->AddShape(mShape);
}

void DrawCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->Finalize();
}

PenBrushCommand::PenBrushCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
: Command(start, shape)
{
    mOldPen = shape->GetOldPen();
    mOldBrush = shape->GetOldBrush();
    mNewPen = shape->GetPen();
    mNewBrush = shape->GetBrush();
}

void PenBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->SetPenColor(mOldPen.GetColour());
    model->SetPenWidth(mOldPen.GetWidth());
    model->SetBrushColor(mOldBrush.GetColour());
}

void PenBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->SetPenColor(mNewPen.GetColour());
    model->SetPenWidth(mNewPen.GetWidth());
    model->SetBrushColor(mNewBrush.GetColour());
}

void PenBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->Finalize();
}
