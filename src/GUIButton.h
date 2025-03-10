/*
 * GUIButton.h is part of GoOdf.
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

#ifndef GUIBUTTON_H
#define GUIBUTTON_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include "GUIElements.h"
#include "GoColor.h"
#include "GoFontSize.h"

class Organ;

class GUIButton : public GUIElement {
public:
	GUIButton();
	virtual ~GUIButton();

	virtual void write(wxTextFile *outFile);
	virtual void read(wxFileConfig *cfg, bool isPiston, Organ *readOrgan);

	virtual GUIButton* clone();
	virtual void updateDisplayName();
	virtual wxBitmap getBitmap();
	virtual wxString getElementName();
	int getDispButtonCol() const;
	void setDispButtonCol(int dispButtonCol);
	int getDispButtonRow() const;
	void setDispButtonRow(int dispButtonRow);
	int getDispDrawstopCol() const;
	void setDispDrawstopCol(int dispDrawstopCol);
	int getDispDrawstopRow() const;
	void setDispDrawstopRow(int dispDrawstopRow);
	int getDispImageNum() const;
	void setDispImageNum(int dispImageNum);
	bool isDispKeyLabelOnLeft() const;
	void setDispKeyLabelOnLeft(bool dispKeyLabelOnLeft);
	GoColor* getDispLabelColour();
	void setDispLabelColour(const GoColor &dispLabelColour);
	GoFontSize* getDispLabelFontSize();
	void setDispLabelFontSize(int size);
	void setDispLabelFont(wxFont font);
	wxFont getDispLabelFont();
	const wxString& getDispLabelText() const;
	void setDispLabelText(wxString dispLabelText);
	bool isDisplayAsPiston() const;
	void setDisplayAsPiston(bool displayAsPiston);

	int getHeight() const;
	void setHeight(int height);
	const wxString& getImageOff() const;
	void setImageOff(const wxString &imageOff);
	const wxString& getImageOn() const;
	void setImageOn(const wxString &imageOn);
	const wxString& getMaskOff() const;
	void setMaskOff(const wxString &maskOff);
	const wxString& getMaskOn() const;
	void setMaskOn(const wxString &maskOn);
	int getMouseRadius() const;
	void setMouseRadius(int mouseRadius);
	int getMouseRectHeight() const;
	void setMouseRectHeight(int mouseRectHeight);
	int getMouseRectLeft() const;
	void setMouseRectLeft(int mouseRectLeft);
	int getMouseRectTop() const;
	void setMouseRectTop(int mouseRectTop);
	int getMouseRectWidth() const;
	void setMouseRectWidth(int mouseRectWidth);
	int getTextBreakWidth() const;
	void setTextBreakWidth(int textBreakWidth);
	int getTextRectHeight() const;
	void setTextRectHeight(int textRectHeight);
	int getTextRectLeft() const;
	void setTextRectLeft(int textRectLeft);
	int getTextRectTop() const;
	void setTextRectTop(int textRectTop);
	int getTextRectWidth() const;
	void setTextRectWidth(int textRectWidth);
	int getTileOffsetX() const;
	void setTileOffsetX(int tileOffsetX);
	int getTileOffsetY() const;
	void setTileOffsetY(int tileOffsetY);
	int getWidth() const;
	void setWidth(int width);
	void setBitmapWidth(int width);
	int getBitmapWidth();
	void setBitmapHeight(int height);
	int getBitmapHeight();
	void setDefaultFont(wxFont &theFont);
	void updateBuiltinBitmapValues();

protected:
	bool m_displayAsPiston;
	GoColor m_dispLabelColour; // (GoColor, default: Dark Red)
	GoFontSize m_dispLabelFontSize; // (font size, default: normal)
	// wxString m_dispLabelFontName; // (string, default: empty)
	wxFont m_dispLabelFont; // this takes care of m_dispLabelFontName and point size input
	wxString m_dispLabelText; // (string, default: Name of the button)
	bool m_dispKeyLabelOnLeft; // (boolean, default: true)
	int m_dispImageNum; // (integer 1- type dependent, default: see below) Builtin bitmap set to use. GrandOrgue has 7 for drawstops and 5 for pistons. The default is 3 (piston) or 4 (drawstops) for read-only buttons, otherwise the default is 1.
	int m_dispButtonRow; // (button row, default: 1)
	int m_dispButtonCol; // (button column, default: 1)
	int m_dispDrawstopRow; // (drawstop row, default: 1)
	int m_dispDrawstopCol; // (drawstop column, default: 1)
	wxString m_imageOn; // (string, default: use internal bitmap according to DispImageNum)
	wxString m_imageOff; // (string, default: use internal bitmap according to DispImageNum)
	wxString m_maskOn; // (string, default: empty)
	wxString m_maskOff; // (string, default: value of MaskOn)
	int m_width; // (integer 0 - panel width, default: bitmap width)
	int m_height; // (integer 0 - panel height, default: bitmap height)
	int m_tileOffsetX; // (integer 0 - bitmap width, default: 0)
	int m_tileOffsetY; // (integer 0 - bitmap width, default: 0)
	int m_mouseRectLeft; // (integer 0 - Width, default: 0)
	int m_mouseRectTop; // (integer 0 - Height, default: 0)
	int m_mouseRectWidth; // (integer 0 - Width, default: Width)
	int m_mouseRectHeight; // (integer 0 - Height, default: Height)
	int m_mouseRadius; // (integer 0 - max(MouseRectHeight, MouseRectWidth) / 2, default: min(MouseRectHeight, MouseRectWidth) / 2)
	int m_textRectLeft; // (integer 0 - Height, default: 0)
	int m_textRectTop; // (integer 0 - Height, default: 0)
	int m_textRectWidth; // (integer 0 - Width, default: Width)
	int m_textRectHeight; // (integer 0 - Height, default: Height)
	int m_textBreakWidth; // (integer 0 - text rectangle width, default: slightly smaller than Width)

	int m_bitmapWidth;
	int m_bitmapHeight;
};

#endif
