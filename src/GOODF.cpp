/* 
 * GOODF.cpp is a part of GOODF software
 * Copyright (C) 2025 Lars Palo and contributors (see AUTHORS)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * You can contact the author on larspalo(at)yahoo DOT se
 */

#include "GOODF.h"
#include "GOODFDef.h"
#include "GoImages.h"
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "wx/fs_zip.h"
#include <wx/mstream.h>

IMPLEMENT_APP(GOODF)

bool GOODF::OnInit() {
	// Create fullAppName with version from cmake
	m_fullAppName = wxT("GoOdf ");
	m_fullAppName.Append(wxT(GOODF_VERSION));

	// Create the frame window
	m_frame = new GOODFFrame(m_fullAppName);

	// Fix paths/locations
	wxFileName fn(wxStandardPaths::Get().GetExecutablePath());
	fn = fn.GetPath();
	wxString BaseDir = fn.GetPath();
	wxString ResourceDir = BaseDir + wxFILE_SEP_PATH + wxT("share");

	// the help controller
	wxFileSystem::AddHandler(new wxZipFSHandler);
	m_helpController = new wxHtmlHelpController();
	m_helpController->AddBook(wxFileName(ResourceDir + wxFILE_SEP_PATH + wxT("GoOdf/help/help.zip")));
	m_helpController->SetFrameParameters(wxT("%s"), wxDefaultSize, wxDefaultPosition);

	// Load icons
	// wxImage::AddHandler(new wxPNGHandler);
	wxInitAllImageHandlers();
	wxString iconPath = ResourceDir + wxFILE_SEP_PATH + wxT("icons") + wxFILE_SEP_PATH + wxT("hicolor");
	wxString appsPath = wxT("apps");
	wxString imgPath = appsPath + wxFILE_SEP_PATH + wxT("GoOdf.png");
	m_icons = wxIconBundle(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("16x16") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("24x24") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("32x32") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("48x48") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("64x64") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("128x128") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("256x256") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("512x512") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));
	m_icons.AddIcon(wxIcon(iconPath + wxFILE_SEP_PATH + wxT("1024x1024") + wxFILE_SEP_PATH + imgPath, wxBITMAP_TYPE_PNG));

	m_frame->SetIcons(m_icons);

	// Extract embedded images
	wxBitmap drawstop1 = wxBITMAP_PNG_FROM_DATA(drawstop01off);
	wxBitmap drawstop2 = wxBITMAP_PNG_FROM_DATA(drawstop02off);
	wxBitmap drawstop3 = wxBITMAP_PNG_FROM_DATA(drawstop03off);
	wxBitmap drawstop4 = wxBITMAP_PNG_FROM_DATA(drawstop04off);
	wxBitmap drawstop5 = wxBITMAP_PNG_FROM_DATA(drawstop05off);
	wxBitmap drawstop6 = wxBITMAP_PNG_FROM_DATA(drawstop06off);
	wxBitmap drawstop7 = wxBITMAP_PNG_FROM_DATA(drawstop07off);
	m_drawstopBitmaps.push_back(drawstop1);
	m_drawstopBitmaps.push_back(drawstop2);
	m_drawstopBitmaps.push_back(drawstop3);
	m_drawstopBitmaps.push_back(drawstop4);
	m_drawstopBitmaps.push_back(drawstop5);
	m_drawstopBitmaps.push_back(drawstop6);
	m_drawstopBitmaps.push_back(drawstop7);
	wxImage drawstopImage = drawstop1.ConvertToImage();
	drawstop1 = wxBitmap(drawstopImage.Scale(32, 32));
	drawstopImage = drawstop2.ConvertToImage();
	drawstop2 = wxBitmap(drawstopImage.Scale(32, 32));
	drawstopImage = drawstop3.ConvertToImage();
	drawstop3 = wxBitmap(drawstopImage.Scale(32, 32));
	drawstopImage = drawstop4.ConvertToImage();
	drawstop4 = wxBitmap(drawstopImage.Scale(32, 32));
	drawstopImage = drawstop5.ConvertToImage();
	drawstop5 = wxBitmap(drawstopImage.Scale(32, 32));
	drawstopImage = drawstop6.ConvertToImage();
	drawstop6 = wxBitmap(drawstopImage.Scale(32, 32));
	drawstopImage = drawstop7.ConvertToImage();
	drawstop7 = wxBitmap(drawstopImage.Scale(32, 32));

	m_scaledDrawstopBitmaps.push_back(drawstop1);
	m_scaledDrawstopBitmaps.push_back(drawstop2);
	m_scaledDrawstopBitmaps.push_back(drawstop3);
	m_scaledDrawstopBitmaps.push_back(drawstop4);
	m_scaledDrawstopBitmaps.push_back(drawstop5);
	m_scaledDrawstopBitmaps.push_back(drawstop6);
	m_scaledDrawstopBitmaps.push_back(drawstop7);

	wxBitmap piston1 = wxBITMAP_PNG_FROM_DATA(piston01off);
	wxBitmap piston2 = wxBITMAP_PNG_FROM_DATA(piston02off);
	wxBitmap piston3 = wxBITMAP_PNG_FROM_DATA(piston03off);
	wxBitmap piston4 = wxBITMAP_PNG_FROM_DATA(piston04off);
	wxBitmap piston5 = wxBITMAP_PNG_FROM_DATA(piston05off);
	m_buttonBitmaps.push_back(piston1);
	m_buttonBitmaps.push_back(piston2);
	m_buttonBitmaps.push_back(piston3);
	m_buttonBitmaps.push_back(piston4);
	m_buttonBitmaps.push_back(piston5);

	wxBitmap enclosure1 = wxBITMAP_PNG_FROM_DATA(EnclosureA00);
	wxBitmap enclosure2 = wxBITMAP_PNG_FROM_DATA(EnclosureB00);
	wxBitmap enclosure3 = wxBITMAP_PNG_FROM_DATA(EnclosureC00);
	wxBitmap enclosure4 = wxBITMAP_PNG_FROM_DATA(EnclosureD00);
	m_enclosureStyleBitmaps.push_back(enclosure1);
	m_enclosureStyleBitmaps.push_back(enclosure2);
	m_enclosureStyleBitmaps.push_back(enclosure3);
	m_enclosureStyleBitmaps.push_back(enclosure4);

	wxBitmap label1 = wxBITMAP_PNG_FROM_DATA(label01);
	// wxBitmap label2 = wxBITMAP_PNG_FROM_DATA(label02); Same style as 03 but 80x50
	wxBitmap label3 = wxBITMAP_PNG_FROM_DATA(label03);
	// wxBitmap label4 = wxBITMAP_PNG_FROM_DATA(label04); Same style as 03 but 160x25
	// wxBitmap label5 = wxBITMAP_PNG_FROM_DATA(label05); Same style as 03 but 200x50
	// wxBitmap label6 = wxBITMAP_PNG_FROM_DATA(label06); Same style as 07 but 80x50
	wxBitmap label7 = wxBITMAP_PNG_FROM_DATA(label07);
	// wxBitmap label8 = wxBITMAP_PNG_FROM_DATA(label08); Same style as 07 but 160x25
	// wxBitmap label9 = wxBITMAP_PNG_FROM_DATA(label09); Same style as 10 but 80x50
	wxBitmap label10 = wxBITMAP_PNG_FROM_DATA(label10);
	// wxBitmap label11 = wxBITMAP_PNG_FROM_DATA(label11); Same style as 10 but 160x25
	// wxBitmap label12 = wxBITMAP_PNG_FROM_DATA(label12); Same style as 10 but 200x50
	wxBitmap label13 = wxBITMAP_PNG_FROM_DATA(label13);
	wxImage labelImage = label13.ConvertToImage();
	label13 = wxBitmap(labelImage.Scale(80, 25));
	wxBitmap label14 = wxBITMAP_PNG_FROM_DATA(label14);
	labelImage = label14.ConvertToImage();
	label14 = wxBitmap(labelImage.Scale(80, 25));
	wxBitmap label15 = wxBITMAP_PNG_FROM_DATA(label15);
	labelImage = label15.ConvertToImage();
	label15 = wxBitmap(labelImage.Scale(80, 25));
	m_labelBitmaps.push_back(wxNullBitmap);
	m_labelBitmaps.push_back(label1);
	m_labelBitmaps.push_back(label3);
	m_labelBitmaps.push_back(label3);
	m_labelBitmaps.push_back(label3);
	m_labelBitmaps.push_back(label3);
	m_labelBitmaps.push_back(label7);
	m_labelBitmaps.push_back(label7);
	m_labelBitmaps.push_back(label7);
	m_labelBitmaps.push_back(label10);
	m_labelBitmaps.push_back(label10);
	m_labelBitmaps.push_back(label10);
	m_labelBitmaps.push_back(label10);
	m_labelBitmaps.push_back(label13);
	m_labelBitmaps.push_back(label14);
	m_labelBitmaps.push_back(label15);

	wxBitmap label2 = wxBITMAP_PNG_FROM_DATA(label02);
	wxBitmap label4 = wxBITMAP_PNG_FROM_DATA(label04);
	wxBitmap label5 = wxBITMAP_PNG_FROM_DATA(label05);
	wxBitmap label6 = wxBITMAP_PNG_FROM_DATA(label06);
	wxBitmap label8 = wxBITMAP_PNG_FROM_DATA(label08);
	wxBitmap label9 = wxBITMAP_PNG_FROM_DATA(label09);
	wxBitmap label11 = wxBITMAP_PNG_FROM_DATA(label11);
	wxBitmap label12 = wxBITMAP_PNG_FROM_DATA(label12);
	wxBitmap fullLabel13 = wxBITMAP_PNG_FROM_DATA(label13);
	wxBitmap fullLabel14 = wxBITMAP_PNG_FROM_DATA(label14);
	wxBitmap fullLabel15 = wxBITMAP_PNG_FROM_DATA(label15);
	m_fullSizeLabelBmps.push_back(wxNullBitmap);
	m_fullSizeLabelBmps.push_back(label1);
	m_fullSizeLabelBmps.push_back(label2);
	m_fullSizeLabelBmps.push_back(label3);
	m_fullSizeLabelBmps.push_back(label4);
	m_fullSizeLabelBmps.push_back(label5);
	m_fullSizeLabelBmps.push_back(label6);
	m_fullSizeLabelBmps.push_back(label7);
	m_fullSizeLabelBmps.push_back(label8);
	m_fullSizeLabelBmps.push_back(label9);
	m_fullSizeLabelBmps.push_back(label10);
	m_fullSizeLabelBmps.push_back(label11);
	m_fullSizeLabelBmps.push_back(label12);
	m_fullSizeLabelBmps.push_back(fullLabel13);
	m_fullSizeLabelBmps.push_back(fullLabel14);
	m_fullSizeLabelBmps.push_back(fullLabel15);

	wxBitmap defaultManual1 = wxBITMAP_PNG_FROM_DATA(ManualDefaultComposite01);
	wxBitmap defaultManual2 = wxBITMAP_PNG_FROM_DATA(ManualDefaultComposite02);
	m_defaultManualBitmaps.push_back(defaultManual1);
	m_defaultManualBitmaps.push_back(defaultManual2);

	wxBitmap invertedManual1 = wxBITMAP_PNG_FROM_DATA(ManualInvertedComposite01);
	wxBitmap invertedManual2 = wxBITMAP_PNG_FROM_DATA(ManualInvertedComposite02);
	m_invertedManualBitmaps.push_back(invertedManual1);
	m_invertedManualBitmaps.push_back(invertedManual2);

	wxBitmap woodenManual1 = wxBITMAP_PNG_FROM_DATA(ManualWoodenComposite01);
	wxBitmap woodenManual2 = wxBITMAP_PNG_FROM_DATA(ManualWoodenComposite02);
	m_woodenManualBitmaps.push_back(woodenManual1);
	m_woodenManualBitmaps.push_back(woodenManual2);

	wxBitmap invertedWoodenManual1 = wxBITMAP_PNG_FROM_DATA(ManualInvertedWoodenComposite01);
	wxBitmap invertedWoodenManual2 = wxBITMAP_PNG_FROM_DATA(ManualInvertedWoodenComposite02);
	m_invertedWoodenManualBitmaps.push_back(invertedWoodenManual1);
	m_invertedWoodenManualBitmaps.push_back(invertedWoodenManual2);

	wxBitmap defaultPedal1 = wxBITMAP_PNG_FROM_DATA(PedalDefaultComposite01);
	wxBitmap defaultPedal2 = wxBITMAP_PNG_FROM_DATA(PedalDefaultComposite02);
	m_defaultPedalBitmaps.push_back(defaultPedal1);
	m_defaultPedalBitmaps.push_back(defaultPedal2);

	wxBitmap invertedPedal1 = wxBITMAP_PNG_FROM_DATA(PedalInvertedComposite01);
	wxBitmap invertedPedal2 = wxBITMAP_PNG_FROM_DATA(PedalInvertedComposite02);
	m_invertedPedalBitmaps.push_back(invertedPedal1);
	m_invertedPedalBitmaps.push_back(invertedPedal2);

	wxBitmap whiteCF = wxBITMAP_PNG_FROM_DATA(ManualCWhiteUp01);
	wxBitmap whiteDGA = wxBITMAP_PNG_FROM_DATA(ManualDWhiteUp01);
	wxBitmap whiteEB = wxBITMAP_PNG_FROM_DATA(ManualEWhiteUp01);
	wxBitmap blackSharp = wxBITMAP_PNG_FROM_DATA(ManualSharpBlackUp01);
	wxBitmap whiteWhole = wxBITMAP_PNG_FROM_DATA(ManualNaturalWhiteUp01);
	m_manualKeyBmps01.push_back(whiteCF);
	m_manualKeyBmps01.push_back(blackSharp);
	m_manualKeyBmps01.push_back(whiteDGA);
	m_manualKeyBmps01.push_back(blackSharp);
	m_manualKeyBmps01.push_back(whiteEB);
	m_manualKeyBmps01.push_back(whiteCF);
	m_manualKeyBmps01.push_back(blackSharp);
	m_manualKeyBmps01.push_back(whiteDGA);
	m_manualKeyBmps01.push_back(blackSharp);
	m_manualKeyBmps01.push_back(whiteDGA);
	m_manualKeyBmps01.push_back(blackSharp);
	m_manualKeyBmps01.push_back(whiteEB);
	m_manualKeyBmps01.push_back(whiteWhole);

	wxBitmap blackCF = wxBITMAP_PNG_FROM_DATA(ManualCBlackUp01);
	wxBitmap blackDGA = wxBITMAP_PNG_FROM_DATA(ManualDBlackUp01);
	wxBitmap blackEB = wxBITMAP_PNG_FROM_DATA(ManualEBlackUp01);
	wxBitmap whiteSharp = wxBITMAP_PNG_FROM_DATA(ManualSharpWhiteUp01);
	wxBitmap blackWhole = wxBITMAP_PNG_FROM_DATA(ManualNaturalBlackUp01);
	m_invertedManualKeysBmps01.push_back(blackCF);
	m_invertedManualKeysBmps01.push_back(whiteSharp);
	m_invertedManualKeysBmps01.push_back(blackDGA);
	m_invertedManualKeysBmps01.push_back(whiteSharp);
	m_invertedManualKeysBmps01.push_back(blackEB);
	m_invertedManualKeysBmps01.push_back(blackCF);
	m_invertedManualKeysBmps01.push_back(whiteSharp);
	m_invertedManualKeysBmps01.push_back(blackDGA);
	m_invertedManualKeysBmps01.push_back(whiteSharp);
	m_invertedManualKeysBmps01.push_back(blackDGA);
	m_invertedManualKeysBmps01.push_back(whiteSharp);
	m_invertedManualKeysBmps01.push_back(blackEB);
	m_invertedManualKeysBmps01.push_back(blackWhole);

	wxBitmap woodCF = wxBITMAP_PNG_FROM_DATA(ManualCWoodUp01);
	wxBitmap woodDGA = wxBITMAP_PNG_FROM_DATA(ManualDWoodUp01);
	wxBitmap woodEB = wxBITMAP_PNG_FROM_DATA(ManualEWoodUp01);
	wxBitmap woodWhole = wxBITMAP_PNG_FROM_DATA(ManualNaturalWoodUp01);
	m_woodenManualKeysBmps01.push_back(woodCF);
	m_woodenManualKeysBmps01.push_back(blackSharp);
	m_woodenManualKeysBmps01.push_back(woodDGA);
	m_woodenManualKeysBmps01.push_back(blackSharp);
	m_woodenManualKeysBmps01.push_back(woodEB);
	m_woodenManualKeysBmps01.push_back(woodCF);
	m_woodenManualKeysBmps01.push_back(blackSharp);
	m_woodenManualKeysBmps01.push_back(woodDGA);
	m_woodenManualKeysBmps01.push_back(blackSharp);
	m_woodenManualKeysBmps01.push_back(woodDGA);
	m_woodenManualKeysBmps01.push_back(blackSharp);
	m_woodenManualKeysBmps01.push_back(woodEB);
	m_woodenManualKeysBmps01.push_back(woodWhole);

	wxBitmap woodSharp = wxBITMAP_PNG_FROM_DATA(ManualSharpWoodUp01);
	m_invertedManualWoodenKeysBmps01.push_back(blackCF);
	m_invertedManualWoodenKeysBmps01.push_back(woodSharp);
	m_invertedManualWoodenKeysBmps01.push_back(blackDGA);
	m_invertedManualWoodenKeysBmps01.push_back(woodSharp);
	m_invertedManualWoodenKeysBmps01.push_back(blackEB);
	m_invertedManualWoodenKeysBmps01.push_back(blackCF);
	m_invertedManualWoodenKeysBmps01.push_back(woodSharp);
	m_invertedManualWoodenKeysBmps01.push_back(blackDGA);
	m_invertedManualWoodenKeysBmps01.push_back(woodSharp);
	m_invertedManualWoodenKeysBmps01.push_back(blackDGA);
	m_invertedManualWoodenKeysBmps01.push_back(woodSharp);
	m_invertedManualWoodenKeysBmps01.push_back(blackEB);
	m_invertedManualWoodenKeysBmps01.push_back(blackWhole);

	wxBitmap naturalPedalUp = wxBITMAP_PNG_FROM_DATA(PedalNaturalWoodUp01);
	wxBitmap sharpPedalUp = wxBITMAP_PNG_FROM_DATA(PedalSharpBlackUp01);
	m_pedalKeysBmps01.push_back(naturalPedalUp);
	m_pedalKeysBmps01.push_back(sharpPedalUp);
	m_pedalKeysBmps01.push_back(naturalPedalUp);
	m_pedalKeysBmps01.push_back(sharpPedalUp);
	m_pedalKeysBmps01.push_back(naturalPedalUp);
	m_pedalKeysBmps01.push_back(naturalPedalUp);
	m_pedalKeysBmps01.push_back(sharpPedalUp);
	m_pedalKeysBmps01.push_back(naturalPedalUp);
	m_pedalKeysBmps01.push_back(sharpPedalUp);
	m_pedalKeysBmps01.push_back(naturalPedalUp);
	m_pedalKeysBmps01.push_back(sharpPedalUp);
	m_pedalKeysBmps01.push_back(naturalPedalUp);
	m_pedalKeysBmps01.push_back(naturalPedalUp);

	wxBitmap invertedPedalUp = wxBITMAP_PNG_FROM_DATA(PedalNaturalBlackUp01);
	wxBitmap woodPedalSharpUp = wxBITMAP_PNG_FROM_DATA(PedalSharpWoodUp01);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);
	m_invertedPedalKeysBmps01.push_back(woodPedalSharpUp);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);
	m_invertedPedalKeysBmps01.push_back(woodPedalSharpUp);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);
	m_invertedPedalKeysBmps01.push_back(woodPedalSharpUp);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);
	m_invertedPedalKeysBmps01.push_back(woodPedalSharpUp);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);
	m_invertedPedalKeysBmps01.push_back(woodPedalSharpUp);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);
	m_invertedPedalKeysBmps01.push_back(invertedPedalUp);

	wxBitmap whiteCF2 = wxBITMAP_PNG_FROM_DATA(ManualCWhiteUp02);
	wxBitmap whiteDGA2 = wxBITMAP_PNG_FROM_DATA(ManualDWhiteUp02);
	wxBitmap whiteEB2 = wxBITMAP_PNG_FROM_DATA(ManualEWhiteUp02);
	wxBitmap blackSharp2 = wxBITMAP_PNG_FROM_DATA(ManualSharpBlackUp02);
	wxBitmap whiteWhole2 = wxBITMAP_PNG_FROM_DATA(ManualNaturalWhiteUp02);
	m_manualKeyBmps02.push_back(whiteCF2);
	m_manualKeyBmps02.push_back(blackSharp2);
	m_manualKeyBmps02.push_back(whiteDGA2);
	m_manualKeyBmps02.push_back(blackSharp2);
	m_manualKeyBmps02.push_back(whiteEB2);
	m_manualKeyBmps02.push_back(whiteCF2);
	m_manualKeyBmps02.push_back(blackSharp2);
	m_manualKeyBmps02.push_back(whiteDGA2);
	m_manualKeyBmps02.push_back(blackSharp2);
	m_manualKeyBmps02.push_back(whiteDGA2);
	m_manualKeyBmps02.push_back(blackSharp2);
	m_manualKeyBmps02.push_back(whiteEB2);
	m_manualKeyBmps02.push_back(whiteWhole2);

	wxBitmap blackCF2 = wxBITMAP_PNG_FROM_DATA(ManualCBlackUp02);
	wxBitmap blackDGA2 = wxBITMAP_PNG_FROM_DATA(ManualDBlackUp02);
	wxBitmap blackEB2 = wxBITMAP_PNG_FROM_DATA(ManualEBlackUp02);
	wxBitmap whiteSharp2 = wxBITMAP_PNG_FROM_DATA(ManualSharpWhiteUp02);
	wxBitmap blackWhole2 = wxBITMAP_PNG_FROM_DATA(ManualNaturalBlackUp02);
	m_invertedManualKeysBmps02.push_back(blackCF2);
	m_invertedManualKeysBmps02.push_back(whiteSharp2);
	m_invertedManualKeysBmps02.push_back(blackDGA2);
	m_invertedManualKeysBmps02.push_back(whiteSharp2);
	m_invertedManualKeysBmps02.push_back(blackEB2);
	m_invertedManualKeysBmps02.push_back(blackCF2);
	m_invertedManualKeysBmps02.push_back(whiteSharp2);
	m_invertedManualKeysBmps02.push_back(blackDGA2);
	m_invertedManualKeysBmps02.push_back(whiteSharp2);
	m_invertedManualKeysBmps02.push_back(blackDGA2);
	m_invertedManualKeysBmps02.push_back(whiteSharp2);
	m_invertedManualKeysBmps02.push_back(blackEB2);
	m_invertedManualKeysBmps02.push_back(blackWhole2);

	wxBitmap woodCF2 = wxBITMAP_PNG_FROM_DATA(ManualCWoodUp02);
	wxBitmap woodDGA2 = wxBITMAP_PNG_FROM_DATA(ManualDWoodUp02);
	wxBitmap woodEB2 = wxBITMAP_PNG_FROM_DATA(ManualEWoodUp02);
	wxBitmap woodWhole2 = wxBITMAP_PNG_FROM_DATA(ManualNaturalWoodUp02);
	m_woodenManualKeysBmps02.push_back(woodCF2);
	m_woodenManualKeysBmps02.push_back(blackSharp2);
	m_woodenManualKeysBmps02.push_back(woodDGA2);
	m_woodenManualKeysBmps02.push_back(blackSharp2);
	m_woodenManualKeysBmps02.push_back(woodEB2);
	m_woodenManualKeysBmps02.push_back(woodCF2);
	m_woodenManualKeysBmps02.push_back(blackSharp2);
	m_woodenManualKeysBmps02.push_back(woodDGA2);
	m_woodenManualKeysBmps02.push_back(blackSharp2);
	m_woodenManualKeysBmps02.push_back(woodDGA2);
	m_woodenManualKeysBmps02.push_back(blackSharp2);
	m_woodenManualKeysBmps02.push_back(woodEB2);
	m_woodenManualKeysBmps02.push_back(woodWhole2);

	wxBitmap woodSharp2 = wxBITMAP_PNG_FROM_DATA(ManualSharpWoodUp02);
	m_invertedManualWoodenKeysBmps02.push_back(blackCF2);
	m_invertedManualWoodenKeysBmps02.push_back(woodSharp2);
	m_invertedManualWoodenKeysBmps02.push_back(blackDGA2);
	m_invertedManualWoodenKeysBmps02.push_back(woodSharp2);
	m_invertedManualWoodenKeysBmps02.push_back(blackEB2);
	m_invertedManualWoodenKeysBmps02.push_back(blackCF2);
	m_invertedManualWoodenKeysBmps02.push_back(woodSharp2);
	m_invertedManualWoodenKeysBmps02.push_back(blackDGA2);
	m_invertedManualWoodenKeysBmps02.push_back(woodSharp2);
	m_invertedManualWoodenKeysBmps02.push_back(blackDGA2);
	m_invertedManualWoodenKeysBmps02.push_back(woodSharp2);
	m_invertedManualWoodenKeysBmps02.push_back(blackEB2);
	m_invertedManualWoodenKeysBmps02.push_back(blackWhole2);

	wxBitmap naturalPedalUp2 = wxBITMAP_PNG_FROM_DATA(PedalNaturalWoodUp02);
	wxBitmap sharpPedalUp2 = wxBITMAP_PNG_FROM_DATA(PedalSharpBlackUp02);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);
	m_pedalKeysBmps02.push_back(sharpPedalUp2);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);
	m_pedalKeysBmps02.push_back(sharpPedalUp2);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);
	m_pedalKeysBmps02.push_back(sharpPedalUp2);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);
	m_pedalKeysBmps02.push_back(sharpPedalUp2);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);
	m_pedalKeysBmps02.push_back(sharpPedalUp2);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);
	m_pedalKeysBmps02.push_back(naturalPedalUp2);

	wxBitmap invertedPedal2Up = wxBITMAP_PNG_FROM_DATA(PedalNaturalBlackUp02);
	wxBitmap woodPedalSharp2Up = wxBITMAP_PNG_FROM_DATA(PedalSharpWoodUp02);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);
	m_invertedPedalKeysBmps02.push_back(woodPedalSharp2Up);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);
	m_invertedPedalKeysBmps02.push_back(woodPedalSharp2Up);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);
	m_invertedPedalKeysBmps02.push_back(woodPedalSharp2Up);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);
	m_invertedPedalKeysBmps02.push_back(woodPedalSharp2Up);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);
	m_invertedPedalKeysBmps02.push_back(woodPedalSharp2Up);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);
	m_invertedPedalKeysBmps02.push_back(invertedPedal2Up);

	// Extract embedded wood jpg images into m_woodBitmaps vector
	m_woodBitmaps.push_back(GetJpegBitmap(Wood01_jpg, sizeof(Wood01_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood01_jpg, sizeof(Wood01_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood03_jpg, sizeof(Wood03_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood03_jpg, sizeof(Wood03_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood05_jpg, sizeof(Wood05_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood05_jpg, sizeof(Wood05_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood07_jpg, sizeof(Wood07_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood07_jpg, sizeof(Wood07_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood09_jpg, sizeof(Wood09_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood09_jpg, sizeof(Wood09_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood11_jpg, sizeof(Wood11_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood11_jpg, sizeof(Wood11_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood13_jpg, sizeof(Wood13_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood13_jpg, sizeof(Wood13_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood15_jpg, sizeof(Wood15_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood15_jpg, sizeof(Wood15_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood17_jpg, sizeof(Wood17_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood17_jpg, sizeof(Wood17_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood19_jpg, sizeof(Wood19_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood19_jpg, sizeof(Wood19_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood21_jpg, sizeof(Wood21_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood21_jpg, sizeof(Wood21_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood23_jpg, sizeof(Wood23_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood23_jpg, sizeof(Wood23_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood25_jpg, sizeof(Wood25_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood25_jpg, sizeof(Wood25_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood27_jpg, sizeof(Wood27_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood27_jpg, sizeof(Wood27_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood29_jpg, sizeof(Wood29_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood29_jpg, sizeof(Wood29_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood31_jpg, sizeof(Wood31_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood31_jpg, sizeof(Wood31_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood33_jpg, sizeof(Wood33_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood33_jpg, sizeof(Wood33_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood35_jpg, sizeof(Wood35_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood35_jpg, sizeof(Wood35_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood37_jpg, sizeof(Wood37_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood37_jpg, sizeof(Wood37_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood39_jpg, sizeof(Wood39_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood39_jpg, sizeof(Wood39_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood41_jpg, sizeof(Wood41_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood41_jpg, sizeof(Wood41_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood43_jpg, sizeof(Wood43_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood43_jpg, sizeof(Wood43_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood45_jpg, sizeof(Wood45_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood45_jpg, sizeof(Wood45_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood47_jpg, sizeof(Wood47_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood47_jpg, sizeof(Wood47_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood49_jpg, sizeof(Wood49_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood49_jpg, sizeof(Wood49_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood51_jpg, sizeof(Wood51_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood51_jpg, sizeof(Wood51_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood53_jpg, sizeof(Wood53_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood53_jpg, sizeof(Wood53_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood55_jpg, sizeof(Wood55_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood55_jpg, sizeof(Wood55_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood57_jpg, sizeof(Wood57_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood57_jpg, sizeof(Wood57_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood59_jpg, sizeof(Wood59_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood59_jpg, sizeof(Wood59_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood61_jpg, sizeof(Wood61_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood61_jpg, sizeof(Wood61_jpg)));
	m_woodBitmaps.push_back(GetJpegBitmap(Wood63_jpg, sizeof(Wood63_jpg)));
	m_woodBitmaps.push_back(GetJpeg90Bitmap(Wood63_jpg, sizeof(Wood63_jpg)));

	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood01_jpg, sizeof(Wood01_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood01_jpg, sizeof(Wood01_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood03_jpg, sizeof(Wood03_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood03_jpg, sizeof(Wood03_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood05_jpg, sizeof(Wood05_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood05_jpg, sizeof(Wood05_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood07_jpg, sizeof(Wood07_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood07_jpg, sizeof(Wood07_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood09_jpg, sizeof(Wood09_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood09_jpg, sizeof(Wood09_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood11_jpg, sizeof(Wood11_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood11_jpg, sizeof(Wood11_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood13_jpg, sizeof(Wood13_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood13_jpg, sizeof(Wood13_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood15_jpg, sizeof(Wood15_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood15_jpg, sizeof(Wood15_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood17_jpg, sizeof(Wood17_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood17_jpg, sizeof(Wood17_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood19_jpg, sizeof(Wood19_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood19_jpg, sizeof(Wood19_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood21_jpg, sizeof(Wood21_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood21_jpg, sizeof(Wood21_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood23_jpg, sizeof(Wood23_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood23_jpg, sizeof(Wood23_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood25_jpg, sizeof(Wood25_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood25_jpg, sizeof(Wood25_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood27_jpg, sizeof(Wood27_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood27_jpg, sizeof(Wood27_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood29_jpg, sizeof(Wood29_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood29_jpg, sizeof(Wood29_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood31_jpg, sizeof(Wood31_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood31_jpg, sizeof(Wood31_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood33_jpg, sizeof(Wood33_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood33_jpg, sizeof(Wood33_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood35_jpg, sizeof(Wood35_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood35_jpg, sizeof(Wood35_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood37_jpg, sizeof(Wood37_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood37_jpg, sizeof(Wood37_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood39_jpg, sizeof(Wood39_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood39_jpg, sizeof(Wood39_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood41_jpg, sizeof(Wood41_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood41_jpg, sizeof(Wood41_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood43_jpg, sizeof(Wood43_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood43_jpg, sizeof(Wood43_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood45_jpg, sizeof(Wood45_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood45_jpg, sizeof(Wood45_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood47_jpg, sizeof(Wood47_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood47_jpg, sizeof(Wood47_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood49_jpg, sizeof(Wood49_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood49_jpg, sizeof(Wood49_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood51_jpg, sizeof(Wood51_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood51_jpg, sizeof(Wood51_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood53_jpg, sizeof(Wood53_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood53_jpg, sizeof(Wood53_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood55_jpg, sizeof(Wood55_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood55_jpg, sizeof(Wood55_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood57_jpg, sizeof(Wood57_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood57_jpg, sizeof(Wood57_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood59_jpg, sizeof(Wood59_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood59_jpg, sizeof(Wood59_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood61_jpg, sizeof(Wood61_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood61_jpg, sizeof(Wood61_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpegBitmap(Wood63_jpg, sizeof(Wood63_jpg), true));
	m_scaledWoodBitmaps.push_back(GetJpeg90Bitmap(Wood63_jpg, sizeof(Wood63_jpg), true));

	// Show the frame
	m_frame->Show(true);

	// if a <file.organ> command line argument exists, try opening it as an organ file
	if (wxApp::argc > 1) {
		wxFileName f_name(wxApp::argv[1]);
		if (f_name.Exists() && f_name.GetExt().IsSameAs("organ", false)) { // ignore case
			// might like to use GetAbsolutePath() here, but it's only available in wx 3.1.6 or above
			if (f_name.MakeAbsolute()) {
				m_frame->DoOpenOrgan(f_name.GetFullPath());
			}
		}
	}

	// Start the event loop
	return true;
}

int GOODF::OnExit() {
	return wxApp::OnExit();
}

wxBitmap GOODF::GetJpegBitmap(const unsigned char *data, int length, bool doScale) {
	wxMemoryInputStream is(data, length);
	wxImage jpgImg(is, wxBITMAP_TYPE_JPEG);
	if (doScale)
		jpgImg.Rescale(32, 32);
	return wxBitmap(jpgImg);
}

wxBitmap GOODF::GetJpeg90Bitmap(const unsigned char *data, int length, bool doScale) {
	wxMemoryInputStream is(data, length);
	wxImage jpgImg(is, wxBITMAP_TYPE_JPEG);
	if (doScale)
		jpgImg.Rescale(32, 32);
	return wxBitmap(jpgImg.Rotate90());
}
