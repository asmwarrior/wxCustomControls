#include "clScrollBar.h"
#include "clTreeCtrl.h"
#include "clTreeCtrlModel.h"
#include "clTreeNodeVisitor.h"
#include <algorithm>
#include <cmath>
#include <wx/app.h>
#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/dcmemory.h>
#include <wx/dnd.h>
#include <wx/log.h>
#include <wx/renderer.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/utils.h>
#include <wx/wupdlock.h>

#define CHECK_PTR_RET(p) \
    if(!p) { return; }

#define CHECK_ITEM_RET(item) \
    if(!item.IsOk()) { return; }

#define CHECK_ITEM_RET_INVALID_ITEM(item) \
    if(!item.IsOk()) { return wxTreeItemId(); }

#define CHECK_ITEM_RET_FALSE(item) \
    if(!item.IsOk()) { return false; }

#define CHECK_ROOT_RET() \
    if(!m_model.GetRoot()) { return; }

clTreeCtrl::clTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
    : clScrolledPanel(parent, wxID_ANY, pos, size, wxWANTS_CHARS)
    , m_model(this)
    , m_treeStyle(style)
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    wxSize textSize = GetTextSize("Tp");
    m_lineHeight = clRowEntry::Y_SPACER + textSize.GetHeight() + clRowEntry::Y_SPACER;
    SetIndent(m_lineHeight);
    Bind(wxEVT_IDLE, &clTreeCtrl::OnIdle, this);
    Bind(wxEVT_PAINT, &clTreeCtrl::OnPaint, this);
    Bind(wxEVT_SIZE, &clTreeCtrl::OnSize, this);
    Bind(wxEVT_ERASE_BACKGROUND, [&](wxEraseEvent& event) { wxUnusedVar(event); });
    Bind(wxEVT_LEFT_DOWN, &clTreeCtrl::OnMouseLeftDown, this);
    Bind(wxEVT_LEFT_UP, &clTreeCtrl::OnMouseLeftUp, this);
    Bind(wxEVT_LEFT_DCLICK, &clTreeCtrl::OnMouseLeftDClick, this);
    Bind(wxEVT_MOUSEWHEEL, &clTreeCtrl::OnMouseScroll, this);
    Bind(wxEVT_LEAVE_WINDOW, &clTreeCtrl::OnLeaveWindow, this);
    Bind(wxEVT_ENTER_WINDOW, &clTreeCtrl::OnEnterWindow, this);
    Bind(wxEVT_CONTEXT_MENU, &clTreeCtrl::OnContextMenu, this);
    Bind(wxEVT_RIGHT_DOWN, &clTreeCtrl::OnRightDown, this);

    // Initialise default colours
    m_colours.InitDefaults();

    // There is always a header
    clHeaderBar header;
    header.Add("");
    SetHeader(header);
    SetShowHeader(false);
}

clTreeCtrl::~clTreeCtrl()
{
    Unbind(wxEVT_IDLE, &clTreeCtrl::OnIdle, this);
    Unbind(wxEVT_PAINT, &clTreeCtrl::OnPaint, this);
    Unbind(wxEVT_SIZE, &clTreeCtrl::OnSize, this);
    Unbind(wxEVT_ERASE_BACKGROUND, [&](wxEraseEvent& event) { wxUnusedVar(event); });
    Unbind(wxEVT_LEFT_DOWN, &clTreeCtrl::OnMouseLeftDown, this);
    Unbind(wxEVT_LEFT_UP, &clTreeCtrl::OnMouseLeftUp, this);
    Unbind(wxEVT_LEFT_DCLICK, &clTreeCtrl::OnMouseLeftDClick, this);
    Unbind(wxEVT_MOUSEWHEEL, &clTreeCtrl::OnMouseScroll, this);
    Unbind(wxEVT_LEAVE_WINDOW, &clTreeCtrl::OnLeaveWindow, this);
    Unbind(wxEVT_ENTER_WINDOW, &clTreeCtrl::OnEnterWindow, this);
    Unbind(wxEVT_CONTEXT_MENU, &clTreeCtrl::OnContextMenu, this);
    Unbind(wxEVT_RIGHT_DOWN, &clTreeCtrl::OnRightDown, this);
}

