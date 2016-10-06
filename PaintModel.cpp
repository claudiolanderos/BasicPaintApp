#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
}

// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
    for(auto& iter : mShapes)
    {
        iter->Draw(dc);
    }
}

// Clear the current paint model and start fresh
void PaintModel::New()
{
    mActiveCommand.reset();
    while(!mRedo.empty())
    {
        mRedo.pop();
    }
    while(!mUndo.empty())
    {
        mUndo.pop();
    }
    mShapes.clear();
    mPen = *wxBLACK_PEN;
    mBrush = *wxWHITE_BRUSH;
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
	mShapes.emplace_back(shape);
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
	auto iter = std::find(mShapes.begin(), mShapes.end(), shape);
	if (iter != mShapes.end())
	{
		mShapes.erase(iter);
	}
}

// Returns true if there's currently an active command
bool PaintModel::HasActiveCommand()
{
    if(mActiveCommand != nullptr)
    {
        return true;
    }
    return false;
}

void PaintModel::CreateCommand(CommandType commandType, const wxPoint& start)
{
    mActiveCommand = CommandFactory::Create(shared_from_this(), commandType, start);
    while(!mRedo.empty())
    {
        mRedo.pop();
    }
}

void PaintModel::UpdateCommand(wxPoint point)
{
    mActiveCommand->Update(point);
}

void PaintModel::FinalizeCommand()
{
    mActiveCommand->Finalize(shared_from_this());
    mUndo.push(mActiveCommand);
    mActiveCommand = nullptr;
}

void PaintModel::Undo()
{
    if(CanUndo())
    {
        auto command = mUndo.top();
        mRedo.push(command);
        command->Undo(shared_from_this());
        mUndo.pop();
    }
}

void PaintModel::Redo()
{
    if(CanRedo())
    {
        auto command = mRedo.top();
        mUndo.push(command);
        command->Redo(shared_from_this());
        mRedo.pop();
    }
}
