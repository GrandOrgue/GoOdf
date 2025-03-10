/*
 * GUILabel.cpp is part of GoOdf.
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

#include "GUILabel.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUILabel::GUILabel() {
	m_type = wxT("Label");
	m_freeXPlacement = true;
	m_freeYPlacement = true;
	m_dispXpos = 0;
	m_dispYpos = 0;
	m_dispAtTopOfDrawstopCol = false;
	m_dispDrawstopCol = 1;
	m_dispSpanDrawstopColToRight = false;
	m_dispLabelColour.setSelectedColorIndex(1);
	m_dispLabelFontSize.setSelectedSizeIndex(1);
	m_dispLabelFont = wxFont(wxFontInfo(7).FaceName(wxT("Arial")));
	m_name = wxEmptyString;
	m_dispImageNum = 1;
	m_image = GoImage();
	m_width = 80;
	m_height = 25;
	m_tileOffsetX = 0;
	m_tileOffsetY = 0;
	m_textRectLeft = 1;
	m_textRectTop = 1;
	m_textRectWidth = 79;
	m_textRectHeight = 24;
	m_textBreakWidth = 79;
	m_bitmapWidth = 80;
	m_bitmapHeight = 25;
}

GUILabel::~GUILabel() {

}

void GUILabel::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	if (!m_freeXPlacement)
		outFile->AddLine(wxT("FreeXPlacement=N"));
	if (!m_freeYPlacement)
		outFile->AddLine(wxT("FreeYPlacement=N"));
	if (m_dispXpos != 0 && m_freeXPlacement)
		outFile->AddLine(wxT("DispXpos=") + wxString::Format(wxT("%i"), m_dispXpos));
	if (m_dispYpos != 0 && m_freeYPlacement)
		outFile->AddLine(wxT("DispYpos=") + wxString::Format(wxT("%i"), m_dispYpos));
	if (m_positionX != -1 && m_freeXPlacement)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1 && m_freeYPlacement)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	if (!m_freeYPlacement) {
		if (m_dispAtTopOfDrawstopCol)
			outFile->AddLine(wxT("DispAtTopOfDrawstopCol=Y"));
		else
			outFile->AddLine(wxT("DispAtTopOfDrawstopCol=N"));
	}
	if (!m_freeXPlacement) {
		outFile->AddLine(wxT("DispDrawstopCol=") + wxString::Format(wxT("%i"), m_dispDrawstopCol));
		if (m_dispSpanDrawstopColToRight)
			outFile->AddLine(wxT("DispSpanDrawstopColToRight=Y"));
		else
			outFile->AddLine(wxT("DispSpanDrawstopColToRight=N"));
	}
	if (m_dispLabelColour.getSelectedColorIndex() != 1) {
		if (m_dispLabelColour.getSelectedColorIndex() > 0) {
			outFile->AddLine(wxT("DispLabelColour=") + m_dispLabelColour.getColorName());
		} else {
			outFile->AddLine(wxT("DispLabelColour=") + m_dispLabelColour.getHtmlValue());
		}
	}
	if (!m_dispLabelFontSize.getSizeName().IsSameAs(wxT("NORMAL"))) {
		if (m_dispLabelFontSize.getSelectedSizeIndex() < 3) {
			outFile->AddLine(wxT("DispLabelFontSize=") + m_dispLabelFontSize.getSizeName());
		} else {
			outFile->AddLine(wxT("DispLabelFontSize=") + wxString::Format(wxT("%i"), m_dispLabelFontSize.getSizeValue()));
		}
	}
	if (m_dispLabelFont.GetFaceName() != getOwningPanel()->getDisplayMetrics()->m_dispGroupLabelFont.GetFaceName())
		outFile->AddLine(wxT("DispLabelFontName=") + m_dispLabelFont.GetFaceName());
	if (m_name != wxEmptyString)
		outFile->AddLine(wxT("Name=") + m_name);
	if (m_dispImageNum != 1 && m_image.getImage() == wxEmptyString)
		outFile->AddLine(wxT("DispImageNum=") + wxString::Format(wxT("%i"), m_dispImageNum));
	if (m_image.getImage() != wxEmptyString)
		outFile->AddLine(GOODF_functions::fixSeparator(wxT("Image=") + m_image.getRelativeImagePath()));
	if (m_image.getMask() != wxEmptyString && m_image.getImage() != wxEmptyString)
		outFile->AddLine(GOODF_functions::fixSeparator(wxT("Mask=") + m_image.getRelativeMaskPath()));
	if (m_width != m_bitmapWidth)
		outFile->AddLine(wxT("Width=") + wxString::Format(wxT("%i"), m_width));
	if (m_height != m_bitmapHeight)
		outFile->AddLine(wxT("Height=") + wxString::Format(wxT("%i"), m_height));
	if (m_tileOffsetX != 0)
		outFile->AddLine(wxT("TileOffsetX=") + wxString::Format(wxT("%i"), m_tileOffsetX));
	if (m_tileOffsetY != 0)
		outFile->AddLine(wxT("TileOffsetY=") + wxString::Format(wxT("%i"), m_tileOffsetY));
	if (m_textRectLeft != 1)
		outFile->AddLine(wxT("TextRectLeft=") + wxString::Format(wxT("%i"), m_textRectLeft));
	if (m_textRectTop != 1)
		outFile->AddLine(wxT("TextRectTop=") + wxString::Format(wxT("%i"), m_textRectTop));
	if (m_textRectWidth != (m_width - m_textRectLeft))
		outFile->AddLine(wxT("TextRectWidth=") + wxString::Format(wxT("%i"), m_textRectWidth));
	if (m_textRectHeight != (m_height - m_textRectTop))
		outFile->AddLine(wxT("TextRectHeight=") + wxString::Format(wxT("%i"), m_textRectHeight));
	if (m_textBreakWidth != m_textRectWidth)
		outFile->AddLine(wxT("TextBreakWidth=") + wxString::Format(wxT("%i"), m_textBreakWidth));
}

void GUILabel::read(wxFileConfig *cfg, Organ *readOrgan) {
	wxString cfgBoolValue = cfg->Read("FreeXPlacement", wxEmptyString);
	m_freeXPlacement = GOODF_functions::parseBoolean(cfgBoolValue, true);
	cfgBoolValue = cfg->Read("FreeYPlacement", wxEmptyString);
	m_freeYPlacement = GOODF_functions::parseBoolean(cfgBoolValue, true);
	int xValue = static_cast<int>(cfg->ReadLong("DispXpos", 0));
	if (xValue > -1 && xValue <= getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()) {
		m_dispXpos = xValue;
	}
	int yValue = static_cast<int>(cfg->ReadLong("DispYpos", 0));
	if (yValue > -1 && yValue <= getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()) {
		m_dispYpos = yValue;
	}
	cfgBoolValue = cfg->Read("DispAtTopOfDrawstopCol", wxEmptyString);
	m_dispAtTopOfDrawstopCol = GOODF_functions::parseBoolean(cfgBoolValue, false);
	int col = static_cast<int>(cfg->ReadLong("DispDrawstopCol", 1));
	if (col > 0 && col <= getOwningPanel()->getDisplayMetrics()->m_dispDrawstopCols) {
		m_dispDrawstopCol = col;
	}
	cfgBoolValue = cfg->Read("DispSpanDrawstopColToRight", wxEmptyString);
	m_dispSpanDrawstopColToRight = GOODF_functions::parseBoolean(cfgBoolValue, false);
	wxString labelColor = cfg->Read("DispLabelColour", wxT("BLACK"));
	int colorIdx = m_dispLabelColour.getColorNames().Index(labelColor, false);
	if (colorIdx != wxNOT_FOUND) {
		m_dispLabelColour.setSelectedColorIndex(colorIdx);
	} else {
		wxColour theColor;
		theColor.Set(labelColor);
		if (theColor.IsOk())
			m_dispLabelColour.setColorValue(theColor);
	}
	wxString fontSize = cfg->Read("DispLabelFontSize", wxT("NORMAL"));
	int sizeIdx = m_dispLabelFontSize.getSizeNames().Index(fontSize, false);
	if (sizeIdx != wxNOT_FOUND) {
		m_dispLabelFontSize.setSelectedSizeIndex(sizeIdx);
	} else {
		// it's possible that it can be a numerical value instead
		long value;
		if (fontSize.ToLong(&value)) {
			if (value > 0 && value < 51)
				setDispLabelFontSize(value);
		}
	}
	wxString fontStr = cfg->Read("DispLabelFontName", wxEmptyString);
	wxFont labelFont = wxFont(wxFontInfo(getDispLabelFontSize()->getSizeValue()).FaceName(fontStr));
	if (fontStr != wxEmptyString && labelFont.IsOk() && labelFont.GetFaceName() == fontStr) {
		setDispLabelFont(labelFont);
	} else {
		labelFont = getOwningPanel()->getDisplayMetrics()->m_dispGroupLabelFont;
		labelFont.SetPointSize(m_dispLabelFontSize.getSizeValue());
		setDispLabelFont(labelFont);
	}
	m_name = cfg->Read("Name", wxEmptyString);
	int imgNum = static_cast<int>(cfg->ReadLong("DispImageNum", 1));
	if (imgNum > -1 && imgNum < 16) {
		m_dispImageNum = imgNum;
		if (imgNum == 0 || imgNum == 1 || imgNum == 3 || imgNum == 7 || imgNum == 10) {
			setBitmapWidth(80);
			setBitmapHeight(25);
		} else if (imgNum == 2 || imgNum == 6 || imgNum == 9) {
			setBitmapWidth(80);
			setBitmapHeight(50);
		} else if (imgNum == 4 || imgNum == 8 || imgNum == 11) {
			setBitmapWidth(160);
			setBitmapHeight(25);
		} else if (imgNum == 5 || imgNum == 12) {
			setBitmapWidth(200);
			setBitmapHeight(50);
		} else if (imgNum > 12 && imgNum < 16) {
			setBitmapWidth(400);
			setBitmapHeight(50);
		}
	}
	wxString img = cfg->Read("Image", wxEmptyString);
	wxString fullImgPath = GOODF_functions::checkIfFileExist(img, readOrgan);
	if (fullImgPath != wxEmptyString) {
		wxImage realImage = wxImage(fullImgPath);
		if (realImage.IsOk()) {
			m_image.setImage(fullImgPath);
			m_image.setOriginalWidth(realImage.GetWidth());
			setBitmapWidth(realImage.GetWidth());
			m_image.setOriginalHeight(realImage.GetHeight());
			setBitmapHeight(realImage.GetHeight());
		}
	}
	wxString mask = cfg->Read("Mask", wxEmptyString);
	wxString fullMaskPath = GOODF_functions::checkIfFileExist(mask, readOrgan);
	if (fullMaskPath != wxEmptyString) {
		m_image.setMask(fullMaskPath);
	}
	int pX = static_cast<int>(cfg->ReadLong("PositionX", -1));
	if (pX > -1 && pX <= getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()) {
		m_positionX = pX;
	}
	int pY = static_cast<int>(cfg->ReadLong("PositionY", -1));
	if (pY > -1 && pY <= getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()) {
		m_positionY = pY;
	}
	int width = static_cast<int>(cfg->ReadLong("Width", -1));
	if (width > 0 && width <= getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue()) {
		m_width = width;
	} else {
		m_width = m_bitmapWidth;
	}
	int height = static_cast<int>(cfg->ReadLong("Height", -1));
	if (height > 0 && height <= getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue()) {
		m_height = height;
	} else {
		m_height = m_bitmapHeight;
	}
	int tileX = static_cast<int>(cfg->ReadLong("TileOffsetX", 0));
	if (tileX > -1 && tileX < m_bitmapWidth) {
		m_tileOffsetX = tileX;
	}
	int tileY = static_cast<int>(cfg->ReadLong("TileOffsetY", 0));
	if (tileY > -1 && tileY < m_bitmapHeight) {
		m_tileOffsetY = tileY;
	}
	int textLeft = static_cast<int>(cfg->ReadLong("TextRectLeft", 1));
	if (textLeft > -1 && textLeft < m_width) {
		m_textRectLeft = textLeft;
	}
	int textTop = static_cast<int>(cfg->ReadLong("TextRectTop", 1));
	if (textTop > -1 && textTop < m_height) {
		m_textRectTop = textTop;
	}
	int textWidth = static_cast<int>(cfg->ReadLong("TextRectWidth", (m_width - m_textRectLeft)));
	if (textWidth >= 0 && textWidth <= (m_width - m_textRectLeft)) {
		m_textRectWidth = textWidth;
	}
	int textHeight = static_cast<int>(cfg->ReadLong("TextRectHeight", (m_height - m_textRectTop)));
	if (textHeight >= 0 && textHeight <= (m_height - m_textRectTop)) {
		m_textRectHeight = textHeight;
	}
	int breakWidth = static_cast<int>(cfg->ReadLong("TextBreakWidth", m_textRectWidth));
	if (breakWidth >= 0 && breakWidth <= m_textRectWidth) {
		m_textBreakWidth = breakWidth;
	}
}

GUILabel* GUILabel::clone() {
	return new GUILabel(*this);
}

void GUILabel::updateDisplayName() {
	if (m_type.IsSameAs(wxT("Label"))) {
		if (m_name != wxEmptyString) {
			wxString dispName;
			if (m_name.Len() > 13)
				dispName = m_name.Left(10) + wxT("...");
			else
				dispName = m_name;
			setDisplayName(wxT("GUI Label (") + dispName + wxT(")"));
			} else {
			setDisplayName(wxT("GUI Label"));
		}
	}
}

bool GUILabel::isDispAtTopOfDrawstopCol() const {
	return m_dispAtTopOfDrawstopCol;
}

void GUILabel::setDispAtTopOfDrawstopCol(bool dispAtTopOfDrawstopCol) {
	m_dispAtTopOfDrawstopCol = dispAtTopOfDrawstopCol;
}

int GUILabel::getDispDrawstopCol() const {
	return m_dispDrawstopCol;
}

void GUILabel::setDispDrawstopCol(int dispDrawstopCol) {
	m_dispDrawstopCol = dispDrawstopCol;
}

int GUILabel::getDispImageNum() const {
	return m_dispImageNum;
}

void GUILabel::setDispImageNum(int dispImageNum) {
	m_dispImageNum = dispImageNum;
}

GoColor* GUILabel::getDispLabelColour() {
	return &m_dispLabelColour;
}

void GUILabel::setDispLabelColour(const GoColor &dispLabelColour) {
	m_dispLabelColour = dispLabelColour;
}

GoFontSize* GUILabel::getDispLabelFontSize() {
	return &m_dispLabelFontSize;
}

void GUILabel::setDispLabelFontSize(int size) {
	m_dispLabelFontSize.setSizeValue(size);
	m_dispLabelFont.SetPointSize(m_dispLabelFontSize.getSizeValue());
}

wxFont GUILabel::getDispLabelFont() {
	return m_dispLabelFont;
}

void GUILabel::setDispLabelFont(wxFont font) {
	m_dispLabelFont = font;
}

bool GUILabel::isDispSpanDrawstopColToRight() const {
	return m_dispSpanDrawstopColToRight;
}

void GUILabel::setDispSpanDrawstopColToRight(bool dispSpanDrawstopColToRight) {
	m_dispSpanDrawstopColToRight = dispSpanDrawstopColToRight;
}

int GUILabel::getDispXpos() const {
	return m_dispXpos;
}

void GUILabel::setDispXpos(int dispXpos) {
	m_dispXpos = dispXpos;
}

int GUILabel::getDispYpos() const {
	return m_dispYpos;
}

void GUILabel::setDispYpos(int dispYpos) {
	m_dispYpos = dispYpos;
}

bool GUILabel::isFreeXPlacement() const {
	return m_freeXPlacement;
}

void GUILabel::setFreeXPlacement(bool freeXPlacement) {
	m_freeXPlacement = freeXPlacement;
}

bool GUILabel::isFreeYPlacement() const {
	return m_freeYPlacement;
}

void GUILabel::setFreeYPlacement(bool freeYPlacement) {
	m_freeYPlacement = freeYPlacement;
}

int GUILabel::getHeight() const {
	return m_height;
}

void GUILabel::setHeight(int height) {
	m_height = height;
}

GoImage* GUILabel::getImage() {
	return &m_image;
}

const wxString& GUILabel::getName() const {
	return m_name;
}

void GUILabel::setName(const wxString &name) {
	m_name = name;
}

int GUILabel::getTextBreakWidth() const {
	return m_textBreakWidth;
}

void GUILabel::setTextBreakWidth(int textBreakWidth) {
	m_textBreakWidth = textBreakWidth;
}

int GUILabel::getTextRectHeight() const {
	return m_textRectHeight;
}

void GUILabel::setTextRectHeight(int textRectHeight) {
	m_textRectHeight = textRectHeight;
}

int GUILabel::getTextRectLeft() const {
	return m_textRectLeft;
}

void GUILabel::setTextRectLeft(int textRectLeft) {
	m_textRectLeft = textRectLeft;
}

int GUILabel::getTextRectTop() const {
	return m_textRectTop;
}

void GUILabel::setTextRectTop(int textRectTop) {
	m_textRectTop = textRectTop;
}

int GUILabel::getTextRectWidth() const {
	return m_textRectWidth;
}

void GUILabel::setTextRectWidth(int textRectWidth) {
	m_textRectWidth = textRectWidth;
}

int GUILabel::getTileOffsetX() const {
	return m_tileOffsetX;
}

void GUILabel::setTileOffsetX(int tileOffsetX) {
	m_tileOffsetX = tileOffsetX;
}

int GUILabel::getTileOffsetY() const {
	return m_tileOffsetY;
}

void GUILabel::setTileOffsetY(int tileOffsetY) {
	m_tileOffsetY = tileOffsetY;
}

int GUILabel::getWidth() const {
	return m_width;
}

void GUILabel::setWidth(int width) {
	m_width = width;
}

int GUILabel::getBitmapWidth() {
	return m_bitmapWidth;
}

void GUILabel::setBitmapWidth(int width) {
	m_bitmapWidth = width;
}

int GUILabel::getBitmapHeight() {
	return m_bitmapHeight;
}

void GUILabel::setBitmapHeight(int height) {
	m_bitmapHeight = height;
}

void GUILabel::setDefaultFont(wxFont &theFont) {
	m_dispLabelFont = theFont;
	m_dispLabelFontSize.setSizeValue(theFont.GetPointSize());
}

wxBitmap GUILabel::getBitmap() {
	if (m_image.getImage() == wxEmptyString)
		return ::wxGetApp().m_fullSizeLabelBmps[m_dispImageNum];
	else
		return m_image.getBitmap();
}
