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
    
	// TODO: Actually create a command based on the type passed in
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
            break;
        case CM_SetPen:
            break;
        case CM_SetBrush:
            break;
    }
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
