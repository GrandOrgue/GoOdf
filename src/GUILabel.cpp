/*
 * GUILabel.cpp is part of GOODF.
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

#include "GUILabel.h"

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
	m_name = wxT("New Label");
	m_dispImageNum = 1;
	m_image = GoImage();
	m_width = 80;
	m_height = 25;
	m_tileOffsetX = 0;
	m_tileOffsetY = 0;
	m_textRectLeft = 0;
	m_textRectTop = 0;
	m_textRectWidth = 80;
	m_textRectHeight = 25;
	m_textBreakWidth = 80;
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
		if (m_dispLabelFontSize.getSelectedSizeIndex() > -1) {
			outFile->AddLine(wxT("DispLabelFontSize=") + m_dispLabelFontSize.getSizeName());
		} else {
			outFile->AddLine(wxT("DispLabelFontSize=") + m_dispLabelFontSize.getSizeValue());
		}
	}
	if (m_dispLabelFont != wxFont(wxFontInfo(7).FaceName(wxT("Arial"))))
		outFile->AddLine(wxT("DispLabelFontName=") + m_dispLabelFont.GetFaceName());
	outFile->AddLine(wxT("Name=") + m_name);
	if (m_dispImageNum != 1 && m_image.getImage() == wxEmptyString)
		outFile->AddLine(wxT("DispImageNum=") + wxString::Format(wxT("%i"), m_dispImageNum));
	if (m_image.getImage() != wxEmptyString)
		outFile->AddLine(wxT("Image=") + m_image.getRelativeImagePath());
	if (m_image.getMask() != wxEmptyString && m_image.getImage() != wxEmptyString)
		outFile->AddLine(wxT("Mask=") + m_image.getRelativeMaskPath());
	if (m_width != m_bitmapWidth)
		outFile->AddLine(wxT("Width=") + wxString::Format(wxT("%i"), m_width));
	if (m_height != m_bitmapHeight)
		outFile->AddLine(wxT("Height=") + wxString::Format(wxT("%i"), m_height));
	if (m_tileOffsetX != 0)
		outFile->AddLine(wxT("TileOffsetX=") + wxString::Format(wxT("%i"), m_tileOffsetX));
	if (m_tileOffsetY != 0)
		outFile->AddLine(wxT("TileOffsetY=") + wxString::Format(wxT("%i"), m_tileOffsetY));
	if (m_textRectLeft != 0)
		outFile->AddLine(wxT("TextRectLeft=") + wxString::Format(wxT("%i"), m_textRectLeft));
	if (m_textRectTop != 0)
		outFile->AddLine(wxT("TextRectTop=") + wxString::Format(wxT("%i"), m_textRectTop));
	if (m_textRectWidth != m_width)
		outFile->AddLine(wxT("TextRectWidth=") + wxString::Format(wxT("%i"), m_textRectWidth));
	if (m_textRectHeight != m_height)
		outFile->AddLine(wxT("TextRectHeight=") + wxString::Format(wxT("%i"), m_textRectHeight));
	if (m_textBreakWidth != m_width)
		outFile->AddLine(wxT("TextBreakWidth=") + wxString::Format(wxT("%i"), m_textBreakWidth));
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

const GoFontSize& GUILabel::getDispLabelFontSize() const {
	return m_dispLabelFontSize;
}

void GUILabel::setDispLabelFontSize(int size) {
	m_dispLabelFontSize.setSizeValue(size);
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
