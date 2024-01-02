/*
 * GUIReversiblePiston.cpp is part of GOODF.
 * Copyright (C) 2024 Lars Palo and contributors (see AUTHORS)
 *
 * GOODF is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOODF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GOODF.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#include "GUIReversiblePiston.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIReversiblePiston::GUIReversiblePiston(ReversiblePiston *reversiblePiston) : GUIButton(), m_reversiblePiston(reversiblePiston) {
	m_type = wxT("ReversiblePiston");
	m_displayAsPiston = true;
}

GUIReversiblePiston::~GUIReversiblePiston() {

}

void GUIReversiblePiston::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	int reversiblePistonNbr = ::wxGetApp().m_frame->m_organ->getIndexOfReversiblePiston(m_reversiblePiston);
	wxString divId = wxT("ReversiblePiston=") + GOODF_functions::number_format(reversiblePistonNbr);
	outFile->AddLine(divId);
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

bool GUIReversiblePiston::isReferencing(ReversiblePiston *reversiblePiston) {
	return m_reversiblePiston == reversiblePiston ? true : false;
}

void GUIReversiblePiston::updateDisplayName() {
	setDisplayName(m_reversiblePiston->getName());
}
