/*
 * Rank.cpp is part of GoOdf.
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

#include "Rank.h"
#include "GOODF.h"
#include "GOODFFunctions.h"
#include <wx/unichar.h>
#include <wx/regex.h>

Rank::Rank() {
	name = wxT("New Rank");
	firstMidiNoteNumber = 36;
	numberOfLogicalPipes = 1;
	amplitudeLevel = 100;
	gain = 0;
	pitchTuning = 0;
	trackerDelay = 0;
	harmonicNumber = 8;
	pitchCorrection = 0;
	windchest = NULL;
	percussive = ::wxGetApp().m_frame->m_organ->getIsPercussive();
	hasIndependentRelease = ::wxGetApp().m_frame->m_organ->getHasIndependentRelease();
	minVelocityVolume = 100;
	maxVelocityVolume = 100;
	acceptsRetuning = true;

	m_latestPipesRootPath = wxEmptyString;
	createDummyPipes();
}

Rank::Rank(const Rank& r) {
	name = r.name;
	firstMidiNoteNumber = r.firstMidiNoteNumber;
	numberOfLogicalPipes = r.numberOfLogicalPipes;
	amplitudeLevel = r.amplitudeLevel;
	gain = r.gain;
	pitchTuning = r.pitchTuning;
	trackerDelay = r.trackerDelay;
	harmonicNumber = r.harmonicNumber;
	pitchCorrection = r.pitchCorrection;
	windchest = r.windchest;
	percussive = r.percussive;
	hasIndependentRelease = r.hasIndependentRelease;
	minVelocityVolume = r.minVelocityVolume;
	maxVelocityVolume = r.maxVelocityVolume;
	acceptsRetuning = r.acceptsRetuning;
	m_latestPipesRootPath = r.m_latestPipesRootPath;

	for (Pipe p : r.m_pipes) {
		m_pipes.push_back(p);
	}
}

Rank::~Rank() {

}

void Rank::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + name);
	if (firstMidiNoteNumber > -1)
		outFile->AddLine(wxT("FirstMidiNoteNumber=") + wxString::Format(wxT("%i"), firstMidiNoteNumber));
	outFile->AddLine(wxT("NumberOfLogicalPipes=") + wxString::Format(wxT("%i"), numberOfLogicalPipes));
	if (amplitudeLevel != 100)
		outFile->AddLine(wxT("AmplitudeLevel=") + wxString::Format(wxT("%f"), amplitudeLevel));
	if (gain != 0)
		outFile->AddLine(wxT("Gain=") + wxString::Format(wxT("%f"), gain));
	if (pitchTuning != 0)
		outFile->AddLine(wxT("PitchTuning=") + wxString::Format(wxT("%f"), pitchTuning));
	if (trackerDelay != 0)
		outFile->AddLine(wxT("TrackerDelay=") + wxString::Format(wxT("%i"), trackerDelay));
	if (harmonicNumber != 8)
		outFile->AddLine(wxT("HarmonicNumber=") + wxString::Format(wxT("%i"), harmonicNumber));
	if (pitchCorrection != 0)
		outFile->AddLine(wxT("PitchCorrection=") + wxString::Format(wxT("%f"), pitchCorrection));
	if (windchest) {
		wxString wcRef = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganWindchest(windchest));
		outFile->AddLine(wxT("WindchestGroup=") + wcRef);
		if (percussive != windchest->getIsPercussive()) {
			if (percussive) {
				outFile->AddLine(wxT("Percussive=Y"));
				if (hasIndependentRelease != windchest->getHasIndependentRelease()) {
					if (hasIndependentRelease) {
						outFile->AddLine(wxT("HasIndependentRelease=Y"));
					} else {
						outFile->AddLine(wxT("HasIndependentRelease=N"));
					}
				}
			} else {
				outFile->AddLine(wxT("Percussive=N"));
			}
		}
	} else {
		wxLogWarning("No windchestgroup is set for rank %s! The .organ file won't be functional!", getName());
		::wxGetApp().m_frame->GetLogWindow()->Show(true);
	}
	if (minVelocityVolume != 100)
		outFile->AddLine(wxT("MinVelocityVolume=") + wxString::Format(wxT("%f"), minVelocityVolume));
	if (maxVelocityVolume != 100)
		outFile->AddLine(wxT("MaxVelocityVolume=") + wxString::Format(wxT("%f"), maxVelocityVolume));
	if (!acceptsRetuning)
		outFile->AddLine(wxT("AcceptsRetuning=N"));

	// pipes of the rank
	unsigned pipeCounter = 0;
	for (Pipe &p : m_pipes) {
		pipeCounter++;
		wxString formattedPipe = wxT("Pipe") + GOODF_functions::number_format(pipeCounter);

		p.write(outFile, formattedPipe, this);
	}
}

void Rank::writeFromStop(wxTextFile *outFile) {
	outFile->AddLine(wxT("NumberOfLogicalPipes=") + wxString::Format(wxT("%i"), numberOfLogicalPipes));
	if (amplitudeLevel != 100)
		outFile->AddLine(wxT("AmplitudeLevel=") + wxString::Format(wxT("%f"), amplitudeLevel));
	if (gain != 0)
		outFile->AddLine(wxT("Gain=") + wxString::Format(wxT("%f"), gain));
	if (pitchTuning != 0)
		outFile->AddLine(wxT("PitchTuning=") + wxString::Format(wxT("%f"), pitchTuning));
	if (trackerDelay != 0)
		outFile->AddLine(wxT("TrackerDelay=") + wxString::Format(wxT("%i"), trackerDelay));
	if (harmonicNumber != 8)
		outFile->AddLine(wxT("HarmonicNumber=") + wxString::Format(wxT("%i"), harmonicNumber));
	if (pitchCorrection != 0)
		outFile->AddLine(wxT("PitchCorrection=") + wxString::Format(wxT("%f"), pitchCorrection));
	if (windchest) {
		wxString wcRef = GOODF_functions::number_format(::wxGetApp().m_frame->m_organ->getIndexOfOrganWindchest(windchest));
		outFile->AddLine(wxT("WindchestGroup=") + wcRef);
		if (percussive != windchest->getIsPercussive()) {
			if (percussive) {
				outFile->AddLine(wxT("Percussive=Y"));
				if (hasIndependentRelease) {
					outFile->AddLine(wxT("HasIndependentRelease=Y"));
				}
			} else {
				outFile->AddLine(wxT("Percussive=N"));
			}
		}
	} else {
		wxLogWarning("No windchestgroup is set for internal rank of stop %s! The .organ file won't be functional!", getName());
		::wxGetApp().m_frame->GetLogWindow()->Show(true);
	}
	if (minVelocityVolume != 100)
		outFile->AddLine(wxT("MinVelocityVolume=") + wxString::Format(wxT("%f"), minVelocityVolume));
	if (maxVelocityVolume != 100)
		outFile->AddLine(wxT("MaxVelocityVolume=") + wxString::Format(wxT("%f"), maxVelocityVolume));
	if (!acceptsRetuning)
		outFile->AddLine(wxT("AcceptsRetuning=N"));

	// pipes of the rank
	unsigned pipeCounter = 0;
	for (Pipe &p : m_pipes) {
		pipeCounter++;
		wxString formattedPipe = wxT("Pipe") + GOODF_functions::number_format(pipeCounter);

		p.write(outFile, formattedPipe, this);
	}
}

void Rank::read(wxFileConfig *cfg, Organ *readOrgan) {
	name = cfg->Read("Name", wxEmptyString);
	int firstMIDInote = static_cast<int>(cfg->ReadLong("FirstMidiNoteNumber", 36));
	if (firstMIDInote > -1 && firstMIDInote < 257) {
		setFirstMidiNoteNumber(firstMIDInote);
	}
	int nbrPipes = static_cast<int>(cfg->ReadLong("NumberOfLogicalPipes", 1));
	if (nbrPipes > 0 && nbrPipes < 193) {
		setNumberOfLogicalPipes(nbrPipes);
	}
	float ampLvl = static_cast<float>(cfg->ReadDouble("AmplitudeLevel", 100.0f));
	if (ampLvl >= 0 && ampLvl <= 1000) {
		setAmplitudeLevel(ampLvl);
	}
	float gainValue = static_cast<float>(cfg->ReadDouble("Gain", 0.0f));
	if (gainValue >= -120 && gainValue <= 40) {
		setGain(gainValue);
	}
	float pitchT = static_cast<float>(cfg->ReadDouble("PitchTuning", 0.0f));
	if (pitchT >= -1800 && pitchT <= 1800) {
		setPitchTuning(pitchT);
	}
	int trackerDly = static_cast<int>(cfg->ReadLong("TrackerDelay", 0));
	if (trackerDly > -1 && trackerDly < 10001) {
		setTrackerDelay(trackerDly);
	}
	int harmNbr = static_cast<int>(cfg->ReadLong("HarmonicNumber", 8));
	if (harmNbr > 0 && harmNbr < 1025) {
		setHarmonicNumber(harmNbr);
	}
	float pitchCorr = static_cast<float>(cfg->ReadDouble("PitchCorrection", 0.0f));
	if (pitchCorr >= -1800 && pitchCorr <= 1800) {
		setPitchCorrection(pitchCorr);
	}
	int windchestRef = static_cast<int>(cfg->ReadLong("WindchestGroup", 0));
	if (windchestRef > 0 && windchestRef <= (int) readOrgan->getNumberOfWindchestgroups()) {
		setWindchest(readOrgan->getOrganWindchestgroupAt(windchestRef - 1));
	} else {
		wxLogWarning("No windchestgroup could be read for %s!", getName());
		::wxGetApp().m_frame->GetLogWindow()->Show(true);
	}
	wxString percussiveStr = cfg->Read("Percussive", wxEmptyString);
	setPercussive(GOODF_functions::parseBoolean(percussiveStr, false));
	if (isPercussive()) {
		wxString independentReleaseStr = cfg->Read("HasIndependentRelease", wxEmptyString);
		hasIndependentRelease = GOODF_functions::parseBoolean(independentReleaseStr, false);
	}
	float minVelocity = static_cast<float>(cfg->ReadDouble("MinVelocityVolume", 100.0f));
	if (minVelocity >= 0 && minVelocity <= 1000) {
		setMinVelocityVolume(minVelocity);
	}
	float maxVelocity = static_cast<float>(cfg->ReadDouble("MaxVelocityVolume", 100.0f));
	if (maxVelocity >= 0 && maxVelocity <= 1000) {
		setMaxVelocityVolume(maxVelocity);
	}
	wxString retuningStr = cfg->Read("AcceptsRetuning", wxEmptyString);
	setAcceptsRetuning(GOODF_functions::parseBoolean(retuningStr, true));
	if (!m_pipes.empty())
		m_pipes.clear();

	bool hadUnusualTremulants = false;
	for (int i = 0; i < numberOfLogicalPipes; i++) {
		Pipe p;
		wxString pipeNbr = wxT("Pipe") + GOODF_functions::number_format(i + 1);
		p.read(cfg, pipeNbr, this, readOrgan);
		m_pipes.push_back(p);
		if (p.hasUnusualTremulants()) {
			hadUnusualTremulants = true;
		}
	}
	if (hadUnusualTremulants) {
		GOODF_functions::logTremulantMessage(getName());
	}
}

bool Rank::doesAcceptsRetuning() const {
	return acceptsRetuning;
}

void Rank::setAcceptsRetuning(bool acceptsRetuning) {
	this->acceptsRetuning = acceptsRetuning;

	for (std::list<Pipe>::iterator pipe = m_pipes.begin(); pipe != m_pipes.end(); ++pipe) {
		pipe->acceptsRetuning = this->acceptsRetuning;
	}
}

float Rank::getAmplitudeLevel() const {
	return amplitudeLevel;
}

void Rank::setAmplitudeLevel(float amplitudeLevel) {
	this->amplitudeLevel = amplitudeLevel;
}

int Rank::getFirstMidiNoteNumber() const {
	return firstMidiNoteNumber;
}

void Rank::setFirstMidiNoteNumber(int firstMidiNoteNumber) {
	this->firstMidiNoteNumber = firstMidiNoteNumber;
}

float Rank::getGain() const {
	return gain;
}

void Rank::setGain(float gain) {
	this->gain = gain;
}

int Rank::getHarmonicNumber() const {
	return harmonicNumber;
}

void Rank::setHarmonicNumber(int harmonicNumber) {
	this->harmonicNumber = harmonicNumber;

	for (std::list<Pipe>::iterator pipe = m_pipes.begin(); pipe != m_pipes.end(); ++pipe) {
		pipe->harmonicNumber = this->harmonicNumber;
	}
}

float Rank::getMaxVelocityVolume() const {
	return maxVelocityVolume;
}

void Rank::setMaxVelocityVolume(float maxVelocityVolume) {
	this->maxVelocityVolume = maxVelocityVolume;

	for (std::list<Pipe>::iterator pipe = m_pipes.begin(); pipe != m_pipes.end(); ++pipe) {
		pipe->maxVelocityVolume = this->maxVelocityVolume;
	}
}

float Rank::getMinVelocityVolume() const {
	return minVelocityVolume;
}

void Rank::setMinVelocityVolume(float minVelocityVolume) {
	this->minVelocityVolume = minVelocityVolume;

	for (std::list<Pipe>::iterator pipe = m_pipes.begin(); pipe != m_pipes.end(); ++pipe) {
		pipe->minVelocityVolume = this->minVelocityVolume;
	}
}

const wxString& Rank::getName() const {
	return name;
}

void Rank::setName(const wxString &name) {
	this->name = name;
}

int Rank::getNumberOfLogicalPipes() const {
	return numberOfLogicalPipes;
}

void Rank::setNumberOfLogicalPipes(int numberOfLogicalPipes) {
	this->numberOfLogicalPipes = numberOfLogicalPipes;
}

bool Rank::isPercussive() const {
	return percussive;
}

void Rank::setPercussive(bool percussive) {
	this->percussive = percussive;
}

bool Rank::isIndependentRelease() {
	return hasIndependentRelease;
}

void Rank::setIndependentRelease(bool independent) {
	this->hasIndependentRelease = independent;
}

float Rank::getPitchCorrection() const {
	return pitchCorrection;
}

void Rank::setPitchCorrection(float pitchCorrection) {
	this->pitchCorrection = pitchCorrection;
}

float Rank::getPitchTuning() const {
	return pitchTuning;
}

void Rank::setPitchTuning(float pitchTuning) {
	this->pitchTuning = pitchTuning;
}

int Rank::getTrackerDelay() const {
	return trackerDelay;
}

void Rank::setTrackerDelay(int trackerDelay) {
	this->trackerDelay = trackerDelay;
}

Windchestgroup* Rank::getWindchest() {
	return windchest;
}

void Rank::setWindchest(Windchestgroup *windchest) {
	this->windchest = windchest;
	for (std::list<Pipe>::iterator pipe = m_pipes.begin(); pipe != m_pipes.end(); ++pipe) {
		pipe->windchest = this->windchest;
	}
}

void Rank::setOnlyRankWindchest(Windchestgroup *windchest) {
	this->windchest = windchest;
}

wxString Rank::getPipesRootPath() {
	return m_latestPipesRootPath;
}

void Rank::setPipesRootPath(wxString path) {
	m_latestPipesRootPath = path;
}

void Rank::readPipes(
	wxString extraAttackFolder,
	bool loadOnlyOneAttack,
	bool loadRelease,
	wxString releaseFolderPrefix,
	bool extractKeyPressTime,
	wxString tremulantFolderPrefix,
	bool loadPipesAsTremOff,
	int startPipeIdx,
	int firstMatchingNumber,
	int totalNbrOfPipes
) {
	bool organRootPathIsSet = false;
	bool hadUnusualTremulants = false;

	if (::wxGetApp().m_frame->m_organ->getOdfRoot() != wxEmptyString)
		organRootPathIsSet = true;

	wxDir pipeRoot(m_latestPipesRootPath);

	if (!pipeRoot.IsOpened())
		return;

	int count = 0;
	for (int i = startPipeIdx; i < startPipeIdx + totalNbrOfPipes; i++) {
		emptyPipeAt(i);
		Pipe *p = getPipeAt(i);
		setupPipeProperties(*p);

		wxArrayString pipeAttacks;
		wxArrayString pipeAttacksToAdd;
		wxArrayString pipeReleases;
		wxArrayString pipeReleasesToAdd;
		wxArrayString releaseFolders;
		wxArrayString tremulantFolders;
		wxArrayString allFolders;

		bool hasTremulantFolders = false;

		// first store all folders existing in root
		wxString folder;
		bool cont = pipeRoot.GetFirst(&folder, wxT("*"), wxDIR_DIRS);
		while (cont) {
			allFolders.Add(folder);
			cont = pipeRoot.GetNext(&folder);
		}

		if (!allFolders.IsEmpty()) {
			// separate release and tremulant folders
			for (unsigned j = 0; j < allFolders.GetCount(); j++) {

				if (allFolders.Item(j).Lower().Find(releaseFolderPrefix.Lower()) != wxNOT_FOUND && releaseFolderPrefix != wxEmptyString) {
					releaseFolders.Add(allFolders.Item(j));
				} else if (allFolders.Item(j).Lower().Find(tremulantFolderPrefix.Lower()) != wxNOT_FOUND && tremulantFolderPrefix != wxEmptyString) {
					tremulantFolders.Add(allFolders.Item(j));
					hasTremulantFolders = true;
				}
			}
		}

		// get attacks from root folder
		pipeRoot.GetAllFiles(
			m_latestPipesRootPath,
			&pipeAttacks,
			wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
			wxDIR_FILES
		);

		// then from possible extra attack folder
		if (extraAttackFolder != wxEmptyString) {
			pipeRoot.GetAllFiles(
				m_latestPipesRootPath + wxFILE_SEP_PATH + extraAttackFolder,
				&pipeAttacks,
				wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
				wxDIR_FILES
			);
		}

		pipeAttacks.Sort();

		// remove any file that's not ending with .wav or .wv
		onlyAddWaveFiles(pipeAttacks, pipeAttacksToAdd);

		// make sure to exactly match the MIDI number
		exactlyMatchMidiNumber(pipeAttacksToAdd, count + firstMatchingNumber);

		// if there are any matching attacks we add them
		if (!pipeAttacksToAdd.IsEmpty()) {
			for (unsigned j = 0; j < pipeAttacksToAdd.GetCount(); j++) {
				wxString relativeFileName;
				if (organRootPathIsSet)
					relativeFileName = getOnlyFileName(pipeAttacksToAdd.Item(j));
				else
					relativeFileName = pipeAttacksToAdd.Item(j);

				// create and add the attack to the pipe
				Attack a;
				a.fileName = relativeFileName;
				a.fullPath = pipeAttacksToAdd.Item(j);
				a.loadRelease = loadRelease;
				if (hasTremulantFolders || loadPipesAsTremOff)
					a.isTremulant = 0;

				p->m_attacks.push_back(a);

				if (loadOnlyOneAttack)
					break;
			}
		}

		pipeAttacks.Empty();
		pipeAttacksToAdd.Empty();

		// add extra releases if they can be found
		if (!releaseFolders.IsEmpty()) {

			releaseFolders.Sort();

			for (unsigned j = 0; j < releaseFolders.GetCount(); j++) {
				pipeRoot.GetAllFiles(
					m_latestPipesRootPath + wxFILE_SEP_PATH + releaseFolders.Item(j),
					&pipeReleases,
					wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
					wxDIR_FILES
				);

				pipeReleases.Sort();

				// remove any file that's not ending with .wav or .wv
				onlyAddWaveFiles(pipeReleases, pipeReleasesToAdd);

				// make sure to exactly match the MIDI number
				exactlyMatchMidiNumber(pipeReleasesToAdd, count + firstMatchingNumber);

				// if there are any matching releases we add them
				if (!pipeReleasesToAdd.IsEmpty()) {

					for (unsigned k = 0; k < pipeReleasesToAdd.GetCount(); k++) {
						wxString relativeFileName;
						if (organRootPathIsSet)
							relativeFileName = getOnlyFileName(pipeReleasesToAdd.Item(k));
						else
							relativeFileName = pipeReleasesToAdd.Item(k);

						// create and add the release to the pipe
						Release rel;
						rel.fileName = relativeFileName;
						rel.fullPath = pipeReleasesToAdd.Item(k);
						if (hasTremulantFolders || loadPipesAsTremOff)
							rel.isTremulant = 0;

						if (extractKeyPressTime) {
							// we try to get a number that have at least 2 digits from the folder name
							wxString relFolderName = releaseFolders.Item(j).AfterLast(wxFILE_SEP_PATH);
							int firstNumberIndex = -1;
							long keyPressTime = -1;

							for (unsigned l = 0; l < relFolderName.Length(); l++) {
								if (wxIsdigit(relFolderName.GetChar(l))) {
									firstNumberIndex = l;
									break;
								}
							}
								
							if (firstNumberIndex > -1) {
								wxString numberPart = relFolderName.Mid(firstNumberIndex);

								if (!numberPart.ToLong(&keyPressTime)) {
									// the number part didn't contain only numbers!
								}

								if (keyPressTime > 9 && keyPressTime < 99999)
									rel.maxKeyPressTime = keyPressTime;
							}
						}

						p->m_releases.push_back(rel);

					}
				}

				pipeReleases.Empty();
				pipeReleasesToAdd.Empty();
			}
		}

		// also scan possible tremulant folders
		if (!tremulantFolders.IsEmpty() && !loadOnlyOneAttack) {
			for (unsigned j = 0; j < tremulantFolders.GetCount(); j++) {
				fillArrayStringWithFiles(
					pipeRoot,
					m_latestPipesRootPath + wxFILE_SEP_PATH + tremulantFolders.Item(j),
					pipeAttacks,
					count,
					firstMatchingNumber
				);

				onlyAddWaveFiles(pipeAttacks, pipeAttacksToAdd);

				// make sure to exactly match the MIDI number
				exactlyMatchMidiNumber(pipeAttacksToAdd, count + firstMatchingNumber);

				// if there are any matching attacks we add them
				if (!pipeAttacksToAdd.IsEmpty()) {
					for (unsigned k = 0; k < pipeAttacksToAdd.GetCount(); k++) {
						wxString relativeFileName;
						if (organRootPathIsSet)
							relativeFileName = getOnlyFileName(pipeAttacksToAdd.Item(k));
						else
							relativeFileName = pipeAttacksToAdd.Item(k);

						// create and add the attack to the pipe
						Attack a;
						a.fileName = relativeFileName;
						a.fullPath = pipeAttacksToAdd.Item(k);
						a.loadRelease = loadRelease;
						a.isTremulant = 1;

						p->m_attacks.push_back(a);

					}
				}

				// also take care of possible tremulant releases
				wxArrayString foldersInTremulantFolder;
				wxArrayString tremReleaseFolders;
				wxString currentTremRootPath = m_latestPipesRootPath + wxFILE_SEP_PATH + tremulantFolders.Item(j);
				wxDir tremRoot(currentTremRootPath);

				if (tremRoot.IsOpened()) {
					// first store all folders existing in root
					wxString folderName;
					bool keepLooking = tremRoot.GetFirst(&folderName, wxT("*"), wxDIR_DIRS);
					do {
						foldersInTremulantFolder.Add(folderName);
						keepLooking = tremRoot.GetNext(&folderName);
					} while (keepLooking);

					// then get only those with release prefix in them
					if (!foldersInTremulantFolder.IsEmpty()) {
						for (unsigned k = 0; k < foldersInTremulantFolder.GetCount(); k++) {
							if (foldersInTremulantFolder.Item(k).Lower().Find(releaseFolderPrefix.Lower()) != wxNOT_FOUND && releaseFolderPrefix != wxEmptyString)
								tremReleaseFolders.Add(foldersInTremulantFolder.Item(k));
						}
					}

					if (!tremReleaseFolders.IsEmpty()) {
						for (unsigned k = 0; k < tremReleaseFolders.GetCount(); k++) {
							fillArrayStringWithFiles(
								tremRoot,
								currentTremRootPath + wxFILE_SEP_PATH + tremReleaseFolders.Item(k),
								pipeReleases,
								count,
								firstMatchingNumber
							);
						}
					}

					pipeReleases.Sort();
					onlyAddWaveFiles(pipeReleases, pipeReleasesToAdd);

					// make sure to exactly match the MIDI number
					exactlyMatchMidiNumber(pipeReleasesToAdd, count + firstMatchingNumber);

					// if there are any matching releases we add them
					if (!pipeReleasesToAdd.IsEmpty()) {
						for (unsigned k = 0; k < pipeReleasesToAdd.GetCount(); k++) {
							wxString relativeFileName;
							if (organRootPathIsSet)
								relativeFileName = getOnlyFileName(pipeReleasesToAdd.Item(k));
							else
								relativeFileName = pipeReleasesToAdd.Item(k);

							// create and add the release to the pipe
							Release rel;
							rel.fileName = relativeFileName;
							rel.fullPath = pipeReleasesToAdd.Item(k);
							rel.isTremulant = 1;

							if (extractKeyPressTime) {
								// we try to get a number that have at least 2 digits from the folder name
								wxString relFolderName = (pipeReleasesToAdd.Item(k).BeforeLast(wxFILE_SEP_PATH)).AfterLast(wxFILE_SEP_PATH);
								int firstNumberIndex = -1;
								long keyPressTime = -1;
								for (unsigned l = 0; l < relFolderName.Length(); l++) {
									if (wxIsdigit(relFolderName.GetChar(l))) {
										firstNumberIndex = l;
										break;
									}
								}

								if (firstNumberIndex > -1) {
									wxString numberPart = relFolderName.Mid(firstNumberIndex);
									if (!numberPart.ToLong(&keyPressTime)) {
										// the number part didn't contain only numbers!
									}

									if (keyPressTime > 9 && keyPressTime < 99999)
										rel.maxKeyPressTime = keyPressTime;
								}
							}

							p->m_releases.push_back(rel);
						}
					}

				}
			}
		}

		if (p->m_attacks.empty()) {
			// if we don't have any other attacks for this pipe, just add a single dummy attack
			Attack a;
			p->m_attacks.push_back(a);
		}

		//m_pipes.push_back(p);
		if (p->hasUnusualTremulants()) {
			hadUnusualTremulants = true;
		}
		count++;
	}
	if (hadUnusualTremulants) {
		GOODF_functions::logTremulantMessage(getName());
	}
}

void Rank::addToPipes(
	wxString extraAttackFolder,
	bool loadOnlyOneAttack,
	bool loadRelease,
	wxString releaseFolderPrefix,
	bool extractKeyPressTime,
	wxString tremulantFolderPrefix,
	bool loadPipesAsTremOff,
	int startPipeIdx,
	int firstMatchingNumber,
	int totalNbrOfPipes
) {
	bool organRootPathIsSet = false;
	bool hadUnusualTremulants = false;

	if (::wxGetApp().m_frame->m_organ->getOdfRoot() != wxEmptyString)
		organRootPathIsSet = true;

	wxDir pipeRoot(m_latestPipesRootPath);

	if (!pipeRoot.IsOpened())
		return;

	int count = 0;
	for (int i = startPipeIdx; i < startPipeIdx + totalNbrOfPipes; i++) {
		Pipe *p = getPipeAt(i);

		wxArrayString pipeAttacks;
		wxArrayString pipeAttacksToAdd;
		wxArrayString pipeReleases;
		wxArrayString pipeReleasesToAdd;
		wxArrayString releaseFolders;
		wxArrayString tremulantFolders;
		wxArrayString allFolders;

		bool hasTremulantFolders = false;

		// first store all folders existing in root
		wxString folder;
		bool cont = pipeRoot.GetFirst(&folder, wxT("*"), wxDIR_DIRS);
		while (cont) {
			allFolders.Add(folder);
		    cont = pipeRoot.GetNext(&folder);
		}

		if (!allFolders.IsEmpty()) {
			// separate release and tremulant folders
			for (unsigned j = 0; j < allFolders.GetCount(); j++) {

				if (allFolders.Item(j).Lower().Find(releaseFolderPrefix.Lower()) != wxNOT_FOUND && releaseFolderPrefix != wxEmptyString) {
					releaseFolders.Add(allFolders.Item(j));
				} else if (allFolders.Item(j).Lower().Find(tremulantFolderPrefix.Lower()) != wxNOT_FOUND && tremulantFolderPrefix != wxEmptyString) {
					tremulantFolders.Add(allFolders.Item(j));
					hasTremulantFolders = true;
				}
			}
		}

		// get attacks from root folder
		pipeRoot.GetAllFiles(
			m_latestPipesRootPath,
			&pipeAttacks,
			wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
			wxDIR_FILES
		);

		// then from possible extra attack folder
		if (extraAttackFolder != wxEmptyString) {
			pipeRoot.GetAllFiles(
				m_latestPipesRootPath + wxFILE_SEP_PATH + extraAttackFolder,
				&pipeAttacks,
				wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
				wxDIR_FILES
			);
		}

		pipeAttacks.Sort();

		// remove any file that's not ending with .wav or .wv
		onlyAddWaveFiles(pipeAttacks, pipeAttacksToAdd);

		// make sure to exactly match the MIDI number
		exactlyMatchMidiNumber(pipeAttacksToAdd, count + firstMatchingNumber);

		// if there are any matching attacks we add them
		if (!pipeAttacksToAdd.IsEmpty()) {
			for (unsigned j = 0; j < pipeAttacksToAdd.GetCount(); j++) {
				wxString relativeFileName;
				if (organRootPathIsSet)
					relativeFileName = getOnlyFileName(pipeAttacksToAdd.Item(j));
				else
					relativeFileName = pipeAttacksToAdd.Item(j);

				// create and add the attack to the pipe
				Attack a;
				a.fileName = relativeFileName;
				a.fullPath = pipeAttacksToAdd.Item(j);
				a.loadRelease = loadRelease;
				if (hasTremulantFolders || loadPipesAsTremOff)
					a.isTremulant = 0;

				p->m_attacks.push_back(a);

				if (loadOnlyOneAttack)
					break;
			}
		}

		pipeAttacks.Empty();
		pipeAttacksToAdd.Empty();

		// add extra releases if they can be found
		if (!releaseFolders.IsEmpty()) {

			releaseFolders.Sort();

			for (unsigned j = 0; j < releaseFolders.GetCount(); j++) {
				pipeRoot.GetAllFiles(
					m_latestPipesRootPath + wxFILE_SEP_PATH + releaseFolders.Item(j),
					&pipeReleases,
					wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
					wxDIR_FILES
				);

				pipeReleases.Sort();

				// remove any file that's not ending with .wav or .wv
				onlyAddWaveFiles(pipeReleases, pipeReleasesToAdd);

				// make sure to exactly match the MIDI number
				exactlyMatchMidiNumber(pipeReleasesToAdd, count + firstMatchingNumber);

				// if there are any matching releases we add them
				if (!pipeReleasesToAdd.IsEmpty()) {

					for (unsigned k = 0; k < pipeReleasesToAdd.GetCount(); k++) {
						wxString relativeFileName;
						if (organRootPathIsSet)
							relativeFileName = getOnlyFileName(pipeReleasesToAdd.Item(k));
						else
							relativeFileName = pipeReleasesToAdd.Item(k);

						// create and add the release to the pipe
						Release rel;
						rel.fileName = relativeFileName;
						rel.fullPath = pipeReleasesToAdd.Item(k);
						if (hasTremulantFolders || loadPipesAsTremOff)
							rel.isTremulant = 0;

						if (extractKeyPressTime) {
							// we try to get a number that have at least 2 digits from the folder name
							wxString relFolderName = releaseFolders.Item(j).AfterLast(wxFILE_SEP_PATH);
							int firstNumberIndex = -1;
							long keyPressTime = -1;

							for (unsigned l = 0; l < relFolderName.Length(); l++) {
								if (wxIsdigit(relFolderName.GetChar(l))) {
									firstNumberIndex = l;
									break;
								}
							}

							if (firstNumberIndex > -1) {
								wxString numberPart = relFolderName.Mid(firstNumberIndex);

								if (!numberPart.ToLong(&keyPressTime)) {
									// the number part didn't contain only numbers!
								}

								if (keyPressTime > 9 && keyPressTime < 99999)
									rel.maxKeyPressTime = keyPressTime;
							}
						}

						p->m_releases.push_back(rel);

					}
				}

				pipeReleases.Empty();
				pipeReleasesToAdd.Empty();
			}
		}

		// also scan possible tremulant folders
		if (!tremulantFolders.IsEmpty() && !loadOnlyOneAttack) {
			for (unsigned j = 0; j < tremulantFolders.GetCount(); j++) {
				fillArrayStringWithFiles(
					pipeRoot,
					m_latestPipesRootPath + wxFILE_SEP_PATH + tremulantFolders.Item(j),
					pipeAttacks,
					count,
					firstMatchingNumber
				);

				onlyAddWaveFiles(pipeAttacks, pipeAttacksToAdd);

				// make sure to exactly match the MIDI number
				exactlyMatchMidiNumber(pipeAttacksToAdd, count + firstMatchingNumber);

				// if there are any matching attacks we add them
				if (!pipeAttacksToAdd.IsEmpty()) {
					for (unsigned k = 0; k < pipeAttacksToAdd.GetCount(); k++) {
						wxString relativeFileName;
						if (organRootPathIsSet)
							relativeFileName = getOnlyFileName(pipeAttacksToAdd.Item(k));
						else
							relativeFileName = pipeAttacksToAdd.Item(k);

						// create and add the attack to the pipe
						Attack a;
						a.fileName = relativeFileName;
						a.fullPath = pipeAttacksToAdd.Item(k);
						a.loadRelease = loadRelease;
						a.isTremulant = 1;

						p->m_attacks.push_back(a);

					}
				}

				// also take care of possible tremulant releases
				wxArrayString foldersInTremulantFolder;
				wxArrayString tremReleaseFolders;
				wxString currentTremRootPath = m_latestPipesRootPath + wxFILE_SEP_PATH + tremulantFolders.Item(j);
				wxDir tremRoot(currentTremRootPath);

				if (tremRoot.IsOpened()) {
					// first store all folders existing in root
					wxString folderName;
					bool keepLooking = tremRoot.GetFirst(&folderName, wxT("*"), wxDIR_DIRS);
					while (keepLooking) {
						foldersInTremulantFolder.Add(folderName);
					    keepLooking = tremRoot.GetNext(&folderName);
					}

					// then get only those with release prefix in them
					if (!foldersInTremulantFolder.IsEmpty()) {
						for (unsigned k = 0; k < foldersInTremulantFolder.GetCount(); k++) {
							if (foldersInTremulantFolder.Item(k).Lower().Find(releaseFolderPrefix.Lower()) != wxNOT_FOUND && releaseFolderPrefix != wxEmptyString)
								tremReleaseFolders.Add(foldersInTremulantFolder.Item(k));
						}
					}

					if (!tremReleaseFolders.IsEmpty()) {
						for (unsigned k = 0; k < tremReleaseFolders.GetCount(); k++) {
							fillArrayStringWithFiles(
								tremRoot,
								currentTremRootPath + wxFILE_SEP_PATH + tremReleaseFolders.Item(k),
								pipeReleases,
								count,
								firstMatchingNumber
							);
						}
					}

					pipeReleases.Sort();
					onlyAddWaveFiles(pipeReleases, pipeReleasesToAdd);

					// make sure to exactly match the MIDI number
					exactlyMatchMidiNumber(pipeReleasesToAdd, count + firstMatchingNumber);

					// if there are any matching releases we add them
					if (!pipeReleasesToAdd.IsEmpty()) {
						for (unsigned k = 0; k < pipeReleasesToAdd.GetCount(); k++) {
							wxString relativeFileName;
							if (organRootPathIsSet)
								relativeFileName = getOnlyFileName(pipeReleasesToAdd.Item(k));
							else
								relativeFileName = pipeReleasesToAdd.Item(k);

							// create and add the release to the pipe
							Release rel;
							rel.fileName = relativeFileName;
							rel.fullPath = pipeReleasesToAdd.Item(k);
							rel.isTremulant = 1;

							if (extractKeyPressTime) {
								// we try to get a number that have at least 2 digits from the folder name
								wxString relFolderName = (pipeReleasesToAdd.Item(k).BeforeLast(wxFILE_SEP_PATH)).AfterFirst(wxFILE_SEP_PATH);
								int firstNumberIndex = -1;
								long keyPressTime = -1;
								for (unsigned l = 0; l < relFolderName.Length(); l++) {
									if (wxIsdigit(relFolderName.GetChar(l))) {
										firstNumberIndex = l;
										break;
									}
								}

								if (firstNumberIndex > -1) {
									wxString numberPart = relFolderName.Mid(firstNumberIndex);
									if (!numberPart.ToLong(&keyPressTime)) {
										// the number part didn't contain only numbers!
									}

									if (keyPressTime > 9 && keyPressTime < 99999)
										rel.maxKeyPressTime = keyPressTime;
								}
							}

							p->m_releases.push_back(rel);
						}
					}

				}
			}
		}

		if (p->hasUnusualTremulants()) {
			hadUnusualTremulants = true;
		}
		count++;
	}
	if (hadUnusualTremulants) {
		GOODF_functions::logTremulantMessage(getName());
	}
}

void Rank::addTremulantToPipes(
	wxString extraAttackFolder,
	bool loadOnlyOneAttack,
	bool loadRelease,
	wxString releaseFolderPrefix,
	bool extractKeyPressTime,
	int startPipeIdx,
	int firstMatchingNumber,
	int totalNbrOfPipes
) {
	// This method is for adding additional attacks/releases as (wave) tremulants only
	bool organRootPathIsSet = false;
	bool hadUnusualTremulants = false;

	if (::wxGetApp().m_frame->m_organ->getOdfRoot() != wxEmptyString)
		organRootPathIsSet = true;

	wxDir pipeRoot(m_latestPipesRootPath);

	if (!pipeRoot.IsOpened())
		return;

	int count = 0;
	for (int i = startPipeIdx; i < startPipeIdx + totalNbrOfPipes; i++) {
		Pipe *p = getPipeAt(i);

		wxArrayString pipeAttacks;
		wxArrayString pipeAttacksToAdd;
		wxArrayString pipeReleases;
		wxArrayString pipeReleasesToAdd;
		wxArrayString releaseFolders;
		wxArrayString allFolders;

		// first store all folders existing in root
		wxString folder;
		bool cont = pipeRoot.GetFirst(&folder, wxT("*"), wxDIR_DIRS);
		while (cont) {
			allFolders.Add(folder);
			cont = pipeRoot.GetNext(&folder);
		}

		if (!allFolders.IsEmpty()) {
			// separate release and tremulant folders
			for (unsigned j = 0; j < allFolders.GetCount(); j++) {

				if (allFolders.Item(j).Lower().Find(releaseFolderPrefix.Lower()) != wxNOT_FOUND && releaseFolderPrefix != wxEmptyString) {
					releaseFolders.Add(allFolders.Item(j));
				}
			}
		}

		// get attacks from root folder
		pipeRoot.GetAllFiles(
			m_latestPipesRootPath,
			&pipeAttacks,
			wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
			wxDIR_FILES
		);

		// then from possible extra attack folder
		if (extraAttackFolder != wxEmptyString) {
			pipeRoot.GetAllFiles(
				m_latestPipesRootPath + wxFILE_SEP_PATH + extraAttackFolder,
				&pipeAttacks,
				wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
				wxDIR_FILES
			);
		}

		pipeAttacks.Sort();

		// remove any file that's not ending with .wav or .wv
		onlyAddWaveFiles(pipeAttacks, pipeAttacksToAdd);

		// make sure to exactly match the MIDI number
		exactlyMatchMidiNumber(pipeAttacksToAdd, count + firstMatchingNumber);

		// if there are any matching attacks we add them
		if (!pipeAttacksToAdd.IsEmpty()) {
			for (unsigned j = 0; j < pipeAttacksToAdd.GetCount(); j++) {
				wxString relativeFileName;
				if (organRootPathIsSet)
					relativeFileName = getOnlyFileName(pipeAttacksToAdd.Item(j));
				else
					relativeFileName = pipeAttacksToAdd.Item(j);

				// create and add the attack to the pipe
				Attack a;
				a.fileName = relativeFileName;
				a.fullPath = pipeAttacksToAdd.Item(j);
				a.loadRelease = loadRelease;
				a.isTremulant = 1;

				p->m_attacks.push_back(a);

				if (loadOnlyOneAttack)
					break;
			}
		}

		pipeAttacks.Empty();
		pipeAttacksToAdd.Empty();

		// add extra releases if they can be found
		if (!releaseFolders.IsEmpty()) {

			releaseFolders.Sort();

			for (unsigned j = 0; j < releaseFolders.GetCount(); j++) {
				pipeRoot.GetAllFiles(
					m_latestPipesRootPath + wxFILE_SEP_PATH + releaseFolders.Item(j),
					&pipeReleases,
					wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
					wxDIR_FILES
				);

				pipeReleases.Sort();

				// remove any file that's not ending with .wav or .wv
				onlyAddWaveFiles(pipeReleases, pipeReleasesToAdd);

				// make sure to exactly match the MIDI number
				exactlyMatchMidiNumber(pipeReleasesToAdd, count + firstMatchingNumber);

				// if there are any matching releases we add them
				if (!pipeReleasesToAdd.IsEmpty()) {

					for (unsigned k = 0; k < pipeReleasesToAdd.GetCount(); k++) {
						wxString relativeFileName;
						if (organRootPathIsSet)
							relativeFileName = getOnlyFileName(pipeReleasesToAdd.Item(k));
						else
							relativeFileName = pipeReleasesToAdd.Item(k);

						// create and add the release to the pipe
						Release rel;
						rel.fileName = relativeFileName;
						rel.fullPath = pipeReleasesToAdd.Item(k);
						rel.isTremulant = 1;

						if (extractKeyPressTime) {
							// we try to get a number that have at least 2 digits from the folder name
							wxString relFolderName = releaseFolders.Item(j).AfterLast(wxFILE_SEP_PATH);
							int firstNumberIndex = -1;
							long keyPressTime = -1;

							for (unsigned l = 0; l < relFolderName.Length(); l++) {
								if (wxIsdigit(relFolderName.GetChar(l))) {
									firstNumberIndex = l;
									break;
								}
							}

							if (firstNumberIndex > -1) {
								wxString numberPart = relFolderName.Mid(firstNumberIndex);

								if (!numberPart.ToLong(&keyPressTime)) {
									// the number part didn't contain only numbers!
								}

								if (keyPressTime > 9 && keyPressTime < 99999)
									rel.maxKeyPressTime = keyPressTime;
							}
						}

						p->m_releases.push_back(rel);

					}
				}

				pipeReleases.Empty();
				pipeReleasesToAdd.Empty();
			}
		}
		if (p->hasUnusualTremulants()) {
			hadUnusualTremulants = true;
		}
		count++;
	}

	if (hadUnusualTremulants) {
		GOODF_functions::logTremulantMessage(getName());
	}
}

void Rank::addReleasesToPipes(
	bool loadPipesAsTremOff,
	int startPipeIdx,
	int firstMatchingNumber,
	int totalNbrOfPipes
) {
	// This method is for adding releases only from a single folder
	bool organRootPathIsSet = false;
	bool hadUnusualTremulants = false;

	if (::wxGetApp().m_frame->m_organ->getOdfRoot() != wxEmptyString)
		organRootPathIsSet = true;

	wxDir pipeRoot(m_latestPipesRootPath);

	if (!pipeRoot.IsOpened())
		return;

	int count = 0;
	for (int i = startPipeIdx; i < startPipeIdx + totalNbrOfPipes; i++) {
		Pipe *p = getPipeAt(i);

		wxArrayString pipeReleases;
		wxArrayString pipeReleasesToAdd;

		// get files from root folder
		pipeRoot.GetAllFiles(
			m_latestPipesRootPath,
			&pipeReleases,
			wxString::Format(wxT("*%i*.*"), (count + firstMatchingNumber)),
			wxDIR_FILES
		);

		pipeReleases.Sort();

		// remove any file that's not ending with .wav or .wv
		onlyAddWaveFiles(pipeReleases, pipeReleasesToAdd);

		// make sure to exactly match the MIDI number
		exactlyMatchMidiNumber(pipeReleasesToAdd, count + firstMatchingNumber);

		// if there are any matching attacks we add them
		if (!pipeReleasesToAdd.IsEmpty()) {
			for (unsigned j = 0; j < pipeReleasesToAdd.GetCount(); j++) {
				wxString relativeFileName;
				if (organRootPathIsSet)
					relativeFileName = getOnlyFileName(pipeReleasesToAdd.Item(j));
				else
					relativeFileName = pipeReleasesToAdd.Item(j);

				// create and add the release to the pipe
				Release r;
				r.fileName = relativeFileName;
				r.fullPath = pipeReleasesToAdd.Item(j);
				if (loadPipesAsTremOff)
					r.isTremulant = 0;

				p->m_releases.push_back(r);

			}
		}

		pipeReleases.Empty();
		pipeReleasesToAdd.Empty();
		if (p->hasUnusualTremulants()) {
			hadUnusualTremulants = true;
		}
		count++;
	}
	if (hadUnusualTremulants) {
		GOODF_functions::logTremulantMessage(getName());
	}
}

void Rank::clearAllPipes() {
	for (Pipe p : m_pipes) {
		p.m_attacks.clear();
		p.m_releases.clear();
	}

	m_pipes.clear();
}

void Rank::createDummyPipes() {
	if (!m_pipes.empty())
		clearAllPipes();

	for (int i = 0; i < numberOfLogicalPipes; i++) {
		Pipe p;
		setupPipeProperties(p);

		Attack a;
		p.m_attacks.push_back(a);

		m_pipes.push_back(p);
	}
}

void Rank::addDummyPipeFront() {
	Pipe p;
	setupPipeProperties(p);

	Attack a;
	p.m_attacks.push_back(a);

	m_pipes.push_front(p);
}

void Rank::addDummyPipeBack() {
	Pipe p;
	setupPipeProperties(p);

	Attack a;
	p.m_attacks.push_back(a);

	m_pipes.push_back(p);
}

bool Rank::hasOnlyDummyPipes() {
	for (Pipe p : m_pipes) {
		for (Attack atk : p.m_attacks) {
			if (atk.fileName != wxT("DUMMY"))
				return false;
		}
	}
	return true;
}

void Rank::removePipeFront() {
	m_pipes.pop_front();
}

void Rank::removePipeBack() {
	m_pipes.pop_back();
}

void Rank::clearPipeAt(unsigned index) {
	auto iterator = std::next(m_pipes.begin(), index);
	(*iterator).m_attacks.clear();
	(*iterator).m_releases.clear();

	setupPipeProperties(*iterator);
	Attack a;
	(*iterator).m_attacks.push_back(a);
}

void Rank::emptyPipeAt(unsigned index) {
	auto iterator = std::next(m_pipes.begin(), index);
	(*iterator).m_attacks.clear();
	(*iterator).m_releases.clear();
}

void Rank::createNewAttackInPipe(unsigned index, wxString filePath, bool loadRelease) {
	auto iterator = std::next(m_pipes.begin(), index);

	bool organRootPathIsSet = false;

	if (::wxGetApp().m_frame->m_organ->getOdfRoot() != wxEmptyString)
		organRootPathIsSet = true;

	wxString relativeFileName;
	if (organRootPathIsSet)
		relativeFileName = getOnlyFileName(filePath);
	else
		relativeFileName = filePath;

	// if the pipe has a DUMMY pipe we'll replace it
	if (iterator->m_attacks.front().fullPath.IsSameAs(wxT("DUMMY"), true)) {
		iterator->m_attacks.pop_front();
	}

	// create and add the attack to the pipe
	Attack a;
	a.fileName = relativeFileName;
	a.fullPath = filePath;
	a.loadRelease = loadRelease;

	(*iterator).m_attacks.push_back(a);
}

void Rank::createNewReleaseInPipe(unsigned index, wxString filePath, bool extractKeyPressTime) {
	auto iterator = std::next(m_pipes.begin(), index);

	bool organRootPathIsSet = false;

	if (::wxGetApp().m_frame->m_organ->getOdfRoot() != wxEmptyString)
		organRootPathIsSet = true;

	wxString relativeFileName;
	if (organRootPathIsSet)
		relativeFileName = getOnlyFileName(filePath);
	else
		relativeFileName = filePath;

	// create and add the release to the pipe
	Release rel;
	rel.fileName = relativeFileName;
	rel.fullPath = filePath;

	if (extractKeyPressTime) {
		// we try to get a number that have at least 2 digits from the folder name
		wxString folderPath = filePath.BeforeLast(wxFILE_SEP_PATH);
		wxString relFolderName = folderPath.AfterLast(wxFILE_SEP_PATH);
		int firstNumberIndex = -1;
		long keyPressTime = -1;

		for (unsigned l = 0; l < relFolderName.Length(); l++) {
			if (wxIsdigit(relFolderName.GetChar(l))) {
				firstNumberIndex = l;
				break;
			}
		}

		if (firstNumberIndex > -1) {
			wxString numberPart = relFolderName.Mid(firstNumberIndex);
			if (!numberPart.ToLong(&keyPressTime)) {
				// the number part didn't contain only numbers!
			}

			if (keyPressTime > 9 && keyPressTime < 99999)
				rel.maxKeyPressTime = keyPressTime;
		}
	}

	(*iterator).m_releases.push_back(rel);
}

bool Rank::deleteAttackInPipe(unsigned pipeIndex, unsigned attackIndex) {
	auto pipeIt = std::next(m_pipes.begin(), pipeIndex);
	auto atkIt = std::next((*pipeIt).m_attacks.begin(), attackIndex);

	if ((*pipeIt).m_attacks.size() > 1) {
		(*pipeIt).m_attacks.erase(atkIt);
		return true;
	} else {
		return false;
	}
}

void Rank::deleteReleaseInPipe(unsigned pipeIndex, unsigned releaseIndex) {
	auto pipeIt = std::next(m_pipes.begin(), pipeIndex);
	auto relIt = std::next((*pipeIt).m_releases.begin(), releaseIndex);

	(*pipeIt).m_releases.erase(relIt);
}

Pipe* Rank::getPipeAt(unsigned index) {
	auto iterator = std::next(m_pipes.begin(), index);
	return &(*iterator);
}

void Rank::fillArrayStringWithFiles(wxDir &root, wxString path, wxArrayString &list, int theCount, int firstMatchingNbr) {
	root.GetAllFiles(
		path,
		&list,
		wxString::Format(wxT("*%i*.*"), (theCount + firstMatchingNbr)),
		wxDIR_FILES
	);
}

void Rank::onlyAddWaveFiles(wxArrayString &source, wxArrayString &selection) {
	if (!source.IsEmpty()) {
		for (unsigned n = 0; n < source.GetCount(); n++) {
			wxString suffix = source.Item(n).AfterLast('.');
			if (suffix.CmpNoCase(wxT("wav")) == 0 || suffix.CmpNoCase(wxT("wv")) == 0)
				selection.Add(source.Item(n));
		}
	}
}

wxString Rank::getOnlyFileName(wxString path) {
	return GOODF_functions::removeBaseOdfPath(path);
}

void Rank::setupPipeProperties(Pipe &pipe) {
	pipe.isPercussive = this->percussive;
	pipe.hasIndependentRelease = this->hasIndependentRelease;
	pipe.harmonicNumber = this->harmonicNumber;
	pipe.acceptsRetuning = this->acceptsRetuning;
	pipe.windchest = this->windchest;
	pipe.minVelocityVolume = this->minVelocityVolume;
	pipe.maxVelocityVolume = this->maxVelocityVolume;
}

void Rank::exactlyMatchMidiNumber(wxArrayString &fileList, int midiNbr) {
	// incoming fileList contains full paths to candidate files
	// but in this function we're interested only in the file name
	// we get it by converting path to wxFileName and then extracting the name part
	// and matching against this regular expression
	wxRegEx midiRegEx = wxRegEx(wxString::Format(wxT("^([0[:alpha:]]*)(%i)([^[:digit:]]*[-._])"), midiNbr));

	if (!fileList.IsEmpty()) {
		for (int i = fileList.size() - 1; i >= 0; i--) {
			wxFileName whole_path(fileList[i]);
			if (!midiRegEx.Matches(whole_path.GetFullName())) {
				fileList.RemoveAt(i);
			}
		}
	}
}

void Rank::updatePipeRelativePaths() {
	for (Pipe& p : m_pipes) {
		p.updateRelativePaths();
	}
}