void clTreeCtrl::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC pdc(this);
    PrepareDC(pdc);

    wxGCDC dc(pdc);
    // draw the background on the entire client area
    dc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    dc.SetBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
    dc.DrawRectangle(GetClientRect());

    // draw the background on the entire client area
    dc.SetPen(m_colours.GetBgColour());
    dc.SetBrush(m_colours.GetBgColour());
    dc.DrawRectangle(GetClientArea());

    // Set the device origin to the X-offset
    dc.SetDeviceOrigin(-m_firstColumn, 0);
    wxRect clientRect = GetItemsRect();
    if(IsHeaderVisible()) {
        wxRect headerRect = GetClientArea();
        headerRect.SetHeight(m_header.GetHeight());
        m_header.Render(dc, headerRect, m_colours);
    }

    if(!m_model.GetRoot()) {
        // Reset the various items
        SetFirstItemOnScreen(nullptr);
        clRowEntry::Vec_t items;
        m_model.SetOnScreenItems(items);
        return;
    }

    int maxItems = GetNumLineCanFitOnScreen();
    if(!GetFirstItemOnScreen()) { SetFirstItemOnScreen(m_model.GetRoot()); }
    clRowEntry* firstItem = GetFirstItemOnScreen();
    if(!firstItem) { return; }

    int y = clientRect.GetY();
    clRowEntry::Vec_t items;
    m_model.GetNextItems(firstItem, maxItems, items);

    for(size_t i = 0; i < items.size(); ++i) {
        clRowEntry* curitem = items[i];
        if(curitem->IsHidden()) {
            // Set the item's rects into something non visible
            curitem->SetRects(wxRect(-100, -100, 0, 0), wxRect(-100, -100, 0, 0));
            continue;
        }
        wxRect itemRect = wxRect(0, y, clientRect.GetWidth(), m_lineHeight);
        wxRect buttonRect;
        if(curitem->HasChildren()) {
            buttonRect = wxRect((curitem->GetIndentsCount() * GetIndent()), y, m_lineHeight, m_lineHeight);
        }
        curitem->SetRects(itemRect, buttonRect);
        curitem->Render(this, dc, m_colours, i);
        y += m_lineHeight;
    }
    m_model.SetOnScreenItems(items); // Keep track of the visible items
}

void clTreeCtrl::OnSize(wxSizeEvent& event)
{
    event.Skip();
    m_firstColumn = 0;
    UpdateScrollBar();
    Refresh();
}

wxTreeItemId clTreeCtrl::InsertItem(const wxTreeItemId& parent, const wxTreeItemId& previous, const wxString& text,
                                    int image, int selImage, wxTreeItemData* data)
{
    wxTreeItemId item = m_model.InsertItem(parent, previous, text, image, selImage, data);
    DoUpdateHeader(item);
    return item;
}

wxTreeItemId clTreeCtrl::AppendItem(const wxTreeItemId& parent, const wxString& text, int image, int selImage,
                                    wxTreeItemData* data)
{
    wxTreeItemId item = m_model.AppendItem(parent, text, image, selImage, data);
    DoUpdateHeader(item);
    return item;
}

wxTreeItemId clTreeCtrl::AddRoot(const wxString& text, int image, int selImage, wxTreeItemData* data)
{
    wxTreeItemId root = m_model.AddRoot(text, image, selImage, data);
    DoUpdateHeader(root);
    return root;
}

wxTreeItemId clTreeCtrl::GetRootItem() const { return m_model.GetRootItem(); }

void clTreeCtrl::Expand(const wxTreeItemId& item)
{
    CHECK_ITEM_RET(item);
    clRowEntry* child = m_model.ToPtr(item);
    if(!child) return;
    child->SetExpanded(true);
    DoUpdateHeader(item);
    UpdateScrollBar();
    Refresh();
}

void clTreeCtrl::Collapse(const wxTreeItemId& item)
{
    CHECK_ITEM_RET(item);
    clRowEntry* child = m_model.ToPtr(item);
    if(!child) return;
    child->SetExpanded(false);
    UpdateScrollBar();
    DoUpdateHeader(item);
    Refresh();
}

void clTreeCtrl::SelectItem(const wxTreeItemId& item, bool select)
{
    CHECK_ITEM_RET(item);
    if((select && m_model.IsItemSelected(item)) || (!select && !m_model.IsItemSelected(item))) { return; }
    m_model.SelectItem(item, select, false, true);
    Refresh();
}

void clTreeCtrl::OnMouseLeftDown(wxMouseEvent& event)
{
    event.Skip();
    CHECK_ROOT_RET();
    int flags = 0;
    wxPoint pt = DoFixPoint(event.GetPosition());
    wxTreeItemId where = HitTest(pt, flags);
    if(where.IsOk()) {
        if(flags & wxTREE_HITTEST_ONITEMBUTTON) {
            if(IsExpanded(where)) {
                Collapse(where);
            } else {
                Expand(where);
            }
        } else {
            clRowEntry* pNode = m_model.ToPtr(where);
            if(HasStyle(wxTR_MULTIPLE)) {
                if(event.ControlDown()) {
                    // Toggle the selection
                    m_model.SelectItem(where, !pNode->IsSelected(), true);
                } else if(event.ShiftDown()) {
                    // Range selection
                    clRowEntry::Vec_t range;
                    m_model.GetRange(pNode, m_model.ToPtr(m_model.GetSingleSelection()), range);
                    std::for_each(range.begin(), range.end(),
                                  [&](clRowEntry* p) { m_model.AddSelection(wxTreeItemId(p)); });
                } else {
                    // The default, single selection
                    bool has_multiple_selection = (m_model.GetSelectionsCount() > 1);
                    if(!has_multiple_selection && pNode->IsSelected()) {
                        // Nothing to be done here
                    } else if(!has_multiple_selection && !pNode->IsSelected()) {
                        // We got other selection (1), but not this one, select it
                        m_model.SelectItem(where, true, false, true);
                    } else if(has_multiple_selection && !pNode->IsSelected()) {
                        // we got multiple selections and we want to select a new item, what we do
                        // is we clear the current selections and select the new item
                        m_model.SelectItem(where, true, false, true);
                    }
                }

            } else {
                if(GetSelection() != wxTreeItemId(pNode)) {
                    // Select it
                    SelectItem(wxTreeItemId(pNode));
                }
            }
        }
        Refresh();
    }
}

