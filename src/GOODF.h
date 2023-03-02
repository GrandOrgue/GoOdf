/* 
 * GOODF.h is a part of GOODF software
 * Copyright (C) 2023 Lars Palo
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
	GOODFFrame *m_frame;
	wxIconBundle m_icons;
	std::vector<wxBitmap> m_buttonBitmaps;
	std::vector<wxBitmap> m_drawstopBitmaps;
	std::vector<wxBitmap> m_enclosureStyleBitmaps;
	std::vector<wxBitmap> m_labelBitmaps;
	wxHtmlHelpController *m_helpController;
};

DECLARE_APP(GOODF)

#endif
