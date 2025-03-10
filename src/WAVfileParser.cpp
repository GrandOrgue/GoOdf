/*
 * WAVfileParser.cpp is part of GoOdf.
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

#include "WAVfileParser.h"
#include <climits>
#include <cstdint>

wxString const WAVE_RIFF = wxT("RIFF");
wxString const WAVE_ID = wxT("WAVE");
wxString const WAVE_FMT = wxT("fmt ");
wxString const WAVE_DATA = wxT("data");
wxString const WAVE_SMPL = wxT("smpl");
wxString const WAVE_CUE = wxT("cue ");
wxString const WAVE_LIST = wxT("LIST");
wxString const WAVE_INFO = wxT("INFO");
wxString const WVPK_ID = wxT("wvpk");

WAVfileParser::WAVfileParser(wxString file) {
	m_wavpackUsed = false;
	m_fileName = file;
	m_errorMessage = wxEmptyString;
	m_AudioFormat = 0;
	m_NumChannels = 0;
	m_SampleRate = 0;
	m_ByteRate = 0;
	m_BlockAlign = 0;
	m_BitsPerSample = 0;
	m_dataSize = 0;
	m_numberOfFrames = 0;
	m_dwMIDIUnityNote = 0;
	m_dwMIDIPitchFraction = 0;
	m_lastChunkSizeParsed = 0;

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

bool WAVfileParser::isWavPacked() {
	return m_wavpackUsed;
}

unsigned WAVfileParser::getNumberOfFrames() {
	return m_numberOfFrames;
}

wxString WAVfileParser::getErrorMessage() {
	return m_errorMessage;
}

unsigned WAVfileParser::getNumberOfChannels() {
	return (unsigned) m_NumChannels;
}

unsigned WAVfileParser::getSampleRate() {
	return m_SampleRate;
}

unsigned WAVfileParser::getNumberOfCues() {
	return m_cues.size();
}

CUEPOINT WAVfileParser::getCuepointAtIndex(unsigned index) {
	return m_cues[index];
}

unsigned WAVfileParser::getNumberOfLoops() {
	return m_loops.size();
}

LOOP WAVfileParser::getLoopAtIndex(unsigned index) {
	return m_loops[index];
}

unsigned WAVfileParser::getBitsPerSample() {
	return m_BitsPerSample;
}

unsigned WAVfileParser::getAudioFormat() {
	return (unsigned) m_AudioFormat;
}

unsigned WAVfileParser::getInfoListSize() {
	return m_infoList.size();
}

std::pair<wxString, wxString> WAVfileParser::getInfoListContentAtIndex(unsigned index) {
	return m_infoList[index];
}

unsigned WAVfileParser::getMidiNote() {
	return m_dwMIDIUnityNote;
}

unsigned WAVfileParser::getPitchFraction() {
	return m_dwMIDIPitchFraction;
}

double WAVfileParser::getPitchFractionCents() {
	return (double) m_dwMIDIPitchFraction / (double)UINT_MAX * 100.0;
}

double WAVfileParser::getPitchInHz() {
	double cents = getPitchFractionCents();
	int midiNote = m_dwMIDIUnityNote;
	double midiNotePitch = 440.0 * pow(2, ((double)(midiNote - 69) / 12.0));
	double resultingPitch = midiNotePitch * pow(2, (cents / 1200.0));
	return resultingPitch;
}

bool WAVfileParser::tryParsingFile(wxString file) {
	wxFFileInputStream waveFile(file);

	if (waveFile.IsOk()) {
		// C-style string (array of chars) with fixed size must be zero-terminated explicitly.
		// This is used by .IsSameAs() when a wxString is implicitly created at the call.
		// For the fourCBuffer we're thus using five elements in the array and initializing them all to zero,
		// even though we'll only ever read four characters into it.
		char fourCBuffer[5] = {};
		unsigned uBuffer;
		m_dataSize = 0;
		bool dataFound = false;
		bool fmtFound = false;
		bool smplFound = false;
		bool cueFound = false;

		waveFile.Read(fourCBuffer, 4);
		if (waveFile.LastRead() != 4 || !WAVE_RIFF.IsSameAs(fourCBuffer)) {
			if (WVPK_ID.IsSameAs(fourCBuffer))
				m_wavpackUsed = true;
			else
				m_errorMessage = wxT("Not a RIFF file or a WavPack file.\n");
			return false;
		}

		waveFile.Read(&uBuffer, 4); // filesize - 8 bytes
		if (waveFile.LastRead() != 4) {
			m_errorMessage = wxT("Couldn't read filesize.\n");
			return false;
		}

		waveFile.Read(fourCBuffer, 4);
		if (waveFile.LastRead() != 4 || !WAVE_ID.IsSameAs(fourCBuffer)) {
			m_errorMessage = wxT("Not a WAVE file.\n");
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
				} else if (WAVE_SMPL.IsSameAs(fourCBuffer) && !smplFound) {
					if (parseSmplChunk(waveFile)) {
						smplFound = true;
						continue;
					} else {
						// if chunk couldn't be parsed error message should already have been set
						return false;
					}
				} else if (WAVE_CUE.IsSameAs(fourCBuffer) && !cueFound) {
					if (parseCueChunk(waveFile)) {
						cueFound = true;
						continue;
					} else {
						// if chunk couldn't be parsed error message should already have been set
						return false;
					}
				} else if (WAVE_LIST.IsSameAs(fourCBuffer)) {
					if (parseInfoListChunk(waveFile)) {
						continue;
					} else {
						// if chunk couldn't be parsed error message should already have been set
						return false;
					}
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
			m_errorMessage += wxT("Chunks for fmt and/or data couldn't be found.\n");
			return false;
		}
	} else {
		m_errorMessage += wxT("Failed to open stream.\n");
		return false;
	}
}

bool WAVfileParser::tryParsingWvFile(wxString file) {
	wxFFileInputStream wvFile(file);

	if (wvFile.IsOk()) {
		char fourCBuffer[5] = {};
		unsigned uBuffer;
		unsigned char uChBuffer;
		unsigned blockNumber = 0;
		unsigned wavpackSamplerates[16] = {
			6000,
			8000,
			9600,
			11025,
			12000,
			16000,
			22050,
			24000,
			32000,
			44100,
			48000,
			64000,
			88200,
			96000,
			192000,
			0
		};
		bool fmtFound = false;
		bool smplFound = false;
		bool cueFound = false;

		while (!wvFile.Eof()) {
			blockNumber++;

			// First in each block always comes a WavPack 32 byte sized header
			wvFile.Read(fourCBuffer, 4);
			if (wvFile.LastRead() != 4 || !WVPK_ID.IsSameAs(fourCBuffer)) {
				if (blockNumber > 1) {
					// If at least one block has already been successfully parsed we can just quit at a failure
					break;
				} else {
					m_errorMessage += wxString::Format(wxT("WavPack block %d have wrong ckID or couldn't be read.\n"), blockNumber);
					return false;
				}
			}

			// WavPack block size doesn't include the ckID or the (unsigned) blockSize itself which makes it equal total blocksize - 8
			unsigned blockSize;
			wvFile.Read(&uBuffer, 4);
			if (wvFile.LastRead() != 4) {
				m_errorMessage += wxT("WavPack block size couldn't be read.\n");
				return false;
			} else
				blockSize = uBuffer;

			unsigned totalBytesRead = 0;
			// we're not interested in the WavPack version
			wvFile.SeekI(2, wxFromCurrent);
			totalBytesRead += 2;

			// we're not really interested in upper block index bits
			wvFile.SeekI(1, wxFromCurrent);
			totalBytesRead += 1;

			unsigned char upperTotalSamplesBits;
			wvFile.Read(&uChBuffer, 1);
			if (wvFile.LastRead() != 1) {
				m_errorMessage += wxT("Upper total sample bits couldn't be read.\n");
				return false;
			} else
				upperTotalSamplesBits = uChBuffer;
			totalBytesRead += 1;

			unsigned lowerTotalSamplesBits;
			wvFile.Read(&uBuffer, 4);
			if (wvFile.LastRead() != 4) {
				m_errorMessage += wxT("Lower total sample bits couldn't be read.\n");
				return false;
			} else {
				lowerTotalSamplesBits = uBuffer;
			}
			totalBytesRead += 4;

			// we're not really interested in the lower block index bits either
			wvFile.SeekI(4, wxFromCurrent);
			totalBytesRead += 4;

			// method to put together the 40 bit block index if we were interested...
			// int64_t totalBlockIndex = (int64_t) lowerBlockIndexBits + ((int64_t) upperBlockIndexBits << 32);

			// put together the 40 bit total samples, code adapted from WavPack source include/wavpack.h macro
			// note that for a WavPack files samples actually means frames, a complete sample for all channels
			int64_t totalSamples;
			if (lowerTotalSamplesBits == (uint32_t) -1) {
				// all 1's in the lower 32 bits indicates "unknown" (regardless of upper 8 bits)
				totalSamples = -1;
			} else {
				totalSamples = (int64_t) lowerTotalSamplesBits + ((int64_t) upperTotalSamplesBits << 32) - upperTotalSamplesBits;
			}

			if (totalSamples >= 0 && blockNumber < 2)
				m_numberOfFrames = (unsigned) totalSamples;

			// we're not really interested in number of samples in this block
			wvFile.SeekI(4, wxFromCurrent);
			totalBytesRead += 4;

			unsigned variousFlags;
			wvFile.Read(&uBuffer, 4);
			if (wvFile.LastRead() != 4) {
				m_errorMessage += wxT("WavPack file various flags couldn't be read.\n");
				return false;
			} else {
				variousFlags = uBuffer;
			}
			totalBytesRead += 4;
			// from the flags available we could be interested in the following if it's the first block:
			// bit 2 (stereo/mono option)
			// bits 26-23 (samplerate)
			if (blockNumber < 2) {
				unsigned bitNbr2 = (variousFlags & ( 1 << 2 )) >> 2;
				if (bitNbr2 == 0)
					m_NumChannels = 2;
				else if (bitNbr2 == 1)
					m_NumChannels = 1;
				else
					m_NumChannels = 0; // not known

				unsigned mask = ((1 << 4) - 1) << 23;
				unsigned bit23to26 = (variousFlags & mask) >> 23;

				if (bit23to26 < 16)
					m_SampleRate = wavpackSamplerates[bit23to26];
				else
					m_SampleRate = 0; // not known
			}

			// we're not really interested in the crc
			wvFile.SeekI(4, wxFromCurrent);
			totalBytesRead += 4;

			// The whole 32 byte WavPack header is now read
			// After the WavPack header comes (possibly a number of) sub-blocks
			while (totalBytesRead < blockSize && !wvFile.Eof()) {
				// get next block id
				wvFile.Read(&uChBuffer, 1);
				if (wvFile.LastRead() != 1) {
					m_errorMessage += wxT("Block ID couldn't be read.\n");
					break;
				}
				unsigned char blockId = uChBuffer;
				totalBytesRead += 1;

				// get size of sub-block
				// WavPack word size is as unsigned short = 2 bytes
				unsigned subBlockSize;
				if (blockId & 0x80) {
					// this is a large block
					unsigned char byte1;
					wvFile.Read(&uChBuffer, 1);
					if (wvFile.LastRead() == 1) {
						byte1 = uChBuffer;
					} else {
						m_errorMessage += wxT("Large block byte 1 couldn't be read.\n");
						break;
					}
					unsigned char byte2;
					wvFile.Read(&uChBuffer, 1);
					if (wvFile.LastRead() == 1) {
						byte2 = uChBuffer;
					} else {
						m_errorMessage += wxT("Large block byte 2 couldn't be read.\n");
						break;
					}
					unsigned char byte3;
					wvFile.Read(&uChBuffer, 1);
					if (wvFile.LastRead() == 1) {
						byte3 = uChBuffer;
					} else {
						m_errorMessage += wxT("Large block byte 3 couldn't be read.\n");
						break;
					}
					subBlockSize = (byte1 | (byte2 << 8) | (byte3 << 16)) * 2;
					totalBytesRead += 3;
				} else {
					// this is a small block
					wvFile.Read(&uChBuffer, 1);
					if (wvFile.LastRead() == 1) {
						subBlockSize = (unsigned) uChBuffer * 2;
					} else {
						m_errorMessage += wxT("Small block size couldn't be read.\n");
						break;
					}
					totalBytesRead += 1;
				}
				unsigned bytesRead = 0;

				if (((blockId & 0x3f) == 0x21) || ((blockId & 0x3f) == 0x22)) {
					if ((blockId & 0x3f) == 0x21) {
						// a RIFF header is present in this block, trust but verify and then parse it
						wvFile.Read(fourCBuffer, 4);
						if (wvFile.LastRead() != 4 || !WAVE_RIFF.IsSameAs(fourCBuffer)) {
							m_errorMessage += wxT("Not a RIFF file in the WavPack file.\n");
							return false;
						}
						wvFile.Read(&uBuffer, 4); // filesize - 8 bytes
						if (wvFile.LastRead() != 4) {
							m_errorMessage += wxT("Couldn't read filesize.\n");
							return false;
						}
						wvFile.Read(fourCBuffer, 4);
						if (wvFile.LastRead() != 4 || !WAVE_ID.IsSameAs(fourCBuffer)) {
							m_errorMessage += wxT("Not a WAVE file.\n");
							return false;
						}
						bytesRead += 12;
						totalBytesRead += 12;
					}

					while (bytesRead < subBlockSize) {
						// get next fourcc chunk id
						wvFile.Read(fourCBuffer, 4);
						if (wvFile.LastRead() == 4) {
							bytesRead += 4;
							totalBytesRead += 4;
							if (WAVE_FMT.IsSameAs(fourCBuffer) && !fmtFound) {
								if (parseFmtChunk(wvFile)) {
									fmtFound = true;
									// the dword for chunk size itself won't be included in m_lastChunkSizeParsed but needs to be added
									unsigned actualBytesRead = m_lastChunkSizeParsed + 4;
									bytesRead += actualBytesRead;
									totalBytesRead += actualBytesRead;
									continue;
								} else {
									// if fmt chunk couldn't be parsed error message should already have been set
									return false;
								}
							} else if (WAVE_SMPL.IsSameAs(fourCBuffer) && !smplFound) {
								if (parseSmplChunk(wvFile)) {
									smplFound = true;
									unsigned actualBytesRead = m_lastChunkSizeParsed + 4;
									bytesRead += actualBytesRead;
									totalBytesRead += actualBytesRead;
									continue;
								} else {
									// if smpl chunk couldn't be parsed error message should already have been set
									return false;
								}
							} else if (WAVE_CUE.IsSameAs(fourCBuffer) && !cueFound) {
								if (parseCueChunk(wvFile)) {
									cueFound = true;
									unsigned actualBytesRead = m_lastChunkSizeParsed + 4;
									bytesRead += actualBytesRead;
									totalBytesRead += actualBytesRead;
									continue;
								} else {
									// if cue chunk couldn't be parsed error message should already have been set
									return false;
								}
							} else if (WAVE_LIST.IsSameAs(fourCBuffer)) {
								if (parseInfoListChunk(wvFile)) {
									unsigned actualBytesRead = m_lastChunkSizeParsed + 4;
									bytesRead += actualBytesRead;
									totalBytesRead += actualBytesRead;
									continue;
								} else {
									// if info list chunk couldn't be parsed error message should already have been set
									return false;
								}
							} else {
								// if the RIFF header or trailer contains any other chunks we just ignore them
								// first get the size that the chunk reports it has
								wvFile.Read(&uBuffer, 4);
								if (wvFile.LastRead() != 4) {
									m_errorMessage += wxT("Couldn't read chunk size in sub-block.\n");
									return false;
								}
								unsigned chunkSize = uBuffer;
								bytesRead += 4;
								totalBytesRead += 4;
								// but as we're about to skip this chunk we just need to verify that the amount of bytes would make sense to jump
								// as in some cases, like with a data chunk, the reported chunk size might be bigger than the size of this sub-block
								unsigned bytesToSkip = chunkSize + (chunkSize & 1);
								// after making sure there's padding to an even number for the chunk size we compare the bytes to skip and set it to the lower value
								if ((bytesToSkip + bytesRead) > subBlockSize)
									bytesToSkip = subBlockSize - bytesRead;
								wvFile.SeekI(bytesToSkip, wxFromCurrent);
								bytesRead += bytesToSkip;
								totalBytesRead += bytesToSkip;
								continue;
							}
						} else {
							m_errorMessage += wxT("Couldn't read fourcc id of sub-block.\n");
							break;
						}
					} // end of while (bytesRead < subBlockSize)
				} // end of if RIFF header or trailer

				// check if enough bytes was read
				if (subBlockSize > bytesRead) {
					unsigned bytesToSkip = subBlockSize - bytesRead;
					wvFile.SeekI(bytesToSkip + (bytesToSkip & 1), wxFromCurrent);
					totalBytesRead += (bytesToSkip + (bytesToSkip & 1));
				}
			} // end of while (totalBytesRead < blockSize && !wvFile.Eof()) sub-block parsing
		} // end of while !wvFile.Eof()

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
		m_errorMessage += wxT("Couldn't read fmt chunk size.\n");
		return false;
	}
	unsigned fmtChunkSize = uBuffer;

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		if(uShBuffer == 1 || uShBuffer == 3 || uShBuffer == 65534)
			m_AudioFormat = uShBuffer; // we only support PCM, IEEE_FLOAT and EXTENSIBLE
		else {
			m_errorMessage += wxT("Unsupported wave format detected.\n");
			return false;
		}
	} else {
		m_errorMessage += wxT("Couldn't read audio format.\n");
		return false;
	}

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		m_NumChannels = uShBuffer;
	} else {
		m_errorMessage += wxT("Couldn't read number of channels.\n");
		return false;
	}

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() == 4) {
		m_SampleRate = uBuffer;
	} else {
		m_errorMessage += wxT("Couldn't read samplerate.\n");
		return false;
	}

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() == 4) {
		m_ByteRate = uBuffer;
	} else {
		m_errorMessage += wxT("Couldn't read byte rate.\n");
		return false;
	}

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		m_BlockAlign = uShBuffer;
	} else {
		m_errorMessage += wxT("Couldn't read block align.\n");
		return false;
	}

	wavFile.Read(&uShBuffer, 2);
	if (wavFile.LastRead() == 2) {
		m_BitsPerSample = uShBuffer;
	} else {
		m_errorMessage += wxT("Couldn't read bits per sample.\n");
		return false;
	}

	if (m_BlockAlign != (m_NumChannels * m_BitsPerSample / 8)) {
		m_errorMessage += wxT("Block align doesn't match (nChannels*bitsPerSample/8).\n");
		return false;
	}

	if (fmtChunkSize > 16) {
		unsigned bytesToSkip = fmtChunkSize - 16;
		wavFile.SeekI(bytesToSkip + (bytesToSkip & 1), wxFromCurrent);
		m_lastChunkSizeParsed = 16 + (bytesToSkip + (bytesToSkip & 1));
		return true;
	} else {
		m_lastChunkSizeParsed = fmtChunkSize;
		return true;
	}

	return true;
}

bool WAVfileParser::parseSmplChunk(wxFFileInputStream &wavFile) {
	unsigned uBuffer;

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() != 4) {
		m_errorMessage += wxT("Couldn't read smpl chunk size.\n");
		return false;
	}
	unsigned smplChunkSize = uBuffer;
	unsigned bytesRead = 0;

	// we're not interested in:
	// dwManufacturer 4 bytes
	// dwProduct 4 bytes
	// dwSamplePeriod 4 bytes
	wavFile.SeekI(12, wxFromCurrent);
	bytesRead += 12;

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() == 4) {
		m_dwMIDIUnityNote = uBuffer;
		bytesRead += 4;
	} else {
		m_errorMessage += wxT("Couldn't read dwMIDIUnityNote.\n");
		return false;
	}

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() == 4) {
		m_dwMIDIPitchFraction = uBuffer;
		bytesRead += 4;
	} else {
		m_errorMessage += wxT("Couldn't read dwMIDIPitchFraction.\n");
		return false;
	}

	// we're not interested in:
	// dwSMPTEFormat 4 bytes
	// dwSMPTEOffset 4 bytes
	wavFile.SeekI(8, wxFromCurrent);
	bytesRead += 8;

	unsigned numberOfLoops = 0;
	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() == 4) {
		numberOfLoops = uBuffer;
		bytesRead += 4;
	} else {
		m_errorMessage += wxT("Couldn't read cSampleLoops.\n");
		return false;
	}

	// we're not interested in:
	// cbSamplerData 4 bytes
	wavFile.SeekI(4, wxFromCurrent);
	bytesRead += 4;

	for (unsigned i = 0; i < numberOfLoops; i++) {
		LOOP l;

		wavFile.Read(&uBuffer, 4);
		if (wavFile.LastRead() == 4) {
			l.dwIdentifier = uBuffer;
			bytesRead += 4;
		} else {
			m_errorMessage += wxT("Couldn't read dwIdentifier.\n");
			return false;
		}

		// not interested in dwType 4 bytes
		wavFile.SeekI(4, wxFromCurrent);
		bytesRead += 4;

		wavFile.Read(&uBuffer, 4);
		if (wavFile.LastRead() == 4) {
			l.dwStart = uBuffer;
			bytesRead += 4;
		} else {
			m_errorMessage += wxT("Couldn't read dwStart.\n");
			return false;
		}

		wavFile.Read(&uBuffer, 4);
		if (wavFile.LastRead() == 4) {
			l.dwEnd = uBuffer;
			bytesRead += 4;
		} else {
			m_errorMessage += wxT("Couldn't read dwEnd.\n");
			return false;
		}

		// not interested in:
		// dwFraction 4 bytes
		// dwPlayCount 4 bytes
		wavFile.SeekI(8, wxFromCurrent);
		bytesRead += 8;

		m_loops.push_back(l);
	}

	if (smplChunkSize > bytesRead) {
		unsigned bytesToSkip = smplChunkSize - bytesRead;
		wavFile.SeekI(bytesToSkip + (bytesToSkip & 1), wxFromCurrent);
		m_lastChunkSizeParsed = bytesRead + (bytesToSkip + (bytesToSkip & 1));
	} else {
		m_lastChunkSizeParsed = bytesRead;
	}

	return true;
}

bool WAVfileParser::parseCueChunk(wxFFileInputStream &wavFile) {
	unsigned uBuffer;

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() != 4) {
		m_errorMessage += wxT("Couldn't read cue chunk size.\n");
		return false;
	}
	unsigned cueChunkSize = uBuffer;
	unsigned bytesRead = 0;

	unsigned cueCount = 0;
	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() != 4) {
		m_errorMessage += wxT("Couldn't read cue count.\n");
		return false;
	}
	cueCount = uBuffer;
	bytesRead += 4;

	for (unsigned i = 0; i < cueCount; i++) {
		CUEPOINT c;

		wavFile.Read(&uBuffer, 4);
		if (wavFile.LastRead() == 4) {
			c.dwName = uBuffer;
			bytesRead += 4;
		} else {
			m_errorMessage += wxT("Couldn't read dwName.\n");
			return false;
		}

		// not interested in:
		// dwPosition 4 bytes
		// fccChunk 4 bytes
		// dwChunkStart 4 bytes
		// dwBlockStart 4 bytes
		wavFile.SeekI(16, wxFromCurrent);
		bytesRead += 16;

		wavFile.Read(&uBuffer, 4);
		if (wavFile.LastRead() == 4) {
			c.dwSampleOffset = uBuffer;
			bytesRead += 4;
		} else {
			m_errorMessage += wxT("Couldn't read dwSampleOffset.\n");
			return false;
		}

		m_cues.push_back(c);
	}

	if (cueChunkSize > bytesRead) {
		unsigned bytesToSkip = cueChunkSize - bytesRead;
		wavFile.SeekI(bytesToSkip + (bytesToSkip & 1), wxFromCurrent);
		m_lastChunkSizeParsed = bytesRead + (bytesToSkip + (bytesToSkip & 1));
	} else {
		m_lastChunkSizeParsed = bytesRead;
	}

	return true;
}

bool WAVfileParser::parseInfoListChunk(wxFFileInputStream &wavFile) {
	unsigned uBuffer;
	char fourCBuffer[5] = {};
	unsigned char uChBuffer;

	wavFile.Read(&uBuffer, 4);
	if (wavFile.LastRead() != 4) {
		m_errorMessage += wxT("Couldn't read LIST chunk size.\n");
		return false;
	}
	unsigned listChunkSize = uBuffer;
	unsigned bytesRead = 0;

	wavFile.Read(fourCBuffer, 4);
	if (wavFile.LastRead() != 4) {
		m_errorMessage += wxT("Couldn't read next four characters.\n");
		return false;
	} else {
		bytesRead += 4;
		if (WAVE_INFO.IsSameAs(fourCBuffer)) {
			// now start reading the four charater keys and the info string values

			while (bytesRead < listChunkSize) {
				wxString theKey = wxEmptyString;
				wavFile.Read(fourCBuffer, 4);
				if (wavFile.LastRead() != 4) {
					m_errorMessage += wxT("Couldn't read next four characters for this key.\n");
					break;
				} else {
					theKey = fourCBuffer;
					bytesRead += 4;
				}

				// after each key is the length of the string including zero terminating character and possibly padding to even length
				unsigned totalStringLength;
				wxString theValue = wxEmptyString;
				wavFile.Read(&uBuffer, 4);
				if (wavFile.LastRead() != 4) {
					m_errorMessage += wxT("Couldn't read string length of a LIST INFO chunk.\n");
					break;
				}
				bytesRead += 4;
				if (uBuffer % 2 == 0)
					totalStringLength = uBuffer;
				else
					totalStringLength = uBuffer + 1;
				for (unsigned i = 0; i < totalStringLength; i++) {
					unsigned char oneByte;
					wavFile.Read(&uChBuffer, 1);
					if (wavFile.LastRead() == 1) {
						oneByte = uChBuffer;
						bytesRead += 1;
					} else {
						break;
					}
					if (oneByte != 0x00)
						theValue += oneByte;
				}

				m_infoList.push_back(std::make_pair(theKey, theValue));
			}
		}
	}

	if (listChunkSize > bytesRead) {
		unsigned bytesToSkip = listChunkSize - bytesRead;
		wavFile.SeekI(bytesToSkip + (bytesToSkip & 1), wxFromCurrent);
		m_lastChunkSizeParsed = bytesRead + (bytesToSkip + (bytesToSkip & 1));
	} else {
		m_lastChunkSizeParsed = bytesRead;
	}

	return true;
}
