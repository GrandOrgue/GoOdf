/*
 * Organ.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo
 *
 * GOODF is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General wxLicense as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOODF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General wxLicense for more details.
 *
 * You should have received a copy of the GNU General wxLicense
 * along with GOODF.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo DOT se
 */

#include "Organ.h"
#include "GOODFFunctions.h"

Organ::Organ() {
	// Initialize a new blank organ
	m_odfRoot = wxEmptyString;
	m_churchName = wxEmptyString;
	m_churchAddress = wxEmptyString;
	m_organBuilder = wxEmptyString;
	m_organBuildDate = wxEmptyString;
	m_organComments = wxEmptyString;
	m_recordingDetails = wxEmptyString;
	m_infoFilename = wxEmptyString;
	m_divisionalsStoreIntermanualCouplers = true;
	m_divisionalsStoreIntramanualCouplers = true;
	m_divisionalsStoreTremulants = true;
	m_generalsStoreDivisionalCouplers = true;
	m_combinationsStoreNonDisplayedDrawstops = true;
	m_hasPedals = false;
	m_amplitudeLevel = 100.0f;
	m_gain = 0.0f;
	m_pitchTuning = 0.0f;
	m_trackerDelay = 0;
	populateSetterElements();
	updateOrganElements();

	// All organs should have a default panel [Panel000]
	GoPanel p;
	p.setName(wxT("Main Panel"));
	this->addPanel(p);
}

Organ::~Organ() {

}

void Organ::readOrgan() {

}

