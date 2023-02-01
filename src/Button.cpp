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

Button::Button() {
	name = wxEmptyString;
	displayed = false;
	displayedInInvertedState = false;
	shortCutKey = 0;
}

Button::~Button() {

}

void Button::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + name);
	if (shortCutKey > 0)
		outFile->AddLine(wxT("ShortCutKey=") + wxString::Format(wxT("%i"), shortCutKey));
}

wxString Button::getName() {
	return name;
}

bool Button::isDisplayed() {
	return displayed;
}

void Button::setDisplayed(bool displayed) {
	this->displayed = displayed;
}

bool Button::isDisplayedInInvertedState() {
	return displayedInInvertedState;
}

void Button::setDisplayedInInvertedState(bool displayedInInvertedState) {
	this->displayedInInvertedState = displayedInInvertedState;
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
