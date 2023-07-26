/* 
 * GOODFFrame.h is a part of GOODF software
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

#ifndef GOODFFRAME_H
#define GOODFFRAME_H

#include <wx/wx.h>
#include "Organ.h"
#include "OrganPanel.h"
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <wx/spinctrl.h>
#include "EnclosurePanel.h"
#include "TremulantPanel.h"
#include "WindchestgroupPanel.h"
#include "SwitchPanel.h"
#include "RankPanel.h"
#include "StopPanel.h"
#include "ManualPanel.h"
#include "CouplerPanel.h"
#include "DivisionalPanel.h"
#include "DivisionalCouplerPanel.h"
#include "GeneralPanel.h"
#include "ReversiblePistonPanel.h"
#include "GoPanelPanel.h"
#include "DisplayMetricsPanel.h"
#include "GoImagePanel.h"
#include "GUIButtonPanel.h"
#include "GUIEnclosurePanel.h"
#include "GUILabelPanel.h"
#include "GUIManualPanel.h"

class GOODFFrame : public wxFrame {
public:
	GOODFFrame(const wxString& title);
	~GOODFFrame();

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
	void OnWriteODF(wxCommandEvent& event);
	void OnReadOrganFile(wxCommandEvent& event);

	void OrganTreeChildItemLabelChanged(wxString label);
	void RemoveCurrentItemFromOrgan();

	void AddStopItemToTree();
	void SelectStopItemInTree(int nbrAdded);
	void AddCouplerItemToTree();
	void AddDivisionalItemToTree();
	void AddImageItemToTree();
	void AddGuiElementToTree(wxString title);
	void RebuildPanelGuiElementsInTree(int panelIndex);
	void PanelGUIPropertyIsChanged();

	Organ *m_organ;

private:
	DECLARE_EVENT_TABLE()

	wxMenu *m_fileMenu;
	wxMenu *m_helpMenu;
	wxMenuBar *m_menuBar;
	wxSplitterWindow *m_Splitter;
	wxPanel *addManual;
	wxPanel *addWindchest;
	wxPanel *addEnclosure;
	wxPanel *addTremulant;
	wxPanel *addRank;
	wxPanel *addSwitch;
	wxPanel *addReversiblePiston;
	wxPanel *addDivisionalCoupler;
	wxPanel *addGeneral;
	wxPanel *addPanel;
	wxButton *addWindchestBtn;
	wxButton *addEnclosureBtn;
	wxButton *addRankBtn;
	wxButton *addSwitchBtn;
	wxSpinCtrl *nbrWindchestSpin;
	wxSpinCtrl *nbrEnclosureSpin;
	wxSpinCtrl *nbrRankSpin;
	wxSpinCtrl *nbrSwitchSpin;
	wxTreeCtrl *m_organTreeCtrl;
	wxTreeItemId tree_organ;
	wxTreeItemId tree_manuals;
	wxTreeItemId tree_windchestgrps;
	wxTreeItemId tree_enclosures;
	wxTreeItemId tree_tremulants;
	wxTreeItemId tree_ranks;
	wxTreeItemId tree_switches;
	wxTreeItemId tree_reversiblePistons;
	wxTreeItemId tree_divisionalCouplers;
	wxTreeItemId tree_generals;
	wxTreeItemId tree_panels;
	wxTreeItemId m_draggedItem;

	OrganPanel *m_organPanel;
	EnclosurePanel *m_enclosurePanel;
	TremulantPanel *m_tremulantPanel;
	WindchestgroupPanel *m_windchestPanel;
	SwitchPanel *m_switchPanel;
	RankPanel *m_rankPanel;
	StopPanel *m_stopPanel;
	ManualPanel *m_manualPanel;
	CouplerPanel *m_couplerPanel;
	DivisionalPanel *m_divisionalPanel;
	DivisionalCouplerPanel *m_divCplrPanel;
	GeneralPanel *m_generalPanel;
	ReversiblePistonPanel *m_reversiblePistonPanel;
	GoPanelPanel *m_panelPanel;
	DisplayMetricsPanel *m_dispMetricsPanel;
	GoImagePanel *m_imagePanel;
	GUIButtonPanel *m_guiButtonPanel;
	GUIEnclosurePanel *m_guiEnclosurePanel;
	GUILabelPanel *m_guiLabelPanel;
	GUIManualPanel *m_guiManualPanel;

	bool m_organHasBeenSaved;

	void OnOrganTreeSelectionChanged(wxTreeEvent& event);
	void OnOrganTreeRightClicked(wxTreeEvent& event);
	void OnOrganTreeLeftDrag(wxTreeEvent& event);
	void OnOrganTreeDragCompleted(wxTreeEvent& event);
	void OnAddNewEnclosure(wxCommandEvent& event);
	void OnAddNewTremulant(wxCommandEvent& event);
	void OnAddNewWindchestgroup(wxCommandEvent& event);
	void OnAddNewSwitch(wxCommandEvent& event);
	void OnAddNewRank(wxCommandEvent& event);
	void OnNewOrgan(wxCommandEvent& event);
	void OnAddNewManual(wxCommandEvent& event);
	void OnAddNewDivisionalCoupler(wxCommandEvent& event);
	void OnAddNewGeneral(wxCommandEvent& event);
	void OnAddNewReversiblePiston(wxCommandEvent& event);
	void OnAddNewPanel(wxCommandEvent& event);

	void SetupOrganMainPanel();
	void removeAllItemsFromTree();

};

#endif
