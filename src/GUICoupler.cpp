/*
 * GUICoupler.cpp is part of GoOdf.
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

#include "GUICoupler.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUICoupler::GUICoupler(Coupler *cplr) : GUIButton(), m_coupler(cplr) {
	m_type = wxT("Coupler");
}

GUICoupler::~GUICoupler() {

}

void GUICoupler::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	unsigned manualNbr = ::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_coupler->getOwningManual());
	wxString manId = wxT("Manual=") + GOODF_functions::number_format(manualNbr);
	outFile->AddLine(manId);
	int couplerNbr = m_coupler->getOwningManual()->getIndexOfCoupler(m_coupler) + 1;
	wxString couplerId = wxT("Coupler=") + GOODF_functions::number_format(couplerNbr);
	outFile->AddLine(couplerId);
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

GUICoupler* GUICoupler::clone() {
	return new GUICoupler(*this);
}

bool GUICoupler::isReferencing(Coupler *cplr) {
	return m_coupler == cplr ? true : false;
}

void GUICoupler::updateDisplayName() {
	setDisplayName(m_coupler->getName() + wxT(" (Coupler in ") + m_coupler->getOwningManual()->getName() + wxT(")"));
}

wxString GUICoupler::getElementName() {
	return m_coupler->getName();
}
