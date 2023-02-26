/*
 * Pipe.cpp is part of GOODF.
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

#include "Pipe.h"
#include "GOODFFunctions.h"

Pipe::Pipe() {
	isPercussive = false;
	amplitudeLevel = 100;
	gain = 0;
	pitchTuning = 0;
	trackerDelay = 0;
	harmonicNumber = 8;
	midiKeyNumber = -1;
	midiPitchFraction = -1.0f;
	pitchCorrection = 0;
	acceptsRetuning = true;
	windchest = NULL;
	minVelocityVolume = 100;
	maxVelocityVolume = 100;
	loopCrossfadeLength = 0;
	releaseCrossfadeLength = 0;
}

Pipe::Pipe(const Pipe& p) {
	isPercussive = p.isPercussive;
	amplitudeLevel = p.amplitudeLevel;
	gain = p.gain;
	pitchTuning = p.pitchTuning;
	trackerDelay = p.trackerDelay;
	harmonicNumber = p.harmonicNumber;
	midiKeyNumber = p.midiKeyNumber;
	midiPitchFraction = p.midiPitchFraction;
	pitchCorrection = p.pitchCorrection;
	acceptsRetuning = p.acceptsRetuning;
	windchest = p.windchest;
	minVelocityVolume = p.minVelocityVolume;
	maxVelocityVolume = p.maxVelocityVolume;
	loopCrossfadeLength = p.loopCrossfadeLength;
	releaseCrossfadeLength = p.releaseCrossfadeLength;

	for (Attack atk : p.m_attacks) {
		m_attacks.push_back(atk);
	}

	for (Release rel : p.m_releases) {
		m_releases.push_back(rel);
	}
}

Pipe::~Pipe() {

}

void Pipe::write(wxTextFile *outFile, wxString pipeNr, Rank *parent) {
	if (!isFirstAttackRefPath()) {
		// remove organ base path from output line path
		wxString relativeFileName = GOODF_functions::removeBaseOdfPath(m_attacks.front().fullPath);
		wxString fullLine = GOODF_functions::fixSeparator(pipeNr + wxT("=") + relativeFileName);
		outFile->AddLine(fullLine);

		if (isPercussive != parent->isPercussive()) {
			if (isPercussive)
				outFile->AddLine(pipeNr + wxT("Percussive=Y"));
			else
				outFile->AddLine(pipeNr + wxT("Percussive=N"));
		}
		if (amplitudeLevel != 100)
			outFile->AddLine(pipeNr + wxT("AmplitudeLevel=") + wxString::Format(wxT("%f"), amplitudeLevel));
		if (gain != 0)
			outFile->AddLine(pipeNr + wxT("Gain=") + wxString::Format(wxT("%f"), gain));
		if (pitchTuning != 0)
			outFile->AddLine(pipeNr + wxT("PitchTuning=") + wxString::Format(wxT("%f"), pitchTuning));
		if (trackerDelay != 0)
			outFile->AddLine(pipeNr + wxT("TrackerDelay=") + wxString::Format(wxT("%i"), trackerDelay));

		writeLoadRelease(outFile, pipeNr, m_attacks.front());
		writeAttackVelocity(outFile, pipeNr, m_attacks.front());
		writeMaxTimeSinceLastRelease(outFile, pipeNr, m_attacks.front());
		writeIsTremulant(outFile, pipeNr, m_attacks.front());
		writeMaxKeyPressTime(outFile, pipeNr, m_attacks.front());
		writeAttackStart(outFile, pipeNr, m_attacks.front());
		writeCuePoint(outFile, pipeNr, m_attacks.front());
		writeReleaseEnd(outFile, pipeNr, m_attacks.front());
		for (Attack &a : m_attacks) {
			writeLoops(outFile, pipeNr, a);
			break;
		}

		if (harmonicNumber != 8 && harmonicNumber != parent->getHarmonicNumber())
			outFile->AddLine(pipeNr + wxT("HarmonicNumber=") + wxString::Format(wxT("%i"), harmonicNumber));
		if (midiKeyNumber > -1)
			outFile->AddLine(pipeNr + wxT("MIDIKeyNumber=") + wxString::Format(wxT("%i"), midiKeyNumber));
		if (midiPitchFraction > -0.1f)
			outFile->AddLine(pipeNr + wxT("MIDIPitchFraction=") + wxString::Format(wxT("%f"), midiPitchFraction));
		if (pitchCorrection != 0 && pitchCorrection != parent->getPitchCorrection())
			outFile->AddLine(pipeNr + wxT("PitchCorrection=") + wxString::Format(wxT("%f"), pitchCorrection));
		if (acceptsRetuning != parent->doesAcceptsRetuning()) {
			if (acceptsRetuning)
				outFile->AddLine(pipeNr + wxT("AcceptsRetuning=Y"));
			else
				outFile->AddLine(pipeNr + wxT("AcceptsRetuning=N"));
		}
		if (windchest != parent->getWindchest()) {
			wxString wcRef = wxString::Format(wxT("%u"), ::wxGetApp().m_frame->m_organ->getIndexOfOrganWindchest(windchest));
			outFile->AddLine(pipeNr + wxT("WindchestGroup=") + wcRef);
		}

		writeAdditionalAttacks(outFile, pipeNr);
		writeAdditionalReleases(outFile, pipeNr);

		if (loopCrossfadeLength != 0)
			outFile->AddLine(pipeNr + wxT("LoopCrossfadeLength=") + wxString::Format(wxT("%i"), loopCrossfadeLength));
		if (releaseCrossfadeLength != 0)
			outFile->AddLine(pipeNr + wxT("ReleaseCrossfadeLength=") + wxString::Format(wxT("%i"), releaseCrossfadeLength));
	} else {
		writeRef(outFile, pipeNr);
	}
}

bool Pipe::isFirstAttackRefPath() {
	return m_attacks.front().fileName.StartsWith(wxT("REF"));
}

void Pipe::writeAdditionalAttacks(wxTextFile *outFile, wxString pipeNr) {
	// Deal with possible additional attacks
	if (m_attacks.size() > 1) {
		unsigned extraAttacks = m_attacks.size() - 1;
		outFile->AddLine(pipeNr + wxT("AttackCount=") + wxString::Format(wxT("%u"), extraAttacks));
		unsigned k = 0;
		bool firstAtk = true;
		for (Attack &atk : m_attacks) {
			if (firstAtk) {
				firstAtk = false;
				continue;
			}
			k++;
			wxString attackName = pipeNr + wxT("Attack") + GOODF_functions::number_format(k);
			wxString fullLine = GOODF_functions::fixSeparator(attackName + wxT("=") + atk.fileName);
			outFile->AddLine(fullLine);

			writeLoadRelease(outFile, attackName, atk);
			writeAttackVelocity(outFile, attackName, atk);
			writeMaxTimeSinceLastRelease(outFile, attackName, atk);
			writeIsTremulant(outFile, attackName, atk);
			writeMaxKeyPressTime(outFile, attackName, atk);
			writeAttackStart(outFile, attackName, atk);
			writeCuePoint(outFile, attackName, atk);
			writeReleaseEnd(outFile, attackName, atk);
			writeLoops(outFile, attackName, atk);
		}
	}
}

void Pipe::writeAdditionalReleases(wxTextFile *outFile, wxString pipeNr) {
	// Deal with possible additional releases
	if (!m_releases.empty()) {
		unsigned extraReleases = m_releases.size();
		outFile->AddLine(pipeNr + wxT("ReleaseCount=") + wxString::Format(wxT("%u"), extraReleases));
		unsigned k = 0;
		for (Release rel : m_releases) {
			k++;
			wxString releaseName = pipeNr + "Release" + GOODF_functions::number_format(k);
			wxString fullLine = GOODF_functions::fixSeparator(releaseName + "=" + rel.fileName);
			outFile->AddLine(fullLine);

			if (rel.isTremulant != -1)
				outFile->AddLine(releaseName + wxT("IsTremulant=") + wxString::Format(wxT("%i"), rel.isTremulant));

			if (rel.maxKeyPressTime != -1)
				outFile->AddLine(releaseName + wxT("MaxKeyPressTime=") + wxString::Format(wxT("%i"), rel.maxKeyPressTime));

			if (rel.cuePoint != -1)
				outFile->AddLine(releaseName + wxT("CuePoint=") + wxString::Format(wxT("%i"), rel.cuePoint));

			if (rel.releaseEnd != -1)
				outFile->AddLine(releaseName + wxT("ReleaseEnd=") + wxString::Format(wxT("%i"), rel.releaseEnd));
		}
	}
}

void Pipe::writeRef(wxTextFile *outFile, wxString pipeNr) {
	outFile->AddLine(pipeNr + wxT("=") + m_attacks.front().fileName);
}

/*
wxString Pipe::fixSeparator(wxString pathToCheck) {
	if (pathToCheck.Contains("/")) {
		pathToCheck.Replace("/", "\\");
	}

	return pathToCheck;
}
*/

