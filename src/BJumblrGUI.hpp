/* B.Jumblr
 * Pattern-controlled audio stream / sample re-sequencer LV2 plugin
 *
 * Copyright (C) 2018, 2019 by Sven Jähnichen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef BJUMBLRGUI_HPP_
#define BJUMBLRGUI_HPP_

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/forge.h>
#include <lv2/lv2plug.in/ns/ext/time/time.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <iostream>
#include <algorithm>

#include "BWidgets/Widget.hpp"
#include "BWidgets/Window.hpp"
#include "BWidgets/Label.hpp"
#include "BWidgets/DrawingSurface.hpp"
#include "BWidgets/DialValue.hpp"
#include "BWidgets/PopupListBox.hpp"
#include "BWidgets/LeftButton.hpp"
#include "BWidgets/PlusButton.hpp"
#include "BWidgets/MinusButton.hpp"
#include "BWidgets/ImageIcon.hpp"
#include "BWidgets/TextToggleButton.hpp"
#include "screen.h"

#ifdef LOCALEFILE
#include LOCALEFILE
#else
#include "Locale_EN.hpp"
#endif

#include "drawbutton.hpp"
#include "HaloButton.hpp"
#include "HaloToggleButton.hpp"
#include "HomeButton.hpp"
#include "LoadButton.hpp"
#include "PadSurface.hpp"
#include "definitions.h"
#include "Ports.hpp"
#include "Urids.hpp"
#include "Pad.hpp"
#include "PadMessage.hpp"
#include "Journal.hpp"
#include "MonitorWidget.hpp"
#include "MarkerWidget.hpp"
#include "SymbolWidget.hpp"
#include "SampleChooser.hpp"

#define BG_FILE "inc/surface.png"
#define HELP_URL "https://github.com/sjaehn/BJumblr/blob/master/README.md"
#define YT_URL "https://www.youtube.com/watch?v=DFSi7TMqvMw"
#define MAXUNDO 20

#ifndef WWW_BROWSER_CMD
#define WWW_BROWSER_CMD "x-www-browser"
#endif

#define RESIZE(widget, x, y, w, h, sz) {widget.moveTo ((x) * (sz), (y) * (sz)); widget.resize ((w) * (sz), (h) * (sz));}

enum editIndex
{
	EDIT_CUT	= 0,
	EDIT_COPY	= 1,
	EDIT_XFLIP	= 2,
	EDIT_YFLIP	= 3,
	EDIT_PASTE	= 4,
	EDIT_RESET	= 5,
	EDIT_UNDO	= 6,
	EDIT_REDO	= 7,
	MAXEDIT		= 8
};

const std::string editLabels[MAXEDIT] =
{
	BJUMBLR_LABEL_SELECT_CUT,
	BJUMBLR_LABEL_SELECT_COPY,
	BJUMBLR_LABEL_SELECT_XFLIP,
	BJUMBLR_LABEL_SELECT_YFLIP,
	BJUMBLR_LABEL_PASTE,
	BJUMBLR_LABEL_RESET,
	BJUMBLR_LABEL_UNDO,
	BJUMBLR_LABEL_REDO};

class BJumblrGUI : public BWidgets::Window
{
public:
	BJumblrGUI (const char *bundle_path, const LV2_Feature *const *features, PuglNativeView parentWindow);
	~BJumblrGUI ();
	void port_event (uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer);
	void send_ui_on ();
	void send_ui_off ();
	void send_samplePath ();
	void send_sampleAmp();
	void send_editMode ();
	void send_maxPage ();
	void send_playbackPage ();
	void send_requestMidiLearn ();
	void send_pad (int page);
	void send_pad (int page, int row, int step);
	void send_flip ();
	virtual void onConfigureRequest (BEvents::ExposeEvent* event) override;
	virtual void onCloseRequest (BEvents::WidgetEvent* event) override;
	virtual void onKeyPressed (BEvents::KeyEvent* event) override;
	virtual void onKeyReleased (BEvents::KeyEvent* event) override;
	void applyTheme (BStyles::Theme& theme) override;

	LV2UI_Controller controller;
	LV2UI_Write_Function write_function;

private:
	static void valueChangedCallback(BEvents::Event* event);
	static void pageClickedCallback(BEvents::Event* event);
	static void pageSymbolClickedCallback(BEvents::Event* event);
	static void pagePlayClickedCallback(BEvents::Event* event);
	static void pageScrollClickedCallback(BEvents::Event* event);
	static void midiSymbolClickedCallback(BEvents::Event* event);
	static void midiButtonClickedCallback(BEvents::Event* event);
	static void midiStatusChangedCallback(BEvents::Event* event);
	static void levelChangedCallback(BEvents::Event* event);
	static void edit1ChangedCallback(BEvents::Event* event);
	static void edit2ChangedCallback(BEvents::Event* event);
	static void padsPressedCallback (BEvents::Event* event);
	static void padsScrolledCallback (BEvents::Event* event);
	static void padsFocusedCallback (BEvents::Event* event);
	static void loadButtonClickedCallback (BEvents::Event* event);
	static void syncButtonClickedCallback (BEvents::Event* event);
	static void delayButtonsClickedCallback (BEvents::Event* event);
	static void patternFlippedClickedCallback (BEvents::Event* event);
	static void helpButtonClickedCallback (BEvents::Event* event);
	static void ytButtonClickedCallback (BEvents::Event* event);
	virtual void resize () override;
	void pushPage ();
	void popPage ();
	void gotoPage (const int page);
	void insertPage (const int page);
	void deletePage (const int page);
	void swapPage (const int page1, const int page2);
	void updatePageContainer ();
	bool validatePad (int page);
	bool validatePad (int page, int row, int step, Pad& pad);

	void setMarkers();
	void drawPad ();
	void drawPad (int row, int step);
	void drawPad (cairo_t* cr, int row, int step);

	std::string pluginPath;
	double sz;
	cairo_surface_t* bgImageSurface;

	BJumblrURIs uris;
	LV2_Atom_Forge forge;

	// Controllers
	std::array<BWidgets::ValueWidget*, MAXCONTROLLERS> controllerWidgets;
	std::array<float, MAXCONTROLLERS> controllers;
	int editMode;

	//Pads
	class Pattern
	{
	public:
		void clear ();
		Pad getPad (const size_t row, const size_t step) const;
		void setPad (const size_t row, const size_t step, const Pad& pad);
		std::vector<PadMessage> undo ();
		std::vector<PadMessage> redo ();
		void store ();
	private:
		Journal<std::vector<PadMessage>, MAXUNDO> journal;
		std::array<std::array<Pad, MAXSTEPS>, MAXSTEPS> pads;
		struct
		{
			std::vector<PadMessage> oldMessage;
			std::vector<PadMessage> newMessage;
		} changes;
	};

	std::array<Pattern, MAXPAGES> pattern;
	bool patternFlipped;

	struct ClipBoard
	{
		std::vector<std::vector<Pad>> data;
		std::pair<int, int> origin;
		std::pair<int, int> extends;
		bool ready = true;
		std::chrono::steady_clock::time_point time;
	};

	ClipBoard clipBoard;

	// Cursor
	float cursor;
	bool wheelScrolled;
	bool padPressed;
	bool deleteMode;

	std::string samplePath;
	int64_t sampleStart;
	int64_t sampleEnd;
	bool sampleLoop;

	// Pages
	int actPage;
	int nrPages;
	int pageOffset;

	//Widgets
	BWidgets::Widget mContainer;
	BWidgets::Label messageLabel;
	BWidgets::ValueWidget pageWidget;
	SymbolWidget pageBackSymbol;
	SymbolWidget pageForwardSymbol;

	BWidgets::Label sourceLabel;
	BWidgets::Label calibrationLabel;
	BWidgets::Label stepEditModeLabel;
	BWidgets::Label stepSizeLabel;
	BWidgets::Label patternSizeLabel;
	BWidgets::Label padLevelLabel;
	BWidgets::Label playbackLabel;
	BWidgets::Label speedLabel;

	struct Tab
	{
		BWidgets::Widget container;
		BWidgets::ImageIcon icon;
		SymbolWidget playSymbol;
		SymbolWidget midiSymbol;
		std::array<SymbolWidget, 4> symbols;
		std::array<BWidgets::ValueWidget, 4> midiWidgets;
	};

	std::array<Tab, MAXPAGES> tabs;

	BWidgets::ValueWidget midiBox;
	BWidgets::Label midiText;
	BWidgets::Label midiStatusLabel;
	BWidgets::PopupListBox midiStatusListBox;
	BWidgets::Label midiChannelLabel;
	BWidgets::PopupListBox midiChannelListBox;
	BWidgets::Label midiNoteLabel;
	BWidgets::PopupListBox midiNoteListBox;
	BWidgets::Label midiValueLabel;
	BWidgets::PopupListBox midiValueListBox;
	BWidgets::TextToggleButton midiLearnButton;
	BWidgets::TextButton midiCancelButton;
	BWidgets::TextButton midiOkButton;

	BWidgets::Widget flipButton;
	PadSurface padSurface;
	MarkerWidget markerFwd;
	MarkerWidget markerRev;
	MonitorWidget monitorWidget;
	BWidgets::PopupListBox sourceListBox;
	LoadButton loadButton;
	BWidgets::Label sampleNameLabel;
	BWidgets::Dial sampleAmpDial;
	HaloToggleButton playButton;
	HaloToggleButton bypassButton;
	HaloButton stopButton;
	std::array<HaloToggleButton, EDIT_RESET> edit1Buttons;
	std::array<HaloButton, MAXEDIT - EDIT_RESET> edit2Buttons;
	BWidgets::ValueWidget syncWidget;
	BWidgets::LeftButton zeroStepOffsetButton;
	BWidgets::MinusButton decStepOffsetButton;
	HomeButton hostSyncButton;
	BWidgets::PlusButton incStepOffsetButton;
	BWidgets::PopupListBox editModeListBox;
	BWidgets::PopupListBox stepSizeListBox;
	BWidgets::PopupListBox stepBaseListBox;
	BWidgets::PopupListBox padSizeListBox;
	std::array<HaloToggleButton, 5> levelButtons;
	BWidgets::DialValue levelDial;
	BWidgets::Label delayDisplayLabel;
	BWidgets::RangeWidget manualProgressionDelayWidget;
	HaloButton resetDelayButton;
	HaloButton increaseDelayButton;
	HaloButton decreaseDelayButton;
	HaloButton setStartDelayButton;
	BWidgets::DialValue speedDial;
	HaloButton helpButton;
	HaloButton ytButton;
	SampleChooser* fileChooser;


	// Definition of styles
	BColors::ColorSet fgColors = {{{0.75, 0.75, 0.0, 1.0}, {1.0, 1.0, 0.25, 1.0}, {0.1, 0.1, 0.0, 1.0}, {0.0, 0.0, 0.0, 0.0}}};
	BColors::ColorSet tgColors = {{BColors::grey, BColors::white, BColors::grey, BColors::darkgrey}};
	BColors::ColorSet buttonBgColors = {{{0.4, 0.4, 0.4, 1.0}, {0.6, 0.6, 0.6, 1.0}, {0.05, 0.05, 0.05, 1.0}, {0.0, 0.0, 0.0, 1.0}}};
	BColors::ColorSet knobBgColors = {{{0.15, 0.15, 0.15, 1.0}, {0.3, 0.3, 0.3, 1.0}, {0.05, 0.05, 0.05, 1.0}, {0.0, 0.0, 0.0, 1.0}}};
	BColors::ColorSet tgBgColors = {{{0.0, 0.03, 0.06, 1.0}, {0.3, 0.3, 0.3, 1.0}, {0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0, 1.0}}};
	BColors::ColorSet ltColors = {{{1.0, 1.0, 1.0, 1.0}, {1.0, 1.0, 1.0, 1.0}, {0.25, 0.25, 0.25, 1.0}, {0.0, 0.0, 0.0, 1.0}}};
	BColors::ColorSet wvColors = {{{1.0, 1.0, 1.0, 0.15}, {1.0, 1.0, 1.0, 0.15}, {0.25, 0.25, 0.25, 0.15}, {0.0, 0.0, 0.0, 0.15}}};
	BColors::ColorSet blkColors = {{{0.0, 0.0, 0.0, 0.75}, {0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0, 0.25}, {0.0, 0.0, 0.0, 0.0}}};
	BColors::Color ink = {0.0, 0.25, 0.5, 1.0};
	BColors::Color evenPadBgColor = {0.0, 0.05, 0.2, 1.0};
	BColors::Color oddPadBgColor = {0.0, 0.0, 0.0, 1.0};

	BStyles::Border border = {{ink, 1.0}, 0.0, 2.0, 0.0};
	BStyles::Border menuBorder = {{BColors::grey, 1.0}, 0.0, 0.0, 0.0};
	BStyles::Border labelborder = {BStyles::noLine, 4.0, 0.0, 0.0};
	BStyles::Border boxlabelborder = {{BColors::grey, 1.0}, 0.0, 3.0, 0.0};
	BStyles::Border focusborder = BStyles::Border (BStyles::Line (BColors::Color (0.0, 0.0, 0.0, 0.5), 2.0));
	BStyles::Fill widgetBg = BStyles::noFill;
	BStyles::Fill tabBg = BStyles::Fill (BColors::Color (0.75, 0.75, 0.0, 0.5));
	BStyles::Fill activeTabBg = BStyles::Fill (BColors::Color (0.75, 0.75, 0.0, 1.0));
	BStyles::Fill menuBg = BStyles::Fill (BColors::Color (0.0, 0.0, 0.05, 1.0));
	BStyles::Fill screenBg = BStyles::Fill (BColors::Color (0.0, 0.0, 0.0, 0.8));
	BStyles::Fill boxBg = BStyles::Fill (BColors::Color (0.0, 0.0, 0.0, 0.9));
	BStyles::Font ctLabelFont = BStyles::Font ("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL, 12.0,
						   BStyles::TEXT_ALIGN_CENTER, BStyles::TEXT_VALIGN_MIDDLE);
   	BStyles::Font tLabelFont = BStyles::Font ("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD, 12.0,
   						   BStyles::TEXT_ALIGN_CENTER, BStyles::TEXT_VALIGN_MIDDLE);
	BStyles::Font tgLabelFont = BStyles::Font ("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL, 12.0,
						   BStyles::TEXT_ALIGN_CENTER, BStyles::TEXT_VALIGN_MIDDLE);
	BStyles::Font lfLabelFont = BStyles::Font ("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL, 12.0,
						   BStyles::TEXT_ALIGN_LEFT, BStyles::TEXT_VALIGN_MIDDLE);
	BStyles::Font boldLfLabelFont = BStyles::Font ("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD, 12.0,
						   BStyles::TEXT_ALIGN_LEFT, BStyles::TEXT_VALIGN_MIDDLE);
	BStyles::Font smLabelFont = BStyles::Font ("Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL, 8.0,
						   BStyles::TEXT_ALIGN_CENTER, BStyles::TEXT_VALIGN_MIDDLE);
	BStyles::StyleSet defaultStyles = {"default", {{"background", STYLEPTR (&BStyles::noFill)},
					  {"border", STYLEPTR (&BStyles::noBorder)}}};
	BStyles::StyleSet labelStyles = {"labels", {{"background", STYLEPTR (&BStyles::noFill)},
					{"border", STYLEPTR (&labelborder)},
					{"textcolors", STYLEPTR (&BColors::whites)},
					{"font", STYLEPTR (&ctLabelFont)}}};
	BStyles::StyleSet smlabelStyles = {"labels", {{"background", STYLEPTR (&BStyles::noFill)},
					  {"border", STYLEPTR (&labelborder)},
					  {"textcolors", STYLEPTR (&BColors::whites)},
					  {"font", STYLEPTR (&smLabelFont)}}};
	BStyles::StyleSet focusStyles = {"labels", {{"background", STYLEPTR (&screenBg)},
					{"border", STYLEPTR (&focusborder)},
					{"textcolors", STYLEPTR (&ltColors)},
					{"font", STYLEPTR (&lfLabelFont)}}};

	BStyles::Theme theme = BStyles::Theme
	({
		defaultStyles,
		{"B.Jumblr", 		{{"background", STYLEPTR (&BStyles::blackFill)},
					 {"border", STYLEPTR (&BStyles::noBorder)}}},
		{"main", 		{{"background", STYLEPTR (&widgetBg)},
					 {"border", STYLEPTR (&BStyles::noBorder)}}},
 		{"monitor", 		{{"uses", STYLEPTR (&defaultStyles)},
 					 {"fgcolors", STYLEPTR (&wvColors)}}},
		{"widget", 		{{"uses", STYLEPTR (&defaultStyles)}}},
		{"widget/focus",	{{"uses", STYLEPTR (&focusStyles)}}},
		{"screen", 		{{"background", STYLEPTR (&screenBg)},
					 {"border", STYLEPTR (&BStyles::noBorder)}}},
		{"tab", 		{{"background", STYLEPTR (&tabBg)},
					 {"border", STYLEPTR (&BStyles::noBorder)},
			 		 {"fgcolors", STYLEPTR (&blkColors)}}},
		{"activetab", 		{{"background", STYLEPTR (&activeTabBg)},
					 {"border", STYLEPTR (&BStyles::noBorder)},
			 		 {"fgcolors", STYLEPTR (&blkColors)}}},
		{"symbol", 		{{"uses", STYLEPTR (&defaultStyles)},
					 {"fgcolors", STYLEPTR (&blkColors)}}},
		{"symbol/focus",	{{"uses", STYLEPTR (&focusStyles)}}},
		{"box", 		{{"background", STYLEPTR (&boxBg)},
					{"border", STYLEPTR (&border)}}},
		{"box/focus",		{{"uses", STYLEPTR (&focusStyles)}}},
		{"boxlabel",		{{"background", STYLEPTR (&boxBg)},
					 {"border", STYLEPTR (&boxlabelborder)},
					 {"textcolors", STYLEPTR (&BColors::whites)},
 					 {"font", STYLEPTR (&lfLabelFont)}}},
 		{"smboxlabel",		{{"background", STYLEPTR (&boxBg)},
 					 {"border", STYLEPTR (&boxlabelborder)},
 					 {"textcolors", STYLEPTR (&BColors::whites)},
  					 {"font", STYLEPTR (&smLabelFont)}}},
		{"button", 		{{"background", STYLEPTR (&BStyles::blackFill)},
					 {"border", STYLEPTR (&border)},
				 	 {"bgcolors", STYLEPTR (&buttonBgColors)}}},
		{"tgbutton", 		{{"border", STYLEPTR (&BStyles::noBorder)},
					 {"textcolors", STYLEPTR (&tgColors)},
					 {"bgcolors", STYLEPTR (&tgBgColors)},
					 {"font", STYLEPTR (&tgLabelFont)}}},
		{"tgbutton/focus",	{{"uses", STYLEPTR (&focusStyles)}}},
		{"dial", 		{{"uses", STYLEPTR (&defaultStyles)},
					 {"fgcolors", STYLEPTR (&fgColors)},
					 {"bgcolors", STYLEPTR (&knobBgColors)},
					 {"textcolors", STYLEPTR (&fgColors)},
					 {"font", STYLEPTR (&ctLabelFont)}}},
		{"dial/focus", 		{{"uses", STYLEPTR (&focusStyles)}}},
		{"tlabel",	 	{{"uses", STYLEPTR (&smlabelStyles)},
					 {"font", STYLEPTR (&tLabelFont)}}},
		{"ylabel",	 	{{"uses", STYLEPTR (&smlabelStyles)},
					 {"textcolors", STYLEPTR (&fgColors)}}},
		{"ctlabel",	 	{{"uses", STYLEPTR (&labelStyles)}}},
		{"lflabel",	 	{{"uses", STYLEPTR (&labelStyles)},
					 {"font", STYLEPTR (&lfLabelFont)}}},
		{"menu",	 	{{"border", STYLEPTR (&menuBorder)},
					 {"background", STYLEPTR (&menuBg)}}},
		{"menu/item",	 	{{"uses", STYLEPTR (&defaultStyles)},
					 {"border", STYLEPTR (&labelborder)},
					 {"textcolors", STYLEPTR (&BColors::whites)},
					 {"font", STYLEPTR (&lfLabelFont)}}},
		{"menu/button",	 	{{"border", STYLEPTR (&menuBorder)},
					 {"background", STYLEPTR (&menuBg)},
					 {"bgcolors", STYLEPTR (&buttonBgColors)}}},
		{"menu/listbox",	{{"border", STYLEPTR (&menuBorder)},
					 {"background", STYLEPTR (&menuBg)}}},
		{"menu/listbox/item",	{{"uses", STYLEPTR (&defaultStyles)},
					 {"border", STYLEPTR (&labelborder)},
					 {"textcolors", STYLEPTR (&BColors::whites)},
					 {"font", STYLEPTR (&lfLabelFont)}}},
		{"menu/listbox/button",	{{"border", STYLEPTR (&menuBorder)},
					 {"background", STYLEPTR (&menuBg)},
					 {"bgcolors", STYLEPTR (&buttonBgColors)}}},
		{"filechooser",	 			{{"border", STYLEPTR (&menuBorder)},
					 		 {"background", STYLEPTR (&menuBg)}}},
		{"filechooser/label",			{{"background", STYLEPTR (&menuBg)},
					 		 {"border", STYLEPTR (&labelborder)},
					 	 	 {"textcolors", STYLEPTR (&BColors::whites)},
					 	 	 {"font", STYLEPTR (&lfLabelFont)}}},
		{"filechooser/textbox", 		{{"background", STYLEPTR (&menuBg)},
					 		 {"border", STYLEPTR (&boxlabelborder)},
					 	 	 {"textcolors", STYLEPTR (&BColors::whites)},
					 	 	 {"font", STYLEPTR (&lfLabelFont)}}},
		{"filechooser/scrollbar",		{{"uses", STYLEPTR (&defaultStyles)},
					 		 {"fgcolors", STYLEPTR (&blkColors)},
				 	 		 {"bgcolors", STYLEPTR (&knobBgColors)}}},
		{"filechooser/marker",			{{"uses", STYLEPTR (&defaultStyles)},
				 	 		 {"bgcolors", STYLEPTR (&fgColors)}}},
		{"filechooser/checkbox",		{{"uses", STYLEPTR (&defaultStyles)},
				 	 		 {"fgcolors", STYLEPTR (&fgColors)},
					 		 {"bgcolors", STYLEPTR (&knobBgColors)}}},
		{"filechooser/listbox",			{{"border", STYLEPTR (&menuBorder)},
					 		 {"background", STYLEPTR (&menuBg)}}},
		{"filechooser/listbox/item",		{{"uses", STYLEPTR (&defaultStyles)},
					 		 {"border", STYLEPTR (&labelborder)},
					 	 	 {"textcolors", STYLEPTR (&BColors::whites)},
					 	 	 {"font", STYLEPTR (&lfLabelFont)}}},
		{"filechooser/listbox/item/dir",	{{"uses", STYLEPTR (&defaultStyles)},
					 		 {"border", STYLEPTR (&labelborder)},
					 	 	 {"textcolors", STYLEPTR (&BColors::whites)},
					 	 	 {"font", STYLEPTR (&boldLfLabelFont)}}},
		{"filechooser/listbox/item/file",	{{"uses", STYLEPTR (&defaultStyles)},
					 		 {"border", STYLEPTR (&labelborder)},
					 	 	 {"textcolors", STYLEPTR (&BColors::whites)},
					 	 	 {"font", STYLEPTR (&lfLabelFont)}}},
		{"filechooser/listbox/button",		{{"border", STYLEPTR (&menuBorder)},
					 		 {"background", STYLEPTR (&menuBg)},
					 	 	 {"bgcolors", STYLEPTR (&buttonBgColors)}}},
		{"filechooser/popup",	 		{{"border", STYLEPTR (&menuBorder)},
					 		 {"background", STYLEPTR (&menuBg)}}},
		{"filechooser/popup/item",	 	{{"uses", STYLEPTR (&defaultStyles)},
					 		 {"border", STYLEPTR (&labelborder)},
					 	 	 {"textcolors", STYLEPTR (&BColors::whites)},
					 	 	 {"font", STYLEPTR (&lfLabelFont)}}},
		{"filechooser/popup/button",	 	{{"border", STYLEPTR (&menuBorder)},
					 		 {"background", STYLEPTR (&menuBg)},
					 	 	 {"bgcolors", STYLEPTR (&buttonBgColors)}}},
		{"filechooser/popup/listbox",		{{"border", STYLEPTR (&menuBorder)},
					 		 {"background", STYLEPTR (&menuBg)}}},
		{"filechooser/popup/listbox/item",	{{"uses", STYLEPTR (&defaultStyles)},
					 		 {"border", STYLEPTR (&labelborder)},
					 	 	 {"textcolors", STYLEPTR (&BColors::whites)},
					 	 	 {"font", STYLEPTR (&lfLabelFont)}}},
		{"filechooser/popup/listbox/button",	{{"border", STYLEPTR (&menuBorder)},
					 		 {"background", STYLEPTR (&menuBg)},
					 	 	 {"bgcolors", STYLEPTR (&buttonBgColors)}}}
	});
};

#endif /* BJUMBLRGUI_HPP_ */
