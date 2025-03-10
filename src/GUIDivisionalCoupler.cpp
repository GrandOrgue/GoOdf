/*
 * GUIDivisionalCoupler.cpp is part of GoOdf.
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

#include "GUIDivisionalCoupler.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIDivisionalCoupler::GUIDivisionalCoupler(DivisionalCoupler *divCplr) : GUIButton(), m_divCoupler(divCplr) {
	m_type = wxT("DivisionalCoupler");
}

GUIDivisionalCoupler::~GUIDivisionalCoupler() {

}

void GUIDivisionalCoupler::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	int divCplrNbr = ::wxGetApp().m_frame->m_organ->getIndexOfOrganDivisionalCoupler(m_divCoupler);
	wxString divId = wxT("DivisionalCoupler=") + GOODF_functions::number_format(divCplrNbr);
	outFile->AddLine(divId);
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

GUIDivisionalCoupler* GUIDivisionalCoupler::clone() {
	return new GUIDivisionalCoupler(*this);
}

bool GUIDivisionalCoupler::isReferencing(DivisionalCoupler *divCplr) {
	return m_divCoupler == divCplr ? true : false;
}

void GUIDivisionalCoupler::updateDisplayName() {
	setDisplayName(m_divCoupler->getName() + wxT(" (Divisional coupler)"));
}

wxString GUIDivisionalCoupler::getElementName() {
	return m_divCoupler->getName();
}
