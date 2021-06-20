#include "Input.h"

bool Input::m_keyPress[1024] = { false };
bool Input::m_keyHold[1024] = { false };
bool Input::m_keyRelease[1024] = { true };
bool Input::m_previousFrameKeyPress[1024] = { false };

bool Input::m_mousePress[32] = { false };

Vec2 Input::m_mousePosition = Vec2(0.0f);

Vec2 Input::m_mouseScroll = Vec2(0.0f);