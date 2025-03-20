/* 
 * GOODFFrame.cpp is a part of GOODF software
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
#include "CmbDialog.h"
#include "DefaultPathsDialog.h"
#include "StopRankImportDialog.h"
#include <vector>
#include <algorithm>

// Event table
BEGIN_EVENT_TABLE(GOODFFrame, wxFrame)
	EVT_MENU(wxID_ABOUT, GOODFFrame::OnAbout)
	EVT_MENU(wxID_HELP, GOODFFrame::OnHelp)
	EVT_MENU(wxID_EXIT, GOODFFrame::OnQuit)
	EVT_MENU(ID_WRITE_ODF, GOODFFrame::OnWriteODF)
	EVT_MENU(ID_NEW_ORGAN, GOODFFrame::OnNewOrgan)
	EVT_MENU(ID_READ_ORGAN, GOODFFrame::OnReadOrganFile)
	EVT_MENU(ID_IMPORT_VOICING_DATA, GOODFFrame::OnImportCMB)
	EVT_MENU(ID_IMPORT_STOP_RANK, GOODFFrame::OnImportStopRank)
	EVT_MENU(ID_GLOBAL_SHOW_TOOLTIPS_OPTION, GOODFFrame::OnEnableTooltipsMenu)
	EVT_MENU(ID_GLOBAL_PARSE_LEGACY_XFADES_OPTION, GOODFFrame::OnImportLegacyXfadesMenu)
	EVT_MENU(ID_CLEAR_HISTORY, GOODFFrame::OnClearHistory)
	EVT_MENU(ID_DEFAULT_PATHS_MENU, GOODFFrame::OnDefaultPathMenuChoice)
	EVT_MENU_RANGE(wxID_FILE1, wxID_FILE9, GOODFFrame::OnRecentFileMenuChoice)
	EVT_TREE_SEL_CHANGED(ID_ORGAN_TREE, GOODFFrame::OnOrganTreeSelectionChanged)
	EVT_TREE_ITEM_RIGHT_CLICK(ID_ORGAN_TREE, GOODFFrame::OnOrganTreeRightClicked)
	EVT_TREE_BEGIN_DRAG(ID_ORGAN_TREE, GOODFFrame::OnOrganTreeLeftDrag)
	EVT_TREE_END_DRAG(ID_ORGAN_TREE, GOODFFrame::OnOrganTreeDragCompleted)
	EVT_MOTION(GOODFFrame::OnOrganTreeMouseMotion)
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
	EVT_SIZE(GOODFFrame::OnSizeChange)
	EVT_CLOSE(GOODFFrame::OnClose)
END_EVENT_TABLE()

GOODFFrame::GOODFFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
	// Start with an empty organ
	m_organ = new Organ();
	m_organHasBeenSaved = false;
	m_enableTooltips = false;
	m_config = new wxFileConfig(wxT("GoOdf"));
	m_defaultOrganDirectory = wxEmptyString;
	m_defaultCmbDirectory = wxEmptyString;
	m_logWindow = new wxLogWindow(this, wxT("Log messages"), false, false);
	wxLog::SetActiveTarget(m_logWindow);

	// Create a file menu
	m_fileMenu = new wxMenu();

	// Add file menu items
	m_fileMenu->Append(ID_NEW_ORGAN, wxT("&New Organ\tCtrl+N"), wxT("Create a new organ"));
	m_fileMenu->Append(ID_READ_ORGAN, wxT("Open file\tCtrl+O"), wxT("Open existing .organ file"));
	m_recentMenu = new wxMenu();
	m_fileMenu->AppendSubMenu(m_recentMenu, wxT("Recent Files"));
	m_fileMenu->AppendSeparator();
	m_fileMenu->Append(ID_WRITE_ODF, wxT("Write ODF\tCtrl+S"), wxT("Write/Save the .organ file"));
	m_fileMenu->Append(wxID_EXIT, wxT("&Exit\tCtrl+Q"), wxT("Quit this program"));

	// Create a tools menu
	m_toolsMenu = new wxMenu();

	// Add tools menu items
	m_toolsMenu->Append(ID_IMPORT_VOICING_DATA, wxT("Import .cmb\tCtrl+I"), wxT("Import voicing data from a .cmb (settings) file"));
	m_toolsMenu->Append(ID_IMPORT_STOP_RANK, wxT("Import Stops/Ranks\tCtrl+R"), wxT("Import stops/ranks from another (working) .organ file"));
	m_toolsMenu->AppendCheckItem(ID_GLOBAL_SHOW_TOOLTIPS_OPTION, wxT("Enable Tooltips"), wxT("Enable tooltips for certain controls"));
	m_toolsMenu->Check(ID_GLOBAL_SHOW_TOOLTIPS_OPTION, false);
	m_toolsMenu->Append(ID_GLOBAL_PARSE_LEGACY_XFADES_OPTION, wxT("Import Legacy X-fades"), wxT("Make extra attacks/releases inherit LoopCrossfadeLength & ReleaseCrossfadeLength values like pre GO v3.14.0"));
	m_toolsMenu->Append(ID_CLEAR_HISTORY, wxT("Clear File History"), wxT("Remove all the entries in the recent file history"));
	m_toolsMenu->Append(ID_DEFAULT_PATHS_MENU, wxT("Default paths\tCtrl+P"), wxT("Set the default paths used by the application here"));

	m_toolsMenu->Enable(ID_GLOBAL_PARSE_LEGACY_XFADES_OPTION, false);

	// Create a help menu
	m_helpMenu = new wxMenu();

	// Add help menu items
	m_helpMenu->Append(wxID_ABOUT, wxT("&About..."), wxT("Show about dialog"));
	m_helpMenu->Append(wxID_HELP, wxT("&GoOdf Help \tF1"), wxT("Show application help"));

	// Create a menu bar and append the menus to it
	m_menuBar = new wxMenuBar();
	m_menuBar->Append(m_fileMenu, wxT("&File"));
	m_menuBar->Append(m_toolsMenu, wxT("&Tools"));
	m_menuBar->Append(m_helpMenu, wxT("&Help"));

	// Attach menu bar to frame
	SetMenuBar(m_menuBar);

	m_recentlyUsed = new wxFileHistory();
	m_recentlyUsed->UseMenu(m_recentMenu);
	m_recentlyUsed->AddFilesToMenu(m_recentMenu);
	m_recentlyUsed->Load(*m_config);

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
	addManSizer->Add(addManBtn, 0, wxALL, 5);
	addManual->SetSizer(addManSizer);
	addManual->Hide();

	addWindchest = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addWindSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *windchestRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *addWindText = new wxStaticText(addWindchest, wxID_STATIC, wxT("Add"));
	windchestRow->Add(addWindText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	nbrWindchestSpin = new wxSpinCtrl(
		addWindchest,
		ID_ADD_WINDCHEST_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		999,
		1
	);
	windchestRow->Add(nbrWindchestSpin, 0, wxALL, 5);
	addWindchestBtn = new wxButton(addWindchest, ID_ADD_WINDCHEST_BTN, wxT("new windchest(s)"));
	windchestRow->Add(addWindchestBtn, 0, wxALL, 5);
	addWindSizer->Add(windchestRow, 0);
	addWindchest->SetSizer(addWindSizer);
	addWindchest->Hide();

	addEnclosure = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addEnclosureSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *enclosureRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *addEnclText = new wxStaticText(addEnclosure, wxID_STATIC, wxT("Add"));
	enclosureRow->Add(addEnclText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	nbrEnclosureSpin = new wxSpinCtrl(
		addEnclosure,
		ID_ADD_ENCLOSURE_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		999,
		1
	);
	enclosureRow->Add(nbrEnclosureSpin, 0, wxALL, 5);
	addEnclosureBtn = new wxButton(addEnclosure, ID_ADD_ENCLOSURE_BTN, wxT("new enclosure(s)"));
	enclosureRow->Add(addEnclosureBtn, 0, wxALL, 5);
	addEnclosureSizer->Add(enclosureRow, 0);
	addEnclosure->SetSizer(addEnclosureSizer);
	addEnclosure->Hide();

	addTremulant = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addTremSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *tremulantRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *addTremText = new wxStaticText(addTremulant, wxID_STATIC, wxT("Add"));
	tremulantRow->Add(addTremText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	nbrTremulantSpin = new wxSpinCtrl(
		addTremulant,
		ID_ADD_TREMULANT_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		999,
		1
	);
	tremulantRow->Add(nbrTremulantSpin, 0, wxALL, 5);
	addTremulantBtn = new wxButton(addTremulant, ID_ADD_TREMULANT_BTN, wxT("new tremulant(s)"));
	tremulantRow->Add(addTremulantBtn, 0, wxALL, 5);
	addTremSizer->Add(tremulantRow, 0);
	addTremulant->SetSizer(addTremSizer);
	addTremulant->Hide();

	addRank = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addRankSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rankRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *addRankText = new wxStaticText(addRank, wxID_STATIC, wxT("Add"));
	rankRow->Add(addRankText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	nbrRankSpin = new wxSpinCtrl(
		addRank,
		ID_ADD_RANK_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		999,
		1
	);
	rankRow->Add(nbrRankSpin, 0, wxALL, 5);
	addRankBtn = new wxButton(addRank, ID_ADD_RANK_BTN, wxT("new rank(s)"));
	rankRow->Add(addRankBtn, 0, wxALL, 5);
	addRankSizer->Add(rankRow, 0);
	addRank->SetSizer(addRankSizer);
	addRank->Hide();

	addSwitch = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addSwitchSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *switchRow = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *addSwitchText = new wxStaticText(addSwitch, wxID_STATIC, wxT("Add"));
	switchRow->Add(addSwitchText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	nbrSwitchSpin = new wxSpinCtrl(
		addSwitch,
		ID_ADD_SWITCH_SPIN,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxSP_ARROW_KEYS,
		1,
		999,
		1
	);
	switchRow->Add(nbrSwitchSpin, 0, wxALL, 5);
	addSwitchBtn = new wxButton(addSwitch, ID_ADD_SWITCH_BTN, wxT("new switch(es)"));
	switchRow->Add(addSwitchBtn, 0, wxALL, 5);
	addSwitchSizer->Add(switchRow, 0);
	addSwitch->SetSizer(addSwitchSizer);
	addSwitch->Hide();

	addReversiblePiston = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addPistonSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addPistonBtn = new wxButton(addReversiblePiston, ID_ADD_REVERSIBLE_PISTON_BTN, wxT("Create new reversible piston"));
	addPistonSizer->Add(addPistonBtn, 0, wxALL, 5);
	addReversiblePiston->SetSizer(addPistonSizer);
	addReversiblePiston->Hide();

	addDivisionalCoupler = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addDivCplrSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addDivCplrBtn = new wxButton(addDivisionalCoupler, ID_ADD_DIV_CPLR_BTN, wxT("Create new divisional coupler"));
	addDivCplrSizer->Add(addDivCplrBtn, 0, wxALL, 5);
	addDivisionalCoupler->SetSizer(addDivCplrSizer);
	addDivisionalCoupler->Hide();

	addGeneral = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addGeneralSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addGeneralBtn = new wxButton(addGeneral, ID_ADD_GENERAL_BTN, wxT("Create new general"));
	addGeneralSizer->Add(addGeneralBtn, 0, wxALL, 5);
	addGeneral->SetSizer(addGeneralSizer);
	addGeneral->Hide();

	addPanel = new wxPanel(m_Splitter, wxID_ANY);
	wxBoxSizer *addPanelSizer = new wxBoxSizer(wxVERTICAL);
	wxButton *addPanelBtn = new wxButton(addPanel, ID_ADD_PANEL_BTN, wxT("Create new panel"));
	addPanelSizer->Add(addPanelBtn, 0, wxALL, 5);
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
	m_draggedItem = (wxTreeItemId) 0l;
	SetupOrganMainPanel();

	// Expand all the nodes
	m_organTreeCtrl->ExpandAll();

	leftSplitSizer->Add(m_organTreeCtrl, 1, wxEXPAND);
	leftSplitPanel->SetSizer(leftSplitSizer);

	m_organPanel = new OrganPanel(m_organ, m_Splitter);

	leftSplitPanel->SetMinSize(leftSplitPanel->GetSizer()->GetMinSize());

	wxSize largestMinSize = m_organPanel->GetSizer()->GetMinSize();
	if (largestMinSize.GetWidth() < m_enclosurePanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_enclosurePanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_tremulantPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_tremulantPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_windchestPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_windchestPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_switchPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_switchPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_rankPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_rankPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_stopPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_stopPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_manualPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_manualPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_couplerPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_couplerPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_divisionalPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_divisionalPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_divCplrPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_divCplrPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_generalPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_generalPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_reversiblePistonPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_reversiblePistonPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_panelPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_panelPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_dispMetricsPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_dispMetricsPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_imagePanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_imagePanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_guiButtonPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_guiButtonPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_guiEnclosurePanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_guiEnclosurePanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_guiLabelPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_guiLabelPanel->GetSizer()->GetMinSize().GetWidth());
	if (largestMinSize.GetWidth() < m_guiManualPanel->GetSizer()->GetMinSize().GetWidth())
		largestMinSize.SetWidth(m_guiManualPanel->GetSizer()->GetMinSize().GetWidth());

	if (largestMinSize.GetHeight() < m_enclosurePanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_enclosurePanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_tremulantPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_tremulantPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_windchestPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_windchestPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_switchPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_switchPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_rankPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_rankPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_stopPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_stopPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_manualPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_manualPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_couplerPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_couplerPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_divisionalPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_divisionalPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_divCplrPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_divCplrPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_generalPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_generalPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_reversiblePistonPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_reversiblePistonPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_panelPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_panelPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_dispMetricsPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_dispMetricsPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_imagePanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_imagePanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_guiButtonPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_guiButtonPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_guiEnclosurePanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_guiEnclosurePanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_guiLabelPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_guiLabelPanel->GetSizer()->GetMinSize().GetHeight());
	if (largestMinSize.GetHeight() < m_guiManualPanel->GetSizer()->GetMinSize().GetHeight())
		largestMinSize.SetHeight(m_guiManualPanel->GetSizer()->GetMinSize().GetHeight());

	m_organPanel->SetMinSize(largestMinSize);
	m_enclosurePanel->SetMinSize(largestMinSize);
	m_tremulantPanel->SetMinSize(largestMinSize);
	m_windchestPanel->SetMinSize(largestMinSize);
	m_switchPanel->SetMinSize(largestMinSize);
	m_rankPanel->SetMinSize(largestMinSize);
	m_stopPanel->SetMinSize(largestMinSize);
	m_manualPanel->SetMinSize(largestMinSize);
	m_couplerPanel->SetMinSize(largestMinSize);
	m_divisionalPanel->SetMinSize(largestMinSize);
	m_divCplrPanel->SetMinSize(largestMinSize);
	m_generalPanel->SetMinSize(largestMinSize);
	m_reversiblePistonPanel->SetMinSize(largestMinSize);
	m_panelPanel->SetMinSize(largestMinSize);
	m_dispMetricsPanel->SetMinSize(largestMinSize);
	m_imagePanel->SetMinSize(largestMinSize);
	m_guiButtonPanel->SetMinSize(largestMinSize);
	m_guiEnclosurePanel->SetMinSize(largestMinSize);
	m_guiLabelPanel->SetMinSize(largestMinSize);
	m_guiManualPanel->SetMinSize(largestMinSize);

	m_Splitter->SplitVertically(leftSplitPanel, m_organPanel);

	// Use top sizer for layout and size window accordingly and prevent it
	// from being resized to smaller size
	SetSizerAndFit(topSizer);

	// Additionally set a minimum width of 1024 x 800
	wxSize calculated = GetSize();
	if (calculated.GetWidth() < 1024 || calculated.GetHeight() < 800)
		SetMinSize(wxSize(1024, 800));
	else
		SetMinSize(calculated);
	m_Splitter->SetSashPosition(205);

	m_xPosition = -1;
	m_yPosition = -1;
	m_frameWidth = GetSize().GetWidth();
	m_frameHeight = GetSize().GetHeight();
	m_frameMaximized = false;

	// Connect mouse motion events for child windows
	m_Splitter->Connect(wxEVT_MOTION, wxMouseEventHandler(GOODFFrame::OnOrganTreeMouseMotion), NULL, this);
	leftSplitPanel->Connect(wxEVT_MOTION, wxMouseEventHandler(GOODFFrame::OnOrganTreeMouseMotion), NULL, this);
	m_organTreeCtrl->Connect(wxEVT_MOTION, wxMouseEventHandler(GOODFFrame::OnOrganTreeMouseMotion), NULL, this);

	// Read settings file and apply if found
	bool b;
	if (m_config->Read(wxT("General/EnableTooltips"), &b)) {
		m_enableTooltips = b;
		if (m_enableTooltips)
			m_toolsMenu->Check(ID_GLOBAL_SHOW_TOOLTIPS_OPTION, true);
		else
			m_toolsMenu->Check(ID_GLOBAL_SHOW_TOOLTIPS_OPTION, false);

		wxCommandEvent evt(wxEVT_MENU, ID_GLOBAL_SHOW_TOOLTIPS_OPTION);
		wxPostEvent(this, evt);
	}

	int readInt;
	if (m_config->Read(wxT("General/FrameXPosition"), &readInt))
		m_xPosition = readInt;

	if (m_config->Read(wxT("General/FrameYPosition"), &readInt))
		m_yPosition = readInt;

	if (m_config->Read(wxT("General/FrameWidth"), &readInt))
		m_frameWidth = readInt;

	if (m_config->Read(wxT("General/FrameHeight"), &readInt))
		m_frameHeight = readInt;

	if (m_config->Read(wxT("General/FrameMaximized"), &b))
		m_frameMaximized = b;

	if (m_frameMaximized)
		Maximize();
	else
		SetSize(m_xPosition, m_yPosition, m_frameWidth, m_frameHeight);

	if (m_config->Read(wxT("General/SashPosition"), &readInt))
		m_Splitter->SetSashPosition(readInt);

	wxString pathString = wxEmptyString;
	m_config->Read(wxT("General/DefaultOrganDirectory"), &pathString);
	if (wxDirExists(pathString))
		m_defaultOrganDirectory = pathString;
	m_config->Read(wxT("General/DefaultCmbDirectory"), &pathString);
	if (wxDirExists(pathString))
		m_defaultCmbDirectory = pathString;

	wxString atkFolder = wxEmptyString;
	m_config->Read(wxT("Rank/AttackSubFolderOption"), &atkFolder);
	bool oneAttack = false;
	m_config->Read(wxT("Rank/OneAttackOnlyOption"), &oneAttack);
	bool loadRelease = true;
	m_config->Read(wxT("Rank/LoadReleaseOption"), &loadRelease);
	wxString releaseFolder = wxT("rel");
	m_config->Read(wxT("Rank/ReleaseSubFolderOption"), &releaseFolder);
	bool extractTime = true;
	m_config->Read(wxT("Rank/ExtractTimeOption"), &extractTime);
	wxString tremFolder = wxT("trem");
	m_config->Read(wxT("Rank/TremulantSubFolderOption"), &tremFolder);
	bool loadPipesTremOff = false;
	m_config->Read(wxT("Rank/LoadPipesAsTremulantOff"), &loadPipesTremOff);

	m_rankPanel->SetPipeReadingOptions(
		atkFolder,
		oneAttack,
		loadRelease,
		releaseFolder,
		extractTime,
		tremFolder,
		loadPipesTremOff
	);
	m_stopPanel->GetInternalRankPanel()->SetPipeReadingOptions(
		atkFolder,
		oneAttack,
		loadRelease,
		releaseFolder,
		extractTime,
		tremFolder,
		loadPipesTremOff
	);

}

GOODFFrame::~GOODFFrame() {
	if (m_organ)
		delete m_organ;
	wxLog::SetActiveTarget(nullptr);
	delete m_logWindow;
}

void GOODFFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
	wxAboutDialogInfo info;
	info.SetName(wxT("GoOdf"));
	info.SetVersion(wxT(GOODF_VERSION));
	info.SetDescription(wxT("A software for creating and editing GrandOrgue ODFs"));
	info.SetCopyright(wxT("Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)\nReleased under GNU GPLv3 licence"));
	info.SetWebSite(wxT("https://github.com/GrandOrgue/GoOdf"));

	wxAboutBox(info);
}

void GOODFFrame::OnHelp(wxCommandEvent& WXUNUSED(event)) {
	::wxGetApp().m_helpController->DisplayContents();
}

void GOODFFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
	// Destroy the frame
	Close();
}

void GOODFFrame::OnClose(wxCloseEvent& event) {
	if (event.CanVeto() && m_organ->isModified()) {
		// Ask if user wants to save/write the organ file
		wxMessageDialog dlg(this, wxT("ODF file is modified. Do you want to save/write it?"), wxT("ODF file is modified"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (dlg.ShowModal() == wxID_YES) {
			// Try triggering save/write by posting an event
			wxCommandEvent evt(wxEVT_MENU, ID_WRITE_ODF);
			wxPostEvent(this, evt);

			if (m_organ->isModified()) {
				// This means that the save failed, ask if the user wants to abort the quitting to fix the issue
				wxMessageDialog confirmDlg(this, wxT("ODF file couldn't be saved/written! Do you want to return to fix it before quitting?"), wxT("ODF file couldn't be saved"), wxYES_NO|wxCENTRE|wxICON_ERROR);
				if (confirmDlg.ShowModal() == wxID_YES) {
					event.Veto();
					return;
				}
			}
		}
	}

	// Write config file (settings)
	m_config->Write(wxT("General/EnableTooltips"), m_enableTooltips);
	UpdateFrameSizeAndPos();
	m_config->Write(wxT("General/FrameXPosition"), m_xPosition);
	m_config->Write(wxT("General/FrameYPosition"), m_yPosition);
	m_config->Write(wxT("General/FrameWidth"), m_frameWidth);
	m_config->Write(wxT("General/FrameHeight"), m_frameHeight);
	m_config->Write(wxT("General/FrameMaximized"), m_frameMaximized);
	m_config->Write(wxT("General/SashPosition"), m_Splitter->GetSashPosition());
	m_config->Write(wxT("General/DefaultOrganDirectory"), m_defaultOrganDirectory);
	m_config->Write(wxT("General/DefaultCmbDirectory"), m_defaultCmbDirectory);
	m_config->Write(wxT("Rank/AttackSubFolderOption"), m_rankPanel->GetAttackFolderOption());
	m_config->Write(wxT("Rank/OneAttackOnlyOption"), m_rankPanel->GetOneAttackOption());
	m_config->Write(wxT("Rank/LoadReleaseOption"), m_rankPanel->GetLoadReleaseOption());
	m_config->Write(wxT("Rank/ReleaseSubFolderOption"), m_rankPanel->GetReleaseFolderOption());
	m_config->Write(wxT("Rank/ExtractTimeOption"), m_rankPanel->GetExtractTimeOption());
	m_config->Write(wxT("Rank/TremulantSubFolderOption"), m_rankPanel->GetTremFolderOption());
	m_config->Write(wxT("Rank/LoadPipesAsTremulantOff"), m_rankPanel->GetLoadPipesAsTremOffOption());
	m_recentlyUsed->Save(*m_config);
	m_config->Flush();
	delete m_config;
	delete m_recentlyUsed;

	// Destroy the frame
	Destroy();
}

void GOODFFrame::OnWriteODF(wxCommandEvent& WXUNUSED(event)) {
	FixAnyIllegalEntries();
	if (m_organPanel->getOdfPath().IsEmpty() || m_organPanel->getOdfName().IsEmpty()) {
		wxMessageDialog incomplete(this, wxT("Both path (location) and name for ODF must be set!"), wxT("Cannot write ODF"), wxOK|wxCENTRE);
		incomplete.ShowModal();
		return;
	}
	wxString fullFileName = m_organPanel->getOdfPath() + wxFILE_SEP_PATH + m_organPanel->getOdfName() + wxT(".organ");
	if (m_recentlyUsed->GetCount() &&  !fullFileName.IsSameAs(m_recentlyUsed->GetHistoryFile(0))) {
		m_organHasBeenSaved = false;
	}
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
	m_organ->setModified(false);
	UpdateFrameTitle();
	m_recentlyUsed->AddFileToHistory(fullFileName);
}

void GOODFFrame::OnReadOrganFile(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->isModified()) {
		wxMessageDialog dlg(this, wxT("All current organ data will be lost! Do you want to proceed?"), wxT("Are you sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
		if (dlg.ShowModal() != wxID_YES) {
			return;
		}
	}

	wxString organFilePath;
	wxString defaultPath = m_defaultOrganDirectory;
	if (defaultPath == wxEmptyString)
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();

	wxFileDialog fileDialog(
		this,
		wxT("Select .organ file to open"),
		defaultPath,
		"",
		"GrandOrgue ODF files (*.organ)|*.organ;*.ORGAN",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() != wxID_OK) {
		return;
	}

	organFilePath = fileDialog.GetPath();
	DoOpenOrgan(organFilePath);
}

void GOODFFrame::DoOpenOrgan(wxString filePath) {
	m_organTreeCtrl->SetFocusedItem(tree_organ);
	if (m_organ) {
		delete m_organ;
		m_organ = NULL;
	}
	m_organ = new Organ();
	removeAllItemsFromTree();
	m_organHasBeenSaved = false;
	RecreateLogWindow();

	OrganFileParser parser(filePath, m_organ);
	if (parser.isOrganReady()) {
		m_recentlyUsed->AddFileToHistory(filePath);
		wxFileName f_name = wxFileName(filePath);
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
		UpdateFrameTitle();
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
		SetTitle(::wxGetApp().m_fullAppName);
	}
	m_organ->organElementHasChanged(true);
	m_organTreeCtrl->SelectItem(tree_organ);
	SetImportXfadeMenuItemState();
	this->Raise();
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
				int existingWindchests = m_organ->getNumberOfWindchestgroups();
				if (existingWindchests < 999) {
					int maxValue = 999 - existingWindchests;
					nbrWindchestSpin->SetRange(1, maxValue);
					nbrWindchestSpin->SetValue(1);
					nbrWindchestSpin->Enable();
					addWindchestBtn->Enable();
				} else {
					nbrWindchestSpin->SetRange(0, 1);
					nbrWindchestSpin->SetValue(0);
					nbrWindchestSpin->Disable();
					addWindchestBtn->Disable();
				}
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
				int existingEnclosures = m_organ->getNumberOfEnclosures();
				if (existingEnclosures < 999) {
					int maxValue = 999 - existingEnclosures;
					nbrEnclosureSpin->SetRange(1, maxValue);
					nbrEnclosureSpin->SetValue(1);
					nbrEnclosureSpin->Enable();
					addEnclosureBtn->Enable();
				} else {
					nbrEnclosureSpin->SetRange(0, 1);
					nbrEnclosureSpin->SetValue(0);
					nbrEnclosureSpin->Disable();
					addEnclosureBtn->Disable();
				}
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
				int existingRanks = m_organ->getNumberOfRanks();
				if (existingRanks < 999) {
					int maxValue = 999 - existingRanks;
					nbrRankSpin->SetRange(1, maxValue);
					nbrRankSpin->SetValue(1);
					nbrRankSpin->Enable();
					addRankBtn->Enable();
				} else {
					nbrRankSpin->SetRange(0, 1);
					nbrRankSpin->SetValue(0);
					nbrRankSpin->Disable();
					addRankBtn->Disable();
				}
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
				int existingSwitches = m_organ->getNumberOfSwitches();
				if (existingSwitches < 999) {
					int maxValue = 999 - existingSwitches;
					nbrSwitchSpin->SetRange(1, maxValue);
					nbrSwitchSpin->SetValue(1);
					nbrSwitchSpin->Enable();
					addSwitchBtn->Enable();
				} else {
					nbrSwitchSpin->SetRange(0, 1);
					nbrSwitchSpin->SetValue(0);
					nbrSwitchSpin->Disable();
					addSwitchBtn->Disable();
				}
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

	if (parentId == tree_panels) {
		// A panel was right clicked, should it be duplicated?
		if (m_organ->getNumberOfPanels() < 1000) {
			wxMessageDialog dlg(this, wxT("Do you want to duplicate the right-clicked panel?"), wxT("Duplicate panel?"), wxYES_NO|wxCENTRE);
			if (dlg.ShowModal() == wxID_YES) {
				int selectedPanelIndex = 0;
				int numPanels = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
				wxTreeItemIdValue cookie;
				for (int i = 0; i < numPanels; i++) {
					wxTreeItemId currentId;
					if (i == 0)
						currentId = m_organTreeCtrl->GetFirstChild(tree_panels, cookie);
					else
						currentId = m_organTreeCtrl->GetNextChild(tree_panels, cookie);
					if (clickedId == currentId)
						selectedPanelIndex = i;
				}

				GoPanel p = *(m_organ->getOrganPanelAt(selectedPanelIndex));
				m_organ->addPanel(p);
				wxTreeItemId thisPanel = m_organTreeCtrl->AppendItem(tree_panels, p.getName());
				// create the subitems for Displaymetrics, Images and GUIElements
				m_organTreeCtrl->AppendItem(thisPanel, wxT("Displaymetrics"));
				wxTreeItemId panelImages = m_organTreeCtrl->AppendItem(thisPanel, wxT("Images"));
				for (unsigned i = 0; i < p.getNumberOfImages(); i++) {
					wxString displayName = p.getImageAt(i)->getImageNameOnly();
					if (displayName == wxEmptyString)
						displayName = wxT("New Image");
					m_organTreeCtrl->AppendItem(panelImages, displayName);
				}
				wxTreeItemId panelGuiElements = m_organTreeCtrl->AppendItem(thisPanel, wxT("GUI Elements"));
				for (int i = 0; i < p.getNumberOfGuiElements(); i++) {
					m_organTreeCtrl->AppendItem(panelGuiElements, p.getGuiElementAt(i)->getDisplayName());
				}
				m_organ->setModified(true);
				m_organTreeCtrl->SelectItem(thisPanel);

			} else {
				event.Skip();
				return;
			}
		} else {
			wxMessageDialog msg(this, wxT("Organ cannot have more than 1000 panels!"), wxT("Too many panels"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
			msg.ShowModal();
			event.Skip();
			return;
		}
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

void GOODFFrame::OnOrganTreeLeftDrag(wxTreeEvent& event) {
	// Only allow dragging if item is a switch, manual, windchestgroup, rank, stop, gui element
	wxTreeItemId sourceItem = event.GetItem();
	if (sourceItem == tree_organ ||
		sourceItem == tree_manuals ||
		sourceItem == tree_windchestgrps ||
		sourceItem == tree_enclosures ||
		sourceItem == tree_tremulants ||
		sourceItem == tree_ranks ||
		sourceItem == tree_switches ||
		sourceItem == tree_reversiblePistons ||
		sourceItem == tree_divisionalCouplers ||
		sourceItem == tree_generals ||
		sourceItem == tree_panels
	) {
		return;
	}
	wxTreeItemId sourceParent = m_organTreeCtrl->GetItemParent(sourceItem);
	wxTreeItemId sourceGrandParent = m_organTreeCtrl->GetItemParent(sourceParent);
	wxTreeItemIdValue cookie;
	wxTreeItemId mainPanel = m_organTreeCtrl->GetFirstChild(tree_panels, cookie);
	if ((sourceParent == tree_switches && m_organTreeCtrl->GetChildrenCount(tree_switches, false) > 1) ||
		(sourceParent == tree_manuals && m_organTreeCtrl->GetChildrenCount(tree_manuals, false) > 1) ||
		(sourceParent == tree_windchestgrps && m_organTreeCtrl->GetChildrenCount(tree_windchestgrps, false) > 1) ||
		(sourceParent == tree_ranks && m_organTreeCtrl->GetChildrenCount(tree_ranks, false) > 1) ||
		(m_organTreeCtrl->GetItemText(sourceParent) == wxT("Stops") && m_organTreeCtrl->GetItemText(m_organTreeCtrl->GetNextSibling(sourceParent)) == wxT("Couplers")) ||
		(m_organTreeCtrl->GetItemParent(sourceGrandParent) == tree_panels && m_organTreeCtrl->GetItemText(sourceParent) == wxT("GUI Elements") && m_organTreeCtrl->GetChildrenCount(sourceParent, false) > 1) ||
		(sourceParent == tree_panels && sourceItem != mainPanel)
	) {
		m_draggedItem = sourceItem;
		event.Allow();
	}
}

void GOODFFrame::OnOrganTreeDragCompleted(wxTreeEvent& event) {
	wxTreeItemId srcItem = m_draggedItem;
	wxTreeItemId dstItem = event.GetItem();
	wxTreeItemId srcParent = m_organTreeCtrl->GetItemParent(srcItem);
	wxTreeItemId dstParent = m_organTreeCtrl->GetItemParent(dstItem);
	wxTreeItemId srcGrandParent = m_organTreeCtrl->GetItemParent(srcParent);
	wxTreeItemId dstGrandParent = m_organTreeCtrl->GetItemParent(dstParent);
	m_draggedItem = (wxTreeItemId) 0l;

	if (dstParent == tree_switches && srcParent == tree_switches) {
		// we drop it after target switch but first check if the move really should happen
		if (m_organTreeCtrl->GetNextSibling(dstItem) == srcItem) {
			return;
		}
		int targetIndex = 0;
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_switches, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_switches, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_switches, cookie);
			if (dstItem == currentId) {
				// this is the target we're looking for
				targetIndex = i;
			}
			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		if (targetIndex < numChildrens) {
			targetIndex += 1;
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_switches, dstItem, m_organTreeCtrl->GetItemText(srcItem));
		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveSwitch(sourceIndex, targetIndex);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (dstItem == tree_switches && srcParent == tree_switches) {
		// we make it first child
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_switches, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_switches, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_switches, cookie);

			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_switches, 0, m_organTreeCtrl->GetItemText(srcItem));
		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveSwitch(sourceIndex, 0);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (dstParent == tree_manuals && srcParent == tree_manuals) {
		// we drop it after target manual but first check if the move really should happen
		if (m_organTreeCtrl->GetNextSibling(dstItem) == srcItem) {
			return;
		}
		int targetIndex = 0;
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_manuals, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_manuals, cookie);
			if (dstItem == currentId) {
				// this is the target we're looking for
				targetIndex = i;
			}
			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		if (targetIndex < numChildrens) {
			targetIndex += 1;
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_manuals, dstItem, m_organTreeCtrl->GetItemText(srcItem));
		// create the subitems for Stops, Couplers and Divisionals
		wxTreeItemId newStops = m_organTreeCtrl->AppendItem(newPos, wxT("Stops"));
		wxTreeItemId newCouplers = m_organTreeCtrl->AppendItem(newPos, wxT("Couplers"));
		wxTreeItemId newDivisionals = m_organTreeCtrl->AppendItem(newPos, wxT("Divisionals"));
		// copy items from source to new pos
		wxTreeItemId srcDivisionals = m_organTreeCtrl->GetLastChild(srcItem);
		wxTreeItemId srcCouplers = m_organTreeCtrl->GetPrevSibling(srcDivisionals);
		wxTreeItemId srcStops = m_organTreeCtrl->GetPrevSibling(srcCouplers);
		int numDivisionals = m_organTreeCtrl->GetChildrenCount(srcDivisionals, false);
		// wxTreeItemIdValue cookie;
		for (int i = 0; i < numDivisionals; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcDivisionals, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcDivisionals, cookie);

			m_organTreeCtrl->AppendItem(newDivisionals, m_organTreeCtrl->GetItemText(currentId));
		}
		int numCouplers = m_organTreeCtrl->GetChildrenCount(srcCouplers, false);
		// wxTreeItemIdValue cookie;
		for (int i = 0; i < numCouplers; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcCouplers, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcCouplers, cookie);

			m_organTreeCtrl->AppendItem(newCouplers, m_organTreeCtrl->GetItemText(currentId));
		}

		int numStops = m_organTreeCtrl->GetChildrenCount(srcStops, false);
		// wxTreeItemIdValue cookie;
		for (int i = 0; i < numStops; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcStops, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcStops, cookie);

			m_organTreeCtrl->AppendItem(newStops, m_organTreeCtrl->GetItemText(currentId));
		}
		m_organTreeCtrl->DeleteChildren(srcItem);
		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveManual(sourceIndex, targetIndex);
		m_organTreeCtrl->SelectItem(newPos);
	} else if (dstItem == tree_manuals && srcParent == tree_manuals) {
		// we make it first child
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_manuals, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_manuals, cookie);

			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_manuals, 0, m_organTreeCtrl->GetItemText(srcItem));
		// create the subitems for Stops, Couplers and Divisionals
		wxTreeItemId newStops = m_organTreeCtrl->AppendItem(newPos, wxT("Stops"));
		wxTreeItemId newCouplers = m_organTreeCtrl->AppendItem(newPos, wxT("Couplers"));
		wxTreeItemId newDivisionals = m_organTreeCtrl->AppendItem(newPos, wxT("Divisionals"));
		// copy items from source to new pos
		wxTreeItemId srcDivisionals = m_organTreeCtrl->GetLastChild(srcItem);
		wxTreeItemId srcCouplers = m_organTreeCtrl->GetPrevSibling(srcDivisionals);
		wxTreeItemId srcStops = m_organTreeCtrl->GetPrevSibling(srcCouplers);
		int numDivisionals = m_organTreeCtrl->GetChildrenCount(srcDivisionals, false);
		// wxTreeItemIdValue cookie;
		for (int i = 0; i < numDivisionals; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcDivisionals, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcDivisionals, cookie);

			m_organTreeCtrl->AppendItem(newDivisionals, m_organTreeCtrl->GetItemText(currentId));
		}
		int numCouplers = m_organTreeCtrl->GetChildrenCount(srcCouplers, false);
		// wxTreeItemIdValue cookie;
		for (int i = 0; i < numCouplers; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcCouplers, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcCouplers, cookie);

			m_organTreeCtrl->AppendItem(newCouplers, m_organTreeCtrl->GetItemText(currentId));
		}

		int numStops = m_organTreeCtrl->GetChildrenCount(srcStops, false);
		// wxTreeItemIdValue cookie;
		for (int i = 0; i < numStops; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcStops, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcStops, cookie);

			m_organTreeCtrl->AppendItem(newStops, m_organTreeCtrl->GetItemText(currentId));
		}
		m_organTreeCtrl->DeleteChildren(srcItem);
		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveManual(sourceIndex, 0);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (dstParent == tree_windchestgrps && srcParent == tree_windchestgrps) {
		// we drop it after target windchestgroup but first check if the move really should happen
		if (m_organTreeCtrl->GetNextSibling(dstItem) == srcItem) {
			return;
		}
		int targetIndex = 0;
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_windchestgrps, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_windchestgrps, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_windchestgrps, cookie);
			if (dstItem == currentId) {
				// this is the target we're looking for
				targetIndex = i;
			}
			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		if (targetIndex < numChildrens) {
			targetIndex += 1;
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_windchestgrps, dstItem, m_organTreeCtrl->GetItemText(srcItem));
		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveWindchestgroup(sourceIndex, targetIndex);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (dstItem == tree_windchestgrps && srcParent == tree_windchestgrps) {
		// we make it first child
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_windchestgrps, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_windchestgrps, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_windchestgrps, cookie);

			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_windchestgrps, 0, m_organTreeCtrl->GetItemText(srcItem));

		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveWindchestgroup(sourceIndex, 0);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (dstParent == tree_ranks && srcParent == tree_ranks) {
		// we drop it after target rank but first check if the move really should happen
		if (m_organTreeCtrl->GetNextSibling(dstItem) == srcItem) {
			return;
		}
		int targetIndex = 0;
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_ranks, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_ranks, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_ranks, cookie);
			if (dstItem == currentId) {
				// this is the target we're looking for
				targetIndex = i;
			}
			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		if (targetIndex < numChildrens) {
			targetIndex += 1;
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_ranks, dstItem, m_organTreeCtrl->GetItemText(srcItem));
		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveRank(sourceIndex, targetIndex);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (dstItem == tree_ranks && srcParent == tree_ranks) {
		// we make it first child
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_ranks, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_ranks, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_ranks, cookie);

			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_ranks, 0, m_organTreeCtrl->GetItemText(srcItem));

		m_organTreeCtrl->Delete(srcItem);
		m_organ->moveRank(sourceIndex, 0);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (m_organTreeCtrl->GetItemText(srcParent) == wxT("Stops") && m_organTreeCtrl->GetItemText(m_organTreeCtrl->GetNextSibling(srcParent)) == wxT("Couplers")) {
		// The source item is certainly a stop, allowed drop destinations are another stop, the stop main category and a manual
		std::vector<wxTreeItemId> manualsInTree;
		int nbrManuals = m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < nbrManuals; i++) {
			wxTreeItemId currentMan;
			if (i == 0)
				currentMan = m_organTreeCtrl->GetFirstChild(tree_manuals, cookie);
			else
				currentMan = m_organTreeCtrl->GetNextChild(tree_manuals, cookie);

			manualsInTree.push_back(currentMan);
		}

		// Get what index the source stop had to begin with
		int srcStopIdx = -1;
		int nbrSrcStops = m_organTreeCtrl->GetChildrenCount(srcParent, false);
		wxTreeItemIdValue srcCookie;
		for (int i = 0; i < nbrSrcStops; i++) {
			wxTreeItemId currentStop;
			if (i == 0)
				currentStop = m_organTreeCtrl->GetFirstChild(srcParent, srcCookie);
			else
				currentStop = m_organTreeCtrl->GetNextChild(srcParent, srcCookie);

			if (currentStop == srcItem) {
				srcStopIdx = i;
				break;
			}
		}

		if (m_organTreeCtrl->GetItemText(dstParent) == wxT("Stops") && m_organTreeCtrl->GetItemText(m_organTreeCtrl->GetNextSibling(dstParent)) == wxT("Couplers")) {
			// The destination item is another stop, should the move proceed?
			if (m_organTreeCtrl->GetNextSibling(dstItem) == srcItem) {
				return;
			}

			// Get what manual each of the stops belongs to
			int srcManualIdx = -1;
			int dstManualIdx = -1;
			for (unsigned i = 0; i < manualsInTree.size(); i++) {
				if (m_organTreeCtrl->GetItemText(manualsInTree[i]) == m_organTreeCtrl->GetItemText(srcGrandParent))
					srcManualIdx = i;
				if (m_organTreeCtrl->GetItemText(manualsInTree[i]) == m_organTreeCtrl->GetItemText(dstGrandParent))
					dstManualIdx = i;
			}

			// Get what index the destination stop has
			int dstStopIdx = -1;
			int nbrDstStops = m_organTreeCtrl->GetChildrenCount(dstParent, false);
			wxTreeItemIdValue dstCookie;
			for (int i = 0; i < nbrDstStops; i++) {
				wxTreeItemId currentStop;
				if (i == 0)
					currentStop = m_organTreeCtrl->GetFirstChild(dstParent, dstCookie);
				else
					currentStop = m_organTreeCtrl->GetNextChild(dstParent, dstCookie);

				if (currentStop == dstItem) {
					dstStopIdx = i;
					break;
				}
			}
			if (dstStopIdx < nbrDstStops) {
				dstStopIdx += 1;
			}

			// Now we should know the source and destination index of respective manual and stop
			if (m_organ->moveStop(srcManualIdx, srcStopIdx, dstManualIdx, dstStopIdx)) {
				// The move succeeded in the organ so now it's just to take care of it in tree
				wxTreeItemId newPos = m_organTreeCtrl->InsertItem(dstParent, dstItem, m_organTreeCtrl->GetItemText(srcItem));
				m_organTreeCtrl->Delete(srcItem);
				m_organTreeCtrl->SelectItem(newPos);
				m_organ->setModified(true);
			}
		} else if ((std::find(manualsInTree.begin(), manualsInTree.end(), dstItem) != manualsInTree.end() && dstParent == tree_manuals) ||
			(std::find(manualsInTree.begin(), manualsInTree.end(), dstParent) != manualsInTree.end() && m_organTreeCtrl->GetItemText(dstItem) == wxT("Stops"))
		) {
			// The destination is a manual or a stops category, should the move proceed?
			if (srcGrandParent == dstItem && m_organTreeCtrl->GetChildrenCount(srcParent, false) < 2) {
				return;
			} else if (srcParent == dstItem && m_organTreeCtrl->GetChildrenCount(srcParent, false) < 2) {
				return;
			}
			// Get what manual the source stop belongs to and what manual is the destination
			int srcManualIdx = -1;
			int dstManualIdx = -1;
			for (unsigned i = 0; i < manualsInTree.size(); i++) {
				if (m_organTreeCtrl->GetItemText(manualsInTree[i]) == m_organTreeCtrl->GetItemText(srcGrandParent))
					srcManualIdx = i;
				if ((m_organTreeCtrl->GetItemText(manualsInTree[i]) == m_organTreeCtrl->GetItemText(dstItem)) ||
					(m_organTreeCtrl->GetItemText(manualsInTree[i]) == m_organTreeCtrl->GetItemText(dstParent))
				)
					dstManualIdx = i;
			}

			// Target we will be the first (0th) element
			int dstStopIdx = 0;
			// Now we should know the source and destination index of respective manual and stop
			if (m_organ->moveStop(srcManualIdx, srcStopIdx, dstManualIdx, dstStopIdx)) {
				// The move succeeded in the organ so now it's just to take care of it in tree
				wxTreeItemId newPos;
				if (m_organTreeCtrl->GetItemText(dstItem) == wxT("Stops")) {
					newPos = m_organTreeCtrl->InsertItem(dstItem, 0, m_organTreeCtrl->GetItemText(srcItem));
				} else {
					wxTreeItemIdValue stopsCookie;
					wxTreeItemId stopCategory = m_organTreeCtrl->GetFirstChild(dstItem, stopsCookie);
					newPos = m_organTreeCtrl->InsertItem(stopCategory, 0, m_organTreeCtrl->GetItemText(srcItem));
				}
				m_organTreeCtrl->Delete(srcItem);
				m_organTreeCtrl->SelectItem(newPos);
				m_organ->setModified(true);
			}
		}
	} else if (m_organTreeCtrl->GetItemText(srcParent) == wxT("GUI Elements") && m_organTreeCtrl->GetItemParent(srcGrandParent) == tree_panels && m_organTreeCtrl->GetItemText(dstParent) == wxT("GUI Elements") && dstGrandParent == srcGrandParent) {
		// the target is another gui element in the same panel so just test if the move really should happen
		if (m_organTreeCtrl->GetNextSibling(dstItem) == srcItem) {
			return;
		}

		int targetIndex = 0;
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(srcParent, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcParent, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcParent, cookie);
			if (dstItem == currentId) {
				// this is the target we're looking for
				targetIndex = i;
			}
			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		if (targetIndex < numChildrens) {
			targetIndex += 1;
		}

		// Okay, so now we need to get what panel we're actually dealing with
		int panelIndex = -1;
		int numPanels = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
		wxTreeItemIdValue panelCookie;
		for (int i = 0; i < numPanels; i++) {
			wxTreeItemId currentPanel;
			if (i == 0)
				currentPanel = m_organTreeCtrl->GetFirstChild(tree_panels, panelCookie);
			else
				currentPanel = m_organTreeCtrl->GetNextChild(tree_panels, panelCookie);

			if (currentPanel == srcGrandParent) {
				panelIndex = i;
				break;
			}
		}

		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(srcParent, dstItem, m_organTreeCtrl->GetItemText(srcItem));
		m_organTreeCtrl->Delete(srcItem);
		m_organ->getOrganPanelAt(panelIndex)->moveGuiElement(sourceIndex, targetIndex);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (m_organTreeCtrl->GetItemText(srcParent) == wxT("GUI Elements") && m_organTreeCtrl->GetItemParent(srcGrandParent) == tree_panels && dstItem == srcParent) {
		// We should make it the first child of the panel in question, first get what child it is
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(srcParent, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(srcParent, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(srcParent, cookie);

			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}

		// Okay, so now we need to get what panel we're actually dealing with
		int panelIndex = -1;
		int numPanels = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
		wxTreeItemIdValue panelCookie;
		for (int i = 0; i < numPanels; i++) {
			wxTreeItemId currentPanel;
			if (i == 0)
				currentPanel = m_organTreeCtrl->GetFirstChild(tree_panels, panelCookie);
			else
				currentPanel = m_organTreeCtrl->GetNextChild(tree_panels, panelCookie);

			if (currentPanel == srcGrandParent) {
				panelIndex = i;
				break;
			}
		}

		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(srcParent, 0, m_organTreeCtrl->GetItemText(srcItem));
		m_organTreeCtrl->Delete(srcItem);
		m_organ->getOrganPanelAt(panelIndex)->moveGuiElement(sourceIndex, 0);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	} else if (dstParent == tree_panels && srcParent == tree_panels) {
		// Check if the move is invalid and if so just return
		if (m_organTreeCtrl->GetNextSibling(dstItem) == srcItem) {
			return;
		}
		// Otherwise drop the panel after the destination panel as main panel must always be first
		int targetIndex = 0;
		int sourceIndex = 0;
		int numChildrens = m_organTreeCtrl->GetChildrenCount(tree_panels, false);
		wxTreeItemIdValue cookie;
		for (int i = 0; i < numChildrens; i++) {
			wxTreeItemId currentId;
			if (i == 0)
				currentId = m_organTreeCtrl->GetFirstChild(tree_panels, cookie);
			else
				currentId = m_organTreeCtrl->GetNextChild(tree_panels, cookie);
			if (dstItem == currentId) {
				// this is the target we're looking for
				targetIndex = i;
			}
			if (srcItem == currentId) {
				sourceIndex = i;
			}
		}
		if (targetIndex < numChildrens) {
			targetIndex += 1;
		}
		GoPanel *thePanel = m_organ->getOrganPanelAt(sourceIndex);
		wxTreeItemId newPos = m_organTreeCtrl->InsertItem(tree_panels, dstItem, m_organTreeCtrl->GetItemText(srcItem));
		// create the subitems for Display metrics, Images and GUI Elements
		m_organTreeCtrl->AppendItem(newPos, wxT("Displaymetrics"));
		wxTreeItemId newImages = m_organTreeCtrl->AppendItem(newPos, wxT("Images"));
		for (unsigned j = 0; j < thePanel->getNumberOfImages(); j++) {
			m_organTreeCtrl->AppendItem(newImages, thePanel->getImageAt(j)->getImageNameOnly());
		}
		wxTreeItemId newGuiElements = m_organTreeCtrl->AppendItem(newPos, wxT("GUI Elements"));
		for (unsigned j = 0; j < (unsigned) thePanel->getNumberOfGuiElements(); j++) {
			m_organTreeCtrl->AppendItem(newGuiElements, thePanel->getGuiElementAt(j)->getDisplayName());
		}
		m_organTreeCtrl->Delete(srcItem);
		m_organ->movePanel(sourceIndex, targetIndex);
		m_organTreeCtrl->SelectItem(newPos);
		m_organ->setModified(true);
	}

}

void GOODFFrame::OnOrganTreeMouseMotion(wxMouseEvent& event) {
	if (m_enableTooltips && !event.Dragging()) {
		int flagsToUse = wxTREE_HITTEST_ONITEMINDENT|wxTREE_HITTEST_ONITEMLABEL;
		wxPoint cursor = event.GetPosition();
		wxTreeItemId itemUnderMouse = m_organTreeCtrl->HitTest(cursor, flagsToUse);

		if (!itemUnderMouse.IsOk()) {
			m_organTreeCtrl->SetToolTip(wxEmptyString);
			event.Skip();
			return;
		}

		if (itemUnderMouse == tree_organ) {
			m_organTreeCtrl->SetToolTip(wxT("Set basic attributes for the whole organ here."));
		} else if (itemUnderMouse == tree_manuals) {
			m_organTreeCtrl->SetToolTip(wxT("Add manuals here. Best practise is to add them in rising order starting with the Pedal and then the next lowest manual and so on to the highest available. Add eventual 'trick' manuals last."));
		} else if (itemUnderMouse == tree_windchestgrps) {
			m_organTreeCtrl->SetToolTip(wxT("Windchestgroups represent the windchest(s) upon which the pipes are placed."));
		} else if (itemUnderMouse == tree_enclosures) {
			m_organTreeCtrl->SetToolTip(wxT("Enclosures represent boxes (usually around the windchests carrying the ranks/pipes) with shutters that can be opened or closed to affect the perceived sound level."));
		} else if (itemUnderMouse == tree_tremulants) {
			m_organTreeCtrl->SetToolTip(wxT("Tremulants are appliances that disturb the wind flow to the pipes (makes the tone tremble)."));
		} else if (itemUnderMouse == tree_ranks) {
			m_organTreeCtrl->SetToolTip(wxT("A rank represents a row of pipes. The main reason to create separate ranks (instead of using the internal rank of a stop) is if the organ would benefit from it. Common use cases are custom compound stops, easy re-using (borrowing) for many stops etc."));
		} else if (itemUnderMouse == tree_switches) {
			m_organTreeCtrl->SetToolTip(wxT("Switches are drawstop objects without any additional attributes. They can be used, for example, to trigger stop action noises or model features that cannot be done with just one stop."));
		} else if (itemUnderMouse == tree_reversiblePistons) {
			m_organTreeCtrl->SetToolTip(wxT("Reversible pistons are a legacy feature of GrandOrgue to allow triggering other elements from the push button. Since the new panel format was added this can easily be accomplished by just adding the same object as another GUI Element and style it appropriately!"));
		} else if (itemUnderMouse == tree_divisionalCouplers) {
			m_organTreeCtrl->SetToolTip(wxT("Divisional couplers couples divisionals between different divisions (manuals)."));
		} else if (itemUnderMouse == tree_generals) {
			m_organTreeCtrl->SetToolTip(wxT("Add old style ODF based Generals here if you really have a special reason for it, but generally it's better to use the built in setter generals available for insertion on any panel."));
		} else if (itemUnderMouse == tree_panels) {
			m_organTreeCtrl->SetToolTip(wxT("Left click to select and add panels for GUI display. Main panel [Panel000] is always present (automatically added) as the first panel. Any other panels can be re-arranged with drag & drop."));
		} else {
			wxTreeItemId parentOfItem = m_organTreeCtrl->GetItemParent(itemUnderMouse);
			if (!parentOfItem.IsOk()) {
				m_organTreeCtrl->SetToolTip(wxEmptyString);
				event.Skip();
				return;
			}

			if (parentOfItem == tree_manuals ||
				parentOfItem == tree_windchestgrps ||
				parentOfItem == tree_switches ||
				parentOfItem == tree_ranks ||
				(parentOfItem == tree_panels && !m_organTreeCtrl->GetItemText(itemUnderMouse).IsSameAs(wxT("Main Panel")))) {
				m_organTreeCtrl->SetToolTip(wxT("It is possible to re-arrange this type of items with drag & drop. The dragged item will be placed after the target."));
				return;
			}

			if (parentOfItem == tree_panels && m_organTreeCtrl->GetItemText(itemUnderMouse).IsSameAs(wxT("Main Panel"))) {
				m_organTreeCtrl->SetToolTip(wxT("The main panel is the panel displayed by default for the organ."));
				return;
			}

			wxTreeItemId grandParentOfItem = m_organTreeCtrl->GetItemParent(parentOfItem);
			if (!grandParentOfItem.IsOk()) {
				m_organTreeCtrl->SetToolTip(wxEmptyString);
				event.Skip();
				return;
			}

			if (grandParentOfItem == tree_panels && m_organTreeCtrl->GetItemText(itemUnderMouse).IsSameAs(wxT("Displaymetrics"))) {
				m_organTreeCtrl->SetToolTip(wxT("Displaymetrics contain the basic layout settings for a panel."));
			}

			wxTreeItemId grandGrandParent = m_organTreeCtrl->GetItemParent(grandParentOfItem);
			if (!grandGrandParent.IsOk()) {
				m_organTreeCtrl->SetToolTip(wxEmptyString);
				event.Skip();
				return;
			}

			if (grandGrandParent == tree_manuals && m_organTreeCtrl->GetItemText(parentOfItem).IsSameAs(wxT("Stops"))) {
				m_organTreeCtrl->SetToolTip(wxT("It is possible to re-arrange stops with drag & drop. The dragged stop will be placed after the target."));
			} else if (grandGrandParent == tree_panels && m_organTreeCtrl->GetItemText(parentOfItem).IsSameAs(wxT("GUI Elements"))) {
				m_organTreeCtrl->SetToolTip(wxT("The GUI Elements can be re-arranged with drag & drop. It is also possible to right click on an element to bring up a dialog that allows copying of properties from one element to others."));
			}
		}
	} else {
		m_organTreeCtrl->SetToolTip(wxEmptyString);
		event.Skip();
	}
}

void GOODFFrame::OnAddNewEnclosure(wxCommandEvent& WXUNUSED(event)) {
	int nbrToAdd = nbrEnclosureSpin->GetValue();
	if (m_organ->getNumberOfEnclosures() + nbrToAdd < 1000) {
		wxTreeItemId firstAdded;
		for (int i = 0; i < nbrToAdd; i++) {
			Enclosure newEnclosure;
			m_organ->addEnclosure(newEnclosure);
			if (i == 0)
				firstAdded = m_organTreeCtrl->AppendItem(tree_enclosures, newEnclosure.getName());
			else
				m_organTreeCtrl->AppendItem(tree_enclosures, newEnclosure.getName());
		}
		m_organ->setModified(true);
		m_organTreeCtrl->SelectItem(firstAdded);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 enclosures!"), wxT("Too many enclosures"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewTremulant(wxCommandEvent& WXUNUSED(event)) {
	int nbrToAdd = nbrTremulantSpin->GetValue();
	if (m_organ->getNumberOfTremulants() + nbrToAdd < 1000) {
		wxTreeItemId firstAdded;
		for (int i = 0; i < nbrToAdd; i++) {
			Tremulant newTremulant;
			m_organ->addTremulant(newTremulant);
			if (i == 0)
				firstAdded = m_organTreeCtrl->AppendItem(tree_tremulants, newTremulant.getName());
			else
				m_organTreeCtrl->AppendItem(tree_tremulants, newTremulant.getName());
		}
		m_organ->setModified(true);
		m_organTreeCtrl->SelectItem(firstAdded);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 tremulants!"), wxT("Too many tremulants"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
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
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeEnclosureAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_tremulants) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeTremulantAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_windchestgrps) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeWindchestgroupAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_switches) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeSwitchAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_reversiblePistons) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeReversiblePistonAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_divisionalCouplers) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeDivisionalCouplerAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_generals) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeGeneralAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_ranks) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeRankAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
			SetImportXfadeMenuItemState();
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
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = theManual;
			}
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
			SetImportXfadeMenuItemState();
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
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = theManual;
			}
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
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
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = theManual;
			}
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_manuals) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removeManualAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}

		if (parentId == tree_panels) {
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = parentId;
			}
			m_organ->removePanelAt(selectedIndex);
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
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
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = thePanel;
			}
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
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
			wxTreeItemId nextToSelect;
			if (numChildrens > 1) {
				// there's more than one child to the parent so we select the other lower (or higher if it was the first)
				if (selectedIndex > 0) {
					nextToSelect = m_organTreeCtrl->GetPrevSibling(selected);
				} else {
					nextToSelect = m_organTreeCtrl->GetNextSibling(selected);
				}
			} else {
				nextToSelect = thePanel;
			}
			m_organTreeCtrl->Delete(selected);
			m_organTreeCtrl->SelectItem(nextToSelect);
		}
		m_organ->setModified(true);
	}
}

void GOODFFrame::AddStopItemToTree() {
	// this is called from a manual that's currently selected in tree
	wxTreeItemId selectedManual = m_organTreeCtrl->GetSelection();
	wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(selectedManual);
	wxTreeItemId couplerChild = m_organTreeCtrl->GetPrevSibling(divisionalChild);
	wxTreeItemId stopChild = m_organTreeCtrl->GetPrevSibling(couplerChild);
	m_organTreeCtrl->AppendItem(stopChild, wxT("New Stop"));
	m_organ->setModified(true);
	SetImportXfadeMenuItemState();
}

void GOODFFrame::AddStopItemForManual(wxString stopName, unsigned manIndex) {
	if (manIndex < m_organ->getNumberOfManuals() && manIndex < m_organTreeCtrl->GetChildrenCount(tree_manuals)) {
		unsigned numChildrens = m_organTreeCtrl->GetChildrenCount(tree_manuals);
		wxTreeItemIdValue cookie;
		wxTreeItemId theManual;
		for (unsigned i = 0; i < numChildrens; i++) {
			if (i == 0)
				theManual = m_organTreeCtrl->GetFirstChild(tree_manuals, cookie);
			else
				theManual = m_organTreeCtrl->GetNextChild(tree_manuals, cookie);
			if (i == manIndex)
				break;
		}

		wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(theManual);
		wxTreeItemId couplerChild = m_organTreeCtrl->GetPrevSibling(divisionalChild);
		wxTreeItemId stopChild = m_organTreeCtrl->GetPrevSibling(couplerChild);
		m_organTreeCtrl->AppendItem(stopChild, stopName);
		m_organ->setModified(true);
		SetImportXfadeMenuItemState();
	}
}

void GOODFFrame::AddRankItemToTree(wxString rankName) {
	m_organTreeCtrl->AppendItem(tree_ranks, rankName);
	m_organ->setModified(true);
	SetImportXfadeMenuItemState();
}

void GOODFFrame::SelectStopItemInTree(int nbrAdded) {
	wxTreeItemId selectedManual = m_organTreeCtrl->GetSelection();
	wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(selectedManual);
	wxTreeItemId couplerChild = m_organTreeCtrl->GetPrevSibling(divisionalChild);
	wxTreeItemId stopChild = m_organTreeCtrl->GetPrevSibling(couplerChild);
	wxTreeItemId lastStopInManual = m_organTreeCtrl->GetLastChild(stopChild);
	wxTreeItemId stopToSelect;
	if (nbrAdded > 1) {
		wxTreeItemId currentItem = lastStopInManual;
		for (int i = 1; i < nbrAdded; i++) {
			stopToSelect = m_organTreeCtrl->GetPrevSibling(currentItem);
			currentItem = stopToSelect;
		}
	} else {
		stopToSelect = lastStopInManual;
	}
	m_organTreeCtrl->ExpandAllChildren(selectedManual);
	m_organTreeCtrl->SelectItem(stopToSelect);
}

void GOODFFrame::AddCouplerItemToTree() {
	// this is called from a manual that's currently selected in tree
	wxTreeItemId selectedManual = m_organTreeCtrl->GetSelection();
	wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(selectedManual);
	wxTreeItemId couplerChild = m_organTreeCtrl->GetPrevSibling(divisionalChild);
	wxTreeItemId newCoupler = m_organTreeCtrl->AppendItem(couplerChild, wxT("New Coupler"));
	m_organTreeCtrl->ExpandAllChildren(selectedManual);
	m_organTreeCtrl->SelectItem(newCoupler);
	m_organ->setModified(true);
}

void GOODFFrame::AddDivisionalItemToTree() {
	// this is called from a manual that's currently selected in tree
	wxTreeItemId selectedManual = m_organTreeCtrl->GetSelection();
	wxTreeItemId divisionalChild = m_organTreeCtrl->GetLastChild(selectedManual);
	wxTreeItemId newDivisional = m_organTreeCtrl->AppendItem(divisionalChild, wxT("New Divisional"));
	m_organTreeCtrl->ExpandAllChildren(selectedManual);
	m_organTreeCtrl->SelectItem(newDivisional);
	m_organ->setModified(true);
}

void GOODFFrame::OnAddNewWindchestgroup(wxCommandEvent& WXUNUSED(event)) {
	int nbrToAdd = nbrWindchestSpin->GetValue();
	if (m_organ->getNumberOfWindchestgroups() + nbrToAdd < 1000) {
		wxTreeItemId firstAdded;
		for (int i = 0; i < nbrToAdd; i++) {
			Windchestgroup newWindchest;
			newWindchest.setIsPercussive(m_organ->getIsPercussive());
			newWindchest.setHasIndependentRelease(m_organ->getHasIndependentRelease());
			m_organ->addWindchestgroup(newWindchest);
			if (i == 0)
				firstAdded = m_organTreeCtrl->AppendItem(tree_windchestgrps, newWindchest.getName());
			else
				m_organTreeCtrl->AppendItem(tree_windchestgrps, newWindchest.getName());
		}
		m_organ->setModified(true);
		m_organTreeCtrl->SelectItem(firstAdded);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 windchests!"), wxT("Too many windchests"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewSwitch(wxCommandEvent& WXUNUSED(event)) {
	int nbrToAdd = nbrSwitchSpin->GetValue();
	if (m_organ->getNumberOfSwitches() + nbrToAdd < 1000) {
		wxTreeItemId firstAdded;
		for (int i = 0; i < nbrToAdd; i++) {
			GoSwitch newSwitch;
			m_organ->addSwitch(newSwitch);
			if (i == 0)
				firstAdded = m_organTreeCtrl->AppendItem(tree_switches, newSwitch.getName());
			else
				m_organTreeCtrl->AppendItem(tree_switches, newSwitch.getName());
		}
		m_organ->setModified(true);
		m_organTreeCtrl->SelectItem(firstAdded);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 switches!"), wxT("Too many switches"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnAddNewRank(wxCommandEvent& WXUNUSED(event)) {
	int nbrToAdd = nbrRankSpin->GetValue();
	if (m_organ->getNumberOfRanks() + nbrToAdd < 1000) {
		wxTreeItemId firstAdded;
		for (int i = 0; i < nbrToAdd; i++) {
			Rank newRank;
			newRank.setPercussive(m_organ->getIsPercussive());
			newRank.setIndependentRelease(m_organ->getHasIndependentRelease());
			m_organ->addRank(newRank);
			if (i == 0)
				firstAdded = m_organTreeCtrl->AppendItem(tree_ranks, newRank.getName());
			else
				m_organTreeCtrl->AppendItem(tree_ranks, newRank.getName());
		}
		SetImportXfadeMenuItemState();
		m_organ->setModified(true);
		m_organTreeCtrl->SelectItem(firstAdded);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 999 ranks!"), wxT("Too many ranks"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnNewOrgan(wxCommandEvent& WXUNUSED(event)) {
	if (m_organ->isModified()) {
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
			SetTitle(::wxGetApp().m_fullAppName);
			RecreateLogWindow();
		}
	} else {
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
		SetTitle(::wxGetApp().m_fullAppName);
		RecreateLogWindow();
	}
	SetImportXfadeMenuItemState();
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

		m_organ->setModified(true);
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

		m_organ->setModified(true);
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

		m_organ->setModified(true);
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

		m_organ->setModified(true);
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

		m_organ->setModified(true);
		m_organTreeCtrl->SelectItem(thisPanel);
	} else {
		wxMessageDialog msg(this, wxT("Organ cannot have more than 1000 panels!"), wxT("Too many panels"), wxOK|wxCENTRE|wxICON_EXCLAMATION);
		msg.ShowModal();
	}
}

void GOODFFrame::OnImportCMB(wxCommandEvent& WXUNUSED(event)) {
	CmbDialog importCmb(this);
	if (importCmb.ShowModal() == wxID_OK) {
		CMB_ORGAN *imported = importCmb.GetCmbOrgan();

		if (m_organ->getNumberOfRanks() != imported->cmbRanks.size() ||
			m_organ->getNumberOfStops() != imported->cmbStops.size() ||
			m_organ->getNumberOfWindchestgroups() != imported->cmbWindchests.size()
		) {
			wxMessageDialog dlg(this, wxT("The number of windchests/ranks/stops in the imported cmb doesn't match current organ! Do you want to import selected data to what is possible anyway?"), wxT("Are you really sure?"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
			if (dlg.ShowModal() != wxID_YES) {
				return;
			}
		}

		if (importCmb.GetImportAmplitude())
			m_organ->setAmplitudeLevel(imported->attributes.amplitude);
		if (importCmb.GetImportGain())
			m_organ->setGain(imported->attributes.gain);
		if (importCmb.GetImportPitchTuning()) {
			float pitchT = m_organ->getPitchTuning() + imported->attributes.pitchTuning;
			if (pitchT >= -1800 && pitchT <= 1800) {
				m_organ->setPitchTuning(pitchT);
			} else {
				wxMessageDialog pTerr(this, wxT("The PitchTuning value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
				if (pTerr.ShowModal() == wxID_YES) {
					if (pitchT > 1800)
						m_organ->setPitchTuning(1800);
					else if (pitchT < -1800)
						m_organ->setPitchTuning(-1800);
				}
			}
		}
		if (importCmb.GetImportPitchCorrection()) {
			float pitchC = m_organ->getPitchCorrection() + imported->attributes.pitchCorrection;
			if (pitchC >= -1800 && pitchC <= 1800) {
				m_organ->setPitchCorrection(pitchC);
			} else {
				wxMessageDialog pCerr(this, wxT("The PitchCorrection value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
				if (pCerr.ShowModal() == wxID_YES) {
					if (pitchC > 1800)
						m_organ->setPitchCorrection(1800);
					else if (pitchC < -1800)
						m_organ->setPitchCorrection(-1800);
				}
			}
		}
		if (importCmb.GetImportTrackerDelay()) {
			m_organ->setTrackerDelay(imported->attributes.trackerDelay);
		}

		unsigned windchestIdx = 0;
		for (CMB_ELEMENT &w : imported->cmbWindchests) {
			if (m_organ->getNumberOfWindchestgroups() > windchestIdx) {
				Windchestgroup *windchest = m_organ->getOrganWindchestgroupAt(windchestIdx);

				if (importCmb.GetImportAmplitude())
					windchest->setAmplitudeLevel(w.amplitude);
				if (importCmb.GetImportGain())
					windchest->setGain(w.gain);
				if (importCmb.GetImportPitchTuning()) {
					float pitchT = windchest->getPitchTuning() + w.pitchTuning;
					if (pitchT >= -1800 && pitchT <= 1800) {
						windchest->setPitchTuning(pitchT);
					} else {
						wxMessageDialog pTerr(this, wxT("The PitchTuning value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
						if (pTerr.ShowModal() == wxID_YES) {
							if (pitchT > 1800)
								windchest->setPitchTuning(1800);
							else if (pitchT < -1800)
								windchest->setPitchTuning(-1800);
						}
					}
				}
				if (importCmb.GetImportPitchCorrection()) {
					float pitchC = windchest->getPitchCorrection() + w.pitchCorrection;
					if (pitchC >= -1800 && pitchC <= 1800) {
						windchest->setPitchCorrection(pitchC);
					} else {
						wxMessageDialog pCerr(this, wxT("The PitchCorrection value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
						if (pCerr.ShowModal() == wxID_YES) {
							if (pitchC > 1800)
								windchest->setPitchCorrection(1800);
							else if (pitchC < -1800)
								windchest->setPitchCorrection(-1800);
						}
					}
				}
				if (importCmb.GetImportTrackerDelay()) {
					windchest->setTrackerDelay(w.trackerDelay);
				}
			}
			windchestIdx++;
		}

		unsigned stopIdx = 0;
		for (CMB_ELEMENT_WITH_PIPES &s : imported->cmbStops) {
			if (m_organ->getNumberOfStops() > stopIdx) {
				Stop *stop = m_organ->getOrganStopAt(stopIdx);
				if (!stop->isUsingInternalRank())
					continue;

				if (importCmb.GetImportAmplitude())
					stop->getInternalRank()->setAmplitudeLevel(s.attributes.amplitude);
				if (importCmb.GetImportGain())
					stop->getInternalRank()->setGain(s.attributes.gain);
				if (importCmb.GetImportPitchTuning()) {
					float pitchT = stop->getInternalRank()->getPitchTuning() + s.attributes.pitchTuning;
					if (pitchT >= -1800 && pitchT <= 1800) {
						stop->getInternalRank()->setPitchTuning(pitchT);
					} else {
						wxMessageDialog pTerr(this, wxT("The PitchTuning value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
						if (pTerr.ShowModal() == wxID_YES) {
							if (pitchT > 1800)
								stop->getInternalRank()->setPitchTuning(1800);
							else if (pitchT < -1800)
								stop->getInternalRank()->setPitchTuning(-1800);
						}
					}
				}
				if (importCmb.GetImportPitchCorrection()) {
					float pitchC = stop->getInternalRank()->getPitchCorrection() + s.attributes.pitchCorrection;
					if (pitchC >= -1800 && pitchC <= 1800) {
						stop->getInternalRank()->setPitchCorrection(pitchC);
					} else {
						wxMessageDialog pCerr(this, wxT("The PitchCorrection value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
						if (pCerr.ShowModal() == wxID_YES) {
							if (pitchC > 1800)
								stop->getInternalRank()->setPitchCorrection(1800);
							else if (pitchC < -1800)
								stop->getInternalRank()->setPitchCorrection(-1800);
						}
					}
				}
				if (importCmb.GetImportTrackerDelay()) {
					stop->getInternalRank()->setTrackerDelay(s.attributes.trackerDelay);
				}

				// next deal with the pipes in the internal rank of the stop
				for (CMB_PIPE &p : s.pipes) {
					unsigned pipeIdx = p.pipeNbr - 1;
					if (pipeIdx < stop->getInternalRank()->m_pipes.size()) {
						Pipe *pipe = stop->getInternalRank()->getPipeAt(pipeIdx);

						if (importCmb.GetImportAmplitude())
							pipe->amplitudeLevel = p.attributes.amplitude;
						if (importCmb.GetImportGain())
							pipe->gain = p.attributes.gain;
						if (importCmb.GetImportPitchTuning()) {
							float pitchT = pipe->pitchTuning + p.attributes.pitchTuning;
							if (pitchT >= -1800 && pitchT <= 1800) {
								pipe->pitchTuning = pitchT;
							}
						}
						if (importCmb.GetImportPitchCorrection()) {
							float pitchC = pipe->pitchCorrection + p.attributes.pitchCorrection;
							if (pitchC >= -1800 && pitchC <= 1800) {
								pipe->pitchCorrection = pitchC;
							}
						}
						if (importCmb.GetImportTrackerDelay()) {
							pipe->trackerDelay = p.attributes.trackerDelay;
						}
					}
				}
			}
			stopIdx++;
		}

		unsigned rankIdx = 0;
		for (CMB_ELEMENT_WITH_PIPES &r : imported->cmbRanks) {
			if (m_organ->getNumberOfRanks() > rankIdx) {
				Rank *rank = m_organ->getOrganRankAt(rankIdx);

				if (importCmb.GetImportAmplitude())
					rank->setAmplitudeLevel(r.attributes.amplitude);
				if (importCmb.GetImportGain())
					rank->setGain(r.attributes.gain);
				if (importCmb.GetImportPitchTuning()) {
					float pitchT = rank->getPitchTuning() + r.attributes.pitchTuning;
					if (pitchT >= -1800 && pitchT <= 1800) {
						rank->setPitchTuning(pitchT);
					} else {
						wxMessageDialog pTerr(this, wxT("The PitchTuning value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
						if (pTerr.ShowModal() == wxID_YES) {
							if (pitchT > 1800)
								rank->setPitchTuning(1800);
							else if (pitchT < -1800)
								rank->setPitchTuning(-1800);
						}
					}
				}
				if (importCmb.GetImportPitchCorrection()) {
					float pitchC = rank->getPitchCorrection() + r.attributes.pitchCorrection;
					if (pitchC >= -1800 && pitchC <= 1800) {
						rank->setPitchCorrection(pitchC);
					} else {
						wxMessageDialog pCerr(this, wxT("The PitchCorrection value would be out of bounds! Do you want to set it to max allowed value?"), wxT("Value is out of bounds!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
						if (pCerr.ShowModal() == wxID_YES) {
							if (pitchC > 1800)
								rank->setPitchCorrection(1800);
							else if (pitchC < -1800)
								rank->setPitchCorrection(-1800);
						}
					}
				}
				if (importCmb.GetImportTrackerDelay()) {
					rank->setTrackerDelay(r.attributes.trackerDelay);
				}

				// next deal with the pipes in the rank
				for (CMB_PIPE &p : r.pipes) {
					unsigned pipeIdx = p.pipeNbr - 1;
					if (pipeIdx < rank->m_pipes.size()) {
						Pipe *pipe = rank->getPipeAt(pipeIdx);

						if (importCmb.GetImportAmplitude())
							pipe->amplitudeLevel = p.attributes.amplitude;
						if (importCmb.GetImportGain())
							pipe->gain = p.attributes.gain;
						if (importCmb.GetImportPitchTuning()) {
							float pitchT = pipe->pitchTuning + p.attributes.pitchTuning;
							if (pitchT >= -1800 && pitchT <= 1800) {
								pipe->pitchTuning = pitchT;
							}
						}
						if (importCmb.GetImportPitchCorrection()) {
							float pitchC = pipe->pitchCorrection + p.attributes.pitchCorrection;
							if (pitchC >= -1800 && pitchC <= 1800) {
								pipe->pitchCorrection = pitchC;
							}
						}
						if (importCmb.GetImportTrackerDelay()) {
							pipe->trackerDelay = p.attributes.trackerDelay;
						}
					}
				}
			}
			rankIdx++;
		}

		// Update display in panels
		m_organ->setModified(true);
		m_organPanel->setCurrentOrgan(m_organ);
		if (m_rankPanel->IsShown()) {
			Rank *currentRank = m_rankPanel->getCurrentRank();
			m_rankPanel->setRank(currentRank);
		}
		if (m_stopPanel->IsShown()) {
			Stop *currentStop = m_stopPanel->getCurrentStop();
			m_stopPanel->setStop(currentStop);
		}
	}
}

void GOODFFrame::OnEnableTooltipsMenu(wxCommandEvent& WXUNUSED(event)) {
	if (m_toolsMenu->IsChecked(ID_GLOBAL_SHOW_TOOLTIPS_OPTION)) {
		m_enableTooltips = true;
	} else {
		m_enableTooltips = false;
	}
	m_organPanel->setTooltipsEnabled(m_enableTooltips);
	m_rankPanel->setTooltipsEnabled(m_enableTooltips);
	m_stopPanel->setTooltipsEnabled(m_enableTooltips);
	m_manualPanel->setTooltipsEnabled(m_enableTooltips);
	m_windchestPanel->setTooltipsEnabled(m_enableTooltips);
	m_enclosurePanel->setTooltipsEnabled(m_enableTooltips);
	m_tremulantPanel->setTooltipsEnabled(m_enableTooltips);
	m_switchPanel->setTooltipsEnabled(m_enableTooltips);
	m_reversiblePistonPanel->setTooltipsEnabled(m_enableTooltips);
	m_divCplrPanel->setTooltipsEnabled(m_enableTooltips);
	m_couplerPanel->setTooltipsEnabled(m_enableTooltips);
	m_divisionalPanel->setTooltipsEnabled(m_enableTooltips);
	m_generalPanel->setTooltipsEnabled(m_enableTooltips);
	m_panelPanel->setTooltipsEnabled(m_enableTooltips);
	m_dispMetricsPanel->setTooltipsEnabled(m_enableTooltips);
	m_imagePanel->setTooltipsEnabled(m_enableTooltips);
	m_guiButtonPanel->setTooltipsEnabled(m_enableTooltips);
	m_guiEnclosurePanel->setTooltipsEnabled(m_enableTooltips);
	m_guiLabelPanel->setTooltipsEnabled(m_enableTooltips);
	m_guiManualPanel->setTooltipsEnabled(m_enableTooltips);
}

void GOODFFrame::OnRecentFileMenuChoice(wxCommandEvent& event) {
	int fileIndex = event.GetId() - wxID_FILE1;
	wxString fName(m_recentlyUsed->GetHistoryFile(event.GetId() - wxID_FILE1));
	wxFileName f_name = wxFileName(fName);
	if (f_name.Exists()) {
		DoOpenOrgan(fName);
	} else {
		wxString errorMessage = wxString::Format(wxT("The file: %s doesn't exist!"), fName);
		wxMessageDialog msg(this, errorMessage, wxT("File not found!"), wxOK|wxCENTRE|wxICON_ERROR);
		msg.ShowModal();
		m_recentlyUsed->RemoveFileFromHistory(fileIndex);
	}
}

void GOODFFrame::OnClearHistory(wxCommandEvent& WXUNUSED(event)) {
	while (m_recentlyUsed->GetCount()) {
		m_recentlyUsed->RemoveFileFromHistory(0);
	}
}

void GOODFFrame::OnSizeChange(wxSizeEvent& event) {
	UpdateFrameSizeAndPos();
	event.Skip();
}

void GOODFFrame::OnDefaultPathMenuChoice(wxCommandEvent& WXUNUSED(event)) {
	DefaultPathsDialog defaultPaths(m_defaultOrganDirectory, m_defaultCmbDirectory, this);
	if (defaultPaths.ShowModal() == wxID_OK) {
		m_defaultOrganDirectory = defaultPaths.GetSelectedOrganDirectory();
		m_defaultCmbDirectory = defaultPaths.GetSelectedCmbDirectory();
	}
}

void GOODFFrame::OnImportStopRank(wxCommandEvent& WXUNUSED(event)) {
	wxString organFilePath;
	wxString defaultPath = m_defaultOrganDirectory;
	if (defaultPath == wxEmptyString)
		defaultPath = wxStandardPaths::Get().GetDocumentsDir();

	wxFileDialog fileDialog(
		this,
		wxT("Select .organ file to import stops/ranks from"),
		defaultPath,
		"",
		"GrandOrgue ODF files (*.organ)|*.organ;*.ORGAN",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST
	);

	if (fileDialog.ShowModal() != wxID_OK) {
		return;
	}

	organFilePath = fileDialog.GetPath();

	Organ *sourceOrgan = new Organ();

	OrganFileParser parser(organFilePath, sourceOrgan);
	if (parser.isOrganReady()) {
		StopRankImportDialog importDialog(sourceOrgan, m_organ, this);
		importDialog.ShowModal();

	} else {
		wxMessageDialog msg(this, wxT("The selected .organ file could not be parsed for importing any stops/ranks!"), wxT("Failure to parse .organ file"), wxOK|wxCENTRE|wxICON_ERROR);
		msg.ShowModal();
	}

	delete sourceOrgan;
}

void GOODFFrame::OnImportLegacyXfadesMenu(wxCommandEvent& WXUNUSED(event)) {
	wxMessageDialog dlg(this, wxT("Loop- and ReleaseCrossfadeLength values of first attack in every pipe for all ranks/stops will be copied to any extra attacks/releases that just have a default value in order to re-create the x-fade behaviour pre GO v3.14.0! Do you want to proceed?"), wxT("X-fade values will be 'imported'!"), wxYES_NO|wxCENTRE|wxICON_EXCLAMATION);
	if (dlg.ShowModal() != wxID_YES)
		return;

	m_organ->doInheritLegacyXfades();
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
	m_organ->setModified(true);
}

void GOODFFrame::AddGuiElementToTree(wxString title) {
	// This is called from a panel that's currently selected in the tree
	wxTreeItemId selectedPanel = m_organTreeCtrl->GetSelection();
	wxTreeItemId guiElementChild = m_organTreeCtrl->GetLastChild(selectedPanel);
	wxTreeItemId newGuiElement = m_organTreeCtrl->AppendItem(guiElementChild, title);
	m_organTreeCtrl->ExpandAllChildren(selectedPanel);
	m_organTreeCtrl->SelectItem(newGuiElement);
	m_organTreeCtrl->SelectItem(selectedPanel);
	m_organ->setModified(true);
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
	if (panelId.IsOk()) {
		wxTreeItemId guiElements = m_organTreeCtrl->GetLastChild(panelId);
		m_organTreeCtrl->DeleteChildren(guiElements);
		int nbrElements = m_organ->getOrganPanelAt(panelIndex)->getNumberOfGuiElements();
		for (int i = 0; i < nbrElements; i++) {
			m_organTreeCtrl->AppendItem(guiElements, m_organ->getOrganPanelAt(panelIndex)->getGuiElementAt(i)->getDisplayName());
		}
	}
}

void GOODFFrame::PanelGUIPropertyIsChanged() {
	m_panelPanel->updateRepresentationLayout();
	m_organ->setModified(true);
}

void GOODFFrame::GUIElementPositionIsChanged() {
	m_imagePanel->updatePositionValues();
	m_guiButtonPanel->updatePositionValues();
	m_guiEnclosurePanel->updatePositionValues();
	m_guiLabelPanel->updatePositionValues();
	m_guiManualPanel->updatePositionValues();
	m_organ->setModified(true);
}

void GOODFFrame::UpdateFrameTitle() {
	if (m_organ->isModified()) {
		SetTitle(::wxGetApp().m_fullAppName + wxT(" - ") + m_organPanel->getOdfName() + wxT(" (modified)"));
	} else {
		SetTitle(::wxGetApp().m_fullAppName + wxT(" - ") + m_organPanel->getOdfName());
	}
}

void GOODFFrame::SynchronizePipeReadingOptions(RankPanel* rankPanel, wxString atkFolder, bool oneAttack, bool loadRelease, wxString releaseFolder, bool extractTime, wxString tremFolder, bool loadAsTremOff) {
	if (rankPanel == m_rankPanel) {
		// The call comes from a stand alone rank so we should call the internal rank panel of the stop panel
		m_stopPanel->GetInternalRankPanel()->SetPipeReadingOptions(atkFolder, oneAttack, loadRelease, releaseFolder, extractTime, tremFolder, loadAsTremOff);
	} else {
		// The call comes from an internal rank of a stop
		m_rankPanel->SetPipeReadingOptions(atkFolder, oneAttack, loadRelease, releaseFolder, extractTime, tremFolder, loadAsTremOff);
	}
}

wxString GOODFFrame::GetDefaultOrganDirectory() {
	return m_defaultOrganDirectory;
}

wxString GOODFFrame::GetDefaultCmbDirectory() {
	return m_defaultCmbDirectory;
}

wxLogWindow* GOODFFrame::GetLogWindow() {
	return m_logWindow;
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

void GOODFFrame::UpdateFrameSizeAndPos() {
	if (IsMaximized())
		m_frameMaximized = true;
	else {
		GetPosition(&m_xPosition, &m_yPosition);
		GetSize(&m_frameWidth, &m_frameHeight);
		m_frameMaximized = false;
	}
}

void GOODFFrame::RecreateLogWindow() {
	wxLog::SetActiveTarget(nullptr);
	delete m_logWindow;
	m_logWindow = new wxLogWindow(this, wxT("Log messages"), false, false);
	wxLog::SetActiveTarget(m_logWindow);
}

void GOODFFrame::SetImportXfadeMenuItemState() {
	if (!m_organ->getNumberOfRanks() && !m_organ->getNumberOfStops())
		m_toolsMenu->Enable(ID_GLOBAL_PARSE_LEGACY_XFADES_OPTION, false);
	else
		m_toolsMenu->Enable(ID_GLOBAL_PARSE_LEGACY_XFADES_OPTION, true);
}

void GOODFFrame::FixAnyIllegalEntries() {
	m_organPanel->fixTrailingSpacesInStrings();
	m_organ->fixTrailingSpacesInStrings();

	if (m_Splitter->GetWindow2() == m_organPanel) {
		m_organPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_enclosurePanel) {
		m_enclosurePanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_tremulantPanel) {
		m_tremulantPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_windchestPanel) {
		m_windchestPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_switchPanel) {
		m_switchPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_rankPanel) {
		m_rankPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_stopPanel) {
		m_stopPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_manualPanel) {
		m_manualPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_couplerPanel) {
		m_couplerPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_divisionalPanel) {
		m_divisionalPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_divCplrPanel) {
		m_divCplrPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_generalPanel) {
		m_generalPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_reversiblePistonPanel) {
		m_reversiblePistonPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_panelPanel) {
		m_panelPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_guiButtonPanel) {
		m_guiButtonPanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_guiEnclosurePanel) {
		m_guiEnclosurePanel->refreshData();
	} else if (m_Splitter->GetWindow2() == m_guiLabelPanel) {
		m_guiLabelPanel->refreshData();
	}

	// Any name entries in the organ tree above the panels should be updated too
	int count = (int) m_organTreeCtrl->GetChildrenCount(tree_manuals, false);
	if (count > 0) {
		wxTreeItemId currentMan = m_organTreeCtrl->GetLastChild(tree_manuals);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentMan, m_organ->getOrganManualAt(i)->getName());

			// Each manual also can contain Stops, Couplers and Divisionals
			wxTreeItemId manDivisionals = m_organTreeCtrl->GetLastChild(currentMan);
			wxTreeItemId manCouplers = m_organTreeCtrl->GetPrevSibling(manDivisionals);
			wxTreeItemId manStops = m_organTreeCtrl->GetPrevSibling(manCouplers);
			int divCount = (int) m_organTreeCtrl->GetChildrenCount(manDivisionals);
			if (divCount > 0) {
				wxTreeItemId currentDiv = m_organTreeCtrl->GetLastChild(manDivisionals);
				for (int j = divCount - 1; j >= 0; j--) {
					m_organTreeCtrl->SetItemText(currentDiv, m_organ->getOrganManualAt(i)->getDivisionalAt(j)->getName());
					if (j > 0)
						currentDiv = m_organTreeCtrl->GetPrevSibling(currentDiv);
				}
			}
			int cplrCount = (int) m_organTreeCtrl->GetChildrenCount(manCouplers);
			if (cplrCount > 0) {
				wxTreeItemId currentCplr = m_organTreeCtrl->GetLastChild(manCouplers);
				for (int j = cplrCount - 1; j >= 0; j--) {
					m_organTreeCtrl->SetItemText(currentCplr, m_organ->getOrganManualAt(i)->getCouplerAt(j)->getName());
					if (j > 0)
						currentCplr = m_organTreeCtrl->GetPrevSibling(currentCplr);
				}
			}
			int stopCount = (int) m_organTreeCtrl->GetChildrenCount(manStops);
			if (stopCount > 0) {
				wxTreeItemId currentStop = m_organTreeCtrl->GetLastChild(manStops);
				for (int j = stopCount - 1; j >= 0; j--) {
					m_organTreeCtrl->SetItemText(currentStop, m_organ->getOrganManualAt(i)->getStopAt(j)->getName());
					if (j > 0)
						currentStop = m_organTreeCtrl->GetPrevSibling(currentStop);
				}
			}

			if (i > 0)
				currentMan = m_organTreeCtrl->GetPrevSibling(currentMan);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_windchestgrps);
	if (count > 0) {
		wxTreeItemId currentWc = m_organTreeCtrl->GetLastChild(tree_windchestgrps);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentWc, m_organ->getOrganWindchestgroupAt(i)->getName());
			if (i > 0)
				currentWc = m_organTreeCtrl->GetPrevSibling(currentWc);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_enclosures);
	if (count > 0) {
		wxTreeItemId currentEnc = m_organTreeCtrl->GetLastChild(tree_enclosures);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentEnc, m_organ->getOrganEnclosureAt(i)->getName());
			if (i > 0)
				currentEnc = m_organTreeCtrl->GetPrevSibling(currentEnc);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_tremulants);
	if (count > 0) {
		wxTreeItemId currentTr = m_organTreeCtrl->GetLastChild(tree_tremulants);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentTr, m_organ->getOrganTremulantAt(i)->getName());
			if (i > 0)
				currentTr = m_organTreeCtrl->GetPrevSibling(currentTr);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_ranks);
	if (count > 0) {
		wxTreeItemId currentRk = m_organTreeCtrl->GetLastChild(tree_ranks);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentRk, m_organ->getOrganRankAt(i)->getName());
			if (i > 0)
				currentRk = m_organTreeCtrl->GetPrevSibling(currentRk);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_switches);
	if (count > 0) {
		wxTreeItemId currentSw = m_organTreeCtrl->GetLastChild(tree_switches);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentSw, m_organ->getOrganSwitchAt(i)->getName());
			if (i > 0)
				currentSw = m_organTreeCtrl->GetPrevSibling(currentSw);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_reversiblePistons);
	if (count > 0) {
		wxTreeItemId currentP = m_organTreeCtrl->GetLastChild(tree_reversiblePistons);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentP, m_organ->getReversiblePistonAt(i)->getName());
			if (i > 0)
				currentP = m_organTreeCtrl->GetPrevSibling(currentP);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_divisionalCouplers);
	if (count > 0) {
		wxTreeItemId currentDc = m_organTreeCtrl->GetLastChild(tree_divisionalCouplers);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentDc, m_organ->getOrganDivisionalCouplerAt(i)->getName());
			if (i > 0)
				currentDc = m_organTreeCtrl->GetPrevSibling(currentDc);
		}
	}

	count = (int) m_organTreeCtrl->GetChildrenCount(tree_generals);
	if (count > 0) {
		wxTreeItemId currentG = m_organTreeCtrl->GetLastChild(tree_generals);
		for (int i = count - 1; i >= 0; i--) {
			m_organTreeCtrl->SetItemText(currentG, m_organ->getOrganGeneralAt(i)->getName());
			if (i > 0)
				currentG = m_organTreeCtrl->GetPrevSibling(currentG);
		}
	}

}
