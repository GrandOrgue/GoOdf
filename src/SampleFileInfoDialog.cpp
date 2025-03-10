/*
 * SampleFileInfoDialog.h is part of GoOdf.
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

#include "SampleFileInfoDialog.h"
#include <wx/statline.h>
#include "GOODFFunctions.h"

IMPLEMENT_CLASS(SampleFileInfoDialog, wxDialog)

SampleFileInfoDialog::SampleFileInfoDialog(wxString sampleFile) {
	Init(sampleFile);
}

SampleFileInfoDialog::SampleFileInfoDialog(
	wxString sampleFile,
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	Init(sampleFile);
	Create(parent, id, caption, pos, size, style);
}

SampleFileInfoDialog::~SampleFileInfoDialog() {
	if (m_sampleFile) {
		delete m_sampleFile;
		m_sampleFile = NULL;
	}
}

void SampleFileInfoDialog::Init(wxString sampleFile) {
	m_relativeFilePath = GOODF_functions::removeBaseOdfPath(sampleFile);
	m_sampleFile = new WAVfileParser(sampleFile);
}

bool SampleFileInfoDialog::Create(
	wxWindow* parent,
	wxWindowID id,
	const wxString& caption,
	const wxPoint& pos,
	const wxSize& size,
	long style
) {
	if (!wxDialog::Create(parent, id, caption, pos, size, style))
		return false;

	CreateControls();

	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Centre();

	return true;
}

void SampleFileInfoDialog::CreateControls() {
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *firstRow = new wxBoxSizer(wxHORIZONTAL);
	firstRow->AddStretchSpacer();
	wxStaticText *sampleFilePath = new wxStaticText (
		this,
		wxID_STATIC,
		m_relativeFilePath
	);
	firstRow->Add(sampleFilePath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	firstRow->AddStretchSpacer();
	mainSizer->Add(firstRow, 0, wxGROW);

	wxStaticLine *topDivider = new wxStaticLine(this);
	mainSizer->Add(topDivider, 0, wxEXPAND);

	if (m_sampleFile->isWavOk()) {
		wxString fileType = wxEmptyString;
		if (m_sampleFile->isWavPacked()) {
			fileType = wxT("The file is wavpacked");
		} else {
			fileType = wxT("The file is a normal WAV file");
		}

		wxBoxSizer *secondRow = new wxBoxSizer(wxHORIZONTAL);
		secondRow->AddStretchSpacer();
		wxStaticText *fileTypeText = new wxStaticText (
			this,
			wxID_STATIC,
			fileType
		);
		secondRow->Add(fileTypeText, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
		secondRow->AddStretchSpacer();
		mainSizer->Add(secondRow, 0, wxGROW|wxTOP|wxBOTTOM, 10);

		wxString audioFormat = wxEmptyString;
		if (m_sampleFile->getAudioFormat() == 1) {
			audioFormat = wxT("WAVE_FORMAT_PCM");
		} else if (m_sampleFile->getAudioFormat() == 3) {
			audioFormat = wxT("WAVE_FORMAT_IEE_FLOAT");
		} else if (m_sampleFile->getAudioFormat() == 65534) {
			audioFormat = wxT("WAVE_FORMAT_EXTENSIBLE");
		}

		double sampleRateInkHz = m_sampleFile->getSampleRate() / 1000.0f;

		wxString channelsName = wxEmptyString;
		if (m_sampleFile->getNumberOfChannels() == 2)
			channelsName = wxT("2 channels (Stereo)");
		else if (m_sampleFile->getNumberOfChannels() == 1)
			channelsName = wxT("1 channel (Mono)");
		else
			channelsName = wxString::Format(wxT("%u channels"), m_sampleFile->getNumberOfChannels());

		wxBoxSizer *thirdRow = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *audioFormatText = new wxStaticText (
			this,
			wxID_STATIC,
			wxString::Format(wxT("Audio format is %u Bit %s at %g kHz with %s"), m_sampleFile->getBitsPerSample(), audioFormat, sampleRateInkHz, channelsName)
		);
		thirdRow->Add(audioFormatText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
		mainSizer->Add(thirdRow, 0, wxGROW|wxTOP|wxBOTTOM, 2);

		double fileDuration = (double) m_sampleFile->getNumberOfFrames() / (double) m_sampleFile->getSampleRate();
		wxBoxSizer *sixthRow = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *framesText = new wxStaticText (
			this,
			wxID_STATIC,
			wxString::Format(wxT("File contains %u complete samples (frames) which equals to %.2f seconds duration"), m_sampleFile->getNumberOfFrames(), fileDuration)
		);
		sixthRow->Add(framesText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
		mainSizer->Add(sixthRow, 0, wxGROW|wxTOP|wxBOTTOM, 2);

		wxBoxSizer *seventhRow = new wxBoxSizer(wxHORIZONTAL);
		wxStaticText *pitchText = new wxStaticText (
			this,
			wxID_STATIC,
			wxString::Format(wxT("Embedded pitch information with MIDI Note: %u and Pitch Fraction: %.2f cents results in %.2f Hz\n"), m_sampleFile->getMidiNote(), m_sampleFile->getPitchFractionCents(), m_sampleFile->getPitchInHz())
		);
		seventhRow->Add(pitchText, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT, 5);
		mainSizer->Add(seventhRow, 0, wxGROW|wxTOP|wxBOTTOM, 2);

		unsigned nbrLoops = m_sampleFile->getNumberOfLoops();
		if (nbrLoops > 0) {
			wxString loopHeader = wxEmptyString;
			if (nbrLoops == 1)
				loopHeader = wxT("Embedded loop:");
			else
				loopHeader = wxT("Embedded loops:");
			wxBoxSizer *loopsRow = new wxBoxSizer(wxVERTICAL);
			wxStaticText *loopLabel = new wxStaticText (
				this,
				wxID_STATIC,
				loopHeader
			);
			loopsRow->Add(loopLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxString loopStrings = wxEmptyString;
			for (unsigned i = 0; i < nbrLoops; i++) {
				LOOP l = m_sampleFile->getLoopAtIndex(i);
				double loopStart = (double) l.dwStart / (double) m_sampleFile->getSampleRate();
				double loopEnd = (double) l.dwEnd / (double) m_sampleFile->getSampleRate();
				loopStrings += wxString::Format(wxT("Loop %u with ID: %u starts at %u samples (%.3f s) and ends at %u samples (%.3f s)\n"), i + 1, l.dwIdentifier, l.dwStart, loopStart, l.dwEnd, loopEnd);
			}
			wxStaticText *loopText = new wxStaticText (
				this,
				wxID_STATIC,
				loopStrings
			);
			loopsRow->Add(loopText, 0, wxGROW|wxALL, 5);
			mainSizer->Add(loopsRow, 0, wxGROW);
		}

		unsigned nbrCues = m_sampleFile->getNumberOfCues();
		if (nbrCues > 0) {
			wxString cueHeader = wxEmptyString;
			if (nbrCues == 1)
				cueHeader = wxT("Embedded cue:");
			else
				cueHeader = wxT("Embedded cues:");
			wxBoxSizer *cuesRow = new wxBoxSizer(wxVERTICAL);
			wxStaticText *cueLabel = new wxStaticText (
				this,
				wxID_STATIC,
				cueHeader
			);
			cuesRow->Add(cueLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
			wxString cueStrings = wxEmptyString;
			for (unsigned i = 0; i < nbrCues; i++) {
				CUEPOINT c = m_sampleFile->getCuepointAtIndex(i);
				double cuePosition = (double) c.dwSampleOffset / (double) m_sampleFile->getSampleRate();
				cueStrings += wxString::Format(wxT("Cue %u with dwName: %u has dwSampleOffset: %u samples (%.3f s)\n"), i + 1, c.dwName, c.dwSampleOffset, cuePosition);
			}
			wxStaticText *cueText = new wxStaticText (
				this,
				wxID_STATIC,
				cueStrings
			);
			cuesRow->Add(cueText, 0, wxGROW|wxALL, 5);
			mainSizer->Add(cuesRow, 0, wxGROW);

		}

		unsigned infoListEntries = m_sampleFile->getInfoListSize();
		if (infoListEntries > 0) {
			wxBoxSizer *listInfoRow = new wxBoxSizer(wxVERTICAL);
			wxStaticText *listInfoLabel = new wxStaticText (
				this,
				wxID_STATIC,
				wxT("LIST INFO:")
			);
			listInfoRow->Add(listInfoLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

			wxString listInfoString = wxEmptyString;
			for (unsigned i = 0; i < infoListEntries; i++) {
				std::pair<wxString, wxString> thePair = m_sampleFile->getInfoListContentAtIndex(i);

				if (thePair.first.IsSameAs(wxT("IART")))
					listInfoString += wxString::Format(wxT("Artist:\t\t\t%s\n"), thePair.second);
				else if (thePair.first.IsSameAs(wxT("ICMT")))
					listInfoString += wxString::Format(wxT("Comments:\t\t%s\n"), thePair.second);
				else if (thePair.first.IsSameAs(wxT("ICOP")))
					listInfoString += wxString::Format(wxT("Copyright:\t\t%s\n"), thePair.second);
				else if (thePair.first.IsSameAs(wxT("ICRD")))
					listInfoString += wxString::Format(wxT("Creation date:\t%s\n"), thePair.second);
				else if (thePair.first.IsSameAs(wxT("ISFT")))
					listInfoString += wxString::Format(wxT("Software:\t\t%s\n"), thePair.second);
				else
					listInfoString += wxString::Format(wxT("%s\t\t\t%s\n"), thePair.first, thePair.second);
			}
			wxStaticText *listInfoText = new wxStaticText (
				this,
				wxID_STATIC,
				listInfoString
			);
			listInfoRow->Add(listInfoText, 0, wxGROW|wxALL, 5);
			mainSizer->Add(listInfoRow, 0, wxGROW);
		}
	} else {
		wxBoxSizer *infoRow = new wxBoxSizer(wxHORIZONTAL);
		infoRow->AddStretchSpacer();
		wxStaticText *sampleFilePath = new wxStaticText (
			this,
			wxID_STATIC,
			wxT("The file couldn't be parsed!")
		);
		infoRow->Add(sampleFilePath, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
		infoRow->AddStretchSpacer();
		mainSizer->Add(infoRow, 0, wxGROW);

		wxBoxSizer *errorMsgRow = new wxBoxSizer(wxVERTICAL);
		wxStaticText *errorLabel = new wxStaticText (
			this,
			wxID_STATIC,
			wxT("Error message:")
		);
		errorMsgRow->Add(errorLabel, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
		wxTextCtrl *infoText = new wxTextCtrl(
			this,
			wxID_ANY,
			m_sampleFile->getErrorMessage(),
			wxDefaultPosition,
			wxDefaultSize,
			wxTE_READONLY|wxTE_MULTILINE
		);
		errorMsgRow->Add(infoText, 1, wxGROW|wxALL, 5);
		mainSizer->Add(errorMsgRow, 1, wxGROW);
	}

	wxStaticLine *bottomDivider = new wxStaticLine(this);
	mainSizer->Add(bottomDivider, 0, wxEXPAND);

	wxBoxSizer *lastRow = new wxBoxSizer(wxHORIZONTAL);
	lastRow->AddStretchSpacer();
	wxButton *okButton = new wxButton(
		this,
		wxID_OK,
		wxT("OK")
	);
	lastRow->Add(okButton, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
	lastRow->AddStretchSpacer();
	mainSizer->Add(lastRow, 0, wxGROW);

	SetSizer(mainSizer);
}
