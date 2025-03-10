/*
 * GoImagePanel.cpp is part of GoOdf.
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

#include "GoImagePanel.h"
#include <wx/statline.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include "GOODFFunctions.h"
#include "GOODF.h"

// Event table
BEGIN_EVENT_TABLE(GoImagePanel, wxPanel)
	EVT_BUTTON(ID_ADD_IMAGE_BTN, GoImagePanel::OnAddImageBtn)
	EVT_BUTTON(ID_ADD_MASK_BTN, GoImagePanel::OnAddMaskBtn)
	EVT_SPINCTRL(ID_IMAGE_POS_X_SPIN, GoImagePanel::OnPosXSpin)
	EVT_SPINCTRL(ID_IMAGE_POS_Y_SPIN, GoImagePanel::OnPosYSpin)
	EVT_SPINCTRL(ID_IMAGE_WIDTH_SPIN, GoImagePanel::OnWidthSpin)
	EVT_SPINCTRL(ID_IMAGE_HEIGHT_SPIN, GoImagePanel::OnHeightSpin)
	EVT_SPINCTRL(ID_IMAGE_TILE_X_SPIN, GoImagePanel::OnTileOffsetXSpin)
	EVT_SPINCTRL(ID_IMAGE_TILE_Y_SPIN, GoImagePanel::OnTileOffsetYSpin)
	EVT_BUTTON(ID_IMAGE_REMOVE_BTN, GoImagePanel::OnRemoveImageBtn)
END_EVENT_TABLE()

GoImagePanel::GoImagePanel(wxWindow *parent) : wxPanel(parent) {
	m_image = NULL;
	m_isFirstRemoval = true;

	wxBoxSizer *panelSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *imageText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Image: ")
	);
	firstRow->Add(imageText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_imagePathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	firstRow->Add(m_imagePathField, 1, wxEXPAND|wxALL, 5);
	m_addImageBtn = new wxButton(
		this,
		ID_ADD_IMAGE_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	firstRow->Add(m_addImageBtn, 0, wxALL, 5);
	panelSizer->Add(firstRow, 0, wxGROW);

	wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *maskText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Mask: ")
	);
	secondRow->Add(maskText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_maskPathField = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	);
	secondRow->Add(m_maskPathField, 1, wxEXPAND|wxALL, 5);
	m_addMaskBtn = new wxButton(
		this,
		ID_ADD_MASK_BTN,
		wxT("Browse..."),
		wxDefaultPosition,
		wxDefaultSize,
		0
	);
	secondRow->Add(m_addMaskBtn, 0, wxALL, 5);
	m_addMaskBtn->Disable();
	panelSizer->Add(secondRow, 0, wxGROW);

	wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *posXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Position X: ")
	);
	thirdRow->Add(posXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_posXSpin = new wxSpinCtrl(
		this,
		ID_IMAGE_POS_X_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		0,
		0
	);
	thirdRow->Add(m_posXSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(thirdRow, 0, wxGROW);

	wxBoxSizer *fourthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *posYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Position Y: ")
	);
	fourthRow->Add(posYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_posYSpin = new wxSpinCtrl(
		this,
		ID_IMAGE_POS_Y_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		0,
		0
	);
	fourthRow->Add(m_posYSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fourthRow, 0, wxGROW);

	wxBoxSizer *fifthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *widthText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Width: ")
	);
	fifthRow->Add(widthText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_widthSpin = new wxSpinCtrl(
		this,
		ID_IMAGE_WIDTH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	fifthRow->Add(m_widthSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(fifthRow, 0, wxGROW);

	wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *heightText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Height: ")
	);
	sixthRow->Add(heightText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_heightSpin = new wxSpinCtrl(
		this,
		ID_IMAGE_HEIGHT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		1,
		1
	);
	sixthRow->Add(m_heightSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(sixthRow, 0, wxGROW);

	wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tileXText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Tile offset X: ")
	);
	seventhRow->Add(tileXText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_tileOffsetXSpin = new wxSpinCtrl(
		this,
		ID_IMAGE_TILE_X_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		0,
		0
	);
	seventhRow->Add(m_tileOffsetXSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(seventhRow, 0, wxGROW);

	wxBoxSizer *eighthRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *tileYText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Tile offset Y: ")
	);
	eighthRow->Add(tileYText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_tileOffsetYSpin = new wxSpinCtrl(
		this,
		ID_IMAGE_TILE_Y_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		0,
		0,
		0
	);
	eighthRow->Add(m_tileOffsetYSpin, 0, wxEXPAND|wxALL, 5);
	panelSizer->Add(eighthRow, 0, wxGROW);

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	panelSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *bottomRow = new wxBoxSizer(wxHORIZONTAL);
	bottomRow->AddStretchSpacer();
	removeImageBtn = new wxButton(
		this,
		ID_IMAGE_REMOVE_BTN,
		wxT("Remove this image")
	);
	bottomRow->Add(removeImageBtn, 0, wxALIGN_CENTER|wxALL, 10);
	bottomRow->AddStretchSpacer();
	panelSizer->Add(bottomRow, 0, wxEXPAND);

	SetSizer(panelSizer);
}

GoImagePanel::~GoImagePanel() {

}

void GoImagePanel::setImage(GoImage *image) {
	m_image = image;
	UpdateControlValues();
}

void GoImagePanel::setIsFirstRemoval(bool value) {
	m_isFirstRemoval = value;
}

void GoImagePanel::updatePositionValues() {
	if (m_image) {
		m_posXSpin->SetValue(m_image->getPositionX());
		m_posYSpin->SetValue(m_image->getPositionY());
	}
}

void GoImagePanel::setTooltipsEnabled(bool isEnabled) {
	if (isEnabled) {
		m_imagePathField->SetToolTip(wxT("Relative path to the image will be shown here. Not manually editable, use the 'Browse...' button to select the image."));
		m_addImageBtn->SetToolTip(wxT("This button brings up a file dialog where the image file can be chosen."));
		m_maskPathField->SetToolTip(wxT("Relative path to the mask will be shown here. Not manually editable, use the 'Browse...' button to select the mask. NOTE: Not needed for .png images that are the preferred image format for GrandOrgue!"));
		m_addMaskBtn->SetToolTip(wxT("This button brings up a file dialog where the mask file corresponding to the image file can be chosen."));
		m_posXSpin->SetToolTip(wxT("Position coordinate for the x value."));
		m_posYSpin->SetToolTip(wxT("Position coordinate for the y value"));
		m_widthSpin->SetToolTip(wxT("Width of image to use. NOTE: Changing this value does not scale the image! If set greater than real image width - the shown image in GrandOrgue will be tiled. If set smaller than real image width - the shown image in GrandOrgue will be cropped."));
		m_heightSpin->SetToolTip(wxT("Height of image to use. NOTE: Changing this value does not scale the image! If set greater than real image height - the shown image in GrandOrgue will be tiled. If set smaller than real image height - the shown image in GrandOrgue will be cropped."));
		m_tileOffsetXSpin->SetToolTip(wxT("Offset any tiling for the x axis."));
		m_tileOffsetYSpin->SetToolTip(wxT("Offset any tiling for the y axis."));
	} else {
		m_imagePathField->SetToolTip(wxEmptyString);
		m_addImageBtn->SetToolTip(wxEmptyString);
		m_maskPathField->SetToolTip(wxEmptyString);
		m_addMaskBtn->SetToolTip(wxEmptyString);
		m_posXSpin->SetToolTip(wxEmptyString);
		m_posYSpin->SetToolTip(wxEmptyString);
		m_widthSpin->SetToolTip(wxEmptyString);
		m_heightSpin->SetToolTip(wxEmptyString);
		m_tileOffsetXSpin->SetToolTip(wxEmptyString);
		m_tileOffsetYSpin->SetToolTip(wxEmptyString);
	}
}

void GoImagePanel::OnAddImageBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString imageFilePath;
	wxString defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
	if (defaultPath == wxEmptyString) {
		if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
			defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
		else
			defaultPath = wxStandardPaths::Get().GetDocumentsDir();
	}

	wxFileDialog fileDialog(
		this,
		wxT("Select image file"),
		defaultPath,
		"",
		"Image files (*.png;*.bmp;*.gif;*.jpg;*.ico)|*.png;*.bmp;*.gif;*.jpg;*.ico;*.PNG;*.BMP;*.GIF;*.JPG;*.ICO;*.Png;*.Bmp;*.Gif;*.Jpg;*.Ico",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	imageFilePath = fileDialog.GetPath();

	wxImage img = wxImage(imageFilePath);
	if (img.IsOk()) {
		m_image->setImage(imageFilePath);
		int width = img.GetWidth();
		int height = img.GetHeight();
		m_image->setOriginalWidth(width);
		m_image->setOriginalHeight(height);
		m_image->setWidth(width);
		m_image->setHeight(height);
		UpdateControlValues();
		wxString updatedLabel = m_image->getImageNameOnly();
		::wxGetApp().m_frame->OrganTreeChildItemLabelChanged(updatedLabel);
		::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
	}
}

void GoImagePanel::OnAddMaskBtn(wxCommandEvent& WXUNUSED(event)) {
	wxString imageFilePath;
	wxString defaultPath = ::wxGetApp().m_frame->m_organ->getOdfRoot();
	if (defaultPath == wxEmptyString) {
		if (::wxGetApp().m_frame->GetDefaultOrganDirectory() != wxEmptyString)
			defaultPath = ::wxGetApp().m_frame->GetDefaultOrganDirectory();
		else
			defaultPath = wxStandardPaths::Get().GetDocumentsDir();
	}

	wxFileDialog fileDialog(
		this,
		wxT("Select mask file (size must match the image file)"),
		defaultPath,
		"",
		"Image files (*.png;*.bmp;*.gif;*.jpg;*.ico)|*.png;*.bmp;*.gif;*.jpg;*.ico;*.PNG;*.BMP;*.GIF;*.JPG;*.ICO;*.Png;*.Bmp;*.Gif;*.Jpg;*.Ico",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	imageFilePath = fileDialog.GetPath();

	wxImage img = wxImage(imageFilePath);
	if (img.IsOk()) {

		int width = img.GetWidth();
		int height = img.GetHeight();
		if (width == m_image->getOriginalWidth() && height == m_image->getOriginalHeight()) {
			m_image->setMask(imageFilePath);
		}
		UpdateControlValues();
	}
	::wxGetApp().m_frame->m_organ->setModified(true);
}

void GoImagePanel::OnPosXSpin(wxSpinEvent& event) {
	m_image->setPositionX(event.GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GoImagePanel::OnPosYSpin(wxSpinEvent& event) {
	m_image->setPositionY(event.GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GoImagePanel::OnWidthSpin(wxSpinEvent& event) {
	m_image->setWidth(event.GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GoImagePanel::OnHeightSpin(wxSpinEvent& event) {
	m_image->setHeight(event.GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GoImagePanel::OnTileOffsetXSpin(wxSpinEvent& event) {
	m_image->setTileOffsetX(event.GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GoImagePanel::OnTileOffsetYSpin(wxSpinEvent& event) {
	m_image->setTileOffsetY(event.GetValue());
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GoImagePanel::OnRemoveImageBtn(wxCommandEvent& WXUNUSED(event)) {
	if (m_isFirstRemoval) {
		wxMessageDialog msg(this, wxT("Are you really sure you want to delete this image?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (msg.ShowModal() == wxID_YES) {
			// then remove this item from the owning panel
			::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
			m_isFirstRemoval = false;
		}
	} else {
		::wxGetApp().m_frame->RemoveCurrentItemFromOrgan();
	}
	::wxGetApp().m_frame->PanelGUIPropertyIsChanged();
}

void GoImagePanel::UpdateControlValues() {
	if (m_image->getImage() != wxEmptyString) {
		m_imagePathField->SetValue(m_image->getRelativeImagePath());
		m_addMaskBtn->Enable();
	} else {
		m_imagePathField->SetValue(wxEmptyString);
		m_addMaskBtn->Disable();
	}
	if (m_image->getMask() != wxEmptyString) {
		m_maskPathField->SetValue(m_image->getRelativeMaskPath());
	} else {
		m_maskPathField->SetValue(wxEmptyString);
	}
	m_posXSpin->SetRange(0, m_image->getOwningPanelWidth());
	m_posXSpin->SetValue(m_image->getPositionX());
	m_posYSpin->SetRange(0, m_image->getOwningPanelHeight());
	m_posYSpin->SetValue(m_image->getPositionY());
	m_widthSpin->SetRange(1, m_image->getOwningPanelWidth());
	m_widthSpin->SetValue(m_image->getWidth());
	m_heightSpin->SetRange(1, m_image->getOwningPanelHeight());
	m_heightSpin->SetValue(m_image->getHeight());
	m_tileOffsetXSpin->SetRange(0, m_image->getOriginalWidth());
	m_tileOffsetXSpin->SetValue(m_image->getTileOffsetX());
	m_tileOffsetYSpin->SetRange(0, m_image->getOriginalHeight());
	m_tileOffsetYSpin->SetValue(m_image->getTileOffsetY());
}
