/*
 * Organ.h is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General wxLicense as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General wxLicense for more details.
 *
 * You should have received a copy of the GNU General wxLicense
 * along with GoOdf. If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo DOT se
 */

#ifndef ORGAN_H
#define ORGAN_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <list>
#include "Enclosure.h"
#include "Tremulant.h"
#include "Windchestgroup.h"
#include "GoSwitch.h"
#include "Rank.h"
#include "Stop.h"
#include "Manual.h"
#include "Coupler.h"
#include "Divisional.h"
#include "DivisionalCoupler.h"
#include "General.h"
#include "ReversiblePiston.h"
#include "GoPanel.h"

class Organ {
public:
	Organ();
	~Organ();

	void writeOrgan(wxTextFile *outFile);

	float getAmplitudeLevel();
	void setAmplitudeLevel(float amplitudeLevel);
	wxString getChurchAddress();
	void setChurchAddress(wxString churchAddress);
	wxString getChurchName();
	void setChurchName(wxString churchName);
	bool doesCombinationsStoreNonDisplayedDrawstops();
	void setCombinationsStoreNonDisplayedDrawstops(bool combinationsStoreNonDisplayedDrawstops);
	bool doesDivisionalsStoreIntermanualCouplers();
	void setDivisionalsStoreIntermanualCouplers(bool divisionalsStoreIntermanualCouplers);
	bool doesDivisionalsStoreIntramanualCouplers();
	void setDivisionalsStoreIntramanualCouplers(bool divisionalsStoreIntramanualCouplers);
	bool doesDivisionalsStoreTremulants();
	void setDivisionalsStoreTremulants(bool divisionalsStoreTremulants);
	float getGain();
	void setGain(float gain);
	bool doesGeneralsStoreDivisionalCouplers();
	void setGeneralsStoreDivisionalCouplers(bool generalsStoreDivisionalCouplers);
	bool doesHavePedals();
	void setHasPedals(bool hasPedals, bool isParsing = false);
	wxString getInfoFilename();
	void setInfoFilename(wxString infoFilename);
	wxString getOrganBuildDate();
	void setOrganBuildDate(wxString organBuildDate);
	wxString getOrganBuilder();
	void setOrganBuilder(wxString organBuilder);
	wxString getOrganComments();
	void setOrganComments(wxString organComments);
	float getPitchTuning();
	void setPitchTuning(float pitchTuning);
	wxString getRecordingDetails();
	void setRecordingDetails(wxString recordingDetails);
	unsigned getTrackerDelay();
	void setTrackerDelay(unsigned trackerDelay);
	float getPitchCorrection();
	void setPitchCorrection(float pitchCorrection);
	bool getIsPercussive();
	void setIsPercussive(bool percussive);
	bool getHasIndependentRelease();
	void setHasIndependentRelease(bool independentRel);

