/*
  The contents of this file are subject to the Initial Developer's Public
  License Version 1.0 (the "License"); you may not use this file except in
  compliance with the License. You may obtain a copy of the License here:
  http://www.flamerobin.org/license.html.

  Software distributed under the License is distributed on an "AS IS"
  basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
  License for the specific language governing rights and limitations under
  the License.

  The Original Code is FlameRobin (TM).

  The Initial Developer of the Original Code is Milan Babuskov.

  Portions created by the original developer
  are Copyright (C) 2004 Milan Babuskov.

  All Rights Reserved.

  Contributor(s): Nando Dessena
*/

// -*- C++ -*- generated by wxGlade 0.2.2 on Sat Nov  1 19:07:15 2003

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "MainFrame.h"
#include "metadata/metadataitem.h"
#include "metadata/root.h"
#include "treeitem.h"
#include "myTreeCtrl.h"

//-----------------------------------------------------------------------------
MainFrame::MainFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    BaseFrame(parent, id, title, pos, size, style, wxT("FlameRobin_main"))
{
    tree_ctrl_1 = new myTreeCtrl(this, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS|wxSUNKEN_BORDER);
	menuBarM = new wxMenuBar();

	// build menuBarM
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(myTreeCtrl::Menu_Configure, _("&Preferences"), wxEmptyString, wxITEM_NORMAL);
    fileMenu->AppendSeparator();
    fileMenu->Append(myTreeCtrl::Menu_Quit, _("&Quit"), wxEmptyString, wxITEM_NORMAL);
    menuBarM->Append(fileMenu, _("&File"));

    databaseMenu = new wxMenu();					// dynamic menus, created at runtime
    menuBarM->Append(databaseMenu, _("&Database"));
    objectMenu = new wxMenu();
    menuBarM->Append(objectMenu, _("&Object"));
    windowMenu = new wxMenu();
    menuBarM->Append(windowMenu, _("&Window"));

    wxMenu* helpMenu = new wxMenu();
    helpMenu->Append(wxNewId(), _("&Manual"), wxEmptyString, wxITEM_NORMAL);
    helpMenu->Append(wxNewId(), _("&What's new"), wxEmptyString, wxITEM_NORMAL);
    helpMenu->AppendSeparator();
    helpMenu->Append(wxNewId(), _("&License"), wxEmptyString, wxITEM_NORMAL);
    helpMenu->Append(myTreeCtrl::Menu_About, _("&About"), wxEmptyString, wxITEM_NORMAL);
    menuBarM->Append(helpMenu, _("&Help"));
	SetMenuBar(menuBarM);

	statusBarM = CreateStatusBar();
    set_properties();
    do_layout();
}
//-----------------------------------------------------------------------------
void MainFrame::set_properties()
{
    SetTitle(_("FlameRobin Database Admin"));

	// Default tree looks pretty ugly on non-Windows platforms(GTK)
	#ifndef __WXMSW__
	tree_ctrl_1->SetIndent(10);
	#endif

	YTreeItem *rootdata = new YTreeItem(tree_ctrl_1);
	wxTreeItemId root = tree_ctrl_1->AddRoot(_("Firebird Servers"), tree_ctrl_1->getItemImage(ntRoot), -1, rootdata);
	getGlobalRoot().attach(rootdata);									// link wxTree root node with rootNodeM

	getGlobalRoot().load();
	if (tree_ctrl_1->GetCount() <= 1)
	{
		wxMessageBox(_("You may register a new server and databases."), _("File servers.xml not found"));

		YServer s; 					// add localhost
		s.setHostname("localhost");
		s.setPort("3050");
		getGlobalRoot().addServer(s);
	}
	tree_ctrl_1->Expand(root);

	#include "fricon.xpm"
    wxBitmap bmp(fricon_xpm);
    wxIcon icon;
    icon.CopyFromBitmap(bmp);
    SetIcon(icon);
}
//-----------------------------------------------------------------------------
void MainFrame::do_layout()
{
    // begin wxGlade: MainFrame::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxVERTICAL);
    sizer_1->Add(tree_ctrl_1, 1, wxEXPAND, 0);
    SetAutoLayout(true);
    SetSizer(sizer_1);
    Layout();
    // end wxGlade
}
//-----------------------------------------------------------------------------
const wxRect MainFrame::getDefaultRect() const
{
	return wxRect(-1, -1, 257, 367);
}
//-----------------------------------------------------------------------------

