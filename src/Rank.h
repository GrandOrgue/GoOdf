/*
 * Rank.h is part of GoOdf.
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

#ifndef RANK_H
#define RANK_H

#include "Pipe.h"
#include "Windchestgroup.h"
#include <list>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <wx/fileconf.h>

class Organ;

class Rank {
public:
	Rank();
	Rank(const Rank& r);
	~Rank();

	void write(wxTextFile *outFile);
	void writeFromStop(wxTextFile *outFile);
	void read(wxFileConfig *cfg, Organ *readOrgan);

	bool doesAcceptsRetuning() const;
	void setAcceptsRetuning(bool acceptsRetuning);
	float getAmplitudeLevel() const;
	void setAmplitudeLevel(float amplitudeLevel);
	int getFirstMidiNoteNumber() const;
	void setFirstMidiNoteNumber(int firstMidiNoteNumber);
	float getGain() const;
	void setGain(float gain);
	int getHarmonicNumber() const;
	void setHarmonicNumber(int harmonicNumber);
	float getMaxVelocityVolume() const;
	void setMaxVelocityVolume(float maxVelocityVolume);
	float getMinVelocityVolume() const;
	void setMinVelocityVolume(float minVelocityVolume);
	const wxString& getName() const;
	void setName(const wxString &name);
	int getNumberOfLogicalPipes() const;
	void setNumberOfLogicalPipes(int numberOfLogicalPipes);
	bool isPercussive() const;
	void setPercussive(bool percussive);
	bool isIndependentRelease();
	void setIndependentRelease(bool independent);
	float getPitchCorrection() const;
	void setPitchCorrection(float pitchCorrection);
	float getPitchTuning() const;
	void setPitchTuning(float pitchTuning);
	int getTrackerDelay() const;
	void setTrackerDelay(int trackerDelay);
	Windchestgroup* getWindchest();
	void setWindchest(Windchestgroup *windchest);
	void setOnlyRankWindchest(Windchestgroup *windchest);
	wxString getPipesRootPath();
	void setPipesRootPath(wxString path);
	void readPipes(
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
	);
	void addToPipes(
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
	);
	void addTremulantToPipes(
		wxString extraAttackFolder,
		bool loadOnlyOneAttack,
		bool loadRelease,
		wxString releaseFolderPrefix,
		bool extractKeyPressTime,
		int startPipeIdx,
		int firstMatchingNumber,
		int totalNbrOfPipes
	);
	void addReleasesToPipes(
		bool loadPipesAsTremOff,
		int startPipeIdx,
		int firstMatchingNumber,
		int totalNbrOfPipes
	);
	void clearAllPipes();
	void createDummyPipes();
	void addDummyPipeFront();
	void addDummyPipeBack();
	bool hasOnlyDummyPipes();
	void removePipeFront();
	void removePipeBack();
	void clearPipeAt(unsigned index);
	void emptyPipeAt(unsigned index);
	void createNewAttackInPipe(unsigned index, wxString filePath, bool loadRelease);
	void createNewReleaseInPipe(unsigned index, wxString filePath, bool extractKeyPressTime);
	bool deleteAttackInPipe(unsigned pipeIndex, unsigned attackIndex);
	void deleteReleaseInPipe(unsigned pipeIndex, unsigned releaseIndex);
	Pipe* getPipeAt(unsigned index);
	void updatePipeRelativePaths();

	std::list<Pipe> m_pipes;

protected:
	wxString name;
	int firstMidiNoteNumber;
	int numberOfLogicalPipes;
	float amplitudeLevel;
	float gain;
	float pitchTuning;
	int trackerDelay;
	int harmonicNumber;
	float pitchCorrection;
	Windchestgroup* windchest;
	bool percussive;
	bool hasIndependentRelease;
	float minVelocityVolume;
	float maxVelocityVolume;
	bool acceptsRetuning;
	wxString m_latestPipesRootPath;

	void fillArrayStringWithFiles(wxDir &root, wxString path, wxArrayString &list, int theCount, int firstMatchingNbr);
	void onlyAddWaveFiles(wxArrayString &source, wxArrayString &selection);
	wxString getOnlyFileName(wxString path);
	void setupPipeProperties(Pipe &pipe);
	void exactlyMatchMidiNumber(wxArrayString &fileList, int midiNbr);
};

#endif
