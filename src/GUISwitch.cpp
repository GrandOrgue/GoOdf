/*
 * GUIGoSwitch.cpp is part of GoOdf.
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

#include "GUISwitch.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUISwitch::GUISwitch(GoSwitch *sw) : GUIButton(), m_switch(sw) {
	m_type = wxT("Switch");
}

GUISwitch::~GUISwitch() {

}

void GUISwitch::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	if (m_switch) {
		int switchNbr = ::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(m_switch);
		wxString swId = wxT("Switch=") + GOODF_functions::number_format(switchNbr);
		outFile->AddLine(swId);
	}
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

void GUISwitch::read(wxFileConfig *cfg, Organ *readOrgan) {
	GUIButton::read(cfg, false, readOrgan);
}

GUISwitch* GUISwitch::clone() {
	return new GUISwitch(*this);
}

bool GUISwitch::isReferencing(GoSwitch *sw) {
	return m_switch == sw ? true : false;
}

void GUISwitch::updateDisplayName() {
	if (m_switch)
		setDisplayName(m_switch->getName() + wxT(" (Switch[") + GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganSwitch(m_switch)) + wxT("])"));
}

wxString GUISwitch::getElementName() {
	if (m_switch)
		return m_switch->getName();
	else
		return getDisplayName();
}