	Enclosure* getOrganEnclosureAt(unsigned index);
	unsigned getNumberOfEnclosures();
	unsigned getIndexOfOrganEnclosure(Enclosure *enclosure);
	void addEnclosure(Enclosure enclosure, bool isParsing = false);
	void removeEnclosureAt(unsigned index);
	Tremulant* getOrganTremulantAt(unsigned index);
	unsigned getNumberOfTremulants();
	unsigned getIndexOfOrganTremulant(Tremulant *tremulant);
	void addTremulant(Tremulant tremulant, bool isParsing = false);
	void removeTremulantAt(unsigned index);
	Windchestgroup* getOrganWindchestgroupAt(unsigned index);
	unsigned getNumberOfWindchestgroups();
	unsigned getIndexOfOrganWindchest(Windchestgroup *windchest);
	void addWindchestgroup(Windchestgroup windchest);
	void removeWindchestgroupAt(unsigned index);
	void moveWindchestgroup(int sourceIndex, int toBeforeIndex);
	GoSwitch* getOrganSwitchAt(unsigned index);
	unsigned getNumberOfSwitches();
	unsigned getIndexOfOrganSwitch(GoSwitch *switchToFind);
	void addSwitch(GoSwitch theSwitch, bool isParsing = false);
	void removeSwitchAt(unsigned index);
	void moveSwitch(int sourceIndex, int toBeforeIndex);
	Rank* getOrganRankAt(unsigned index);
	unsigned getNumberOfRanks();
	unsigned getIndexOfOrganRank(Rank *rank);
	void addRank(Rank rank);
	void removeRankAt(unsigned index);
	void moveRank(int sourceIndex, int toBeforeIndex);
	Stop* getOrganStopAt(unsigned index);
	unsigned getNumberOfStops();
	unsigned getIndexOfOrganStop(Stop *stop);
	void addStop(Stop stop, bool isParsing = false);
	void removeStopAt(unsigned index);
	void removeStop(Stop *stop);
	bool moveStop(int srcManualIdx, int srcStopIdxOnManual, int dstManualIdx, int dstStopIdxOnManual);
	wxString getOdfRoot();
	void setOdfRoot(wxString root);
	void removeReferenceToRankInStops(Rank *rank);
	void updateManualPipeReferences(int sourceIdx, int newIdx);
	void updatePipeReferencesFromPedalChoice();
	Stop* getStopFromRefString(wxString refString);
	Manual* getOrganManualAt(unsigned index);
	unsigned getNumberOfManuals();
	unsigned getIndexOfOrganManual(Manual *manual);
	void addManual(Manual manual, bool isParsing = false);
	void removeManualAt(unsigned index);
	void moveManual(int sourceIndex, int toBeforeIndex);
	Coupler* getOrganCouplerAt(unsigned index);
	unsigned getNumberOfCouplers();
	unsigned getIndexOfOrganCoupler(Coupler *coupler);
	void addCoupler(Coupler coupler, bool isParsing = false);
	void removeCouplerAt(unsigned index);
	void removeCoupler(Coupler *coupler);
	Divisional* getOrganDivisionalAt(unsigned index);
	unsigned getNumberOfDivisionals();
	unsigned getIndexOfOrganDivisional(Divisional *divisional);
	void addDivisional(Divisional divisional, bool isParsing = false);
	void removeDivisionalAt(unsigned index);
	void removeDivisional(Divisional *divisional);
	DivisionalCoupler* getOrganDivisionalCouplerAt(unsigned index);
	unsigned getNumberOfOrganDivisionalCouplers();
	unsigned getIndexOfOrganDivisionalCoupler(DivisionalCoupler *divCplr);
	void addDivisionalCoupler(DivisionalCoupler divCplr, bool isParsing = false);
	void removeDivisionalCouplerAt(unsigned index);
	void removeDivisionalCoupler(DivisionalCoupler *divCplr);
	General* getOrganGeneralAt(unsigned index);
	unsigned getNumberOfGenerals();
	unsigned getIndexOfOrganGeneral(General *general);
	void addGeneral(General general, bool isParsing = false);
	void removeGeneralAt(unsigned index);
	void removeGeneral(General *general);
	ReversiblePiston* getReversiblePistonAt(unsigned index);
	unsigned getNumberOfReversiblePistons();
	unsigned getIndexOfReversiblePiston(ReversiblePiston *piston);
	void addReversiblePiston(ReversiblePiston piston, bool isParsing = false);
	void removeReversiblePistonAt(unsigned index);
	void removeReversiblePiston(ReversiblePiston *piston);
	GoPanel* getOrganPanelAt(unsigned index);
	unsigned getNumberOfPanels();
	unsigned getIndexOfOrganPanel(GoPanel *panel);
	void addPanel(GoPanel panel);
	void removePanelAt(unsigned index);
	void removePanel(GoPanel *panel);
	void movePanel(int sourceIndex, int toBeforeIndex);
	void panelDisplayMetricsUpdate(DisplayMetrics *metrics);
	void panelApplyButtonFontName(DisplayMetrics *metrics);
	void panelApplyButtonFontSize(DisplayMetrics *metrics);
	void panelApplyLabelFontName(DisplayMetrics *metrics);
	void panelApplyLabelFontSize(DisplayMetrics *metrics);
	GoPanel* getPanelOwningDisplayMetrics(DisplayMetrics *metrics);

	const wxArrayString& getSetterElements() const;
	const wxArrayString& getOrganElements() const;
	std::pair<wxString, int> getTypeAndIndexOfElement(int index);
	void organElementHasChanged(bool isParsing = false);
	void updateRelativePipePaths();
	bool isModified();
	void setModified(bool modified);
	void doInheritLegacyXfades();
	bool isElementReferenced(GoSwitch *sw);
	void fixTrailingSpacesInStrings();

private:
	wxString m_odfRoot;
	bool m_isModified;
	// Organ properties
	wxString m_churchName;
	wxString m_churchAddress;
	wxString m_organBuilder;
	wxString m_organBuildDate;
	wxString m_organComments;
	wxString m_recordingDetails;
	wxString m_infoFilename;
	bool m_divisionalsStoreIntermanualCouplers;
	bool m_divisionalsStoreIntramanualCouplers;
	bool m_divisionalsStoreTremulants;
	bool m_generalsStoreDivisionalCouplers;
	bool m_combinationsStoreNonDisplayedDrawstops;
	bool m_hasPedals;
	float m_amplitudeLevel;
	float m_gain;
	float m_pitchTuning;
	float m_pitchCorrection;
	unsigned m_trackerDelay;
	bool m_isPercussive;
	bool m_hasIndependentRelease;

	// The stuff the organ has
	wxArrayString m_setterElements;
	std::list<Enclosure> m_Enclosures;
	std::list<Tremulant> m_Tremulants;
	std::list<Windchestgroup> m_Windchestgroups;
	std::list<GoSwitch> m_Switches;
	std::list<Rank> m_Ranks;
	std::list<Stop> m_Stops;
	std::list<Manual> m_Manuals;
	std::list<Coupler> m_Couplers;
	std::list<Divisional> m_Divisionals;
	std::list<DivisionalCoupler> m_DivisionalCouplers;
	std::list<General> m_Generals;
	std::list<ReversiblePiston> m_ReversiblePistons;
	std::list<GoPanel> m_Panels;
	wxArrayString m_organElements;

	void populateSetterElements();
	void updateOrganElements();

};

#endif