void clTreeCtrl::OnMouseLeftUp(wxMouseEvent& event)
{
    event.Skip();
    int flags = 0;
    wxPoint pt = DoFixPoint(event.GetPosition());
    wxTreeItemId where = HitTest(pt, flags);
    if(where.IsOk() && (flags & wxTREE_HITTEST_ONITEM)) {
        bool has_multiple_selection = (m_model.GetSelectionsCount() > 1);
        clRowEntry* pNode = m_model.ToPtr(where);
        if(has_multiple_selection && pNode->IsSelected() && !event.HasAnyModifiers()) {
            // Select this item while clearing the others
            m_model.SelectItem(where, true, false, true);
            Refresh();
        }
    }
}

wxTreeItemId clTreeCtrl::HitTest(const wxPoint& point, int& flags) const
{
    if(!m_model.GetRoot()) { return wxTreeItemId(); }
    flags = 0;
    for(size_t i = 0; i < m_model.GetOnScreenItems().size(); ++i) {
        const clRowEntry* item = m_model.GetOnScreenItems()[i];
        wxRect buttonRect = item->GetButtonRect();
        // Adjust the coordiantes incase we got h-scroll
        buttonRect.SetX(buttonRect.GetX() - m_firstColumn);
        if(buttonRect.Contains(point)) {
            flags |= wxTREE_HITTEST_ONITEMBUTTON;
            return wxTreeItemId(const_cast<clRowEntry*>(item));
        }
        if(item->GetItemRect().Contains(point)) {
            flags |= wxTREE_HITTEST_ONITEM;
            return wxTreeItemId(const_cast<clRowEntry*>(item));
        }
    }
    return wxTreeItemId();
}

void clTreeCtrl::UnselectAll()
{
    if(!m_model.GetRoot()) { return; }
    m_model.UnselectAll();
    Refresh();
}

wxPoint clTreeCtrl::DoFixPoint(const wxPoint& pt)
{
    wxPoint point = pt;
    return point;
}

void clTreeCtrl::EnsureVisible(const wxTreeItemId& item)
{
    if(!item.IsOk()) { return; }
    // Make sure that all parents of ítem are expanded
    if(!m_model.ExpandToItem(item)) { return; }
    Refresh();
    CallAfter(&clTreeCtrl::DoEnsureVisible, item);
}

void clTreeCtrl::DoEnsureVisible(const wxTreeItemId& item)
{
    // scroll to the item
    if(!item.IsOk()) { return; }
    clRowEntry* pNode = m_model.ToPtr(item);
    if(IsItemVisible(pNode)) { return; }
    EnsureItemVisible(pNode, false); // make it visible at the bottom
    UpdateScrollBar();               // Make sure that the scrollbar fits the view
    Refresh();
}

void clTreeCtrl::OnMouseLeftDClick(wxMouseEvent& event)
{
    event.Skip();
    CHECK_ROOT_RET();

    int flags = 0;
    wxPoint pt = DoFixPoint(event.GetPosition());
    wxTreeItemId where = HitTest(pt, flags);
    if(where.IsOk()) {
        SelectItem(where, true);

        // Let sublclasses handle this first
        wxTreeEvent evt(wxEVT_TREE_ITEM_ACTIVATED);
        evt.SetEventObject(this);
        evt.SetItem(where);
        if(GetEventHandler()->ProcessEvent(evt)) { return; }

        // Process the default action
        if(ItemHasChildren(where)) {
            if(IsExpanded(where)) {
                Collapse(where);
            } else {
                Expand(where);
            }
        }
    }
}

bool clTreeCtrl::IsExpanded(const wxTreeItemId& item) const
{
    if(!item.GetID()) return false;
    clRowEntry* child = m_model.ToPtr(item);
    if(!child) return false;
    return child->IsExpanded();
}

bool clTreeCtrl::ItemHasChildren(const wxTreeItemId& item) const
{
    if(!item.GetID()) return false;
    clRowEntry* child = m_model.ToPtr(item);
    if(!child) return false;
    return child->HasChildren();
}

void clTreeCtrl::SetIndent(int size)
{
    m_model.SetIndentSize(size);
    Refresh();
}

int clTreeCtrl::GetIndent() const { return m_model.GetIndentSize(); }

bool clTreeCtrl::IsEmpty() const { return m_model.IsEmpty(); }

