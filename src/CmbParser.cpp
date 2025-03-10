/*
 * CmbParser.h is part of GoOdf.
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

#include "CmbParser.h"
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/zstream.h>

CmbParser::CmbParser(wxString filePath, CMB_ORGAN *cmbOrgan) {
	m_isOk = false;
	m_errorText = wxT("Cmb file not parsed yet!");

	m_isOk = readCmbFile(filePath, cmbOrgan);
	if (m_isOk)
		m_errorText = wxEmptyString;
}

CmbParser::~CmbParser() {

}

bool CmbParser::IsParsedOk() {
	return m_isOk;
}

wxString CmbParser::GetErrorText() {
	return m_errorText;
}

bool CmbParser::readCmbFile(wxString fileName, CMB_ORGAN *cmbOrgan) {
	// convert file parameter to a stream for further parsing
	wxFFileInputStream cmbFile(fileName);

	if (cmbFile.IsOk()) {
		wxZlibInputStream cmbIn(cmbFile, wxZLIB_GZIP);
		if (cmbIn.IsOk()) {
			wxFileConfig iniFile(cmbIn);
			if (iniFile.HasGroup(wxT("Organ"))) {
				// An [Organ] section is found

				parseSection(&iniFile, wxT("Organ"), cmbOrgan);

				bool lookForWindchest = true;
				int windchestNbr = 1;
				while (lookForWindchest) {
					wxString windchestName = wxString::Format(wxT("WindchestGroup%0.3d"), windchestNbr);
					if (parseSection(&iniFile, windchestName, cmbOrgan)) {
						windchestNbr++;
					} else {
						lookForWindchest = false;
					}
				}

				bool lookForRank = true;
				int rankNbr = 1;
				while (lookForRank) {
					wxString rankName = wxString::Format(wxT("Rank%0.3d"), rankNbr);
					if (parseSection(&iniFile, rankName, cmbOrgan)) {
						rankNbr++;
					} else {
						lookForRank = false;
					}
				}

				bool lookForStop = true;
				int stopNbr = 1;
				while (lookForStop) {
					wxString stopName = wxString::Format(wxT("Stop%0.3d"), stopNbr);
					if (parseSection(&iniFile, stopName, cmbOrgan)) {
						stopNbr++;
					} else {
						lookForStop = false;
					}
				}

				return true;
			} else {
				m_errorText = wxT("Couldn't find an [Organ] section in the file!");
				return false;
			}
		} else {
			m_errorText = wxT("Decompression of the stream failed!");
			return false;
		}
	} else {
		m_errorText = wxT("Failed to create stream from filename!\n");
		return false;
	}
}

bool CmbParser::parseString(wxFileConfig *ini, wxString key, wxString *value, wxString defaultValue) {
	return ini->Read(key, value, defaultValue);
}

bool CmbParser::parseInt(wxFileConfig *ini, wxString key, int *value, int defaultValue) {
	long parsedValue = defaultValue;
	bool returnValue = ini->Read(key, &parsedValue, defaultValue);
	*value = static_cast<long>(parsedValue);
	return returnValue;
}

bool CmbParser::parseFloat(wxFileConfig *ini, wxString key, float *value, float defaultValue) {
	return ini->Read(key, value, defaultValue);
}

bool CmbParser::parseSection(wxFileConfig *ini, wxString section, CMB_ORGAN *cmbOrgan) {
	if (ini->HasGroup(section))
		ini->SetPath(wxT("/") + section);
	else
		return false;

	float amplitude;
	parseFloat(ini, wxT("Amplitude"), &amplitude, 100.0f);

	float gain;
	parseFloat(ini, wxT("UserGain"), &gain, 0.0f);

	float pitchTuning;
	parseFloat(ini, wxT("ManualTuning"), &pitchTuning, 0.0f);

	float pitchCorrection;
	parseFloat(ini, wxT("AutoTuningCorrection"), &pitchCorrection, 0.0f);

	int trackerDelay;
	parseInt(ini, wxT("Delay"), &trackerDelay, 0);

	if (!section.IsSameAs(wxT("Organ")) && !section.StartsWith(wxT("WindchestGroup"))) {
		CMB_ELEMENT_WITH_PIPES elementWithPipes;
		elementWithPipes.attributes.amplitude = amplitude;
		elementWithPipes.attributes.gain = gain;
		elementWithPipes.attributes.pitchTuning = pitchTuning;
		elementWithPipes.attributes.pitchCorrection = pitchCorrection;
		elementWithPipes.attributes.trackerDelay = trackerDelay;

		int pipeId = 1;
		while (pipeId < 193) {
			if (hasPipeKey(ini, pipeId))
				elementWithPipes.pipes.push_back(parsePipe(ini, pipeId));

			pipeId++;
		}

		if (section.StartsWith(wxT("Rank"))) {
			cmbOrgan->cmbRanks.push_back(elementWithPipes);
		} else if (section.StartsWith(wxT("Stop"))) {
			cmbOrgan->cmbStops.push_back(elementWithPipes);
		}
	} else if (section.StartsWith(wxT("WindchestGroup"))) {
		CMB_ELEMENT windchest;
		windchest.amplitude = amplitude;
		windchest.gain = gain;
		windchest.pitchTuning = pitchTuning;
		windchest.pitchCorrection = pitchCorrection;
		windchest.trackerDelay = trackerDelay;
		cmbOrgan->cmbWindchests.push_back(windchest);
	} else {
		wxString churchName;
		parseString(ini, wxT("ChurchName"), &churchName, wxEmptyString);
		cmbOrgan->churchName = churchName;

		wxString odfPath;
		parseString(ini, wxT("ODFPath"), &odfPath, wxEmptyString);
		cmbOrgan->odfPath = odfPath;

		cmbOrgan->attributes.amplitude = amplitude;
		cmbOrgan->attributes.gain = gain;
		cmbOrgan->attributes.pitchTuning = pitchTuning;
		cmbOrgan->attributes.pitchCorrection = pitchCorrection;
		cmbOrgan->attributes.trackerDelay = trackerDelay;
	}

	ini->SetPath("/");
	return true;
}

bool CmbParser::hasPipeKey(wxFileConfig *ini, int pipeIndex) {
	wxString pipeStr = wxString::Format(wxT("Pipe%0.3d"), pipeIndex);
	if (ini->HasEntry(pipeStr + wxT("Amplitude")))
		return true;
	else
		return false;
}

CMB_PIPE CmbParser::parsePipe(wxFileConfig *ini, int pipeIndex) {
	wxString pipeStr = wxString::Format(wxT("Pipe%0.3d"), pipeIndex);

	float amplitude;
	parseFloat(ini, pipeStr + wxT("Amplitude"), &amplitude, 100.0f);

	float gain;
	parseFloat(ini, pipeStr + wxT("UserGain"), &gain, 0.0f);

	float pitchTuning;
	parseFloat(ini, pipeStr + wxT("ManualTuning"), &pitchTuning, 0.0f);

	float pitchCorrection;
	parseFloat(ini, pipeStr + wxT("AutoTuningCorrection"), &pitchCorrection, 0.0f);

	int trackerDelay;
	parseInt(ini, pipeStr + wxT("Delay"), &trackerDelay, 0);

	CMB_PIPE thePipe;
	thePipe.pipeNbr = pipeIndex;
	thePipe.attributes.amplitude = amplitude;
	thePipe.attributes.gain = gain;
	thePipe.attributes.pitchTuning = pitchTuning;
	thePipe.attributes.pitchCorrection = pitchCorrection;
	thePipe.attributes.trackerDelay = trackerDelay;

	return thePipe;
}
