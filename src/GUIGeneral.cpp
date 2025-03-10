/*
 * GUIGeneral.cpp is part of GoOdf.
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

#include "GUIGeneral.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIGeneral::GUIGeneral(General *general) : GUIButton(), m_general(general) {
	m_type = wxT("General");
	setDisplayAsPiston(true);
}

GUIGeneral::~GUIGeneral() {

}

void GUIGeneral::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	if (m_general) {
		int generalNbr = ::wxGetApp().m_frame->m_organ->getIndexOfOrganGeneral(m_general);
		wxString generalId = wxT("General=") + GOODF_functions::number_format(generalNbr);
		outFile->AddLine(generalId);
	}
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

GUIGeneral* GUIGeneral::clone() {
	return new GUIGeneral(*this);
}

bool GUIGeneral::isReferencing(General *general) {
	return m_general == general ? true : false;
}

void GUIGeneral::updateDisplayName() {
	if (m_general)
		setDisplayName(m_general->getName() + wxT(" (General)"));
}

wxString GUIGeneral::getElementName() {
	if (m_general)
		return m_general->getName();
	else
		return getDisplayName();
}
