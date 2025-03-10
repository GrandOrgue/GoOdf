/*
 * GUIItemBtn.h is part of GoOdf.
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

#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include "GUIButton.h"
#include <wx/wx.h>
#include <wx/textfile.h>

template<class T> class GUIItemBtn : public GUIButton {
public:
	GUIItemBtn(T *t) : m_ptr(t) {
		if (typeid(*m_ptr) == typeid(Divisional))
			m_type = wxT("Divisional");
		else if (typeid(*m_ptr) == typeid(Stop))
			m_type = wxT("Stop");
		else if (typeid(*m_ptr) == typeid(Coupler))
			m_type = wxT("Coupler");
	}

	void write(wxTextFile *outFile) {
		GUIButton::write(outFile);
	}
	bool isReferencing(T *p) {
		if (m_ptr == p)
			return true;
		else
			return false;
	}
private:
	const T *m_ptr;
};

#endif
