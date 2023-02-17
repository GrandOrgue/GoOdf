/*
 * GUIDivisional.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo
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

#include "GUIDivisional.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIDivisional::GUIDivisional(Divisional *divisional) : GUIButton(), m_divisional(divisional) {
	m_type = wxT("Divisional");
	m_displayAsPiston = true;
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
	GUIButton::write(outFile);
}

bool GUIDivisional::isReferencing(Divisional *divisional) {
	return m_divisional == divisional ? true : false;
}

void GUIDivisional::updateDisplayName() {
	setDisplayName(m_divisional->getName());
}