void Pipe::writeLoadRelease(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (!isPercussive) {
		if (atk.fullPath != wxT("DUMMY")) {
			// Load release is default Y for non percussive so we only need to care if it's false
			if (!atk.loadRelease)
				outFile->AddLine(pipeNr + wxT("LoadRelease=N"));
		}
	}
}

void Pipe::writeAttackVelocity(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.attackVelocity != 0)
		outFile->AddLine(pipeNr + wxT("AttackVelocity=") + wxString::Format(wxT("%i"), atk.attackVelocity));
}

void Pipe::writeMaxTimeSinceLastRelease(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.maxTimeSinceLastRelease != -1)
		outFile->AddLine(pipeNr + wxT("MaxTimeSinceLastRelease=") + wxString::Format(wxT("%i"), atk.maxTimeSinceLastRelease));
}

void Pipe::writeIsTremulant(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.isTremulant != -1)
		outFile->AddLine(pipeNr + wxT("IsTremulant=") + wxString::Format(wxT("%i"), atk.isTremulant));
}

void Pipe::writeMaxKeyPressTime(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.maxKeyPressTime != -1)
		outFile->AddLine(pipeNr + wxT("MaxKeyPressTime=") + wxString::Format(wxT("%i"), atk.maxKeyPressTime));
}

