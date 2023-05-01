/* 
 * GOODFFrame.cpp is a part of GOODF software
 * Copyright (C) 2023 Lars Palo and contributors (see AUTHORS)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo DOT se
 */

#include "GOODFFrame.h"
#include <wx/aboutdlg.h>
#include "GOODF.h"
#include "GOODFDef.h"
#include <wx/textfile.h>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include <wx/button.h>
#include "Enclosure.h"
#include "Windchestgroup.h"
#include "OrganFileParser.h"
#include "CopyElementAttributesDialog.h"

// Event table
BEGIN_EVENT_TABLE(GOODFFrame, wxFrame)
	EVT_MENU(wxID_ABOUT, GOODFFrame::OnAbout)
	EVT_MENU(wxID_HELP, GOODFFrame::OnHelp)
	EVT_MENU(wxID_EXIT, GOODFFrame::OnQuit)
	EVT_MENU(ID_WRITE_ODF, GOODFFrame::OnWriteODF)
	EVT_MENU(ID_NEW_ORGAN, GOODFFrame::OnNewOrgan)
	EVT_MENU(ID_READ_ORGAN, GOODFFrame::OnReadOrganFile)
	EVT_TREE_SEL_CHANGED(ID_ORGAN_TREE, GOODFFrame::OnOrganTreeSelectionChanged)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_ORGAN_TREE, GOODFFrame::OnOrganTreeRightClicked)
	EVT_BUTTON(ID_ADD_ENCLOSURE_BTN, GOODFFrame::OnAddNewEnclosure)
	EVT_BUTTON(ID_ADD_TREMULANT_BTN, GOODFFrame::OnAddNewTremulant)
	EVT_BUTTON(ID_ADD_WINDCHEST_BTN, GOODFFrame::OnAddNewWindchestgroup)
	EVT_BUTTON(ID_ADD_SWITCH_BTN, GOODFFrame::OnAddNewSwitch)
	EVT_BUTTON(ID_ADD_RANK_BTN, GOODFFrame::OnAddNewRank)
	EVT_BUTTON(ID_ADD_MANUAL_BTN, GOODFFrame::OnAddNewManual)
	EVT_BUTTON(ID_ADD_DIV_CPLR_BTN, GOODFFrame::OnAddNewDivisionalCoupler)
	EVT_BUTTON(ID_ADD_GENERAL_BTN, GOODFFrame::OnAddNewGeneral)
	EVT_BUTTON(ID_ADD_REVERSIBLE_PISTON_BTN, GOODFFrame::OnAddNewReversiblePiston)
	EVT_BUTTON(ID_ADD_PANEL_BTN, GOODFFrame::OnAddNewPanel)
END_EVENT_TABLE()

