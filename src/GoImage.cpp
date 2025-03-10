/*
 * Image.cpp is part of GoOdf.
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

#include "GoImage.h"
#include "GOODFFunctions.h"
#include <wx/filename.h>

GoImage::GoImage() {
	m_imagePath = wxEmptyString;
	m_maskPath = wxEmptyString;
	m_imageOrginalWidth = 0;
	m_imageOrginalHeight = 0;
	m_owningPanelWidth = 0;
	m_owningPanelHeight = 0;
	m_positionX = 0;
	m_positionY = 0;
	m_width = 1;
	m_height = 1;
	m_tileOffsetX = 0;
	m_tileOffsetY = 0;
}

GoImage::~GoImage() {

}

void GoImage::write(wxTextFile *outFile) {
	// we need to remove base odf path from image and mask paths
	wxString relativeFileName = getRelativeImagePath();
	wxString fullImageLine = GOODF_functions::fixSeparator(wxT("Image=") + relativeFileName);
	outFile->AddLine(fullImageLine);
	if (m_maskPath != wxEmptyString) {
		wxString relativeMaskName = getRelativeMaskPath();
		wxString fullMaskLine = GOODF_functions::fixSeparator(wxT("Mask=") + relativeMaskName);
		outFile->AddLine(fullMaskLine);
	}
	if (m_positionX != 0)
		outFile->AddLine(wxT("PositionX=") + wxString::Format(wxT("%i"), m_positionX));
	if (m_positionY != 0)
		outFile->AddLine(wxT("PositionY=") + wxString::Format(wxT("%i"), m_positionY));
	if (m_width != m_imageOrginalWidth)
		outFile->AddLine(wxT("Width=") + wxString::Format(wxT("%i"), m_width));
	if (m_height != m_imageOrginalHeight)
		outFile->AddLine(wxT("Height=") + wxString::Format(wxT("%i"), m_height));
	if (m_tileOffsetX != 0)
		outFile->AddLine(wxT("TileOffsetX=") + wxString::Format(wxT("%i"), m_tileOffsetX));
	if (m_tileOffsetY != 0)
		outFile->AddLine(wxT("TileOffsetY=") + wxString::Format(wxT("%i"), m_tileOffsetY));
}

bool GoImage::read(wxFileConfig *cfg, Organ *readOrgan) {
	bool imageIsValid = false;

	wxString relImgPath = cfg->Read("Image", wxEmptyString);
	wxString imgPath = GOODF_functions::checkIfFileExist(relImgPath, readOrgan);
	if (imgPath != wxEmptyString) {
		wxImage img = wxImage(imgPath);
		if (img.IsOk()) {
			imageIsValid = true;
			setImage(imgPath);
			int width = img.GetWidth();
			int height = img.GetHeight();
			setOriginalWidth(width);
			setOriginalHeight(height);
			int imgWidth = static_cast<int>(cfg->ReadLong("Width", width));
			if (imgWidth > 0 && imgWidth <= getOwningPanelWidth()) {
				// the width value read is in valid range
				setWidth(imgWidth);
			} else {
				setWidth(width);
			}
			int imgHeight = static_cast<int>(cfg->ReadLong("Height", height));
			if (imgHeight > 0 && imgHeight <= getOwningPanelHeight()) {
				setHeight(imgHeight);
			} else {
				setHeight(height);
			}
		}
		wxString relMaskPath = cfg->Read("Mask", wxEmptyString);
		wxString maskPath = GOODF_functions::checkIfFileExist(relMaskPath, readOrgan);
		if (maskPath != wxEmptyString) {
			wxImage mask = wxImage(maskPath);
			if (mask.IsOk()) {
				int width = mask.GetWidth();
				int height = mask.GetHeight();
				if (width == getOriginalWidth() && height == getOriginalHeight()) {
					setMask(maskPath);
				}
			}
		}
		int posX = static_cast<int>(cfg->ReadLong("PositionX", 0));
		if (posX > -1 && posX <= m_owningPanelWidth) {
			setPositionX(posX);
		}
		int posY = static_cast<int>(cfg->ReadLong("PositionY", 0));
		if (posY > -1 && posY <= m_owningPanelHeight) {
			setPositionY(posY);
		}
		int tileOffsetX = static_cast<int>(cfg->ReadLong("TileOffsetX", 0));
		if (tileOffsetX > -1 && tileOffsetX < getOriginalWidth()) {
			setTileOffsetX(tileOffsetX);
		}
		int tileOffsetY = static_cast<int>(cfg->ReadLong("TileOffsetY", 0));
		if (tileOffsetY > -1 && tileOffsetY < getOriginalHeight()) {
			setTileOffsetY(tileOffsetY);
		}
	}

	return imageIsValid;
}

int GoImage::getHeight() const {
	return m_height;
}

void GoImage::setHeight(int height) {
	m_height = height;
}

const wxString& GoImage::getImage() const {
	return m_imagePath;
}

void GoImage::setImage(wxString mImage) {
	m_imagePath = mImage;
}

const wxString& GoImage::getMask() const {
	return m_maskPath;
}

void GoImage::setMask(wxString mask) {
	m_maskPath = mask;
}

int GoImage::getPositionX() const {
	return m_positionX;
}

void GoImage::setPositionX(int posX) {
	m_positionX = posX;
}

int GoImage::getPositionY() const {
	return m_positionY;
}

void GoImage::setPositionY(int posY) {
	m_positionY = posY;
}

int GoImage::getTileOffsetX() const {
	return m_tileOffsetX;
}

void GoImage::setTileOffsetX(int tileOffsetX) {
	m_tileOffsetX = tileOffsetX;
}

int GoImage::getTileOffsetY() const {
	return m_tileOffsetY;
}

void GoImage::setTileOffsetY(int tileOffsetY) {
	m_tileOffsetY = tileOffsetY;
}

int GoImage::getWidth() const {
	return m_width;
}

void GoImage::setWidth(int width) {
	m_width = width;
}

void GoImage::updateOwningPanelSize(int width, int height) {
	m_owningPanelWidth = width;
	m_owningPanelHeight = height;
}

int GoImage::getOwningPanelWidth() {
	return m_owningPanelWidth;
}

int GoImage::getOwningPanelHeight() {
	return m_owningPanelHeight;
}

void GoImage::setOwningPanelWidth(int width) {
	m_owningPanelWidth = width;
}

void GoImage::setOwningPanelHeight(int height) {
	m_owningPanelHeight = height;
}

void GoImage::setOriginalWidth(int width) {
	m_imageOrginalWidth = width;
}

void GoImage::setOriginalHeight(int height) {
	m_imageOrginalHeight = height;
}

int GoImage::getOriginalWidth() {
	return m_imageOrginalWidth;
}

int GoImage::getOriginalHeight() {
	return m_imageOrginalHeight;
}

wxString GoImage::getRelativeImagePath() {
	if (m_imagePath != wxEmptyString)
		return GOODF_functions::removeBaseOdfPath(m_imagePath);
	else
		return wxEmptyString;
}

wxString GoImage::getRelativeMaskPath() {
	if (m_maskPath != wxEmptyString)
		return GOODF_functions::removeBaseOdfPath(m_maskPath);
	else
		return wxEmptyString;
}

wxString GoImage::getImageNameOnly() {
	if (m_imagePath != wxEmptyString) {
		wxFileName fileName = wxFileName(m_imagePath);
		return fileName.GetName();
	} else {
		return wxEmptyString;
	}
}

wxString GoImage::getMaskNameOnly() {
	if (m_maskPath != wxEmptyString) {
		wxFileName fileName = wxFileName(m_maskPath);
		return fileName.GetName();
	} else {
		return wxEmptyString;
	}
}

wxBitmap GoImage::getBitmap() {
	wxImage img(m_imagePath);
	if (img.IsOk()) {
		wxBitmap bmp;
		if (m_maskPath == wxEmptyString) {
			bmp = wxBitmap(m_imagePath, img.GetType());
		} else {
			wxImage maskImg(m_maskPath);
			if (maskImg.IsOk()) {
				img.SetMaskFromImage(maskImg, 0xFF, 0xFF, 0xFF);
				bmp = wxBitmap(img);
			} else {
				bmp = wxBitmap(m_imagePath, img.GetType());
			}
		}
		return bmp;
	} else {
		return wxNullBitmap;
	}
}
