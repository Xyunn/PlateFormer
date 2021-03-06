#include "Character.h"


Character::Character()
{
	m_shadowUse = 0;
	m_orientation = 1;
	m_jump = 0;
	m_jumpMax = 200;
	m_stateMachine = new StateMachine_Character();
	m_chronoTrigger = 0;
	m_player1 = new CXBOXController(1);
}

Character::~Character()
{
	delete(m_stateMachine);
	delete(m_player1);
}

void Character::setX(int x)
{
	m_x = x;
}

void Character::setY(int y)
{
	m_y = y;
}

void Character::setCenter()
{
	int width = CHARACTER_WIDTH;
	int height = CHARACTER_HEIGHT;
	m_center.x = m_x + width / 2;
	m_center.y = m_y + height / 2;
}

void Character::setChronoTrigger()
{
	if (m_chronoTrigger == 0)
	{
		m_chronoTrigger = 1;
	}
	else
		m_chronoTrigger = 0;
}

void Character::setJump(int jump)
{
	m_jump = jump;
}

void Character::setXOrientation(int xOrientation)
{
	m_orientation = xOrientation;
}

void Character::setYOrientation(int yOrientation)
{
	m_yOrientation = yOrientation;
}

void Character::setShadowUse(int shadowUse)
{
	m_shadowUse = shadowUse;
}

int Character::getX()
{
	return m_x;
}

int Character::getY()
{
	return m_y;
}

int Character::getChronoTrigger()
{
	return m_chronoTrigger;
}

coordonnee Character::getCenter()
{
	return m_center;
}

int Character::getJump()
{
	return m_jump;
}

int Character::getShadowUse()
{
	return m_shadowUse;
}

void Character::move()
{
	switch (m_stateMachine->getIdCurrentState())
	{
	case RUN :
		m_x += m_orientation * MOVE_VELOCITY;
		break;

	case JUMP :
		m_yOrientation = -1;
		if (m_jump < m_jumpMax)
		{
			m_y -= JUMP_VELOCITY;
			m_jump += JUMP_VELOCITY;
		}

		if (m_jump >= m_jumpMax)
		{
			m_y += abs(m_jumpMax - m_jump);
			m_jump = m_jumpMax;
			m_stateMachine->changeState(FLY);
		}
		break;

	case FLY :
		m_yOrientation = 1;
		/*if (m_jump > 0)
		{
			m_y += FALL_VELOCITY;
			m_jump -= FALL_VELOCITY;
		}

		if (m_jump < 0)
		{
			m_y += m_jump;			
			m_jump = 0;
			m_stateMachine->changeState(LAND);
		}*/
		m_y += FALL_VELOCITY;
		break;


	case LAND :
		m_jump = 0;
		m_yOrientation = 0;
		break;

	default :
		break;
	}
	//cout << m_center.y << endl;
	//cout << m_yOrientation << endl;
	setCenter();
}

void Character::jumpControl()
{
	m_x += m_orientation * MOVE_VELOCITY;
}

void Character::processEvents(Event event)
{
	m_state = m_player1->GetState();

	if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape || event.type == Event::KeyPressed && event.key.code == Keyboard::Up || (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_A) )
	{
		m_stateMachine->changeState(JUMP);
	}

	if ((event.type == Event::KeyPressed && event.key.code == Keyboard::LShift) || (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_X))
	{
		m_shadowUse = 1;
	}

	if (((event.type == Event::KeyPressed && event.key.code == Keyboard::Space) || (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_Y)) && m_shadowUse == 2)
	{
		m_shadowUse = 3;
	}
}

void Character::processEvents()
{
	m_state = m_player1->GetState();

	float gamePadDirection = 0;
	float LX = m_state.Gamepad.sThumbLX;	
	float magnitude = sqrt(LX * LX + LX * LX);
	float normalizedLX = LX / magnitude;

	if (magnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		if (normalizedLX < 0)
			gamePadDirection = -1;
		else
			gamePadDirection = 1;

	if (Keyboard::isKeyPressed(Keyboard::Right) || gamePadDirection == 1)
	{
		m_stateMachine->changeState(RUN);
		m_orientation = 1;
		if (m_stateMachine->getIdCurrentState() == JUMP || m_stateMachine->getIdCurrentState() == FLY)
		{
			jumpControl();
		}			
	}

	if (Keyboard::isKeyPressed(Keyboard::Left) ||gamePadDirection == -1)
	{
		m_stateMachine->changeState(RUN);
		m_orientation = -1;		
		if (m_stateMachine->getIdCurrentState() == JUMP || m_stateMachine->getIdCurrentState() == FLY)
		{
			jumpControl();
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::LControl) || (m_state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) )
	{		
		setChronoTrigger();
	}

	/*if (Keyboard::isKeyPressed(Keyboard::Space) || Keyboard::isKeyPressed(Keyboard::Up))
	{
		m_stateMachine->changeState(JUMP);
	}*/
}

StateMachine_Character * Character::getStateMachine()
{
	return m_stateMachine;
}

int Character::getOrientation()
{
	return m_orientation;
}

int Character::getYOrientation()
{
	return m_yOrientation;
}