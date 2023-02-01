/*
 * Enclosure.cpp is part of GOODF.
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

#include "Enclosure.h"

Enclosure::Enclosure() {
	name = wxT("New enclosure");
	ampMinimumLevel = 1;
	MIDIInputNumber = 0;
}

Enclosure::~Enclosure() {

}

void Enclosure::write(wxTextFile *outFile) {
	outFile->AddLine(wxT("Name=") + this->name);
	outFile->AddLine(wxT("AmpMinimumLevel=") + wxString::Format(wxT("%i"), ampMinimumLevel));
	if (MIDIInputNumber > 0)
		outFile->AddLine(wxT("MIDIInputNumber=") + wxString::Format(wxT("%i"), MIDIInputNumber));
}

int Enclosure::getAmpMinimumLevel() {
	return ampMinimumLevel;
}

void Enclosure::setAmpMinimumLevel(int ampMinimumLevel) {
	this->ampMinimumLevel = ampMinimumLevel;
}

int Enclosure::getMidiInputNumber() {
	return MIDIInputNumber;
}

void Enclosure::setMidiInputNumber(int midiInputNumber) {
	MIDIInputNumber = midiInputNumber;
}

wxString Enclosure::getName() {
	return name;
}

void Enclosure::setName(wxString name) {
	this->name = name;
}
