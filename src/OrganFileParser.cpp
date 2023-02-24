/*
 * OrganFileParser.cpp is part of GOODF.
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

#include "OrganFileParser.h"
#include <wx/filename.h>
#include <wx/image.h>
#include "GOODFFunctions.h"

OrganFileParser::OrganFileParser(wxString filePath, Organ *organ) {
	m_filePath = filePath;
	m_organ = organ;
	m_fileIsOk = false;
	m_organIsReady = false;
	m_isUsingOldPanelFormat = false;
	m_errorMessage = wxEmptyString;

	readIniFile();
	if (m_fileIsOk)
		parseOrgan();
}

OrganFileParser::~OrganFileParser() {

}

void OrganFileParser::parseOrgan() {
	wxFileName odf = wxFileName(m_filePath);
	m_organ->setOdfRoot(odf.GetPath());
	parseOrganSection();
}

bool OrganFileParser::isOrganReady() {
	return m_organIsReady;
}

void OrganFileParser::readIniFile() {
	m_organFile = wxFileConfig(wxEmptyString, wxEmptyString, m_filePath);
	if (m_organFile.HasGroup(wxT("Organ")))
		m_fileIsOk = true;
	else {
		m_fileIsOk = false;
		m_errorMessage = wxT("No [Organ] section could be found in file!");
	}
	if (m_organFile.HasGroup(wxT("Panel000"))) {
		m_isUsingOldPanelFormat = false;
	} else {
		m_isUsingOldPanelFormat = true;
	}
}

bool OrganFileParser::parseBoolean(wxString key, bool defaultValue) {
	wxString value = m_organFile.Read(key, wxEmptyString);
	if (value.IsSameAs(wxT("Y"), false))
		return true;
	if (value.IsSameAs(wxT("N"), false))
		return false;
	return defaultValue;
}

wxString OrganFileParser::checkIfFileExist(wxString relativePath) {
	// checks if a file actually exist relative the organ path and if so return the full path, or else an empty string
	wxString returnPath = wxEmptyString;
	if (relativePath != wxEmptyString) {
		wxString fullFilePath = m_organ->getOdfRoot() + wxFILE_SEP_PATH + relativePath;
		wxFileName theFile = wxFileName(fullFilePath);
		if (theFile.FileExists()) {
			returnPath = theFile.GetFullPath();
		}
	}
	return returnPath;
}

void OrganFileParser::parseOrganSection() {
	m_organFile.SetPath("/Organ");

	m_organ->setChurchName(m_organFile.Read("ChurchName", wxEmptyString));
	m_organ->setChurchAddress(m_organFile.Read("ChurchAddress", wxEmptyString));
	m_organ->setOrganBuilder(m_organFile.Read("OrganBuilder", wxEmptyString));
	m_organ->setOrganBuildDate(m_organFile.Read("OrganBuildDate", wxEmptyString));
	m_organ->setOrganComments(m_organFile.Read("OrganComments", wxEmptyString));
	m_organ->setRecordingDetails(m_organFile.Read("RecordingDetails", wxEmptyString));
	wxString infoFilename = m_organFile.Read("InfoFilename", wxEmptyString);
	wxString infoFile = checkIfFileExist(infoFilename);
	if (infoFile != wxEmptyString) {
		m_organ->setInfoFilename(infoFile);
	}
	m_organ->setDivisionalsStoreIntermanualCouplers(parseBoolean(wxT("DivisionalsStoreIntermanualCouplers"), true));
	m_organ->setDivisionalsStoreIntramanualCouplers(parseBoolean(wxT("DivisionalsStoreIntramanualCouplers"), true));
	m_organ->setDivisionalsStoreTremulants(parseBoolean(wxT("DivisionalsStoreTremulants"), true));
	m_organ->setGeneralsStoreDivisionalCouplers(parseBoolean(wxT("GeneralsStoreDivisionalCouplers"), true));
	m_organ->setCombinationsStoreNonDisplayedDrawstops(parseBoolean(wxT("CombinationsStoreNonDisplayedDrawstops"), true));
	float ampLvl = static_cast<float>(m_organFile.ReadDouble("AmplitudeLevel", 100.0f));
	if (ampLvl >= 0.0f && ampLvl <= 1000.0f)
		m_organ->setAmplitudeLevel(ampLvl);
	float gain = static_cast<float>(m_organFile.ReadDouble("Gain", 0.0f));
	if (gain >= -120.0f && gain <= 40.0f)
		m_organ->setGain(gain);
	float pitchTuning = static_cast<float>(m_organFile.ReadDouble("PitchTuning", 0.0f));
	if (pitchTuning >= -1800.0f && pitchTuning <= 1800.0f)
		m_organ->setPitchTuning(pitchTuning);
	float pitchCorrection = static_cast<float>(m_organFile.ReadDouble("PitchCorrection", 0.0f));
	if (pitchCorrection >= -1800.0f && pitchCorrection <= 1800.0f)
		m_organ->setPitchCorrection(pitchCorrection);
	long trackerDelay = m_organFile.ReadLong("TrackerDelay", 0);
	if (trackerDelay >= 0 && trackerDelay <= 10000) {
		m_organ->setTrackerDelay(static_cast<unsigned>(trackerDelay));
	}
	m_organ->setHasPedals(parseBoolean(wxT("HasPedals"), false));

	if (m_isUsingOldPanelFormat) {
		// the display metrics must be read from the organ section
		parseDisplayMetrics(wxT("/Organ"), m_organ->getOrganPanelAt(0));

		// images can also exist that must be transferred to the main panel
		int nbrImages = static_cast<int>(m_organFile.ReadLong("NumberOfImages", 0));
		if (nbrImages > 0 && nbrImages < 1000) {
			for (int i = 0; i < nbrImages; i++) {
				wxString imgGroupName = wxT("Image") + GOODF_functions::number_format(i);
				if (m_organFile.HasGroup(imgGroupName)) {
					parseImageSection(wxT("/") + imgGroupName, m_organ->getOrganPanelAt(0));
				}
			}
		}

		// setter elements can exist that should be re created as GUI elements

		// labels can exist that also should be re created as GUI elements
	}

	// parse enclosures
	int nbrEnclosures = static_cast<int>(m_organFile.ReadLong("NumberOfEnclosures", 0));
	if (nbrEnclosures > - 1 && nbrEnclosures < 51) {
		for (int i = 0; i < nbrEnclosures; i++) {
			wxString enclosureGroupName = wxT("Enclosure") + GOODF_functions::number_format(i);
			if (m_organFile.HasGroup(enclosureGroupName)) {
				parseEnclosureSection(wxT("/") + enclosureGroupName, m_organ->getOrganPanelAt(0));
			}
		}
	}

	// parse windchests

	// parse switches

	// parse tremulants

	// parse ranks

	// parse manuals which contain the stops, couplers and divisionals
	// also they have tremulants and switches even if they are globally managed

	// parse reversible pistons

	// parse divisional couplers

	// parse generals

	// parse panels
}

void OrganFileParser::parseDisplayMetrics(wxString sourcePanel, GoPanel *targetPanel) {
	m_organFile.SetPath(sourcePanel);

	wxString horizSize = m_organFile.Read("DispScreenSizeHoriz", wxEmptyString);
	if (horizSize != wxEmptyString) {
		if (horizSize.IsSameAs(wxT("SMALL"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(0, true);
		} else if (horizSize.IsSameAs(wxT("MEDIUM"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(1, true);
		} else if (horizSize.IsSameAs(wxT("MEDIUM LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(2, true);
		} else if (horizSize.IsSameAs(wxT("LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setSelectedNameIndex(3, true);
		} else {
			// The value is not a matching string so we try to convert it into a number
			long value = -1;
			if (horizSize.ToLong(&value)) {
				if (value > 99 && value < 4001) {
					targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.setNumericalValue(value);
				}
			}
		}
	}
	wxString vertSize = m_organFile.Read("DispScreenSizeVert", wxEmptyString);
	if (vertSize != wxEmptyString) {
		if (vertSize.IsSameAs(wxT("SMALL"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(0, true);
		} else if (vertSize.IsSameAs(wxT("MEDIUM"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(1, true);
		} else if (vertSize.IsSameAs(wxT("MEDIUM LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(2, true);
		} else if (vertSize.IsSameAs(wxT("LARGE"), false)) {
			targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setSelectedNameIndex(3, true);
		} else {
			// The value is not a matching string so we try to convert it into a number
			long value = -1;
			if (vertSize.ToLong(&value)) {
				if (value > 99 && value < 4001) {
					targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.setNumericalValue(value);
				}
			}
		}
	}
	int drawstopBgNum = static_cast<int>(m_organFile.ReadLong("DispDrawstopBackgroundImageNum", 1));
	if (drawstopBgNum > 0 && drawstopBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispDrawstopBackgroundImageNum = drawstopBgNum;
	int consoleBgNum = static_cast<int>(m_organFile.ReadLong("DispConsoleBackgroundImageNum", 1));
	if (consoleBgNum > 0 && consoleBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispConsoleBackgroundImageNum = consoleBgNum;
	int keyHorizBgNum = static_cast<int>(m_organFile.ReadLong("DispKeyHorizBackgroundImageNum", 1));
	if (keyHorizBgNum > 0 && keyHorizBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispKeyHorizBackgroundImageNum = keyHorizBgNum;
	int keyVertBgNum = static_cast<int>(m_organFile.ReadLong("DispKeyVertBackgroundImageNum", 1));
	if (keyVertBgNum > 0 && keyVertBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispKeyVertBackgroundImageNum = keyVertBgNum;
	int insetBgNum = static_cast<int>(m_organFile.ReadLong("DispDrawstopInsetBackgroundImageNum", 1));
	if (insetBgNum > 0 && insetBgNum < 65)
		targetPanel->getDisplayMetrics()->m_dispDrawstopInsetBackgroundImageNum = insetBgNum;
	wxString fontName = m_organFile.Read("DispControlLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		targetPanel->getDisplayMetrics()->m_dispControlLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	fontName = m_organFile.Read("DispShortcutKeyLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		targetPanel->getDisplayMetrics()->m_dispShortcutKeyLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	fontName = m_organFile.Read("DispGroupLabelFont", wxEmptyString);
	if (fontName != wxEmptyString)
		targetPanel->getDisplayMetrics()->m_dispGroupLabelFont = wxFont(wxFontInfo(7).FaceName(fontName));
	int drawstopCols = static_cast<int>(m_organFile.ReadLong("DispDrawstopCols", 2));
	if (drawstopCols > 1 && drawstopCols < 13 && drawstopCols % 2 == 0)
		targetPanel->getDisplayMetrics()->m_dispDrawstopCols = drawstopCols;
	int drawstopRows = static_cast<int>(m_organFile.ReadLong("DispDrawstopRows", 1));
	if (drawstopRows > 0 && drawstopRows < 21)
		targetPanel->getDisplayMetrics()->m_dispDrawstopRows = drawstopRows;
	targetPanel->getDisplayMetrics()->m_dispDrawstopColsOffset = parseBoolean(wxT("DispDrawstopColsOffset"), false);
	targetPanel->getDisplayMetrics()->m_dispDrawstopOuterColOffsetUp = parseBoolean(wxT("DispDrawstopOuterColOffsetUp"), false);
	targetPanel->getDisplayMetrics()->m_dispPairDrawstopCols = parseBoolean(wxT("DispPairDrawstopCols"), false);
	int extraDrawstopRows = static_cast<int>(m_organFile.ReadLong("DispExtraDrawstopRows", 0));
	if (extraDrawstopRows > -1 && extraDrawstopRows < 100)
		targetPanel->getDisplayMetrics()->m_dispExtraDrawstopRows = extraDrawstopRows;
	int extraDrawstopCols = static_cast<int>(m_organFile.ReadLong("DispExtraDrawstopCols", 0));
	if (extraDrawstopCols > -1 && extraDrawstopCols < 41)
		targetPanel->getDisplayMetrics()->m_dispExtraDrawstopCols = extraDrawstopCols;
	int btnCols = static_cast<int>(m_organFile.ReadLong("DispButtonCols", 1));
	if (btnCols > 0 && btnCols < 33)
		targetPanel->getDisplayMetrics()->m_dispButtonCols = btnCols;
	int extraBtnRows = static_cast<int>(m_organFile.ReadLong("DispExtraButtonRows", 0));
	if (extraBtnRows > -1 && extraBtnRows < 100)
		targetPanel->getDisplayMetrics()->m_dispExtraButtonRows = extraBtnRows;
	targetPanel->getDisplayMetrics()->m_dispExtraPedalButtonRow = parseBoolean(wxT("DispExtraPedalButtonRow"), false);
	targetPanel->getDisplayMetrics()->m_dispExtraPedalButtonRowOffset = parseBoolean(wxT("DispExtraPedalButtonRowOffset"), false);
	targetPanel->getDisplayMetrics()->m_dispExtraPedalButtonRowOffsetRight = parseBoolean(wxT("DispExtraPedalButtonRowOffsetRight"), false);
	targetPanel->getDisplayMetrics()->m_dispButtonsAboveManuals = parseBoolean(wxT("DispButtonsAboveManuals"), false);
	targetPanel->getDisplayMetrics()->m_dispTrimAboveManuals = parseBoolean(wxT("DispTrimAboveManuals"), false);
	targetPanel->getDisplayMetrics()->m_dispTrimBelowManuals = parseBoolean(wxT("DispTrimBelowManuals"), false);
	targetPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows = parseBoolean(wxT("DispTrimAboveExtraRows"), false);
	targetPanel->getDisplayMetrics()->m_dispExtraDrawstopRowsAboveExtraButtonRows = parseBoolean(wxT("DispExtraDrawstopRowsAboveExtraButtonRows"), false);
	int drawstopWidth = static_cast<int>(m_organFile.ReadLong("DispDrawstopWidth", 78));
	if (drawstopWidth > 0 && drawstopWidth < 151)
		targetPanel->getDisplayMetrics()->m_dispDrawstopWidth = drawstopWidth;
	int drawstopHeight = static_cast<int>(m_organFile.ReadLong("DispDrawstopHeight", 69));
	if (drawstopHeight > 0 && drawstopHeight < 151)
		targetPanel->getDisplayMetrics()->m_dispDrawstopHeight = drawstopHeight;
	int pistonWidth = static_cast<int>(m_organFile.ReadLong("DispPistonWidth", 44));
	if (pistonWidth > 0 && pistonWidth < 151)
		targetPanel->getDisplayMetrics()->m_dispPistonWidth = pistonWidth;
	int pistonHeight = static_cast<int>(m_organFile.ReadLong("DispPistonHeight", 40));
	if (pistonHeight > 0 && pistonHeight < 151)
		targetPanel->getDisplayMetrics()->m_dispPistonHeight = pistonHeight;
	int enclosureWidth = static_cast<int>(m_organFile.ReadLong("DispEnclosureWidth", 52));
	if (enclosureWidth > 0 && enclosureWidth < 151)
		targetPanel->getDisplayMetrics()->m_dispEnclosureWidth = enclosureWidth;
	int enclosureHeight = static_cast<int>(m_organFile.ReadLong("DispEnclosureHeight", 63));
	if (enclosureHeight > 0 && enclosureHeight < 151)
		targetPanel->getDisplayMetrics()->m_dispEnclosureHeight = enclosureHeight;
	int pedalHeight = static_cast<int>(m_organFile.ReadLong("DispPedalHeight", 40));
	if (pedalHeight > 0 && pedalHeight < 501)
		targetPanel->getDisplayMetrics()->m_dispPedalHeight = pedalHeight;
	int pedalKeyWidth = static_cast<int>(m_organFile.ReadLong("DispPedalKeyWidth", 7));
	if (pedalKeyWidth > 0 && pedalKeyWidth < 501)
		targetPanel->getDisplayMetrics()->m_dispPedalKeyWidth = pedalKeyWidth;
	int manualHeight = static_cast<int>(m_organFile.ReadLong("DispManualHeight", 32));
	if (manualHeight > 0 && manualHeight < 501)
		targetPanel->getDisplayMetrics()->m_dispManualHeight = manualHeight;
	int manualKeyWidth = static_cast<int>(m_organFile.ReadLong("DispManualKeyWidth", 12));
	if (manualKeyWidth > 0 && manualKeyWidth < 501)
		targetPanel->getDisplayMetrics()->m_dispManualKeyWidth = manualKeyWidth;
}

void OrganFileParser::parseImageSection(wxString source, GoPanel *targetPanel) {
	bool imageIsValid = false;
	m_organFile.SetPath(source);
	GoImage image;
	image.setOwningPanelWidth(targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue());
	image.setOwningPanelHeight(targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	wxString relImgPath = m_organFile.Read("Image", wxEmptyString);
	wxString imgPath = checkIfFileExist(relImgPath);
	if (imgPath != wxEmptyString) {
		wxImage img = wxImage(imgPath);
		if (img.IsOk()) {
			imageIsValid = true;
			image.setImage(imgPath);
			int width = img.GetWidth();
			int height = img.GetHeight();
			image.setOriginalWidth(width);
			image.setOriginalHeight(height);
			int imgWidth = static_cast<int>(m_organFile.ReadLong("Width", width));
			if (imgWidth > -1 && imgWidth < image.getOwningPanelWidth()) {
				// the width value read is in valid range
				image.setWidth(imgWidth);
			} else {
				image.setWidth(width);
			}
			int imgHeight = static_cast<int>(m_organFile.ReadLong("Height", height));
			if (imgHeight > -1 && imgHeight < image.getOwningPanelHeight()) {
				image.setHeight(imgHeight);
			} else {
				image.setHeight(height);
			}
		}
		wxString relMaskPath = m_organFile.Read("Mask", wxEmptyString);
		wxString maskPath = checkIfFileExist(relMaskPath);
		if (maskPath != wxEmptyString) {
			wxImage mask = wxImage(maskPath);
			if (mask.IsOk()) {
				int width = mask.GetWidth();
				int height = mask.GetHeight();
				if (width == image.getOriginalWidth() && height == image.getOriginalHeight()) {
					image.setMask(maskPath);
				}
			}
		}
		int posX = static_cast<int>(m_organFile.ReadLong("PositionX", 0));
		if (posX > -1 && posX < targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()) {
			image.setPositionX(posX);
		}
		int posY = static_cast<int>(m_organFile.ReadLong("PositionY", 0));
		if (posY > -1 && posY < targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()) {
			image.setPositionY(posY);
		}
		int tileOffsetX = static_cast<int>(m_organFile.ReadLong("TileOffsetX", 0));
		if (tileOffsetX > -1 && tileOffsetX < image.getOriginalWidth()) {
			image.setTileOffsetX(tileOffsetX);
		}
		int tileOffsetY = static_cast<int>(m_organFile.ReadLong("TileOffsetY", 0));
		if (tileOffsetY > -1 && tileOffsetY < image.getOriginalHeight()) {
			image.setTileOffsetY(tileOffsetY);
		}
	}

	if (imageIsValid)
		targetPanel->addImage(image);
}

void OrganFileParser::parseEnclosureSection(wxString source, GoPanel *targetPanel) {
	m_organFile.SetPath(source);
	Enclosure enc;
	enc.setName(m_organFile.Read("Name", wxEmptyString));
	int ampMinLvl = static_cast<int>(m_organFile.ReadLong("AmpMinimumLevel", 1));
	if (ampMinLvl > -1 && ampMinLvl < 101)
		enc.setAmpMinimumLevel(ampMinLvl);
	int midiInput = static_cast<int>(m_organFile.ReadLong("MIDIInputNumber", 0));
	if (midiInput > -1 && midiInput < 201)
		enc.setMidiInputNumber(midiInput);

	m_organ->addEnclosure(enc);

	// the enclosure section can also contain GUI element data that must be taken into account
	bool defaultDisplayed = false;
	if (m_isUsingOldPanelFormat) {
		defaultDisplayed = true;
	}

	bool encIsDisplayed = parseBoolean(wxT("Displayed"), defaultDisplayed);
	if (encIsDisplayed) {
		int lastEnclosureIdx = m_organ->getNumberOfEnclosures() - 1;
		createGUIEnclosure(source, targetPanel, m_organ->getOrganEnclosureAt(lastEnclosureIdx));
	}
}

void OrganFileParser::createGUIEnclosure(wxString source, GoPanel *targetPanel, Enclosure *enclosure) {
	m_organFile.SetPath(source);
	GUIElement *guiEnc = new GUIEnclosure(enclosure);
	guiEnc->setOwningPanel(targetPanel);
	guiEnc->setDisplayName(enclosure->getName());
	targetPanel->addGuiElement(guiEnc);

	// convert gui element back to enclosure type
	GUIEnclosure *encElement = dynamic_cast<GUIEnclosure*>(guiEnc);
	if (encElement) {
		wxString colorStr = m_organFile.Read("DispLabelColour", wxT("WHITE"));
		int colorIdx = encElement->getDispLabelColour()->getColorNames().Index(colorStr, false);
		if (colorIdx != wxNOT_FOUND) {
			encElement->getDispLabelColour()->setSelectedColorIndex(colorIdx);
		} else {
			// is it possible that it's a html format color code instead
			wxColour color;
			color.Set(colorStr);
			if (color.IsOk()) {
				encElement->getDispLabelColour()->setColorValue(color);
			}
		}
		wxString sizeStr = m_organFile.Read("DispLabelFontSize", wxT("7"));
		int sizeIdx = encElement->getDispLabelFontSize()->getSizeNames().Index(sizeStr, false);
		if (sizeIdx != wxNOT_FOUND) {
			encElement->getDispLabelFontSize()->setSelectedSizeIndex(sizeIdx);
		} else {
			// it's possible that it can be a numerical value instead
			long value;
			if (sizeStr.ToLong(&value)) {
				if (value > 0 && value < 51)
					encElement->setDispLabelFontSize(value);
			}
		}
		wxFont labelFont(wxFontInfo(encElement->getDispLabelFontSize()->getSizeValue()).FaceName(m_organFile.Read("DispLabelFontName", wxEmptyString)));
		if (labelFont.IsOk())
			encElement->setDispLabelFont(labelFont);
		encElement->setDispLabelText(m_organFile.Read("DispLabelText", wxEmptyString));
		int encStyle = static_cast<int>(m_organFile.ReadLong("EnclosureStyle", 1));
		if (encStyle > 0 && encStyle < 5) {
			encElement->setEnclosureStyle(encStyle);
		}
		int bitmapCount = static_cast<int>(m_organFile.ReadLong("BitmapCount", 0));
		if (bitmapCount > 0 && bitmapCount < 129) {
			for (int i = 0; i < bitmapCount; i++) {
				GoImage tmpBmp;
				wxString bmpStr = wxT("Bitmap") + GOODF_functions::number_format(i + 1);
				wxString maskStr = wxT("Mask") + GOODF_functions::number_format(i + 1);
				wxString relBmpPath = m_organFile.Read(bmpStr, wxEmptyString);
				wxString fullBmpPath = checkIfFileExist(relBmpPath);
				wxString relMaskPath = m_organFile.Read(maskStr, wxEmptyString);
				wxString fullMaskPath = checkIfFileExist(relMaskPath);
				if (fullBmpPath != wxEmptyString) {
					wxImage img = wxImage(fullBmpPath);
					if (img.IsOk()) {
						tmpBmp.setImage(fullBmpPath);
						if (fullMaskPath != wxEmptyString) {
							wxImage mask = wxImage(fullMaskPath);
							if (mask.IsOk()) {
								tmpBmp.setMask(fullMaskPath);
							}
						}
						if (encElement->getNumberOfBitmaps() == 0) {
							// from the first bitmap we can store the original size values
							int width = img.GetWidth();
							int height = img.GetHeight();
							tmpBmp.setOriginalWidth(width);
							tmpBmp.setOriginalHeight(height);
							encElement->setBitmapWidth(width);
							encElement->setBitmapHeight(height);
						}
						encElement->addBitmap(tmpBmp);
					}
				}
			}
		}
		int thePanelWidth = targetPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
		int thePanelHeight = targetPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();
		int posX = static_cast<int>(m_organFile.ReadLong("PositionX", 0));
		if (posX > -1 && posX < thePanelWidth)
			encElement->setPosX(posX);
		int posY = static_cast<int>(m_organFile.ReadLong("PositionY", 0));
		if (posY > -1 && posY < thePanelHeight)
			encElement->setPosY(posY);
		int encWidth = static_cast<int>(m_organFile.ReadLong("Width", 0));
		if (encWidth > -1 && encWidth < thePanelWidth) {
			encElement->setWidth(encWidth);
		}
		int encHeight = static_cast<int>(m_organFile.ReadLong("Height", 0));
		if (encHeight > -1 && encHeight < thePanelHeight) {
			encElement->setHeight(encHeight);
		}
		int tileX = static_cast<int>(m_organFile.ReadLong("TileOffsetX", 0));
		if (tileX > -1 && tileX < encElement->getBitmapWidth() + 1) {
			encElement->setTileOffsetX(tileX);
		}
		int tileY = static_cast<int>(m_organFile.ReadLong("TileOffsetY", 0));
		if (tileY > -1 && tileY < encElement->getBitmapHeight() + 1) {
			encElement->setTileOffsetY(tileY);
		}
		int mouseRectLeft = static_cast<int>(m_organFile.ReadLong("MouseRectLeft", 0));
		if (mouseRectLeft > -1 && mouseRectLeft < encElement->getWidth() + 1) {
			encElement->setMouseRectLeft(mouseRectLeft);
		}
		int mouseRectTop = static_cast<int>(m_organFile.ReadLong("MouseRectTop", 0));
		if (mouseRectTop > -1 && mouseRectTop < encElement->getHeight() + 1) {
			encElement->setMouseRectTop(mouseRectTop);
		}
		int mouseRectWidth = static_cast<int>(m_organFile.ReadLong("MouseRectWidth", 0));
		if (mouseRectWidth > -1 && mouseRectWidth < encElement->getWidth() + 1) {
			encElement->setMouseRectWidth(mouseRectWidth);
		}
		int mouseRectHeight = static_cast<int>(m_organFile.ReadLong("MouseRectHeight", 0));
		if (mouseRectHeight > -1 && mouseRectHeight < encElement->getHeight() + 1) {
			encElement->setMouseRectHeight(mouseRectHeight);
		}
		int mouseAxisStart = static_cast<int>(m_organFile.ReadLong("MouseAxisStart", 0));
		if (mouseAxisStart > - 1 && mouseAxisStart < encElement->getMouseRectHeight() + 1) {
			encElement->setMouseAxisStart(mouseAxisStart);
		}
		int mouseAxisEnd = static_cast<int>(m_organFile.ReadLong("MouseAxisEnd", 0));
		if (mouseAxisEnd > -1 && mouseAxisEnd < encElement->getMouseRectHeight() + 1) {
			encElement->setMouseAxisEnd(mouseAxisEnd);
		}
		int textRectLeft = static_cast<int>(m_organFile.ReadLong("TextRectLeft", 0));
		if (textRectLeft > -1 && textRectLeft < encElement->getWidth() + 1) {
			encElement->setTextRectLeft(textRectLeft);
		}
		int textRectTop = static_cast<int>(m_organFile.ReadLong("TextRectTop", 0));
		if (textRectTop > -1 && textRectTop < encElement->getHeight() + 1) {
			encElement->setTextRectTop(textRectTop);
		}
		int textRectWidth = static_cast<int>(m_organFile.ReadLong("TextRectWidth", 0));
		if (textRectWidth > -1 && textRectWidth < encElement->getWidth() + 1) {
			encElement->setTextRectWidth(textRectWidth);
		}
		int textRectHeight = static_cast<int>(m_organFile.ReadLong("TextRectHeight", 0));
		if (textRectHeight > -1 && textRectHeight < encElement->getHeight() + 1) {
			encElement->setTextRectHeight(textRectHeight);
		}
		int textBreakWidth = static_cast<int>(m_organFile.ReadLong("TextBreakWidth", encElement->getTextRectWidth()));
		if (textBreakWidth > -1 && textBreakWidth < encElement->getTextRectWidth() + 1) {
			encElement->setTextBreakWidth(textBreakWidth);
		}
	}
}
