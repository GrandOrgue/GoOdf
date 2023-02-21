/*
 * OrganFileParser.h is part of GOODF.
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

#ifndef ORGANFILEPARSER_H
#define ORGANFILEPARSER_H

#include <wx/wx.h>
#include <wx/fileconf.h>
#include "Organ.h"

class OrganFileParser {
public:
	OrganFileParser(wxString filePath, Organ *organ);
	~OrganFileParser();

	bool isOrganReady();

private:

	Organ *m_organ;
	wxString m_filePath;
	wxFileConfig m_organFile;
	bool m_fileIsOk;
	bool m_organIsReady;
	bool m_isUsingOldPanelFormat;
	wxString m_errorMessage;

	void readIniFile();
	void parseOrgan();

	bool parseBoolean(wxString key, bool defaultValue = true);

	void parseOrganSection();
	void parseDisplayMetrics(wxString sourcePanel, GoPanel *targetPanel);

};

#endif
