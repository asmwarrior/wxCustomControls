//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#include "wxcrafter.h"

// Declare the bitmap loading function
extern void wxC9ED9InitBitmapResources();

static bool bBitmapLoaded = false;

MainFrameBaseClass::MainFrameBaseClass(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos,
                                       const wxSize& size, long style)
    : wxFrame(parent, id, title, pos, size, style)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }

    wxBoxSizer* boxSizer1 = new wxBoxSizer(wxVERTICAL);
    this->SetSizer(boxSizer1);

    m_mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(this, wxSize(-1, -1)), wxTAB_TRAVERSAL);

    boxSizer1->Add(m_mainPanel, 1, wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer11 = new wxBoxSizer(wxVERTICAL);
    m_mainPanel->SetSizer(boxSizer11);

    m_toolbar =
        new clToolBar(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)), wxTB_FLAT);
    m_toolbar->SetToolBitmapSize(wxSize(16, 16));

    boxSizer11->Add(m_toolbar, 0, wxEXPAND, WXC_FROM_DIP(5));

    m_toolbar->AddTool(wxID_OPEN, _("Open"), wxXmlResource::Get()->LoadBitmap(wxT("16-folder-open-regular@2x")),
                       wxNullBitmap, wxITEM_DROPDOWN, wxT(""), wxT(""), NULL);

    m_toolbar->AddTool(wxID_NEW, _("Tool Label"), wxXmlResource::Get()->LoadBitmap(wxT("16-mime-txt@2x")), wxNullBitmap,
                       wxITEM_NORMAL, wxT(""), wxT(""), NULL);

    m_toolbar->AddStretchableSpace();

    m_toolbar->AddSeparator();

    m_toolbar->AddTool(wxID_BOOKMARK, _("Tool Label"), wxXmlResource::Get()->LoadBitmap(wxT("16-bookmark-yellow@2x")),
                       wxNullBitmap, wxITEM_CHECK, wxT(""), wxT(""), NULL);

    m_toolbar->AddTool(wxID_COLOURS, _("Tool Label"), wxXmlResource::Get()->LoadBitmap(wxT("16-colour-pallette@2x")),
                       wxNullBitmap, wxITEM_CHECK, wxT(""), wxT(""), NULL);

    m_toolbar->AddTool(wxID_ANY, _("Another Tool"), wxXmlResource::Get()->LoadBitmap(wxT("16-file-regular@2x")),
                       wxNullBitmap, wxITEM_CHECK, wxT(""), wxT(""), NULL);
    m_toolbar->Realize();

    m_panelControls =
        new wxPanel(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)), wxTAB_TRAVERSAL);

    boxSizer11->Add(m_panelControls, 1, wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer71 = new wxBoxSizer(wxHORIZONTAL);
    m_panelControls->SetSizer(boxSizer71);

    m_dataView = new clDataViewListCtrl(m_panelControls, wxID_ANY, wxDefaultPosition,
                                        wxDLG_UNIT(m_panelControls, wxSize(-1, 300)), wxDV_ROW_LINES | wxDV_SINGLE);

    boxSizer71->Add(m_dataView, 1, wxALL | wxEXPAND, WXC_FROM_DIP(1));

    m_dataView->AppendIconTextColumn(_("Path"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                     wxDATAVIEW_COL_RESIZABLE);
    m_dataView->AppendTextColumn(_("Is file?"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                 wxDATAVIEW_COL_RESIZABLE);
    m_dataView->AppendTextColumn(_("File size"), wxDATAVIEW_CELL_INERT, WXC_FROM_DIP(-2), wxALIGN_LEFT,
                                 wxDATAVIEW_COL_RESIZABLE);
    wxBoxSizer* boxSizer133 = new wxBoxSizer(wxVERTICAL);

    boxSizer11->Add(boxSizer133, 0, wxEXPAND, WXC_FROM_DIP(5));

    m_panelButtons =
        new wxPanel(m_mainPanel, wxID_ANY, wxDefaultPosition, wxDLG_UNIT(m_mainPanel, wxSize(-1, -1)), wxTAB_TRAVERSAL);

    boxSizer133->Add(m_panelButtons, 1, wxEXPAND, WXC_FROM_DIP(5));

    wxBoxSizer* boxSizer137 = new wxBoxSizer(wxHORIZONTAL);
    m_panelButtons->SetSizer(boxSizer137);

    wxFlexGridSizer* flexGridSizer141 = new wxFlexGridSizer(0, 3, 0, 0);
    flexGridSizer141->SetFlexibleDirection(wxBOTH);
    flexGridSizer141->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

    boxSizer137->Add(flexGridSizer141, 0, wxALL | wxEXPAND, WXC_FROM_DIP(5));

    m_buttonOne = new clButton(m_panelButtons, wxID_ANY, _("Close the Folder"), wxDefaultPosition,
                               wxDLG_UNIT(m_panelButtons, wxSize(150, -1)), 0);
    m_buttonOne->SetToolTip(_("Close the Folder"));

    flexGridSizer141->Add(m_buttonOne, 0, wxALL, WXC_FROM_DIP(5));

    m_buttonTwo = new clButton(m_panelButtons, wxID_ANY, _("Button Two Clicker"), wxDefaultPosition,
                               wxDLG_UNIT(m_panelButtons, wxSize(-1, -1)), 0);

    flexGridSizer141->Add(m_buttonTwo, 0, wxALL, WXC_FROM_DIP(5));

    m_buttonDisabled = new clButton(m_panelButtons, wxID_ANY, _("Disabled"), wxDefaultPosition,
                                    wxDLG_UNIT(m_panelButtons, wxSize(-1, -1)), 0);
    m_buttonDisabled->Enable(false);

    flexGridSizer141->Add(m_buttonDisabled, 0, wxALL, WXC_FROM_DIP(5));

    m_textCtrlLog = new wxTextCtrl(m_mainPanel, wxID_ANY, wxT(""), wxDefaultPosition,
                                   wxDLG_UNIT(m_mainPanel, wxSize(-1, 100)), wxTE_RICH | wxTE_MULTILINE);

    boxSizer11->Add(m_textCtrlLog, 0, wxEXPAND, WXC_FROM_DIP(1));

    m_myMenuBar = new wxMenuBar(0);
    this->SetMenuBar(m_myMenuBar);

    menu_clTreeCtrl = new wxMenu();
    m_myMenuBar->Append(menu_clTreeCtrl, _("clTreeCtrl"));

    m_menuItem13 = new wxMenuItem(menu_clTreeCtrl, ID_OPEN_FOLDER, _("Open folder..."), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItem13);

    menu_clTreeCtrl->AppendSeparator();

    m_menuItemExpandAll = new wxMenuItem(menu_clTreeCtrl, wxID_ANY, _("Expand All"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemExpandAll);

    m_menuItemCollapseAll = new wxMenuItem(menu_clTreeCtrl, wxID_ANY, _("Collapse All"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemCollapseAll);

    m_menuItemFirstVisible =
        new wxMenuItem(menu_clTreeCtrl, wxID_ANY, _("First Visible Item\tShift-F4"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemFirstVisible);

    m_menuItemNextVisibleItem =
        new wxMenuItem(menu_clTreeCtrl, wxID_ANY, _("Next Visible Item\tF4"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemNextVisibleItem);

    m_menuItemEnsureVisible =
        new wxMenuItem(menu_clTreeCtrl, wxID_ANY, _("Ensure Item Visible"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemEnsureVisible);

    m_menuItemSelectChildren =
        new wxMenuItem(menu_clTreeCtrl, ID_SELECT_CHILDREN, _("Select Children"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemSelectChildren);

    m_menuItemNextSibling =
        new wxMenuItem(menu_clTreeCtrl, ID_NEXT_SIBLING, _("Next Sibling\tF2"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemNextSibling);

    m_menuItemPrevSibling =
        new wxMenuItem(menu_clTreeCtrl, ID_PREV_SIBLING, _("Prev Sibling\tShift-F2"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemPrevSibling);

    m_menuItemDeleteAllItems = new wxMenuItem(menu_clTreeCtrl, wxID_ANY, _("Delete All Items"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemDeleteAllItems);

    menu_clTreeCtrl->AppendSeparator();

    m_menuItemFind = new wxMenuItem(menu_clTreeCtrl, wxID_FIND, _("Find Item...\tCtrl-F"), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemFind);

    m_menuItemSetWidth =
        new wxMenuItem(menu_clTreeCtrl, wxID_SET_TREE_COL_WIDTH, _("Set Column WIdth..."), wxT(""), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemSetWidth);

    menu_clTreeCtrl->AppendSeparator();

    m_menuItemExit = new wxMenuItem(menu_clTreeCtrl, wxID_EXIT, _("Exit\tAlt-X"), _("Quit"), wxITEM_NORMAL);
    menu_clTreeCtrl->Append(m_menuItemExit);

    menu_clDataViewListCtrl = new wxMenu();
    m_myMenuBar->Append(menu_clDataViewListCtrl, _("clDataViewListCtrl"));

    m_menuItem83 = new wxMenuItem(menu_clDataViewListCtrl, ID_DV_OPEN_FOLDER, _("Open folder"), wxT(""), wxITEM_NORMAL);
    menu_clDataViewListCtrl->Append(m_menuItem83);

    m_menuItem87 = new wxMenuItem(menu_clDataViewListCtrl, ID_FILL_WITH_5000_ENTRIES,
                                  _("Create view with 10,000 items"), wxT(""), wxITEM_NORMAL);
    menu_clDataViewListCtrl->Append(m_menuItem87);

    m_menuItem89 =
        new wxMenuItem(menu_clDataViewListCtrl, ID_DV_DELETE_ALL_ITEMS, _("Delete All Items"), wxT(""), wxITEM_NORMAL);
    menu_clDataViewListCtrl->Append(m_menuItem89);

    menu_Style = new wxMenu();
    m_myMenuBar->Append(menu_Style, _("Style"));

    m_menuItemZebra = new wxMenuItem(menu_Style, ID_ZEBRA, _("Zebra Colouring..."), wxT(""), wxITEM_CHECK);
    menu_Style->Append(m_menuItemZebra);
    m_menuItemZebra->Check();

    m_menuItemThemes = new wxMenuItem(menu_Style, ID_TOGGLE_THEMES, _("Toggle Themes"), wxT(""), wxITEM_NORMAL);
    menu_Style->Append(m_menuItemThemes);

    m_menuItemHideRoot = new wxMenuItem(menu_Style, ID_HIDE_ROOT, _("Hide Root"), wxT(""), wxITEM_CHECK);
    menu_Style->Append(m_menuItemHideRoot);

    menu_Style->AppendSeparator();

    m_menuItemSingleSelection =
        new wxMenuItem(menu_Style, ID_SINGLE_SELECTION, _("Single Selection Tree"), wxT(""), wxITEM_CHECK);
    menu_Style->Append(m_menuItemSingleSelection);
    m_menuItemSingleSelection->Check();

    m_menuItemShowSBOnFocus =
        new wxMenuItem(menu_Style, ID_SHOW_SB_ON_FOCUS, _("Show Scrollbar when focused Only"), wxT(""), wxITEM_CHECK);
    menu_Style->Append(m_menuItemShowSBOnFocus);

    m_menuItemHideHeaders = new wxMenuItem(menu_Style, wxID_ANY, _("Hide Header"), wxT(""), wxITEM_CHECK);
    menu_Style->Append(m_menuItemHideHeaders);

    m_menuItemNativeHeader =
        new wxMenuItem(menu_Style, ID_NATIVE_HEADER, _("Use Native Drawings"), wxT(""), wxITEM_CHECK);
    menu_Style->Append(m_menuItemNativeHeader);

    m_menuItemSupportSearch = new wxMenuItem(menu_Style, wxID_ANY, _("Enable Search"), wxT(""), wxITEM_CHECK);
    menu_Style->Append(m_menuItemSupportSearch);

    SetName(wxT("MainFrameBaseClass"));
    SetSize(wxDLG_UNIT(this, wxSize(-1, -1)));
    if(GetSizer()) { GetSizer()->Fit(this); }
    if(GetParent()) {
        CentreOnParent(wxBOTH);
    } else {
        CentreOnScreen(wxBOTH);
    }
#if wxVERSION_NUMBER >= 2900
    if(!wxPersistenceManager::Get().Find(this)) {
        wxPersistenceManager::Get().RegisterAndRestore(this);
    } else {
        wxPersistenceManager::Get().Restore(this);
    }
#endif
    // Connect events
    this->Connect(wxID_COLOURS, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnColoursUI), NULL, this);
    m_buttonOne->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnCloseFolderUI), NULL, this);
    m_buttonOne->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnButtonCloseFolder),
                         NULL, this);
    m_buttonTwo->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnButtonClicked), NULL,
                         this);
    this->Connect(m_menuItem13->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnOpenFolder), NULL, this);
    this->Connect(m_menuItemExpandAll->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnExpandAll), NULL, this);
    this->Connect(m_menuItemCollapseAll->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnCollapseAll), NULL, this);
    this->Connect(m_menuItemFirstVisible->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnFirstVisible), NULL, this);
    this->Connect(m_menuItemNextVisibleItem->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnNextVisible), NULL, this);
    this->Connect(m_menuItemEnsureVisible->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnEnsureItemVisible), NULL, this);
    this->Connect(m_menuItemSelectChildren->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnSelectChildren), NULL, this);
    this->Connect(m_menuItemNextSibling->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnNextSibling), NULL, this);
    this->Connect(m_menuItemPrevSibling->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnPrevSibling), NULL, this);
    this->Connect(m_menuItemDeleteAllItems->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnDeleteAllItems), NULL, this);
    this->Connect(m_menuItemFind->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnTreeFind), NULL, this);
    this->Connect(m_menuItemSetWidth->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnSetTreeColWidth), NULL, this);
    this->Connect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Connect(m_menuItem83->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnDVOpenFolder), NULL, this);
    this->Connect(m_menuItem87->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnFillWith500Entries), NULL, this);
    this->Connect(m_menuItem89->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnDVDeleteAllItems), NULL, this);
    this->Connect(m_menuItemZebra->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnZebraColouring), NULL, this);
    this->Connect(m_menuItemThemes->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnToggleTheme), NULL, this);
    this->Connect(m_menuItemHideRoot->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnHideRoot), NULL, this);
    this->Connect(m_menuItemSingleSelection->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnSingleSelection), NULL, this);
    this->Connect(m_menuItemShowSBOnFocus->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnShowSBOnFocus), NULL, this);
    this->Connect(m_menuItemHideHeaders->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnHideHeaders), NULL, this);
    this->Connect(m_menuItemNativeHeader->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnNativeHeader), NULL, this);
    this->Connect(m_menuItemSupportSearch->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                  wxCommandEventHandler(MainFrameBaseClass::OnMenuitemsupportsearchMenuSelected), NULL, this);
}

