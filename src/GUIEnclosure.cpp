/*
 * GUIEnclosure.cpp is part of GoOdf.
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

#include "GUIEnclosure.h"
#include "GOODFFunctions.h"
#include "GOODF.h"

GUIEnclosure::GUIEnclosure(Enclosure *enclosure) : GUIElement(), m_enclosure(enclosure) {
	if (m_enclosure != NULL)
		m_type = wxT("Enclosure");
	else
		m_type = wxT("Swell");

	m_dispLabelColour.setSelectedColorIndex(16);
	m_dispLabelFontSize.setSizeValue(7);
	m_dispLabelFont = wxFont(wxFontInfo(7).FaceName(wxT("Arial")));
	m_dispLabelText = wxEmptyString;
	m_enclosureStyle = 1;
	m_width = 46;
	m_height = 61;
	m_tileOffsetX = 0;
	m_tileOffsetY = 0;
	m_mouseRectLeft = 0;
	m_mouseRectTop = 13;
	m_mouseRectWidth = 46;
	m_mouseRectHeight = 45;
	m_mouseAxisStart = 15;
	m_mouseAxisEnd = 30;
	m_textRectLeft = 0;
	m_textRectTop = 0;
	m_textRectWidth = 46;
	m_textRectHeight = 61;
	m_textBreakWidth = 46;
	m_bitmapWidth = 46;
	m_bitmapHeight = 61;
}

GUIEnclosure::~GUIEnclosure() {

}

void GUIEnclosure::write(wxTextFile *outFile) {
	GUIElement::write(outFile);
	if (m_enclosure != NULL) {
		wxString encId = wxT("Enclosure=") + GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganEnclosure(m_enclosure));
		outFile->AddLine(encId);
	}
	if (m_positionX != -1)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != -1)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	if (m_dispLabelColour.getSelectedColorIndex() != 16) {
		if (m_dispLabelColour.getSelectedColorIndex() > -1) {
			outFile->AddLine(wxT("DispLabelColour=") + m_dispLabelColour.getColorName());
		} else {
			outFile->AddLine(wxT("DispLabelColour=") + m_dispLabelColour.getHtmlValue());
		}
	}
	if (!m_dispLabelFontSize.getSizeName().IsSameAs(wxT("NORMAL"))) {
		if (m_dispLabelFontSize.getSelectedSizeIndex() < 3 ) {
			outFile->AddLine(wxT("DispLabelFontSize=") + m_dispLabelFontSize.getSizeName());
		} else {
			outFile->AddLine(wxT("DispLabelFontSize=") + wxString::Format(wxT("%i"), m_dispLabelFontSize.getSizeValue()));
		}
	}
	if (m_dispLabelFont != wxFont(wxFontInfo(7).FaceName(wxT("Arial"))))
		outFile->AddLine(wxT("DispLabelFontName=") + m_dispLabelFont.GetFaceName());
	if (m_dispLabelText != wxEmptyString)
		outFile->AddLine(wxT("DispLabelText=") + m_dispLabelText);
	if (m_enclosureStyle != 1 && m_bitmaps.empty())
		outFile->AddLine(wxT("EnclosureStyle=") + wxString::Format(wxT("%i"), m_enclosureStyle));
	if (!m_bitmaps.empty()) {
		unsigned nbBitmaps = m_bitmaps.size();
		outFile->AddLine(wxT("BitmapCount=") + wxString::Format(wxT("%u"), nbBitmaps));
		unsigned counter = 1;
		for (GoImage& bitmap : m_bitmaps) {
			wxString bitmapId = wxT("Bitmap") + GOODF_functions::number_format(counter) + wxT("=");
			outFile->AddLine(bitmapId + GOODF_functions::fixSeparator(bitmap.getRelativeImagePath()));
			if (bitmap.getMask() != wxEmptyString) {
				wxString maskId = wxT("Mask") + GOODF_functions::number_format(counter) + wxT("=");
				outFile->AddLine(maskId + GOODF_functions::fixSeparator(bitmap.getRelativeMaskPath()));
			}
			counter++;
		}
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
	if (m_mouseRectTop != 13)
		outFile->AddLine(wxT("MouseRectTop=") + wxString::Format(wxT("%i"), m_mouseRectTop));
	if (m_mouseRectWidth != (m_width - m_mouseRectLeft))
		outFile->AddLine(wxT("MouseRectWidth=") + wxString::Format(wxT("%i"), m_mouseRectWidth));
	if (m_mouseRectHeight != (m_height - m_mouseRectTop - 3))
		outFile->AddLine(wxT("MouseRectHeight=") + wxString::Format(wxT("%i"), m_mouseRectHeight));
	if (m_mouseAxisStart != (m_mouseRectHeight / 3))
		outFile->AddLine(wxT("MouseAxisStart=") + wxString::Format(wxT("%i"), m_mouseAxisStart));
	if (m_mouseAxisEnd != (m_mouseRectHeight / 3 * 2))
		outFile->AddLine(wxT("MouseAxisEnd=") + wxString::Format(wxT("%i"), m_mouseAxisEnd));
	if (m_textRectLeft != 0)
		outFile->AddLine(wxT("TextRectLeft=") + wxString::Format(wxT("%i"), m_textRectLeft));
	if (m_textRectTop != 0)
		outFile->AddLine(wxT("TextRectTop=") + wxString::Format(wxT("%i"), m_textRectTop));
	if (m_textRectWidth != (m_width - m_textRectLeft))
		outFile->AddLine(wxT("TextRectWidth=") + wxString::Format(wxT("%i"), m_textRectWidth));
	if (m_textRectHeight != (m_height - m_textRectTop))
		outFile->AddLine(wxT("TextRectHeight=") + wxString::Format(wxT("%i"), m_textRectHeight));
	if (m_textBreakWidth != m_textRectWidth)
		outFile->AddLine(wxT("TextBreakWidth=") + wxString::Format(wxT("%i"), m_textBreakWidth));
}

void GUIEnclosure::read(wxFileConfig *cfg, Organ *readOrgan) {
	wxString colorStr = cfg->Read("DispLabelColour", wxT("WHITE"));
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
	wxFont labelFont = wxFont(fontStr);
	if (fontStr != wxEmptyString && labelFont.IsOk()) {
		labelFont.SetPointSize(m_dispLabelFontSize.getSizeValue());
		setDispLabelFont(labelFont);
	}
	setDispLabelText(cfg->Read("DispLabelText", wxEmptyString));
	int encStyle = static_cast<int>(cfg->ReadLong("EnclosureStyle", 1));
	if (encStyle > 0 && encStyle < 5) {
		setEnclosureStyle(encStyle);
	}
	int bitmapCount = static_cast<int>(cfg->ReadLong("BitmapCount", 0));
	if (bitmapCount > 0 && bitmapCount < 129) {
		for (int i = 0; i < bitmapCount; i++) {
			GoImage tmpBmp;
			wxString bmpStr = wxT("Bitmap") + GOODF_functions::number_format(i + 1);
			wxString maskStr = wxT("Mask") + GOODF_functions::number_format(i + 1);
			wxString relBmpPath = cfg->Read(bmpStr, wxEmptyString);
			wxString fullBmpPath = GOODF_functions::checkIfFileExist(relBmpPath, readOrgan);
			wxString relMaskPath = cfg->Read(maskStr, wxEmptyString);
			wxString fullMaskPath = GOODF_functions::checkIfFileExist(relMaskPath, readOrgan);
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
					if (getNumberOfBitmaps() == 0) {
						// from the first bitmap we can store the original size values
						int width = img.GetWidth();
						int height = img.GetHeight();
						tmpBmp.setOriginalWidth(width);
						tmpBmp.setOriginalHeight(height);
						setBitmapWidth(width);
						setBitmapHeight(height);
					}
					addBitmap(tmpBmp);
				}
			}
		}
	}
	int thePanelWidth = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
	int thePanelHeight = getOwningPanel()->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();
	int posX = static_cast<int>(cfg->ReadLong("PositionX", -1));
	if (posX > -1 && posX <= thePanelWidth)
		setPosX(posX);
	int posY = static_cast<int>(cfg->ReadLong("PositionY", -1));
	if (posY > -1 && posY <= thePanelHeight)
		setPosY(posY);
	int encWidth = static_cast<int>(cfg->ReadLong("Width", -1));
	if (encWidth > -1 && encWidth <= thePanelWidth) {
		setWidth(encWidth);
	} else {
		setWidth(m_bitmapWidth);
	}
	int encHeight = static_cast<int>(cfg->ReadLong("Height", -1));
	if (encHeight > -1 && encHeight <= thePanelHeight) {
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
	} else {
		setMouseRectTop(13);
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
		setMouseRectHeight(getHeight() - m_mouseRectTop - 3);
	}
	int mouseAxisStart = static_cast<int>(cfg->ReadLong("MouseAxisStart", -1));
	if (mouseAxisStart > -1 && mouseAxisStart <= m_mouseRectHeight) {
		setMouseAxisStart(mouseAxisStart);
	} else {
		setMouseAxisStart(m_mouseRectHeight / 3);
	}
	int mouseAxisEnd = static_cast<int>(cfg->ReadLong("MouseAxisEnd", -1));
	if (mouseAxisEnd >= m_mouseAxisStart && mouseAxisEnd <= m_mouseRectHeight) {
		setMouseAxisEnd(mouseAxisEnd);
	} else {
		setMouseAxisEnd(m_mouseRectHeight / 3 * 2);
	}
	int textRectLeft = static_cast<int>(cfg->ReadLong("TextRectLeft", -1));
	if (textRectLeft > -1 && textRectLeft < getWidth()) {
		setTextRectLeft(textRectLeft);
	}
	int textRectTop = static_cast<int>(cfg->ReadLong("TextRectTop", -1));
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
		setTextBreakWidth(m_textRectWidth);
	}
	// Fix incorrect usage of DispLabelText set as empty
	// with the intent to "hide" any text written from GO
	if (cfg->HasEntry("DispLabelText") && getDispLabelText() == wxEmptyString)
		setTextBreakWidth(0);
}

GUIEnclosure* GUIEnclosure::clone() {
	return new GUIEnclosure(*this);
}

bool GUIEnclosure::isReferencing(Enclosure *enclosure) {
	return m_enclosure == enclosure ? true : false;
}

void GUIEnclosure::updateDisplayName() {
	if (m_enclosure)
		setDisplayName(m_enclosure->getName() + wxT(" (Enclosure)"));
}

wxBitmap GUIEnclosure::getBitmap() {
	if (m_bitmaps.empty()) {
		return ::wxGetApp().m_enclosureStyleBitmaps[m_enclosureStyle - 1];
	} else {
		return m_bitmaps.front().getBitmap();
	}
}

wxString GUIEnclosure::getElementName() {
	if (m_enclosure)
		return m_enclosure->getName();
	else
		return getDisplayName();
}

GoColor* GUIEnclosure::getDispLabelColour() {
	return &m_dispLabelColour;
}

void GUIEnclosure::setDispLabelColour(const GoColor &dispLabelColour) {
	m_dispLabelColour = dispLabelColour;
}

GoFontSize* GUIEnclosure::getDispLabelFontSize() {
	return &m_dispLabelFontSize;
}

void GUIEnclosure::setDispLabelFontSize(int fontSize) {
	m_dispLabelFontSize.setSizeValue(fontSize);
	m_dispLabelFont.SetPointSize(m_dispLabelFontSize.getSizeValue());
}

const wxString& GUIEnclosure::getDispLabelText() const {
	return m_dispLabelText;
}

void GUIEnclosure::setDispLabelText(wxString dispLabelText) {
	m_dispLabelText = dispLabelText;
}

void GUIEnclosure::setDispLabelFont(wxFont font) {
	m_dispLabelFont = font;
}

wxFont GUIEnclosure::getDispLabelFont() {
	return m_dispLabelFont;
}

int GUIEnclosure::getEnclosureStyle() const {
	return m_enclosureStyle;
}

void GUIEnclosure::setEnclosureStyle(int enclosureStyle) {
	m_enclosureStyle = enclosureStyle;
}

int GUIEnclosure::getHeight() const {
	return m_height;
}

void GUIEnclosure::setHeight(int height) {
	m_height = height;
}

int GUIEnclosure::getMouseAxisEnd() const {
	return m_mouseAxisEnd;
}

void GUIEnclosure::setMouseAxisEnd(int mouseAxisEnd) {
	m_mouseAxisEnd = mouseAxisEnd;
}

int GUIEnclosure::getMouseAxisStart() const {
	return m_mouseAxisStart;
}

void GUIEnclosure::setMouseAxisStart(int mouseAxisStart) {
	m_mouseAxisStart = mouseAxisStart;
}

int GUIEnclosure::getMouseRectHeight() const {
	return m_mouseRectHeight;
}

void GUIEnclosure::setMouseRectHeight(int mouseRectHeight) {
	m_mouseRectHeight = mouseRectHeight;
}

int GUIEnclosure::getMouseRectLeft() const {
	return m_mouseRectLeft;
}

void GUIEnclosure::setMouseRectLeft(int mouseRectLeft) {
	m_mouseRectLeft = mouseRectLeft;
}

int GUIEnclosure::getMouseRectTop() const {
	return m_mouseRectTop;
}

void GUIEnclosure::setMouseRectTop(int mouseRectTop) {
	m_mouseRectTop = mouseRectTop;
}

int GUIEnclosure::getMouseRectWidth() const {
	return m_mouseRectWidth;
}

void GUIEnclosure::setMouseRectWidth(int mouseRectWidth) {
	m_mouseRectWidth = mouseRectWidth;
}

int GUIEnclosure::getTextBreakWidth() const {
	return m_textBreakWidth;
}

void GUIEnclosure::setTextBreakWidth(int textBreakWidth) {
	m_textBreakWidth = textBreakWidth;
}

int GUIEnclosure::getTextRectHeight() const {
	return m_textRectHeight;
}

void GUIEnclosure::setTextRectHeight(int textRectHeight) {
	m_textRectHeight = textRectHeight;
}

int GUIEnclosure::getTextRectLeft() const {
	return m_textRectLeft;
}

void GUIEnclosure::setTextRectLeft(int textRectLeft) {
	m_textRectLeft = textRectLeft;
}

int GUIEnclosure::getTextRectTop() const {
	return m_textRectTop;
}

void GUIEnclosure::setTextRectTop(int textRectTop) {
	m_textRectTop = textRectTop;
}

int GUIEnclosure::getTextRectWidth() const {
	return m_textRectWidth;
}

void GUIEnclosure::setTextRectWidth(int textRectWidth) {
	m_textRectWidth = textRectWidth;
}

int GUIEnclosure::getTileOffsetX() const {
	return m_tileOffsetX;
}

void GUIEnclosure::setTileOffsetX(int tileOffsetX) {
	m_tileOffsetX = tileOffsetX;
}

int GUIEnclosure::getTileOffsetY() const {
	return m_tileOffsetY;
}

void GUIEnclosure::setTileOffsetY(int tileOffsetY) {
	m_tileOffsetY = tileOffsetY;
}

int GUIEnclosure::getWidth() const {
	return m_width;
}

void GUIEnclosure::setWidth(int width) {
	m_width = width;
}

void GUIEnclosure::addBitmap(GoImage bitmap) {
	if (m_bitmaps.size() < 128)
		m_bitmaps.push_back(bitmap);
}

unsigned GUIEnclosure::getNumberOfBitmaps() {
	return m_bitmaps.size();
}

GoImage* GUIEnclosure::getBitmapAtIndex(unsigned index) {
	auto iterator = std::next(m_bitmaps.begin(), index);
	return &(*iterator);
}

void GUIEnclosure::removeBitmapAt(unsigned index) {
	std::list<GoImage>::iterator it = m_bitmaps.begin();
	std::advance(it, index);
	m_bitmaps.erase(it);
}

void GUIEnclosure::setBitmapWidth(int width) {
	m_bitmapWidth = width;
}

int GUIEnclosure::getBitmapWidth() {
	return m_bitmapWidth;
}

void GUIEnclosure::setBitmapHeight(int height) {
	m_bitmapHeight = height;
}

int GUIEnclosure::getBitmapHeight() {
	return m_bitmapHeight;
}
