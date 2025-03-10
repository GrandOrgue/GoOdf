/*
 * GUIElements.h is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include "GoColor.h"
#include "GoFontSize.h"
#include "GoImage.h"
#include "Manual.h"

class GoPanel;

class GUIElement {
public:
	GUIElement();
	virtual ~GUIElement();

	virtual void write(wxTextFile *outFile);
	virtual void read(wxFileConfig *cfg);

	virtual GUIElement* clone();
	virtual void updateDisplayName();
	virtual void setDefaultFont(wxFont&);
	virtual wxBitmap getBitmap();
	virtual wxString getElementName();
	wxString getType();
	void setType(wxString type);
	int getPosX();
	void setPosX(int x);
	int getPosY();
	void setPosY(int y);
	void setOwningPanel(GoPanel *panel);
	GoPanel* getOwningPanel();
	void setDisplayName(wxString name);
	wxString getDisplayName();

protected:
	wxString m_type;
	int m_positionX;
	int m_positionY;

private:
	GoPanel *m_owningPanel;
	wxString m_displayName;
};

#endif
