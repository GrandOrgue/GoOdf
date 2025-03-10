/*
 * GoPanel.h is part of GoOdf.
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

#ifndef GOPANEL_H
#define GOPANEL_H

#include <wx/wx.h>
#include <wx/textfile.h>
#include <wx/fileconf.h>
#include <list>
#include "GoImage.h"
#include "DisplayMetrics.h"
#include "GUIElements.h"
#include "Tremulant.h"
#include "DivisionalCoupler.h"
#include "ReversiblePiston.h"
#include "General.h"
#include "GUIManual.h"
#include "GUIEnclosure.h"

class Organ;

class GoPanel {
public:
	GoPanel();
	GoPanel(const GoPanel& p);
	~GoPanel();

	void write(wxTextFile *outFile, unsigned panelNbr);
	void read(wxFileConfig *cfg, wxString panelId, Organ *readOrgan);

	wxString getName();
	void setName(wxString name);
	wxString getGroup();
	void setGroup(wxString group);
	bool getHasPedals();
	void setHasPedals(bool hasPedals);
	unsigned getNumberOfImages();
	unsigned getIndexOfImage(GoImage *image);
	GoImage* getImageAt(unsigned index);
	void addImage(GoImage image);
	void removeImageAt(unsigned index);
	void removeImage(GoImage *image);
	DisplayMetrics* getDisplayMetrics();
	void addGuiElement(GUIElement *element);
	void removeGuiElementAt(unsigned index);
	int getNumberOfGuiElements();
	GUIElement* getGuiElementAt(unsigned index);
	bool hasItemAsGuiElement(Tremulant *trem);
	void removeItemFromPanel(Tremulant *trem);
	bool hasItemAsGuiElement(Enclosure *enclosure);
	void removeItemFromPanel(Enclosure *enclosure);
	bool hasItemAsGuiElement(Manual *manual);
	void removeItemFromPanel(Manual *manual);
	bool hasItemAsGuiElement(Stop *stop);
	void removeItemFromPanel(Stop *stop);
	bool hasItemAsGuiElement(Coupler *coupler);
	void removeItemFromPanel(Coupler *coupler);
	bool hasItemAsGuiElement(Divisional *divisional);
	void removeItemFromPanel(Divisional *divisional);
	bool hasItemAsGuiElement(GoSwitch *sw);
	void removeItemFromPanel(GoSwitch *sw);
	bool hasItemAsGuiElement(DivisionalCoupler *divCplr);
	void removeItemFromPanel(DivisionalCoupler *divCplr);
	bool hasItemAsGuiElement(ReversiblePiston *revPist);
	void removeItemFromPanel(ReversiblePiston *revPist);
	bool hasItemAsGuiElement(General *general);
	void removeItemFromPanel(General *general);
	void updateGuiElementsDisplayNames();
	void moveGuiElement(int sourceIndex, int toBeforeIndex);
	void updateButtonRowsAndCols();
	void applyButtonFontName();
	void applyButtonFontSize();
	void applyLabelFontName();
	void applyLabelFontSize();
	void setIsGuiElementFirstRemoval(bool value);
	bool getIsGuiElementFirstRemoval();
	bool getIsGuiManualTheFirst(GUIManual *manual);
	void updateGuiManuals();
	unsigned getNumberOfManuals();
	GUIManual* getGuiManualAt(unsigned index);
	void updateGuiEnclosures();
	unsigned getNumberOfEnclosures();
	GUIEnclosure* getGuiEnclosureAt(unsigned index);

private:
	wxString m_name;
	wxString m_group;
	bool m_hasPedals;
	std::list<GoImage> m_images;
	DisplayMetrics m_displayMetrics;
	std::list<GUIElement*> m_guiElements;
	bool m_isGuiElementFirstRemoval;
	std::list<GUIManual*> m_manuals;
	std::list<GUIEnclosure*> m_enclosures;

};

#endif
