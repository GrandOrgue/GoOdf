/*
 * GOODFFunctions.h is part of GoOdf.
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

#ifndef GOODF_FUNCTIONS_H
#define GOODF_FUNCTIONS_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/filename.h>
#include <vector>
#include "GOODF.h"

class Organ;

namespace GOODF_functions {

	inline wxString number_format(int number) {
		 wxString str = wxString::Format(wxT("%0.3d"), number);
		 return str;
	}

	inline void writeReferences(wxTextFile *outFile, wxString elementName, std::vector<int> list) {
		for (unsigned k = 0; k < list.size(); k++) {
			wxString refNumber = wxString::Format(wxT("%i"), list[k]);
			wxString outStr = elementName + number_format(k + 1) + wxT("=") + refNumber;
			outFile->AddLine(outStr);
		}
	}

	inline void CheckForStartingWhitespace(wxString *str, wxTextCtrl *ctrl) {
		if (str->StartsWith(wxT(" "))) {
			str->Replace(wxT(" "), wxT(""), false);
			ctrl->SetValue(*str);
		} else if (str->Find(wxT(";")) != wxNOT_FOUND) {
			str->Replace(wxT(";"), wxT(""), false);
			ctrl->SetValue(*str);
		}
	}

	inline void RemoveTrailingWhitespace(wxString *str) {
		str->Trim();
	}

	inline wxString fixSeparator(wxString pathToCheck) {
		if (pathToCheck.Contains("/")) {
			pathToCheck.Replace("/", "\\");
		}

		return pathToCheck;
	}

	inline wxString removeBaseOdfPath(wxString path) {
		wxString stringToReturn = path;
		wxFileName fName = wxFileName(path);
		if (fName.FileExists()) {
			fName.MakeRelativeTo(::wxGetApp().m_frame->m_organ->getOdfRoot());
			stringToReturn = fName.GetFullPath();
			if (stringToReturn.StartsWith(wxFILE_SEP_PATH))
				stringToReturn.erase(0, 1);
			else if (stringToReturn.StartsWith(wxT("./")) || stringToReturn.StartsWith(wxT(".\\")))
				stringToReturn.erase(0, 2);
		}
		return stringToReturn;
	}

	inline wxString checkIfFileExist(wxString relativePath, Organ *currentOrgan) {
		if (relativePath != wxEmptyString) {
			if (relativePath.StartsWith(wxT("./")) || relativePath.StartsWith(wxT(".\\")))
				relativePath.erase(0, 2);
			wxString fullFilePath = currentOrgan->getOdfRoot() + wxFILE_SEP_PATH + relativePath;
			wxFileName theFile = wxFileName(fullFilePath, wxPATH_DOS);
			if (theFile.FileExists()) {
				return theFile.GetFullPath();
			}
		}
		return wxEmptyString;
	}

	inline bool parseBoolean(wxString value, bool defaultValue = true) {
		if (value.IsSameAs(wxT("Y"), false))
			return true;
		if (value.IsSameAs(wxT("N"), false))
			return false;
		return defaultValue;
	}

	inline int getGreatestCommonDivisor(int a, int b) {
		if (a == 0)
			return b;
		return getGreatestCommonDivisor(b % a, a);
	}

	inline wxString getFootLengthSize(int nbr) {
		int whole = 64 / nbr;
		int rest = 64 % nbr;

		if (rest == 0) {
			return wxT("equals to ") + wxString::Format(wxT("%i"), whole) + wxT("'");
		} else {
			int gcd = getGreatestCommonDivisor(rest, nbr);
			if (gcd != 0) {
				int numerator = rest / gcd;
				int denominator = nbr / gcd;
				if (whole > 0)
					return wxT("equals to ") + wxString::Format(wxT("%i %i/%i'"), whole, numerator, denominator);
				else
					return wxT("equals to ") + wxString::Format(wxT("%i/%i'"), numerator, denominator);
			} else {
				if (whole > 0)
					return wxT("equals to ") + wxString::Format(wxT("%i %i/%i'"), whole, rest, nbr);
				else
					return wxT("equals to ") + wxString::Format(wxT("%i/%i'"), rest, nbr);
			}
		}
	}
}

#endif