size_t clTreeCtrl::GetChildrenCount(const wxTreeItemId& item, bool recursively) const
{
    if(!item.GetID()) return 0;
    clRowEntry* node = m_model.ToPtr(item);
    return node->GetChildrenCount(recursively);
}

void clTreeCtrl::DeleteChildren(const wxTreeItemId& item)
{
    if(!item.GetID()) return;
    clRowEntry* node = m_model.ToPtr(item);
    node->DeleteAllChildren();
    UpdateScrollBar();
    Refresh();
}

wxTreeItemId clTreeCtrl::GetFirstChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const
{
    if(!item.GetID()) return wxTreeItemId();
    clRowEntry* node = m_model.ToPtr(item);
    const clRowEntry::Vec_t& children = node->GetChildren();
    if(children.empty()) return wxTreeItemId(); // No children
    int* pidx = (int*)&cookie;
    int& idx = (*pidx);
    idx = 1; // the next item
    return wxTreeItemId(children[0]);
}

wxTreeItemId clTreeCtrl::GetNextChild(const wxTreeItemId& item, wxTreeItemIdValue& cookie) const
{
    if(!item.GetID()) return wxTreeItemId();
    int* pidx = (int*)&cookie;
    int& idx = (*pidx);
    clRowEntry* node = m_model.ToPtr(item);
    const clRowEntry::Vec_t& children = node->GetChildren();
    if(idx >= (int)children.size()) return wxTreeItemId();
    wxTreeItemId child(children[idx]);
    idx++;
    return child;
}

wxString clTreeCtrl::GetItemText(const wxTreeItemId& item, size_t col) const
{
    if(!item.GetID()) return "";
    clRowEntry* node = m_model.ToPtr(item);
    return node->GetLabel(col);
}

wxTreeItemData* clTreeCtrl::GetItemData(const wxTreeItemId& item) const
{
    if(!item.GetID()) return nullptr;
    clRowEntry* node = m_model.ToPtr(item);
    return node->GetClientObject();
}

void clTreeCtrl::OnMouseScroll(wxMouseEvent& event)
{
    event.Skip();
    CHECK_ROOT_RET();
    if(!GetFirstItemOnScreen()) { return; }

    // Ignore the first tick (should fix an annoyance on OSX)
    wxDirection direction = (event.GetWheelRotation() > 0) ? wxUP : wxDOWN;
    if(direction != m_lastScrollDir) {
        // Changing direction
        m_lastScrollDir = direction;
        return;
    }

    const clRowEntry::Vec_t& onScreenItems = m_model.GetOnScreenItems();
    if(onScreenItems.empty()) { return; }
    clRowEntry* lastItem = onScreenItems.back();
    clRowEntry* firstItem = onScreenItems.front();

    // Can we scroll any further?
    wxTreeItemId nextItem;
    if(event.GetWheelRotation() > 0) { // Scrolling up
        nextItem = m_model.GetItemAfter(firstItem, true);
    } else {
        nextItem = m_model.GetItemAfter(lastItem, true);
    }
    if(!nextItem.IsOk()) {
        // No more items to draw
        return;
    }
    clRowEntry::Vec_t items;
    if(event.GetWheelRotation() > 0) { // Scrolling up
        m_model.GetPrevItems(GetFirstItemOnScreen(), m_scrollTick, items);
        SetFirstItemOnScreen(items.front()); // first item
        UpdateScrollBar();
    } else {
        m_model.GetNextItems(GetFirstItemOnScreen(), m_scrollTick, items);
        SetFirstItemOnScreen(items.back()); // the last item
        UpdateScrollBar();
    }
    Refresh();
}

const wxBitmap& clTreeCtrl::GetBitmap(size_t index) const
{
    if(index >= m_bitmaps.size()) {
        static wxBitmap emptyBitmap;
        return emptyBitmap;
    }
    return m_bitmaps[index];
}

void clTreeCtrl::SetBitmaps(const std::vector<wxBitmap>& bitmaps)
{
    m_bitmaps = bitmaps;
    int heighestBitmap = 0;
    for(size_t i = 0; i < m_bitmaps.size(); ++i) {
        heighestBitmap = wxMax(heighestBitmap, m_bitmaps[i].GetScaledHeight());
    }
    heighestBitmap += 2 * clRowEntry::Y_SPACER;
    m_lineHeight = wxMax(heighestBitmap, m_lineHeight);
    SetIndent(m_lineHeight);
    Refresh();
}

void clTreeCtrl::ProcessIdle()
{
    if(HasStyle(wxTR_FULL_ROW_HIGHLIGHT)) {
        CHECK_ROOT_RET();
        int flags = 0;
        wxPoint pt = ScreenToClient(::wxGetMousePosition());
        wxTreeItemId item = HitTest(pt, flags);
        if(item.IsOk()) {
            clRowEntry::Vec_t& items = m_model.GetOnScreenItems();
            clRowEntry* hoveredNode = m_model.ToPtr(item);
            bool refreshNeeded = false;
            for(size_t i = 0; i < items.size(); ++i) {
                bool new_state = hoveredNode == items[i];
                bool old_state = items[i]->IsHovered();
                if(!refreshNeeded) { refreshNeeded = (new_state != old_state); }
                items[i]->SetHovered(hoveredNode == items[i]);
            }
            if(refreshNeeded) { Refresh(); }
        }
    }
}

