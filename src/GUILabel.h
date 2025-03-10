/*
 * GUILabel.h is part of GoOdf.
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

#ifndef GUILABEL_H
#define GUILABEL_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include "GUIElements.h"
#include "GoColor.h"
#include "GoFontSize.h"
#include "GoImage.h"
#include <wx/fileconf.h>

class Organ;

class GUILabel : public GUIElement {
public:
	GUILabel();
	~GUILabel();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, Organ *readOrgan);
	virtual GUILabel* clone();

	void updateDisplayName();

	bool isDispAtTopOfDrawstopCol() const;
	void setDispAtTopOfDrawstopCol(bool dispAtTopOfDrawstopCol);
	int getDispDrawstopCol() const;
	void setDispDrawstopCol(int dispDrawstopCol);
	int getDispImageNum() const;
	void setDispImageNum(int dispImageNum);
	GoColor* getDispLabelColour();
	void setDispLabelColour(const GoColor &dispLabelColour);
	GoFontSize* getDispLabelFontSize();
	void setDispLabelFontSize(int size);
	wxFont getDispLabelFont();
	void setDispLabelFont(wxFont font);
	bool isDispSpanDrawstopColToRight() const;
	void setDispSpanDrawstopColToRight(bool dispSpanDrawstopColToRight);
	int getDispXpos() const;
	void setDispXpos(int dispXpos);
	int getDispYpos() const;
	void setDispYpos(int dispYpos);
	bool isFreeXPlacement() const;
	void setFreeXPlacement(bool freeXPlacement);
	bool isFreeYPlacement() const;
	void setFreeYPlacement(bool freeYPlacement);
	int getHeight() const;
	void setHeight(int height);
	GoImage* getImage();
	const wxString& getName() const;
	void setName(const wxString &name);
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
	int getBitmapWidth();
	void setBitmapWidth(int width);
	int getBitmapHeight();
	void setBitmapHeight(int height);
	void setDefaultFont(wxFont &theFont);
	wxBitmap getBitmap();

protected:
	bool m_freeXPlacement; // (boolean, default: true)
	bool m_freeYPlacement; // (boolean, default: true)
	int m_dispXpos; // (integer 0-panel width, default: 0)
	int m_dispYpos; // (integer 0-panel height, default: 0)
	bool m_dispAtTopOfDrawstopCol; // (boolean, required if FreeYPlacement is false)
	int m_dispDrawstopCol; // (integer 1- number of drawstop columns, required if FreeXPlacement is false)
	bool m_dispSpanDrawstopColToRight; // (boolean, required if FreeXPlacement is false)
	GoColor m_dispLabelColour; // (GoColor, default: Black)
	GoFontSize m_dispLabelFontSize; // (font size, default: normal)
	wxFont m_dispLabelFont; // (string, default: empty) - replace with wxFont
	wxString m_name; // (string)
	int m_dispImageNum; // (integer 0 - 15 default 1, 0 is a transparent background
	GoImage m_image; // (default: use internal bitmap according to DispImageNum) - GoImage takes care of both image and mask
	int m_width; // (integer 0 - panel width, default: bitmap width)
	int m_height; // (integer 0 - panel height, default: bitmap height)
	int m_tileOffsetX; // (integer 0 - bitmap width, default: 0)
	int m_tileOffsetY; // (integer 0 - bitmap width, default: 0)
	int m_textRectLeft; // (integer 0 - Height, default: 0)
	int m_textRectTop; // (integer 0 - Height, default: 0)
	int m_textRectWidth; // (integer 0 - Width, default: Width)
	int m_textRectHeight; // (integer 0 - Height, default: Height)
	int m_textBreakWidth; // (integer 0 - text rectangle width, default: slightly smaller than Width)

	int m_bitmapWidth; // used for storing original image size
	int m_bitmapHeight;
};

#endif
