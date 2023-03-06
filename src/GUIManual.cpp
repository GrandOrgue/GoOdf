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

void GUIManual::read(wxFileConfig *cfg) {
	int thePanelWidth = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
	int thePanelHeight = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();
	int posX = static_cast<int>(cfg->ReadLong("PositionX", 0));
	if (posX > -1 && posX < thePanelWidth)
		setPosX(posX);
	int posY = static_cast<int>(cfg->ReadLong("PositionY", 0));
	if (posY > -1 && posY < thePanelHeight)
		setPosY(posY);
	for (int i = 0; i < 3; i++) {
		wxString typePrefix = wxEmptyString;
		if (i == 1)
			typePrefix = wxT("First");
		else if (i == 2)
			typePrefix = wxT("Last");

		for (int j = 0; j < 12; j++) {
			wxString noteName = wxEmptyString;
			if (j == 0)
				noteName = wxT("C");
			else if (j == 1)
				noteName = wxT("Cis");
			else if (j == 2)
				noteName = wxT("D");
			else if (j == 3)
				noteName = wxT("Dis");
			else if (j == 4)
				noteName = wxT("E");
			else if (j == 5)
				noteName = wxT("F");
			else if (j == 6)
				noteName = wxT("Fis");
			else if (j == 7)
				noteName = wxT("G");
			else if (j == 8)
				noteName = wxT("Gis");
			else if (j == 9)
				noteName = wxT("A");
			else if (j == 10)
				noteName = wxT("Ais");
			else if (j == 11)
				noteName = wxT("B");

			wxString keyType = typePrefix + noteName;

			wxString keyImageOn = wxT("ImageOn_") + keyType;
			wxString cfgImgOn = cfg->Read(keyImageOn, wxEmptyString);
			if (cfgImgOn != wxEmptyString) {
				wxString fullImgOnPath = GOODF_functions::checkIfFileExist(cfgImgOn);
				if (fullImgOnPath != wxEmptyString) {
					wxImage img = wxImage(fullImgOnPath);
					if (img.IsOk()) {
						int width = img.GetWidth();
						int height = img.GetHeight();
						addKeytype(keyType);
						KEYTYPE *type = getKeytypeAt(m_keytypes.size() - 1);
						type->ImageOn.setImage(fullImgOnPath);
						type->ImageOn.setOriginalWidth(width);
						type->ImageOn.setOriginalHeight(height);
						type->BitmapWidth = width;
						type->BitmapHeight = height;

						wxString keyImageOff = wxT("ImageOff_") + keyType;
						wxString cfgImgOff = cfg->Read(keyImageOff, wxEmptyString);
						if (cfgImgOff != wxEmptyString) {
							wxString fullImgOffPath = GOODF_functions::checkIfFileExist(cfgImgOff);
							if (fullImgOffPath != wxEmptyString) {
								type->ImageOff.setImage(fullImgOffPath);
							}
						}
						wxString keyMaskOn = wxT("MaskOn_") + keyType;
						wxString cfgMaskOn = cfg->Read(keyMaskOn, wxEmptyString);
						if (cfgMaskOn != wxEmptyString) {
							wxString fullMaskOnPath = GOODF_functions::checkIfFileExist(cfgMaskOn);
							if (fullMaskOnPath != wxEmptyString) {
								type->ImageOn.setMask(fullMaskOnPath);
							}
						}
						wxString keyMaskOff = wxT("MaskOff_") + keyType;
						wxString cfgMaskOff = cfg->Read(keyMaskOff, wxEmptyString);
						if (cfgMaskOff != wxEmptyString) {
							wxString fullMaskOffPath = GOODF_functions::checkIfFileExist(cfgMaskOff);
							if (fullMaskOffPath != wxEmptyString) {
								type->ImageOff.setMask(fullMaskOffPath);
							}
						}
						wxString keyWidth = wxT("Width_") + keyType;
						int cfgKeyWidth = static_cast<int>(cfg->ReadLong(keyWidth, -1));
						if (cfgKeyWidth > -1 && cfgKeyWidth < 501) {
							type->Width = cfgKeyWidth;
						} else {
							type->Width = width;
						}
						wxString keyOffset = wxT("Offset_") + keyType;
						int cfgOffset = static_cast<int>(cfg->ReadLong(keyOffset, 0));
						if (cfgOffset > -501 && cfgOffset < 501) {
							type->Offset = cfgOffset;
						}
						wxString keyYoffset = wxT("YOffset_") + keyType;
						int cfgYoffset = static_cast<int>(cfg->ReadLong(keyYoffset, 0));
						if (cfgYoffset > -501 && cfgYoffset < 501) {
							type->YOffset = cfgYoffset;
						}
					}
				}
			}
		}
	}
	int dispKeys = static_cast<int>(cfg->ReadLong("DisplayKeys", m_manual->getNumberOfAccessibleKeys()));
	if (dispKeys > 0 && dispKeys <= m_manual->getNumberOfAccessibleKeys()) {
		m_displayKeys = dispKeys;
	} else {
		m_displayKeys = m_manual->getNumberOfAccessibleKeys();
	}
	for (int i = 0; i < m_displayKeys; i++) {
		wxString keyStr = wxT("Key") + GOODF_functions::number_format(i + 1);
		wxString keyImgOn = cfg->Read(keyStr + wxT("ImageOn"), wxEmptyString);
		if (keyImgOn != wxEmptyString) {
			wxString fullImgOnPath = GOODF_functions::checkIfFileExist(keyImgOn);
			if (fullImgOnPath != wxEmptyString) {
				wxImage img = wxImage(fullImgOnPath);
				if (img.IsOk()) {
					int width = img.GetWidth();
					int height = img.GetHeight();
					addKeytype(keyStr);
					KEYTYPE *type = getKeytypeAt(m_keytypes.size() - 1);
					type->ImageOn.setImage(fullImgOnPath);
					type->ImageOn.setOriginalWidth(width);
					type->ImageOn.setOriginalHeight(height);
					type->BitmapWidth = width;
					type->BitmapHeight = height;

					wxString keyImgOff = cfg->Read(keyStr + wxT("ImageOff"), wxEmptyString);
					if (keyImgOff != wxEmptyString) {
						wxString fullImgOffPath = GOODF_functions::checkIfFileExist(keyImgOff);
						if (fullImgOffPath != wxEmptyString) {
							type->ImageOff.setImage(fullImgOffPath);
						}
					}
					wxString keyMskOn = cfg->Read(keyStr + wxT("MaskOn"), wxEmptyString);
					if (keyMskOn != wxEmptyString) {
						wxString fullMskOnPath = GOODF_functions::checkIfFileExist(keyMskOn);
						if (fullMskOnPath != wxEmptyString) {
							type->ImageOn.setMask(fullMskOnPath);
						}
					}
					wxString keyMskOff = cfg->Read(keyStr + wxT("MaskOff"), wxEmptyString);
					if (keyMskOff != wxEmptyString) {
						wxString fullMaskOffPath = GOODF_functions::checkIfFileExist(keyMskOff);
						if (fullMaskOffPath != wxEmptyString) {
							type->ImageOff.setMask(fullMaskOffPath);
						}
					}
					int keyWidth = static_cast<int>(cfg->ReadLong(keyStr + wxT("Width"), -1));
					if (keyWidth > -1 && keyWidth < 501) {
						type->Width = keyWidth;
					} else {
						type->Width = width;
					}
					int keyOffset = static_cast<int>(cfg->ReadLong(keyStr + wxT("Offset"), 0));
					if (keyOffset > -501 && keyOffset < 501) {
						type->Offset = keyOffset;
					}
					int keyYoffset = static_cast<int>(cfg->ReadLong(keyStr + wxT("YOffset"), 0));
					if (keyYoffset > -501 && keyYoffset < 501) {
						type->YOffset = keyYoffset;
					}

					int keyMouseRectLeft = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectLeft"), 0));
					if (keyMouseRectLeft >= 0 && keyMouseRectLeft < type->BitmapWidth) {
						type->MouseRectLeft = keyMouseRectLeft;
					}
					int keyMouseRectTop = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectTop"), 0));
					if (keyMouseRectTop >= 0 && keyMouseRectTop < type->BitmapHeight) {
						type->MouseRectTop = keyMouseRectTop;
					}
					int keyMouseRectWidth = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectWidth"), 0));
					if (keyMouseRectWidth >= 0 && keyMouseRectWidth <= type->BitmapWidth) {
						type->MouseRectWidth = keyMouseRectWidth;
					}
					int keyMouseRectHeight = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectHeight"), 0));
					if (keyMouseRectHeight >= 0 && keyMouseRectHeight <= type->BitmapHeight) {
						type->MouseRectHeight = keyMouseRectHeight;
					}
				}
			}
		}
	}
	wxString cfgBoolValue = cfg->Read("DispKeyColourInverted", wxEmptyString);
	m_dispKeyColourInverted = GOODF_functions::parseBoolean(cfgBoolValue, false);
	cfgBoolValue = cfg->Read("DispKeyColourWooden", wxEmptyString);
	m_dispKeyColourWooden = GOODF_functions::parseBoolean(cfgBoolValue, false);
	int dispFirstNote = static_cast<int>(cfg->ReadLong("DisplayFirstNote", m_manual->getFirstAccessibleKeyMIDINoteNumber()));
	if (dispFirstNote >= 0 && dispFirstNote < 128) {
		m_displayFirstNote = dispFirstNote;
	} else {
		m_displayFirstNote = m_manual->getFirstAccessibleKeyMIDINoteNumber();
	}
	for (int i = 0; i < m_displayKeys; i ++) {
		wxString dispKeyNbr = wxT("DisplayKey") + GOODF_functions::number_format(i + 1);
		int backendMidiKeyNbr = static_cast<int>(cfg->ReadLong(dispKeyNbr, -1));
		int frontendMidiKeyNbr = static_cast<int>(cfg->ReadLong(dispKeyNbr + wxT("Note"), -1));
		if (backendMidiKeyNbr >= 0 && backendMidiKeyNbr < 128) {
			if (frontendMidiKeyNbr >= 0 && frontendMidiKeyNbr < 128) {
				std::pair<int, int> *key_map = getDisplayKeyAt(i);
				key_map->first = backendMidiKeyNbr;
				key_map->second = frontendMidiKeyNbr;
			}
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