GOODFFrame::GOODFFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
	// Start with an empty organ
	m_organ = new Organ();
	m_organHasBeenSaved = false;

	// Create a file menu
	m_fileMenu = new wxMenu();

	// Add file menu items
	m_fileMenu->Append(ID_NEW_ORGAN, wxT("&New Organ\tCtrl+N"), wxT("Create a new organ"));
	m_fileMenu->Append(ID_READ_ORGAN, wxT("Open file\tCtrl+O"), wxT("Open existing .organ file"));
	m_fileMenu->Append(ID_WRITE_ODF, wxT("Write ODF\tCtrl+S"), wxT("Write/Save the .organ file"));
	m_fileMenu->Append(wxID_EXIT, wxT("&Exit\tCtrl+Q"), wxT("Quit this program"));

	// Create a help menu
	m_helpMenu = new wxMenu();

	// Add help menu items
	m_helpMenu->Append(wxID_ABOUT, wxT("&About..."), wxT("Show about dialog"));
	m_helpMenu->Append(wxID_HELP, wxT("&GOODF Help \tF1"), wxT("Show application help"));

	// Create a menu bar and append the menus to it
	m_menuBar = new wxMenuBar();
	m_menuBar->Append(m_fileMenu, wxT("&File"));
	m_menuBar->Append(m_helpMenu, wxT("&Help"));

	// Attach menu bar to frame
	SetMenuBar(m_menuBar);

	// Create a top level sizer for the frame
	wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);
	
	// The main splitter window
	m_Splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D | wxSP_LIVE_UPDATE);
	m_Splitter->SetMinimumPaneSize(200);
	m_Splitter->SetSashGravity(0);
	topSizer->Add(m_Splitter, 1, wxEXPAND, 0);
	
	addManual = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addManSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addManBtn = new wxButton(addManual, ID_ADD_MANUAL_BTN, wxT("Create new manual"));
	addManSizer->Add(addManBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addManual->SetSizer(addManSizer);
	addManual->Hide();

	addWindchest = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addWindSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addWindchestBtn = new wxButton(addWindchest, ID_ADD_WINDCHEST_BTN, wxT("Create new windchest"));
	addWindSizer->Add(addWindchestBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addWindchest->SetSizer(addWindSizer);
	addWindchest->Hide();

	addEnclosure = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addEnclosureSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addEnclosureBtn = new wxButton(addEnclosure, ID_ADD_ENCLOSURE_BTN, wxT("Create new enclosure"));
	addEnclosureSizer->Add(addEnclosureBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addEnclosure->SetSizer(addEnclosureSizer);
	addEnclosure->Hide();

	addTremulant = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addTremSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addTremulantBtn = new wxButton(addTremulant, ID_ADD_TREMULANT_BTN, wxT("Create new tremulant"));
	addTremSizer->Add(addTremulantBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addTremulant->SetSizer(addTremSizer);
	addTremulant->Hide();

	addRank = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addRankSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addRankBtn = new wxButton(addRank, ID_ADD_RANK_BTN, wxT("Create new rank"));
	addRankSizer->Add(addRankBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addRank->SetSizer(addRankSizer);
	addRank->Hide();

	addSwitch = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addSwitchSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addSwitchBtn = new wxButton(addSwitch, ID_ADD_SWITCH_BTN, wxT("Create new switch"));
	addSwitchSizer->Add(addSwitchBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addSwitch->SetSizer(addSwitchSizer);
	addSwitch->Hide();

	addReversiblePiston = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addPistonSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addPistonBtn = new wxButton(addReversiblePiston, ID_ADD_REVERSIBLE_PISTON_BTN, wxT("Create new reversible piston"));
	addPistonSizer->Add(addPistonBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addReversiblePiston->SetSizer(addPistonSizer);
	addReversiblePiston->Hide();

	addDivisionalCoupler = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addDivCplrSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addDivCplrBtn = new wxButton(addDivisionalCoupler, ID_ADD_DIV_CPLR_BTN, wxT("Create new divisional coupler"));
	addDivCplrSizer->Add(addDivCplrBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addDivisionalCoupler->SetSizer(addDivCplrSizer);
	addDivisionalCoupler->Hide();

	addGeneral = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addGeneralSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addGeneralBtn = new wxButton(addGeneral, ID_ADD_GENERAL_BTN, wxT("Create new general"));
	addGeneralSizer->Add(addGeneralBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addGeneral->SetSizer(addGeneralSizer);
	addGeneral->Hide();

	addPanel = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addPanelSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addPanelBtn = new wxButton(addPanel, ID_ADD_PANEL_BTN, wxT("Create new panel"));
	addPanelSizer->Add(addPanelBtn, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	addPanel->SetSizer(addPanelSizer);
	addPanel->Hide();

	m_enclosurePanel = new EnclosurePanel(m_Splitter);
	m_enclosurePanel->Hide();

	m_tremulantPanel = new TremulantPanel(m_Splitter);
	m_tremulantPanel->Hide();

	m_windchestPanel = new WindchestgroupPanel(m_Splitter);
	m_windchestPanel->Hide();

	m_switchPanel = new SwitchPanel(m_Splitter);
	m_switchPanel->Hide();

	m_rankPanel = new RankPanel(m_Splitter);
	m_rankPanel->Hide();

	m_stopPanel = new StopPanel(m_Splitter);
	m_stopPanel->Hide();

	m_manualPanel = new ManualPanel(m_Splitter);
	m_manualPanel->Hide();

	m_couplerPanel = new CouplerPanel(m_Splitter);
	m_couplerPanel->Hide();

	m_divisionalPanel = new DivisionalPanel(m_Splitter);
	m_divisionalPanel->Hide();

	m_divCplrPanel = new DivisionalCouplerPanel(m_Splitter);
	m_divCplrPanel->Hide();

	m_generalPanel = new GeneralPanel(m_Splitter);
	m_generalPanel->Hide();

	m_reversiblePistonPanel = new ReversiblePistonPanel(m_Splitter);
	m_reversiblePistonPanel->Hide();

	m_panelPanel = new GoPanelPanel(m_Splitter);
	m_panelPanel->Hide();

	m_dispMetricsPanel = new DisplayMetricsPanel(m_Splitter);
	m_dispMetricsPanel->Hide();

	m_imagePanel = new GoImagePanel(m_Splitter);
	m_imagePanel->Hide();

	m_guiButtonPanel = new GUIButtonPanel(m_Splitter);
	m_guiButtonPanel->Hide();

	m_guiEnclosurePanel = new GUIEnclosurePanel(m_Splitter);
	m_guiEnclosurePanel->Hide();

	m_guiLabelPanel = new GUILabelPanel(m_Splitter);
	m_guiLabelPanel->Hide();

	m_guiManualPanel = new GUIManualPanel(m_Splitter);
	m_guiManualPanel->Hide();

	wxPanel *leftSplitPanel = new wxPanel(m_Splitter, wxID_ANY);

	wxBoxSizer *leftSplitSizer = new wxBoxSizer(wxVERTICAL);

	// Here goes the wxTreeCtrl
	m_organTreeCtrl = new wxTreeCtrl(leftSplitPanel, ID_ORGAN_TREE, wxDefaultPosition, wxDefaultSize);
	tree_organ = m_organTreeCtrl->AddRoot("[Organ]");
	tree_manuals = m_organTreeCtrl->AppendItem(tree_organ, "Manuals");
	tree_windchestgrps = m_organTreeCtrl->AppendItem(tree_organ, "Windchestgroups");
	tree_enclosures = m_organTreeCtrl->AppendItem(tree_organ, "Enclosures");
	tree_tremulants = m_organTreeCtrl->AppendItem(tree_organ, "Tremulants");
	tree_ranks = m_organTreeCtrl->AppendItem(tree_organ, "Ranks");
	tree_switches = m_organTreeCtrl->AppendItem(tree_organ, "Switches");
	tree_reversiblePistons = m_organTreeCtrl->AppendItem(tree_organ, "Reversible Pistons");
	tree_divisionalCouplers = m_organTreeCtrl->AppendItem(tree_organ, "Divisional Couplers");
	tree_generals = m_organTreeCtrl->AppendItem(tree_organ, "Generals");
	tree_panels = m_organTreeCtrl->AppendItem(tree_organ, "Panels");
	SetupOrganMainPanel();

	// Expand all the nodes
	m_organTreeCtrl->ExpandAll();

	leftSplitSizer->Add(m_organTreeCtrl, 1, wxEXPAND);
	leftSplitPanel->SetSizer(leftSplitSizer);

	m_organPanel = new OrganPanel(m_organ, m_Splitter);

	leftSplitPanel->SetMinSize(leftSplitPanel->GetSizer()->GetMinSize());
	m_organPanel->SetMinSize(m_organPanel->GetSizer()->GetMinSize());

	m_Splitter->SplitVertically(leftSplitPanel, m_organPanel);

	// Use top sizer for layout and size window accordingly and prevent it
	// from being resized to smaller size
	SetSizerAndFit(topSizer);

	// Additionally set a minimum width of 1024 x 768
	wxSize calculated = GetSize();
	if (calculated.GetWidth() < 1024 || calculated.GetHeight() < 768)
		SetMinSize(wxSize(1024, 768));
	else
		SetMinSize(calculated);
	m_Splitter->SetSashPosition(205);
}

GOODFFrame::~GOODFFrame() {
	if (m_organ)
		delete m_organ;
}

void GOODFFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxAboutDialogInfo info;
	info.SetName(wxT("GOODF"));
	info.SetVersion(wxT(GOODF_VERSION));
	info.SetDescription(wxT("A software for creating and editing GrandOrgue ODFs"));
	info.SetCopyright(wxT("Copyright (C) 2023 Lars Palo and contributors (see AUTHORS)\nReleased under GNU GPLv3 licence"));
	info.SetWebSite(wxT("https://github.com/larspalo/GOODF"));

	wxAboutBox(info);
}

void GOODFFrame::OnHelp(wxCommandEvent& WXUNUSED(event)) {
	::wxGetApp().m_helpController->DisplayContents();
}

void GOODFFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	// Destroy the frame
	Close();
}

void GOODFFrame::OnWriteODF(wxCommandEvent& WXUNUSED(event)) {
	if (m_organPanel->getOdfPath().IsEmpty() || m_organPanel->getOdfName().IsEmpty()) {
		wxMessageDialog incomplete(this, wxT("Path and name for ODF must be set!"), wxT("Cannot write ODF"), wxOK|wxCENTRE);
		incomplete.ShowModal();
		return;
	}
	wxString fullFileName = m_organPanel->getOdfPath() + wxFILE_SEP_PATH + m_organPanel->getOdfName() + wxT(".organ");
	wxTextFile *odfFile = new wxTextFile(fullFileName);
	if (odfFile->Exists() && !m_organHasBeenSaved) {
		wxMessageDialog dlg(this, wxT("ODF file already exist. Do you want to overwrite it?"), wxT("Existing ODF file"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (dlg.ShowModal() == wxID_YES) {
			odfFile->Open(fullFileName);
			odfFile->Clear();
		} else {
			delete odfFile;
			return;
		}
	} else {
		odfFile->Create(fullFileName);
	}
	m_organ->writeOrgan(odfFile);

	odfFile->Write(wxTextFileType_Dos, wxCSConv("ISO-8859-1"));
	if (!m_organHasBeenSaved) {
		wxMessageDialog msg(this, wxT("ODF file ") + m_organPanel->getOdfName() + wxT(".organ has been written!"), wxT("ODF file written"), wxOK|wxCENTRE);
		msg.ShowModal();
	}
	odfFile->Close();
	delete odfFile;
	m_organHasBeenSaved = true;
}

void GOODFFrame::OnReadOrganFile(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->isModified()) {
		wxMessageDialog dlg(this, wxT("All current organ data will be lost! Do you want to proceed?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (dlg.ShowModal() != wxID_YES) {
			return;
		}
	}
	if (m_organ) {
		delete m_organ;
		m_organ = NULL;
	}
	m_organ = new Organ();
	removeAllItemsFromTree();
	m_organHasBeenSaved = false;

	wxString organFilePath;

	wxFileDialog fileDialog(
		this,
		wxT("Select .organ file to open"),
		wxStandardPaths::Get().GetDocumentsDir(),
		"",
		"GrandOrgue ODF files (*.organ)|*.organ;*.ORGAN",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() == wxID_CANCEL)
		return;

	organFilePath = fileDialog.GetPath();

	OrganFileParser parser(organFilePath, m_organ);
	if (parser.isOrganReady()) {
		wxFileName f_name = wxFileName(organFilePath);
		m_organPanel->setCurrentOrgan(m_organ);
		m_organPanel->setOdfPath(f_name.GetPath());
		m_organPanel->setOdfName(f_name.GetName());
		for (unsigned i = 0; i < m_organ->getNumberOfEnclosures(); i++) {
			m_organTreeCtrl->AppendItem(tree_enclosures, m_organ->getOrganEnclosureAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfTremulants(); i++) {
			m_organTreeCtrl->AppendItem(tree_tremulants, m_organ->getOrganTremulantAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfWindchestgroups(); i++) {
			m_organTreeCtrl->AppendItem(tree_windchestgrps, m_organ->getOrganWindchestgroupAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfSwitches() ; i++) {
			m_organTreeCtrl->AppendItem(tree_switches, m_organ->getOrganSwitchAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfRanks() ; i++) {
			m_organTreeCtrl->AppendItem(tree_ranks, m_organ->getOrganRankAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfManuals() ; i++) {
			wxTreeItemId thisManual = m_organTreeCtrl->AppendItem(tree_manuals, m_organ->getOrganManualAt(i)->getName());

			// create the subitems for Stops, Couplers and Divisionals
			wxTreeItemId thisManualStops = m_organTreeCtrl->AppendItem(thisManual, wxT("Stops"));
			wxTreeItemId thisManualCouplers = m_organTreeCtrl->AppendItem(thisManual, wxT("Couplers"));
			wxTreeItemId thisManualDivisionals = m_organTreeCtrl->AppendItem(thisManual, wxT("Divisionals"));

			// then they can be populated
			for (unsigned j = 0; j < m_organ->getOrganManualAt(i)->getNumberOfStops(); j++) {
				m_organTreeCtrl->AppendItem(thisManualStops, m_organ->getOrganManualAt(i)->getStopAt(j)->getName());
			}
			for (unsigned j = 0; j < m_organ->getOrganManualAt(i)->getNumberOfCouplers(); j++) {
				m_organTreeCtrl->AppendItem(thisManualCouplers, m_organ->getOrganManualAt(i)->getCouplerAt(j)->getName());
			}
			for (unsigned j = 0; j < m_organ->getOrganManualAt(i)->getNumberOfDivisionals(); j++) {
				m_organTreeCtrl->AppendItem(thisManualDivisionals, m_organ->getOrganManualAt(i)->getDivisionalAt(j)->getName());
			}
		}
		for (unsigned i = 0; i < m_organ->getNumberOfOrganDivisionalCouplers() ; i++) {
			m_organTreeCtrl->AppendItem(tree_divisionalCouplers, m_organ->getOrganDivisionalCouplerAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfGenerals() ; i++) {
			m_organTreeCtrl->AppendItem(tree_generals, m_organ->getOrganGeneralAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfReversiblePistons() ; i++) {
			m_organTreeCtrl->AppendItem(tree_reversiblePistons, m_organ->getReversiblePistonAt(i)->getName());
		}
		for (unsigned i = 0; i < m_organ->getNumberOfPanels(); i++) {
			wxTreeItemId thisPanel = m_organTreeCtrl->AppendItem(tree_panels, m_organ->getOrganPanelAt(i)->getName());

			// create the subitems for Displaymetrics, Images and GUIElements
			m_organTreeCtrl->AppendItem(thisPanel, wxT("Displaymetrics"));
			wxTreeItemId panelImages = m_organTreeCtrl->AppendItem(thisPanel, wxT("Images"));
			m_organTreeCtrl->AppendItem(thisPanel, wxT("GUI Elements"));

			for (unsigned j = 0; j < m_organ->getOrganPanelAt(i)->getNumberOfImages(); j++) {
				m_organTreeCtrl->AppendItem(panelImages, m_organ->getOrganPanelAt(i)->getImageAt(j)->getImageNameOnly());
			}
			RebuildPanelGuiElementsInTree(i);
		}
	} else {
		if (m_organ) {
			delete m_organ;
			m_organ = NULL;
		}
		m_organ = new Organ();
		removeAllItemsFromTree();
		m_organHasBeenSaved = false;
		SetupOrganMainPanel();
		m_organPanel->setCurrentOrgan(m_organ);
		m_organPanel->setOdfPath(wxEmptyString);
		m_organPanel->setOdfName(wxEmptyString);
	}
	m_organ->organElementHasChanged();
	m_organTreeCtrl->SelectItem(tree_organ);
}

void GOODFFrame::OrganTreeChildItemLabelChanged(wxString label) {
	wxTreeItemId selected;
	selected = m_organTreeCtrl->GetSelection();
	m_organTreeCtrl->SetItemText(selected, label);
}

void GOODFFrame::OnOrganTreeSelectionChanged(wxTreeEvent& event) {
	wxTreeItemId selectedId = event.GetItem();

	if (selectedId == m_organTreeCtrl->GetRootItem()) {
		if (m_Splitter->GetWindow2() != m_organPanel) {
			int sashPos = m_Splitter->GetSashPosition();
			m_Splitter->GetWindow2()->Hide();
			m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_organPanel);
			m_Splitter->SetSashPosition(sashPos);
			m_organPanel->Show();
		}

		return;
	} else if (m_organTreeCtrl->GetItemParent(selectedId) == m_organTreeCtrl->GetRootItem()) {
		// this is for the direct children of the [Organ]
		if (selectedId == tree_manuals) {
			if (m_Splitter->GetWindow2() != addManual) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addManual);
				m_Splitter->SetSashPosition(sashPos);
				addManual->Show();
			}

			return;
		}
		if (selectedId == tree_windchestgrps) {
			if (m_Splitter->GetWindow2() != addWindchest) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addWindchest);
				m_Splitter->SetSashPosition(sashPos);
				addWindchest->Show();
			}

			return;
		}
		if (selectedId == tree_enclosures) {
			if (m_Splitter->GetWindow2() != addEnclosure) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addEnclosure);
				m_Splitter->SetSashPosition(sashPos);
				addEnclosure->Show();
			}

			return;
		}
		if (selectedId == tree_tremulants) {
			if (m_Splitter->GetWindow2() != addTremulant) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addTremulant);
				m_Splitter->SetSashPosition(sashPos);
				addTremulant->Show();
			}

			return;
		}
		if (selectedId == tree_ranks) {
			if (m_Splitter->GetWindow2() != addRank) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addRank);
				m_Splitter->SetSashPosition(sashPos);
				addRank->Show();
			}

			return;
		}

		if (selectedId == tree_switches) {
			if (m_Splitter->GetWindow2() != addSwitch) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addSwitch);
				m_Splitter->SetSashPosition(sashPos);
				addSwitch->Show();
			}

			return;
		}

		if (selectedId == tree_reversiblePistons) {
			if (m_Splitter->GetWindow2() != addReversiblePiston) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addReversiblePiston);
				m_Splitter->SetSashPosition(sashPos);
				addReversiblePiston->Show();
			}

			return;
		}

		if (selectedId == tree_divisionalCouplers) {
			if (m_Splitter->GetWindow2() != addDivisionalCoupler) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addDivisionalCoupler);
				m_Splitter->SetSashPosition(sashPos);
				addDivisionalCoupler->Show();
			}

			return;
		}

		if (selectedId == tree_generals) {
			if (m_Splitter->GetWindow2() != addGeneral) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addGeneral);
				m_Splitter->SetSashPosition(sashPos);
				addGeneral->Show();
			}

			return;
		}

		if (selectedId == tree_panels) {
			if (m_Splitter->GetWindow2() != addPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), addPanel);
				m_Splitter->SetSashPosition(sashPos);
				addPanel->Show();
			}

			return;
		}
	} else {
		// this is a child of some other group get what index it has
		int selectedIndex = 0;
		wxTreeItemId parentId = m_organTreeCtrl->GetItemParent(selectedId);
		wxTreeItemId grandParentId = m_organTreeCtrl->GetItemParent(parentId);
		int numChildrens = m_organTreeCtrl->GetChildrenCount(parentId, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(parentId, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(parentId, cookie);
			if (selectedId == currentId)
				selectedIndex = i;
		}

		if (parentId == tree_enclosures) {
			if (m_Splitter->GetWindow2() != m_enclosurePanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_enclosurePanel->setEnclosure(m_organ->getOrganEnclosureAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_enclosurePanel);
				m_Splitter->SetSashPosition(sashPos);
				m_enclosurePanel->Show();
			} else {
				// another enclosure might be chosen, update panel
				m_enclosurePanel->setEnclosure(m_organ->getOrganEnclosureAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_tremulants) {
			if (m_Splitter->GetWindow2() != m_tremulantPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_tremulantPanel->setTremulant(m_organ->getOrganTremulantAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_tremulantPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_tremulantPanel->Show();
			} else {
				// another tremulant might be chosen, update panel
				m_tremulantPanel->setTremulant(m_organ->getOrganTremulantAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_windchestgrps) {
			if (m_Splitter->GetWindow2() != m_windchestPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_windchestPanel->setWindchest(m_organ->getOrganWindchestgroupAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_windchestPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_windchestPanel->Show();
			} else {
				// another windchest might be chosen, update panel
				m_windchestPanel->setWindchest(m_organ->getOrganWindchestgroupAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_switches) {
			if (m_Splitter->GetWindow2() != m_switchPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_switchPanel->setSwitch(m_organ->getOrganSwitchAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_switchPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_switchPanel->Show();
			} else {
				// another switch might be chosen, update panel
				m_switchPanel->setSwitch(m_organ->getOrganSwitchAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_reversiblePistons) {
			if (m_Splitter->GetWindow2() != m_reversiblePistonPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_reversiblePistonPanel->setReversiblePiston(m_organ->getReversiblePistonAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_reversiblePistonPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_reversiblePistonPanel->Show();
			} else {
				// another piston might be chosen, update panel
				m_reversiblePistonPanel->setReversiblePiston(m_organ->getReversiblePistonAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_divisionalCouplers) {
			if (m_Splitter->GetWindow2() != m_divCplrPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_divCplrPanel->setDivisionalCoupler(m_organ->getOrganDivisionalCouplerAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_divCplrPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_divCplrPanel->Show();
			} else {
				// another divisional coupler might be chosen, update panel
				m_divCplrPanel->setDivisionalCoupler(m_organ->getOrganDivisionalCouplerAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_generals) {
			if (m_Splitter->GetWindow2() != m_generalPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_generalPanel->setGeneral(m_organ->getOrganGeneralAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_generalPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_generalPanel->Show();
			} else {
				// another divisional coupler might be chosen, update panel
				m_generalPanel->setGeneral(m_organ->getOrganGeneralAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_ranks) {
			if (m_Splitter->GetWindow2() != m_rankPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_rankPanel->setRank(m_organ->getOrganRankAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_rankPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_rankPanel->Show();
			} else {
				// another rank might be chosen, update panel
				m_rankPanel->setRank(m_organ->getOrganRankAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_manuals) {
			if (m_Splitter->GetWindow2() != m_manualPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_manualPanel->setManual(m_organ->getOrganManualAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_manualPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_manualPanel->Show();
			} else {
				// another manual might be chosen, update panel
				m_manualPanel->setManual(m_organ->getOrganManualAt(selectedIndex));
			}

			return;
		}

		if (parentId == tree_panels) {
			if (m_Splitter->GetWindow2() != m_panelPanel) {
				int sashPos = m_Splitter->GetSashPosition();
				m_panelPanel->setPanel(m_organ->getOrganPanelAt(selectedIndex));
				m_Splitter->GetWindow2()->Hide();
				m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_panelPanel);
				m_Splitter->SetSashPosition(sashPos);
				m_panelPanel->Show();
			} else {
				// another panel might be chosen, update panel
				m_panelPanel->setPanel(m_organ->getOrganPanelAt(selectedIndex));
			}

			return;
		}

		if (grandParentId == tree_panels) {
			// the selected item is the Displaymetrics, Images or GUI Elements of a panel
			// the parentId is what panel but we need its index
			int thePanelIndex = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
			wxTreeItemId currentComparisonPanel = m_organTreeCtrl->GetLastChild(tree_panels);
			while (currentComparisonPanel.IsOk()) {
				thePanelIndex--;
				if (currentComparisonPanel == parentId)
					break;

				currentComparisonPanel = m_organTreeCtrl->GetPrevSibling(currentComparisonPanel);
			}
			// if thePanelIndex is positive it now contains the panel index
			if (m_organTreeCtrl->GetItemText(selectedId).IsSameAs(wxT("Displaymetrics")) && thePanelIndex > -1) {
				// the Displaymetrics of a panel is selected
				if (m_Splitter->GetWindow2() != m_dispMetricsPanel) {
					int sashPos = m_Splitter->GetSashPosition();
					m_dispMetricsPanel->setDisplayMetrics(m_organ->getOrganPanelAt(thePanelIndex)->getDisplayMetrics());
					m_Splitter->GetWindow2()->Hide();
					m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_dispMetricsPanel);
					m_Splitter->SetSashPosition(sashPos);
					m_dispMetricsPanel->Show();
				} else {
					// another panels displaymetrics might be chosen, update panel
					m_dispMetricsPanel->setDisplayMetrics(m_organ->getOrganPanelAt(thePanelIndex)->getDisplayMetrics());
				}
			} else {
				// the Images category or GUI Element category of a panel is selected
				// but not a specific item so we push selection back to panel
				m_organTreeCtrl->SelectItem(parentId);
			}
			return;
		}

		if (grandParentId != tree_organ && m_organTreeCtrl->GetItemParent(grandParentId) == tree_panels) {
			// This could match either a specific image or GUI element belonging to a panel
			// the grandparentId is what panel but we need its index
			int thePanelIndex = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
			wxTreeItemId currentComparisonPanel = m_organTreeCtrl->GetLastChild(tree_panels);
			while (currentComparisonPanel.IsOk()) {
				thePanelIndex--;
				if (currentComparisonPanel == grandParentId)
					break;

				currentComparisonPanel = m_organTreeCtrl->GetPrevSibling(currentComparisonPanel);
			}
			// if thePanelIndex is positive it now contains the panel index

			if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Images")) && thePanelIndex > -1) {
				// This is a specific image
				if (m_Splitter->GetWindow2() != m_imagePanel) {
					int sashPos = m_Splitter->GetSashPosition();
					m_imagePanel->setImage(m_organ->getOrganPanelAt(thePanelIndex)->getImageAt(selectedIndex));
					m_Splitter->GetWindow2()->Hide();
					m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_imagePanel);
					m_Splitter->SetSashPosition(sashPos);
					m_imagePanel->Show();
				} else {
					// another image might be chosen, update panel
					m_imagePanel->setImage(m_organ->getOrganPanelAt(thePanelIndex)->getImageAt(selectedIndex));
				}
			} else if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("GUI Elements")) && thePanelIndex > -1) {
				// This is a specific GUI Element

				// Test if it's a button type
				GUIButton *btnElement = dynamic_cast<GUIButton*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
				if (btnElement) {
					if (m_Splitter->GetWindow2() != m_guiButtonPanel) {
						int sashPos = m_Splitter->GetSashPosition();
						m_guiButtonPanel->setButton(btnElement);
						m_Splitter->GetWindow2()->Hide();
						m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_guiButtonPanel);
						m_Splitter->SetSashPosition(sashPos);
						m_guiButtonPanel->Show();
					} else {
						// another button type might be chosen, update panel
						m_guiButtonPanel->setButton(btnElement);
					}
					return;
				}

				// test if if's an enclosure type
				GUIEnclosure *encElement = dynamic_cast<GUIEnclosure*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
				if (encElement) {
					if (m_Splitter->GetWindow2() != m_guiEnclosurePanel) {
						int sashPos = m_Splitter->GetSashPosition();
						m_guiEnclosurePanel->setEnclosure(encElement);
						m_Splitter->GetWindow2()->Hide();
						m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_guiEnclosurePanel);
						m_Splitter->SetSashPosition(sashPos);
						m_guiEnclosurePanel->Show();
					} else {
						// another enclosure might be chosen, update panel
						m_guiEnclosurePanel->setEnclosure(encElement);
					}
					return;
				}

				// test if it's a label type
				GUILabel *labelElement = dynamic_cast<GUILabel*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
				if (labelElement) {
					if (m_Splitter->GetWindow2() != m_guiLabelPanel) {
						int sashPos = m_Splitter->GetSashPosition();
						m_guiLabelPanel->setLabel(labelElement);
						m_Splitter->GetWindow2()->Hide();
						m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_guiLabelPanel);
						m_Splitter->SetSashPosition(sashPos);
						m_guiLabelPanel->Show();
					} else {
						// another label might be chosen, update panel
						m_guiLabelPanel->setLabel(labelElement);
					}
					return;
				}

				// test if if's a manual type
				GUIManual *manElement = dynamic_cast<GUIManual*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
				if (manElement) {
					if (m_Splitter->GetWindow2() != m_guiManualPanel) {
						int sashPos = m_Splitter->GetSashPosition();
						m_guiManualPanel->setManual(manElement);
						m_Splitter->GetWindow2()->Hide();
						m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_guiManualPanel);
						m_Splitter->SetSashPosition(sashPos);
						m_guiManualPanel->Show();
					} else {
						// another manual might be chosen, update panel
						m_guiManualPanel->setManual(manElement);
					}
					return;
				}
			}
			return;
		}

		if (grandParentId == tree_manuals) {
			// the category of Stop, Coupler or Divisional is selected
			// so we push selection upwards to avoid issues
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (grandParentId != tree_organ && m_organTreeCtrl->GetItemParent(grandParentId) == tree_manuals) {
			// the selected item could be a Stop, Coupler or Divisional
			// selectedIndex is which index it has
			// the parentId tell which category it belongs to
			// the grandParentId is which manual but we need to get its index
			int theManualIndex = m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
			wxTreeItemId currentComparisonManual = m_organTreeCtrl->GetLastChild(tree_manuals);
			while (currentComparisonManual.IsOk()) {
				theManualIndex--;
				if (currentComparisonManual == grandParentId)
					break;

				currentComparisonManual = m_organTreeCtrl->GetPrevSibling(currentComparisonManual);
			}
			// if theManualIndex is positive it now contains the manual index

			if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Stops")) && theManualIndex > -1) {

				if (m_Splitter->GetWindow2() != m_stopPanel) {
					int sashPos = m_Splitter->GetSashPosition();
					m_stopPanel->setStop(m_organ->getOrganManualAt(theManualIndex)->getStopAt(selectedIndex));
					m_Splitter->GetWindow2()->Hide();
					m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_stopPanel);
					m_Splitter->SetSashPosition(sashPos);
					m_stopPanel->Show();
				} else {
					// another stop might be chosen, update panel
					m_stopPanel->setStop(m_organ->getOrganManualAt(theManualIndex)->getStopAt(selectedIndex));
				}

				return;

			} else if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Couplers")) && theManualIndex > -1) {

				if (m_Splitter->GetWindow2() != m_couplerPanel) {
					int sashPos = m_Splitter->GetSashPosition();
					m_couplerPanel->setCoupler(m_organ->getOrganManualAt(theManualIndex)->getCouplerAt(selectedIndex));
					m_Splitter->GetWindow2()->Hide();
					m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_couplerPanel);
					m_Splitter->SetSashPosition(sashPos);
					m_couplerPanel->Show();
				} else {
					// another stop might be chosen, update panel
					m_couplerPanel->setCoupler(m_organ->getOrganManualAt(theManualIndex)->getCouplerAt(selectedIndex));
				}

				return;

			} else if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Divisionals")) && theManualIndex > -1) {

				if (m_Splitter->GetWindow2() != m_divisionalPanel) {
					int sashPos = m_Splitter->GetSashPosition();
					m_divisionalPanel->setDivisional(m_organ->getOrganManualAt(theManualIndex)->getDivisionalAt(selectedIndex));
					m_Splitter->GetWindow2()->Hide();
					m_Splitter->ReplaceWindow(m_Splitter->GetWindow2(), m_divisionalPanel);
					m_Splitter->SetSashPosition(sashPos);
					m_divisionalPanel->Show();
				} else {
					// another divisional might be chosen, update panel
					m_divisionalPanel->setDivisional(m_organ->getOrganManualAt(theManualIndex)->getDivisionalAt(selectedIndex));
				}

				return;
			}
		}

	}
}

void GOODFFrame::OnOrganTreeRightClicked(wxTreeEvent& event) {
	wxTreeItemId selected;
	selected = m_organTreeCtrl->GetSelection();
	wxTreeItemId clickedId = event.GetItem();
	if (clickedId != selected || clickedId == tree_organ) {
		event.Skip();
		return;
	}
	wxTreeItemId parentId = m_organTreeCtrl->GetItemParent(clickedId);
	if (parentId == tree_organ) {
		event.Skip();
		return;
	}
	wxTreeItemId grandParentId = m_organTreeCtrl->GetItemParent(parentId);
	if (grandParentId != tree_organ && m_organTreeCtrl->GetItemParent(grandParentId) == tree_panels) {
		int selectedIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(parentId, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(parentId, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(parentId, cookie);
			if (clickedId == currentId)
				selectedIndex = i;
		}

		int thePanelIndex = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
		wxTreeItemId currentComparisonPanel = m_organTreeCtrl->GetLastChild(tree_panels);
		while (currentComparisonPanel.IsOk()) {
			thePanelIndex--;
			if (currentComparisonPanel == grandParentId)
				break;

			currentComparisonPanel = m_organTreeCtrl->GetPrevSibling(currentComparisonPanel);
		}

		if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("GUI Elements")) && thePanelIndex > -1) {
			// This is a specific GUI Element so we create the Copy Gui Element Attributes dialog
			CopyElementAttributesDialog copyElementDlg(m_organ->getOrganPanelAt(thePanelIndex), selectedIndex, this);
			if (copyElementDlg.ShowModal() == wxID_OK) {
				GoPanel *targetPanel = copyElementDlg.GetSelectedTargetPanel();
				wxArrayInt selectedTargetElements;
				if (copyElementDlg.GetSelectedElementIndices(selectedTargetElements)) {
					// we have some targets to copy to but we also need to get the actual source element

					// Test if the source a button type
					GUIButton *btnElement = dynamic_cast<GUIButton*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
					if (btnElement) {
						for (unsigned i = 0; i < selectedTargetElements.size(); i++) {
							GUIButton *targetBtn = dynamic_cast<GUIButton*>(targetPanel->getGuiElementAt(selectedTargetElements[i]));
							if (targetBtn) {
								// we copy images and display attributes but not any position type of information
								targetBtn->setWidth(btnElement->getWidth());
								targetBtn->setHeight(btnElement->getHeight());
								targetBtn->setDispImageNum(btnElement->getDispImageNum());
								targetBtn->setImageOn(btnElement->getImageOn());
								targetBtn->setImageOff(btnElement->getImageOff());
								targetBtn->setMaskOn(btnElement->getMaskOn());
								targetBtn->setMaskOff(btnElement->getMaskOff());
								targetBtn->setBitmapWidth(btnElement->getBitmapWidth());
								targetBtn->setBitmapHeight(btnElement->getBitmapHeight());
								targetBtn->setDispLabelFont(btnElement->getDispLabelFont());
								targetBtn->setDispLabelFontSize(btnElement->getDispLabelFontSize()->getSizeValue());
								targetBtn->setMouseRectLeft(btnElement->getMouseRectLeft());
								targetBtn->setMouseRectTop(btnElement->getMouseRectTop());
								targetBtn->setMouseRectWidth(btnElement->getMouseRectWidth());
								targetBtn->setMouseRectHeight(btnElement->getMouseRectHeight());
								targetBtn->setMouseRadius(btnElement->getMouseRadius());
								targetBtn->setTextRectLeft(btnElement->getTextRectLeft());
								targetBtn->setTextRectTop(btnElement->getTextRectTop());
								targetBtn->setTextRectWidth(btnElement->getTextRectWidth());
								targetBtn->setTextRectHeight(btnElement->getTextRectHeight());
								targetBtn->setTextBreakWidth(btnElement->getTextBreakWidth());
								targetBtn->setDispLabelColour(*(btnElement->getDispLabelColour()));
								targetBtn->setDisplayAsPiston(btnElement->isDisplayAsPiston());
								targetBtn->setDispKeyLabelOnLeft(btnElement->isDispKeyLabelOnLeft());
							}
						}
						return;
					}

					// test if it's an enclosure type
					GUIEnclosure *encElement = dynamic_cast<GUIEnclosure*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
					if (encElement) {
						for (unsigned i = 0; i < selectedTargetElements.size(); i++) {
							GUIEnclosure *targetEnclosure = dynamic_cast<GUIEnclosure*>(targetPanel->getGuiElementAt(selectedTargetElements[i]));
							if (targetEnclosure) {
								targetEnclosure->setDispLabelColour(*(encElement->getDispLabelColour()));
								targetEnclosure->setDispLabelFontSize(encElement->getDispLabelFontSize()->getSizeValue());
								targetEnclosure->setDispLabelFont(encElement->getDispLabelFont());
								targetEnclosure->setEnclosureStyle(encElement->getEnclosureStyle());
								if (targetEnclosure->getNumberOfBitmaps() > 0) {
									int lastExistingBmpIdx = targetEnclosure->getNumberOfBitmaps() - 1;
									while (lastExistingBmpIdx >= 0) {
										targetEnclosure->removeBitmapAt(lastExistingBmpIdx);
										lastExistingBmpIdx--;
									}
								}
								for (unsigned j = 0; j < encElement->getNumberOfBitmaps(); j++) {
									targetEnclosure->addBitmap(*(encElement->getBitmapAtIndex(j)));
								}
								targetEnclosure->setWidth(encElement->getWidth());
								targetEnclosure->setHeight(encElement->getHeight());
								targetEnclosure->setTileOffsetX(encElement->getTileOffsetX());
								targetEnclosure->setTileOffsetY(encElement->getTileOffsetY());
								targetEnclosure->setMouseRectLeft(encElement->getMouseRectLeft());
								targetEnclosure->setMouseRectTop(encElement->getMouseRectTop());
								targetEnclosure->setMouseRectWidth(encElement->getMouseRectWidth());
								targetEnclosure->setMouseRectHeight(encElement->getMouseRectHeight());
								targetEnclosure->setMouseAxisStart(encElement->getMouseAxisStart());
								targetEnclosure->setMouseAxisEnd(encElement->getMouseAxisEnd());
								targetEnclosure->setTextRectLeft(encElement->getTextRectLeft());
								targetEnclosure->setTextRectTop(encElement->getTextRectTop());
								targetEnclosure->setTextRectWidth(encElement->getTextRectWidth());
								targetEnclosure->setTextRectHeight(encElement->getTextRectHeight());
								targetEnclosure->setTextBreakWidth(encElement->getTextBreakWidth());
								targetEnclosure->setBitmapWidth(encElement->getBitmapWidth());
								targetEnclosure->setBitmapHeight(encElement->getBitmapHeight());
							}
						}
						return;
					}

					// test if it's a label type
					GUILabel *labelElement = dynamic_cast<GUILabel*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
					if (labelElement) {
						for (unsigned i = 0; i < selectedTargetElements.size(); i++) {
							GUILabel *targetLabel = dynamic_cast<GUILabel*>(targetPanel->getGuiElementAt(selectedTargetElements[i]));
							if (targetLabel) {
								targetLabel->setDispLabelColour(*(labelElement->getDispLabelColour()));
								targetLabel->setDispLabelFontSize(labelElement->getDispLabelFontSize()->getSizeValue());
								targetLabel->setDispLabelFont(labelElement->getDispLabelFont());
								targetLabel->setDispImageNum(labelElement->getDispImageNum());
								targetLabel->getImage()->setImage(labelElement->getImage()->getImage());
								targetLabel->getImage()->setMask(labelElement->getImage()->getMask());
								targetLabel->setWidth(labelElement->getWidth());
								targetLabel->setHeight(labelElement->getHeight());
								targetLabel->setTileOffsetX(labelElement->getTileOffsetX());
								targetLabel->setTileOffsetY(labelElement->getTileOffsetY());
								targetLabel->setTextRectLeft(labelElement->getTextRectLeft());
								targetLabel->setTextRectTop(labelElement->getTextRectTop());
								targetLabel->setTextRectWidth(labelElement->getTextRectWidth());
								targetLabel->setTextRectHeight(labelElement->getTextRectHeight());
								targetLabel->setTextBreakWidth(labelElement->getTextBreakWidth());
								targetLabel->setBitmapWidth(labelElement->getBitmapWidth());
								targetLabel->setBitmapHeight(labelElement->getBitmapHeight());
							}
						}
						return;
					}

					// test if if's a manual type
					GUIManual *manElement = dynamic_cast<GUIManual*>(m_organ->getOrganPanelAt(thePanelIndex)->getGuiElementAt(selectedIndex));
					if (manElement) {
						for (unsigned i = 0; i < selectedTargetElements.size(); i++) {
							GUIManual *targetMan = dynamic_cast<GUIManual*>(targetPanel->getGuiElementAt(selectedTargetElements[i]));
							if (targetMan) {
								// we copy images and display attributes but not any position type of information
								targetMan->setDispKeyColourInverted(manElement->getDispKeyColourInverted());
								targetMan->setDispKeyColurWooden(manElement->getDispKeyColourWooden());

								// if any keytypes already exist in target we remove them first
								if (targetMan->getNumberOfKeytypes() > 0) {
									int lastExistingKeyIdx = targetMan->getNumberOfKeytypes() - 1;
									while (lastExistingKeyIdx >= 0) {
										targetMan->removeKeytypeAt(lastExistingKeyIdx);
										lastExistingKeyIdx--;
									}
								}

								// then we add the new ones from the source manual
								for (unsigned j = 0; j < manElement->getNumberOfKeytypes(); j++) {
									KEYTYPE *currentKey = manElement->getKeytypeAt(j);
									if (currentKey->KeytypeIdentifier.StartsWith(wxT("Key"))) {
										// We need to make sure that the target manual doesn't get a key number that it cannot contain
										if (targetMan->getAvailableKeynumbers().Index(currentKey->KeytypeIdentifier) == wxNOT_FOUND) {
											// so if that key number is not available for the target we skip this keytype
											continue;
										}
									}

									targetMan->addKeytype(currentKey->KeytypeIdentifier);
									KEYTYPE *targetKey = targetMan->getKeytypeAt(targetMan->getNumberOfKeytypes() - 1);
									targetKey->ImageOn = currentKey->ImageOn;
									targetKey->ImageOff = currentKey->ImageOff;
									targetKey->Width = currentKey->Width;
									targetKey->Offset = currentKey->Offset;
									targetKey->YOffset = currentKey->YOffset;
									targetKey->MouseRectLeft = currentKey->MouseRectLeft;
									targetKey->MouseRectTop = currentKey->MouseRectTop;
									targetKey->MouseRectWidth = currentKey->MouseRectWidth;
									targetKey->MouseRectHeight = currentKey->MouseRectHeight;
									targetKey->BitmapWidth = currentKey->BitmapWidth;
									targetKey->BitmapHeight = currentKey->BitmapHeight;
								}
							}
						}
						return;
					}
				}
			}
		}
	} else {
		event.Skip();
	}
}

void GOODFFrame::OnAddNewEnclosure(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfEnclosures() < 50) {
		Enclosure newEnclosure;
		m_organ->addEnclosure(newEnclosure);

		m_organTreeCtrl->SelectItem(m_organTreeCtrl->AppendItem(tree_enclosures, newEnclosure.getName()));
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 50 enclosures!"), wxT("Too many enclosures"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewTremulant(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfTremulants() < 10) {
		Tremulant newTremulant;
		m_organ->addTremulant(newTremulant);

		m_organTreeCtrl->SelectItem(m_organTreeCtrl->AppendItem(tree_tremulants, newTremulant.getName()));
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 10 tremulants!"), wxT("Too many tremulants"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::RemoveCurrentItemFromOrgan() {
	wxTreeItemId selected;
	selected = m_organTreeCtrl->GetSelection();
	if (selected.IsOk() && m_organTreeCtrl->GetItemParent(selected) != m_organTreeCtrl->GetRootItem()) {
		// this is indeed a removable item, get what index it has
		int selectedIndex = 0;
		wxTreeItemId parentId = m_organTreeCtrl->GetItemParent(selected);
		int numChildrens = m_organTreeCtrl->GetChildrenCount(parentId);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(parentId, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(parentId, cookie);
			if (selected == currentId)
				selectedIndex = i;
		}

		if (parentId == tree_enclosures) {
			m_organ->removeEnclosureAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_tremulants) {
			m_organ->removeTremulantAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_windchestgrps) {
			m_organ->removeWindchestgroupAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_switches) {
			m_organ->removeSwitchAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_reversiblePistons) {
			m_organ->removeReversiblePistonAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_divisionalCouplers) {
			m_organ->removeDivisionalCouplerAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_generals) {
			m_organ->removeGeneralAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_ranks) {
			m_organ->removeRankAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Stops"))) {
			wxTreeItemId theManual = m_organTreeCtrl->GetItemParent(parentId);

			int theManualIndex = m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
			wxTreeItemId currentComparisonManual = m_organTreeCtrl->GetLastChild(tree_manuals);
			while (currentComparisonManual.IsOk()) {
				theManualIndex--;
				if (currentComparisonManual == theManual)
					break;

				currentComparisonManual = m_organTreeCtrl->GetPrevSibling(currentComparisonManual);
			}
			// if theManualIndex is positive it now contains the manual index

			Stop *s = m_organ->getOrganManualAt(theManualIndex)->getStopAt(selectedIndex);
			m_organ->removeStop(s);
			m_organ->getOrganManualAt(theManualIndex)->removeStop(s);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(theManual);
		}

		if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Couplers"))) {
			wxTreeItemId theManual = m_organTreeCtrl->GetItemParent(parentId);

			int theManualIndex = m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
			wxTreeItemId currentComparisonManual = m_organTreeCtrl->GetLastChild(tree_manuals);
			while (currentComparisonManual.IsOk()) {
				theManualIndex--;
				if (currentComparisonManual == theManual)
					break;

				currentComparisonManual = m_organTreeCtrl->GetPrevSibling(currentComparisonManual);
			}
			// if theManualIndex is positive it now contains the manual index

			Coupler *c = m_organ->getOrganManualAt(theManualIndex)->getCouplerAt(selectedIndex);
			m_organ->removeCoupler(c);
			m_organ->getOrganManualAt(theManualIndex)->removeCoupler(c);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(theManual);
		}

		if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Divisionals"))) {
			wxTreeItemId theManual = m_organTreeCtrl->GetItemParent(parentId);

			int theManualIndex = m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
			wxTreeItemId currentComparisonManual = m_organTreeCtrl->GetLastChild(tree_manuals);
			while (currentComparisonManual.IsOk()) {
				theManualIndex--;
				if (currentComparisonManual == theManual)
					break;

				currentComparisonManual = m_organTreeCtrl->GetPrevSibling(currentComparisonManual);
			}
			// if theManualIndex is positive it now contains the manual index

			Divisional *d = m_organ->getOrganManualAt(theManualIndex)->getDivisionalAt(selectedIndex);
			m_organ->removeDivisional(d);
			m_organ->getOrganManualAt(theManualIndex)->removeDivisional(d);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(theManual);
		}

		if (parentId == tree_manuals) {
			m_organ->removeManualAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (parentId == tree_panels) {
			m_organ->removePanelAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(parentId);
		}

		if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("Images"))) {
			wxTreeItemId thePanel = m_organTreeCtrl->GetItemParent(parentId);

			int thePanelIndex = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
			wxTreeItemId currentComparisonPanel = m_organTreeCtrl->GetLastChild(tree_panels);
			while (currentComparisonPanel.IsOk()) {
				thePanelIndex--;
				if (currentComparisonPanel == thePanel)
					break;

				currentComparisonPanel = m_organTreeCtrl->GetPrevSibling(currentComparisonPanel);
			}
			// if thePanelIndex is positive it now contains the panel index

			GoImage *img = m_organ->getOrganPanelAt(thePanelIndex)->getImageAt(selectedIndex);
			m_organ->getOrganPanelAt(thePanelIndex)->removeImage(img);

			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(thePanel);
		}

		if (m_organTreeCtrl->GetItemText(parentId).IsSameAs(wxT("GUI Elements"))) {
			wxTreeItemId thePanel = m_organTreeCtrl->GetItemParent(parentId);

			int thePanelIndex = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
			wxTreeItemId currentComparisonPanel = m_organTreeCtrl->GetLastChild(tree_panels);
			while (currentComparisonPanel.IsOk()) {
				thePanelIndex--;
				if (currentComparisonPanel == thePanel)
					break;

				currentComparisonPanel = m_organTreeCtrl->GetPrevSibling(currentComparisonPanel);
			}
			// if thePanelIndex is positive it now contains the panel index
			m_organ->getOrganPanelAt(thePanelIndex)->removeGuiElementAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(thePanel);
		}
	}
}

void GOODFFrame::AddStopItemToTree() {
	// this is called from a manual that's currently selected in tree
	wxTreeItemId selectedManual = m_organTreeCtrl->GetSelection();
	wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(selectedManual);
	wxTreeItemId couplerChild = m_organTreeCtrl->GetPrevSibling(divisionalChild);
	wxTreeItemId stopChild = m_organTreeCtrl->GetPrevSibling(couplerChild);
	wxTreeItemId newStop = m_organTreeCtrl->AppendItem(stopChild, wxT("New Stop"));
	m_organTreeCtrl->ExpandAllChildren(selectedManual);
	m_organTreeCtrl->SelectItem(newStop);
}

void GOODFFrame::AddCouplerItemToTree() {
	// this is called from a manual that's currently selected in tree
	wxTreeItemId selectedManual = m_organTreeCtrl->GetSelection();
	wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(selectedManual);
	wxTreeItemId couplerChild = m_organTreeCtrl->GetPrevSibling(divisionalChild);
	wxTreeItemId newCoupler = m_organTreeCtrl->AppendItem(couplerChild, wxT("New Coupler"));
	m_organTreeCtrl->ExpandAllChildren(selectedManual);
	m_organTreeCtrl->SelectItem(newCoupler);
}

void GOODFFrame::AddDivisionalItemToTree() {
	// this is called from a manual that's currently selected in tree
	wxTreeItemId selectedManual = m_organTreeCtrl->GetSelection();
	wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(selectedManual);
	wxTreeItemId newDivisional = m_organTreeCtrl->AppendItem(divisionalChild, wxT("New Divisional"));
	m_organTreeCtrl->ExpandAllChildren(selectedManual);
	m_organTreeCtrl->SelectItem(newDivisional);
}

void GOODFFrame::OnAddNewWindchestgroup(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfWindchestgroups() < 999) {
		Windchestgroup newWindchest;
		m_organ->addWindchestgroup(newWindchest);

		m_organTreeCtrl->SelectItem(m_organTreeCtrl->AppendItem(tree_windchestgrps, newWindchest.getName()));
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 windchests!"), wxT("Too many windchests"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewSwitch(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfSwitches() < 999) {
		GoSwitch newSwitch;
		m_organ->addSwitch(newSwitch);

		m_organTreeCtrl->SelectItem(m_organTreeCtrl->AppendItem(tree_switches, newSwitch.getName()));
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 switches!"), wxT("Too many switches"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewRank(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfRanks() < 999) {
		Rank newRank;
		m_organ->addRank(newRank);
		m_organTreeCtrl->SelectItem(m_organTreeCtrl->AppendItem(tree_ranks, newRank.getName()));
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 ranks!"), wxT("Too many ranks"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnNewOrgan(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog dlg(this, wxT("Are you really sure you want to create a completely new organ?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (dlg.ShowModal() == wxID_YES) {
		if (m_organ) {
			delete m_organ;
			m_organ = NULL;
		}
		m_organ = new Organ();
		m_organHasBeenSaved = false;

		removeAllItemsFromTree();
		SetupOrganMainPanel();
		m_organTreeCtrl->SelectItem(tree_organ);

		m_organPanel->setCurrentOrgan(m_organ);
		m_organPanel->setOdfPath(wxEmptyString);
		m_organPanel->setOdfName(wxEmptyString);
	}
}

void GOODFFrame::OnAddNewManual(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfManuals() < 16) {
		Manual m;
		m_organ->addManual(m);
		wxTreeItemId thisManual = m_organTreeCtrl->AppendItem(tree_manuals, wxT("New Manual"));

		// create the subitems for Stops, Couplers and Divisionals
		m_organTreeCtrl->AppendItem(thisManual, wxT("Stops"));
		m_organTreeCtrl->AppendItem(thisManual, wxT("Couplers"));
		m_organTreeCtrl->AppendItem(thisManual, wxT("Divisionals"));

		m_organTreeCtrl->SelectItem(thisManual);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 16 manuals!"), wxT("Too many manuals"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewDivisionalCoupler(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfOrganDivisionalCouplers() < 8) {
		DivisionalCoupler divCplr;
		m_organ->addDivisionalCoupler(divCplr);
		wxTreeItemId thisDivCplr = m_organTreeCtrl->AppendItem(tree_divisionalCouplers, wxT("New Divisional Coupler"));

		m_organTreeCtrl->SelectItem(thisDivCplr);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 8 divisional couplers!"), wxT("Too many divisional couplers"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewGeneral(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfGenerals() < 99) {
		General gen;
		m_organ->addGeneral(gen);
		wxTreeItemId thisGeneral = m_organTreeCtrl->AppendItem(tree_generals, wxT("New General"));

		m_organTreeCtrl->SelectItem(thisGeneral);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 99 generals!"), wxT("Too many generals"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewReversiblePiston(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfReversiblePistons() < 32) {
		ReversiblePiston p;
		m_organ->addReversiblePiston(p);
		wxTreeItemId thisPiston = m_organTreeCtrl->AppendItem(tree_reversiblePistons, wxT("New Reversible Piston"));

		m_organTreeCtrl->SelectItem(thisPiston);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 32 reversible pistons!"), wxT("Too many reversible pistons"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewPanel(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->getNumberOfPanels() < 1000) {
		GoPanel p;
		m_organ->addPanel(p);
		wxTreeItemId thisPanel = m_organTreeCtrl->AppendItem(tree_panels, wxT("New Panel"));

		// create the subitems for Displaymetrics, Images and GUIElements
		m_organTreeCtrl->AppendItem(thisPanel, wxT("Displaymetrics"));
		m_organTreeCtrl->AppendItem(thisPanel, wxT("Images"));
		m_organTreeCtrl->AppendItem(thisPanel, wxT("GUI Elements"));

		m_organTreeCtrl->SelectItem(thisPanel);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 1000 panels!"), wxT("Too many panels"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::SetupOrganMainPanel() {
	// Main panel is created by Organ itself but we need to add it to the tree
	wxTreeItemId mainPanel = m_organTreeCtrl->AppendItem(tree_panels, m_organ->getOrganPanelAt(0)->getName());

	// create the subitems for Displaymetrics, Images and GUIElements
	m_organTreeCtrl->AppendItem(mainPanel, wxT("Displaymetrics"));
	m_organTreeCtrl->AppendItem(mainPanel, wxT("Images"));
	m_organTreeCtrl->AppendItem(mainPanel, wxT("GUI Elements"));
}

void GOODFFrame::AddImageItemToTree() {
	// This is called from a panel that's currently selected in the tree
	wxTreeItemId selectedPanel = m_organTreeCtrl->GetSelection();
	wxTreeItemId guiElementChild = m_organTreeCtrl->GetLastChild(selectedPanel);
	wxTreeItemId imageChild = m_organTreeCtrl->GetPrevSibling(guiElementChild);
	wxTreeItemId newImage = m_organTreeCtrl->AppendItem(imageChild, wxT("New Image"));
	m_organTreeCtrl->ExpandAllChildren(selectedPanel);
	m_organTreeCtrl->SelectItem(newImage);
}

void GOODFFrame::AddGuiElementToTree(wxString title) {
	// This is called from a panel that's currently selected in the tree
	wxTreeItemId selectedPanel = m_organTreeCtrl->GetSelection();
	wxTreeItemId guiElementChild = m_organTreeCtrl->GetLastChild(selectedPanel);
	wxTreeItemId newGuiElement = m_organTreeCtrl->AppendItem(guiElementChild, title);
	m_organTreeCtrl->ExpandAllChildren(selectedPanel);
	m_organTreeCtrl->SelectItem(newGuiElement);
	m_organTreeCtrl->SelectItem(selectedPanel);
}

void GOODFFrame::RebuildPanelGuiElementsInTree(int panelIndex) {
	int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
	wxTreeItemIdValue cookie;
	wxTreeItemId panelId;
	for (int i = 0; i < numChildrens; i++) {

		if (i == 0)
			panelId = m_organTreeCtrl->GetFirstChild(tree_panels, cookie);
		else
			panelId = m_organTreeCtrl->GetNextChild(tree_panels, cookie);
		if (i == panelIndex)
			break;
	}
	wxTreeItemId guiElements = m_organTreeCtrl->GetLastChild(panelId);
	m_organTreeCtrl->DeleteChildren(guiElements);
	int nbrElements = m_organ->getOrganPanelAt(panelIndex)->getNumberOfGuiElements();
	for (int i = 0; i < nbrElements; i++) {
		m_organTreeCtrl->AppendItem(guiElements, m_organ->getOrganPanelAt(panelIndex)->getGuiElementAt(i)->getDisplayName());
	}
}

void GOODFFrame::removeAllItemsFromTree() {
	m_organTreeCtrl->DeleteChildren(tree_manuals);
	m_organTreeCtrl->DeleteChildren(tree_windchestgrps);
	m_organTreeCtrl->DeleteChildren(tree_enclosures);
	m_organTreeCtrl->DeleteChildren(tree_tremulants);
	m_organTreeCtrl->DeleteChildren(tree_ranks);
	m_organTreeCtrl->DeleteChildren(tree_switches);
	m_organTreeCtrl->DeleteChildren(tree_reversiblePistons);
	m_organTreeCtrl->DeleteChildren(tree_divisionalCouplers);
	m_organTreeCtrl->DeleteChildren(tree_generals);
	m_organTreeCtrl->DeleteChildren(tree_panels);
}
