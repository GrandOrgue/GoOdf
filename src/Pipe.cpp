/*
 * Pipe.cpp is part of GoOdf.
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

#include "Pipe.h"
#include "GOODFFunctions.h"

Pipe::Pipe() {
	isPercussive = false;
	hasIndependentRelease = false;
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
}

Pipe::Pipe(const Pipe& p) {
	isPercussive = p.isPercussive;
	hasIndependentRelease = p.hasIndependentRelease;
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
			if (isPercussive) {
				outFile->AddLine(pipeNr + wxT("Percussive=Y"));
				if (hasIndependentRelease != parent->isIndependentRelease()) {
					if (hasIndependentRelease)
						outFile->AddLine(pipeNr + wxT("HasIndependentRelease=Y"));
					else
						outFile->AddLine(pipeNr + wxT("HasIndependentRelease=N"));
				}
			} else
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
		writeLoopXfade(outFile, pipeNr, m_attacks.front());
		writeReleaseXfade(outFile, pipeNr, m_attacks.front());

		if ((harmonicNumber != 8 && harmonicNumber != parent->getHarmonicNumber()) || (harmonicNumber == 8 && harmonicNumber != parent->getHarmonicNumber()))
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

	} else {
		writeRef(outFile, pipeNr);
	}
}

void Pipe::read(wxFileConfig *cfg, wxString pipeNr, Rank *parent, Organ *readOrgan) {
	wxString cfgBoolValue = cfg->Read(pipeNr + wxT("Percussive"), wxEmptyString);
	isPercussive = GOODF_functions::parseBoolean(cfgBoolValue, parent->isPercussive());
	cfgBoolValue = cfg->Read(pipeNr + wxT("HasIndependentRelease"), wxEmptyString);
	hasIndependentRelease = GOODF_functions::parseBoolean(cfgBoolValue, parent->isIndependentRelease());
	float ampLvl = static_cast<float>(cfg->ReadDouble(pipeNr + wxT("AmplitudeLevel"), 100.0f));
	if (ampLvl >= 0 && ampLvl <= 1000) {
		amplitudeLevel = ampLvl;
	}
	float gainValue = static_cast<float>(cfg->ReadDouble(pipeNr + wxT("Gain"), 0.0f));
	if (gainValue >= -120 && gainValue <= 40) {
		gain = gainValue;
	}
	float pitchT = static_cast<float>(cfg->ReadDouble(pipeNr + wxT("PitchTuning"), 0.0f));
	if (pitchT >= -1800 && pitchT <= 1800) {
		pitchTuning = pitchT;
	}
	int trackerDly = static_cast<int>(cfg->ReadLong(pipeNr + wxT("TrackerDelay"), 0));
	if (trackerDly > -1 && trackerDly < 10001) {
		trackerDelay = trackerDly;
	}
	int harmNbr = static_cast<int>(cfg->ReadLong(pipeNr + wxT("HarmonicNumber"), parent->getHarmonicNumber()));
	if (harmNbr > 0 && harmNbr < 1025) {
		harmonicNumber = harmNbr;
	}
	int keyNbr = static_cast<int>(cfg->ReadLong(pipeNr + wxT("MIDIKeyNumber"), -1));
	if (keyNbr > -1 && keyNbr < 128) {
		midiKeyNumber = keyNbr;
	}
	float pitchFrac = static_cast<float>(cfg->ReadDouble(pipeNr + wxT("MIDIPitchFraction"), -1.0f));
	if (pitchFrac >= 0 && pitchFrac <= 100) {
		midiPitchFraction = pitchFrac;
	}
	float pitchCorr = static_cast<float>(cfg->ReadDouble(pipeNr + wxT("PitchCorrection"), 0.0f));
	if (pitchCorr >= -1800 && pitchCorr <= 1800) {
		pitchCorrection = pitchCorr;
	}
	int windchestRef = static_cast<int>(cfg->ReadLong(pipeNr + wxT("WindchestGroup"), 0));
	if (windchestRef > 0 && windchestRef <= (int) readOrgan->getNumberOfWindchestgroups()) {
		windchest = readOrgan->getOrganWindchestgroupAt(windchestRef - 1);
	} else {
		windchest = parent->getWindchest();
	}
	float minVelocity = static_cast<float>(cfg->ReadDouble(pipeNr + wxT("MinVelocityVolume"), parent->getMinVelocityVolume()));
	if (minVelocity >= 0 && minVelocity <= 1000) {
		minVelocityVolume = minVelocity;
	}
	float maxVelocity = static_cast<float>(cfg->ReadDouble(pipeNr + wxT("MaxVelocityVolume"), parent->getMaxVelocityVolume()));
	if (maxVelocity >= 0 && maxVelocity <= 1000) {
		maxVelocityVolume = maxVelocity;
	}
	wxString retuningStr = cfg->Read(pipeNr + wxT("AcceptsRetuning"), wxEmptyString);
	acceptsRetuning = GOODF_functions::parseBoolean(retuningStr, parent->doesAcceptsRetuning());

	// the main attack is added first
	readAttack(cfg, pipeNr, readOrgan);
	// next any additional attacks
	int nbrExtraAtks = static_cast<int>(cfg->ReadLong(pipeNr + wxT("AttackCount"), 0));
	if (nbrExtraAtks > 0 && nbrExtraAtks < 101) {
		for (int atk = 0; atk < nbrExtraAtks; atk++) {
			wxString atkStr = pipeNr + wxT("Attack") + GOODF_functions::number_format(atk + 1);
			readAttack(cfg, atkStr, readOrgan);
		}
	}

	int nbrExtraRel = static_cast<int>(cfg->ReadLong(pipeNr + wxT("ReleaseCount"), 0));
	// Releases should only exist for pipes that are not percussive without independent release!
	if ((nbrExtraRel > 0 && nbrExtraRel < 101 && !isPercussive) ||
		(nbrExtraRel > 0 && nbrExtraRel < 101 && isPercussive && hasIndependentRelease)
		) {
		for (int rel = 0; rel < nbrExtraRel; rel++) {
			wxString relStr = pipeNr + wxT("Release") + GOODF_functions::number_format(rel + 1);
			wxString relPath = cfg->Read(relStr, wxEmptyString);
			wxString fullRelPath = GOODF_functions::checkIfFileExist(relPath, readOrgan);
			if (fullRelPath != wxEmptyString) {
				int isTrem = static_cast<int>(cfg->ReadLong(relStr + wxT("IsTremulant"), -1));
				int maxKeyPress = static_cast<int>(cfg->ReadLong(relStr + wxT("MaxKeyPressTime"), -1));
				int cuePoint = static_cast<int>(cfg->ReadLong(relStr + wxT("CuePoint"), -1));
				int relEnd = static_cast<int>(cfg->ReadLong(relStr + wxT("ReleaseEnd"), -1));
				int relXfade = static_cast<int>(cfg->ReadLong(relStr + wxT("ReleaseCrossfadeLength"), 0));
				Release r;
				r.fileName = relPath;
				r.fullPath = fullRelPath;
				if (isTrem > -2 && isTrem < 2)
					r.isTremulant = isTrem;
				if (maxKeyPress > -2 && maxKeyPress < 100001)
					r.maxKeyPressTime = maxKeyPress;
				if (cuePoint > -2 && cuePoint < 158760001)
					r.cuePoint = cuePoint;
				if (relEnd > -2 && relEnd < 158760001)
					r.releaseEnd = relEnd;
				if (relXfade > 0 && relXfade < 3001)
					r.releaseCrossfadeLength = relXfade;

				m_releases.push_back(r);
			}
		}
	} else if (nbrExtraRel > 0 && isPercussive) {
		if (nbrExtraRel > 1)
			wxLogWarning("Separate releases found in %s %s that is percussive! Ignoring them.", parent->getName(), pipeNr);
		else
			wxLogWarning("Separate release found in %s %s that is percussive! Ignoring it.", parent->getName(), pipeNr);
		::wxGetApp().m_frame->GetLogWindow()->Show(true);
	}

	// finally a sanity check to see that there is at least one valid attack in the pipe
	if (m_attacks.empty()) {
		Attack a;
		a.fileName = wxT("DUMMY");
		a.fullPath = wxT("DUMMY");
		m_attacks.push_back(a);
		wxLogWarning("No valid pipe could be added for %s %s! Setting it to DUMMY.", parent->getName(), pipeNr);
		::wxGetApp().m_frame->GetLogWindow()->Show(true);
	} else {
		// update the pipes root path of parent rank from the main attack
		wxFileName fileName = m_attacks.front().fullPath;
		wxString pipePath = fileName.GetPath();
		parent->setPipesRootPath(pipePath);
	}
}

void Pipe::readAttack(wxFileConfig *cfg, wxString pipeStr, Organ *readOrgan) {
	wxString mainAtkStr = cfg->Read(pipeStr, wxEmptyString);
	if (mainAtkStr != wxEmptyString) {
		// the pipe can have a relative path to a sample file or start with REF
		wxString fullAtkPath = GOODF_functions::checkIfFileExist(mainAtkStr, readOrgan);
		if (fullAtkPath != wxEmptyString) {
			wxString loadReleaseStr = cfg->Read(pipeStr + wxT("LoadRelease"), wxEmptyString);
			int atkVel = static_cast<int>(cfg->ReadLong(pipeStr + wxT("AttackVelocity"), 0));
			int maxTime = static_cast<int>(cfg->ReadLong(pipeStr + wxT("MaxTimeSinceLastRelease"), -1));
			int isTrem = static_cast<int>(cfg->ReadLong(pipeStr + wxT("IsTremulant"), -1));
			int maxKeyPress = static_cast<int>(cfg->ReadLong(pipeStr + wxT("MaxKeyPressTime"), -1));
			int atkStart = static_cast<int>(cfg->ReadLong(pipeStr + wxT("AttackStart"), 0));
			int cuePoint = static_cast<int>(cfg->ReadLong(pipeStr + wxT("CuePoint"), -1));
			int relEnd = static_cast<int>(cfg->ReadLong(pipeStr + wxT("ReleaseEnd"), -1));
			int loops = static_cast<int>(cfg->ReadLong(pipeStr + wxT("LoopCount"), 0));
			if (loops < 0)
				loops = 0;
			if (loops > 100)
				loops = 100;
			Attack a;
			a.fileName = GOODF_functions::removeBaseOdfPath(fullAtkPath);
			a.fullPath = fullAtkPath;
			a.loadRelease = GOODF_functions::parseBoolean(loadReleaseStr, !isPercussive);
			if (atkVel > -1 && atkVel < 128)
				a.attackVelocity = atkVel;
			if (maxTime > -2 && maxTime < 100001)
				a.maxTimeSinceLastRelease = maxTime;
			if (isTrem > -2 && isTrem < 2)
				a.isTremulant = isTrem;
			if (maxKeyPress > -2 && maxKeyPress < 100001)
				a.maxKeyPressTime = maxKeyPress;
			if (atkStart > -1 && atkStart < 158760001)
				a.attackStart = atkStart;

			// If the pipe is percussive the next keys=values should not exist and we shouldn't read them
			if (!isPercussive) {
				if (cuePoint > -2 && cuePoint < 158760001)
					a.cuePoint = cuePoint;
				if (relEnd > -2 && relEnd < 158760001)
					a.releaseEnd = relEnd;
				for (int i = 0; i < loops; i++) {
					Loop l;
					wxString loopId = wxT("Loop") + GOODF_functions::number_format(i + 1);
					int loopStart = static_cast<int>(cfg->ReadLong(pipeStr + loopId + wxT("Start"), 0));
					if (loopStart > -1 && loopStart < 158760001)
						l.start = loopStart;
					else
						l.start = 0;
					int loopEnd = static_cast<int>(cfg->ReadLong(pipeStr + loopId + wxT("End"), 1));
					if (loopEnd > l.start + 1 && loopEnd < 158760001)
						l.end = loopEnd;
					else
						l.end = l.start + 1;
					a.addNewLoop(l);
				}
				int loopXfade = static_cast<int>(cfg->ReadLong(pipeStr + wxT("LoopCrossfadeLength"), 0));
				if (loopXfade > 0 && loopXfade < 3001) {
					a.loopCrossfadeLength = loopXfade;
				}
				int relXfade = static_cast<int>(cfg->ReadLong(pipeStr + wxT("ReleaseCrossfadeLength"), 0));
				if (relXfade > 0 && relXfade < 3001) {
					a.releaseCrossfadeLength = relXfade;
				}
			}
			m_attacks.push_back(a);
		} else if (mainAtkStr.StartsWith(wxT("REF")) || mainAtkStr.IsSameAs(wxT("DUMMY"), false)) {
			Attack a;
			a.fileName = mainAtkStr;
			a.fullPath = mainAtkStr;
			m_attacks.push_back(a);
		}
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
			wxString fullLine = GOODF_functions::fixSeparator(attackName + wxT("=") + GOODF_functions::removeBaseOdfPath(atk.fileName));
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
			writeLoopXfade(outFile, attackName, atk);
			writeReleaseXfade(outFile, attackName, atk);
		}
	}
}

void Pipe::writeAdditionalReleases(wxTextFile *outFile, wxString pipeNr) {
	// Deal with possible additional releases if not a percussive pipe
	if ((!m_releases.empty() && !isPercussive) ||
		(!m_releases.empty() && isPercussive && hasIndependentRelease)
		) {
		unsigned extraReleases = m_releases.size();
		outFile->AddLine(pipeNr + wxT("ReleaseCount=") + wxString::Format(wxT("%u"), extraReleases));
		unsigned k = 0;
		for (Release rel : m_releases) {
			k++;
			wxString releaseName = pipeNr + "Release" + GOODF_functions::number_format(k);
			wxString fullLine = GOODF_functions::fixSeparator(releaseName + "=" + GOODF_functions::removeBaseOdfPath(rel.fileName));
			outFile->AddLine(fullLine);

			if (rel.isTremulant != -1)
				outFile->AddLine(releaseName + wxT("IsTremulant=") + wxString::Format(wxT("%i"), rel.isTremulant));

			if (rel.maxKeyPressTime != -1)
				outFile->AddLine(releaseName + wxT("MaxKeyPressTime=") + wxString::Format(wxT("%i"), rel.maxKeyPressTime));

			if (rel.cuePoint != -1)
				outFile->AddLine(releaseName + wxT("CuePoint=") + wxString::Format(wxT("%i"), rel.cuePoint));

			if (rel.releaseEnd != -1)
				outFile->AddLine(releaseName + wxT("ReleaseEnd=") + wxString::Format(wxT("%i"), rel.releaseEnd));

			if (rel.releaseCrossfadeLength)
				outFile->AddLine(releaseName + wxT("ReleaseCrossfadeLength=") + wxString::Format(wxT("%i"), rel.releaseCrossfadeLength));
		}
	}
}

void Pipe::writeRef(wxTextFile *outFile, wxString pipeNr) {
	outFile->AddLine(pipeNr + wxT("=") + m_attacks.front().fileName);
}

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
	if (atk.cuePoint != -1 && !isPercussive)
		outFile->AddLine(pipeNr + wxT("CuePoint=") + wxString::Format(wxT("%i"), atk.cuePoint));
}

void Pipe::writeReleaseEnd(wxTextFile *outFile, wxString pipeNr, Attack atk) {
	if (atk.releaseEnd != -1 && !isPercussive)
		outFile->AddLine(pipeNr + wxT("ReleaseEnd=") + wxString::Format(wxT("%i"), atk.releaseEnd));
}

void Pipe::writeLoops(wxTextFile *outFile, wxString pipeNr, Attack &atk) {
	if (!atk.m_loops.empty() && !isPercussive) {
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

void Pipe::writeLoopXfade(wxTextFile *outFile, wxString pipeNr, Attack &atk) {
	if (atk.loopCrossfadeLength && !isPercussive)
		outFile->AddLine(pipeNr + wxT("LoopCrossfadeLength=") + wxString::Format(wxT("%i"), atk.loopCrossfadeLength));
}

void Pipe::writeReleaseXfade(wxTextFile *outFile, wxString pipeNr, Attack &atk) {
	if (atk.loadRelease && atk.releaseCrossfadeLength && !isPercussive)
		outFile->AddLine(pipeNr + wxT("ReleaseCrossfadeLength=") + wxString::Format(wxT("%i"), atk.releaseCrossfadeLength));
}

void Pipe::updateRelativePaths() {
	for (Attack& a : m_attacks) {
		a.fileName = GOODF_functions::removeBaseOdfPath(a.fullPath);
	}
	for (Release& r : m_releases) {
		r.fileName = GOODF_functions::removeBaseOdfPath(r.fullPath);
	}
}

bool Pipe::isIndependentRelease() {
	return hasIndependentRelease;
}

void Pipe::setIndependentRelease(bool independent) {
	this->hasIndependentRelease = independent;

	if (!independent && isPercussive) {
		if (!m_releases.empty())
			m_releases.clear();
	}
}

// An attack or release is expected to have either no samples,
//   only samples with no tremulant specfified (without Pipe999IsTremulant=),
//   or only samples with tremulant specified (with both Pipe999IsTremulant=0 and Pipe999IsTremulant=1)

#define EXPECTED_TREMULANT(nulls, wavs, nonwavs) ( \
	(((nulls) + (wavs) + (nonwavs)) == 0) || \
	((nulls) > 0 && (wavs) == 0 && (nonwavs) == 0) || \
	((nulls) == 0 && (wavs) > 0 && (nonwavs) > 0) \
	)

bool Pipe::hasUnusualTremulants() {
	int natnulls = 0;		// attacks with no isTremulant
	int natwavs = 0;		// attacks with wave isTremulant=1
	int natnonwavs = 0;		// attacks with wave isTremulant=0
	int nrelnulls = 0;
	int nrelwavs = 0;
	int nrelnonwavs = 0;

	for (Attack atk : this->m_attacks) {
		if (atk.isTremulant == -1) {
			natnulls++;
		} else if (atk.isTremulant == 1) {
			natwavs++;
		} else {
			natnonwavs++;
		}
	}

	for (Release rel : this->m_releases) {
		if (rel.isTremulant == -1) {
			nrelnulls++;
		} else if (rel.isTremulant == 1) {
			nrelwavs++;
		} else {
			nrelnonwavs++;
		}
	}

	if (EXPECTED_TREMULANT(natnulls, natwavs, natnonwavs) && EXPECTED_TREMULANT(nrelnulls, nrelwavs, nrelnonwavs)) {
		return false;
	} else {
		return true;
	}
}
