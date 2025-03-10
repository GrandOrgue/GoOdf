/*
 * GUIStop.cpp is part of GoOdf.
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

#include "GUIStop.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIStop::GUIStop(Stop *stop) : GUIButton(), m_stop(stop) {
	m_type = wxT("Stop");
}

GUIStop::~GUIStop() {

}

void GUIStop::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	unsigned manualNbr = ::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_stop->getOwningManual());
	wxString manId = wxT("Manual=") + GOODF_functions::number_format(manualNbr);
	outFile->AddLine(manId);
	int stopNbr = m_stop->getOwningManual()->getIndexOfStop(m_stop) + 1;
	wxString stopId = wxT("Stop=") + GOODF_functions::number_format(stopNbr);
	outFile->AddLine(stopId);
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

GUIStop* GUIStop::clone() {
	return new GUIStop(*this);
}

bool GUIStop::isReferencing(Stop *stop) {
	return m_stop == stop ? true : false;
}

void GUIStop::updateDisplayName() {
	setDisplayName(m_stop->getName() + wxT(" (Stop in ") + m_stop->getOwningManual()->getName() + wxT(")"));
}

wxString GUIStop::getElementName() {
	return m_stop->getName();
}
