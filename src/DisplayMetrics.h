/*
 * DisplayMetrics.h is part of GoOdf.
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

#ifndef DISPLAYMETRICS_H
#define DISPLAYMETRICS_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include <wx/font.h>
#include "GoPanelSize.h"
#include "GoColor.h"

class DisplayMetrics {
public:
	DisplayMetrics();
	~DisplayMetrics();

	void write(wxTextFile *outFile);
	void read(wxFileConfig *cfg);

	wxBitmap getDrawstopBg();
	wxBitmap getConsoleBg();
	wxBitmap getKeyHorizBg();
	wxBitmap getKeyVertBg();
	wxBitmap getInsetBg();

	GoPanelSize m_dispScreenSizeHoriz; // 100 - 32000, SMALL = 800, MEDIUM = 1007, MEDIUM LARGE = 1263, LARGE = 1583
	GoPanelSize m_dispScreenSizeVert; // 100 - 32000, SMALL = 500, MEDIUM = 663, MEDIUM LARGE = 855, LARGE = 1095
	int m_dispDrawstopBackgroundImageNum;
	int m_dispConsoleBackgroundImageNum;
	int m_dispKeyHorizBackgroundImageNum;
	int m_dispKeyVertBackgroundImageNum;
	int m_dispDrawstopInsetBackgroundImageNum;
	wxFont m_dispControlLabelFont;
	wxFont m_dispShortcutKeyLabelFont;
	GoColor m_dispShortcutKeyLabelColour;
	wxFont m_dispGroupLabelFont;
	int m_dispDrawstopCols; // 2-12 must be even
	int m_dispDrawstopRows; // 1-20
	bool m_dispDrawstopColsOffset;
	bool m_dispDrawstopOuterColOffsetUp; // (required if m_dispDrawstopColsOffset is true)
	bool m_dispPairDrawstopCols; // m_dispDrawstopCols must be multiple of 4
	int m_dispExtraDrawstopRows; // 0-99, numbering starts at 100
	int m_dispExtraDrawstopCols; // 0-40
	int m_dispButtonCols; // 1-32
	int m_dispExtraButtonRows; // 0-99
	bool m_dispExtraPedalButtonRow; // extra piston row (number 9)
	bool m_dispExtraPedalButtonRowOffset; // (required if m_dispExtraPedalButtonRow is true)
	bool m_dispExtraPedalButtonRowOffsetRight; // (required if m_dispExtraPedalButtonRow is true)
	bool m_dispButtonsAboveManuals;
	bool m_dispTrimAboveManuals;
	bool m_dispTrimBelowManuals;
	bool m_dispTrimAboveExtraRows;
	bool m_dispExtraDrawstopRowsAboveExtraButtonRows;
	int m_dispDrawstopWidth; // (integer 1-150, default: 78)
	int m_dispDrawstopHeight; // (integer 1-150, default: 69)
	int m_dispPistonWidth; // (integer 1-150, default: 44)
	int m_dispPistonHeight; // (integer 1-150, default: 40)
	int m_dispEnclosureWidth; // (integer 1-150, default: 52)
	int m_dispEnclosureHeight; // (integer 1-150, default: 63)
	int m_dispPedalHeight; // (integer 1-500, default: 40)
	int m_dispPedalKeyWidth; // (integer 1-500, default: 7)
	int m_dispManualHeight; // (integer 1-500, default: 32)
	int m_dispManualKeyWidth; // (integer 1-500, default: 12)
};

#endif
