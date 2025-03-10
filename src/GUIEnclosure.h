/*
 * GUIEnclosure.h is part of GoOdf.
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

#ifndef GUIENCLOSURE_H
#define GUIENCLOSURE_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include "GUIElements.h"
#include "GoColor.h"
#include "GoFontSize.h"
#include "GoImage.h"
#include <list>
#include "Enclosure.h"

class Organ;

class GUIEnclosure : public GUIElement {
public:
	GUIEnclosure(Enclosure *enclosure);
	~GUIEnclosure();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg, Organ *readOrgan);

	virtual GUIEnclosure* clone();
	bool isReferencing(Enclosure *enclosure);
	void updateDisplayName();
	virtual wxBitmap getBitmap();
	virtual wxString getElementName();

	GoColor* getDispLabelColour();
	void setDispLabelColour(const GoColor &dispLabelColour);
	GoFontSize* getDispLabelFontSize();
	void setDispLabelFontSize(int fontSize);
	const wxString& getDispLabelText() const;
	void setDispLabelText(wxString dispLabelText);
	void setDispLabelFont(wxFont font);
	wxFont getDispLabelFont();
	int getEnclosureStyle() const;
	void setEnclosureStyle(int enclosureStyle);
	int getHeight() const;
	void setHeight(int height);
	int getMouseAxisEnd() const;
	void setMouseAxisEnd(int mouseAxisEnd);
	int getMouseAxisStart() const;
	void setMouseAxisStart(int mouseAxisStart);
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
	void addBitmap(GoImage bitmap);
	unsigned getNumberOfBitmaps();
	GoImage* getBitmapAtIndex(unsigned index);
	void removeBitmapAt(unsigned index);
	void setBitmapWidth(int width);
	int getBitmapWidth();
	void setBitmapHeight(int height);
	int getBitmapHeight();

protected:
	Enclosure *m_enclosure;
	GoColor m_dispLabelColour; // (color, default: White)
	GoFontSize m_dispLabelFontSize; // (font size, default: 7)
	wxFont m_dispLabelFont; // (string, default: empty)
	wxString m_dispLabelText; // (string, default: Name of the button)
	int m_enclosureStyle; // (integer 1 - 4, default: implementation dependent)
	std::list<GoImage> m_bitmaps;
	int m_width; // (integer 0 - panel width, default: bitmap width)
	int m_height; // (integer 0 - panel height, default: bitmap height)
	int m_tileOffsetX; // (integer 0 - bitmap width, default: 0)
	int m_tileOffsetY; // (integer 0 - bitmap width, default: 0)
	int m_mouseRectLeft; // (integer 0 - Width, default: 0)
	int m_mouseRectTop; // (integer 0 - Height, default: implementation dependent)
	int m_mouseRectWidth; // (integer 0 - Width, default: Width)
	int m_mouseRectHeight; // (integer 0 - Height, default: implementation dependent)
	int m_mouseAxisStart; // (integer 0 - MouseRectHeight, default: implementation dependent)
	int m_mouseAxisEnd; // (integer MouseAxisStart - MouseRectHeight, default: implementation dependent)
	int m_textRectLeft; // (integer 0 - Height, default: 0)
	int m_textRectTop; // (integer 0 - Height, default: implementation dependent)
	int m_textRectWidth; // (integer 0 - Width, default: Width)
	int m_textRectHeight; // (integer 0 - Height, default: implementation dependent)
	int m_textBreakWidth; // (integer 0 - text rectangle width, default: TextWidth)

	int m_bitmapWidth;
	int m_bitmapHeight;
};

#endif
