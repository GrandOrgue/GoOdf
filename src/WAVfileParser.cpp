/*
 * WAVfileParser.cpp is part of GOODF.
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

#include "WAVfileParser.h"
#include <climits>

wxString const WAVE_RIFF = wxT("RIFF");
wxString const WAVE_ID = wxT("WAVE");
wxString const WAVE_FMT = wxT("fmt ");
wxString const WAVE_DATA = wxT("data");
wxString const WVPK_ID = wxT("wvpk");

WAVfileParser::WAVfileParser(wxString file) {
	m_wavpackUsed = false;
	m_fileName = file;
	m_errorMessage = wxEmptyString;

	if (tryParsingFile(m_fileName))
		m_wavOk = true;
	else
		m_wavOk = false;

	if (m_wavpackUsed) {
		if (tryParsingWvFile(m_fileName))
			m_wavOk = true;
		else
			m_wavOk = false;
	}
}

WAVfileParser::~WAVfileParser() {
}

bool WAVfileParser::isWavOk() {
	return m_wavOk;
}

unsigned WAVfileParser::getNumberOfFrames() {
	return m_numberOfFrames;
}

wxString WAVfileParser::getErrorMessage() {
	return m_errorMessage;
}

bool WAVfileParser::tryParsingFile(wxString file) {
	wxFFileInputStream waveFile(file);

	if (waveFile.IsOk()) {
		char fourCBuffer[4];
		unsigned uBuffer;
		m_dataSize = 0;
		bool dataFound = false;
		bool fmtFound = false;
		
		waveFile.Read(fourCBuffer, 4);
		if (waveFile.LastRead() != 4 || !WAVE_RIFF.IsSameAs(fourCBuffer)) {
			if (WVPK_ID.IsSameAs(fourCBuffer))
				m_wavpackUsed = true;
			else
				m_errorMessage = wxT("Not a RIFF file or a wavpack file.");
			return false;
		}

		waveFile.Read(&uBuffer, 4); // filesize - 8 bytes
		if (waveFile.LastRead() != 4) {
			m_errorMessage = wxT("Couldn't read filesize.");
			return false;
		}

		waveFile.Read(fourCBuffer, 4);
		if (waveFile.LastRead() != 4 || !WAVE_ID.IsSameAs(fourCBuffer)) {
			m_errorMessage = wxT("Not a WAVE file.");
			return false;
		}
		
		while (!waveFile.Eof()) {
			// get next fourcc chunk
			waveFile.Read(fourCBuffer, 4);
			if (waveFile.LastRead() == 4) {
				if (WAVE_FMT.IsSameAs(fourCBuffer) && !fmtFound) {
					if (parseFmtChunk(waveFile)) {
						fmtFound = true;
						continue;
					} else {
						// if fmt chunk couldn't be parsed error message should already have been set
						return false;
					}
				} else if (WAVE_DATA.IsSameAs(fourCBuffer) && !dataFound && fmtFound) {
					dataFound = true;
				}

			} else {
				break;
			}

			// get size of chunk so we know how far to skip until next chunk
			waveFile.Read(&uBuffer, 4);
			if (waveFile.LastRead() == 4) {
				if (dataFound && m_dataSize == 0)
					m_dataSize = uBuffer;
			} else {
				break;
			}

			waveFile.SeekI(uBuffer + (uBuffer & 1), wxFromCurrent);
		}
		
		if (fmtFound && dataFound) {
			m_numberOfFrames = m_dataSize / m_BlockAlign;
			return true;
		} else {
			m_errorMessage = wxT("Chunks for fmt and/or data couldn't be found.");
			return false;
		}
	} else {
		m_errorMessage = wxT("Failed to open stream.");
		return false;
	}
}

bool WAVfileParser::tryParsingWvFile(wxString file) {
	wxFFileInputStream wvFile(file);
	
	if (wvFile.IsOk()) {
		char fourCBuffer[4];
		unsigned uBuffer;
		
		wvFile.Read(fourCBuffer, 4);
		if (wvFile.LastRead() != 4 || !WVPK_ID.IsSameAs(fourCBuffer)) {
			return false;
		}
		
		wvFile.SeekI(8, wxFromCurrent);
		
		wvFile.Read(&uBuffer, 4);
		if (wvFile.LastRead() != 4) {
			return false;
		}
		
		m_numberOfFrames = uBuffer;
		
		if (m_numberOfFrames == UINT_MAX)
			return false;
		else
			return true;
	} else {
		return false;
	}
}

bool WAVfileParser::parseFmtChunk(wxFFileInputStream &wavFile) {
	unsigned uBuffer;
	unsigned short uShBuffer;

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() != 4 || uBuffer < 16) {
		m_errorMessage = wxT("Couldn't read fmt chunk size.");
		return false;
	}
	unsigned fmtChunkSize = uBuffer;

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		if(uShBuffer == 1 || uShBuffer == 3 || uShBuffer == 65534)
			m_AudioFormat = uShBuffer; // we only support PCM, IEEE_FLOAT and EXTENSIBLE
		else {
			m_errorMessage = wxT("Unsupported wave format detected.");
			return false;
		}
	} else {
		m_errorMessage = wxT("Couldn't read audio format.");
		return false;
	}

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		m_NumChannels = uShBuffer;
	} else {
		m_errorMessage = wxT("Couldn't read number of channels.");
		return false;
	}

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() == 4) {
		m_SampleRate = uBuffer;
	} else {
		m_errorMessage = wxT("Couldn't read samplerate.");
		return false;
	}

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() == 4) {
		m_ByteRate = uBuffer;
	} else {
		m_errorMessage = wxT("Couldn't read byte rate.");
		return false;
	}

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		m_BlockAlign = uShBuffer;
	} else {
		m_errorMessage = wxT("Couldn't read block align.");
		return false;
	}

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		m_BitsPerSample = uShBuffer;
	} else {
		m_errorMessage = wxT("Couldn't read bits per sample.");
		return false;
	}

	if (m_BlockAlign != (m_NumChannels * m_BitsPerSample / 8)) {
		m_errorMessage = wxT("Block align doesn't match (nChannels*bitsPerSample/8).");
		return false;
	}

	if (fmtChunkSize > 16) {
		unsigned bytesToSkip = fmtChunkSize - 16;
		wavFile.SeekI(bytesToSkip + (bytesToSkip & 1), wxFromCurrent);
		return true;
	}

	return true;
}
