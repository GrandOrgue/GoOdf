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

void Tremulant::read(wxFileConfig *cfg, bool usingOldPanelFormat) {
	Drawstop::read(cfg, usingOldPanelFormat);
	wxString typeValue = cfg->Read("TremulantType", wxT("Synth"));
	if (typeValue.IsSameAs(wxT("Synth"), false)) {
		tremType = typeValue;
		int cfgPeriod = static_cast<int>(cfg->ReadLong("Period", 160));
		if (cfgPeriod > 31 && cfgPeriod < 44101) {
			period = cfgPeriod;
		}
		int cfgStart = static_cast<int>(cfg->ReadLong("StartRate", 8));
		if (cfgStart > 0 && cfgStart < 101) {
			startRate = cfgStart;
		}
		int cfgStop = static_cast<int>(cfg->ReadLong("StopRate", 8));
		if (cfgStop > 0 && cfgStop < 101) {
			stopRate = cfgStop;
		}
		int cfgAmpMod = static_cast<int>(cfg->ReadLong("AmpModDepth", 18));
		if (cfgAmpMod > 0 && cfgAmpMod < 101) {
			ampModDepth = cfgAmpMod;
		}
	} else if (typeValue.IsSameAs(wxT("Wave"), false)) {
		tremType = typeValue;
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