void clTreeCtrl::OnLeaveWindow(wxMouseEvent& event)
{
    event.Skip();
    CHECK_ROOT_RET();
    clRowEntry::Vec_t& items = m_model.GetOnScreenItems();
    for(size_t i = 0; i < items.size(); ++i) {
        items[i]->SetHovered(false);
    }
    Update();
}

void clTreeCtrl::SetColours(const clColours& colours)
{
    m_colours = colours;
    Refresh();
}

void clTreeCtrl::ExpandAllChildren(const wxTreeItemId& item)
{
    wxBusyCursor bc;
    m_model.ExpandAllChildren(item);
    UpdateScrollBar();
    DoUpdateHeader(item);
    Refresh();
}

void clTreeCtrl::CollapseAllChildren(const wxTreeItemId& item)
{
    wxBusyCursor bc;
    m_model.CollapseAllChildren(item);
    SetFirstItemOnScreen(m_model.ToPtr(item));
    SelectItem(item);
    UpdateScrollBar();
    DoUpdateHeader(item);
    Refresh();
}

wxTreeItemId clTreeCtrl::GetFirstVisibleItem() const
{
    const clRowEntry::Vec_t& items = m_model.GetOnScreenItems();
    if(items.empty()) { return wxTreeItemId(); }
    return wxTreeItemId(items[0]);
}

wxTreeItemId clTreeCtrl::GetNextVisible(const wxTreeItemId& item) const { return DoGetSiblingVisibleItem(item, true); }
wxTreeItemId clTreeCtrl::GetPrevVisible(const wxTreeItemId& item) const { return DoGetSiblingVisibleItem(item, false); }

wxTreeItemId clTreeCtrl::DoGetSiblingVisibleItem(const wxTreeItemId& item, bool next) const
{
    if(!item.IsOk()) { return wxTreeItemId(); }
    const clRowEntry::Vec_t& items = m_model.GetOnScreenItems();
    if(items.empty()) { return wxTreeItemId(); }
    clRowEntry* from = m_model.ToPtr(item);
    clRowEntry::Vec_t::const_iterator iter =
        std::find_if(items.begin(), items.end(), [&](clRowEntry* p) { return p == from; });
    if(next && (iter == items.end())) { return wxTreeItemId(); }
    if(!next && (iter == items.begin())) { return wxTreeItemId(); }
    if(next) {
        ++iter;
        if(iter == items.end()) { return wxTreeItemId(); }
    } else {
        --iter;
        if(iter == items.begin()) { return wxTreeItemId(); }
    }
    return wxTreeItemId(*iter);
}

wxTreeItemId clTreeCtrl::GetSelection() const { return m_model.GetSingleSelection(); }

wxTreeItemId clTreeCtrl::GetFocusedItem() const { return GetSelection(); }

size_t clTreeCtrl::GetSelections(wxArrayTreeItemIds& selections) const
{
    const clRowEntry::Vec_t& items = m_model.GetSelections();
    if(items.empty()) { return 0; }
    std::for_each(items.begin(), items.end(), [&](clRowEntry* item) { selections.Add(wxTreeItemId(item)); });
    return selections.size();
}

bool clTreeCtrl::DoKeyDown(const wxKeyEvent& event)
{
    if(!m_model.GetRoot()) {
        // we didnt process this event, carry on
        return true;
    }
    wxTreeItemId selectedItem = GetSelection();
    if(!selectedItem.IsOk()) { return true; }

    // Let the user chance to process this first
    wxTreeEvent evt(wxEVT_TREE_KEY_DOWN);
    evt.SetEventObject(this);
    evt.SetKeyEvent(event);
    evt.SetItem(selectedItem);
    if(GetEventHandler()->ProcessEvent(evt)) { return true; }

    if(event.GetKeyCode() == WXK_LEFT) {
        if(m_model.ToPtr(selectedItem)->IsExpanded()) {
            Collapse(selectedItem);
            return true;
        }
    } else if(event.GetKeyCode() == WXK_RIGHT) {
        if(!m_model.ToPtr(selectedItem)->IsExpanded()) {
            Expand(selectedItem);
            return true;
        }
        //    } else if(event.GetKeyCode() == WXK_NUMPAD_DELETE || event.GetKeyCode() == WXK_DELETE) {
        //        // Delete the item (this will also fire
        //        // wxEVT_TREE_DELETE_ITEM
        //        Delete(selectedItem);
        //        return true;
    } else if(event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
        wxTreeEvent evt(wxEVT_TREE_ITEM_ACTIVATED);
        evt.SetEventObject(this);
        evt.SetItem(selectedItem);
        GetEventHandler()->ProcessEvent(evt);
        return true;
    }
    // We didnt process this event, continue on processing it
    return false;
}

