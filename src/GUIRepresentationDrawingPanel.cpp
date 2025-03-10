/*
 * GUIRepresentationDrawingPanel.cpp is part of GoOdf.
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

#include "GUIRepresentationDrawingPanel.h"
#include "GUIButton.h"
#include "GUILabel.h"
#include "GOODF.h"

// Event table
BEGIN_EVENT_TABLE(GUIRepresentationDrawingPanel, wxPanel)
	EVT_PAINT(GUIRepresentationDrawingPanel::OnPaintEvent)
	EVT_LEFT_DOWN(GUIRepresentationDrawingPanel::OnLeftClick)
	EVT_MOTION(GUIRepresentationDrawingPanel::OnMouseMotion)
	EVT_LEFT_UP(GUIRepresentationDrawingPanel::OnLeftRelease)
	EVT_RIGHT_DOWN(GUIRepresentationDrawingPanel::OnRightDown)
	EVT_KEY_DOWN(GUIRepresentationDrawingPanel::OnKeyboardInput)
	EVT_CHAR(GUIRepresentationDrawingPanel::OnKeyboardInput)
	EVT_KEY_UP(GUIRepresentationDrawingPanel::OnKeyRelease)
	EVT_SIZE(GUIRepresentationDrawingPanel::OnPanelSize)
END_EVENT_TABLE()

GUIRepresentationDrawingPanel::GUIRepresentationDrawingPanel(wxWindow *parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS) {
	m_currentPanel = NULL;
	m_isFirstRender = true;
	m_selectedObjectIndex = -1;
	m_isDraggingObject = false;
	m_isSelecting = false;
	m_hasSelection = false;
	NotChangingTheSelection();
	m_currentDragX = -1;
	m_currentDragY = -1;
	m_startDragX = -1;
	m_startDragY = -1;
	m_selectionChangingWidth = 0;
	m_selectionChangingHeight = 0;
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
			if ((m_guiObjects[i].boundingRect.Contains(xPos, yPos) && !m_hasSelection) ||
				(m_guiObjects[i].boundingRect.Contains(xPos, yPos) && !m_selectionRect.Contains(xPos, yPos))
			) {
				m_selectedObjectIndex = i;
				m_guiObjects[i].isSelected = true;
				break;
			}
		}
		if (m_selectedObjectIndex < 0) {
			for (unsigned i = 0; i < m_guiObjects.size(); i++) {
				m_guiObjects[i].isSelected = false;
			}

			// a selection box/rectangle could be allowed so a click outside an existing should remove selection
			// and prepare for starting a new selection if none already exists
			if (m_hasSelection) {
				if(!m_selectionRect.Contains(xPos, yPos)) {
					m_hasSelection = false;
					m_selectionRect.SetSize(wxDefaultSize);
					m_selectionRect.SetPosition(wxDefaultPosition);
					NotChangingTheSelection();
				} else {
					if (m_selectionRectLeftUpCorner.Contains(xPos, yPos)) {
						m_changingLeftUpCorner = true;
					} else if (m_selectionRectLeftDownCorner.Contains(xPos, yPos)) {
						m_changingLeftDownCorner = true;
					} else if (m_selectionRectRightUpCorner.Contains(xPos, yPos)) {
						m_changingRightUpCorner = true;
					} else if (m_selectionRectRightDownCorner.Contains(xPos, yPos)) {
						m_changingRightDownCorner = true;
					} else {
						NotChangingTheSelection();
					}
				}
			} else {
				NotChangingTheSelection();
			}
		} else {
			m_hasSelection = false;
			m_selectionRect.SetSize(wxDefaultSize);
			m_selectionRect.SetPosition(wxDefaultPosition);
			NotChangingTheSelection();
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
		int xOffset = m_currentDragX - m_startDragX;
		int yOffset = m_currentDragY - m_startDragY;

		for (unsigned i = 0; i < m_guiObjects.size(); i++) {
			if (m_guiObjects[i].isSelected) {
				int xPos = m_guiObjects[i].boundingRect.x + xOffset;
				int yPos = m_guiObjects[i].boundingRect.y + yOffset;
				wxRect tempOutline(
					xPos,
					yPos,
					m_guiObjects[i].boundingRect.width,
					m_guiObjects[i].boundingRect.height
				);
				dc.DrawRectangle(tempOutline);
				dc.SetFont(wxFont(8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
				dc.SetTextForeground(*wxYELLOW);
				dc.DrawText(wxString::Format(wxT("(%i, %i)"), xPos, yPos), xPos + 1, yPos + 1);
			}
		}
	} else if (m_selectedObjectIndex < 0 && event.Dragging() && !IsSelectionChanging()) {
		// this means we're creating a selection
		if (!m_isSelecting) {
			// the first time we set an origin
			m_selectionRect.SetPosition(wxPoint(m_startDragX, m_startDragY));
		}
		m_isSelecting = true;
		m_currentDragX = event.GetX();
		m_currentDragY = event.GetY();

		if (m_currentDragX < 0)
			m_currentDragX = 0;
		if (m_currentDragX > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
			m_currentDragX = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
		if (m_currentDragY < 0)
			m_currentDragY = 0;
		if (m_currentDragY > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue())
			m_currentDragY = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();

		wxClientDC dc(this);
		wxDCOverlay overlaydc(m_overlay, &dc);
		overlaydc.Clear();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColour(*wxRED), 1, wxPENSTYLE_DOT_DASH));
		int xOffset = m_currentDragX - m_startDragX;
		int yOffset = m_currentDragY - m_startDragY;
		m_selectionRect.SetSize(wxSize(xOffset, yOffset));
		dc.DrawRectangle(m_selectionRect);
	} else if (m_hasSelection && !event.Dragging()) {
		wxCoord currentX = event.GetX();
		wxCoord currentY = event.GetY();
		if (m_selectionRect.Contains(currentX, currentY)) {
			wxClientDC dc(this);
			wxDCOverlay overlaydc(m_overlay, &dc);
			overlaydc.Clear();
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			if (m_selectionRectLeftUpCorner.Contains(currentX, currentY)) {
				dc.SetPen(wxPen(wxColour(*wxYELLOW), 1, wxPENSTYLE_SOLID));
				dc.DrawRectangle(m_selectionRectLeftUpCorner);
			} else if (m_selectionRectLeftDownCorner.Contains(currentX, currentY)) {
				dc.SetPen(wxPen(wxColour(*wxYELLOW), 1, wxPENSTYLE_SOLID));
				dc.DrawRectangle(m_selectionRectLeftDownCorner);
			} else if (m_selectionRectRightUpCorner.Contains(currentX, currentY)) {
				dc.SetPen(wxPen(wxColour(*wxYELLOW), 1, wxPENSTYLE_SOLID));
				dc.DrawRectangle(m_selectionRectRightUpCorner);
			} else if (m_selectionRectRightDownCorner.Contains(currentX, currentY)) {
				dc.SetPen(wxPen(wxColour(*wxYELLOW), 1, wxPENSTYLE_SOLID));
				dc.DrawRectangle(m_selectionRectRightDownCorner);
			} else {
				// all corners for adjusting selection rectangle
				dc.SetPen(wxPen(wxColour(*wxGREEN), 1, wxPENSTYLE_SOLID));
				dc.DrawLine(m_selectionRectLeftUpCorner.GetBottomLeft(), m_selectionRectLeftUpCorner.GetBottomRight());
				dc.DrawLine(m_selectionRectLeftUpCorner.GetBottomRight(), m_selectionRectLeftUpCorner.GetTopRight());

				dc.DrawLine(m_selectionRectRightUpCorner.GetTopLeft(), m_selectionRectRightUpCorner.GetBottomLeft());
				dc.DrawLine(m_selectionRectRightUpCorner.GetBottomLeft(), m_selectionRectRightUpCorner.GetBottomRight());

				dc.DrawLine(m_selectionRectRightDownCorner.GetBottomLeft(), m_selectionRectRightDownCorner.GetTopLeft());
				dc.DrawLine(m_selectionRectRightDownCorner.GetTopLeft(), m_selectionRectRightDownCorner.GetTopRight());

				dc.DrawLine(m_selectionRectLeftDownCorner.GetTopLeft(), m_selectionRectLeftDownCorner.GetTopRight());
				dc.DrawLine(m_selectionRectLeftDownCorner.GetTopRight(), m_selectionRectLeftDownCorner.GetBottomRight());
			}
			dc.SetPen(wxPen(wxColour(*wxGREEN), 1, wxPENSTYLE_DOT));
			dc.DrawRectangle(m_selectionRect);
		} else {
			Refresh();
		}
	} else if (m_hasSelection && event.Dragging() && IsSelectionChanging()) {
		// the current (already existing) selection is changing
		if (!m_isSelecting) {
			// the first time we set an origin which should be the diagonal corner
			// also the width and height needs to be adjusted accordingly
			if (m_changingLeftUpCorner) {
				m_selectionRect.SetPosition(m_selectionRectRightDownCorner.GetBottomRight());
				m_selectionChangingWidth = -m_selectionRect.GetWidth();
				m_selectionChangingHeight = -m_selectionRect.GetHeight();
			} else if (m_changingLeftDownCorner) {
				m_selectionRect.SetPosition(m_selectionRectRightUpCorner.GetTopRight());
				m_selectionChangingWidth = -m_selectionRect.GetWidth();
				m_selectionChangingHeight = m_selectionRect.GetHeight();
			} else if (m_changingRightUpCorner) {
				m_selectionRect.SetPosition(m_selectionRectLeftDownCorner.GetBottomLeft());
				m_selectionChangingWidth = m_selectionRect.GetWidth();
				m_selectionChangingHeight = -m_selectionRect.GetHeight();
			} else if (m_changingRightDownCorner) {
				m_selectionRect.SetPosition(m_selectionRectLeftUpCorner.GetTopLeft());
				m_selectionChangingWidth = m_selectionRect.GetWidth();
				m_selectionChangingHeight = m_selectionRect.GetHeight();
			}
		}
		m_isSelecting = true;
		m_currentDragX = event.GetX();
		m_currentDragY = event.GetY();

		if (m_selectionRect.GetPosition().x + m_selectionChangingWidth + m_currentDragX - m_startDragX < 0)
			m_currentDragX = m_startDragX - m_selectionRect.GetPosition().x - m_selectionChangingWidth;
		if (m_selectionRect.GetPosition().x + m_selectionChangingWidth + m_currentDragX - m_startDragX > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
			m_currentDragX = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() + m_startDragX - m_selectionRect.GetPosition().x - m_selectionChangingWidth;
		if (m_selectionRect.GetPosition().y + m_selectionChangingHeight + m_currentDragY - m_startDragY < 0)
			m_currentDragY = m_startDragY - m_selectionRect.GetPosition().y - m_selectionChangingHeight;
		if (m_selectionRect.GetPosition().y + m_selectionChangingHeight + m_currentDragY - m_startDragY > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue())
			m_currentDragY = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() + m_startDragY - m_selectionRect.GetPosition().y - m_selectionChangingHeight;

		wxClientDC dc(this);
		wxDCOverlay overlaydc(m_overlay, &dc);
		overlaydc.Clear();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColour(*wxRED), 1, wxPENSTYLE_DOT_DASH));
		int xOffset = m_selectionChangingWidth + m_currentDragX - m_startDragX;
		int yOffset = m_selectionChangingHeight + m_currentDragY - m_startDragY;
		m_selectionRect.SetSize(wxSize(xOffset, yOffset));
		dc.DrawRectangle(m_selectionRect);
	}
	event.Skip();
}

void GUIRepresentationDrawingPanel::OnLeftRelease(wxMouseEvent& event) {
	if (m_selectedObjectIndex >= 0 && m_isDraggingObject) {
		m_currentDragX = event.GetX();
		m_currentDragY = event.GetY();
		int finalXoffset = m_currentDragX - m_startDragX;
		int finalYoffset = m_currentDragY - m_startDragY;

		for (unsigned i = 0; i < m_guiObjects.size(); i++) {
			if (m_guiObjects[i].isSelected) {
				int finalXpos = m_guiObjects[i].boundingRect.x + finalXoffset;
				int finalYpos = m_guiObjects[i].boundingRect.y + finalYoffset;

				if (m_guiObjects[i].element) {
					m_guiObjects[i].element->setPosX(finalXpos);
					m_guiObjects[i].element->setPosY(finalYpos);
					if (m_guiObjects[i].element->getType().IsSameAs(wxT("Label"))) {
						GUILabel *labelElement = dynamic_cast<GUILabel*>(m_guiObjects[i].element);
						if (labelElement) {
							if (!labelElement->isFreeXPlacement())
								labelElement->setFreeXPlacement(true);
							if (!labelElement->isFreeYPlacement())
								labelElement->setFreeYPlacement(true);
						}
					}
				} else if (m_guiObjects[m_selectedObjectIndex].img) {
					m_guiObjects[i].img->setPositionX(finalXpos);
					m_guiObjects[i].img->setPositionY(finalYpos);
				}
				m_guiObjects[i].boundingRect.x = finalXpos;
				m_guiObjects[i].boundingRect.y = finalYpos;
			}
		}

		m_isDraggingObject = false;
		// reset all drag coordinates
		m_currentDragX = -1;
		m_currentDragY = -1;
		m_startDragX = -1;
		m_startDragY = -1;

		::wxGetApp().m_frame->GUIElementPositionIsChanged();
		DoPaintNow();
	} else if (m_selectedObjectIndex < 0 && m_isSelecting && !IsSelectionChanging()) {
		m_isSelecting = false;
		m_hasSelection = true;
		m_currentDragX = event.GetX();
		m_currentDragY = event.GetY();

		// limit selection position within the panel
		if (m_currentDragX < 0)
			m_currentDragX = 0;
		if (m_currentDragX > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
			m_currentDragX = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
		if (m_currentDragY < 0)
			m_currentDragY = 0;
		if (m_currentDragY > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue())
			m_currentDragY = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();

		// calculate and set position and size for the whole selection rectrangle
		int width;
		int height;
		int xCoord;
		int yCoord;
		if (m_currentDragX < m_startDragX) {
			width = m_startDragX - m_currentDragX;
			xCoord = m_currentDragX;
		} else {
			width = m_currentDragX - m_startDragX;
			xCoord = m_startDragX;
		}
		if (m_currentDragY < m_startDragY) {
			height = m_startDragY - m_currentDragY;
			yCoord = m_currentDragY;
		} else {
			height = m_currentDragY - m_startDragY;
			yCoord = m_startDragY;
		}
		m_selectionRect.SetPosition(wxPoint(xCoord, yCoord));
		m_selectionRect.SetSize(wxSize(width, height));

		// reset all drag coordinates
		m_currentDragX = -1;
		m_currentDragY = -1;
		m_startDragX = -1;
		m_startDragY = -1;

		// calculate all active mouse rect sections for adjusting the selection rectangle
		wxPoint leftUp = m_selectionRect.GetLeftTop();
		wxPoint rightUp = m_selectionRect.GetTopRight();
		wxPoint rightDown = m_selectionRect.GetBottomRight();
		wxPoint leftDown = m_selectionRect.GetBottomLeft();
		int rectWidth = m_selectionRect.GetWidth();
		int rectHeight = m_selectionRect.GetHeight();
		wxSize selectionRectSize(rectWidth / 4, rectHeight / 4);
		m_selectionRectLeftUpCorner.SetPosition(leftUp);
		m_selectionRectLeftUpCorner.SetSize(selectionRectSize);
		m_selectionRectLeftDownCorner.SetPosition(wxPoint(leftDown.x, leftDown.y - selectionRectSize.GetHeight() + 1));
		m_selectionRectLeftDownCorner.SetSize(selectionRectSize);
		m_selectionRectRightUpCorner.SetPosition(wxPoint(rightUp.x - selectionRectSize.GetWidth() + 1, rightUp.y));
		m_selectionRectRightUpCorner.SetSize(selectionRectSize);
		m_selectionRectRightDownCorner.SetPosition(wxPoint(rightDown.x - selectionRectSize.GetWidth() + 1, rightDown.y - selectionRectSize.GetHeight() + 1));
		m_selectionRectRightDownCorner.SetSize(selectionRectSize);

		DoPaintNow();
	} else if (m_hasSelection && m_isSelecting && IsSelectionChanging()) {
		m_isSelecting = false;
		m_hasSelection = true;
		m_currentDragX = event.GetX();
		m_currentDragY = event.GetY();


		if (m_selectionRect.GetPosition().x + m_selectionChangingWidth + m_currentDragX - m_startDragX < 0)
			m_currentDragX = m_startDragX - m_selectionRect.GetPosition().x - m_selectionChangingWidth;
		if (m_selectionRect.GetPosition().x + m_selectionChangingWidth + m_currentDragX - m_startDragX > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
			m_currentDragX = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue() + m_startDragX - m_selectionRect.GetPosition().x - m_selectionChangingWidth;
		if (m_selectionRect.GetPosition().y + m_selectionChangingHeight + m_currentDragY - m_startDragY < 0)
			m_currentDragY = m_startDragY - m_selectionRect.GetPosition().y - m_selectionChangingHeight;
		if (m_selectionRect.GetPosition().y + m_selectionChangingHeight + m_currentDragY - m_startDragY > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue())
			m_currentDragY = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue() + m_startDragY - m_selectionRect.GetPosition().y - m_selectionChangingHeight;

		int finalWidth = m_selectionChangingWidth + m_currentDragX - m_startDragX;
		int finalHeight = m_selectionChangingHeight + m_currentDragY - m_startDragY;
		m_selectionRect.SetSize(wxSize(finalWidth, finalHeight));

		// calculate and set position and size for the whole selection rectrangle
		int width;
		int height;
		int xCoord;
		int yCoord;
		if (m_currentDragX < m_selectionRect.GetPosition().x) {
			width = (m_selectionRect.GetWidth() * -1) + 1;
			xCoord = m_selectionRect.GetPosition().x + m_selectionRect.GetWidth();
		} else {
			width = m_selectionRect.GetWidth() + 1;
			xCoord = m_selectionRect.GetPosition().x;
		}
		if (m_currentDragY < m_selectionRect.GetPosition().y) {
			height = (m_selectionRect.GetHeight() * -1) + 1;
			yCoord = m_selectionRect.GetPosition().y + m_selectionRect.GetHeight();
		} else {
			height = m_selectionRect.GetHeight() + 1;
			yCoord = m_selectionRect.GetPosition().y;
		}
		if (width > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue())
			width = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeHoriz.getNumericalValue();
		if (height > m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue())
			height = m_currentPanel->getDisplayMetrics()->m_dispScreenSizeVert.getNumericalValue();
		m_selectionRect.SetPosition(wxPoint(xCoord, yCoord));
		m_selectionRect.SetSize(wxSize(width, height));

		// reset all drag coordinates
		m_currentDragX = -1;
		m_currentDragY = -1;
		m_startDragX = -1;
		m_startDragY = -1;
		NotChangingTheSelection();

		// calculate all active mouse rect sections for adjusting the selection rectangle
		wxPoint leftUp = m_selectionRect.GetLeftTop();
		wxPoint rightUp = m_selectionRect.GetTopRight();
		wxPoint rightDown = m_selectionRect.GetBottomRight();
		wxPoint leftDown = m_selectionRect.GetBottomLeft();
		int rectWidth = m_selectionRect.GetWidth();
		int rectHeight = m_selectionRect.GetHeight();
		wxSize selectionRectSize(rectWidth / 4, rectHeight / 4);
		m_selectionRectLeftUpCorner.SetPosition(leftUp);
		m_selectionRectLeftUpCorner.SetSize(selectionRectSize);
		m_selectionRectLeftDownCorner.SetPosition(wxPoint(leftDown.x, leftDown.y - selectionRectSize.GetHeight() + 1));
		m_selectionRectLeftDownCorner.SetSize(selectionRectSize);
		m_selectionRectRightUpCorner.SetPosition(wxPoint(rightUp.x - selectionRectSize.GetWidth() + 1, rightUp.y));
		m_selectionRectRightUpCorner.SetSize(selectionRectSize);
		m_selectionRectRightDownCorner.SetPosition(wxPoint(rightDown.x - selectionRectSize.GetWidth() + 1, rightDown.y - selectionRectSize.GetHeight() + 1));
		m_selectionRectRightDownCorner.SetSize(selectionRectSize);

		DoPaintNow();
	}
	event.Skip();
}

void GUIRepresentationDrawingPanel::OnRightDown(wxMouseEvent& event) {
	if (m_hasSelection) {
		wxCoord xPos = event.GetX();
		wxCoord yPos = event.GetY();
		if (m_selectionRect.Contains(xPos, yPos)) {
			wxMenu selectionMenu;
			selectionMenu.Append(ID_CONVERT_SELECTION_CONTAINING, "Select contained elements");
			selectionMenu.Append(ID_CONVERT_SELECTION_INTERSECTING, "Select intersecting elements");
			selectionMenu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GUIRepresentationDrawingPanel::OnPopupMenuClick), NULL, this);
			PopupMenu(&selectionMenu);
		} else {
			event.Skip();
		}
	} else {
		event.Skip();
	}
}

void GUIRepresentationDrawingPanel::OnPopupMenuClick(wxCommandEvent& event) {
	switch(event.GetId()) {
		case ID_CONVERT_SELECTION_CONTAINING:
			SelectContainedElements();
			break;
		case ID_CONVERT_SELECTION_INTERSECTING:
			SelectIntersectingElements();
			break;
	}
}

void GUIRepresentationDrawingPanel::SelectContainedElements() {
	if (m_hasSelection) {
		for (int i = (int) m_guiObjects.size() - 1; i >= 0; i--) {
			if (m_selectionRect.Contains(m_guiObjects[i].boundingRect)) {
				m_selectedObjectIndex = i;
				m_guiObjects[i].isSelected = true;
				m_hasSelection = false;
			}
		}
		Refresh();
	}
}

void GUIRepresentationDrawingPanel::SelectIntersectingElements() {
	if (m_hasSelection) {
		for (int i = (int) m_guiObjects.size() - 1; i >= 0; i--) {
			if (m_selectionRect.Intersects(m_guiObjects[i].boundingRect)) {
				m_selectedObjectIndex = i;
				m_guiObjects[i].isSelected = true;
				m_hasSelection = false;
			}
		}
		Refresh();
	}
}

void GUIRepresentationDrawingPanel::OnKeyboardInput(wxKeyEvent& event) {
	if (m_selectedObjectIndex > -1) {
		int xOffset = 0;
		int yOffset = 0;

		if (event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION)) {
			switch (event.GetKeyCode()) {
				case WXK_LEFT:
					xOffset -= 1;
					break;
				case WXK_UP:
					yOffset -= 1;
					break;
				case WXK_RIGHT:
					xOffset += 1;
					break;
				case WXK_DOWN:
					yOffset += 1;
					break;
				default:
					break;
			}

			for (unsigned i = 0; i < m_guiObjects.size(); i++) {
				if (m_guiObjects[i].isSelected) {
					int xPos = m_guiObjects[i].boundingRect.x + xOffset;
					int yPos = m_guiObjects[i].boundingRect.y + yOffset;
					if (m_guiObjects[i].element) {
						m_guiObjects[i].element->setPosX(xPos);
						m_guiObjects[i].element->setPosY(yPos);
						if (m_guiObjects[i].element->getType().IsSameAs(wxT("Label"))) {
							GUILabel *labelElement = dynamic_cast<GUILabel*>(m_guiObjects[i].element);
							if (labelElement) {
								if (!labelElement->isFreeXPlacement())
									labelElement->setFreeXPlacement(true);
								if (!labelElement->isFreeYPlacement())
									labelElement->setFreeYPlacement(true);
							}
						}
					} else if (m_guiObjects[i].img) {
						m_guiObjects[i].img->setPositionX(xPos);
						m_guiObjects[i].img->setPositionY(yPos);
					}
					m_guiObjects[i].boundingRect.x = xPos;
					m_guiObjects[i].boundingRect.y = yPos;
				}
			}
			::wxGetApp().m_frame->GUIElementPositionIsChanged();

			wxClientDC dc(this);
			wxDCOverlay overlaydc(m_overlay, &dc);
			overlaydc.Clear();
			dc.SetBrush(*wxTRANSPARENT_BRUSH);
			dc.SetPen(wxPen(wxColour(*wxYELLOW), 1, wxPENSTYLE_DOT));
			for (unsigned i = 0; i < m_guiObjects.size(); i++) {
				if (m_guiObjects[i].isSelected) {
					int xPos = m_guiObjects[i].boundingRect.x;
					int yPos = m_guiObjects[i].boundingRect.y;
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

			return;
		} else {
			event.Skip();
		}
	}
	event.Skip();
}

void GUIRepresentationDrawingPanel::OnKeyRelease(wxKeyEvent& event) {
	if (m_selectedObjectIndex > -1) {
		if (event.IsKeyInCategory(WXK_CATEGORY_NAVIGATION)) {
			// When key is released a full re-paint is made
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
		Refresh();
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
			// if the image is empty it should just be skipped
			if (!m_currentPanel->getImageAt(i)->getBitmap().IsOk())
				continue;
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
		int manYpos = currentMan->m_renderInfo.keys_y;
		if (currentMan->getPosX() >= 0)
			manXpos = currentMan->getPosX();
		if (currentMan->getPosY() >= 0) {
			manYpos = currentMan->getPosY();
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
						textToDisplay = btnElement->getElementName();
					if (btnElement->getDispLabelText() == wxEmptyString) {
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
						} else if (textToDisplay.IsSameAs(wxT("SetterXXXDivisionalPrevBank"))) {
							textToDisplay = wxT("-");
						} else if (textToDisplay.IsSameAs(wxT("SetterXXXDivisionalNextBank"))) {
							textToDisplay = wxT("+");
						} else if (textToDisplay.IsSameAs(wxT("GeneralPrev"))) {
							textToDisplay = wxT("Prev");
						} else if (textToDisplay.IsSameAs(wxT("GeneralNext"))) {
							textToDisplay = wxT("Next");
						} else if (textToDisplay.IsSameAs(wxT("Home"))) {
							textToDisplay = wxT("000");
						} else if (textToDisplay.IsSameAs(wxT("LoadFile"))) {
							textToDisplay = wxT("Load File");
						} else if (textToDisplay.IsSameAs(wxT("PrevFile"))) {
							textToDisplay = wxT("Prev File");
						} else if (textToDisplay.IsSameAs(wxT("NextFile"))) {
							textToDisplay = wxT("Next File");
						} else if (textToDisplay.IsSameAs(wxT("SaveFile"))) {
							textToDisplay = wxT("Save File");
						} else if (textToDisplay.IsSameAs(wxT("RefreshFiles"))) {
							textToDisplay = wxT("Refresh Files");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoA"))) {
							textToDisplay = wxT("A");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoB"))) {
							textToDisplay = wxT("B");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoC"))) {
							textToDisplay = wxT("C");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoD"))) {
							textToDisplay = wxT("D");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoPrev"))) {
							textToDisplay = wxT("<");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoNext"))) {
							textToDisplay = wxT(">");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoCurrent"))) {
							textToDisplay = wxT("Current");
						} else if (textToDisplay.IsSameAs(wxT("CrescendoOverride"))) {
							textToDisplay = wxT("Override");
						} else if (textToDisplay.IsSameAs(wxT("GC"))) {
							textToDisplay = wxT("G.C.");
						} else if (textToDisplay.IsSameAs(wxT("P1"))) {
							textToDisplay = wxT("+1");
						} else if (textToDisplay.IsSameAs(wxT("M1"))) {
							textToDisplay = wxT("-1");
						} else if (textToDisplay.IsSameAs(wxT("P10"))) {
							textToDisplay = wxT("+10");
						} else if (textToDisplay.IsSameAs(wxT("M10"))) {
							textToDisplay = wxT("-10");
						} else if (textToDisplay.IsSameAs(wxT("P100"))) {
							textToDisplay = wxT("+100");
						} else if (textToDisplay.IsSameAs(wxT("M100"))) {
							textToDisplay = wxT("-100");
						} else if (textToDisplay.IsSameAs(wxT("PitchP1"))) {
							textToDisplay = wxT("+1");
						} else if (textToDisplay.IsSameAs(wxT("PitchP10"))) {
							textToDisplay = wxT("+10");
						} else if (textToDisplay.IsSameAs(wxT("PitchP100"))) {
							textToDisplay = wxT("+100");
						} else if (textToDisplay.IsSameAs(wxT("PitchM1"))) {
							textToDisplay = wxT("-1");
						} else if (textToDisplay.IsSameAs(wxT("PitchM10"))) {
							textToDisplay = wxT("-10");
						} else if (textToDisplay.IsSameAs(wxT("PitchM100"))) {
							textToDisplay = wxT("-100");
						} else if (textToDisplay.IsSameAs(wxT("TemperamentPrev"))) {
							textToDisplay = wxT("<");
						} else if (textToDisplay.IsSameAs(wxT("TemperamentNext"))) {
							textToDisplay = wxT(">");
						} else if (textToDisplay.IsSameAs(wxT("TransposeUp"))) {
							textToDisplay = wxT("+");
						} else if (textToDisplay.IsSameAs(wxT("TransposeDown"))) {
							textToDisplay = wxT("-");
						} else if (textToDisplay.IsSameAs(wxT("L0"))) {
							textToDisplay = wxT("__0");
						} else if (textToDisplay.IsSameAs(wxT("L1"))) {
							textToDisplay = wxT("__1");
						} else if (textToDisplay.IsSameAs(wxT("L2"))) {
							textToDisplay = wxT("__2");
						} else if (textToDisplay.IsSameAs(wxT("L3"))) {
							textToDisplay = wxT("__3");
						} else if (textToDisplay.IsSameAs(wxT("L4"))) {
							textToDisplay = wxT("__4");
						} else if (textToDisplay.IsSameAs(wxT("L5"))) {
							textToDisplay = wxT("__5");
						} else if (textToDisplay.IsSameAs(wxT("L6"))) {
							textToDisplay = wxT("__6");
						} else if (textToDisplay.IsSameAs(wxT("L7"))) {
							textToDisplay = wxT("__7");
						} else if (textToDisplay.IsSameAs(wxT("L8"))) {
							textToDisplay = wxT("__8");
						} else if (textToDisplay.IsSameAs(wxT("L9"))) {
							textToDisplay = wxT("__9");
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
					wxString textToDisplay;
					if (encElement->getDispLabelText() != wxEmptyString)
						textToDisplay = encElement->getDispLabelText();
					else
						textToDisplay = encElement->getElementName();
					if (encElement->getDispLabelText() == wxEmptyString) {
						if (textToDisplay.IsSameAs(wxT("Swell")))
							textToDisplay = wxT("Crescendo");
					}
					dc.DrawLabel(BreakTextLine(textToDisplay, encElement->getTextBreakWidth(), dc), textRect, wxALIGN_CENTER_HORIZONTAL);
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
		for (unsigned i = 0; i < m_guiObjects.size(); i++) {
			if (m_guiObjects[i].isSelected)
				dc.DrawRectangle(m_guiObjects[i].boundingRect);
		}
	} else if (m_hasSelection) {
		wxDCOverlay overlaydc(m_overlay, &dc);
		overlaydc.Clear();
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.SetPen(wxPen(wxColour(*wxGREEN), 1, wxPENSTYLE_DOT_DASH));
		dc.DrawRectangle(m_selectionRect);

		// corners for adjusting selection rectangle
		dc.SetPen(wxPen(wxColour(*wxGREEN), 1, wxPENSTYLE_SOLID));
		dc.DrawLine(m_selectionRectLeftUpCorner.GetBottomLeft(), m_selectionRectLeftUpCorner.GetBottomRight());
		dc.DrawLine(m_selectionRectLeftUpCorner.GetBottomRight(), m_selectionRectLeftUpCorner.GetTopRight());

		dc.DrawLine(m_selectionRectRightUpCorner.GetTopLeft(), m_selectionRectRightUpCorner.GetBottomLeft());
		dc.DrawLine(m_selectionRectRightUpCorner.GetBottomLeft(), m_selectionRectRightUpCorner.GetBottomRight());

		dc.DrawLine(m_selectionRectRightDownCorner.GetBottomLeft(), m_selectionRectRightDownCorner.GetTopLeft());
		dc.DrawLine(m_selectionRectRightDownCorner.GetTopLeft(), m_selectionRectRightDownCorner.GetTopRight());

		dc.DrawLine(m_selectionRectLeftDownCorner.GetTopLeft(), m_selectionRectLeftDownCorner.GetTopRight());
		dc.DrawLine(m_selectionRectLeftDownCorner.GetTopRight(), m_selectionRectLeftDownCorner.GetBottomRight());

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
		theManual->m_renderInfo.width += 16;
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

void GUIRepresentationDrawingPanel::NotChangingTheSelection() {
	m_changingLeftUpCorner = false;
	m_changingLeftDownCorner = false;
	m_changingRightUpCorner = false;
	m_changingRightDownCorner = false;
	m_selectionChangingWidth = 0;
	m_selectionChangingHeight = 0;
}

bool GUIRepresentationDrawingPanel::IsSelectionChanging() {
	if (m_changingLeftUpCorner || m_changingLeftDownCorner || m_changingRightUpCorner || m_changingRightDownCorner)
		return true;
	else
		return false;
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
	PostSizeEvent();
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

void GUIRepresentationDrawingPanel::OnPanelSize(wxSizeEvent& WXUNUSED(event)) {
	GetParent()->SetClientSize(GetClientSize());
	GetParent()->SetMaxClientSize(GetClientSize());
}
