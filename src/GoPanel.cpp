/*
 * GoPanel.cpp is part of GoOdf.
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

#include "GoPanel.h"
#include "GOODFFunctions.h"
#include "GUITremulant.h"
#include "GUIStop.h"
#include "GUICoupler.h"
#include "GUIDivisional.h"
#include "GUISwitch.h"
#include "GUIDivisionalCoupler.h"
#include "GUIReversiblePiston.h"
#include "GUIGeneral.h"

GoPanel::GoPanel() {
	m_name = wxT("New Panel");
	m_group = wxEmptyString;
	m_hasPedals = false;
	m_isGuiElementFirstRemoval = true;
}

GoPanel::GoPanel(const GoPanel& p) {
	m_name = p.m_name;
	m_group = p.m_group;
	m_hasPedals = p.m_hasPedals;
	m_isGuiElementFirstRemoval = true;
	m_displayMetrics = p.m_displayMetrics;

	for (GoImage img : p.m_images) {
		addImage(img);
	}

	for (GUIElement *e : p.m_guiElements) {
		addGuiElement(e->clone());
	}

	updateGuiManuals();
	updateGuiEnclosures();
}

GoPanel::~GoPanel() {
	m_guiElements.remove_if([](GUIElement *element){delete element; return true;});
}

void GoPanel::write(wxTextFile *outFile, unsigned panelNbr) {
	if (panelNbr > 0) {
		outFile->AddLine(wxT("Name=") + m_name);
		if (m_group != wxEmptyString)
			outFile->AddLine(wxT("Group=") + m_group);
	}
	if (m_hasPedals)
		outFile->AddLine(wxT("HasPedals=Y"));
	else
		outFile->AddLine(wxT("HasPedals=N"));
	unsigned nbImages = getNumberOfImages();
	outFile->AddLine(wxT("NumberOfImages=") + wxString::Format(wxT("%u"), nbImages));

	unsigned nbGUIElements = m_guiElements.size();
	outFile->AddLine(wxT("NumberOfGUIElements=") + wxString::Format(wxT("%u"), nbGUIElements));

	m_displayMetrics.write(outFile);
	outFile->AddLine(wxT(""));

	unsigned i = 1;
	// images
	for (auto& img :  m_images) {
		wxString imgId = wxT("[Panel") + GOODF_functions::number_format(panelNbr) + wxT("Image") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(imgId);
		img.write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	i = 1;
	// GUI Elements
	for (GUIElement* gui : m_guiElements) {
		wxString guiId = wxT("[Panel") + GOODF_functions::number_format(panelNbr) + wxT("Element") + GOODF_functions::number_format(i) + wxT("]");
		outFile->AddLine(guiId);
		gui->write(outFile);
		outFile->AddLine(wxT(""));
		i++;
	}

	if (panelNbr == 0 && nbGUIElements < 1) {
		wxLogWarning("Nothing is displayed as a GUI Element on the main panel! Is this really intentional?");
		::wxGetApp().m_frame->GetLogWindow()->Show(true);
	}
}

void GoPanel::read(wxFileConfig *cfg, wxString panelId, Organ *readOrgan) {
	if (panelId != wxT("Panel000"))
		m_name = cfg->Read("Name", wxEmptyString);
	m_group = cfg->Read("Group", wxEmptyString);
	wxString cfgBoolValue = cfg->Read("HasPedals", wxEmptyString);
	m_hasPedals = GOODF_functions::parseBoolean(cfgBoolValue, false);
	m_displayMetrics.read(cfg);
	int nbrImages = static_cast<int>(cfg->ReadLong("NumberOfImages", 0));
	if (nbrImages > 0 && nbrImages < 1000) {
		for (int i = 0; i < nbrImages; i++) {
			wxString imgGroupName = wxT("Image") + GOODF_functions::number_format(i + 1);
			cfg->SetPath(wxT("/"));
			if (cfg->HasGroup(panelId + imgGroupName)) {
				cfg->SetPath(wxT("/") + panelId + imgGroupName);
				GoImage img;
				img.setOwningPanelWidth(m_displayMetrics.m_dispScreenSizeHoriz.getNumericalValue());
				img.setOwningPanelHeight(m_displayMetrics.m_dispScreenSizeVert.getNumericalValue());
				bool imgIsOk = img.read(cfg, readOrgan);
				if (imgIsOk)
					addImage(img);
			}
		}
		cfg->SetPath(wxT("/") + panelId);
	}
}

wxString GoPanel::getName() {
	return m_name;
}

void GoPanel::setName(wxString name) {
	m_name = name;
}

wxString GoPanel::getGroup() {
	return m_group;
}

void GoPanel::setGroup(wxString group) {
	m_group = group;
}

bool GoPanel::getHasPedals() {
	return m_hasPedals;
}

void GoPanel::setHasPedals(bool hasPedals) {
	m_hasPedals = hasPedals;
	updateGuiManuals();
}

unsigned GoPanel::getNumberOfImages() {
	return m_images.size();
}

unsigned GoPanel::getIndexOfImage(GoImage *image) {
	unsigned i = 0;
	bool found = false;
	for (auto& img : m_images) {
		i++;
		if (&img == image) {
			found = true;
	    	break;
	    }
	}
	if (found)
		return i;
	else
		return 0;
}

GoImage* GoPanel::getImageAt(unsigned index) {
	auto iterator = std::next(m_images.begin(), index);
	return &(*iterator);
}

void GoPanel::addImage(GoImage image) {
	m_images.push_back(image);
}

void GoPanel::removeImageAt(unsigned index) {
	std::list<GoImage>::iterator it = m_images.begin();
	std::advance(it, index);
	m_images.erase(it);
}

void GoPanel::removeImage(GoImage *image) {
	unsigned index = 0;
	for (GoImage& i : m_images) {
		if (&i == image) {
			removeImageAt(index);
			break;
		}
		index++;
	}
}

DisplayMetrics* GoPanel::getDisplayMetrics() {
	return &m_displayMetrics;
}

void GoPanel::addGuiElement(GUIElement *element) {
	m_guiElements.push_back(element);
	updateGuiManuals();
	updateGuiEnclosures();
}

void GoPanel::removeGuiElementAt(unsigned index) {
	std::list<GUIElement*>::iterator it = m_guiElements.begin();
	std::advance(it, index);
	delete *it;
	m_guiElements.erase(it);
	updateGuiManuals();
	updateGuiEnclosures();
}

int GoPanel::getNumberOfGuiElements() {
	return m_guiElements.size();
}

GUIElement* GoPanel::getGuiElementAt(unsigned index) {
	auto iterator = std::next(m_guiElements.begin(), index);
	return *iterator;
}

bool GoPanel::hasItemAsGuiElement(Tremulant* trem) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Tremulant")) {
			GUITremulant *tr = dynamic_cast<GUITremulant*>(e);
			if (tr) {
				if (tr->isReferencing(trem))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Tremulant* trem) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Tremulant")) {
			GUITremulant *tr = dynamic_cast<GUITremulant*>(*it);
			if (tr) {
				if (tr->isReferencing(trem)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Enclosure* enclosure) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Enclosure")) {
			GUIEnclosure *enc = dynamic_cast<GUIEnclosure*>(e);
			if (enc) {
				if (enc->isReferencing(enclosure))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Enclosure* enclosure) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Enclosure")) {
			GUIEnclosure *enc = dynamic_cast<GUIEnclosure*>(*it);
			if (enc) {
				if (enc->isReferencing(enclosure)) {
					// erase and go to next
					m_enclosures.remove(enc);
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
	updateGuiEnclosures();
}

bool GoPanel::hasItemAsGuiElement(Manual *manual) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Manual")) {
			GUIManual *man = dynamic_cast<GUIManual*>(e);
			if (man) {
				if (man->isReferencing(manual))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Manual *manual) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Manual")) {
			GUIManual *man = dynamic_cast<GUIManual*>(*it);
			if (man) {
				if (man->isReferencing(manual)) {
					// erase and go to next
					m_manuals.remove(man);
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
	updateGuiManuals();
}

bool GoPanel::hasItemAsGuiElement(Stop *stop) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Stop")) {
			GUIStop *st = dynamic_cast<GUIStop*>(e);
			if (st) {
				if (st->isReferencing(stop))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Stop *stop) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Stop")) {
			GUIStop *st = dynamic_cast<GUIStop*>(*it);
			if (st) {
				if (st->isReferencing(stop)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Coupler *coupler) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Coupler")) {
			GUICoupler *cplr = dynamic_cast<GUICoupler*>(e);
			if (cplr) {
				if (cplr->isReferencing(coupler))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Coupler *coupler) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Coupler")) {
			GUICoupler *cplr = dynamic_cast<GUICoupler*>(*it);
			if (cplr) {
				if (cplr->isReferencing(coupler)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(Divisional *divisional) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Divisional")) {
			GUIDivisional *div = dynamic_cast<GUIDivisional*>(e);
			if (div) {
				if (div->isReferencing(divisional))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(Divisional *divisional) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Divisional")) {
			GUIDivisional *div = dynamic_cast<GUIDivisional*>(*it);
			if (div) {
				if (div->isReferencing(divisional)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(GoSwitch *sw) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Switch")) {
			GUISwitch *s = dynamic_cast<GUISwitch*>(e);
			if (s) {
				if (s->isReferencing(sw))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(GoSwitch *sw) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("Switch")) {
			GUISwitch *s = dynamic_cast<GUISwitch*>(*it);
			if (s) {
				if (s->isReferencing(sw)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(DivisionalCoupler *divCplr) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("DivisionalCoupler")) {
			GUIDivisionalCoupler *div = dynamic_cast<GUIDivisionalCoupler*>(e);
			if (div) {
				if (div->isReferencing(divCplr))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(DivisionalCoupler *divCplr) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("DivisionalCoupler")) {
			GUIDivisionalCoupler *div = dynamic_cast<GUIDivisionalCoupler*>(*it);
			if (div) {
				if (div->isReferencing(divCplr)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(ReversiblePiston *revPist) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("ReversiblePiston")) {
			GUIReversiblePiston *p = dynamic_cast<GUIReversiblePiston*>(e);
			if (p) {
				if (p->isReferencing(revPist))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(ReversiblePiston *revPist) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("ReversiblePiston")) {
			GUIReversiblePiston *p = dynamic_cast<GUIReversiblePiston*>(*it);
			if (p) {
				if (p->isReferencing(revPist)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

bool GoPanel::hasItemAsGuiElement(General *general) {
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("General")) {
			GUIGeneral *g = dynamic_cast<GUIGeneral*>(e);
			if (g) {
				if (g->isReferencing(general))
					return true;
			}
		}
	}
	return false;
}

void GoPanel::removeItemFromPanel(General *general) {
	auto it = m_guiElements.begin();
	while (it != m_guiElements.end()) {
		if((*it)->getType() == wxT("General")) {
			GUIGeneral *g = dynamic_cast<GUIGeneral*>(*it);
			if (g) {
				if (g->isReferencing(general)) {
					// erase and go to next
					it = m_guiElements.erase(it);
				} else {
					// go to next
					++it;
				}
			} else {
				// go to next
				++it;
			}
		} else{
			// go to next
			++it;
		}
	}
}

void GoPanel::updateGuiElementsDisplayNames() {
	for (GUIElement* e : m_guiElements) {
		e->updateDisplayName();
	}
}

void GoPanel::moveGuiElement(int sourceIndex, int toBeforeIndex) {
	auto theOneToMove = std::next(m_guiElements.begin(), sourceIndex);
	std::list<GUIElement*>::iterator it = m_guiElements.begin();

	if (toBeforeIndex > (int) m_guiElements.size() - 1) {
		it = m_guiElements.end();
	} else {
		it = std::next(m_guiElements.begin(), toBeforeIndex);
	}

	m_guiElements.splice(it, m_guiElements, theOneToMove);
}

void GoPanel::updateButtonRowsAndCols() {
	for (GUIElement* e : m_guiElements) {
		GUIButton *btnElement = dynamic_cast<GUIButton*>(e);
		if (btnElement) {
			if (btnElement->getDispButtonCol() > m_displayMetrics.m_dispButtonCols) {
				btnElement->setDispButtonCol(1);
			}
			if (btnElement->getDispButtonRow() > 99 + m_displayMetrics.m_dispExtraButtonRows) {
				btnElement->setDispButtonRow(1);
			}
			if (btnElement->getDispDrawstopRow() < 100 && btnElement->getDispDrawstopCol() > m_displayMetrics.m_dispDrawstopCols) {
				btnElement->setDispDrawstopCol(1);
			} else if (btnElement->getDispDrawstopRow() > 99 && btnElement->getDispDrawstopCol() > 99 + m_displayMetrics.m_dispExtraDrawstopCols) {
				btnElement->setDispDrawstopCol(1);
			}
			if (btnElement->getDispDrawstopRow() > 99 + m_displayMetrics.m_dispExtraDrawstopRows) {
				btnElement->setDispDrawstopRow(1);
			}
		}
	}
}

void GoPanel::applyButtonFontName() {
	for (GUIElement* e : m_guiElements) {
		GUIButton *btnElement = dynamic_cast<GUIButton*>(e);
		if (btnElement) {
			wxFont control_font = m_displayMetrics.m_dispControlLabelFont;
			control_font.SetPointSize(btnElement->getDispLabelFontSize()->getSizeValue());
			btnElement->setDispLabelFont(control_font);
		}
	}
}

void GoPanel::applyButtonFontSize() {
	for (GUIElement* e : m_guiElements) {
		GUIButton *btnElement = dynamic_cast<GUIButton*>(e);
		if (btnElement) {
			int font_size = m_displayMetrics.m_dispControlLabelFont.GetPointSize();
			btnElement->setDispLabelFontSize(font_size);
		}
	}
}

void GoPanel::applyLabelFontName() {
	for (GUIElement* e : m_guiElements) {
		GUILabel *lblElement = dynamic_cast<GUILabel*>(e);
		if (lblElement) {
			wxFont label_font = m_displayMetrics.m_dispGroupLabelFont;
			label_font.SetPointSize(lblElement->getDispLabelFontSize()->getSizeValue());
			lblElement->setDispLabelFont(label_font);
		}
	}
}
void GoPanel::applyLabelFontSize() {
	for (GUIElement* e : m_guiElements) {
		GUILabel *lblElement = dynamic_cast<GUILabel*>(e);
		if (lblElement) {
			int font_size = m_displayMetrics.m_dispGroupLabelFont.GetPointSize();
			lblElement->setDispLabelFontSize(font_size);
		}
	}
}

void GoPanel::setIsGuiElementFirstRemoval(bool value) {
	m_isGuiElementFirstRemoval = value;
}
bool GoPanel::getIsGuiElementFirstRemoval() {
	return m_isGuiElementFirstRemoval;
}

bool GoPanel::getIsGuiManualTheFirst(GUIManual *manual) {
	bool isFirst = false;
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Manual")) {
			GUIManual *man = dynamic_cast<GUIManual*>(e);
			if (man) {
				if (man == manual) {
					isFirst = true;
					if (m_hasPedals)
						man->setDisplayAsPedal(true);
				}
			}
			break;
		}
	}
	return isFirst;
}

void GoPanel::updateGuiManuals() {
	if (!m_manuals.empty())
		m_manuals.clear();
	bool isFirst = true;
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Manual")) {
			GUIManual *man = dynamic_cast<GUIManual*>(e);
			if (man) {
				m_manuals.push_back(man);
				if (isFirst) {
					if (m_hasPedals) {
						man->setDisplayAsPedal(true);
					} else {
						man->setDisplayAsPedal(false);
					}
					isFirst = false;
				}
			}
		}
	}
}

unsigned GoPanel::getNumberOfManuals() {
	return m_manuals.size();
}

GUIManual* GoPanel::getGuiManualAt(unsigned index) {
	if (index < m_manuals.size()) {
		auto iterator = std::next(m_manuals.begin(), index);
		return (*iterator);
	} else {
		return NULL;
	}
}

void GoPanel::updateGuiEnclosures() {
	if (!m_enclosures.empty())
		m_enclosures.clear();
	for (GUIElement* e : m_guiElements) {
		if (e->getType() == wxT("Enclosure") || e->getType() == wxT("Swell")) {
			GUIEnclosure *enc = dynamic_cast<GUIEnclosure*>(e);
			if (enc) {
				m_enclosures.push_back(enc);
			}
		}
	}
}

unsigned GoPanel::getNumberOfEnclosures() {
	return m_enclosures.size();
}

GUIEnclosure* GoPanel::getGuiEnclosureAt(unsigned index) {
	if (index < m_enclosures.size()) {
		auto iterator = std::next(m_enclosures.begin(), index);
		return (*iterator);
	} else {
		return NULL;
	}
}