bool clTreeCtrl::IsItemVisible(clRowEntry* item) const
{
    const clRowEntry::Vec_t& onScreenItems = m_model.GetOnScreenItems();
    return (std::find_if(onScreenItems.begin(), onScreenItems.end(), [&](clRowEntry* p) { return p == item; }) !=
            onScreenItems.end());
}

void clTreeCtrl::EnsureItemVisible(clRowEntry* item, bool fromTop)
{
    CHECK_PTR_RET(item)
    if(IsItemVisible(item)) { return; }
    if(fromTop) {
        SetFirstItemOnScreen(item);
    } else {
        int max_lines_on_screen = GetNumLineCanFitOnScreen();
        clRowEntry::Vec_t items;
        m_model.GetPrevItems(item, max_lines_on_screen, items);
        if(items.empty()) { return; }
        SetFirstItemOnScreen(items[0]);
    }
}

int clTreeCtrl::GetNumLineCanFitOnScreen() const
{
    wxRect clientRect = GetItemsRect();
    int max_lines_on_screen = ceil(clientRect.GetHeight() / m_lineHeight);
    return max_lines_on_screen;
}

void clTreeCtrl::Delete(const wxTreeItemId& item)
{
    CHECK_ITEM_RET(item);
    // delete the item + its children
    // fires event
    m_model.DeleteItem(item);
    UpdateScrollBar();
    Refresh();
}

void clTreeCtrl::SetItemData(const wxTreeItemId& item, wxTreeItemData* data)
{
    clRowEntry* node = m_model.ToPtr(item);
    CHECK_PTR_RET(node);
    node->SetClientData(data);
}

void clTreeCtrl::SetItemBackgroundColour(const wxTreeItemId& item, const wxColour& colour, size_t col)
{
    clRowEntry* node = m_model.ToPtr(item);
    CHECK_PTR_RET(node);
    node->SetBgColour(colour, col);
    Refresh();
}

wxColour clTreeCtrl::GetItemBackgroundColour(const wxTreeItemId& item, size_t col) const
{
    clRowEntry* node = m_model.ToPtr(item);
    if(!node) { return wxNullColour; }
    return node->GetBgColour(col);
}

void clTreeCtrl::SetItemTextColour(const wxTreeItemId& item, const wxColour& colour, size_t col)
{
    clRowEntry* node = m_model.ToPtr(item);
    CHECK_PTR_RET(node);
    node->SetTextColour(colour, col);
    Refresh();
}

wxColour clTreeCtrl::GetItemTextColour(const wxTreeItemId& item, size_t col) const
{
    clRowEntry* node = m_model.ToPtr(item);
    if(!node) { return wxNullColour; }
    return node->GetTextColour(col);
}

void clTreeCtrl::SetItemText(const wxTreeItemId& item, const wxString& text, size_t col)
{
    clRowEntry* node = m_model.ToPtr(item);
    CHECK_PTR_RET(node);
    node->SetLabel(text, col);
    Refresh();
}

void clTreeCtrl::SetItemBold(const wxTreeItemId& item, bool bold, size_t col)
{
    clRowEntry* node = m_model.ToPtr(item);
    CHECK_PTR_RET(node);
    wxFont f = node->GetFont(col);
    if(!f.IsOk()) { f = GetDefaultFont(); }
    f.SetWeight(bold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL);
    node->SetFont(f, col);

    // Changing font can change the width of the text, so update the header if needed
    DoUpdateHeader(item);
    Refresh();
}

void clTreeCtrl::SetItemFont(const wxTreeItemId& item, const wxFont& font, size_t col)
{
    clRowEntry* node = m_model.ToPtr(item);
    CHECK_PTR_RET(node);
    node->SetFont(font, col);
    Refresh();
}

wxFont clTreeCtrl::GetItemFont(const wxTreeItemId& item, size_t col) const
{
    clRowEntry* node = m_model.ToPtr(item);
    if(!node) { return wxNullFont; }
    return node->GetFont(col);
}

void clTreeCtrl::OnContextMenu(wxContextMenuEvent& event)
{
    event.Skip();
    CHECK_ROOT_RET();
    int flags = 0;
    wxPoint pt = ScreenToClient(::wxGetMousePosition());
    wxTreeItemId item = HitTest(pt, flags);
    if(item.IsOk()) {
        SelectItem(item, true);
        wxTreeEvent evt(wxEVT_TREE_ITEM_MENU);
        evt.SetItem(item);
        evt.SetEventObject(this);
        GetEventHandler()->ProcessEvent(evt);
    }
}

void clTreeCtrl::OnRightDown(wxMouseEvent& event)
{
    event.Skip();
    CHECK_ROOT_RET();
    int flags = 0;
    wxPoint pt = DoFixPoint(event.GetPosition());
    wxTreeItemId where = HitTest(pt, flags);
    if(where.IsOk()) {
        wxTreeEvent evt(wxEVT_TREE_ITEM_RIGHT_CLICK);
        evt.SetEventObject(this);
        evt.SetItem(where);
        event.Skip(false);
        if(GetEventHandler()->ProcessEvent(evt)) { return; }
        event.Skip();
    }
}

