/*
 * GUIButton.cpp is part of GoOdf.
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

#include "GUIButton.h"
#include "GOODFFunctions.h"
#include "GOODF.h"
#include <algorithm>

GUIButton::GUIButton() : GUIElement() {
	m_displayAsPiston = false;
	m_dispLabelColour.setSelectedColorIndex(9); // (GoColor, default: Dark Red)
	m_dispLabelFontSize = GoFontSize();
	m_dispLabelFont = wxFont(wxFontInfo(7).FaceName(wxT("Arial")));
	m_dispLabelText = wxEmptyString; // (string, default: Name of the button)
	m_dispKeyLabelOnLeft = true; // (boolean, default: true)
	m_dispImageNum = 1; // (integer 1- type dependent, default: see below) Builtin bitmap set to use. GrandOrgue has 7 for drawstops and 5 for pistons. The default is 3 (piston) or 4 (drawstops) for read-only buttons, otherwise the default is 1.
	m_dispButtonRow = 1; // (button row, default: 1)
	m_dispButtonCol = 1; // (button column, default: 1)
	m_dispDrawstopRow = 1; // (drawstop row, default: 1)
	m_dispDrawstopCol = 1; // (drawstop column, default: 1)
	m_imageOn = wxEmptyString; // (string, default: use internal bitmap according to DispImageNum)
	m_imageOff = wxEmptyString; // (string, default: use internal bitmap according to DispImageNum)
	m_maskOn = wxEmptyString; // (string, default: empty)
	m_maskOff = wxEmptyString; // (string, default: value of MaskOn)
	m_tileOffsetX = 0; // (integer 0 - bitmap width, default: 0)
	m_tileOffsetY = 0; // (integer 0 - bitmap width, default: 0)
	m_mouseRectLeft = 0; // (integer 0 - Width, default: 0)
	m_mouseRectTop = 0; // (integer 0 - Height, default: 0)
	m_textRectLeft = 1; // (integer 0 - Width, default: 1)
	m_textRectTop = 1; // (integer 0 - Height, default: 1)
	updateBuiltinBitmapValues();
}

GUIButton::~GUIButton() {

}

void GUIButton::write(wxTextFile *outFile) {
	// Divisionals, Generals and Pistons are displayed as piston by default
	// Any other type is displayed as a drawstop by default
	if (
		(m_type.Contains(wxT("Divisional")) && !m_type.IsSameAs(wxT("DivisionalCoupler"))) ||
		(m_type.Contains(wxT("General")) && !m_type.IsSameAs(wxT("GeneralPrev")) && !m_type.IsSameAs(wxT("GeneralNext"))) ||
		m_type.IsSameAs(wxT("ReversiblePiston"))) {
		if (!m_displayAsPiston)
			outFile->AddLine(wxT("DisplayAsPiston=N"));
	} else {
		if (m_displayAsPiston)
			outFile->AddLine(wxT("DisplayAsPiston=Y"));
	}
	if (m_dispLabelColour.getSelectedColorIndex() != 9) {
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
	if (m_dispLabelFont.GetFaceName() != getOwningPanel()->getDisplayMetrics()->m_dispControlLabelFont.GetFaceName())
		outFile->AddLine(wxT("DispLabelFontName=") + m_dispLabelFont.GetFaceName());
	if (m_dispLabelText != wxEmptyString)
		outFile->AddLine(wxT("DispLabelText=") + m_dispLabelText);
	if (!m_dispKeyLabelOnLeft)
		outFile->AddLine(wxT("DispKeyLabelOnLeft=N"));
	if (m_dispImageNum != 1 && m_imageOn == wxEmptyString)
		outFile->AddLine(wxT("DispImageNum=") + wxString::Format(wxT("%i"), m_dispImageNum));
	if (m_dispButtonRow != 1 && (getPosY() == -1) && m_displayAsPiston)
		outFile->AddLine(wxT("DispButtonRow=") + wxString::Format(wxT("%i"), m_dispButtonRow));
	if (m_dispButtonCol != 1 && (getPosX() == -1) && m_displayAsPiston)
		outFile->AddLine(wxT("DispButtonCol=") + wxString::Format(wxT("%i"), m_dispButtonCol));
	if (m_dispDrawstopRow != 1 && (getPosY() == -1) && !m_displayAsPiston)
		outFile->AddLine(wxT("DispDrawstopRow=") + wxString::Format(wxT("%i"), m_dispDrawstopRow));
	if (m_dispDrawstopCol != 1 && (getPosX() == -1) && !m_displayAsPiston)
		outFile->AddLine(wxT("DispDrawstopCol=") + wxString::Format(wxT("%i"), m_dispDrawstopCol));
	if (m_imageOn != wxEmptyString) {
		wxString relativePath = GOODF_functions::removeBaseOdfPath(m_imageOn);
		outFile->AddLine(wxT("ImageOn=") + GOODF_functions::fixSeparator(relativePath));
	}
	if (m_imageOff != wxEmptyString) {
		wxString relativePath = GOODF_functions::removeBaseOdfPath(m_imageOff);
		outFile->AddLine(wxT("ImageOff=") + GOODF_functions::fixSeparator(relativePath));
	}
	if (m_maskOn != wxEmptyString) {
		wxString relativePath = GOODF_functions::removeBaseOdfPath(m_maskOn);
		outFile->AddLine(wxT("MaskOn=") + GOODF_functions::fixSeparator(relativePath));
	}
	if (m_maskOff != wxEmptyString) {
		wxString relativePath = GOODF_functions::removeBaseOdfPath(m_maskOff);
		outFile->AddLine(wxT("MaskOff=") + GOODF_functions::fixSeparator(relativePath));
	}
	if (m_width != m_bitmapWidth)
		outFile->AddLine(wxT("Width=") + wxString::Format(wxT("%i"), m_width));
	if (m_height != m_bitmapHeight)
		outFile->AddLine(wxT("Height=") + wxString::Format(wxT("%i"), m_height));
	if (m_tileOffsetX != 0)
		outFile->AddLine(wxT("TileOffsetX=") + wxString::Format(wxT("%i"), m_tileOffsetX));
	if (m_tileOffsetY != 0)
		outFile->AddLine(wxT("TileOffsetY=") + wxString::Format(wxT("%i"), m_tileOffsetY));
	if (m_mouseRectLeft != 0)
		outFile->AddLine(wxT("MouseRectLeft=") + wxString::Format(wxT("%i"), m_mouseRectLeft));
	if (m_mouseRectTop != 0)
		outFile->AddLine(wxT("MouseRectTop=") + wxString::Format(wxT("%i"), m_mouseRectTop));
	if (m_mouseRectWidth != (m_width - m_mouseRectLeft))
		outFile->AddLine(wxT("MouseRectWidth=") + wxString::Format(wxT("%i"), m_mouseRectWidth));
	if (m_mouseRectHeight != (m_height - m_mouseRectTop))
		outFile->AddLine(wxT("MouseRectHeight=") + wxString::Format(wxT("%i"), m_mouseRectHeight));
	if (m_mouseRadius != (std::min(m_mouseRectWidth, m_mouseRectHeight) / 2) )
		outFile->AddLine(wxT("MouseRadius=") + wxString::Format(wxT("%i"), m_mouseRadius));
	if (m_textRectLeft != 1)
		outFile->AddLine(wxT("TextRectLeft=") + wxString::Format(wxT("%i"), m_textRectLeft));
	if (m_textRectTop != 1)
		outFile->AddLine(wxT("TextRectTop=") + wxString::Format(wxT("%i"), m_textRectTop));
	if (m_textRectWidth != (m_width - m_textRectLeft))
		outFile->AddLine(wxT("TextRectWidth=") + wxString::Format(wxT("%i"), m_textRectWidth));
	if (m_textRectHeight != (m_height - m_textRectTop))
		outFile->AddLine(wxT("TextRectHeight=") + wxString::Format(wxT("%i"), m_textRectHeight));
	if (m_textBreakWidth != (m_textRectWidth - (m_textRectWidth < 50 ? 4 : 14)))
		outFile->AddLine(wxT("TextBreakWidth=") + wxString::Format(wxT("%i"), m_textBreakWidth));
}

void GUIButton::read(wxFileConfig *cfg, bool isPiston, Organ *readOrgan) {
	wxString cfgBoolValue = cfg->Read("DisplayAsPiston", wxEmptyString);
	m_displayAsPiston = GOODF_functions::parseBoolean(cfgBoolValue, isPiston);
	wxString colorStr = cfg->Read("DispLabelColour", wxT("DARK RED"));
	int colorIdx = getDispLabelColour()->getColorNames().Index(colorStr, false);
	if (colorIdx != wxNOT_FOUND) {
		getDispLabelColour()->setSelectedColorIndex(colorIdx);
	} else {
		// is it possible that it's a html format color code instead
		wxColour color;
		color.Set(colorStr);
		if (color.IsOk()) {
			getDispLabelColour()->setColorValue(color);
		}
	}
	wxString sizeStr = cfg->Read("DispLabelFontSize", wxT("7"));
	int sizeIdx = getDispLabelFontSize()->getSizeNames().Index(sizeStr, false);
	if (sizeIdx != wxNOT_FOUND) {
		getDispLabelFontSize()->setSelectedSizeIndex(sizeIdx);
	} else {
		// it's possible that it can be a numerical value instead
		long value;
		if (sizeStr.ToLong(&value)) {
			if (value > 0 && value < 51)
				setDispLabelFontSize(value);
		}
	}
	wxString fontStr = cfg->Read("DispLabelFontName", wxEmptyString);
	wxFont labelFont = wxFont(wxFontInfo(getDispLabelFontSize()->getSizeValue()).FaceName(fontStr));
	if (fontStr != wxEmptyString && labelFont.IsOk() && labelFont.GetFaceName() == fontStr) {
		setDispLabelFont(labelFont);
	} else {
		labelFont = getOwningPanel()->getDisplayMetrics()->m_dispControlLabelFont;
		labelFont.SetPointSize(m_dispLabelFontSize.getSizeValue());
		setDispLabelFont(labelFont);
	}
	setDispLabelText(cfg->Read("DispLabelText", wxEmptyString));
	cfgBoolValue = cfg->Read("DisplayKeyLabelOnLeft", wxEmptyString);
	m_dispKeyLabelOnLeft = GOODF_functions::parseBoolean(cfgBoolValue, true);
	int imageNbr = static_cast<int>(cfg->ReadLong("DispImageNum", 0));
	if (isPiston) {
		if (imageNbr > 0 && imageNbr < 6)
			m_dispImageNum = imageNbr;
	} else {
		if (imageNbr > 0 && imageNbr < 8)
			m_dispImageNum = imageNbr;
	}
	int btnRow = static_cast<int>(cfg->ReadLong("DispButtonRow", 1));
	if (btnRow > -1 && btnRow <= 99 + getOwningPanel()->getDisplayMetrics()->m_dispExtraButtonRows)
		m_dispButtonRow = btnRow;
	int btnCol = static_cast<int>(cfg->ReadLong("DispButtonCol", 1));
	if (btnCol > 0 && btnCol <= getOwningPanel()->getDisplayMetrics()->m_dispButtonCols)
		m_dispButtonCol = btnCol;
	int stopRow = static_cast<int>(cfg->ReadLong("DispDrawstopRow", 1));
	if (stopRow > 0 && stopRow <= 99 + getOwningPanel()->getDisplayMetrics()->m_dispExtraDrawstopRows)
		m_dispDrawstopRow = stopRow;
	int stopCol = static_cast<int>(cfg->ReadLong("DispDrawstopCol", 1));
	if (stopCol > 0 && stopCol <= (stopRow > 99 ? getOwningPanel()->getDisplayMetrics()->m_dispExtraDrawstopCols : getOwningPanel()->getDisplayMetrics()->m_dispDrawstopCols))
		m_dispDrawstopCol = stopCol;
	wxString image_on = cfg->Read("ImageOn", wxEmptyString);
	m_imageOn = GOODF_functions::checkIfFileExist(image_on, readOrgan);
	if (m_imageOn != wxEmptyString) {
		wxImage img = wxImage(m_imageOn);
		if (img.IsOk()) {
			int width = img.GetWidth();
			int height = img.GetHeight();
			setBitmapWidth(width);
			setBitmapHeight(height);
		}
	} else {
		if (m_displayAsPiston) {
			setBitmapWidth(32);
			setBitmapHeight(32);
		} else {
			setBitmapWidth(65);
			setBitmapHeight(65);
		}
	}
	wxString image_off = cfg->Read("ImageOff", wxEmptyString);
	m_imageOff = GOODF_functions::checkIfFileExist(image_off, readOrgan);
	wxString mask_on = cfg->Read("MaskOn", wxEmptyString);
	m_maskOn = GOODF_functions::checkIfFileExist(mask_on, readOrgan);
	wxString mask_off = cfg->Read("MaskOff", wxEmptyString);
	m_maskOff = GOODF_functions::checkIfFileExist(mask_off, readOrgan);
	int thePanelWidth = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
	int thePanelHeight = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();
	int posX = static_cast<int>(cfg->ReadLong("PositionX", -1));
	if (posX > -1 && posX <= thePanelWidth)
		setPosX(posX);
	int posY = static_cast<int>(cfg->ReadLong("PositionY", -1));
	if (posY > -1 && posY <= thePanelHeight)
		setPosY(posY);
	int encWidth = static_cast<int>(cfg->ReadLong("Width", -1));
	if (encWidth > 0 && encWidth <= thePanelWidth) {
		setWidth(encWidth);
	} else {
		setWidth(m_bitmapWidth);
	}
	int encHeight = static_cast<int>(cfg->ReadLong("Height", -1));
	if (encHeight > 0 && encHeight <= thePanelHeight) {
		setHeight(encHeight);
	} else {
		setHeight(m_bitmapHeight);
	}
	int tileX = static_cast<int>(cfg->ReadLong("TileOffsetX", -1));
	if (tileX > -1 && tileX < getBitmapWidth()) {
		setTileOffsetX(tileX);
	}
	int tileY = static_cast<int>(cfg->ReadLong("TileOffsetY", -1));
	if (tileY > -1 && tileY < getBitmapHeight()) {
		setTileOffsetY(tileY);
	}
	int mouseRectLeft = static_cast<int>(cfg->ReadLong("MouseRectLeft", -1));
	if (mouseRectLeft > -1 && mouseRectLeft < getWidth()) {
		setMouseRectLeft(mouseRectLeft);
	}
	int mouseRectTop = static_cast<int>(cfg->ReadLong("MouseRectTop", -1));
	if (mouseRectTop > -1 && mouseRectTop < getHeight()) {
		setMouseRectTop(mouseRectTop);
	}
	int mouseRectWidth = static_cast<int>(cfg->ReadLong("MouseRectWidth", -1));
	if (mouseRectWidth > 0 && mouseRectWidth <= (getWidth() - m_mouseRectLeft)) {
		setMouseRectWidth(mouseRectWidth);
	} else {
		setMouseRectWidth(getWidth() - m_mouseRectLeft);
	}
	int mouseRectHeight = static_cast<int>(cfg->ReadLong("MouseRectHeight", -1));
	if (mouseRectHeight > 0 && mouseRectHeight <= (getHeight() - m_mouseRectTop)) {
		setMouseRectHeight(mouseRectHeight);
	} else {
		setMouseRectHeight(getHeight() - m_mouseRectTop);
	}
	int mouseRadius = static_cast<int>(cfg->ReadLong("MouseRadius", -1));
	if (mouseRadius > -1 && mouseRadius <= std::max(m_mouseRectWidth, m_mouseRectHeight)) {
		setMouseRadius(mouseRadius);
	} else {
		setMouseRadius(std::min(m_mouseRectWidth, m_mouseRectHeight) / 2);
	}
	int textRectLeft = static_cast<int>(cfg->ReadLong("TextRectLeft", 1));
	if (textRectLeft > -1 && textRectLeft < getWidth()) {
		setTextRectLeft(textRectLeft);
	}
	int textRectTop = static_cast<int>(cfg->ReadLong("TextRectTop", 1));
	if (textRectTop > -1 && textRectTop < getHeight()) {
		setTextRectTop(textRectTop);
	}
	int textRectWidth = static_cast<int>(cfg->ReadLong("TextRectWidth", -1));
	if (textRectWidth > 0 && textRectWidth <= (getWidth() - m_textRectLeft)) {
		setTextRectWidth(textRectWidth);
	} else {
		setTextRectWidth(getWidth() - m_textRectLeft);
	}
	int textRectHeight = static_cast<int>(cfg->ReadLong("TextRectHeight", -1));
	if (textRectHeight > 0 && textRectHeight <= (getHeight() - m_textRectTop)) {
		setTextRectHeight(textRectHeight);
	} else {
		setTextRectHeight(getHeight() - m_textRectTop);
	}
	int textBreakWidth = static_cast<int>(cfg->ReadLong("TextBreakWidth", -1));
	if (textBreakWidth > -1 && textBreakWidth <= m_textRectWidth) {
		setTextBreakWidth(textBreakWidth);
	} else {
		setTextBreakWidth(m_textRectWidth - (m_textRectWidth < 50 ? 4 : 14));
	}
	// Fix incorrect usage of DispLabelText set as empty
	// with the intent to "hide" any text written from GO
	if (cfg->HasEntry("DispLabelText") && getDispLabelText() == wxEmptyString)
		setTextBreakWidth(0);
}


GUIButton* GUIButton::clone() {
	return new GUIButton(*this);
}

void GUIButton::updateDisplayName() {

}

wxBitmap GUIButton::getBitmap() {
	if (m_imageOff == wxEmptyString) {
		if (m_displayAsPiston) {
			return ::wxGetApp().m_buttonBitmaps[m_dispImageNum - 1];
		} else {
			return ::wxGetApp().m_drawstopBitmaps[m_dispImageNum - 1];
		}
	} else {
		wxImage img(m_imageOff);
		if (img.IsOk()) {
			wxBitmap bmp(m_imageOff, img.GetType());
			return bmp;
		} else {
			return wxNullBitmap;
		}
	}
}

wxString GUIButton::getElementName() {
	return wxEmptyString;
}

int GUIButton::getDispButtonCol() const {
	return m_dispButtonCol;
}

void GUIButton::setDispButtonCol(int dispButtonCol) {
	m_dispButtonCol = dispButtonCol;
}

int GUIButton::getDispButtonRow() const {
	return m_dispButtonRow;
}

void GUIButton::setDispButtonRow(int dispButtonRow) {
	m_dispButtonRow = dispButtonRow;
}

int GUIButton::getDispDrawstopCol() const {
	return m_dispDrawstopCol;
}

void GUIButton::setDispDrawstopCol(int dispDrawstopCol) {
	m_dispDrawstopCol = dispDrawstopCol;
}

int GUIButton::getDispDrawstopRow() const {
	return m_dispDrawstopRow;
}

void GUIButton::setDispDrawstopRow(int dispDrawstopRow) {
	m_dispDrawstopRow = dispDrawstopRow;
}

int GUIButton::getDispImageNum() const {
	return m_dispImageNum;
}

void GUIButton::setDispImageNum(int dispImageNum) {
	m_dispImageNum = dispImageNum;
}

bool GUIButton::isDispKeyLabelOnLeft() const {
	return m_dispKeyLabelOnLeft;
}

void GUIButton::setDispKeyLabelOnLeft(bool dispKeyLabelOnLeft) {
	m_dispKeyLabelOnLeft = dispKeyLabelOnLeft;
}

GoColor* GUIButton::getDispLabelColour() {
	return &m_dispLabelColour;
}

void GUIButton::setDispLabelColour(const GoColor &dispLabelColour) {
	m_dispLabelColour = dispLabelColour;
}

GoFontSize* GUIButton::getDispLabelFontSize() {
	return &m_dispLabelFontSize;
}

void GUIButton::setDispLabelFontSize(int size) {
	m_dispLabelFontSize.setSizeValue(size);
	m_dispLabelFont.SetPointSize(m_dispLabelFontSize.getSizeValue());
}

void GUIButton::setDispLabelFont(wxFont font) {
	m_dispLabelFont = font;
}

wxFont GUIButton::getDispLabelFont() {
	return m_dispLabelFont;
}

const wxString& GUIButton::getDispLabelText() const {
	return m_dispLabelText;
}

void GUIButton::setDispLabelText(wxString dispLabelText) {
	m_dispLabelText = dispLabelText;
}

bool GUIButton::isDisplayAsPiston() const {
	return m_displayAsPiston;
}

void GUIButton::setDisplayAsPiston(bool displayAsPiston) {
	m_displayAsPiston = displayAsPiston;
	if (m_imageOn == wxEmptyString)
		updateBuiltinBitmapValues();
}

int GUIButton::getHeight() const {
	return m_height;
}

void GUIButton::setHeight(int height) {
	m_height = height;
}

const wxString& GUIButton::getImageOff() const {
	return m_imageOff;
}

void GUIButton::setImageOff(const wxString &imageOff) {
	m_imageOff = imageOff;
}

const wxString& GUIButton::getImageOn() const {
	return m_imageOn;
}

void GUIButton::setImageOn(const wxString &imageOn) {
	m_imageOn = imageOn;
}

const wxString& GUIButton::getMaskOff() const {
	return m_maskOff;
}

void GUIButton::setMaskOff(const wxString &maskOff) {
	m_maskOff = maskOff;
}

const wxString& GUIButton::getMaskOn() const {
	return m_maskOn;
}

void GUIButton::setMaskOn(const wxString &maskOn) {
	m_maskOn = maskOn;
}

int GUIButton::getMouseRadius() const {
	return m_mouseRadius;
}

void GUIButton::setMouseRadius(int mouseRadius) {
	m_mouseRadius = mouseRadius;
}

int GUIButton::getMouseRectHeight() const {
	return m_mouseRectHeight;
}

void GUIButton::setMouseRectHeight(int mouseRectHeight) {
	m_mouseRectHeight = mouseRectHeight;
}

int GUIButton::getMouseRectLeft() const {
	return m_mouseRectLeft;
}

void GUIButton::setMouseRectLeft(int mouseRectLeft) {
	m_mouseRectLeft = mouseRectLeft;
}

int GUIButton::getMouseRectTop() const {
	return m_mouseRectTop;
}

void GUIButton::setMouseRectTop(int mouseRectTop) {
	m_mouseRectTop = mouseRectTop;
}

int GUIButton::getMouseRectWidth() const {
	return m_mouseRectWidth;
}

void GUIButton::setMouseRectWidth(int mouseRectWidth) {
	m_mouseRectWidth = mouseRectWidth;
}

int GUIButton::getTextBreakWidth() const {
	return m_textBreakWidth;
}

void GUIButton::setTextBreakWidth(int textBreakWidth) {
	m_textBreakWidth = textBreakWidth;
}

int GUIButton::getTextRectHeight() const {
	return m_textRectHeight;
}

void GUIButton::setTextRectHeight(int textRectHeight) {
	m_textRectHeight = textRectHeight;
}

int GUIButton::getTextRectLeft() const {
	return m_textRectLeft;
}

void GUIButton::setTextRectLeft(int textRectLeft) {
	m_textRectLeft = textRectLeft;
}

int GUIButton::getTextRectTop() const {
	return m_textRectTop;
}

void GUIButton::setTextRectTop(int textRectTop) {
	m_textRectTop = textRectTop;
}

int GUIButton::getTextRectWidth() const {
	return m_textRectWidth;
}

void GUIButton::setTextRectWidth(int textRectWidth) {
	m_textRectWidth = textRectWidth;
}

int GUIButton::getTileOffsetX() const {
	return m_tileOffsetX;
}

void GUIButton::setTileOffsetX(int tileOffsetX) {
	m_tileOffsetX = tileOffsetX;
}

int GUIButton::getTileOffsetY() const {
	return m_tileOffsetY;
}

void GUIButton::setTileOffsetY(int tileOffsetY) {
	m_tileOffsetY = tileOffsetY;
}

int GUIButton::getWidth() const {
	return m_width;
}

void GUIButton::setWidth(int width) {
	m_width = width;
}

void GUIButton::setBitmapWidth(int width) {
	m_bitmapWidth = width;
}

int GUIButton::getBitmapWidth() {
	return m_bitmapWidth;
}

void GUIButton::setBitmapHeight(int height) {
	m_bitmapHeight = height;
}

int GUIButton::getBitmapHeight() {
	return m_bitmapHeight;
}

void GUIButton::setDefaultFont(wxFont &theFont) {
	m_dispLabelFont = theFont;
	m_dispLabelFontSize.setSizeValue(theFont.GetPointSize());
}

void GUIButton::updateBuiltinBitmapValues() {
	int width, height;
	if (isDisplayAsPiston()) {
		width = 32;
		height = 32;
	} else {
		width = 65;
		height = 65;
	}
	m_bitmapWidth = width;
	m_bitmapHeight = height;
	m_width = width;
	m_height = height;
	m_mouseRectWidth = width - m_mouseRectLeft;
	m_mouseRectHeight = height - m_mouseRectTop;
	m_mouseRadius = width / 2;
	m_textRectWidth = width - m_textRectLeft;
	m_textRectHeight = height - m_textRectTop;
	m_textBreakWidth = m_textRectWidth - (m_textRectWidth < 50 ? 4 : 14);
}
