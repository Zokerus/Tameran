#include "ControlManager.h"

Hydro::ControlManager::ControlManager()
	: m_control(), m_selectedControl(0), m_focusChanged()
{}

Hydro::ControlManager::~ControlManager()
{}

void Hydro::ControlManager::Shutdown()
{
	if (m_control.size() > 0)
	{
		m_control.clear();
	}
}

bool Hydro::ControlManager::Update(float eTime, Input * input)
{
	int size = (int)m_control.size();

	for (int i = 0; i < size; i++)
	{
		if (m_control[i]->IsEnable())
		{
			m_control[i]->HandleInput(input);
		}
		if (!m_control[i]->Update(eTime))
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
	int size = (int)m_control.size();

	for (int i = 0; i < size; i++)
	{
		if (m_control[i]->IsVisible())
		{
			if (!m_control[i]->Draw(eTime, deviceContext, shaderManager, worldMatrix, viewMatrix, orthoMatrix))
			{
				return false;
			}
		}
	}
	return true;
}

int Hydro::ControlManager::ControlCount()
{
	return (int)m_control.size();
}

void Hydro::ControlManager::AddControl(IControl* control)
{
	//Add the newest control at the last position(index)
	m_control.push_back(control);
}

void Hydro::ControlManager::RemoveControl()
{
	//Remove the last control of the vector (newest one)
	m_control.pop_back();
}

void Hydro::ControlManager::NextControl()
{
	if (m_control.size() <= 0)
	{
		return;
	}

	int size = (int)m_control.size();		//current number of controls
	int currentControl = m_selectedControl;	//index of selected control
	m_control[m_selectedControl]->SetFocus(false);	//defocus selected control

	do
	{
		m_selectedControl++;

		if (m_selectedControl >= size)  //Close the loops between both ends of the array
		{
			m_selectedControl = 0;
		}

		if (m_control[m_selectedControl]->GetTabStop() && m_control[m_selectedControl]->IsEnable() && m_control[m_selectedControl]->IsVisible())
		{
			if (m_focusChanged != nullptr)
			{
				m_focusChanged(m_control[m_selectedControl]); //the control got the focus and the event was fired
			}
			break;
		}
	} while (currentControl != m_selectedControl);

	m_control[m_selectedControl]->SetFocus(true);
}

void Hydro::ControlManager::PrevControl()
{
	if (m_control.size() <= 0)
	{
		return;
	}

	int size = (int)m_control.size();		//current number of controls
	int currentControl = m_selectedControl;	//index of selected control
	m_control[m_selectedControl]->SetFocus(false);	//defocus selected control

	do
	{
		m_selectedControl--;

		if (m_selectedControl < 0)  //Close the loops between both ends of the array
		{
			m_selectedControl = size - 1;
		}

		if (m_control[m_selectedControl]->GetTabStop() && m_control[m_selectedControl]->IsEnable() && m_control[m_selectedControl]->IsVisible())
		{
			if (m_focusChanged != nullptr)
			{
				m_focusChanged(m_control[m_selectedControl]); //the control got the focus and the event was fired
			}
			break;
		}
	} while (currentControl != m_selectedControl);

	m_control[m_selectedControl]->SetFocus(true);
}

void Hydro::ControlManager::SetFocusChangeEvent(std::function<void(const IControl*)> func)
{
	m_focusChanged = func;
}