clRowEntry* clTreeCtrl::GetFirstItemOnScreen() { return m_model.GetFirstItemOnScreen(); }

void clTreeCtrl::SetFirstItemOnScreen(clRowEntry* item) { m_model.SetFirstItemOnScreen(item); }

void clTreeCtrl::SetSortFunction(const std::function<bool(const wxTreeItemId&, const wxTreeItemId&)>& CompareFunc)
{
    m_model.SetSortFunction(CompareFunc);
}

void clTreeCtrl::UpdateScrollBar()
{
    {
        // V-scrollbar
        wxRect rect = GetItemsRect();
        int thumbSize = (rect.GetHeight() / m_lineHeight); // Number of lines can be drawn
        int pageSize = (thumbSize - 1);
        int rangeSize = m_model.GetExpandedLines();
        int position = m_model.GetItemIndex(m_model.GetFirstItemOnScreen());
        UpdateVScrollBar(position, thumbSize, rangeSize, pageSize);
    }
    {
        // H-scrollbar
        int thumbSize = GetClientArea().GetWidth();
        int pageSize = (thumbSize - 1);
        int rangeSize = IsEmpty() ? 0 : m_header.GetWidth();
        int position = m_firstColumn;
        UpdateHScrollBar(position, thumbSize, rangeSize, pageSize);
    }
}

void clTreeCtrl::ScrollToRow(int firstLine)
{
    clRowEntry* newTopLine = nullptr;
    // Thumbtrack sends the top-line index in event.GetPosition()
    newTopLine = m_model.GetItemFromIndex(firstLine);
    if(newTopLine) { SetFirstItemOnScreen(m_model.ToPtr(newTopLine)); }
    Refresh();
}

void clTreeCtrl::ScollToColumn(int firstColumn)
{
    m_firstColumn = firstColumn;
    Refresh();
}

void clTreeCtrl::ScrollColumns(int steps, wxDirection direction)
{
    if((steps == 0) && (direction == wxLEFT)) {
        m_firstColumn = 0;
    } else if((steps == 0) && (direction == wxRIGHT)) {
        m_firstColumn = m_header.GetWidth();
    } else {
        int max_width = m_header.GetWidth();
        int firstColumn = m_firstColumn + ((direction == wxRIGHT) ? steps : -steps);
        if(firstColumn < 0) { firstColumn = 0; }
        int pageSize = GetClientArea().GetWidth();
        if((firstColumn + pageSize) > max_width) { firstColumn = max_width - pageSize; }
        m_firstColumn = firstColumn;
    }
    Refresh();
}

void clTreeCtrl::ScrollRows(int steps, wxDirection direction)
{
    // Process the rest of the scrolling events here
    wxTreeItemId nextSelection;
    bool fromTop = false;
    if(steps == 0) {
        // Top or Bottom
        if(direction == wxUP) {
            if(IsRootHidden()) {
                nextSelection = wxTreeItemId(m_model.ToPtr(GetRootItem())->GetFirstChild());
            } else {
                nextSelection = GetRootItem();
            }
            fromTop = true;
        } else {
            // Find the last item, it does not matter if the root is hidden
            clRowEntry* node = m_model.ToPtr(GetRootItem());
            while(node->GetLastChild()) {
                node = node->GetLastChild();
            }
            nextSelection = wxTreeItemId(node);
        }
    } else {
        nextSelection = DoScrollLines(steps, direction == wxUP, GetFocusedItem(), false);
        fromTop = (direction == wxUP);
    }

    if(::wxGetKeyState(WXK_SHIFT) && HasStyle(wxTR_MULTIPLE)) {
        m_model.AddSelection(nextSelection);
    } else {
        SelectItem(nextSelection);
    }
    EnsureItemVisible(m_model.ToPtr(nextSelection), fromTop);
    Refresh();
    UpdateScrollBar();
}

void clTreeCtrl::SelectChildren(const wxTreeItemId& item)
{
    CHECK_ITEM_RET(item);
    if(!(GetTreeStyle() & wxTR_MULTIPLE)) {
        // Can only be used with multiple selection trees
        return;
    }
    m_model.SelectChildren(item);
    Refresh();
}

wxTreeItemId clTreeCtrl::GetNextSibling(const wxTreeItemId& item) const
{
    CHECK_ITEM_RET_INVALID_ITEM(item);
    return m_model.GetNextSibling(m_model.ToPtr(item));
}

wxTreeItemId clTreeCtrl::GetPrevSibling(const wxTreeItemId& item) const
{
    CHECK_ITEM_RET_INVALID_ITEM(item);
    return m_model.GetPrevSibling(m_model.ToPtr(item));
}

