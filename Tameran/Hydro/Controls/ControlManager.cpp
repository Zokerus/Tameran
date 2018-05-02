#include "ControlManager.h"

Hydro::ControlManager::ControlManager()
	: control(), selectedControl(0), focusChanged()
{}

Hydro::ControlManager::~ControlManager()
{
	if (control.size() > 0)
	{
		control.clear();
	}
}

bool Hydro::ControlManager::Update(float eTime, Input* input)
{
	int size = (int)control.size();

	for (int i = 0; i < size; i++)
	{
		if (control[i]->IsEnable())
		{
			control[i]->HandleInput(input);
		}
		if (!control[i]->Update(eTime))
		{
			return false;
		}
	}

	if (input->IsKeyPressed(DIK_UP))
	{
		PrevControl();
	}
	else if (input->IsKeyPressed(DIK_DOWN))
	{
		NextControl();
	}
	return true;
}

bool Hydro::ControlManager::Draw(float eTime, ID3D11DeviceContext* deviceContext, ShaderManager* shaderManager, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX orthoMatrix)
{
	int size = (int)control.size();

	for (int i = 0; i < size; i++)
	{
		if (control[i]->IsVisible())
		{
			if (!control[i]->Draw(eTime, deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix))
			{
				return false;
			}
		}
	}
	return true;
}

int Hydro::ControlManager::ControlCount()
{
	return (int)control.size();
}

void Hydro::ControlManager::AddControl(IControl* Control)
{
	//Add the newest control at the last position(index)
	control.push_back(Control);
}

void Hydro::ControlManager::RemoveControl()
{
	//Remove the last control of the vector (newest one)
	control.pop_back();
}

void Hydro::ControlManager::NextControl()
{
	if (control.size() <= 0)
	{
		return;
	}

	int size = (int)control.size();		//current number of controls
	int currentControl = selectedControl;	//index of selected control
	control[selectedControl]->SetFocus(false);	//defocus selected control

	do
	{
		selectedControl++;

		if (selectedControl >= size)  //Close the loops between both ends of the array
		{
			selectedControl = 0;
		}

		if (control[selectedControl]->GetTabStop() && control[selectedControl]->IsEnable() && control[selectedControl]->IsVisible())
		{
			if (focusChanged != nullptr)
			{
				focusChanged(control[selectedControl]); //the control got the focus and the event was fired
			}
			break;
		}
	} while (currentControl != selectedControl);

	control[selectedControl]->SetFocus(true);
}

void Hydro::ControlManager::PrevControl()
{
	if (control.size() <= 0)
	{
		return;
	}

	int size = (int)control.size();		//current number of controls
	int currentControl = selectedControl;	//index of selected control
	control[selectedControl]->SetFocus(false);	//defocus selected control

	do
	{
		selectedControl--;

		if (selectedControl < 0)  //Close the loops between both ends of the array
		{
			selectedControl = size - 1;
		}

		if (control[selectedControl]->GetTabStop() && control[selectedControl]->IsEnable() && control[selectedControl]->IsVisible())
		{
			if (focusChanged != nullptr)
			{
				focusChanged(control[selectedControl]); //the control got the focus and the event was fired
			}
			break;
		}
	} while (currentControl != selectedControl);

	control[selectedControl]->SetFocus(true);
}

void Hydro::ControlManager::SetFocusChangeEvent(std::function<void(const IControl*)> func)
{
	focusChanged = func;
}