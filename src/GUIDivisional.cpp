/*
 * GUIDivisional.cpp is part of GoOdf.
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

#include "GUIDivisional.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIDivisional::GUIDivisional(Divisional *divisional) : GUIButton(), m_divisional(divisional) {
	m_type = wxT("Divisional");
	setDisplayAsPiston(true);
}

GUIDivisional::~GUIDivisional() {

}

void GUIDivisional::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	if (m_divisional) {
		unsigned manualNbr = ::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_divisional->getOwningManual());
		wxString manId = wxT("Manual=") + GOODF_functions::number_format(manualNbr);
		outFile->AddLine(manId);
		int divisionalNbr = m_divisional->getOwningManual()->getIndexOfDivisional(m_divisional) + 1;
		wxString divId = wxT("Divisional=") + GOODF_functions::number_format(divisionalNbr);
		outFile->AddLine(divId);
	}
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

GUIDivisional* GUIDivisional::clone() {
	return new GUIDivisional(*this);
}

bool GUIDivisional::isReferencing(Divisional *divisional) {
	return m_divisional == divisional ? true : false;
}

void GUIDivisional::updateDisplayName() {
	if (m_divisional)
		setDisplayName(m_divisional->getName() + wxT(" (Divisional in ") + m_divisional->getOwningManual()->getName() + wxT(")"));
}

wxString GUIDivisional::getElementName() {
	if (m_divisional)
		return m_divisional->getName();
	else
		return getDisplayName();
}
