/*
 * GUIPanelRepresentation.cpp is part of GOODF.
 * Copyright (C) 2023 Lars Palo and contributors (see AUTHORS)
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

#include "GUIPanelRepresentation.h"
#include "GUIButton.h"
#include "GUILabel.h"

// Event table
BEGIN_EVENT_TABLE(GUIPanelRepresentation, wxDialog)
	EVT_PAINT(GUIPanelRepresentation::OnPaintEvent)
END_EVENT_TABLE()

GUIPanelRepresentation::GUIPanelRepresentation(wxWindow *parent, const wxString& title) : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX) {
	m_currentPanel = NULL;
	m_HackY = 0;
	m_EnclosureY = 0;
	m_CenterY = 0;
	m_CenterWidth = 0;
}

GUIPanelRepresentation::~GUIPanelRepresentation() {

}

void GUIPanelRepresentation::SetCurrentPanel(GoPanel *thePanel) {
	m_currentPanel = thePanel;
	SetClientSize(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	SetTitle(m_currentPanel->getName());
	UpdateLayout();
	DoPaintNow();
}

void GUIPanelRepresentation::OnPaintEvent(wxPaintEvent& WXUNUSED(event)) {
	wxPaintDC dc(this);
	RenderPanel(dc);
}

void GUIPanelRepresentation::DoPaintNow() {
	if (this->IsShown()) {
		wxClientDC dc(this);
		RenderPanel(dc);
	}
}

void GUIPanelRepresentation::RenderPanel(wxDC& dc) {
	// First draw the basic background of left jamb
	wxRect rect = wxRect(0, 0, GetCenterX(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	wxBitmap stopBg = m_currentPanel->getDisplayMetrics()->getDrawstopBg();
	TileBitmap(rect, dc, stopBg, 0, 0);

	// Right jamb
	rect = wxRect((GetCenterX() + GetCenterWidth()), 0, m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - (GetCenterX() + GetCenterWidth()), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	TileBitmap(rect, dc, stopBg, 0, 0);

	// Console, middle part
	rect = wxRect(GetCenterX(), 0, GetCenterWidth(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	wxBitmap consoleBg = m_currentPanel->getDisplayMetrics()->getConsoleBg();
	TileBitmap(rect, dc, consoleBg, 0, 0);

	// Inset for paired drawstops
	if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols) {
		for (int i = 0; i < (m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols >> 2); i++) {
			rect = wxRect(i * (2 * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth + 18) + GetJambLeftX() - 5, GetJambLeftRightY(), 2 * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth + 10, GetJambLeftRightHeight());
			wxBitmap insetBg = m_currentPanel->getDisplayMetrics()->getInsetBg();
			TileBitmap(rect, dc, insetBg, 0, 0);

			rect = wxRect(i * (2 * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth + 18) + GetJambRightX() - 5, GetJambLeftRightY(), 2 * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth + 10, GetJambLeftRightHeight());
			TileBitmap(rect, dc, insetBg, 0, 0);
		}
	}

	// Trim above extra rows
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows) {
		rect = wxRect(GetCenterX(), GetCenterY(), GetCenterWidth(), 8);
		wxBitmap keyVert = m_currentPanel->getDisplayMetrics()->getKeyVertBg();
		TileBitmap(rect, dc, keyVert, 0, 0);
	}

	if (GetJambTopHeight() + GetPistonTopHeight()) {
		rect = wxRect(GetCenterX(), GetJambTopY(), GetCenterWidth(), GetJambTopHeight() + GetPistonTopHeight());
		wxBitmap keyHoriz = m_currentPanel->getDisplayMetrics()->getKeyHorizBg();
		TileBitmap(rect, dc, keyHoriz, 0, 0);
	}

	// Manual backgrounds
	for (unsigned i = 0; i < m_currentPanel->getNumberOfManuals(); i++) {
		GUIManual *currentMan = m_currentPanel->getGuiManualAt(i);
		wxRect vRect = wxRect(GetCenterX(), currentMan->m_renderInfo.y, GetCenterWidth(), currentMan->m_renderInfo.height);
		wxBitmap keyVert = m_currentPanel->getDisplayMetrics()->getKeyVertBg();
		TileBitmap(vRect, dc, keyVert, 0, 0);

		wxRect hRect = wxRect(GetCenterX(), currentMan->m_renderInfo.piston_y, GetCenterWidth(), (!i && m_currentPanel->getDisplayMetrics()->m_dispExtraPedalButtonRow) ? 2 * m_currentPanel->getDisplayMetrics()->m_dispPistonHeight : m_currentPanel->getDisplayMetrics()->m_dispPistonHeight);
		wxBitmap keyHoriz = m_currentPanel->getDisplayMetrics()->getKeyHorizBg();
		TileBitmap(hRect, dc, keyHoriz, 0, 0);
	}

	if (m_currentPanel->getNumberOfImages() > 0) {
		for (unsigned i = 0; i < m_currentPanel->getNumberOfImages(); i++) {
			if (m_currentPanel->getImageAt(i)->getWidth() > m_currentPanel->getImageAt(i)->getOriginalWidth() || m_currentPanel->getImageAt(i)->getHeight() > m_currentPanel->getImageAt(i)->getOriginalHeight()) {
				wxRect imgRect(
					m_currentPanel->getImageAt(i)->getPositionX(),
					m_currentPanel->getImageAt(i)->getPositionY(),
					m_currentPanel->getImageAt(i)->getWidth(),
					m_currentPanel->getImageAt(i)->getHeight()
				);
				wxBitmap theBmp = m_currentPanel->getImageAt(i)->getBitmap();
				TileBitmap(imgRect, dc, theBmp, m_currentPanel->getImageAt(i)->getTileOffsetX(), m_currentPanel->getImageAt(i)->getTileOffsetY());
			} else {
				dc.DrawBitmap(m_currentPanel->getImageAt(i)->getBitmap(), m_currentPanel->getImageAt(i)->getPositionX(), m_currentPanel->getImageAt(i)->getPositionY(), true);
			}
		}
	}

	// Manual keys drawn after images!
	for (unsigned i = 0; i < m_currentPanel->getNumberOfManuals(); i++) {
		GUIManual *currentMan = m_currentPanel->getGuiManualAt(i);
		int manXpos = currentMan->m_renderInfo.x;
		int manYpos = currentMan->m_renderInfo.y;
		if (currentMan->getPosX() >= 0)
			manXpos = currentMan->getPosX();
		if (currentMan->getPosY() >= 0)
			manYpos = currentMan->getPosY();
		for (int j = 0; j < currentMan->getNumberOfDisplayKeys(); j++) {
			KEY_INFO *currentKey = currentMan->getKeyInfoAt(j);
			wxBitmap theKey = currentKey->KeyImage;
			dc.DrawBitmap(theKey, manXpos + currentKey->Xpos, manYpos + currentKey->Ypos, true);
		}
	}

	if (m_currentPanel->getNumberOfGuiElements() > 0) {
		for (unsigned i = 0; i < (unsigned) m_currentPanel->getNumberOfGuiElements(); i++) {
			GUIElement *guiElement = m_currentPanel->getGuiElementAt(i);
			wxBitmap theBmp = guiElement->getBitmap();
			// Test if it's a button type
			GUIButton *btnElement = dynamic_cast<GUIButton*>(guiElement);
			if (btnElement) {
				if (btnElement->getPosX() != -1) {
					// the "button" uses absolute pixel positioning
					if (btnElement->getWidth() > btnElement->getBitmapWidth() || btnElement->getHeight() > btnElement->getBitmapHeight()) {
						wxRect imgRect(btnElement->getPosX(), btnElement->getPosY(), btnElement->getWidth(), btnElement->getHeight());
						TileBitmap(imgRect, dc, theBmp, btnElement->getTileOffsetX(), btnElement->getTileOffsetY());
					} else {
						dc.DrawBitmap(theBmp, btnElement->getPosX(), btnElement->getPosY(), true);
					}
					if (btnElement->getTextBreakWidth()) {
						dc.SetFont(btnElement->getDispLabelFont());
						dc.SetBackgroundMode(wxTRANSPARENT);
						dc.SetTextForeground(btnElement->getDispLabelColour()->getColor());
						wxRect textRect(
							btnElement->getPosX() + btnElement->getTextRectLeft(),
							btnElement->getPosY() + btnElement->getTextRectTop(),
							btnElement->getTextRectWidth(),
							btnElement->getTextRectHeight()
						);
						wxString textToDisplay;
						if (btnElement->getDispLabelText() != wxEmptyString)
							textToDisplay = btnElement->getDispLabelText();
						else
							textToDisplay = btnElement->getDisplayName();
						if (textToDisplay.Contains(wxT("Setter")) && textToDisplay.Contains(wxT("Divisional")) && textToDisplay.Len() == 22) {
							wxString combinationNbr = textToDisplay.Mid(19);
							long theNbr;
							if (combinationNbr.ToLong(&theNbr)) {
								theNbr += 1;
								textToDisplay = wxString::Format(wxT("%ld"), theNbr);
							}
						} else if (textToDisplay.Contains(wxT("General")) && textToDisplay.Len() == 9) {
							wxString combinationNbr = textToDisplay.Mid(7);
							long theNbr;
							if (combinationNbr.ToLong(&theNbr)) {
								textToDisplay = wxString::Format(wxT("%ld"), theNbr);
							}
						}
						dc.DrawLabel(BreakTextLine(textToDisplay, btnElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
					}
				} else {
					if (btnElement->isDisplayAsPiston()) {
						wxPoint thePos = GetPushbuttonPosition(btnElement->getDispButtonRow(), btnElement->getDispButtonCol());
						if (btnElement->getWidth() > btnElement->getBitmapWidth() || btnElement->getHeight() > btnElement->getBitmapHeight()) {
							wxRect imgRect(thePos.x, thePos.y, btnElement->getWidth(), btnElement->getHeight());
							TileBitmap(imgRect, dc, theBmp, btnElement->getTileOffsetX(), btnElement->getTileOffsetY());
						} else {
							dc.DrawBitmap(theBmp, thePos.x, thePos.y, true);
						}
						if (btnElement->getTextBreakWidth()) {
							dc.SetFont(btnElement->getDispLabelFont());
							dc.SetBackgroundMode(wxTRANSPARENT);
							dc.SetTextForeground(btnElement->getDispLabelColour()->getColor());
							wxRect textRect(
								thePos.x + btnElement->getTextRectLeft(),
								thePos.y + btnElement->getTextRectTop(),
								btnElement->getTextRectWidth(),
								btnElement->getTextRectHeight()
							);
							wxString textToDisplay;
							if (btnElement->getDispLabelText() != wxEmptyString)
								textToDisplay = btnElement->getDispLabelText();
							else
								textToDisplay = btnElement->getDisplayName();
							if (textToDisplay.Contains(wxT("Setter")) && textToDisplay.Contains(wxT("Divisional")) && textToDisplay.Len() == 22) {
								wxString combinationNbr = textToDisplay.Mid(19);
								long theNbr;
								if (combinationNbr.ToLong(&theNbr)) {
									theNbr += 1;
									textToDisplay = wxString::Format(wxT("%ld"), theNbr);
								}
							} else if (textToDisplay.Contains(wxT("General")) && textToDisplay.Len() == 9) {
								wxString combinationNbr = textToDisplay.Mid(7);
								long theNbr;
								if (combinationNbr.ToLong(&theNbr)) {
									textToDisplay = wxString::Format(wxT("%ld"), theNbr);
								}
							}
							dc.DrawLabel(BreakTextLine(textToDisplay, btnElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
						}
					} else {
						wxPoint thePos = GetDrawstopPosition(btnElement->getDispDrawstopRow(), btnElement->getDispDrawstopCol());
						if (btnElement->getWidth() > btnElement->getBitmapWidth() || btnElement->getHeight() > btnElement->getBitmapHeight()) {
							wxRect imgRect(thePos.x, thePos.y, btnElement->getWidth(), btnElement->getHeight());
							TileBitmap(imgRect, dc, theBmp, btnElement->getTileOffsetX(), btnElement->getTileOffsetY());
						} else {
							dc.DrawBitmap(theBmp, thePos.x, thePos.y, true);
						}
						if (btnElement->getTextBreakWidth()) {
							dc.SetFont(btnElement->getDispLabelFont());
							dc.SetBackgroundMode(wxTRANSPARENT);
							dc.SetTextForeground(btnElement->getDispLabelColour()->getColor());
							wxRect textRect(
								thePos.x + btnElement->getTextRectLeft(),
								thePos.y + btnElement->getTextRectTop(),
								btnElement->getTextRectWidth(),
								btnElement->getTextRectHeight()
							);
							wxString textToDisplay;
							if (btnElement->getDispLabelText() != wxEmptyString)
								textToDisplay = btnElement->getDispLabelText();
							else
								textToDisplay = btnElement->getDisplayName();
							if (textToDisplay.Contains(wxT("Setter")) && textToDisplay.Contains(wxT("Divisional")) && textToDisplay.Len() == 22) {
								wxString combinationNbr = textToDisplay.Mid(19);
								long theNbr;
								if (combinationNbr.ToLong(&theNbr)) {
									theNbr += 1;
									textToDisplay = wxString::Format(wxT("%ld"), theNbr);
								}
							} else if (textToDisplay.Contains(wxT("General")) && textToDisplay.Len() == 9) {
								wxString combinationNbr = textToDisplay.Mid(7);
								long theNbr;
								if (combinationNbr.ToLong(&theNbr)) {
									textToDisplay = wxString::Format(wxT("%ld"), theNbr);
								}
							}
							dc.DrawLabel(BreakTextLine(textToDisplay, btnElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
						}
					}
				}
				continue;
			}
			GUIEnclosure *encElement = dynamic_cast<GUIEnclosure*>(guiElement);
			if (encElement) {
				if (encElement->getPosX() != -1) {
					// the enclosure uses absolute pixel positioning
					if (encElement->getWidth() > encElement->getBitmapWidth() || encElement->getHeight() > encElement->getBitmapHeight()) {
						wxRect imgRect(encElement->getPosX(), encElement->getPosY(), encElement->getWidth(), encElement->getHeight());
						TileBitmap(imgRect, dc, theBmp, encElement->getTileOffsetX(), encElement->getTileOffsetY());
					} else {
						dc.DrawBitmap(theBmp, encElement->getPosX(), encElement->getPosY(), true);
					}
					if (encElement->getTextBreakWidth()) {
						dc.SetFont(encElement->getDispLabelFont());
						dc.SetBackgroundMode(wxTRANSPARENT);
						dc.SetTextForeground(encElement->getDispLabelColour()->getColor());
						wxRect textRect(
							encElement->getPosX() + encElement->getTextRectLeft(),
							encElement->getPosY() + encElement->getTextRectTop(),
							encElement->getTextRectWidth(),
							encElement->getTextRectHeight()
						);
						dc.DrawLabel(BreakTextLine(encElement->getDisplayName(), encElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_HORIZONTAL);
					}
				} else {
					// the enclosure uses the default layout model
					int enclosureNumberOnPanel = -1;
					for (unsigned j = 0; j < m_currentPanel->getNumberOfEnclosures(); j++) {
						if (encElement == m_currentPanel->getGuiEnclosureAt(j)) {
							enclosureNumberOnPanel = j;
							break;
						}
					}
					if (enclosureNumberOnPanel != -1) {
						enclosureNumberOnPanel += 1;
						if (encElement->getWidth() > encElement->getBitmapWidth() || encElement->getHeight() > encElement->getBitmapHeight()) {
							wxRect imgRect(GetEnclosureX(enclosureNumberOnPanel), GetEnclosureY(), encElement->getWidth(), encElement->getHeight());
							TileBitmap(imgRect, dc, theBmp, encElement->getTileOffsetX(), encElement->getTileOffsetY());
						} else {
							dc.DrawBitmap(theBmp, GetEnclosureX(enclosureNumberOnPanel), GetEnclosureY(), true);
						}
						if (encElement->getTextBreakWidth()) {
							dc.SetFont(encElement->getDispLabelFont());
							dc.SetBackgroundMode(wxTRANSPARENT);
							dc.SetTextForeground(encElement->getDispLabelColour()->getColor());
							wxRect textRect(
								GetEnclosureX(enclosureNumberOnPanel) + encElement->getTextRectLeft(),
								GetEnclosureY() + encElement->getTextRectTop(),
								encElement->getTextRectWidth(),
								encElement->getTextRectHeight()
							);
							dc.DrawLabel(BreakTextLine(encElement->getDisplayName(), encElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_HORIZONTAL);
						}
					} else {
						continue;
					}
				}
				continue;
			}
			GUILabel *labelElement = dynamic_cast<GUILabel*>(guiElement);
			if (labelElement) {
				int xPosToUse = 0;
				if (labelElement->isFreeXPlacement()) {
					// the label uses absolute x positioning either via m_dispXpos or m_positionX
					if (labelElement->getPosX() >= 0)
						xPosToUse = labelElement->getPosX();
					else
						xPosToUse = labelElement->getDispXpos();
				} else {
					// the label uses the default layout model for x positioning
					int drawstopsPerJamb = m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols / 2;
					if (labelElement->isDispSpanDrawstopColToRight())
						xPosToUse = 39;
					if (labelElement->getDispDrawstopCol() <= drawstopsPerJamb)
						xPosToUse = xPosToUse + (labelElement->getDispDrawstopCol() - 1) * 78 + 1;
					else
						xPosToUse = -(xPosToUse + (labelElement->getDispDrawstopCol() - 1 - drawstopsPerJamb) * 78 + 1);

					if (xPosToUse >= 0)
						xPosToUse = GetJambLeftX() + xPosToUse;
					else
						xPosToUse = GetJambRightX() - xPosToUse;
				}

				int yPosToUse = 0;
				if (labelElement->isFreeYPlacement()) {
					// the label uses absolute y positioning either via m_dispYpos or m_positionY
					if (labelElement->getPosY() >= 0)
						yPosToUse = labelElement->getPosY();
					else
						yPosToUse = labelElement->getDispYpos();
				} else {
					// the label uses default layout model for y positioning
					yPosToUse = 1;
					if (!labelElement->isDispAtTopOfDrawstopCol())
						yPosToUse += -32;

					if (yPosToUse >= 0)
						yPosToUse = GetJambLeftRightY() + 1;
					else
						yPosToUse = GetJambLeftRightY() + 1 + GetJambLeftRightHeight() - 32;
				}

				if (theBmp.IsOk()) { // @suppress("Method cannot be resolved")
					if (labelElement->getWidth() > labelElement->getBitmapWidth() || labelElement->getHeight() > labelElement->getBitmapHeight()) {
						wxRect imgRect(xPosToUse, yPosToUse, labelElement->getWidth(), labelElement->getHeight());
						TileBitmap(imgRect, dc, theBmp, labelElement->getTileOffsetX(), labelElement->getTileOffsetY());
					} else {
						dc.DrawBitmap(theBmp, xPosToUse, yPosToUse, true);
					}
				}

				if (labelElement->getTextBreakWidth() && labelElement->getName() != wxEmptyString) {
					dc.SetFont(labelElement->getDispLabelFont());
					dc.SetBackgroundMode(wxTRANSPARENT);
					dc.SetTextForeground(labelElement->getDispLabelColour()->getColor());
					wxRect textRect(
						xPosToUse + labelElement->getTextRectLeft(),
						yPosToUse + labelElement->getTextRectTop(),
						labelElement->getTextRectWidth(),
						labelElement->getTextRectHeight()
					);
					dc.DrawLabel(BreakTextLine(labelElement->getName(), labelElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
				}
			}
		}
	}
}

void GUIPanelRepresentation::TileBitmap(wxRect rect, wxDC& dc, wxBitmap& bitmap, int tileOffsetX, int tileOffsetY) {
	int w = bitmap.GetWidth();
	int h = bitmap.GetHeight();

	wxImage wholeImg(rect.width, rect.height);
	wxImage bmp = bitmap.ConvertToImage();
	for (int i = -tileOffsetX; i < rect.width; i += w) {
		for (int j = -tileOffsetY; j < rect.height; j += h) {
			wholeImg.Paste(bmp, i, j);
		}
	}
	wxBitmap fullBmp = wxBitmap(wholeImg);

	dc.DrawBitmap(fullBmp, rect.x, rect.y, true);
}

wxPoint GUIPanelRepresentation::GetDrawstopPosition(int row, int col) {
	wxPoint position;
	int i;
	if (row > 99) {
		position.x = GetJambTopX() + (col - 1) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth + 6;
		if (m_currentPanel->getDisplayMetrics()->m_dispExtraDrawstopRowsAboveExtraButtonRows) {
			position.y = GetJambTopDrawstop() + (row - 100) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight + 2;
		} else {
			position.y = GetJambTopDrawstop() + (row - 100) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight + (m_currentPanel->getDisplayMetrics()->m_dispExtraButtonRows * m_currentPanel->getDisplayMetrics()->m_dispPistonHeight) + 2;
		}
	} else {
		i = m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols >> 1;
		if (col <= i) {
			position.x = GetJambLeftX() + (col - 1) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth + 6;
			position.y = GetJambLeftRightY() + (row - 1) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight + 32;
		} else {
			position.x = GetJambRightX() + (col - 1 - i) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth + 6;
			position.y = GetJambLeftRightY() + (row - 1) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight + 32;
		}
		if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols)
			position.x += (((col - 1) % i) >> 1) * (m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth / 4);

		if (col <= i)
			i = col;
		else
			i = m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols - col + 1;
		if (m_currentPanel->getDisplayMetrics()->m_dispDrawstopColsOffset && ((i & 1) ^ m_currentPanel->getDisplayMetrics()->m_dispDrawstopOuterColOffsetUp))
			position.y += m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight / 2;
	}
	return position;
}

wxPoint GUIPanelRepresentation::GetPushbuttonPosition(int row, int col) {
	wxPoint position;
	position.x = GetPistonX() + (col - 1) * m_currentPanel->getDisplayMetrics()->m_dispPistonWidth + 6;
	if (row > 99) {
		if (m_currentPanel->getDisplayMetrics()->m_dispExtraDrawstopRowsAboveExtraButtonRows) {
			position.y = GetJambTopPiston() + (row - 100) * m_currentPanel->getDisplayMetrics()->m_dispPistonHeight + (m_currentPanel->getDisplayMetrics()->m_dispExtraDrawstopRows * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight) + 5;
		} else {
			position.y = GetJambTopPiston() + (row - 100) * m_currentPanel->getDisplayMetrics()->m_dispPistonHeight + 5;
		}
	} else {
		int i = row;
		if (i == 99)
			i = 0;

		if (i >= (int) m_currentPanel->getNumberOfManuals())
			position.y = GetHackY() - (i + 1 - (int) m_currentPanel->getNumberOfManuals()) * (m_currentPanel->getDisplayMetrics()->m_dispManualHeight + m_currentPanel->getDisplayMetrics()->m_dispPistonHeight) + m_currentPanel->getDisplayMetrics()->m_dispManualHeight + 5;
		else
			position.y = m_currentPanel->getGuiManualAt(i)->m_renderInfo.piston_y + 5;

		if (m_currentPanel->getDisplayMetrics()->m_dispExtraPedalButtonRow && !row)
			position.y += m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
		if (m_currentPanel->getDisplayMetrics()->m_dispExtraPedalButtonRowOffset && row == 99)
			position.x -= m_currentPanel->getDisplayMetrics()->m_dispPistonWidth / 2 + 2;
	}
	return position;
}

unsigned GUIPanelRepresentation::GetEnclosuresWidth() {
	return m_currentPanel->getDisplayMetrics()->m_dispEnclosureWidth * m_currentPanel->getNumberOfEnclosures();
}

int GUIPanelRepresentation::GetEnclosureY() {
	return m_EnclosureY;
}

int GUIPanelRepresentation::GetEnclosureX(int enclosureNbr) {
	int enclosure_x = (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetEnclosuresWidth() + 6) >> 1;
	enclosure_x += m_currentPanel->getDisplayMetrics()->m_dispEnclosureWidth * (enclosureNbr - 1);

	return enclosure_x;
}

int GUIPanelRepresentation::GetJambLeftRightWidth() {
	int jamblrw = m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth / 2;
	if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols)
		jamblrw += ((m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols >> 2) * (m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth / 4)) - 8;
	return jamblrw;
}

unsigned GUIPanelRepresentation::GetJambLeftRightHeight() {
	return (m_currentPanel->getDisplayMetrics()->m_dispDrawstopRows + 1) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight;
}

int GUIPanelRepresentation::GetJambLeftRightY() {
	return ((int)(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - GetJambLeftRightHeight() - (m_currentPanel->getDisplayMetrics()->m_dispDrawstopColsOffset ? (m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight / 2) : 0))) / 2;
}

int GUIPanelRepresentation::GetJambLeftX() {
	int jamblx = (GetCenterX() - GetJambLeftRightWidth()) >> 1;
	if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols)
		jamblx += 5;
	return jamblx;
}

int GUIPanelRepresentation::GetJambRightX() {
	int jambrx = GetJambLeftX() + GetCenterX() + GetCenterWidth();
	if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols)
		jambrx += 5;
	return jambrx;
}

int GUIPanelRepresentation::GetJambTopDrawstop() {
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows)
		return GetCenterY() + 8;
	return GetCenterY();
}

int GUIPanelRepresentation::GetJambTopPiston() {
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows)
		return GetCenterY() + 8;
	return GetCenterY();
}

unsigned GUIPanelRepresentation::GetJambTopHeight() {
	return m_currentPanel->getDisplayMetrics()->m_dispExtraDrawstopRows * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight;
}

unsigned GUIPanelRepresentation::GetJambTopWidth() {
	return m_currentPanel->getDisplayMetrics()->m_dispExtraDrawstopCols * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth;
}

int GUIPanelRepresentation::GetJambTopX() {
	return (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetJambTopWidth()) >> 1;
}

int GUIPanelRepresentation::GetJambTopY() {
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows)
		return GetCenterY() + 8;
	return GetCenterY();
}

unsigned GUIPanelRepresentation::GetPistonTopHeight() {
	return m_currentPanel->getDisplayMetrics()->m_dispExtraButtonRows * m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
}

unsigned GUIPanelRepresentation::GetPistonWidth() {
	return m_currentPanel->getDisplayMetrics()->m_dispButtonCols * m_currentPanel->getDisplayMetrics()->m_dispPistonWidth;
}

int GUIPanelRepresentation::GetPistonX() {
	return (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetPistonWidth()) >> 1;
}

int GUIPanelRepresentation::GetCenterWidth() {
	return m_CenterWidth;
}

int GUIPanelRepresentation::GetCenterY() {
	return m_CenterY;
}

int GUIPanelRepresentation::GetCenterX() {
	return (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetCenterWidth()) >> 1;
}

int GUIPanelRepresentation::GetHackY() {
	return m_HackY;
}

void GUIPanelRepresentation::UpdateLayout() {
	m_CenterY = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - m_currentPanel->getDisplayMetrics()->m_dispPedalHeight;
	m_CenterWidth = std::max(GetJambTopWidth(), GetPistonWidth());

	unsigned nbrManuals = m_currentPanel->getNumberOfManuals();
	if (!nbrManuals)
		nbrManuals = 1;
	for (unsigned i = 0; i < nbrManuals; i++) {
		GUIManual *theManual = m_currentPanel->getGuiManualAt(i);
		if (!i && theManual && m_currentPanel->getHasPedals()) {
			// this is the first manual on the panel and it should be displayed as a pedal
			theManual->m_renderInfo.height = m_currentPanel->getDisplayMetrics()->m_dispPedalHeight;
			theManual->m_renderInfo.keys_y = theManual->m_renderInfo.y = m_CenterY;
			m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispPedalHeight;
			if (m_currentPanel->getDisplayMetrics()->m_dispExtraPedalButtonRow)
				m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
			theManual->m_renderInfo.piston_y = m_CenterY;
			m_CenterWidth = std::max(m_CenterWidth, (int)GetEnclosuresWidth());
			m_CenterY -= 12;
			m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispEnclosureHeight;
			m_EnclosureY = m_CenterY;
			m_CenterY -= 12;
		}
		if (!i && !theManual &&	m_currentPanel->getNumberOfEnclosures()) {
			// this is if there's no manuals on the panel but there's enclosures
			m_CenterY -= 12;
			m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispEnclosureHeight;
			m_EnclosureY = m_CenterY;
			m_CenterY -= 12;
		}

		if (!theManual)
			continue;

		if (i || !m_currentPanel->getHasPedals()) {
			if (!m_currentPanel->getDisplayMetrics()->m_dispButtonsAboveManuals) {
				m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
				theManual->m_renderInfo.piston_y = m_CenterY;
			}
			theManual->m_renderInfo.height = m_currentPanel->getDisplayMetrics()->m_dispManualHeight;
			if (m_currentPanel->getDisplayMetrics()->m_dispTrimBelowManuals && i == 1) {
				theManual->m_renderInfo.height += 8;
				m_CenterY -= 8;
			}
			m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispManualHeight;
			theManual->m_renderInfo.keys_y = m_CenterY;
			if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveManuals && i + 1 == m_currentPanel->getNumberOfManuals()) {
				m_CenterY -= 8;
				theManual->m_renderInfo.height += 8;
			}
			if (m_currentPanel->getDisplayMetrics()->m_dispButtonsAboveManuals) {
				m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
				theManual->m_renderInfo.piston_y = m_CenterY;
			}
			theManual->m_renderInfo.y = m_CenterY;
		}
		theManual->m_renderInfo.width = 0;
		if (i || !m_currentPanel->getHasPedals()) {
			// this calculates the total width of a manual keyboard by adding the natural widths
			int startingMidiNbr = theManual->getDisplayFirstNote();
			for (int j = 0; j < theManual->getNumberOfDisplayKeys(); j++) {
				int key_nbr = startingMidiNbr + j;
				if (((key_nbr % 12) < 5 && !(key_nbr & 1)) || ((key_nbr % 12) >= 5 && (key_nbr & 1))) {
					theManual->m_renderInfo.width += m_currentPanel->getDisplayMetrics()->m_dispManualKeyWidth;
				}
			}
		} else {
			// this calculates the total width of a pedal
			int startingMidiNbr = theManual->getDisplayFirstNote();
			for (int j = 0; j < theManual->getNumberOfDisplayKeys(); j++) {
				int key_nbr = startingMidiNbr + j;
				theManual->m_renderInfo.width += m_currentPanel->getDisplayMetrics()->m_dispPedalKeyWidth;
				if (j && (key_nbr % 12 == 4 || key_nbr % 12 == 11))
					theManual->m_renderInfo.width += m_currentPanel->getDisplayMetrics()->m_dispPedalKeyWidth;
			}
		}
		theManual->m_renderInfo.x = (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - theManual->m_renderInfo.width) >> 1;
		//theManual->m_renderInfo.width += 16;
		if ((int)theManual->m_renderInfo.width > m_CenterWidth)
			m_CenterWidth = theManual->m_renderInfo.width;
	}

	m_HackY = m_CenterY;

	if (m_CenterWidth + GetJambLeftRightWidth() * 2 < m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
		m_CenterWidth += (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - m_CenterWidth - GetJambLeftRightWidth() * 2) / 3;

	m_CenterY -= GetPistonTopHeight();
	m_CenterY -= GetJambTopHeight();
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows)
		m_CenterY -= 8;
}

wxString GUIPanelRepresentation::BreakTextLine(wxString text, int textBreakWidth, wxDC& dc) {
	wxString string = text;
	wxString str, line, work;
	wxCoord cx, cy;

	/* text.Length() + 1 iterations */
	for (unsigned i = 0; i <= string.Length(); i++) {
		bool maybreak = false;
		if (string[i] == wxT(' ') || string[i] == wxT('\n')) {
			if (work.length() < 2)
				maybreak = false;
			else
				maybreak = true;
		}
		if (maybreak || i == string.Length()) {
			if (!work.Length())
				continue;
			dc.GetTextExtent(line + wxT(' ') + work, &cx, &cy);
			if (cx > textBreakWidth) {
				if (!str.Length())
					str = line;
				else
					str = str + wxT('\n') + line;
				line = wxT("");
			}

			if (!line.Length())
				line = work;
			else
				line = line + wxT(' ') + work;

			work = wxT("");
		} else {
			if (string[i] == wxT(' ') || string[i] == wxT('\n')) {
				if (work.Length() && work[work.Length() - 1] != wxT(' '))
					work += wxT(' ');
			} else
				work += string[i];
		}
	}

	if (!str.Length())
		str = line;
	else
		str = str + wxT('\n') + line;
	return str;
}

void GUIPanelRepresentation::DoUpdateLayout() {
	SetClientSize(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	UpdateLayout();
	DoPaintNow();
}
