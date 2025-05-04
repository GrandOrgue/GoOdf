/*
 * Pipe.h is part of GoOdf.
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

#ifndef PIPE_H
#define PIPE_H

#include "Windchestgroup.h"
#include <list>
#include "Attack.h"
#include "Release.h"
#include <wx/textfile.h>
#include <wx/fileconf.h>

class Rank;
class Organ;

class Pipe {
public:
	Pipe();
	Pipe(const Pipe& p);
	~Pipe();

	void write(wxTextFile *outFile, wxString pipeNr, Rank *parent);
	void read(wxFileConfig *cfg, wxString pipeNr, Rank *parent, Organ *readOrgan);
	void readAttack(wxFileConfig *cfg, wxString pipeStr, Organ *readOrgan);

	bool isFirstAttackRefPath();
	void writeAdditionalAttacks(wxTextFile *outFile, wxString pipeNr);
	void writeAdditionalReleases(wxTextFile *outFile, wxString pipeNr);
	void writeRef(wxTextFile *outFile, wxString pipeNr);
	void writeLoadRelease(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeAttackVelocity(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeMaxTimeSinceLastRelease(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeIsTremulant(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeMaxKeyPressTime(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeAttackStart(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeCuePoint(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeReleaseEnd(wxTextFile *outFile, wxString pipeNr, Attack atk);
	void writeLoops(wxTextFile *outFile, wxString pipeNr, Attack &atk);
	void writeLoopXfade(wxTextFile *outFile, wxString pipeNr, Attack &atk);
	void writeReleaseXfade(wxTextFile *outFile, wxString pipeNr, Attack &atk);
	void updateRelativePaths();
	void updateRefString();
	bool isIndependentRelease();
	void setIndependentRelease(bool independent);
	bool hasUnusualTremulants();

	bool isPercussive;
	bool hasIndependentRelease;
	float amplitudeLevel;
	float gain;
	float pitchTuning;
	int trackerDelay;
	int harmonicNumber;
	int midiKeyNumber;
	float midiPitchFraction;
	float pitchCorrection;
	bool acceptsRetuning;
	Windchestgroup *windchest;
	float minVelocityVolume;
	float maxVelocityVolume;

	std::list<Attack> m_attacks;
	std::list<Release> m_releases;

};

#endif

