#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
    mPen = *wxBLACK_PEN;
    mOldPen = mPen;
    mBrush = *wxWHITE_BRUSH;
    mOldBrush = mBrush;
}

void PaintModel::LoadBitmap(wxString filename, wxBitmapType type)
{
    New();
    mBitmap.LoadFile(filename, type);
}
// Draws any shapes in the model to the provided DC (draw context)
void PaintModel::DrawShapes(wxDC& dc, bool showSelection)
{
    if(mBitmap.IsOk())
    {
        dc.DrawBitmap(mBitmap, wxPoint(0,0));
    }
    for(auto& iter : mShapes)
    {
        iter->Draw(dc);
    }
    if(mSelectedShape != nullptr)
    {
        mSelectedShape->DrawSelection(dc);
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
    mOldPen = mPen;
    mBrush = *wxWHITE_BRUSH;
    mOldBrush = mBrush;
    mSelectedShape.reset();
    mBitmap = wxBitmap();
}

// Add a shape to the paint model
void PaintModel::AddShape(std::shared_ptr<Shape> shape)
{
    UnSelectShape();
    if (std::find(mShapes.begin(), mShapes.end(), shape) == mShapes.end())
    {
        mShapes.emplace_back(shape);
    }
}

// Remove a shape from the paint model
void PaintModel::RemoveShape(std::shared_ptr<Shape> shape)
{
    UnSelectShape();
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

void PaintModel::DeleteCommand()
{
    if(mSelectedShape != nullptr)
    {
        CreateCommand(CM_Delete, wxPoint());
        FinalizeCommand();
    }
}

void PaintModel::SetPenCommand()
{
    if(mSelectedShape != nullptr)
    {
        CreateCommand(CM_SetPen, wxPoint());
        FinalizeCommand();
    }
}

void PaintModel::SetBrushCommand()
{
    if(mSelectedShape != nullptr)
    {
        CreateCommand(CM_SetBrush, wxPoint());
        FinalizeCommand();
    }
}

void PaintModel::Undo()
{
    if(CanUndo())
    {
        auto command = mUndo.top();
        command->Undo(shared_from_this());
        mRedo.push(command);
        mUndo.pop();
    }
}

void PaintModel::Redo()
{
    if(CanRedo())
    {
        auto command = mRedo.top();
        command->Redo(shared_from_this());
        mUndo.push(command);
        mRedo.pop();
    }
}

void PaintModel::SelectShape(wxPoint point)
{
    for(auto iter = mShapes.rbegin(); iter != mShapes.rend(); iter++)
    {
        if((*iter)->Intersects(point))
        {
            mSelectedShape = *iter;
            break;
        }
    }
}

void PaintModel::UnSelectShape()
{
    mSelectedShape.reset();
}
