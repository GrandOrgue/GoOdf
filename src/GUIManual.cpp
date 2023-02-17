/*
 * GUIManual.cpp is part of GOODF.
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

#include "GUIManual.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIManual::GUIManual(Manual *manual) : GUIElement(), m_manual(manual) {
	m_type = wxT("Manual");

	m_dispKeyColourInverted = false;
	m_dispKeyColourWooden = false;
	m_displayFirstNote = m_manual->getFirstAccessibleKeyMIDINoteNumber();
	m_displayKeys = m_manual->getNumberOfAccessibleKeys();
	populateKeyTypes();
	populateKeyNumbers();
	setupDefaultDisplayKeys();
}

GUIManual::~GUIManual() {

}

void GUIManual::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	wxString manId = wxT("Manual=") + GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_manual));
	outFile->AddLine(manId);
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	if (!m_keytypes.empty()) {
		for (KEYTYPE& key : m_keytypes) {
			if (key.KeytypeIdentifier.StartsWith(wxT("Key"))) {
				if (key.ImageOn.getImage() != wxEmptyString)
					outFile->AddLine(key.KeytypeIdentifier + wxT("ImageOn=") + key.ImageOn.getRelativeImagePath());
				if (key.ImageOff.getImage() != wxEmptyString)
					outFile->AddLine(key.KeytypeIdentifier + wxT("ImageOff=") + key.ImageOff.getRelativeImagePath());
				if (key.ImageOn.getMask() != wxEmptyString)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MaskOn=") + key.ImageOn.getRelativeMaskPath());
				if (key.ImageOff.getMask() != wxEmptyString)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MaskOff=") + key.ImageOff.getRelativeMaskPath());
				if (key.Width != key.BitmapWidth)
					outFile->AddLine(key.KeytypeIdentifier + wxT("Width=") + wxString::Format(wxT("%i"), key.Width));
				if (key.Offset != 0)
					outFile->AddLine(key.KeytypeIdentifier + wxT("Offset=") + wxString::Format(wxT("%i"), key.Offset));
				if (key.YOffset != 0)
					outFile->AddLine(key.KeytypeIdentifier + wxT("YOffset=") + wxString::Format(wxT("%i"), key.YOffset));
				if (key.MouseRectLeft != 0)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectLeft=") + wxString::Format(wxT("%i"), key.MouseRectLeft));
				if (key.MouseRectTop != 0)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectTop=") + wxString::Format(wxT("%i"), key.MouseRectTop));
				if (key.MouseRectWidth != key.Width)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectWidth=") + wxString::Format(wxT("%i"), key.MouseRectWidth));
				if (key.MouseRectHeight != key.BitmapHeight)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectHeight=") + wxString::Format(wxT("%i"), key.MouseRectHeight));
			} else {
				if (key.ImageOn.getImage() != wxEmptyString)
					outFile->AddLine(wxT("ImageOn_") + key.KeytypeIdentifier + wxT("=") + key.ImageOn.getRelativeImagePath());
				if (key.ImageOff.getImage() != wxEmptyString)
					outFile->AddLine(wxT("ImageOff_") + key.KeytypeIdentifier + wxT("=") + key.ImageOff.getRelativeImagePath());
				if (key.ImageOn.getMask() != wxEmptyString)
					outFile->AddLine(wxT("MaskOn_") + key.KeytypeIdentifier + wxT("=") + key.ImageOn.getRelativeMaskPath());
				if (key.ImageOff.getMask() != wxEmptyString)
					outFile->AddLine(wxT("MaskOff_") + key.KeytypeIdentifier + wxT("=") + key.ImageOff.getRelativeMaskPath());
				if (key.Width != key.BitmapWidth)
					outFile->AddLine(wxT("Width_") + key.KeytypeIdentifier + wxT("=") + wxString::Format(wxT("%i"), key.Width));
				if (key.Offset != 0)
					outFile->AddLine(wxT("Offset_") + key.KeytypeIdentifier + wxT("=") + wxString::Format(wxT("%i"), key.Offset));
				if (key.YOffset != 0)
					outFile->AddLine(wxT("YOffset_") + key.KeytypeIdentifier + wxT("=") + wxString::Format(wxT("%i"), key.YOffset));
			}
		}
	}
	if (m_dispKeyColourInverted)
		outFile->AddLine(wxT("DispKeyColourInverted=Y"));
	if (m_dispKeyColourWooden)
		outFile->AddLine(wxT("DispKeyColourWooden=Y"));
	if (m_displayFirstNote != m_manual->getFirstAccessibleKeyMIDINoteNumber())
		outFile->AddLine(wxT("DisplayFirstNote=") + wxString::Format(wxT("%i"), m_displayFirstNote));
	if (m_displayKeys != m_manual->getNumberOfAccessibleKeys()) {
		outFile->AddLine(wxT("DisplayKeys=") + wxString::Format(wxT("%i"), m_displayKeys));
	}
	if (displayKeysHaveChanged()) {
		int index = 1;
		for (std::pair<int, int>& key : m_displayKeyMapping) {
			wxString keyId = wxT("DisplayKey") + GOODF_functions::number_format(index);
			outFile->AddLine(keyId + wxT("=") + wxString::Format(wxT("%i"), key.first));
			outFile->AddLine(keyId + wxT("Note=") + wxString::Format(wxT("%i"), key.second));
			index++;
		}
	}
}

bool GUIManual::isReferencing(Manual *man) {
	return m_manual == man ? true : false;
}

void GUIManual::updateDisplayName() {
	setDisplayName(m_manual->getName());
}

Manual* GUIManual::getManual() {
	return m_manual;
}

void GUIManual::addKeytype(wxString identifier) {
	KEYTYPE type;
	type.KeytypeIdentifier = identifier;
	type.ImageOn = GoImage();
	type.ImageOff = GoImage();
	type.Width = 0;
	type.Offset = 0;
	type.YOffset = 0;
	type.MouseRectLeft = 0;
	type.MouseRectTop = 0;
	type.MouseRectWidth = 0;
	type.MouseRectHeight = 0;
	type.BitmapWidth = 0;
	type.BitmapHeight = 0;
	m_keytypes.push_back(type);
}

unsigned GUIManual::getNumberOfKeytypes() {
	return m_keytypes.size();
}

KEYTYPE* GUIManual::getKeytypeAt(unsigned index) {
	auto iterator = std::next(m_keytypes.begin(), index);
	return &(*iterator);
}

void GUIManual::removeKeytypeAt(unsigned index) {
	std::list<KEYTYPE>::iterator it = m_keytypes.begin();
	std::advance(it, index);
	m_keytypes.erase(it);
}

const wxArrayString& GUIManual::getAvailableKeytypes() {
	return m_availableKeytypes;
}

const wxArrayString& GUIManual::getAvailableKeynumbers() {
	return m_availableKeynumbers;
}

bool GUIManual::getDispKeyColourInverted() {
	return m_dispKeyColourInverted;
}

void GUIManual::setDispKeyColourInverted(bool inverted) {
	m_dispKeyColourInverted = inverted;
}

bool GUIManual::getDispKeyColourWooden() {
	return m_dispKeyColourWooden;
}

void GUIManual::setDispKeyColurWooden(bool wooden) {
	m_dispKeyColourWooden = wooden;
}

int GUIManual::getDisplayFirstNote() {
	return m_displayFirstNote;
}

void GUIManual::setDisplayFirstNote(int first) {
	m_displayFirstNote = first;
}

int GUIManual::getNumberOfDisplayKeys() {
	return m_displayKeys;
}

void GUIManual::setNumberOfDisplayKeys(int nbr) {
	if (nbr > m_manual->getNumberOfAccessibleKeys())
		m_displayKeys = m_manual->getNumberOfAccessibleKeys();
	else
		m_displayKeys = nbr;
	populateKeyNumbers();
}

std::pair<int, int>* GUIManual::getDisplayKeyAt(unsigned index) {
	auto iterator = std::next(m_displayKeyMapping.begin(), index);
	return &(*iterator);
}

void GUIManual::populateKeyTypes() {
	if (m_availableKeytypes.IsEmpty()) {
		m_availableKeytypes.Add(wxT("C"));
		m_availableKeytypes.Add(wxT("Cis"));
		m_availableKeytypes.Add(wxT("D"));
		m_availableKeytypes.Add(wxT("Dis"));
		m_availableKeytypes.Add(wxT("E"));
		m_availableKeytypes.Add(wxT("F"));
		m_availableKeytypes.Add(wxT("Fis"));
		m_availableKeytypes.Add(wxT("G"));
		m_availableKeytypes.Add(wxT("Gis"));
		m_availableKeytypes.Add(wxT("A"));
		m_availableKeytypes.Add(wxT("Ais"));
		m_availableKeytypes.Add(wxT("B"));
		m_availableKeytypes.Add(wxT("FirstC"));
		m_availableKeytypes.Add(wxT("FirstCis"));
		m_availableKeytypes.Add(wxT("FirstD"));
		m_availableKeytypes.Add(wxT("FirstDis"));
		m_availableKeytypes.Add(wxT("FirstE"));
		m_availableKeytypes.Add(wxT("FirstF"));
		m_availableKeytypes.Add(wxT("FirstFis"));
		m_availableKeytypes.Add(wxT("FirstG"));
		m_availableKeytypes.Add(wxT("FirstGis"));
		m_availableKeytypes.Add(wxT("FirstA"));
		m_availableKeytypes.Add(wxT("FirstAis"));
		m_availableKeytypes.Add(wxT("FirstB"));
		m_availableKeytypes.Add(wxT("LastC"));
		m_availableKeytypes.Add(wxT("LastCis"));
		m_availableKeytypes.Add(wxT("LastD"));
		m_availableKeytypes.Add(wxT("LastDis"));
		m_availableKeytypes.Add(wxT("LastE"));
		m_availableKeytypes.Add(wxT("LastF"));
		m_availableKeytypes.Add(wxT("LastFis"));
		m_availableKeytypes.Add(wxT("LastG"));
		m_availableKeytypes.Add(wxT("LastGis"));
		m_availableKeytypes.Add(wxT("LastA"));
		m_availableKeytypes.Add(wxT("LastAis"));
		m_availableKeytypes.Add(wxT("LastB"));
	}
}

void GUIManual::populateKeyNumbers() {
	if (!m_availableKeynumbers.IsEmpty())
		m_availableKeynumbers.Empty();
	for (int i = 0; i < m_displayKeys; i++) {
		wxString keyId = wxT("Key") + GOODF_functions::number_format(i + 1);
		m_availableKeynumbers.Add(keyId);
	}
}

void GUIManual::setupDefaultDisplayKeys() {
	// int firstMIDInote = m_manual->getFirstAccessibleKeyMIDINoteNumber();
	if (!m_displayKeyMapping.empty())
		m_displayKeyMapping.clear();
	if (!defaultDisplayKeyMapping.empty())
		defaultDisplayKeyMapping.clear();
	for (int i = 0; i < m_displayKeys; i++) {
		int value = m_displayFirstNote + i;
		m_displayKeyMapping.push_back(std::make_pair(value, value));
		defaultDisplayKeyMapping.push_back(std::make_pair(value, value));
	}
}

bool GUIManual::displayKeysHaveChanged() {
	if (m_displayKeyMapping.size() != defaultDisplayKeyMapping.size())
		return true;
	else {
		unsigned index = 0;
		for (std::pair<int, int>& key : defaultDisplayKeyMapping) {
			if (key.first != getDisplayKeyAt(index)->first || key.second != getDisplayKeyAt(index)->second)
				return true;

			index++;
		}
		return false;
	}
}
