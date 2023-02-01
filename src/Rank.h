/*
 * Rank.h is part of GOODF.
 * Copyright (C) 2023 Lars Palo
 *
 * GOODF is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GOODF is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GOODF.  If not, see <https://www.gnu.org/licenses/>.
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

class Rank {
public:
	Rank();
	~Rank();

	void write(wxTextFile *outFile);
	void writeFromStop(wxTextFile *outFile);

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
	float getPitchCorrection() const;
	void setPitchCorrection(float pitchCorrection);
	float getPitchTuning() const;
	void setPitchTuning(float pitchTuning);
	int getTrackerDelay() const;
	void setTrackerDelay(int trackerDelay);
	Windchestgroup* getWindchest();
	void setWindchest(Windchestgroup *windchest);
	wxString getPipesRootPath();
	void setPipesRootPath(wxString path);
	void readPipes(
		wxString extraAttackFolder,
		bool loadOnlyOneAttack,
		bool loadRelease,
		wxString releaseFolderPrefix,
		bool extractKeyPressTime,
		wxString tremulantFolderPrefix
	);
	void clearAllPipes();
	void createDummyPipes();
	void addDummyPipeFront();
	void addDummyPipeBack();
	bool hasOnlyDummyPipes();
	void removePipeFront();
	void removePipeBack();
	void clearPipeAt(unsigned index);
	void createNewAttackInPipe(unsigned index, wxString filePath, bool loadRelease);
	void createNewReleaseInPipe(unsigned index, wxString filePath, bool extractKeyPressTime);
	bool deleteAttackInPipe(unsigned pipeIndex, unsigned attackIndex);
	void deleteReleaseInPipe(unsigned pipeIndex, unsigned releaseIndex);
	Pipe* getPipeAt(unsigned index);

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
	float minVelocityVolume;
	float maxVelocityVolume;
	bool acceptsRetuning;
	wxString m_latestPipesRootPath;

	void fillArrayStringWithFiles(wxDir &root, wxString path, wxArrayString &list, int pipeIndex);
	void onlyAddWaveFiles(wxArrayString &source, wxArrayString &selection);
	wxString getOnlyFileName(wxString path);
	void setupPipeProperties(Pipe &pipe);
};

#endif
