/*
 * CmbOrgan.h is part of GoOdf.
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

#ifndef CMBORGAN_H
#define CMBORGAN_H

#include <vector>
#include <wx/wx.h>

struct CMB_ELEMENT {
	float amplitude;
	float gain;
	float pitchTuning;
	float pitchCorrection;
	int trackerDelay;
};

struct CMB_PIPE {
	int pipeNbr;
	CMB_ELEMENT attributes;
};

struct CMB_ELEMENT_WITH_PIPES {
	CMB_ELEMENT attributes;
	std::vector<CMB_PIPE> pipes;
};

struct CMB_ORGAN {
	wxString churchName;
	wxString odfPath;
	CMB_ELEMENT attributes;
	std::vector<CMB_ELEMENT_WITH_PIPES> cmbStops;
	std::vector<CMB_ELEMENT_WITH_PIPES> cmbRanks;
	std::vector<CMB_ELEMENT> cmbWindchests;
};

#endif
