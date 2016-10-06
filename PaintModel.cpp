#include "PaintModel.h"
#include <algorithm>
#include <wx/dcmemory.h>

PaintModel::PaintModel()
{
	
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
	// TODO
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
}

void PaintModel::UpdateCommand(wxPoint point)
{
    mActiveCommand->Update(point);
}

void PaintModel::FinalizeCommand()
{
    mActiveCommand->Finalize(shared_from_this());
    mActiveCommand = nullptr;
}
