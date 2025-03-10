/* 
 * GOODF.h is a part of GOODF software
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo DOT se
 */

#ifndef GOODF_H
#define GOODF_H

#include <wx/wx.h>
#include "GOODFFrame.h"
#include <vector>
#include <wx/html/helpctrl.h>

class GOODF : public wxApp {
public:
	virtual bool OnInit();
	int OnExit();
	wxBitmap GetJpegBitmap(const unsigned char *data, int length, bool doScale = false);
	wxBitmap GetJpeg90Bitmap(const unsigned char *data, int length, bool doScale = false);
	GOODFFrame *m_frame;
	wxIconBundle m_icons;
	std::vector<wxBitmap> m_buttonBitmaps;
	std::vector<wxBitmap> m_drawstopBitmaps;
	std::vector<wxBitmap> m_scaledDrawstopBitmaps;
	std::vector<wxBitmap> m_enclosureStyleBitmaps;
	std::vector<wxBitmap> m_labelBitmaps;
	std::vector<wxBitmap> m_fullSizeLabelBmps;
	std::vector<wxBitmap> m_defaultManualBitmaps;
	std::vector<wxBitmap> m_invertedManualBitmaps;
	std::vector<wxBitmap> m_woodenManualBitmaps;
	std::vector<wxBitmap> m_invertedWoodenManualBitmaps;
	std::vector<wxBitmap> m_defaultPedalBitmaps;
	std::vector<wxBitmap> m_invertedPedalBitmaps;

	std::vector<wxBitmap> m_manualKeyBmps01;
	std::vector<wxBitmap> m_invertedManualKeysBmps01;
	std::vector<wxBitmap> m_woodenManualKeysBmps01;
	std::vector<wxBitmap> m_invertedManualWoodenKeysBmps01;
	std::vector<wxBitmap> m_pedalKeysBmps01;
	std::vector<wxBitmap> m_invertedPedalKeysBmps01;
	std::vector<wxBitmap> m_manualKeyBmps02;
	std::vector<wxBitmap> m_invertedManualKeysBmps02;
	std::vector<wxBitmap> m_woodenManualKeysBmps02;
	std::vector<wxBitmap> m_invertedManualWoodenKeysBmps02;
	std::vector<wxBitmap> m_pedalKeysBmps02;
	std::vector<wxBitmap> m_invertedPedalKeysBmps02;

	std::vector<wxBitmap> m_woodBitmaps;
	std::vector<wxBitmap> m_scaledWoodBitmaps;
	wxHtmlHelpController *m_helpController;
	wxString m_fullAppName;
};

DECLARE_APP(GOODF)

#endif