MainFrameBaseClass::~MainFrameBaseClass()
{
    this->Disconnect(wxID_COLOURS, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnColoursUI), NULL,
                     this);
    m_buttonOne->Disconnect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MainFrameBaseClass::OnCloseFolderUI), NULL, this);
    m_buttonOne->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                            wxCommandEventHandler(MainFrameBaseClass::OnButtonCloseFolder), NULL, this);
    m_buttonTwo->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainFrameBaseClass::OnButtonClicked),
                            NULL, this);
    this->Disconnect(m_menuItem13->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnOpenFolder), NULL, this);
    this->Disconnect(m_menuItemExpandAll->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnExpandAll), NULL, this);
    this->Disconnect(m_menuItemCollapseAll->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnCollapseAll), NULL, this);
    this->Disconnect(m_menuItemFirstVisible->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnFirstVisible), NULL, this);
    this->Disconnect(m_menuItemNextVisibleItem->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnNextVisible), NULL, this);
    this->Disconnect(m_menuItemEnsureVisible->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnEnsureItemVisible), NULL, this);
    this->Disconnect(m_menuItemSelectChildren->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnSelectChildren), NULL, this);
    this->Disconnect(m_menuItemNextSibling->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnNextSibling), NULL, this);
    this->Disconnect(m_menuItemPrevSibling->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnPrevSibling), NULL, this);
    this->Disconnect(m_menuItemDeleteAllItems->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnDeleteAllItems), NULL, this);
    this->Disconnect(m_menuItemFind->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnTreeFind), NULL, this);
    this->Disconnect(m_menuItemSetWidth->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnSetTreeColWidth), NULL, this);
    this->Disconnect(m_menuItemExit->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnExit), NULL, this);
    this->Disconnect(m_menuItem83->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnDVOpenFolder), NULL, this);
    this->Disconnect(m_menuItem87->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnFillWith500Entries), NULL, this);
    this->Disconnect(m_menuItem89->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnDVDeleteAllItems), NULL, this);
    this->Disconnect(m_menuItemZebra->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnZebraColouring), NULL, this);
    this->Disconnect(m_menuItemThemes->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnToggleTheme), NULL, this);
    this->Disconnect(m_menuItemHideRoot->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnHideRoot), NULL, this);
    this->Disconnect(m_menuItemSingleSelection->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnSingleSelection), NULL, this);
    this->Disconnect(m_menuItemShowSBOnFocus->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnShowSBOnFocus), NULL, this);
    this->Disconnect(m_menuItemHideHeaders->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnHideHeaders), NULL, this);
    this->Disconnect(m_menuItemNativeHeader->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnNativeHeader), NULL, this);
    this->Disconnect(m_menuItemSupportSearch->GetId(), wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(MainFrameBaseClass::OnMenuitemsupportsearchMenuSelected), NULL, this);
}

MyImages::MyImages()
    : wxImageList(16, 16, true)
    , m_imagesWidth(16)
    , m_imagesHeight(16)
{
    if(!bBitmapLoaded) {
        // We need to initialise the default bitmap handler
        wxXmlResource::Get()->AddHandler(new wxBitmapXmlHandler);
        wxC9ED9InitBitmapResources();
        bBitmapLoaded = true;
    }

    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("folder"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())) {
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("folder"), bmp));
        }
    }
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("folder@2x"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())) {
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("folder@2x"), bmp));
        }
    }

    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("folder_open"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())) {
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("folder_open"), bmp));
        }
    }
    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("folder_open@2x"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())) {
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("folder_open@2x"), bmp));
        }
    }

    {
        wxBitmap bmp;
        wxIcon icn;
        bmp = wxXmlResource::Get()->LoadBitmap(wxT("file"));
        if(bmp.IsOk()) {
            if((m_imagesWidth == bmp.GetWidth()) && (m_imagesHeight == bmp.GetHeight())) {
                icn.CopyFromBitmap(bmp);
                this->Add(icn);
            }
            m_bitmaps.insert(std::make_pair(wxT("file"), bmp));
        }
    }
}

MyImages::~MyImages() {}
