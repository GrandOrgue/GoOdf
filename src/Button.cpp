/*
 * Button.cpp is part of GOODF.
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

#include "Button.h"
#include "GOODFFunctions.h"

Button::Button() {
	name = wxEmptyString;
	displayed = false;
	shortCutKey = 0;
}

Button::~Button() {

}

void Button::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + name);
	if (shortCutKey > 0)
		outFile->AddLine(wxT("ShortcutKey=") + wxString::Format(wxT("%i"), shortCutKey));
}

void Button::read(wxFileConfig *cfg, bool usingOldPanelFormat) {
	name = cfg->Read("Name", wxEmptyString);
	wxString cfgBoolValue = cfg->Read("Displayed", wxEmptyString);
	displayed = GOODF_functions::parseBoolean(cfgBoolValue, usingOldPanelFormat);
	int theShortCutKey = static_cast<int>(cfg->ReadLong("ShortcutKey", 0));
	if (theShortCutKey > -1 && theShortCutKey < 256)
		shortCutKey = theShortCutKey;
}

wxString Button::getName() {
	return name;
}

bool Button::isDisplayed() {
	return displayed;
}

void Button::setName(wxString name) {
	this->name = name;
}

int Button::getShortCutKey() {
	return shortCutKey;
}

void Button::setShortCutKey(int shortCutKey) {
	this->shortCutKey = shortCutKey;
}
