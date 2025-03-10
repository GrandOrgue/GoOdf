/*
 * GUIManualPanel.h is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GoOdf.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#ifndef GUIMANUALPANEL_H
#define GUIMANUALPANEL_H

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/fontpicker.h>
#include <wx/bmpcbox.h>
#include "GUIManual.h"

class GUIManualPanel : public wxPanel {
public:
	GUIManualPanel(wxWindow *parent);
	~GUIManualPanel();

	void setManual(GUIManual *manual);
	void updatePositionValues();
	void setTooltipsEnabled(bool isEnabled);

private:
	DECLARE_EVENT_TABLE()

	wxRadioButton *m_keyColorInvertedYes;
	wxRadioButton *m_keyColorInvertedNo;
	wxRadioButton *m_keyColorWoodYes;
	wxRadioButton *m_keyColorWoodNo;
	wxRadioButton *m_forceWriteWidthYes;
	wxRadioButton *m_forceWriteWidthNo;
	wxBitmapComboBox *m_dispImageNbrBox; // this should be filled with different images depending on the inverted and wooden booleans
	wxSpinCtrl *m_elementPosXSpin;
	wxSpinCtrl *m_elementPosYSpin;
	wxListBox *m_availableKeyTypes;
	wxListBox *m_availableKeyNumbers;
	wxButton *m_addKey;
	wxButton *m_removeKey;
	wxButton *m_copyKey;
	wxListBox *m_addedKeyTypes;

	// for each selected (added) key type next members will items fill with existing data and also allow adjustments
	// but they will only be filled and available when there is a selected key type added...
	wxTextCtrl *m_imageOnPathField;
	wxButton *m_addImageOnBtn;
	wxTextCtrl *m_maskOnPathField;
	wxButton *m_addMaskOnBtn;
	wxTextCtrl *m_imageOffPathField;
	wxButton *m_addImageOffBtn;
	wxTextCtrl *m_maskOffPathField;
	wxButton *m_addMaskOffBtn;
	wxSpinCtrl *m_widthSpin; // 0 - 500, default bitmap width
	wxSpinCtrl *m_offsetSpin; // (integer -500 - 500 bitmap width, default: 0)
	wxSpinCtrl *m_offsetYSpin; // (integer 0 - 500, default: 0)
	wxSpinCtrl *m_mouseRectLeftSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_mouseRectTopSpin; // (integer 0 - Height, default: 0)
	wxSpinCtrl *m_mouseRectWidthSpin; // (integer 0 - Width, default: Width)
	wxSpinCtrl *m_mouseRectHeightSpin; // (integer 0 - Height, default: Height)

	wxSpinCtrl *m_displayKeysSpin;
	wxSpinCtrl *m_firstNoteSpin;
	wxChoice *m_displayKeyChoice;
	wxSpinCtrl *m_backendMIDIkey;
	wxSpinCtrl *m_frontendMIDIkey;

	wxButton *m_removeManualBtn;

	GUIManual *m_manual;

	wxString m_lastUsedPath;

	void OnKeyColorInvertedRadio(wxCommandEvent& event);
	void OnKeyColorWoodRadio(wxCommandEvent& event);
	void OnForceWriteWidthRadio(wxCommandEvent& event);
	void OnImageNumberChoice(wxCommandEvent& event);
	void OnPositionXSpin(wxSpinEvent& event);
	void OnPositionYSpin(wxSpinEvent& event);
	void OnAvailableKeyTypesChoice(wxCommandEvent& event);
	void OnAvailableKeyNumbersChoice(wxCommandEvent& event);
	void OnAddKeyBtn(wxCommandEvent& event);
	void OnRemoveKeyBtn(wxCommandEvent& event);
	void OnCopyKeyBtn(wxCommandEvent& event);
	void OnAddedKeysChoice(wxCommandEvent& event);

	void OnAddImageOnBtn(wxCommandEvent& event);
	void OnAddImageOffBtn(wxCommandEvent& event);
	void OnAddMaskOffBtn(wxCommandEvent& event);
	void OnAddMaskOnBtn(wxCommandEvent& event);
	void OnWidthSpin(wxSpinEvent& event);
	void OnOffsetSpin(wxSpinEvent& event);
	void OnOffsetYSpin(wxSpinEvent& event);
	void OnMouseRectLeftSpin(wxSpinEvent& event);
	void OnMouseRectTopSpin(wxSpinEvent& event);
	void OnMouseRectWidthSpin(wxSpinEvent& event);
	void OnMouseRectHeightSpin(wxSpinEvent& event);

	void OnDisplayKeysSpin(wxSpinEvent& event);
	void OnFirstNoteSpin(wxSpinEvent& event);
	void OnDisplayKeyChoice(wxCommandEvent& event);
	void OnBackendMIDIkeySpin(wxSpinEvent& event);
	void OnFrontendMIDIkeySpin(wxSpinEvent& event);

	void OnRemoveManualBtn(wxCommandEvent& event);

	wxString GetPathForImageFile();
	void UpdateExistingSelectedKeyData();
	void SetupKeyChoiceAndMapping();
	void UpdateAddedKeyTypes();
	void SetupImageNbrBoxContent();
};

#endif
