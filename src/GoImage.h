/*
 * GoImage.h is part of GoOdf.
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

#ifndef GOIMAGE_H
#define GOIMAGE_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>

class Organ;

class GoImage {
public:
	GoImage();
	~GoImage();

	void write(wxTextFile *outFile);
	bool read(wxFileConfig *cfg, Organ *readOrgan);

	int getHeight() const;
	void setHeight(int height);
	const wxString& getImage() const;
	void setImage(wxString image);
	const wxString& getMask() const;
	void setMask(wxString mask);
	int getPositionX() const;
	void setPositionX(int posX);
	int getPositionY() const;
	void setPositionY(int posY);
	int getTileOffsetX() const;
	void setTileOffsetX(int tileOffsetX);
	int getTileOffsetY() const;
	void setTileOffsetY(int tileOffsetY);
	int getWidth() const;
	void setWidth(int width);
	void updateOwningPanelSize(int width, int height);
	int getOwningPanelWidth();
	int getOwningPanelHeight();
	void setOwningPanelWidth(int width);
	void setOwningPanelHeight(int height);
	void setOriginalWidth(int width);
	void setOriginalHeight(int height);
	int getOriginalWidth();
	int getOriginalHeight();
	wxString getRelativeImagePath();
	wxString getRelativeMaskPath();
	wxString getImageNameOnly();
	wxString getMaskNameOnly();
	wxBitmap getBitmap();

private:
	wxString m_imagePath; // full path to file
	wxString m_maskPath; // full path to file
	int m_imageOrginalWidth;
	int m_imageOrginalHeight;
	int m_owningPanelWidth;
	int m_owningPanelHeight;
	int m_positionX; // (0 - panel width, default: 0)
	int m_positionY; // (0 - panel height, default: 0)
	int m_width; // (0 - panel width, default: bitmap width)
	int m_height; // ( 0 - panel height, default: bitmap height)
	int m_tileOffsetX; //(0 - bitmap width, default: 0)
	int m_tileOffsetY; // 0 - bitmap height?, default: 0
};

#endif