void Organ::writeOrgan(wxTextFile *outFile) {
	// Header of odf file
	outFile->AddLine(wxT("[Organ]"));
	outFile->AddLine(wxT("ChurchName=") + m_churchName);
	outFile->AddLine(wxT("ChurchAddress=") + m_churchAddress);
	outFile->AddLine(wxT("OrganBuilder=") + m_organBuilder);
	outFile->AddLine(wxT("OrganBuildDate=") + m_organBuildDate);
	outFile->AddLine(wxT("OrganComments=") + m_organComments);
	outFile->AddLine(wxT("RecordingDetails=") + m_recordingDetails);
	if (m_infoFilename != wxEmptyString)
		outFile->AddLine(wxT("InfoFileName=") + m_infoFilename);
	unsigned nbMan = m_Manuals.size();
	if (m_hasPedals) {
		nbMan--;
		outFile->AddLine(wxT("NumberOfManuals=") + wxString::Format(wxT("%u"), nbMan));
		outFile->AddLine(wxT("HasPedals=Y"));
	} else {
		outFile->AddLine(wxT("NumberOfManuals=") + wxString::Format(wxT("%u"), nbMan));
		outFile->AddLine(wxT("HasPedals=N"));
	}
	unsigned nbEnc = m_Enclosures.size();
	outFile->AddLine(wxT("NumberOfEnclosures=") + wxString::Format(wxT("%u"), nbEnc));
	unsigned nbTrem = m_Tremulants.size();
	outFile->AddLine(wxT("NumberOfTremulants=") + wxString::Format(wxT("%u"), nbTrem));
	unsigned nbWind = m_Windchestgroups.size();
	outFile->AddLine(wxT("NumberOfWindchestGroups=") + wxString::Format(wxT("%u"), nbWind));
	unsigned nbRevPistons = m_ReversiblePistons.size();
	outFile->AddLine(wxT("NumberOfReversiblePistons=") + wxString::Format(wxT("%u"), nbRevPistons));
	unsigned nbGenerals = m_Generals.size();
	outFile->AddLine(wxT("NumberOfGenerals=") + wxString::Format(wxT("%u"), nbGenerals));
	unsigned nbDivCplrs = m_DivisionalCouplers.size();
	outFile->AddLine(wxT("NumberOfDivisionalCouplers=") + wxString::Format(wxT("%u"), nbDivCplrs));
	// The number of panels is the additional panels, not the main [Panel000]
	unsigned nbPanels = m_Panels.size() - 1;
	outFile->AddLine(wxT("NumberOfPanels=") + wxString::Format(wxT("%u"), nbPanels));
	unsigned nbSwitches = m_Switches.size();
	if (nbSwitches)
		outFile->AddLine(wxT("NumberOfSwitches=") + wxString::Format(wxT("%u"), nbSwitches));
	unsigned nbRanks = m_Ranks.size();
	if (nbRanks)
		outFile->AddLine(wxT("NumberOfRanks=") + wxString::Format(wxT("%u"), nbRanks));
	if (m_divisionalsStoreIntermanualCouplers)
		outFile->AddLine(wxT("DivisionalsStoreIntermanualCouplers=Y"));
	else
		outFile->AddLine(wxT("DivisionalsStoreIntermanualCouplers=N"));
	if (m_divisionalsStoreIntramanualCouplers)
		outFile->AddLine(wxT("DivisionalsStoreIntramanualCouplers=Y"));
	else
		outFile->AddLine(wxT("DivisionalsStoreIntramanualCouplers=N"));
	if (m_divisionalsStoreTremulants)
		outFile->AddLine(wxT("DivisionalsStoreTremulants=Y"));
	else
		outFile->AddLine(wxT("DivisionalsStoreTremulants=N"));
	if (m_generalsStoreDivisionalCouplers)
		outFile->AddLine(wxT("GeneralsStoreDivisionalCouplers=Y"));
	else
		outFile->AddLine(wxT("GeneralsStoreDivisionalCouplers=N"));
	if (!m_combinationsStoreNonDisplayedDrawstops)
		outFile->AddLine(wxT("CombinationsStoreNonDisplayedDrawstops=N"));
	if (m_amplitudeLevel != 100)
		outFile->AddLine(wxT("AmplitudeLevel=") + wxString::Format(wxT("%f"), m_amplitudeLevel));
	if (m_gain != 0)
		outFile->AddLine(wxT("Gain=") + wxString::Format(wxT("%f"), m_gain));
	if (m_pitchTuning != 0)
		outFile->AddLine(wxT("PitchTuning=") + wxString::Format(wxT("%f"), m_pitchTuning));
	outFile->AddLine(wxT(""));

	// counter used in all the ranged loops below
	unsigned i = 1;

	// Enclosures
	for (auto& enclosure :  m_Enclosures) {
		wxString encIdentifier = wxT("[Enclosure") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(encIdentifier);
		enclosure.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Tremulants
	i = 1;
	for (auto& tremulant : m_Tremulants) {
		wxString tremId = wxT("[Tremulant") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(tremId);
		tremulant.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Windchestgroups
	i = 1;
	for (auto& windchest : m_Windchestgroups) {
		wxString wcId = wxT("[WindchestGroup") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(wcId);
		windchest.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Couplers
	i = 1;
	for (auto& coupler : m_Couplers) {
		wxString cplId = wxT("[Coupler") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(cplId);
		coupler.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Reversible pistons
	i = 1;
	for (auto& piston : m_ReversiblePistons) {
		wxString pId = wxT("[ReversiblePiston") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(pId);
		piston.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Divisional couplers
	i = 1;
	for (auto& divCplr : m_DivisionalCouplers) {
		wxString divId = wxT("[DivisionalCoupler") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(divId);
		divCplr.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Manuals
	if (m_hasPedals)
		i = 0;
	else
		i = 1;
	for (auto& manual : m_Manuals) {
		wxString manId = wxT("[Manual") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(manId);
		manual.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Switches
	i = 1;
	for (auto& sw : m_Switches) {
		wxString switchId = wxT("[Switch") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(switchId);
		sw.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Ranks
	i = 1;
	for (auto& rank : m_Ranks) {
		wxString rankId = wxT("[Rank") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(rankId);
		rank.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Stops
	i = 1;
	for (auto& stop : m_Stops) {
		wxString stopId = wxT("[Stop") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(stopId);
		stop.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Divisionals
	i = 1;
	for (auto& divisional : m_Divisionals) {
		wxString divId = wxT("[Divisional") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(divId);
		divisional.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Generals
	i = 1;
	for (auto& general : m_Generals) {
		wxString genId = wxT("[General") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(genId);
		general.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	// Panels
	i = 0;
	for (auto& pan : m_Panels) {
		wxString panelId = wxT("[Panel") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(panelId);
		pan.write(outFile, i);
		outFile->AddLine(wxT(""));
		i++;
	}
}

float Organ::getAmplitudeLevel() {
	return m_amplitudeLevel;
}

void Organ::setAmplitudeLevel(float amplitudeLevel) {
	m_amplitudeLevel = amplitudeLevel;
}

wxString Organ::getChurchAddress() {
	return m_churchAddress;
}

void Organ::setChurchAddress(wxString churchAddress) {
	m_churchAddress = churchAddress;
}

wxString Organ::getChurchName() {
	return m_churchName;
}

void Organ::setChurchName(wxString mChurchName) {
	m_churchName = mChurchName;
}

bool Organ::doesCombinationsStoreNonDisplayedDrawstops() {
	return m_combinationsStoreNonDisplayedDrawstops;
}

void Organ::setCombinationsStoreNonDisplayedDrawstops(bool combinationsStoreNonDisplayedDrawstops) {
	m_combinationsStoreNonDisplayedDrawstops = combinationsStoreNonDisplayedDrawstops;
}

bool Organ::doesDivisionalsStoreIntermanualCouplers() {
	return m_divisionalsStoreIntermanualCouplers;
}

void Organ::setDivisionalsStoreIntermanualCouplers(
		bool divisionalsStoreIntermanualCouplers) {
	m_divisionalsStoreIntermanualCouplers =
			divisionalsStoreIntermanualCouplers;
}

bool Organ::doesDivisionalsStoreIntramanualCouplers() {
	return m_divisionalsStoreIntramanualCouplers;
}

void Organ::setDivisionalsStoreIntramanualCouplers(
		bool divisionalsStoreIntramanualCouplers) {
	m_divisionalsStoreIntramanualCouplers =
			divisionalsStoreIntramanualCouplers;
}

bool Organ::doesDivisionalsStoreTremulants() {
	return m_divisionalsStoreTremulants;
}

void Organ::setDivisionalsStoreTremulants(bool divisionalsStoreTremulants) {
	m_divisionalsStoreTremulants = divisionalsStoreTremulants;
}

float Organ::getGain() {
	return m_gain;
}

void Organ::setGain(float gain) {
	m_gain = gain;
}

bool Organ::doesGeneralsStoreDivisionalCouplers() {
	return m_generalsStoreDivisionalCouplers;
}

void Organ::setGeneralsStoreDivisionalCouplers(
		bool generalsStoreDivisionalCouplers) {
	m_generalsStoreDivisionalCouplers = generalsStoreDivisionalCouplers;
}

bool Organ::doesHavePedals() {
	return m_hasPedals;
}

void Organ::setHasPedals(bool hasPedals) {
	m_hasPedals = hasPedals;
}

wxString Organ::getInfoFilename() {
	return m_infoFilename;
}

void Organ::setInfoFilename(wxString infoFilename) {
	m_infoFilename = infoFilename;
}

wxString Organ::getOrganBuildDate() {
	return m_organBuildDate;
}

void Organ::setOrganBuildDate(wxString organBuildDate) {
	m_organBuildDate = organBuildDate;
}

wxString Organ::getOrganBuilder() {
	return m_organBuilder;
}

void Organ::setOrganBuilder(wxString organBuilder) {
	m_organBuilder = organBuilder;
}

wxString Organ::getOrganComments() {
	return m_organComments;
}

void Organ::setOrganComments(wxString mOrganComments) {
	m_organComments = mOrganComments;
}

float Organ::getPitchTuning() {
	return m_pitchTuning;
}

void Organ::setPitchTuning(float pitchTuning) {
	m_pitchTuning = pitchTuning;
}

wxString Organ::getRecordingDetails() {
	return m_recordingDetails;
}

void Organ::setRecordingDetails(wxString recordingDetails) {
	m_recordingDetails = recordingDetails;
}

unsigned Organ::getTrackerDelay() {
	return m_trackerDelay;
}

void Organ::setTrackerDelay(unsigned trackerDelay) {
	m_trackerDelay = trackerDelay;
}

Enclosure* Organ::getOrganEnclosureAt(unsigned index) {
	auto iterator = std::next(m_Enclosures.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfEnclosures() {
	return m_Enclosures.size();
}

unsigned Organ::getIndexOfOrganEnclosure(Enclosure *enclosure) {
	unsigned i = 0;
	bool found = false;
	for (auto& enc : m_Enclosures) {
		i++;
		if (&enc == enclosure) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addEnclosure(Enclosure enclosure) {
	m_Enclosures.push_back(enclosure);
	updateOrganElements();
}

void Organ::removeEnclosureAt(unsigned index) {
	std::list<Enclosure>::iterator it = m_Enclosures.begin();
	std::advance(it, index);
	m_Enclosures.erase(it);
	updateOrganElements();
}

Tremulant* Organ::getOrganTremulantAt(unsigned index) {
	auto iterator = std::next(m_Tremulants.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfTremulants() {
	return m_Tremulants.size();
}

unsigned Organ::getIndexOfOrganTremulant(Tremulant *tremulant) {
	unsigned i = 0;
	bool found = false;
	for (auto& trem : m_Tremulants) {
		i++;
		if (&trem == tremulant) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addTremulant(Tremulant tremulant) {
	m_Tremulants.push_back(tremulant);
	updateOrganElements();
}

void Organ::removeTremulantAt(unsigned index) {
	std::list<Tremulant>::iterator it = m_Tremulants.begin();
	std::advance(it, index);
	// the tremulant can be referenced in a reversible piston so we just reset it
	for (ReversiblePiston& rp : m_ReversiblePistons) {
		if (rp.getTremulant() == &(*it)) {
			rp.setTremulant(NULL);
			rp.setName(wxT("Empty reversible piston"));
		}
	}
	m_Tremulants.erase(it);
	updateOrganElements();
}

Windchestgroup* Organ::getOrganWindchestgroupAt(unsigned index) {
	auto iterator = std::next(m_Windchestgroups.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfWindchestgroups() {
	return m_Windchestgroups.size();
}

unsigned Organ::getIndexOfOrganWindchest(Windchestgroup *windchest) {
	unsigned i = 0;
	bool found = false;
	for (auto& wind : m_Windchestgroups) {
		i++;
		if (&wind == windchest) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addWindchestgroup(Windchestgroup windchest) {
	m_Windchestgroups.push_back(windchest);
}

void Organ::removeWindchestgroupAt(unsigned index) {
	std::list<Windchestgroup>::iterator it = m_Windchestgroups.begin();
	std::advance(it, index);
	// now we're at the windchest to remove but first we should remove it from any stop/rank/pipe that have it set
	for (Stop& s : m_Stops) {
		if (s.isUsingInternalRank()) {
			if (s.getInternalRank()->getWindchest() == &(*it)) {
				s.getInternalRank()->setOnlyRankWindchest(NULL);
			}
			for (Pipe &p : s.getInternalRank()->m_pipes) {
				if (p.windchest == &(*it))
					p.windchest = NULL;
			}
		}
	}
	for (Rank& r : m_Ranks) {
		if (r.getWindchest() == &(*it)) {
			r.setOnlyRankWindchest(NULL);
		}
		for (Pipe &p : r.m_pipes) {
			if (p.windchest == &(*it))
				p.windchest = NULL;
		}
	}
	m_Windchestgroups.erase(it);
}

GoSwitch* Organ::getOrganSwitchAt(unsigned index) {
	auto iterator = std::next(m_Switches.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfSwitches() {
	return m_Switches.size();
}

void Organ::addSwitch(GoSwitch theSwitch) {
	m_Switches.push_back(theSwitch);
	updateOrganElements();
}

unsigned Organ::getIndexOfOrganSwitch(GoSwitch *switchToFind) {
	unsigned i = 0;
	bool found = false;
	for (auto& sw : m_Switches) {
		i++;
		if (&sw == switchToFind) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::removeSwitchAt(unsigned index) {
	std::list<GoSwitch>::iterator it = m_Switches.begin();
	std::advance(it, index);
	// now that we are at the switch to remove we should make sure to remove it from any manual that have it
	// also remove from any gui element on any panel and any general that reference it
	for (unsigned i = 0; i < m_Manuals.size(); i++) {
		if (getOrganManualAt(i)->hasGoSwitchReference(&(*it)))
			getOrganManualAt(i)->removeGoSwitch(&(*it));
	}
	for (unsigned i = 0; i < m_Panels.size(); i++) {
		if (getOrganPanelAt(i)->hasItemAsGuiElement(&(*it))) {
			getOrganPanelAt(i)->removeItemFromPanel(&(*it));
			::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
		}
	}
	for (General& g : m_Generals) {
		if (g.hasSwitch(&(*it))) {
			g.removeSwitch(&(*it));
		}
	}
	// the switch can be referenced in a reversible piston so we just reset it
	for (ReversiblePiston& rp : m_ReversiblePistons) {
		if (rp.getSwitch() == &(*it)) {
			rp.setSwitch(NULL);
			rp.setName(wxT("Empty reversible piston"));
		}
	}
	// a switch can be referenced in any drawstop related item, including other switches, so they must be removed from them too
	for (GoSwitch& sw : m_Switches) {
		if (sw.hasSwitchReference(&(*it))) {
			sw.removeSwitchReference(&(*it));
		}
	}
	for (DivisionalCoupler& cplr : m_DivisionalCouplers) {
		if (cplr.hasSwitchReference(&(*it))) {
			cplr.removeSwitchReference(&(*it));
		}
	}
	m_Switches.erase(it);
	updateOrganElements();
}

Rank* Organ::getOrganRankAt(unsigned index) {
	auto iterator = std::next(m_Ranks.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfRanks() {
	return m_Ranks.size();
}

unsigned Organ::getIndexOfOrganRank(Rank *rank) {
	unsigned i = 0;
	bool found = false;
	for (auto& rk : m_Ranks) {
		i++;
		if (&rk == rank) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addRank(Rank rank) {
	m_Ranks.push_back(rank);
}

void Organ::removeRankAt(unsigned index) {
	std::list<Rank>::iterator it = m_Ranks.begin();
	std::advance(it, index);
	m_Ranks.erase(it);
}

Stop* Organ::getOrganStopAt(unsigned index) {
	auto iterator = std::next(m_Stops.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfStops() {
	return m_Stops.size();
}

unsigned Organ::getIndexOfOrganStop(Stop *stop) {
	unsigned i = 0;
	bool found = false;
	for (auto& s : m_Stops) {
		i++;
		if (&s == stop) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addStop(Stop stop) {
	m_Stops.push_back(stop);
	updateOrganElements();
}

void Organ::removeStopAt(unsigned index) {
	std::list<Stop>::iterator it = m_Stops.begin();
	std::advance(it, index);
	// the stop can be referenced in a reversible piston so we just reset it
	for (ReversiblePiston& rp : m_ReversiblePistons) {
		if (rp.getStop() == &(*it)) {
			rp.setStop(NULL);
			rp.setName(wxT("Empty reversible piston"));
		}
	}
	m_Stops.erase(it);
	updateOrganElements();
}

void Organ::removeStop(Stop *stop) {
	unsigned index = 0;
	for (Stop& s : m_Stops) {
		if (&s == stop) {
			// if any gui element exist for this stop in any panel it should be removed
			for (unsigned i = 0; i < m_Panels.size(); i++) {
				if (getOrganPanelAt(i)->hasItemAsGuiElement(stop)) {
					getOrganPanelAt(i)->removeItemFromPanel(stop);
					::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
				}
			}
			// this stop should also be removed from any general
			for (General& g : m_Generals) {
				if (g.hasStop(stop)) {
					g.removeStop(stop);
				}
			}
			removeStopAt(index);
			break;
		}
		index++;
	}
	updateOrganElements();
}

wxString Organ::getOdfRoot() {
	return m_odfRoot;
}

void Organ::setOdfRoot(wxString root) {
	m_odfRoot = root;
}

void Organ::removeReferenceToRankInStops(Rank *rank) {
	for (Stop& s : m_Stops) {
		if (s.hasRankReference(rank))
			s.removeRankReference(rank);
	}
}

Manual* Organ::getOrganManualAt(unsigned index) {
	auto iterator = std::next(m_Manuals.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfManuals() {
	return m_Manuals.size();
}

unsigned Organ::getIndexOfOrganManual(Manual *manual) {
	unsigned i = 0;
	bool found = false;
	for (auto& m : m_Manuals) {
		if (!m_hasPedals)
			i++;
		if (&m == manual) {
			found = true;
	    	break;
	    }
		if (m_hasPedals)
			i++;
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addManual(Manual manual) {
	m_Manuals.push_back(manual);
	updateOrganElements();
}

void Organ::removeManualAt(unsigned index) {
	std::list<Manual>::iterator it = m_Manuals.begin();
	std::advance(it, index);
	// remove the manual from any divisional coupler too
	for (DivisionalCoupler& cplr : m_DivisionalCouplers) {
		if (cplr.hasManualReference(&(*it))) {
			cplr.removeManualReference(&(*it));
		}
	}
	m_Manuals.erase(it);
	updateOrganElements();
}

Coupler* Organ::getOrganCouplerAt(unsigned index) {
	auto iterator = std::next(m_Couplers.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfCouplers() {
	return m_Couplers.size();
}

unsigned Organ::getIndexOfOrganCoupler(Coupler *coupler) {
	unsigned i = 0;
	bool found = false;
	for (auto& c : m_Couplers) {
		i++;
		if (&c == coupler) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addCoupler(Coupler coupler) {
	m_Couplers.push_back(coupler);
	updateOrganElements();
}

void Organ::removeCouplerAt(unsigned index) {
	std::list<Coupler>::iterator it = m_Couplers.begin();
	std::advance(it, index);
	// the coupler can be referenced in a reversible piston so we just reset it
	for (ReversiblePiston& rp : m_ReversiblePistons) {
		if (rp.getCoupler() == &(*it)) {
			rp.setCoupler(NULL);
			rp.setName(wxT("Empty reversible piston"));
		}
	}
	m_Couplers.erase(it);
	updateOrganElements();
}

void Organ::removeCoupler(Coupler *coupler) {
	unsigned index = 0;
	for (Coupler& c : m_Couplers) {
		if (&c == coupler) {
			// if any gui element exist for this coupler in any panel it should be removed
			for (unsigned i = 0; i < m_Panels.size(); i++) {
				if (getOrganPanelAt(i)->hasItemAsGuiElement(coupler)) {
					getOrganPanelAt(i)->removeItemFromPanel(coupler);
					::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
				}
			}
			// this coupler should also be removed from any general
			for (General& g : m_Generals) {
				if (g.hasCoupler(coupler)) {
					g.removeCoupler(coupler);
				}
			}
			removeCouplerAt(index);
			break;
		}
		index++;
	}
	updateOrganElements();
}

Divisional* Organ::getOrganDivisionalAt(unsigned index) {
	auto iterator = std::next(m_Divisionals.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfDivisionals() {
	return m_Divisionals.size();
}

unsigned Organ::getIndexOfOrganDivisional(Divisional *divisional) {
	unsigned i = 0;
	bool found = false;
	for (auto& div : m_Divisionals) {
		i++;
		if (&div == divisional) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addDivisional(Divisional divisional) {
	m_Divisionals.push_back(divisional);
	updateOrganElements();
}

void Organ::removeDivisionalAt(unsigned index) {
	std::list<Divisional>::iterator it = m_Divisionals.begin();
	std::advance(it, index);
	m_Divisionals.erase(it);
	updateOrganElements();
}

void Organ::removeDivisional(Divisional *divisional) {
	unsigned index = 0;
	for (Divisional& d : m_Divisionals) {
		if (&d == divisional) {
			// if any gui element exist for this divisional in any panel it should be removed
			for (unsigned i = 0; i < m_Panels.size(); i++) {
				if (getOrganPanelAt(i)->hasItemAsGuiElement(divisional)) {
					getOrganPanelAt(i)->removeItemFromPanel(divisional);
					::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
				}
			}
			removeDivisionalAt(index);
			break;
		}
		index++;
	}
	updateOrganElements();
}

DivisionalCoupler* Organ::getOrganDivisionalCouplerAt(unsigned index) {
	auto iterator = std::next(m_DivisionalCouplers.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfOrganDivisionalCouplers() {
	return m_DivisionalCouplers.size();
}

unsigned Organ::getIndexOfOrganDivisionalCoupler(DivisionalCoupler *divCplr) {
	unsigned i = 0;
	bool found = false;
	for (auto& divCp : m_DivisionalCouplers) {
		i++;
		if (&divCp == divCplr) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addDivisionalCoupler(DivisionalCoupler divCplr) {
	m_DivisionalCouplers.push_back(divCplr);
	updateOrganElements();
}

void Organ::removeDivisionalCouplerAt(unsigned index) {
	std::list<DivisionalCoupler>::iterator it = m_DivisionalCouplers.begin();
	std::advance(it, index);
	// if any gui element exist for this divisional coupler in any panel it should be removed
	for (unsigned i = 0; i < m_Panels.size(); i++) {
		if (getOrganPanelAt(i)->hasItemAsGuiElement(&(*it))) {
			getOrganPanelAt(i)->removeItemFromPanel(&(*it));
			::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
		}
	}
	// this divisonal coupler should also be removed from any general
	for (General& g : m_Generals) {
		if (g.hasDivisionalCoupler(&(*it))) {
			g.removeDivisionalCoupler(&(*it));
		}
	}
	m_DivisionalCouplers.erase(it);
	updateOrganElements();
}

void Organ::removeDivisionalCoupler(DivisionalCoupler *divCplr) {
	for (auto it = m_DivisionalCouplers.begin(); it != m_DivisionalCouplers.end();) {
		if (&(*it) == divCplr) {
			// remove any gui representations first
			for (unsigned i = 0; i < m_Panels.size(); i++) {
				if (getOrganPanelAt(i)->hasItemAsGuiElement(&(*it))) {
					getOrganPanelAt(i)->removeItemFromPanel(&(*it));
					::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
				}
			}
			it == m_DivisionalCouplers.erase(it);
		} else {
			++it;
		}
	}
	updateOrganElements();
}

General* Organ::getOrganGeneralAt(unsigned index) {
	auto iterator = std::next(m_Generals.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfGenerals() {
	return m_Generals.size();
}

unsigned Organ::getIndexOfOrganGeneral(General *general) {
	unsigned i = 0;
	bool found = false;
	for (auto& gen : m_Generals) {
		i++;
		if (&gen == general) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addGeneral(General general) {
	m_Generals.push_back(general);
	updateOrganElements();
}

void Organ::removeGeneralAt(unsigned index) {
	std::list<General>::iterator it = m_Generals.begin();
	std::advance(it, index);
	// remove any gui representations first
	for (unsigned i = 0; i < m_Panels.size(); i++) {
		if (getOrganPanelAt(i)->hasItemAsGuiElement(&(*it))) {
			getOrganPanelAt(i)->removeItemFromPanel(&(*it));
			::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
		}
	}
	m_Generals.erase(it);
	updateOrganElements();
}

void Organ::removeGeneral(General *general) {
	for (auto it = m_Generals.begin(); it != m_Generals.end();) {
		if (&(*it) == general) {
			// remove any gui representations first
			for (unsigned i = 0; i < m_Panels.size(); i++) {
				if (getOrganPanelAt(i)->hasItemAsGuiElement(&(*it))) {
					getOrganPanelAt(i)->removeItemFromPanel(&(*it));
					::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
				}
			}
			it == m_Generals.erase(it);
		} else {
			++it;
		}
	}
	updateOrganElements();
}

ReversiblePiston* Organ::getReversiblePistonAt(unsigned index) {
	auto iterator = std::next(m_ReversiblePistons.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfReversiblePistons() {
	return m_ReversiblePistons.size();
}

unsigned Organ::getIndexOfReversiblePiston(ReversiblePiston *piston) {
	unsigned i = 0;
	bool found = false;
	for (auto& p : m_ReversiblePistons) {
		i++;
		if (&p == piston) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addReversiblePiston(ReversiblePiston piston) {
	m_ReversiblePistons.push_back(piston);
	updateOrganElements();
}

void Organ::removeReversiblePistonAt(unsigned index) {
	std::list<ReversiblePiston>::iterator it = m_ReversiblePistons.begin();
	std::advance(it, index);
	// if any gui element exist for this piston in any panel it should be removed
	for (unsigned i = 0; i < m_Panels.size(); i++) {
		if (getOrganPanelAt(i)->hasItemAsGuiElement(&(*it))) {
			getOrganPanelAt(i)->removeItemFromPanel(&(*it));
			::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
		}
	}
	m_ReversiblePistons.erase(it);
	updateOrganElements();
}

void Organ::removeReversiblePiston(ReversiblePiston *piston) {
	for (auto it = m_ReversiblePistons.begin(); it != m_ReversiblePistons.end();) {
		if (&(*it) == piston) {
			// if any gui element exist for this piston in any panel it should be removed
			for (unsigned i = 0; i < m_Panels.size(); i++) {
				if (getOrganPanelAt(i)->hasItemAsGuiElement(&(*it))) {
					getOrganPanelAt(i)->removeItemFromPanel(&(*it));
					::wxGetApp().m_frame->RebuildPanelGuiElementsInTree(i);
				}
			}
			it == m_ReversiblePistons.erase(it);
		} else {
			++it;
		}
	}
	updateOrganElements();
}

GoPanel* Organ::getOrganPanelAt(unsigned index) {
	auto iterator = std::next(m_Panels.begin(), index);
	return &(*iterator);
}

unsigned Organ::getNumberOfPanels() {
	return m_Panels.size();
}

unsigned Organ::getIndexOfOrganPanel(GoPanel *panel) {
	unsigned i = 0;
	bool found = false;
	for (auto& p : m_Panels) {
		i++;
		if (&p == panel) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

void Organ::addPanel(GoPanel panel) {
	m_Panels.push_back(panel);
}

void Organ::removePanelAt(unsigned index) {
	// TODO: Check index usage (the getindex function always returns +1 from list index)
	if (index > 0) {
		std::list<GoPanel>::iterator it = m_Panels.begin();
		std::advance(it, index);
		m_Panels.erase(it);
	}
}

void Organ::removePanel(GoPanel *panel) {
	// The main panel may not be removed and it will be returned as 1 from getindex
	if (getIndexOfOrganPanel(panel) > 1) {
		for (auto it = m_Panels.begin(); it != m_Panels.end();) {
			if (&(*it) == panel)
				it == m_Panels.erase(it);
			else
				++it;
		}
	}
}

void Organ::populateSetterElements() {
	if (m_setterElements.IsEmpty()) {
		m_setterElements.Add(wxT("CrescendoA"));
		m_setterElements.Add(wxT("CrescendoB"));
		m_setterElements.Add(wxT("CrescendoC"));
		m_setterElements.Add(wxT("CrescendoD"));
		m_setterElements.Add(wxT("CrescendoPrev"));
		m_setterElements.Add(wxT("CrescendoNext"));
		m_setterElements.Add(wxT("CrescendoCurrent"));
		m_setterElements.Add(wxT("CrescendoLabel"));
		m_setterElements.Add(wxT("Current"));
		m_setterElements.Add(wxT("Delete"));
		m_setterElements.Add(wxT("Full"));
		m_setterElements.Add(wxT("GC"));
		m_setterElements.Add(wxT("GeneralXX")); // XX can have values 01 - 50
		m_setterElements.Add(wxT("GeneralLabel"));
		m_setterElements.Add(wxT("GeneralNext"));
		m_setterElements.Add(wxT("GeneralPrev"));
		m_setterElements.Add(wxT("Home"));
		m_setterElements.Add(wxT("Insert"));
		m_setterElements.Add(wxT("L0"));
		m_setterElements.Add(wxT("L1"));
		m_setterElements.Add(wxT("L2"));
		m_setterElements.Add(wxT("L3"));
		m_setterElements.Add(wxT("L4"));
		m_setterElements.Add(wxT("L5"));
		m_setterElements.Add(wxT("L6"));
		m_setterElements.Add(wxT("L7"));
		m_setterElements.Add(wxT("L8"));
		m_setterElements.Add(wxT("L9"));
		// m_setterElements.Add(wxT("Label")); This would be for old style panels and is replaced by SequencerLabel now
		m_setterElements.Add(wxT("M100"));
		m_setterElements.Add(wxT("M10"));
		m_setterElements.Add(wxT("M1"));
		m_setterElements.Add(wxT("Next"));
		m_setterElements.Add(wxT("P1"));
		m_setterElements.Add(wxT("P10"));
		m_setterElements.Add(wxT("P100"));
		m_setterElements.Add(wxT("PitchLabel"));
		m_setterElements.Add(wxT("PitchM1"));
		m_setterElements.Add(wxT("PitchM10"));
		m_setterElements.Add(wxT("PitchM100"));
		m_setterElements.Add(wxT("PitchP1"));
		m_setterElements.Add(wxT("PitchP10"));
		m_setterElements.Add(wxT("PitchP100"));
		m_setterElements.Add(wxT("Prev"));
		m_setterElements.Add(wxT("Regular"));
		m_setterElements.Add(wxT("Save"));
		m_setterElements.Add(wxT("Scope"));
		m_setterElements.Add(wxT("Scoped"));
		m_setterElements.Add(wxT("Set"));
		m_setterElements.Add(wxT("SetterXXXDivisionalYYY")); // XXX for manual code and YYY for divisional number starting from 000 as 1
		m_setterElements.Add(wxT("SetterXXXDivisionalPrevBank"));
		m_setterElements.Add(wxT("SetterXXXDivisionalNextBank"));
		m_setterElements.Add(wxT("SetterXXXDivisionalBank"));
		m_setterElements.Add(wxT("SequencerLabel"));
		m_setterElements.Add(wxT("Swell"));
		m_setterElements.Add(wxT("TemperamentLabel"));
		m_setterElements.Add(wxT("TemperamentNext"));
		m_setterElements.Add(wxT("TemperamentPrev"));
		m_setterElements.Add(wxT("TransposeDown"));
		m_setterElements.Add(wxT("TransposeLabel"));
		m_setterElements.Add(wxT("TransposeUp"));
	}
}

void Organ::updateOrganElements() {
	if (!m_organElements.IsEmpty())
		m_organElements.Empty();

	// The organElements arrayString always contain all available elements in the exact same order
	// so that one can find out what precise element this is so that its pointer can be found
	// just using the index

	// Manuals first
	for (Manual m : m_Manuals) {
		m_organElements.Add(m.getName() + wxT(" (Manual)"));
	}

	// Stops
	for (Stop s : m_Stops) {
		m_organElements.Add(s.getName() + wxT(" (Stop in ") + s.getOwningManual()->getName() + wxT(")"));
	}

	// Couplers
	for (Coupler c : m_Couplers) {
		m_organElements.Add(c.getName() + wxT(" (Coupler for ") + c.getOwningManual()->getName() + wxT(")"));
	}

	// Divisionals
	for (Divisional d : m_Divisionals) {
		m_organElements.Add(d.getName() + wxT(" (Divisional in ") + d.getOwningManual()->getName() + wxT(")"));
	}

	// Enclosures
	for (Enclosure e : m_Enclosures) {
		m_organElements.Add(e.getName() + wxT(" (Enclosure)"));
	}

	// Tremulants
	for (Tremulant t : m_Tremulants) {
		m_organElements.Add(t.getName() + wxT(" (Tremulant)"));
	}

	// Switches
	for (GoSwitch sw : m_Switches) {
		m_organElements.Add(sw.getName() + wxT(" (Switch)"));
	}

	// Reversible pistons
	for (ReversiblePiston p : m_ReversiblePistons) {
		m_organElements.Add(p.getName() + wxT(" (Reversible piston)"));
	}

	// Divisional couplers
	for (DivisionalCoupler divC : m_DivisionalCouplers) {
		m_organElements.Add(divC.getName() + wxT(" (Divisional coupler)"));
	}

	// Generals
	for (General g : m_Generals) {
		m_organElements.Add(g.getName() + wxT(" (General)"));
	}
}

std::pair<wxString, int> Organ::getTypeAndIndexOfElement(int index) {
	wxString elementType = wxEmptyString;
	int returnIndex = -1;

	int manuals = m_Manuals.size();
	int stops = m_Stops.size();
	int couplers = m_Couplers.size();
	int divisionals = m_Divisionals.size();
	int enclosures = m_Enclosures.size();
	int tremulants = m_Tremulants.size();
	int switches = m_Switches.size();
	int revPistons = m_ReversiblePistons.size();
	int divCouplers = m_DivisionalCouplers.size();
	int generals = m_Generals.size();

	if (index < manuals) {
		// The selected index is a manual
		elementType = wxT("Manual");
		returnIndex = index;
	} else if (index >= manuals && index < (manuals + stops)) {
		// the selected index is a stop
		elementType = wxT("Stop");
		returnIndex = index - manuals;
	} else if (index >= (manuals + stops) && index < (manuals + stops + couplers)) {
		// the selected index is a coupler
		elementType = wxT("Coupler");
		returnIndex = index - (manuals + stops);
	} else if (index >= (manuals + stops + couplers) && index < (manuals + stops + couplers + divisionals)) {
		// the selected index is a divisional
		elementType = wxT("Divisional");
		returnIndex = index - (manuals + stops + couplers);
	} else if (index >= (manuals + stops + couplers + divisionals) &&
		index < (manuals + stops + couplers + divisionals + enclosures)) {
		// the selected index is an enclosure
		elementType = wxT("Enclosure");
		returnIndex = index - (manuals + stops + couplers + divisionals);
	} else if (index >= (manuals + stops + couplers + divisionals + enclosures) &&
		index < (manuals + stops + couplers + divisionals + enclosures + tremulants)) {
		// the selected index is a tremulant
		elementType = wxT("Tremulant");
		returnIndex = index - (manuals + stops + couplers + divisionals + enclosures);
	} else if (index >= (manuals + stops + couplers + divisionals + enclosures + tremulants) &&
		index < (manuals + stops + couplers + divisionals + enclosures + tremulants + switches)) {
		// the selected index is a switch
		elementType = wxT("Switch");
		returnIndex = index - (manuals + stops + couplers + divisionals + enclosures + tremulants);
	} else if (index >= (manuals + stops + couplers + divisionals + enclosures + tremulants + switches) &&
		index < (manuals + stops + couplers + divisionals + enclosures + tremulants + switches + revPistons)) {
		// the selected index is a reversible piston
		elementType = wxT("ReversiblePiston");
		returnIndex = index - (manuals + stops + couplers + divisionals + enclosures + tremulants + switches);
	} else if (index >= (manuals + stops + couplers + divisionals + enclosures + tremulants + switches + revPistons) &&
		index < (manuals + stops + couplers + divisionals + enclosures + tremulants + switches + revPistons + divCouplers)) {
		// the selected index is a divisional coupler
		elementType = wxT("DivisionalCoupler");
		returnIndex = index - (manuals + stops + couplers + divisionals + enclosures + tremulants + switches + revPistons);
	} else if (index >= (manuals + stops + couplers + divisionals + enclosures + tremulants + switches + revPistons + divCouplers) &&
		index < (manuals + stops + couplers + divisionals + enclosures + tremulants + switches + revPistons + divCouplers + generals)) {
		// the selected index is a general
		elementType = wxT("General");
		returnIndex = index - (manuals + stops + couplers + divisionals + enclosures + tremulants + switches + revPistons + divCouplers);
	}

	return std::make_pair(elementType, returnIndex);
}

const wxArrayString& Organ::getSetterElements() const {
	return m_setterElements;
}

const wxArrayString& Organ::getOrganElements() const {
	return m_organElements;
}

void Organ::organElementHasChanged() {
	updateOrganElements();
}
