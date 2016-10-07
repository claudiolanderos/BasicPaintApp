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
            model->AddShape(shape);
            break;
            
        case CM_DrawEllipse:
            shape = std::make_shared<EllipseShape>(start);
            retVal = std::make_shared<DrawCommand>(start, shape);
            model->AddShape(shape);
            break;
            
        case CM_DrawRect:
            shape = std::make_shared<RectShape>(start);
            retVal = std::make_shared<DrawCommand>(start, shape);
            model->AddShape(shape);
            break;
            
        case CM_DrawPencil:
            shape = std::make_shared<PencilShape>(start);
            retVal = std::make_shared<DrawCommand>(start, shape);
            model->AddShape(shape);
            break;
            
        case CM_Move:
            break;
            
        case CM_Delete:
            shape = model->GetSelectedShape();
            retVal = std::make_shared<DeleteCommand>(start, shape);
            retVal->SetShape(shape);
            model->RemoveShape(shape);
            break;
            
        case CM_SetPen:
        case CM_SetBrush:
            shape = model->GetSelectedShape();
            retVal = std::make_shared<PenBrushCommand>(start, shape);
            std::dynamic_pointer_cast<PenBrushCommand>(retVal)->SetOldPen(shape->GetPen());
            std::dynamic_pointer_cast<PenBrushCommand>(retVal)->SetNewPen(model->GetPen());
            std::dynamic_pointer_cast<PenBrushCommand>(retVal)->SetOldBrush(shape->GetBrush());
            std::dynamic_pointer_cast<PenBrushCommand>(retVal)->SetNewBrush(model->GetBrush());
            std::dynamic_pointer_cast<PenBrushCommand>(retVal)->SetShape(shape);
            break;
    }
    
    shape->SetPen(model->GetPen());
    shape->SetBrush(model->GetBrush());
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
    if(!mOldPen.IsOk())
    {
        mOldPen = *wxBLACK_PEN;
    }
    if(!mOldBrush.IsOk())
    {
        mOldBrush = *wxBLACK_BRUSH;
    }
    
    mNewPen = shape->GetPen();
    mNewBrush = shape->GetBrush();
}

void PenBrushCommand::Undo(std::shared_ptr<PaintModel> model)
{
    mShape->SetPen(mOldPen);
    mShape->SetBrush(mOldBrush);
    model->SetPenColor(mOldPen.GetColour());
    model->SetPenWidth(mOldPen.GetWidth());
    model->SetBrushColor(mOldBrush.GetColour());
}

void PenBrushCommand::Redo(std::shared_ptr<PaintModel> model)
{
    mShape->SetPen(mNewPen);
    mShape->SetBrush(mNewBrush);
    model->SetPenColor(mNewPen.GetColour());
    model->SetPenWidth(mNewPen.GetWidth());
    model->SetBrushColor(mNewBrush.GetColour());
}

void PenBrushCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->Finalize();
}

DeleteCommand::DeleteCommand(const wxPoint& start, std::shared_ptr<Shape> shape)
: Command(start, shape)
{

}


void DeleteCommand::Finalize(std::shared_ptr<PaintModel> model)
{
    mShape->Finalize();
}

void DeleteCommand::Undo(std::shared_ptr<PaintModel> model)
{
    model->AddShape(mShape);
}

void DeleteCommand::Redo(std::shared_ptr<PaintModel> model)
{
    model->RemoveShape(mShape);
}
