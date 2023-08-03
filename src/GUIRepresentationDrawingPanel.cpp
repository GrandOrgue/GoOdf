/*
 * GUIRepresentationDrawingPanel.cpp is part of GOODF.
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

#include "GUIRepresentationDrawingPanel.h"
#include "GUIButton.h"
#include "GUILabel.h"

// Event table
BEGIN_EVENT_TABLE(GUIRepresentationDrawingPanel, wxPanel)
	EVT_PAINT(GUIRepresentationDrawingPanel::OnPaintEvent)
	EVT_LEFT_DOWN(GUIRepresentationDrawingPanel::OnLeftClick)
	EVT_MOTION(GUIRepresentationDrawingPanel::OnMouseMotion)
	EVT_LEFT_UP(GUIRepresentationDrawingPanel::OnLeftRelease)
	EVT_KEY_DOWN(GUIRepresentationDrawingPanel::OnKeyboardInput)
	EVT_SIZE(GUIRepresentationDrawingPanel::OnPanelSize)
END_EVENT_TABLE()

GUIRepresentationDrawingPanel::GUIRepresentationDrawingPanel(wxWindow *parent) : wxPanel(parent) {
	m_currentPanel = NULL;
	m_isFirstRender = true;
	m_selectedObjectIndex = -1;
	m_isDraggingObject = false;
	m_currentDragX = -1;
	m_currentDragY = -1;
	m_startDragX = -1;
	m_startDragY = -1;
	m_HackY = 0;
	m_EnclosureY = 0;
	m_CenterY = 0;
	m_CenterWidth = 0;
	m_FontScale = 1.0;
	InitFont();
	SetFocus();
}

GUIRepresentationDrawingPanel::~GUIRepresentationDrawingPanel() {

}

void GUIRepresentationDrawingPanel::SetCurrentPanel(GoPanel *thePanel) {
	m_currentPanel = thePanel;
	m_selectedObjectIndex = -1;
	m_isFirstRender = true;
	if (!m_guiObjects.empty()) {
		m_guiObjects.clear();
	}
	SetClientSize(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());

	UpdateLayout();
	DoPaintNow();

	PostSizeEvent();
}

void GUIRepresentationDrawingPanel::OnPaintEvent(wxPaintEvent& WXUNUSED(event)) {
	wxPaintDC dc(this);
	RenderPanel(dc);
}

void GUIRepresentationDrawingPanel::OnLeftClick(wxMouseEvent& event) {
	if (!m_guiObjects.empty()) {
		wxCoord xPos = event.GetX();
		wxCoord yPos = event.GetY();
		m_startDragX = xPos;
		m_startDragY = yPos;
		m_selectedObjectIndex = -1;
		for (int i = (int) m_guiObjects.size() - 1; i >= 0; i--) {
			if (m_guiObjects[i].boundingRect.Contains(xPos, yPos)) {
				m_selectedObjectIndex = i;
				m_guiObjects[i].isSelected = true;
				for (unsigned j = 0; j < m_guiObjects.size(); j++){
					if (m_guiObjects[j].isSelected && j != (unsigned) i){
						m_guiObjects[j].isSelected = false;
					}
				}
				break;
			}
		}
		if (m_selectedObjectIndex < 0) {
			for (unsigned i = 0; i < m_guiObjects.size(); i++) {
				m_guiObjects[i].isSelected = false;
			}
		}
		DoPaintNow();
	} else {
		event.Skip();
	}
}

void GUIRepresentationDrawingPanel::OnMouseMotion(wxMouseEvent& event) {
	if (m_selectedObjectIndex >= 0 && event.Dragging()) {
		m_isDraggingObject = true;
		m_currentDragX = event.GetX();
		m_currentDragY = event.GetY();
		wxClientDC dc(this);
		wxDCOverlay overlaydc(m_overlay, &dc);
		overlaydc.Clear();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColour(*wxYELLOW), 1, wxPENSTYLE_DOT));
		int xPos = m_guiObjects[m_selectedObjectIndex].boundingRect.x + m_currentDragX - m_startDragX;
		int yPos = m_guiObjects[m_selectedObjectIndex].boundingRect.y + m_currentDragY - m_startDragY;
		wxRect tempOutline(
			xPos,
			yPos,
			m_guiObjects[m_selectedObjectIndex].boundingRect.width,
			m_guiObjects[m_selectedObjectIndex].boundingRect.height
		);
		dc.DrawRectangle(tempOutline);
		dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
		dc.SetTextForeground(*wxYELLOW);
		dc.DrawText(wxString::Format(wxT("(%i, %i)"), xPos, yPos), xPos + 1, yPos + 1);
	}
}

void GUIRepresentationDrawingPanel::OnLeftRelease(wxMouseEvent& event) {
	if (m_selectedObjectIndex >= 0 && m_isDraggingObject) {
		m_currentDragX = event.GetX();
		m_currentDragY = event.GetY();
		int finalXpos = m_guiObjects[m_selectedObjectIndex].boundingRect.x + m_currentDragX - m_startDragX;
		int finalYpos = m_guiObjects[m_selectedObjectIndex].boundingRect.y + m_currentDragY - m_startDragY;

		if (m_guiObjects[m_selectedObjectIndex].element) {
			m_guiObjects[m_selectedObjectIndex].element->setPosX(finalXpos);
			m_guiObjects[m_selectedObjectIndex].element->setPosY(finalYpos);
		} else if (m_guiObjects[m_selectedObjectIndex].img) {
			m_guiObjects[m_selectedObjectIndex].img->setPositionX(finalXpos);
			m_guiObjects[m_selectedObjectIndex].img->setPositionY(finalYpos);
		}
		m_guiObjects[m_selectedObjectIndex].boundingRect.x = finalXpos;
		m_guiObjects[m_selectedObjectIndex].boundingRect.y = finalYpos;
		m_guiObjects[m_selectedObjectIndex].isSelected = false;

		m_isDraggingObject = false;
		// m_selectedObjectIndex = -1;
		// reset all drag coordinates
		m_currentDragX = -1;
		m_currentDragY = -1;
		m_startDragX = -1;
		m_startDragY = -1;

		UpdateLayout();
		DoPaintNow();
	}
	event.Skip();
}

void GUIRepresentationDrawingPanel::OnKeyboardInput(wxKeyEvent& event) {
	if (m_selectedObjectIndex > -1) {
		int xPos = m_guiObjects[m_selectedObjectIndex].boundingRect.x;
		int yPos = m_guiObjects[m_selectedObjectIndex].boundingRect.y;

		if (event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION)) {
			switch (event.GetKeyCode()) {
				case WXK_LEFT:
					xPos -= 1;
					break;
				case WXK_UP:
					yPos -= 1;
					break;
				case WXK_RIGHT:
					xPos += 1;
					break;
				case WXK_DOWN:
					yPos += 1;
					break;
				default:
					break;
			}

			if (m_guiObjects[m_selectedObjectIndex].element) {
				m_guiObjects[m_selectedObjectIndex].element->setPosX(xPos);
				m_guiObjects[m_selectedObjectIndex].element->setPosY(yPos);
			} else if (m_guiObjects[m_selectedObjectIndex].img) {
				m_guiObjects[m_selectedObjectIndex].img->setPositionX(xPos);
				m_guiObjects[m_selectedObjectIndex].img->setPositionY(yPos);
			}
			m_guiObjects[m_selectedObjectIndex].boundingRect.x = xPos;
			m_guiObjects[m_selectedObjectIndex].boundingRect.y = yPos;
			UpdateLayout();
			DoPaintNow();

			return;
		} else {
			event.Skip();
		}
	}
	event.Skip();
}

void GUIRepresentationDrawingPanel::DoPaintNow() {
	if (this->IsShown()) {
		wxClientDC dc(this);
		RenderPanel(dc);
	}
}

void GUIRepresentationDrawingPanel::RenderPanel(wxDC& dc) {
	m_overlay.Reset();
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
			int imgX = m_currentPanel->getImageAt(i)->getPositionX();
			int imgY = m_currentPanel->getImageAt(i)->getPositionY();
			int imgWidth = m_currentPanel->getImageAt(i)->getWidth();
			int imgHeight = m_currentPanel->getImageAt(i)->getHeight();
			if (m_currentPanel->getImageAt(i)->getWidth() > m_currentPanel->getImageAt(i)->getOriginalWidth() || m_currentPanel->getImageAt(i)->getHeight() > m_currentPanel->getImageAt(i)->getOriginalHeight()) {
				wxRect imgRect(
					imgX,
					imgY,
					imgWidth,
					imgHeight
				);
				wxBitmap theBmp = m_currentPanel->getImageAt(i)->getBitmap();
				TileBitmap(imgRect, dc, theBmp, m_currentPanel->getImageAt(i)->getTileOffsetX(), m_currentPanel->getImageAt(i)->getTileOffsetY());
			} else {
				dc.DrawBitmap(m_currentPanel->getImageAt(i)->getBitmap(), imgX, imgY, true);
			}
			if (m_isFirstRender && imgWidth < m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() / 2 && imgHeight < m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() / 2) {
				GUI_OBJECT theImage;
				theImage.element = NULL;
				theImage.img = m_currentPanel->getImageAt(i);
				theImage.boundingRect = wxRect(imgX, imgY, imgWidth, imgHeight);
				theImage.isSelected = false;
				m_guiObjects.push_back(theImage);
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
		if (currentMan->getPosY() >= 0) {
			manYpos = currentMan->getPosY();
		} else if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveManuals && i + 1 == m_currentPanel->getNumberOfManuals()) {
			manYpos += 8;
		}
		for (int j = 0; j < currentMan->getNumberOfDisplayKeys(); j++) {
			KEY_INFO *currentKey = currentMan->getKeyInfoAt(j);
			wxBitmap theKey = currentKey->KeyImage;
			dc.DrawBitmap(theKey, manXpos + currentKey->Xpos, manYpos + currentKey->Ypos, true);
		}
		if (m_isFirstRender) {
			GUI_OBJECT theManual;
			theManual.element = currentMan;
			theManual.img = NULL;
			theManual.boundingRect = wxRect(manXpos, manYpos, currentMan->m_renderInfo.width, currentMan->m_renderInfo.height);
			theManual.isSelected = false;
			m_guiObjects.push_back(theManual);
		}
	}

	if (m_currentPanel->getNumberOfGuiElements() > 0) {
		for (unsigned i = 0; i < (unsigned) m_currentPanel->getNumberOfGuiElements(); i++) {
			GUIElement *guiElement = m_currentPanel->getGuiElementAt(i);
			wxBitmap theBmp = guiElement->getBitmap();
			// Test if it's a button type
			GUIButton *btnElement = dynamic_cast<GUIButton*>(guiElement);
			if (btnElement) {
				wxPoint thePos;
				if (btnElement->isDisplayAsPiston()) {
					thePos = GetPushbuttonPosition(btnElement->getDispButtonRow(), btnElement->getDispButtonCol());
				} else {
					thePos = GetDrawstopPosition(btnElement->getDispDrawstopRow(), btnElement->getDispDrawstopCol());
				}
				if (btnElement->getPosX() != -1) {
					thePos.x = btnElement->getPosX();
				}
				if (btnElement->getPosY() != -1) {
					thePos.y = btnElement->getPosY();
				}

				if (theBmp.IsOk()) {
					wxRect imgRect(thePos.x, thePos.y, btnElement->getWidth(), btnElement->getHeight());
					TileBitmap(imgRect, dc, theBmp, btnElement->getTileOffsetX(), btnElement->getTileOffsetY());
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
				if (m_isFirstRender) {
					GUI_OBJECT theButton;
					theButton.element = btnElement;
					theButton.img = NULL;
					theButton.boundingRect = wxRect(thePos.x, thePos.y, btnElement->getWidth(), btnElement->getHeight());
					theButton.isSelected = false;
					m_guiObjects.push_back(theButton);
				}
				continue;
			}
			GUIEnclosure *encElement = dynamic_cast<GUIEnclosure*>(guiElement);
			if (encElement) {
				wxPoint thePos(0, 0);

				int enclosureNumberOnPanel = -1;
				for (unsigned j = 0; j < m_currentPanel->getNumberOfEnclosures(); j++) {
					if (encElement == m_currentPanel->getGuiEnclosureAt(j)) {
						enclosureNumberOnPanel = j;
						break;
					}
				}
				if (enclosureNumberOnPanel != -1) {
					enclosureNumberOnPanel += 1;
					thePos.x = GetEnclosureX(enclosureNumberOnPanel);
				}
				thePos.y = GetEnclosureY();

				if (encElement->getPosX() != -1) {
					thePos.x = encElement->getPosX();
				}
				if (encElement->getPosY() != -1) {
					thePos.y = encElement->getPosY();
				}

				wxRect imgRect(thePos.x, thePos.y, encElement->getWidth(), encElement->getHeight());
				TileBitmap(imgRect, dc, theBmp, encElement->getTileOffsetX(), encElement->getTileOffsetY());

				if (encElement->getTextBreakWidth()) {
					wxFont theFont = encElement->getDispLabelFont();
					int pointSize = theFont.GetPointSize();
					pointSize *= m_FontScale;
					theFont.SetPointSize(pointSize);
					dc.SetFont(theFont);
					dc.SetBackgroundMode(wxTRANSPARENT);
					dc.SetTextForeground(encElement->getDispLabelColour()->getColor());
					wxRect textRect(
						thePos.x + encElement->getTextRectLeft(),
						thePos.y + encElement->getTextRectTop(),
						encElement->getTextRectWidth(),
						encElement->getTextRectHeight()
					);
					dc.DrawLabel(BreakTextLine(encElement->getDisplayName(), encElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_HORIZONTAL);
				}
				if (m_isFirstRender) {
					GUI_OBJECT theEnclosure;
					theEnclosure.element = encElement;
					theEnclosure.img = NULL;
					theEnclosure.boundingRect = wxRect(thePos.x, thePos.y, encElement->getWidth(), encElement->getHeight());
					theEnclosure.isSelected = false;
					m_guiObjects.push_back(theEnclosure);
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
					wxRect imgRect(xPosToUse, yPosToUse, labelElement->getWidth(), labelElement->getHeight());
					TileBitmap(imgRect, dc, theBmp, labelElement->getTileOffsetX(), labelElement->getTileOffsetY());
				}

				if (labelElement->getTextBreakWidth() && labelElement->getName() != wxEmptyString) {
					wxFont theFont = labelElement->getDispLabelFont();
					int pointSize = theFont.GetPointSize();
					pointSize *= m_FontScale;
					theFont.SetPointSize(pointSize);
					dc.SetFont(theFont);
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
				if (m_isFirstRender) {
					GUI_OBJECT theLabel;
					theLabel.element = labelElement;
					theLabel.img = NULL;
					theLabel.boundingRect = wxRect(xPosToUse, yPosToUse, labelElement->getWidth(), labelElement->getHeight());
					theLabel.isSelected = false;
					m_guiObjects.push_back(theLabel);
				}
			}
		}
	}

	// Draw any selection that exist
	if (m_selectedObjectIndex >= 0) {
		wxDCOverlay overlaydc(m_overlay, &dc);
		overlaydc.Clear();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColour(*wxYELLOW), 1, wxPENSTYLE_DOT));
		dc.DrawRectangle(m_guiObjects[m_selectedObjectIndex].boundingRect);
	}
	m_isFirstRender = false;
}

void GUIRepresentationDrawingPanel::TileBitmap(wxRect rect, wxDC& dc, wxBitmap& bitmap, int tileOffsetX, int tileOffsetY) {
	int w = bitmap.GetWidth();
	int h = bitmap.GetHeight();

	wxImage wholeImg(rect.width, rect.height);
	wxImage bmp = bitmap.ConvertToImage();
	for (int i = -tileOffsetX; i < rect.width; i += w) {
		for (int j = -tileOffsetY; j < rect.height; j += h) {
			wholeImg.Paste(bmp, i, j);
		}
	}
	if (!wholeImg.IsOk())
		return;

	wxBitmap fullBmp = wxBitmap(wholeImg);

	dc.DrawBitmap(fullBmp, rect.x, rect.y, true);
}

wxPoint GUIRepresentationDrawingPanel::GetDrawstopPosition(int row, int col) {
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

wxPoint GUIRepresentationDrawingPanel::GetPushbuttonPosition(int row, int col) {
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

		if (i > (int) m_currentPanel->getNumberOfManuals())
			position.y = GetHackY() - (i + 1 - (int) m_currentPanel->getNumberOfManuals()) * (m_currentPanel->getDisplayMetrics()->m_dispManualHeight + m_currentPanel->getDisplayMetrics()->m_dispPistonHeight) + m_currentPanel->getDisplayMetrics()->m_dispManualHeight + 5;
		else {
			if (!m_currentPanel->getHasPedals() && i > 0)
				position.y = m_currentPanel->getGuiManualAt(i - 1)->m_renderInfo.piston_y + 5;
			else
				position.y = m_currentPanel->getGuiManualAt(i)->m_renderInfo.piston_y + 5;
		}

		if (m_currentPanel->getDisplayMetrics()->m_dispExtraPedalButtonRow && !row)
			position.y += m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
		if (m_currentPanel->getDisplayMetrics()->m_dispExtraPedalButtonRowOffset && row == 99)
			position.x -= m_currentPanel->getDisplayMetrics()->m_dispPistonWidth / 2 + 2;
	}
	return position;
}

unsigned GUIRepresentationDrawingPanel::GetEnclosuresWidth() {
	return m_currentPanel->getDisplayMetrics()->m_dispEnclosureWidth * m_currentPanel->getNumberOfEnclosures();
}

int GUIRepresentationDrawingPanel::GetEnclosureY() {
	return m_EnclosureY;
}

int GUIRepresentationDrawingPanel::GetEnclosureX(int enclosureNbr) {
	int enclosure_x = (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetEnclosuresWidth() + 6) >> 1;
	enclosure_x += m_currentPanel->getDisplayMetrics()->m_dispEnclosureWidth * (enclosureNbr - 1);

	return enclosure_x;
}

int GUIRepresentationDrawingPanel::GetJambLeftRightWidth() {
	int jamblrw = m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth / 2;
	if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols)
		jamblrw += ((m_currentPanel->getDisplayMetrics()->m_dispDrawstopCols >> 2) * (m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth / 4)) - 8;
	return jamblrw;
}

unsigned GUIRepresentationDrawingPanel::GetJambLeftRightHeight() {
	return (m_currentPanel->getDisplayMetrics()->m_dispDrawstopRows + 1) * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight;
}

int GUIRepresentationDrawingPanel::GetJambLeftRightY() {
	return ((int)(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - GetJambLeftRightHeight() - (m_currentPanel->getDisplayMetrics()->m_dispDrawstopColsOffset ? (m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight / 2) : 0))) / 2;
}

int GUIRepresentationDrawingPanel::GetJambLeftX() {
	int jamblx = (GetCenterX() - GetJambLeftRightWidth()) >> 1;
	if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols)
		jamblx += 5;
	return jamblx;
}

int GUIRepresentationDrawingPanel::GetJambRightX() {
	int jambrx = GetJambLeftX() + GetCenterX() + GetCenterWidth();
	if (m_currentPanel->getDisplayMetrics()->m_dispPairDrawstopCols)
		jambrx += 5;
	return jambrx;
}

int GUIRepresentationDrawingPanel::GetJambTopDrawstop() {
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows)
		return GetCenterY() + 8;
	return GetCenterY();
}

int GUIRepresentationDrawingPanel::GetJambTopPiston() {
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows)
		return GetCenterY() + 8;
	return GetCenterY();
}

unsigned GUIRepresentationDrawingPanel::GetJambTopHeight() {
	return m_currentPanel->getDisplayMetrics()->m_dispExtraDrawstopRows * m_currentPanel->getDisplayMetrics()->m_dispDrawstopHeight;
}

unsigned GUIRepresentationDrawingPanel::GetJambTopWidth() {
	return m_currentPanel->getDisplayMetrics()->m_dispExtraDrawstopCols * m_currentPanel->getDisplayMetrics()->m_dispDrawstopWidth;
}

int GUIRepresentationDrawingPanel::GetJambTopX() {
	return (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetJambTopWidth()) >> 1;
}

int GUIRepresentationDrawingPanel::GetJambTopY() {
	if (m_currentPanel->getDisplayMetrics()->m_dispTrimAboveExtraRows)
		return GetCenterY() + 8;
	return GetCenterY();
}

unsigned GUIRepresentationDrawingPanel::GetPistonTopHeight() {
	return m_currentPanel->getDisplayMetrics()->m_dispExtraButtonRows * m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
}

unsigned GUIRepresentationDrawingPanel::GetPistonWidth() {
	return m_currentPanel->getDisplayMetrics()->m_dispButtonCols * m_currentPanel->getDisplayMetrics()->m_dispPistonWidth;
}

int GUIRepresentationDrawingPanel::GetPistonX() {
	return (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetPistonWidth()) >> 1;
}

int GUIRepresentationDrawingPanel::GetCenterWidth() {
	return m_CenterWidth;
}

int GUIRepresentationDrawingPanel::GetCenterY() {
	return m_CenterY;
}

int GUIRepresentationDrawingPanel::GetCenterX() {
	return (m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() - GetCenterWidth()) >> 1;
}

int GUIRepresentationDrawingPanel::GetHackY() {
	return m_HackY;
}

void GUIRepresentationDrawingPanel::UpdateLayout() {
	m_CenterY = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() - m_currentPanel->getDisplayMetrics()->m_dispPedalHeight;
	m_CenterWidth = std::max(GetJambTopWidth(), GetPistonWidth());

	unsigned nbrManuals = m_currentPanel->getNumberOfManuals();
	if (!nbrManuals)
		nbrManuals = 1;
	for (unsigned i = 0; i < nbrManuals; i++) {
		GUIManual *theManual = m_currentPanel->getGuiManualAt(i);
		if (!i && theManual && m_currentPanel->getHasPedals()) {
			// this is the first manual (and a real one) on the panel and it should be a pedal
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
		if (!i && !m_currentPanel->getHasPedals() && m_currentPanel->getNumberOfEnclosures()) {
			// this is if there's no pedal on the panel but there are enclosures
			m_CenterY -= 12;
			m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispEnclosureHeight;
			m_EnclosureY = m_CenterY;
			m_CenterY -= 12;
		}

		if (!theManual)
			continue;

		if (i || (i == 0 && !m_currentPanel->getHasPedals())) {
			if (!m_currentPanel->getDisplayMetrics()->m_dispButtonsAboveManuals) {
				m_CenterY -= m_currentPanel->getDisplayMetrics()->m_dispPistonHeight;
				theManual->m_renderInfo.piston_y = m_CenterY;
			}
			theManual->m_renderInfo.height = m_currentPanel->getDisplayMetrics()->m_dispManualHeight;
			if (m_currentPanel->getDisplayMetrics()->m_dispTrimBelowManuals && (i == 1 || (i == 0 && !m_currentPanel->getHasPedals()))) {
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

		theManual->m_renderInfo.width = 1;
		if (i || (i == 0 && !m_currentPanel->getHasPedals())) {
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

wxString GUIRepresentationDrawingPanel::BreakTextLine(wxString text, int textBreakWidth, wxDC& dc) {
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

void GUIRepresentationDrawingPanel::DoUpdateLayout() {
	m_selectedObjectIndex = -1;
	m_isFirstRender = true;
	if (!m_guiObjects.empty()) {
		m_guiObjects.clear();
	}
	SetClientSize(m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue(), m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue());
	UpdateLayout();
	DoPaintNow();
}

void GUIRepresentationDrawingPanel::InitFont() {
	wxMemoryDC dc;
	wxFont font = *wxNORMAL_FONT;
	wxCoord cx, cy;
	font.SetPointSize(39);
	dc.SetFont(font);
	dc.GetTextExtent(wxT("M"), &cx, &cy);
	m_FontScale = 62.0 / cy;
}

void  GUIRepresentationDrawingPanel::OnPanelSize(wxSizeEvent& WXUNUSED(event)) {
	GetParent()->SetClientSize(GetClientSize());
}
