/*
 * WAVfileParser.h is part of GoOdf.
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

#ifndef WAVFILEPARSER_H
#define WAVFILEPARSER_H

#include <wx/wx.h>
#include <wx/wfstream.h>
#include <vector>

extern wxString const WAVE_RIFF;
extern wxString const WAVE_ID;
extern wxString const WAVE_FMT;
extern wxString const WAVE_DATA;
extern wxString const WAVE_SMPL;
extern wxString const WAVE_CUE;
extern wxString const WAVE_LIST;
extern wxString const WAVE_INFO;
extern wxString const WVPK_ID;

struct CUEPOINT {
	unsigned dwName;
	unsigned dwSampleOffset;
};

struct LOOP {
	unsigned dwIdentifier;
	unsigned dwStart;
	unsigned dwEnd;
};

class WAVfileParser {

public:
	WAVfileParser(wxString file);
	~WAVfileParser();
	
	bool isWavOk();
	bool isWavPacked();
	unsigned getNumberOfFrames();
	wxString getErrorMessage();
	unsigned getNumberOfChannels();
	unsigned getSampleRate();
	unsigned getNumberOfCues();
	CUEPOINT getCuepointAtIndex(unsigned index);
	unsigned getNumberOfLoops();
	LOOP getLoopAtIndex(unsigned index);
	unsigned getBitsPerSample();
	unsigned getAudioFormat();
	unsigned getInfoListSize();
	std::pair<wxString, wxString> getInfoListContentAtIndex(unsigned index);
	unsigned getMidiNote();
	unsigned getPitchFraction();
	double getPitchFractionCents();
	double getPitchInHz();

private:
	bool m_wavOk;
	bool m_wavpackUsed;
	wxString m_fileName;
	wxString m_errorMessage;
	unsigned short m_AudioFormat; // WAVE_FORMAT_PCM = 1, WAVE_FORMAT_IEE_FLOAT = 3, WAVE_FORMAT_EXTENSIBLE = 65534
	unsigned short m_NumChannels;
	unsigned m_SampleRate;
	unsigned m_ByteRate;
	unsigned short m_BlockAlign;
	unsigned short m_BitsPerSample;
	unsigned m_dataSize;
	unsigned m_numberOfFrames;
	unsigned m_dwMIDIUnityNote;
	unsigned m_dwMIDIPitchFraction;
	std::vector<CUEPOINT> m_cues;
	std::vector<LOOP> m_loops;
	std::vector<std::pair<wxString, wxString>> m_infoList;
	unsigned m_lastChunkSizeParsed;
	
	bool tryParsingFile(wxString file);
	bool tryParsingWvFile(wxString file);
	bool parseFmtChunk(wxFFileInputStream &wavFile);
	bool parseSmplChunk(wxFFileInputStream &wavFile);
	bool parseCueChunk(wxFFileInputStream &wavFile);
	bool parseInfoListChunk(wxFFileInputStream &wavFile);
};

#endif
