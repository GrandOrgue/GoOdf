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

#ifndef CMBPARSER_H
#define CMBPARSER_H

#include <wx/wx.h>
#include <wx/fileconf.h>
#include "CmbOrgan.h"

class CmbParser {
public:
	CmbParser(wxString filePath, CMB_ORGAN *cmbOrgan);
	~CmbParser();

	bool IsParsedOk();
	wxString GetErrorText();

private:
	bool m_isOk;
	wxString m_errorText;

	bool readCmbFile(wxString fileName, CMB_ORGAN *cmbOrgan);
	bool parseString(wxFileConfig *ini, wxString key, wxString *value, wxString defaultValue = wxEmptyString);
	bool parseInt(wxFileConfig *ini, wxString key, int *value, int defaultValue = -1);
	bool parseFloat(wxFileConfig *ini, wxString key, float *value, float defaultValue = 0.0f);
	bool parseSection(wxFileConfig *ini, wxString section, CMB_ORGAN *cmbOrgan);
	bool hasPipeKey(wxFileConfig *ini, int pipeIndex);
	CMB_PIPE parsePipe(wxFileConfig *ini, int pipeIndex);
};

#endif
