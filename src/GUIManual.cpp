/*
 * GUIManual.cpp is part of GoOdf.
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

#include "GUIManual.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIManual::GUIManual(Manual *manual) : GUIElement(), m_manual(manual) {
	m_type = wxT("Manual");

	m_dispKeyColourInverted = false;
	m_dispKeyColourWooden = false;
	m_displayFirstNote = m_manual->getFirstAccessibleKeyMIDINoteNumber();
	m_displayKeys = m_manual->getNumberOfAccessibleKeys();
	m_dispImageNum = 1;
	m_displayedAsPedal = false;
	populateKeyTypes();
	populateKeyNumbers();
	setupDefaultDisplayKeys();
	updateKeyInfo();
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
					outFile->AddLine(key.KeytypeIdentifier + wxT("ImageOn=") + GOODF_functions::fixSeparator(key.ImageOn.getRelativeImagePath()));
				if (key.ImageOff.getImage() != wxEmptyString)
					outFile->AddLine(key.KeytypeIdentifier + wxT("ImageOff=") + GOODF_functions::fixSeparator(key.ImageOff.getRelativeImagePath()));
				if (key.ImageOn.getMask() != wxEmptyString)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MaskOn=") + GOODF_functions::fixSeparator(key.ImageOn.getRelativeMaskPath()));
				if (key.ImageOff.getMask() != wxEmptyString)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MaskOff=") + GOODF_functions::fixSeparator(key.ImageOff.getRelativeMaskPath()));
				if (key.Width != key.BitmapWidth || keyNbrOverrideBaseKeyWidth(&key) || key.ForceWritingWidth)
					outFile->AddLine(key.KeytypeIdentifier + wxT("Width=") + wxString::Format(wxT("%i"), key.Width));
				if (key.Offset != 0 || key.ForceWritingOffset)
					outFile->AddLine(key.KeytypeIdentifier + wxT("Offset=") + wxString::Format(wxT("%i"), key.Offset));
				if (key.YOffset != 0)
					outFile->AddLine(key.KeytypeIdentifier + wxT("YOffset=") + wxString::Format(wxT("%i"), key.YOffset));
				if (key.MouseRectLeft != 0)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectLeft=") + wxString::Format(wxT("%i"), key.MouseRectLeft));
				if (key.MouseRectTop != 0)
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectTop=") + wxString::Format(wxT("%i"), key.MouseRectTop));
				if (key.MouseRectWidth != (key.BitmapWidth - key.MouseRectLeft))
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectWidth=") + wxString::Format(wxT("%i"), key.MouseRectWidth));
				if (key.MouseRectHeight != (key.BitmapHeight - key.MouseRectTop))
					outFile->AddLine(key.KeytypeIdentifier + wxT("MouseRectHeight=") + wxString::Format(wxT("%i"), key.MouseRectHeight));
			} else {
				if (key.ImageOn.getImage() != wxEmptyString)
					outFile->AddLine(wxT("ImageOn_") + key.KeytypeIdentifier + wxT("=") + GOODF_functions::fixSeparator(key.ImageOn.getRelativeImagePath()));
				if (key.ImageOff.getImage() != wxEmptyString)
					outFile->AddLine(wxT("ImageOff_") + key.KeytypeIdentifier + wxT("=") + GOODF_functions::fixSeparator(key.ImageOff.getRelativeImagePath()));
				if (key.ImageOn.getMask() != wxEmptyString)
					outFile->AddLine(wxT("MaskOn_") + key.KeytypeIdentifier + wxT("=") + GOODF_functions::fixSeparator(key.ImageOn.getRelativeMaskPath()));
				if (key.ImageOff.getMask() != wxEmptyString)
					outFile->AddLine(wxT("MaskOff_") + key.KeytypeIdentifier + wxT("=") + GOODF_functions::fixSeparator(key.ImageOff.getRelativeMaskPath()));
				if (key.Width != key.BitmapWidth || key.ForceWritingWidth)
					outFile->AddLine(wxT("Width_") + key.KeytypeIdentifier + wxT("=") + wxString::Format(wxT("%i"), key.Width));
				if (key.Offset != 0 || key.ForceWritingOffset)
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
	if (m_dispImageNum != 1)
		outFile->AddLine(wxT("DispImageNum=") + wxString::Format(wxT("%i"), m_dispImageNum));
	if (m_displayFirstNote != m_manual->getFirstAccessibleKeyMIDINoteNumber())
		outFile->AddLine(wxT("DisplayFirstNote=") + wxString::Format(wxT("%i"), m_displayFirstNote));
	if (m_displayKeys != m_manual->getNumberOfAccessibleKeys()) {
		outFile->AddLine(wxT("DisplayKeys=") + wxString::Format(wxT("%i"), m_displayKeys));
	}

	for (int i = 0; i < m_displayKeys; i++) {
		wxString dispKeyNbr = wxT("DisplayKey") + GOODF_functions::number_format(i + 1);
		if (getDisplayKeyAt(i)->first != m_displayFirstNote + i)
			outFile->AddLine(dispKeyNbr + wxT("=") + wxString::Format(wxT("%i"), getDisplayKeyAt(i)->first));
		if (getDisplayKeyAt(i)->second != m_displayFirstNote + i)
			outFile->AddLine(dispKeyNbr + wxT("Note=") + wxString::Format(wxT("%i"), getDisplayKeyAt(i)->second));
	}
}

void GUIManual::read(wxFileConfig *cfg, Organ *readOrgan) {
	int thePanelWidth = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
	int thePanelHeight = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();
	int dispFirstNote = static_cast<int>(cfg->ReadLong("DisplayFirstNote", m_manual->getFirstAccessibleKeyMIDINoteNumber()));
	if (dispFirstNote >= 0 && dispFirstNote < 128) {
		m_displayFirstNote = dispFirstNote;
	} else {
		m_displayFirstNote = m_manual->getFirstAccessibleKeyMIDINoteNumber();
	}
	int dispKeys = static_cast<int>(cfg->ReadLong("DisplayKeys", m_manual->getNumberOfAccessibleKeys()));
	if (dispKeys > 0 && dispKeys <= m_manual->getNumberOfAccessibleKeys()) {
		setNumberOfDisplayKeys(dispKeys);
	} else {
		setNumberOfDisplayKeys(m_manual->getNumberOfAccessibleKeys());
	}
	for (int i = 0; i < m_displayKeys; i++) {
		wxString dispKeyNbr = wxT("DisplayKey") + GOODF_functions::number_format(i + 1);
		int backendMidiKeyNbr = static_cast<int>(cfg->ReadLong(dispKeyNbr, m_displayFirstNote + i));
		int frontendMidiKeyNbr = static_cast<int>(cfg->ReadLong(dispKeyNbr + wxT("Note"), m_displayFirstNote + i));
		std::pair<int, int> *key_map = getDisplayKeyAt(i);
		if (backendMidiKeyNbr >= 0 && backendMidiKeyNbr < 128) {
			key_map->first = backendMidiKeyNbr;
		}
		if (frontendMidiKeyNbr >= 0 && frontendMidiKeyNbr < 128) {
			key_map->second = frontendMidiKeyNbr;
		}
	}
	int posX = static_cast<int>(cfg->ReadLong("PositionX", -1));
	if (posX > -1 && posX <= thePanelWidth)
		setPosX(posX);
	int posY = static_cast<int>(cfg->ReadLong("PositionY", -1));
	if (posY > -1 && posY <= thePanelHeight)
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
			addKeytype(keyType, true);
			KEYTYPE *type = getKeytypeAt(m_keytypes.size() - 1);
			bool keepKeyType = false;
			wxString keyImageOn = wxT("ImageOn_") + keyType;
			wxString cfgImgOn = cfg->Read(keyImageOn, wxEmptyString);
			if (cfgImgOn != wxEmptyString) {
				wxString fullImgOnPath = GOODF_functions::checkIfFileExist(cfgImgOn, readOrgan);
				if (fullImgOnPath != wxEmptyString) {
					wxImage img = wxImage(fullImgOnPath);
					if (img.IsOk()) {
						int width = img.GetWidth();
						int height = img.GetHeight();

						type->ImageOn.setImage(fullImgOnPath);
						type->ImageOn.setOriginalWidth(width);
						type->ImageOn.setOriginalHeight(height);
						type->BitmapWidth = width;
						type->BitmapHeight = height;
						keepKeyType = true;
						wxString keyImageOff = wxT("ImageOff_") + keyType;
						wxString cfgImgOff = cfg->Read(keyImageOff, wxEmptyString);
						if (cfgImgOff != wxEmptyString) {
							wxString fullImgOffPath = GOODF_functions::checkIfFileExist(cfgImgOff, readOrgan);
							if (fullImgOffPath != wxEmptyString) {
								type->ImageOff.setImage(fullImgOffPath);
							}
						}
						wxString keyMaskOn = wxT("MaskOn_") + keyType;
						wxString cfgMaskOn = cfg->Read(keyMaskOn, wxEmptyString);
						if (cfgMaskOn != wxEmptyString) {
							wxString fullMaskOnPath = GOODF_functions::checkIfFileExist(cfgMaskOn, readOrgan);
							if (fullMaskOnPath != wxEmptyString) {
								type->ImageOn.setMask(fullMaskOnPath);
							}
						}
						wxString keyMaskOff = wxT("MaskOff_") + keyType;
						wxString cfgMaskOff = cfg->Read(keyMaskOff, wxEmptyString);
						if (cfgMaskOff != wxEmptyString) {
							wxString fullMaskOffPath = GOODF_functions::checkIfFileExist(cfgMaskOff, readOrgan);
							if (fullMaskOffPath != wxEmptyString) {
								type->ImageOff.setMask(fullMaskOffPath);
							}
						}
					}
				}
			}

			wxString keyWidth = wxT("Width_") + keyType;
			int cfgKeyWidth = static_cast<int>(cfg->ReadLong(keyWidth, -1));
			if (cfgKeyWidth > -1 && cfgKeyWidth < 501) {
				type->Width = cfgKeyWidth;
				type->ForceWritingWidth = true;
				keepKeyType = true;
			} else {
				type->Width = type->BitmapWidth;
			}
			wxString keyOffset = wxT("Offset_") + keyType;
			int cfgOffset = static_cast<int>(cfg->ReadLong(keyOffset, -1000));
			if (cfgOffset > -501 && cfgOffset < 501) {
				keepKeyType = true;
				type->Offset = cfgOffset;
				type->ForceWritingOffset = true;
			}
			wxString keyYoffset = wxT("YOffset_") + keyType;
			int cfgYoffset = static_cast<int>(cfg->ReadLong(keyYoffset, -1000));
			if (cfgYoffset > -501 && cfgYoffset < 501) {
				type->YOffset = cfgYoffset;
				keepKeyType = true;
			}
			type->MouseRectWidth = type->BitmapWidth - type->MouseRectLeft;
			type->MouseRectHeight = type->BitmapHeight - type->MouseRectTop;

			if (!keepKeyType) {
				m_keytypes.pop_back();
			}
		}
	}
	for (int i = 0; i < m_displayKeys; i++) {
		wxString keyStr = wxT("Key") + GOODF_functions::number_format(i + 1);
		wxString keyImgOn = cfg->Read(keyStr + wxT("ImageOn"), wxEmptyString);
		addKeytype(keyStr, true);
		KEYTYPE *type = getKeytypeAt(m_keytypes.size() - 1);
		bool keepKeyType = false;
		if (keyImgOn != wxEmptyString) {
			wxString fullImgOnPath = GOODF_functions::checkIfFileExist(keyImgOn, readOrgan);
			if (fullImgOnPath != wxEmptyString) {
				wxImage img = wxImage(fullImgOnPath);
				if (img.IsOk()) {
					int width = img.GetWidth();
					int height = img.GetHeight();

					type->ImageOn.setImage(fullImgOnPath);
					type->ImageOn.setOriginalWidth(width);
					type->ImageOn.setOriginalHeight(height);
					type->BitmapWidth = width;
					type->BitmapHeight = height;
					keepKeyType = true;
					wxString keyImgOff = cfg->Read(keyStr + wxT("ImageOff"), wxEmptyString);
					if (keyImgOff != wxEmptyString) {
						wxString fullImgOffPath = GOODF_functions::checkIfFileExist(keyImgOff, readOrgan);
						if (fullImgOffPath != wxEmptyString) {
							type->ImageOff.setImage(fullImgOffPath);
						}
					}
					wxString keyMskOn = cfg->Read(keyStr + wxT("MaskOn"), wxEmptyString);
					if (keyMskOn != wxEmptyString) {
						wxString fullMskOnPath = GOODF_functions::checkIfFileExist(keyMskOn, readOrgan);
						if (fullMskOnPath != wxEmptyString) {
							type->ImageOn.setMask(fullMskOnPath);
						}
					}
					wxString keyMskOff = cfg->Read(keyStr + wxT("MaskOff"), wxEmptyString);
					if (keyMskOff != wxEmptyString) {
						wxString fullMaskOffPath = GOODF_functions::checkIfFileExist(keyMskOff, readOrgan);
						if (fullMaskOffPath != wxEmptyString) {
							type->ImageOff.setMask(fullMaskOffPath);
						}
					}
				}
			}
		}

		int keyWidth = static_cast<int>(cfg->ReadLong(keyStr + wxT("Width"), -1));
		if (keyWidth > -1 && keyWidth < 501) {
			type->Width = keyWidth;
			type->ForceWritingWidth = true;
			keepKeyType = true;
		} else {
			// Check if this key is overriding an existing general key type
			int baseTypeIdx = baseKeyTypeExistAtIndex(keyStr);
			if (baseTypeIdx > -1) {
				KEYTYPE *baseKey = getKeytypeAt(baseTypeIdx);
				type->Width = baseKey->Width;
				type->BitmapWidth = baseKey->BitmapWidth;
				type->BitmapHeight = baseKey->BitmapHeight;
			} else
				type->Width = type->BitmapWidth;
		}
		int keyOffset = static_cast<int>(cfg->ReadLong(keyStr + wxT("Offset"), -1000));
		if (keyOffset > -501 && keyOffset < 501) {
			type->Offset = keyOffset;
			keepKeyType = true;
			type->ForceWritingOffset = true;
		}
		int keyYoffset = static_cast<int>(cfg->ReadLong(keyStr + wxT("YOffset"), -1000));
		if (keyYoffset > -501 && keyYoffset < 501) {
			type->YOffset = keyYoffset;
			keepKeyType = true;
		}
		int keyMouseRectLeft = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectLeft"), -1));
		if (keyMouseRectLeft >= 0 && keyMouseRectLeft < type->BitmapWidth) {
			type->MouseRectLeft = keyMouseRectLeft;
			keepKeyType = true;
		}
		int keyMouseRectTop = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectTop"), -1));
		if (keyMouseRectTop >= 0 && keyMouseRectTop < type->BitmapHeight) {
			type->MouseRectTop = keyMouseRectTop;
			keepKeyType = true;
		}
		int keyMouseRectWidth = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectWidth"), -1));
		if (keyMouseRectWidth > 0 && keyMouseRectWidth <= (type->BitmapWidth - type->MouseRectLeft)) {
			type->MouseRectWidth = keyMouseRectWidth;
			keepKeyType = true;
		} else {
			type->MouseRectWidth = type->BitmapWidth - type->MouseRectLeft;
		}
		int keyMouseRectHeight = static_cast<int>(cfg->ReadLong(keyStr + wxT("MouseRectHeight"), -1));
		if (keyMouseRectHeight > 0 && keyMouseRectHeight <= (type->BitmapHeight - type->MouseRectTop)) {
			type->MouseRectHeight = keyMouseRectHeight;
			keepKeyType = true;
		} else {
			type->MouseRectHeight = type->BitmapHeight - type->MouseRectTop;
		}

		if (!keepKeyType) {
			m_keytypes.pop_back();
		}
	}
	wxString cfgBoolValue = cfg->Read("DispKeyColourInverted", wxEmptyString);
	m_dispKeyColourInverted = GOODF_functions::parseBoolean(cfgBoolValue, false);
	cfgBoolValue = cfg->Read("DispKeyColourWooden", wxEmptyString);
	m_dispKeyColourWooden = GOODF_functions::parseBoolean(cfgBoolValue, false);
	int dispImageNum = static_cast<int>(cfg->ReadLong("DispImageNum", 1));
	if (dispImageNum > 0 && dispImageNum < 3) {
		setDispImageNum(dispImageNum);
	}

	updateKeyInfo();
}

GUIManual* GUIManual::clone() {
	return new GUIManual(*this);
}

bool GUIManual::isReferencing(Manual *man) {
	return m_manual == man ? true : false;
}

void GUIManual::updateDisplayName() {
	setDisplayName(m_manual->getName() + wxT(" (Manual[") + GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganManual(m_manual)) + wxT("])"));
}

Manual* GUIManual::getManual() {
	return m_manual;
}

void GUIManual::addKeytype(wxString identifier, bool isReading) {
	KEYTYPE type;
	type.KeytypeIdentifier = identifier;
	type.ImageOn = GoImage();
	type.ImageOff = GoImage();
	type.IsSharp = false;
	int typeWidth = 0;
	int typeHeight = 0;
	int index = m_availableKeynumbers.Index(type.KeytypeIdentifier);
	if (type.KeytypeIdentifier.StartsWith(wxT("Key")) && index > -1) {
		// This is for numbered keys
		int key_nbr = getDisplayKeyAt((unsigned) index)->second;
		if (((key_nbr % 12) < 5 && !(key_nbr & 1)) || ((key_nbr % 12) >= 5 && (key_nbr & 1))) {
			// It's a natural key
			if (m_manual->isThePedal()) {
				typeWidth = 7;
				typeHeight = 40;
			} else {
				typeWidth = 12;
				typeHeight = 32;
			}
		} else {
			// It's a sharp
			type.IsSharp = true;
			if (m_manual->isThePedal()) {
				typeWidth = 7;
				typeHeight = 18;
			} else {
				typeWidth = 6;
				typeHeight = 19;
			}
		}
	} else {
		// This is for named key types so any that contain "is" is a sharp type
		if (type.KeytypeIdentifier.Find(wxT("is")) == wxNOT_FOUND) {
			// It's a natural key
			if (m_manual->isThePedal()) {
				typeWidth = 7;
				typeHeight = 40;
			} else {
				typeWidth = 12;
				typeHeight = 32;
			}
		} else {
			// It's a sharp
			type.IsSharp = true;
			if (m_manual->isThePedal()) {
				typeWidth = 7;
				typeHeight = 18;
			} else {
				typeWidth = 6;
				typeHeight = 19;
			}
		}
	}
	type.Width = typeWidth;
	type.Offset = 0;
	type.YOffset = 0;
	type.MouseRectLeft = 0;
	type.MouseRectTop = 0;
	type.MouseRectWidth = typeWidth;
	type.MouseRectHeight = typeHeight;
	type.BitmapWidth = typeWidth;
	type.BitmapHeight = typeHeight;
	type.ForceWritingWidth = false;
	type.ForceWritingOffset = false;
	m_keytypes.push_back(type);

	if (!isReading)
		updateKeyInfo();
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

	updateKeyInfo();
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
	updateKeyInfo();
}

bool GUIManual::getDispKeyColourWooden() {
	return m_dispKeyColourWooden;
}

void GUIManual::setDispKeyColurWooden(bool wooden) {
	m_dispKeyColourWooden = wooden;
	updateKeyInfo();
}

int GUIManual::getDisplayFirstNote() {
	return m_displayFirstNote;
}

void GUIManual::setDisplayFirstNote(int first) {
	m_displayFirstNote = first;
	populateKeyNumbers();
	setupDefaultDisplayKeys();
	validateKeyTypes();
	updateKeyInfo();
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
	setupDefaultDisplayKeys();
	validateKeyTypes();
	updateKeyInfo();
}

std::pair<int, int>* GUIManual::getDisplayKeyAt(unsigned index) {
	auto iterator = std::next(m_displayKeyMapping.begin(), index);
	return &(*iterator);
}

void GUIManual::validateKeyTypes() {
	// This function will check if the added keytypes are valid (still) and remove any non valid ones
	std::list<KEYTYPE>::iterator it = m_keytypes.begin();
	while (it != m_keytypes.end()) {
		if (m_availableKeytypes.Index(it->KeytypeIdentifier) == wxNOT_FOUND && m_availableKeynumbers.Index(it->KeytypeIdentifier) == wxNOT_FOUND) {
			it = m_keytypes.erase(it);
		} else {
			++it;
		}
	}
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

int GUIManual::baseKeyTypeExistAtIndex(wxString keyNbrType) {
	// This function searches if a number key type will override any base keytype and return that index (or -1 if not found)
	int returnIndex = -1;
	if (keyNbrType.StartsWith(wxT("Key"))) {
		int keyNbrIdx = getIndexOfKeyNumber(keyNbrType);
		if (keyNbrIdx > -1){
			int keyMidiNumber = getDisplayKeyAt(keyNbrIdx)->second;
			wxString base_key_str = wxEmptyString;
			int restFromDivision = keyMidiNumber % 12;
			switch (restFromDivision) {
				case 0:
					base_key_str = wxT("C");
					break;
				case 1:
					base_key_str = wxT("Cis");
					break;
				case 2:
					base_key_str = wxT("D");
					break;
				case 3:
					base_key_str = wxT("Dis");
					break;
				case 4:
					base_key_str = wxT("E");
					break;
				case 5:
					base_key_str = wxT("F");
					break;
				case 6:
					base_key_str = wxT("Fis");
					break;
				case 7:
					base_key_str = wxT("G");
					break;
				case 8:
					base_key_str = wxT("Gis");
					break;
				case 9:
					base_key_str = wxT("A");
					break;
				case 10:
					base_key_str = wxT("Ais");
					break;
				case 11:
					base_key_str = wxT("B");
					break;
				default:
					base_key_str = wxEmptyString;
			}
			if (base_key_str != wxEmptyString) {
				wxString searchedKeyType = wxEmptyString;
				if (keyNbrIdx == 0) {
					searchedKeyType = wxT("First") + base_key_str;
				} else if (keyNbrIdx == (int) (m_availableKeynumbers.size() - 1)) {
					searchedKeyType = wxT("Last") + base_key_str;
				} else {
					searchedKeyType = base_key_str;
				}
				int foundIdx = 0;
				for (KEYTYPE& key : m_keytypes) {
					if (key.KeytypeIdentifier.IsSameAs(searchedKeyType)) {
						returnIndex = foundIdx;
						break;
					}
					foundIdx++;
				}
			}
		}
		return returnIndex;
	} else {
		return returnIndex;
	}
}

bool GUIManual::keyNbrOverrideBaseKeyWidth(KEYTYPE *key) {
	int possibleOverride = baseKeyTypeExistAtIndex(key->KeytypeIdentifier);
	if (possibleOverride > -1 && possibleOverride < (int) m_keytypes.size() && !key->IsSharp) {
		if (key->Width != getKeytypeAt((unsigned) possibleOverride)->Width)
			return true;
		else
			return false;
	} else {
		return false;
	}
}

void GUIManual::setupDefaultDisplayKeys() {
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

int GUIManual::getIndexOfKeyNumber(wxString keytype) {
	return m_availableKeynumbers.Index(keytype);
}

int GUIManual::getDispImageNum() {
	return m_dispImageNum;
}

void GUIManual::setDispImageNum(int nbr) {
	m_dispImageNum = nbr;
	updateKeyInfo();
}

void GUIManual::updateKeyInfo() {
	int keyXpos = 0;
	m_keys.resize(m_displayKeys);
	for (int i = 0; i < (int) m_keys.size(); i++) {
		// at first we set builtin default key values
		int key_nb = getDisplayKeyAt(i)->second;
		m_keys[i].DisplayAsMidiNote = key_nb;
		m_keys[i].IsSharp = (((key_nb % 12) < 5 && !(key_nb & 1)) || ((key_nb % 12) >= 5 && (key_nb & 1))) ? false : true;
		m_keys[i].Xpos = keyXpos;
		m_keys[i].Ypos = 0;

		int width = 0;
		if (m_displayedAsPedal) {
			width = 7;
		} else {
			if (!m_keys[i].IsSharp) {
				width = 12;
			} else {
				width = 6;
			}
		}

		if (m_displayedAsPedal) {
			// a pedal key of "e" or "b" should add another key width for the next key
			if ((key_nb % 12) == 4 || (key_nb % 12) == 11)
				width *= 2;
		}

		if (m_displayedAsPedal) {
			// this is for a pedal keyboard
			if (m_dispKeyColourInverted) {
				// keyboard is inverted
				if (m_dispKeyColourWooden) {
					if (m_dispImageNum == 2)
						m_keys[i].KeyImage = ::wxGetApp().m_invertedPedalKeysBmps02[key_nb % 12];
					else
						m_keys[i].KeyImage = ::wxGetApp().m_invertedPedalKeysBmps01[key_nb % 12];
				} else {
					if (m_dispImageNum == 2)
						m_keys[i].KeyImage = ::wxGetApp().m_invertedPedalKeysBmps02[key_nb % 12];
					else
						m_keys[i].KeyImage = ::wxGetApp().m_invertedPedalKeysBmps01[key_nb % 12];
				}
			} else {
				// this is a normal keyboard
				if (m_dispKeyColourWooden) {
					if (m_dispImageNum == 2)
						m_keys[i].KeyImage = ::wxGetApp().m_pedalKeysBmps02[key_nb % 12];
					else
						m_keys[i].KeyImage = ::wxGetApp().m_pedalKeysBmps01[key_nb % 12];
				} else {
					if (m_dispImageNum == 2)
						m_keys[i].KeyImage = ::wxGetApp().m_pedalKeysBmps02[key_nb % 12];
					else
						m_keys[i].KeyImage = ::wxGetApp().m_pedalKeysBmps01[key_nb % 12];
				}
			}
		} else {
			// this is for a manual keyboard
			if (m_dispKeyColourInverted) {
				// keyboard is inverted
				if (m_dispKeyColourWooden) {
					if (i == 0 && !m_keys[i].IsSharp) {
						// this is the first key and it's a natural so if it's an E or B then it should be displayed as a whole
						// otherwise it should be displayed as C unless next key also would be a natural for any reason
						bool nextIsNatural = false;
						if (m_displayKeys > 1) {
							nextIsNatural = (((key_nb % 12) < 5 && !(key_nb & 1)) || ((key_nb % 12) >= 5 && (key_nb & 1))) ? true : false;
						}
						if (key_nb % 12 == 4 || key_nb % 12 == 11 || nextIsNatural) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps01[12];
						} else {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps02[0];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps01[0];
						}
					} else if (i + 1 < m_displayKeys) {
						// this is not the last key of the manual
						if (m_dispImageNum == 2)
							m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps02[key_nb % 12];
						else
							m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps01[key_nb % 12];
					} else {
						// this is the last key of the manual
						if (key_nb % 12 == 0 || key_nb % 12 == 5) {
							// if it's a C or F as a last key we should use a whole natural key instead
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps01[12];
						} else if (key_nb % 12 == 2 || key_nb % 12 == 4 || key_nb % 12 == 7 || key_nb % 12 == 9 || key_nb % 12 == 11) {
							// if it's any other natural key we use E as the last key
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps02[4];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps01[4];
						} else {
							// the last key is a sharp!
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps02[1];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualWoodenKeysBmps01[1];
						}
					}
				} else {
					if (i == 0 && !m_keys[i].IsSharp) {
						// this is the first key and it's a natural so if it's an E or B then it should be displayed as a whole
						// otherwise it should be displayed as C unless next key also would be a natural for any reason
						bool nextIsNatural = false;
						if (m_displayKeys > 1) {
							nextIsNatural = (((key_nb % 12) < 5 && !(key_nb & 1)) || ((key_nb % 12) >= 5 && (key_nb & 1))) ? true : false;
						}
						if (key_nb % 12 == 4 || key_nb % 12 == 11 || nextIsNatural) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps01[12];
						} else {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps02[0];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps01[0];
						}
					} else if (i + 1 < m_displayKeys) {
						if (m_dispImageNum == 2)
							m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps02[key_nb % 12];
						else
							m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps01[key_nb % 12];
					} else {
						if (key_nb % 12 == 0 || key_nb % 12 == 5) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps01[12];
						} else if (key_nb % 12 == 2 || key_nb % 12 == 4 || key_nb % 12 == 7 || key_nb % 12 == 9 || key_nb % 12 == 11) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps02[4];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps01[4];
						} else {
							// the last key is a sharp!
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps02[1];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_invertedManualKeysBmps01[1];
						}
					}
				}
			} else {
				// this is a normal keyboard
				if (m_dispKeyColourWooden) {
					if (i == 0 && !m_keys[i].IsSharp) {
						// this is the first key and it's a natural so if it's an E or B then it should be displayed as a whole
						// otherwise it should be displayed as C unless next key also would be a natural for any reason
						bool nextIsNatural = false;
						if (m_displayKeys > 1) {
							nextIsNatural = (((key_nb % 12) < 5 && !(key_nb & 1)) || ((key_nb % 12) >= 5 && (key_nb & 1))) ? true : false;
						}
						if (key_nb % 12 == 4 || key_nb % 12 == 11 || nextIsNatural) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps01[12];
						} else {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps02[0];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps01[0];
						}
					} else if (i + 1 < m_displayKeys) {
						if (m_dispImageNum == 2)
							m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps02[key_nb % 12];
						else
							m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps01[key_nb % 12];
					} else {
						if (key_nb % 12 == 0 || key_nb % 12 == 5) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps01[12];
						} else if (key_nb % 12 == 2 || key_nb % 12 == 4 || key_nb % 12 == 7 || key_nb % 12 == 9 || key_nb % 12 == 11) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps02[4];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps01[4];
						} else {
							// the last key is a sharp!
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps02[1];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_woodenManualKeysBmps01[1];
						}
					}
				} else {
					if (i == 0 && !m_keys[i].IsSharp) {
						// this is the first key and it's a natural so if it's an E or B then it should be displayed as a whole
						// otherwise it should be displayed as C unless next key also would be a natural for any reason
						bool nextIsNatural = false;
						if (m_displayKeys > 1) {
							nextIsNatural = (((key_nb % 12) < 5 && !(key_nb & 1)) || ((key_nb % 12) >= 5 && (key_nb & 1))) ? true : false;
						}
						if (key_nb % 12 == 4 || key_nb % 12 == 11 || nextIsNatural) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps01[12];
						} else {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps02[0];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps01[0];
						}
					} else if (i + 1 < m_displayKeys) {
						if (m_dispImageNum == 2)
							m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps02[key_nb % 12];
						else
							m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps01[key_nb % 12];
					} else {
						if (key_nb % 12 == 0 || key_nb % 12 == 5) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps02[12];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps01[12];
						} else if (key_nb % 12 == 2 || key_nb % 12 == 4 || key_nb % 12 == 7 || key_nb % 12 == 9 || key_nb % 12 == 11) {
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps02[4];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps01[4];
						} else {
							// the last key is a sharp!
							if (m_dispImageNum == 2)
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps02[1];
							else
								m_keys[i].KeyImage = ::wxGetApp().m_manualKeyBmps01[1];
						}
					}
				}
			}
		}

		int overridingXOffset = 0;
		int overridingYOffset = 0;
		bool forceOffset = false;
		bool forceWidth = false;
		// default values can be overridden by general key types
		wxString typeName = m_availableKeytypes[key_nb % 12];
		if (i == 0)
			typeName = wxT("First") + typeName;
		else if (i + 1 == m_displayKeys)
			typeName = wxT("Last") + typeName;

		for (KEYTYPE& key : m_keytypes) {
			if (key.KeytypeIdentifier.IsSameAs(typeName)) {
				// a base type match is found!
				width = key.Width;
				overridingXOffset = key.Offset;
				overridingYOffset = key.YOffset;
				if (key.ImageOff.getImage() != wxEmptyString) {
					m_keys[i].KeyImage = key.ImageOff.getBitmap();
				}
				if (m_displayedAsPedal && !key.ForceWritingWidth && width == key.BitmapWidth) {
					// a pedal key of "e" or "b" should by default add another key width for the next key
					if ((key_nb % 12) == 4 || (key_nb % 12) == 11)
						width *= 2;
				}
				/*
				if (key.ForceWritingOffset) {
					forceOffset = true;
				}
				*/
				break;
			}
		}

		// and possibly finally by a specific matching Key999 entry
		wxString keyId = wxT("Key") + GOODF_functions::number_format(i + 1);
		for (KEYTYPE& key : m_keytypes) {
			if (key.KeytypeIdentifier.IsSameAs(keyId)) {
				// Key999 overrides any base type
				width = key.Width;
				overridingXOffset = key.Offset;
				overridingYOffset = key.YOffset;
				if (key.ImageOff.getImage() != wxEmptyString) {
					m_keys[i].KeyImage = key.ImageOff.getBitmap();
				}
				if (m_displayedAsPedal && !key.ForceWritingWidth && width == key.BitmapWidth) {
					// a pedal key of "e" or "b" should by default add another key width for the next key
					if ((key_nb % 12) == 4 || (key_nb % 12) == 11)
						width *= 2;
				}
				if (key.ForceWritingOffset) {
					forceOffset = true;
				}
				if (key.ForceWritingWidth) {
					forceWidth = true;
				}
				break;
			}
		}
		m_keys[i].Xpos += overridingXOffset;
		m_keys[i].Ypos += overridingYOffset;
		if (!m_displayedAsPedal) {
			if (m_keys[i].IsSharp && overridingXOffset == 0 && !forceOffset)
				m_keys[i].Xpos -= (width / 2);
		}

		if (!m_keys[i].IsSharp || m_displayedAsPedal || forceWidth)
			keyXpos += width;
	}
}

KEY_INFO* GUIManual::getKeyInfoAt(unsigned index) {
	return &(m_keys[index]);
}

void GUIManual::setDisplayAsPedal(bool isPedal) {
	m_displayedAsPedal = isPedal;
	updateKeyInfo();
}

bool GUIManual::isDisplayedAsPedal() {
	return m_displayedAsPedal;
}
