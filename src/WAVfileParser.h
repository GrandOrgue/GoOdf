/*
 * WAVfileParser.h is part of GOODF.
 * Copyright (C) 2023 Lars Palo and contributors (see AUTHORS)
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

#ifndef WAVFILEPARSER_H
#define WAVFILEPARSER_H

#include <wx/wx.h>
#include <wx/wfstream.h>

extern wxString const WAVE_RIFF;
extern wxString const WAVE_ID;
extern wxString const WAVE_FMT;
extern wxString const WAVE_DATA;
extern wxString const WVPK_ID;

class WAVfileParser {

public:
	WAVfileParser(wxString file);
	~WAVfileParser();
	
	bool isWavOk();
	unsigned getNumberOfFrames();
	wxString getErrorMessage();

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
	
	bool tryParsingFile(wxString file);
	bool tryParsingWvFile(wxString file);
	bool parseFmtChunk(wxFFileInputStream &wavFile);
};

#endif

