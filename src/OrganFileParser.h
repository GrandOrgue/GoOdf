/*
 * OrganFileParser.h is part of GoOdf.
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * GoOdf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GoOdf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GoOdf.  If not, see <https://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo.se
 */

#ifndef ORGANFILEPARSER_H
#define ORGANFILEPARSER_H

#include <wx/wx.h>
#include <wx/fileconf.h>
#include <wx/progdlg.h>
#include "Organ.h"

class OrganFileParser {
public:
	OrganFileParser(wxString filePath, Organ *organ);
	~OrganFileParser();

	bool isOrganReady();

private:

	Organ *m_organ;
	wxString m_filePath;
	wxFileConfig *m_organFile;
	bool m_fileIsOk;
	bool m_organIsReady;
	bool m_isUsingOldPanelFormat;
	wxString m_errorMessage;
	wxProgressDialog *m_progressDlg;

	int m_enclosuresToParse;
	int m_tremulantsToParse;
	int m_windchestgroupsToParse;
	int m_switchesToParse;
	int m_ranksToParse;
	int m_stopsToParse;
	int m_manualsToParse;
	int m_couplersToParse;
	int m_divisionalsToParse;
	int m_divisionalCouplersToParse;
	int m_generalsToParse;
	int m_reversiblePistonsToParse;
	int m_panelsToParse;
	int m_imagesToParse;
	int m_setterElementsToParse;

	void readIniFile();
	void trimKeyValues();
	void parseOrgan();

	void parseOrganSection();

	void createGUIEnclosure(GoPanel *targetPanel, Enclosure *enclosure);
	void createGUITremulant(GoPanel *targetPanel, Tremulant *tremulant);
	void createGUISwitch(GoPanel *targetPanel, GoSwitch *theSwitch);
	void createGUILabel(GoPanel *targetPanel);
	void createGUIManual(GoPanel *targetPanel, Manual *manual);
	void createGUIPiston(GoPanel *targetPanel, ReversiblePiston *piston);
	void createGUIDivCplr(GoPanel *targetPanel, DivisionalCoupler *div_cplr);
	void createGUIGeneral(GoPanel *targetPanel, General *general);
	void createGUIDivisional(GoPanel *targetPanel, Divisional *divisional);
	void createFromSetterElement(GoPanel *targetPanel, wxString elementType);
	void createGUICoupler(GoPanel *targetPanel, Coupler *coupler);
	void createGUIStop(GoPanel *targetPanel, Stop *stop);

	void parsePanelElements(GoPanel *targetPanel, wxString panelId);

};

#endif
