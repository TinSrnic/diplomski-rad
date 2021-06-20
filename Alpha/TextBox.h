#pragma once
#include "BoundedGUI.h"
#include "Text.h"

class TextBox : public BoundedGUI {

private:
	Text m_text;
	bool m_isWriting;

public:
	TextBox(const BoundedGUI& t_boundedGUI, const Text& t_text) 
		: BoundedGUI(t_boundedGUI), m_text(t_text) { }

	Text& GetText() { return m_text; }

	void SetWriting() { m_isWriting = true; }
	void ResetWriting() { m_isWriting = false; }
};
