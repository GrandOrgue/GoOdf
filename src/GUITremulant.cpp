/*
 * GUITremulant.cpp is part of GoOdf.
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

#include "GUITremulant.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUITremulant::GUITremulant(Tremulant *tremulant) : GUIButton(), m_tremulant(tremulant) {
	m_type = wxT("Tremulant");
}

GUITremulant::~GUITremulant() {

}

void GUITremulant::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	int tremulantNbr = ::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(m_tremulant);
	wxString tremId = wxT("Tremulant=") + GOODF_functions::number_format(tremulantNbr);
	outFile->AddLine(tremId);
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	GUIButton::write(outFile);
}

void GUITremulant::read(wxFileConfig *cfg, Organ *readOrgan) {
	GUIButton::read(cfg, false, readOrgan);
}

GUITremulant* GUITremulant::clone() {
	return new GUITremulant(*this);
}

bool GUITremulant::isReferencing(Tremulant *tremulant) {
	return m_tremulant == tremulant ? true : false;
}

void GUITremulant::updateDisplayName() {
	setDisplayName(m_tremulant->getName() + wxT(" (Tremulant[") + GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganTremulant(m_tremulant)) + wxT("])"));
}

wxString GUITremulant::getElementName() {
	return m_tremulant->getName();
}