wxTreeItemId clTreeCtrl::DoScrollLines(int numLines, bool up, wxTreeItemId from, bool selectIt)
{
    wxTreeItemId selectedItem = from;
    if(!selectedItem.IsOk()) { return wxTreeItemId(); }

    int counter = 0;
    wxTreeItemId nextItem = selectedItem;
    while(nextItem.IsOk() && (counter < numLines)) {
        if(up) {
            nextItem = m_model.GetItemBefore(selectedItem, true);
        } else {
            nextItem = m_model.GetItemAfter(selectedItem, true);
        }
        if(nextItem.IsOk()) { selectedItem = nextItem; }
        counter++;
    }
    if(selectIt) { SelectItem(selectedItem); }
    return selectedItem;
}

void clTreeCtrl::EnableStyle(int style, bool enable, bool refresh)
{
    if(!m_model.GetRoot()) { return; }
    if(enable) {
        m_treeStyle |= style;
    } else {
        m_treeStyle &= ~style;
    }

    // When changing the wxTR_HIDE_ROOT style
    // we need to fix the indentation for each node in the tree
    if(style == wxTR_HIDE_ROOT) {
        m_model.GetRoot()->SetHidden(IsRootHidden());
        std::function<bool(clRowEntry*, bool)> UpdateIndentsFunc = [=](clRowEntry* node, bool visibleItem) {
            wxUnusedVar(visibleItem);
            if(node->GetParent()) { node->SetIndentsCount(node->GetParent()->GetIndentsCount() + 1); }
            return true;
        };
        clTreeNodeVisitor V;
        V.Visit(m_model.GetRoot(), false, UpdateIndentsFunc);
        wxTreeItemId newRoot(m_model.GetRoot()->GetFirstChild());
        if(newRoot) { DoUpdateHeader(newRoot); }
    }
    if(refresh) { Refresh(); }
}

wxTreeItemId clTreeCtrl::GetItemParent(const wxTreeItemId& item) const
{
    CHECK_ITEM_RET_INVALID_ITEM(item);
    return wxTreeItemId(m_model.ToPtr(item)->GetParent());
}

void clTreeCtrl::SetItemImage(const wxTreeItemId& item, int imageId, int openImageId, size_t col)
{
    clRowEntry* node = m_model.ToPtr(item);
    CHECK_PTR_RET(node);
    node->SetBitmapIndex(imageId, col);
    node->SetBitmapSelectedIndex(openImageId, col);
    Refresh();
}

int clTreeCtrl::GetItemImage(const wxTreeItemId& item, bool selectedImage, size_t col) const
{
    if(!item.GetID()) { return wxNOT_FOUND; }
    clRowEntry* node = m_model.ToPtr(item);
    return selectedImage ? node->GetBitmapSelectedIndex(col) : node->GetBitmapIndex(col);
}

void clTreeCtrl::OnEnterWindow(wxMouseEvent& event) { event.Skip(); }

wxRect clTreeCtrl::GetItemsRect() const
{
    // Return the rectangle taking header into consideration
    int yOffset = m_header.GetHeight();
    wxRect clientRect = GetClientArea();
    clientRect.SetY(yOffset);
    clientRect.SetHeight(clientRect.GetHeight() - yOffset);
    return clientRect;
}

void clTreeCtrl::SetHeader(const clHeaderBar& header)
{
    wxASSERT_MSG(IsEmpty(), "SetHeader can not be called on a non empty tree");
    m_header = header;
    SetShowHeader(true);
}

wxSize clTreeCtrl::GetTextSize(const wxString& label) const
{
    wxDC& dc = GetTempDC();
    wxFont font = GetDefaultFont();
    dc.SetFont(font);
    wxSize textSize = dc.GetTextExtent("label");
    return textSize;
}

void clTreeCtrl::DoUpdateHeader(const wxTreeItemId& item)
{
    // do we have header?
    if(m_header.empty()) { return; }

    clRowEntry* pNode = m_model.ToPtr(item);
    CHECK_PTR_RET(pNode);
    wxDC& dc = GetTempDC();

    // Use bold font, to get the maximum width needed
    for(size_t i = 0; i < m_header.size(); ++i) {
        int row_width = pNode->CalcItemWidth(dc, m_lineHeight, i);
        m_header.UpdateColWidthIfNeeded(i, row_width, false);
    }
}

bool clTreeCtrl::IsVisible(const wxTreeItemId& item) const { return m_model.IsVisible(item); }

bool clTreeCtrl::IsSelected(const wxTreeItemId& item) const { return m_model.IsItemSelected(item); }

wxTreeItemId clTreeCtrl::GetRow(const wxPoint& pt) const
{
    int flags = 0;
    wxTreeItemId item = HitTest(pt, flags);
    if(item.IsOk() && (flags & wxTREE_HITTEST_ONITEM)) { return item; }
    return wxTreeItemId();
}

void clTreeCtrl::SetShowHeader(bool b)
{
    m_header.SetHideHeaders(!b);
    Refresh();
}

bool clTreeCtrl::IsHeaderVisible() const { return !m_header.IsHideHeaders(); }
