/*
 * Tremulant.cpp is part of GOODF.
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

#include "Tremulant.h"

Tremulant::Tremulant() : Drawstop() {
	tremType = wxT("Synth");
	period = 160;
	startRate = 8;
	stopRate = 8;
	ampModDepth = 18;
	name = wxT("New Tremulant");
}

Tremulant::~Tremulant() {

}

void Tremulant::write(wxTextFile *outFile) {
	Drawstop::write(outFile);
	if (tremType.IsSameAs(wxT("Synth"))) {
		outFile->AddLine(wxT("Period=") + wxString::Format(wxT("%i"), period));
		outFile->AddLine(wxT("AmpModDepth=") + wxString::Format(wxT("%i"), ampModDepth));
		outFile->AddLine(wxT("StartRate=") + wxString::Format(wxT("%i"), startRate));
		outFile->AddLine(wxT("StopRate=") + wxString::Format(wxT("%i"), stopRate));
	} else {
		outFile->AddLine(wxT("TremulantType=") + tremType);
	}
}

int Tremulant::getAmpModDepth() {
	return ampModDepth;
}

void Tremulant::setAmpModDepth(int ampModDepth) {
	this->ampModDepth = ampModDepth;
}

int Tremulant::getPeriod() {
	return period;
}

void Tremulant::setPeriod(int period) {
	this->period = period;
}

int Tremulant::getStartRate() {
	return startRate;
}

void Tremulant::setStartRate(int startRate) {
	this->startRate = startRate;
}

int Tremulant::getStopRate() {
	return stopRate;
}

void Tremulant::setStopRate(int stopRate) {
	this->stopRate = stopRate;
}

wxString Tremulant::getTremType() {
	return tremType;
}

void Tremulant::setTremType(wxString tremType) {
	this->tremType = tremType;
}
