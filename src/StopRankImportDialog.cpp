/*
 * StopRankImportDialog.cpp is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (See AUTHORS)
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

#include "StopRankImportDialog.h"
#include "GOODFDef.h"
#include "GOODF.h"
#include "GOODFFunctions.h"
#include <wx/statline.h>
#include <vector>

IMPLEMENT_CLASS(StopRankImportDialog, wxDialog)

BEGIN_EVENT_TABLE(StopRankImportDialog, wxDialog)
	EVT_LISTBOX(ID_AVAILABLE_STOPS_LIST, StopRankImportDialog::OnAvailableStopRankSelection)
	EVT_LISTBOX(ID_AVAILABLE_RANKS_LIST, StopRankImportDialog::OnAvailableStopRankSelection)
	EVT_CHOICE(ID_TARGET_MANUAL_CHOICE, StopRankImportDialog::OnManualChoice)
	EVT_BUTTON(ID_STOP_RANK_IMPORT_BTN, StopRankImportDialog::OnDoImportBtn)
END_EVENT_TABLE()

StopRankImportDialog::StopRankImportDialog(Organ *source, Organ *target) {
	Init(source, target);
}

StopRankImportDialog::StopRankImportDialog(
	Organ *source,
	Organ *target,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	Init(source, target);
	Create(parent, id, caption, pos, size, style);
}

StopRankImportDialog::~StopRankImportDialog() {

}

void StopRankImportDialog::Init(Organ *source, Organ *target) {
	m_targetOrgan = target;
	m_sourceOrgan = source;

	for (unsigned i = 0; i < m_targetOrgan->getNumberOfManuals(); i++) {
		m_targetManualList.Add(m_targetOrgan->getOrganManualAt(i)->getName());
	}

	for (unsigned i = 0; i < m_sourceOrgan->getNumberOfStops(); i++) {
		wxString stopName = m_sourceOrgan->getOrganStopAt(i)->getName();
		wxString manualName = m_sourceOrgan->getOrganStopAt(i)->getOwningManual()->getName();
		wxString stopIdStr = wxString::Format(wxT(" (Stop in %s)"), manualName);
		wxString fullStr = stopName + stopIdStr;
		m_importStopList.Add(fullStr);
	}

	for (unsigned i = 0; i < m_sourceOrgan->getNumberOfRanks(); i++) {
		wxString rankName = m_sourceOrgan->getOrganRankAt(i)->getName();
		wxString idStr = wxString::Format(wxT(" (Rank%0.3u)"), i + 1);
		wxString rankStr = rankName + idStr;
		m_importRankList.Add(rankStr);
	}

	// populate available windchests
	if (!m_availableWindchests.IsEmpty())
		m_availableWindchests.Empty();
	unsigned nbWc = m_targetOrgan->getNumberOfWindchestgroups();
	if (nbWc > 0) {
		for (unsigned i = 0; i < nbWc; i++) {
			m_availableWindchests.Add(m_targetOrgan->getOrganWindchestgroupAt(i)->getName());
		}
	}
}

bool StopRankImportDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();
	SetTitle(wxT("Import stop(s)/rank(s) from ") + m_sourceOrgan->getChurchName());

	if (m_targetManualList.IsEmpty()) {
		m_availableStopList->Enable(false);
	}

	return true;
}

void StopRankImportDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *firstLeft = new wxBoxSizer(wxVERTICAL);
	wxStaticText *stopText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available Stops")
	);
	firstLeft->Add(stopText, 0, wxALL, 5);
	m_availableStopList = new wxListBox(
		this,
		ID_AVAILABLE_STOPS_LIST,
		wxDefaultPosition,
		wxDefaultSize,
		m_importStopList,
		wxLB_EXTENDED
	);
	firstLeft->Add(m_availableStopList, 1, wxEXPAND|wxALL, 5);
	wxStaticText *rankText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Available Ranks")
	);
	firstLeft->Add(rankText, 0, wxALL, 5);
	m_availableRankList = new wxListBox(
		this,
		ID_AVAILABLE_RANKS_LIST,
		wxDefaultPosition,
		wxDefaultSize,
		m_importRankList,
		wxLB_EXTENDED
	);
	firstLeft->Add(m_availableRankList, 1, wxEXPAND|wxALL, 5);
	firstRow->Add(firstLeft, 1,  wxGROW|wxALL, 5);

	wxBoxSizer *firstRight = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *rightUpper = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *manualText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Import stops to manual: ")
	);
	rightUpper->Add(manualText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_manualChoice = new wxChoice(
		this,
		ID_TARGET_MANUAL_CHOICE,
		wxDefaultPosition,
		wxDefaultSize,
		m_targetManualList
	);
	rightUpper->Add(m_manualChoice, 0, wxALL, 5);
	firstRight->Add(rightUpper, 0, wxGROW|wxALL, 5);
	wxStaticText *windchestNoteText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Note: Choice of windchest for imported stop(s)/rank(s) is optional,\nbut if not set now at import, it must be set later to have a working odf!")
	);
	firstRight->Add(windchestNoteText, 0, wxALL, 5);
	wxBoxSizer *rightMid = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText *windchestText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Windchestgroup: ")
	);
	rightMid->Add(windchestText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	m_windchestChoice = new wxChoice(
		this,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		m_availableWindchests
	);
	rightMid->Add(m_windchestChoice, 0, wxALL, 5);
	firstRight->Add(rightMid, 0, wxGROW|wxALL, 5);
	wxBoxSizer *rightLower = new wxBoxSizer(wxVERTICAL);
	wxStaticText *infoNotesText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Information notes:")
	);
	rightLower->Add(infoNotesText, 0, wxALL, 5);
	m_infoText = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_MULTILINE|wxTE_READONLY
	);
	rightLower->Add(m_infoText, 1, wxEXPAND|wxALL, 5);
	m_doImportBtn = new wxButton(
		this,
		ID_STOP_RANK_IMPORT_BTN,
		wxT("Import selected Stop(s) and/or Rank(s)")
	);
	rightLower->Add(m_doImportBtn, 0, wxGROW|wxALL, 5);
	m_doImportBtn->Enable(false);
	firstRight->Add(rightLower, 1, wxGROW|wxTOP|wxLEFT|wxRIGHT, 5);
	firstRow->Add(firstRight, 1, wxGROW|wxALL, 5);
	mainSizer->Add(firstRow, 1, wxGROW|wxALL, 5);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	wxBoxSizer *secondRow = new wxBoxSizer(wxVERTICAL);
	wxStaticText *logText = new wxStaticText (
		this,
		wxID_STATIC,
		wxT("Import log:")
	);
	secondRow->Add(logText, 0, wxALL, 5);
	m_logText = new wxTextCtrl(
		this,
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxSize(-1, GetCharHeight() * 6),
		wxTE_MULTILINE|wxTE_READONLY
	);
	secondRow->Add(m_logText, 1, wxEXPAND|wxALL, 5);
	mainSizer->Add(secondRow, 0, wxGROW|wxALL, 5);

	SetSizer(mainSizer);
}

void StopRankImportDialog::OnAvailableStopRankSelection(wxCommandEvent& WXUNUSED(event)) {
	m_infoText->Clear();
	wxArrayInt selectedStops;
	int stopCount = m_availableStopList->GetSelections(selectedStops);
	if (!selectedStops.IsEmpty()) {
		for (int i = 0; i < stopCount; i++) {
			Stop *s = m_sourceOrgan->getOrganStopAt(selectedStops[i]);
			if (s->isUsingInternalRank()) {
				std::vector<Stop*> dependsOn;
				for (Pipe p : s->getInternalRank()->m_pipes) {
					if (p.m_attacks.front().fileName.StartsWith(wxT("REF"))) {
						// This stop has some reference borrowing that it depends on
						Stop *pipeDependsOn = m_sourceOrgan->getStopFromRefString(p.m_attacks.front().fileName);
						if (pipeDependsOn != NULL) {
							bool alreadyAdded = false;
							for (unsigned n = 0; n < dependsOn.size(); n++) {
								if (pipeDependsOn == dependsOn[n]) {
									alreadyAdded = true;
									break;
								}
							}
							if (!alreadyAdded) {
								dependsOn.push_back(pipeDependsOn);
							}
						}
					}
				}
				if (!dependsOn.empty()) {
					for (unsigned j = 0; j < dependsOn.size(); j++) {
						m_infoText->AppendText(s->getName() + wxT(" borrows (old style) from ") + dependsOn[j]->getName() + wxT("\n"));
						if (!m_availableStopList->IsSelected(m_sourceOrgan->getIndexOfOrganStop(dependsOn[j]) - 1)) {
							m_availableStopList->SetSelection(m_sourceOrgan->getIndexOfOrganStop(dependsOn[j]) - 1);
							m_infoText->AppendText(wxT("Added ") + dependsOn[j]->getName() + wxT(" to selection.\n"));
						}
					}
				}
			} else {
				// at least one rank must also be selected to be able to import this stop
				for (unsigned j = 0; j < s->getNumberOfRanks(); j++) {
					m_infoText->AppendText(s->getName() + wxT(" references rank ") + s->getRankAt(j)->getName() + wxT("\n"));
					if (!m_availableRankList->IsSelected(m_sourceOrgan->getIndexOfOrganRank(s->getRankAt(j)) - 1)) {
						m_availableRankList->SetSelection(m_sourceOrgan->getIndexOfOrganRank(s->getRankAt(j)) - 1);
					}
				}
			}
		}
	}

	wxArrayInt selectedRanks;
	int rankCount = m_availableRankList->GetSelections(selectedRanks);
	if (!selectedRanks.IsEmpty()) {
		for (int i = 0; i < rankCount; i++) {
			Rank *r = m_sourceOrgan->getOrganRankAt(selectedRanks[i]);
			std::vector<Stop*> dependsOn;
			for (Pipe p : r->m_pipes) {
				if (p.m_attacks.front().fileName.StartsWith(wxT("REF"))) {
					// This rank has some reference borrowing that it depends on
					Stop *pipeDependsOn = m_sourceOrgan->getStopFromRefString(p.m_attacks.front().fileName);
					if (pipeDependsOn != NULL) {
						bool alreadyAdded = false;
						for (unsigned n = 0; n < dependsOn.size(); n++) {
							if (pipeDependsOn == dependsOn[n]) {
								alreadyAdded = true;
								break;
							}
						}
						if (!alreadyAdded) {
							dependsOn.push_back(pipeDependsOn);
						}
					}
				}
			}
			if (!dependsOn.empty()) {
				for (unsigned j = 0; j < dependsOn.size(); j++) {
					m_infoText->AppendText(r->getName() + wxT(" borrows (old style) from ") + dependsOn[j]->getName() + wxT("\n"));
					if (!m_availableStopList->IsSelected(m_sourceOrgan->getIndexOfOrganStop(dependsOn[j]) - 1)) {
						m_availableStopList->SetSelection(m_sourceOrgan->getIndexOfOrganStop(dependsOn[j]) - 1);
						m_infoText->AppendText(wxT("Added ") + dependsOn[j]->getName() + wxT(" to selection.\n"));
					}
				}
			}
		}
	}

	CanImportButtonBeEnabled();
}

void StopRankImportDialog::OnDoImportBtn(wxCommandEvent& WXUNUSED(event)) {
	m_infoText->Clear();
	m_logText->AppendText(wxT("\n"));

	wxArrayInt selectedStops;
	int stopCount = m_availableStopList->GetSelections(selectedStops);
	wxArrayInt selectedRanks;
	int rankCount = m_availableRankList->GetSelections(selectedRanks);
	unsigned nbrStopsBeforeImport = m_targetOrgan->getNumberOfStops();
	unsigned nbrRanksBeforeImport = m_targetOrgan->getNumberOfRanks();

	if (nbrStopsBeforeImport + stopCount < 1000 && nbrRanksBeforeImport + rankCount < 1000) {
		if (!selectedStops.IsEmpty()) {
			for (int i = 0; i < stopCount; i++) {
				Stop *s = m_sourceOrgan->getOrganStopAt(selectedStops[i]);
				Stop importedStop(*s);
				unsigned manIdx = m_manualChoice->GetSelection();
				Manual *targetManual = m_targetOrgan->getOrganManualAt(manIdx);
				importedStop.setOwningManual(targetManual);
				if (importedStop.isUsingInternalRank()) {
					if (m_windchestChoice->GetSelection() != wxNOT_FOUND)
						importedStop.getInternalRank()->setWindchest(m_targetOrgan->getOrganWindchestgroupAt(m_windchestChoice->GetSelection()));
					else
						importedStop.getInternalRank()->setWindchest(NULL);
				}
				m_targetOrgan->addStop(importedStop);
				unsigned nbStops = m_targetOrgan->getNumberOfStops();
				if (nbStops > 0) {
					targetManual->addStop(m_targetOrgan->getOrganStopAt(nbStops - 1));
					::wxGetApp().m_frame->AddStopItemForManual(s->getName(), manIdx);
					m_logText->AppendText(wxT("Imported stop: ") + s->getName() + wxT("\n"));
				}
			}
		}

		if (!selectedRanks.IsEmpty()) {
			for (int i = 0; i < rankCount; i++) {
				Rank *r = m_sourceOrgan->getOrganRankAt(selectedRanks[i]);
				Rank importedRank(*r);
				if (m_windchestChoice->GetSelection() != wxNOT_FOUND)
					importedRank.setWindchest(m_targetOrgan->getOrganWindchestgroupAt(m_windchestChoice->GetSelection()));
				else
					importedRank.setWindchest(NULL);
				m_targetOrgan->addRank(importedRank);
				::wxGetApp().m_frame->AddRankItemToTree(r->getName());
				m_logText->AppendText(wxT("Imported rank: ") + r->getName() + wxT("\n"));
			}
		}

		m_targetOrgan->updateRelativePipePaths();

		// Now any REF type of borrowing needs to be fixed in the imported stops/ranks!
		if (!selectedStops.IsEmpty()) {
			for (int i = 0; i < stopCount; i++) {
				Stop *importedStop = m_targetOrgan->getOrganStopAt(nbrStopsBeforeImport + i);
				if (importedStop->isUsingInternalRank()) {
					for (Pipe &p : importedStop->getInternalRank()->m_pipes) {
						if (p.m_attacks.front().fileName.StartsWith(wxT("REF"))) {
							wxString originalRefStr = p.m_attacks.front().fileName;
							Stop *pipeDependsOn = m_sourceOrgan->getStopFromRefString(originalRefStr);

							if (pipeDependsOn != NULL) {
								unsigned stopNbr = m_sourceOrgan->getIndexOfOrganStop(pipeDependsOn); // returned nbr is one more than index!
								int addedStopIdx = selectedStops.Index((int)  stopNbr - 1);
								Stop *targetStopDependency = m_targetOrgan->getOrganStopAt(nbrStopsBeforeImport + addedStopIdx);
								int manualRefNumber = m_targetOrgan->getIndexOfOrganManual(targetStopDependency->getOwningManual());
								int stopRefNumber = targetStopDependency->getOwningManual()->getIndexOfStop(targetStopDependency) + 1;
								wxString pipeNumber = originalRefStr.AfterLast(':');
								wxString newRefString = wxT("REF:") + GOODF_functions::number_format(manualRefNumber) + wxT(":") + GOODF_functions::number_format(stopRefNumber) + wxT(":") + pipeNumber;
								p.m_attacks.front().fileName = newRefString;
								p.m_attacks.front().fullPath = newRefString;
							}
						}
					}
				}
			}
		}
		if (!selectedRanks.IsEmpty()) {
			for (int i = 0; i < rankCount; i++) {
				Rank *importedRank = m_targetOrgan->getOrganRankAt(nbrRanksBeforeImport + i);
				for (Pipe &p : importedRank->m_pipes) {
					if (p.m_attacks.front().fileName.StartsWith(wxT("REF"))) {
						wxString originalRefStr = p.m_attacks.front().fileName;
						Stop *pipeDependsOn = m_sourceOrgan->getStopFromRefString(originalRefStr);

						if (pipeDependsOn != NULL) {
							unsigned stopNbr = m_sourceOrgan->getIndexOfOrganStop(pipeDependsOn); // returned nbr is one more than index!
							int addedStopIdx = selectedStops.Index((int)  stopNbr - 1);
							Stop *targetStopDependency = m_targetOrgan->getOrganStopAt(nbrStopsBeforeImport + addedStopIdx);
							int manualRefNumber = m_targetOrgan->getIndexOfOrganManual(targetStopDependency->getOwningManual());
							int stopRefNumber = targetStopDependency->getOwningManual()->getIndexOfStop(targetStopDependency) + 1;
							wxString pipeNumber = originalRefStr.AfterLast(':');
							wxString newRefString = wxT("REF:") + GOODF_functions::number_format(manualRefNumber) + wxT(":") + GOODF_functions::number_format(stopRefNumber) + wxT(":") + pipeNumber;
							p.m_attacks.front().fileName = newRefString;
							p.m_attacks.front().fullPath = newRefString;
						}
					}
				}
			}
		}

		// Any rank references must be adjusted correctly
		if (!selectedStops.IsEmpty()) {
			for (int i = 0; i < stopCount; i++) {
				Stop *s = m_sourceOrgan->getOrganStopAt(selectedStops[i]);
				if (!s->isUsingInternalRank()) {
					Stop *targetStop = m_targetOrgan->getOrganStopAt(nbrStopsBeforeImport + i);
					for (unsigned j = 0; j < s->getNumberOfRanks(); j++) {
						RankReference *reference = s->getRankReferenceAt(j);
						unsigned rankNumber = m_sourceOrgan->getIndexOfOrganRank(reference->m_rankReference); // result is one more than the index!
						int addedRankIdx = selectedRanks.Index((int) rankNumber - 1);
						Rank *newRankReference = m_targetOrgan->getOrganRankAt(nbrRanksBeforeImport + addedRankIdx);
						RankReference *targetRef = targetStop->getRankReferenceAt(j);
						targetRef->m_rankReference = newRankReference;
					}
				}

			}
		}

		if (!selectedStops.IsEmpty()) {
			for (int i = 0; i < stopCount; i++) {
				m_availableStopList->Deselect(selectedStops[i]);
			}
		}

		if (!selectedRanks.IsEmpty()) {
			for (int i = 0; i < rankCount; i++) {
				m_availableRankList->Deselect(selectedRanks[i]);
			}
		}

		CanImportButtonBeEnabled();
	} else {
		// the target organ already contain too many stops and/or ranks
		wxMessageDialog msg(this, wxT("The targeted organ would contain too many stops/ranks! Maximum number of stops/ranks is 999."), wxT("Failure to import stops/ranks"), wxOK|wxCENTRE|wxICON_ERROR);
		msg.ShowModal();
	}
}

void StopRankImportDialog::OnManualChoice(wxCommandEvent& WXUNUSED(event)) {
	CanImportButtonBeEnabled();
}

bool StopRankImportDialog::CanImportButtonBeEnabled() {
	wxArrayInt selectedStops;
	m_availableStopList->GetSelections(selectedStops);
	wxArrayInt selectedRanks;
	m_availableRankList->GetSelections(selectedRanks);
	if ((!selectedStops.IsEmpty() && m_manualChoice->GetSelection() != wxNOT_FOUND) || (!selectedRanks.IsEmpty() && selectedStops.IsEmpty())) {
		m_doImportBtn->Enable(true);
		return true;
	} else {
		m_doImportBtn->Enable(false);
		return false;
	}
}