void Pipe::writeAttackStart(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.attackStart != 0)
		outFile->AddLine(pipeNr + wxT("AttackStart=") + wxString::Format(wxT("%i"), atk.attackStart));
}

void Pipe::writeCuePoint(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.cuePoint != -1)
		outFile->AddLine(pipeNr + wxT("CuePoint=") + wxString::Format(wxT("%i"), atk.cuePoint));
}

void Pipe::writeReleaseEnd(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.releaseEnd != -1)
		outFile->AddLine(pipeNr + wxT("ReleaseEnd=") + wxString::Format(wxT("%i"), atk.releaseEnd));
}

void Pipe::writeLoops(wxTextFile *outFile, wxString pipeNr, Attack &atk) {
	if (!atk.m_loops.empty()) {
		unsigned nbLoops = atk.m_loops.size();
		outFile->AddLine(pipeNr + wxT("LoopCount=") + wxString::Format(wxT("%u"), nbLoops));
		unsigned counter = 0;
		for (Loop l : atk.m_loops) {
			counter++;
			wxString formattedLoopNr = GOODF_functions::number_format(counter);
			outFile->AddLine(pipeNr + wxT("Loop") + formattedLoopNr + wxT("Start=") + wxString::Format(wxT("%i"), l.start));
			outFile->AddLine(pipeNr + wxT("Loop") + formattedLoopNr + wxT("End=") + wxString::Format(wxT("%i"), l.end));
		}
	}
}

void Pipe::updateRelativePaths() {
	for (Attack& a : m_attacks) {
		a.fileName = GOODF_functions::removeBaseOdfPath(a.fullPath);
	}
	for (Release& r : m_releases) {
		r.fileName = GOODF_functions::removeBaseOdfPath(r.fullPath);
	}
}
