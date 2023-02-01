/*
 * GUIElements.h is part of GOODF.
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

#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include "GoColor.h"
#include "GoFontSize.h"
#include "GoImage.h"
#include "Manual.h"

class GoPanel;

class GUIElement {
public:
	GUIElement();
	virtual ~GUIElement();

	virtual void write(wxTextFile *outFile);

	wxString getType();
	void setType(wxString type);
	int getPosX();
	void setPosX(int x);
	int getPosY();
	void setPosY(int y);
	void setOwningPanel(GoPanel *panel);
	GoPanel* getOwningPanel();

	// virtual methods
/*
	virtual int getDispButtonCol() const;
	virtual void setDispButtonCol(int dispButtonCol);
	virtual int getDispButtonRow() const;
	virtual void setDispButtonRow(int dispButtonRow);
	virtual int getDispDrawstopCol() const;
	virtual void setDispDrawstopCol(int dispDrawstopCol);
	virtual int getDispDrawstopRow() const;
	virtual void setDispDrawstopRow(int dispDrawstopRow);
	virtual int getDispImageNum() const;
	virtual void setDispImageNum(int dispImageNum);
	virtual bool isDispKeyLabelOnLeft() const;
	virtual void setDispKeyLabelOnLeft(bool dispKeyLabelOnLeft);
	virtual const GoColor& getDispLabelColour() const;
	virtual void setDispLabelColour(const GoColor &dispLabelColour);
	virtual const GoFontSize& getDispLabelFontSize() const;
	virtual void setDispLabelFontSize(const GoFontSize &dispLabelFontSize);
	virtual const wxString& getDispLabelText() const;
	virtual void setDispLabelText(const wxString &dispLabelText);
	virtual bool isDisplayAsPiston() const;
	virtual void setDisplayAsPiston(bool displayAsPiston);
	virtual const wxString& getDisplLabelFontName() const;
	virtual void setDisplLabelFontName(const wxString &displLabelFontName);
	virtual int getHeight() const;
	virtual void setHeight(int height);
	virtual const wxString& getImageOff() const;
	virtual void setImageOff(const wxString &imageOff);
	virtual const wxString& getImageOn() const;
	virtual void setImageOn(const wxString &imageOn);
	virtual const wxString& getMaskOff() const;
	virtual void setMaskOff(const wxString &maskOff);
	virtual const wxString& getMaskOn() const;
	virtual void setMaskOn(const wxString &maskOn);
	virtual int getMouseRadius() const;
	virtual void setMouseRadius(int mouseRadius);
	virtual int getMouseRectHeight() const;
	virtual void setMouseRectHeight(int mouseRectHeight);
	virtual int getMouseRectLeft() const;
	virtual void setMouseRectLeft(int mouseRectLeft);
	virtual int getMouseRectTop() const;
	virtual void setMouseRectTop(int mouseRectTop);
	virtual int getMouseRectWidth() const;
	virtual void setMouseRectWidth(int mouseRectWidth);
	virtual int getTextBreakWidth() const;
	virtual void setTextBreakWidth(int textBreakWidth);
	virtual int getTextRectHeight() const;
	virtual void setTextRectHeight(int textRectHeight);
	virtual int getTextRectLeft() const;
	virtual void setTextRectLeft(int textRectLeft);
	virtual int getTextRectTop() const;
	virtual void setTextRectTop(int textRectTop);
	virtual int getTextRectWidth() const;
	virtual void setTextRectWidth(int textRectWidth);
	virtual int getTileOffsetX() const;
	virtual void setTileOffsetX(int tileOffsetX);
	virtual int getTileOffsetY() const;
	virtual void setTileOffsetY(int tileOffsetY);
	virtual int getWidth() const;
	virtual void setWidth(int width);
	virtual int getEnclosureStyle() const;
	virtual void setEnclosureStyle(int enclosureStyle);
	virtual int getMouseAxisEnd() const;
	virtual void setMouseAxisEnd(int mouseAxisEnd);
	virtual int getMouseAxisStart() const;
	virtual void setMouseAxisStart(int mouseAxisStart);
	virtual void addBitmap(GoImage bitmap);
	virtual unsigned getNumberOfBitmaps();
	virtual GoImage* getBitmapAtIndex(unsigned index);
	virtual bool isDispAtTopOfDrawstopCol() const;
	virtual void setDispAtTopOfDrawstopCol(bool dispAtTopOfDrawstopCol);
	virtual bool isDispSpanDrawstopColToRight() const;
	virtual void setDispSpanDrawstopColToRight(bool dispSpanDrawstopColToRight);
	virtual int getDispXpos() const;
	virtual void setDispXpos(int dispXpos);
	virtual int getDispYpos() const;
	virtual void setDispYpos(int dispYpos);
	virtual bool isFreeXPlacement() const;
	virtual void setFreeXPlacement(bool freeXPlacement);
	virtual bool isFreeYPlacement() const;
	virtual void setFreeYPlacement(bool freeYPlacement);
	virtual const wxString& getImage() const;
	virtual void setImage(const wxString &image);
	virtual const wxString& getMask() const;
	virtual void setMask(const wxString &mask);
	virtual const wxString& getName() const;
	virtual void setName(const wxString &name);
	virtual const Manual* getManual();
	virtual void addKeytype(KEYTYPE type);
	virtual unsigned getNumberOfKeytypes();
	virtual KEYTYPE* getKeytypeAt(unsigned index);
	virtual const wxArrayString& getAvailableKeytypes();
	virtual const wxArrayString& getAvailableKeynumbers();
	virtual bool getDispKeyColourInverted();
	virtual void setDispKeyColourInverted(bool inverted);
	virtual bool getDispKeyColourWooden();
	virtual void setDispKeyColurWooden(bool wooden);
	virtual int getDisplayFirstNote();
	virtual void setDisplayFirstNote(int first);
	virtual unsigned getNumberOfDisplayKeys();
	virtual std::pair<int, int>* getDisplayKeyAt(unsigned index);
*/
protected:
	wxString m_type;
	int m_positionX;
	int m_positionY;

private:
	GoPanel *m_owningPanel;
};

#endif
