#include "clHeaderBar.h"
#include <wx/dcmemory.h>
#include <wx/font.h>
#include <wx/settings.h>

clHeaderBar::clHeaderBar() {}

clHeaderBar::~clHeaderBar() {}

size_t clHeaderBar::GetHeight() const
{
    if(m_columns.empty()) { return 0; }
    return m_columns[0].GetRect().GetHeight();
}

void clHeaderBar::push_back(const clHeaderItem& item)
{
    m_columns.push_back(item);
    DoUpdateSize();
}

void clHeaderBar::DoUpdateSize()
{
    wxSize fixedText = GetTextSize("Tp");
    int xx = 0;
    for(size_t i = 0; i < m_columns.size(); ++i) {
        clHeaderItem& item = m_columns[i];
        wxSize textSize = GetTextSize(item.GetLabel());
        wxRect headerRect(xx, 0, textSize.GetWidth() + (2 * clHeaderItem::X_SPACER),
            fixedText.GetHeight() + (2 * clHeaderItem::Y_SPACER));
        item.SetRect(headerRect);
        xx += item.GetRect().GetWidth();
    }
}

wxSize clHeaderBar::GetTextSize(const wxString& label) const
{
    wxBitmap bmp(1, 1);
    wxMemoryDC memDC(bmp);
    wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    memDC.SetFont(font);
    wxSize textSize = memDC.GetTextExtent(label);
    return textSize;
}

void clHeaderBar::Render(wxDC& dc, const wxRect& rect, const clColours& colours)
{
    wxColour barBgColour = colours.GetBgColour().ChangeLightness(120);
    clColours _colours = colours;
    _colours.SetBgColour(barBgColour);

    dc.SetPen(_colours.GetBgColour());
    dc.SetBrush(_colours.GetBgColour());
    dc.DrawRectangle(rect);
    for(size_t i = 0; i < size(); ++i) {
        bool is_last = (i == (size() - 1));
        Item(i).Render(dc, _colours);
        if(!is_last) {
            dc.SetPen(_colours.GetHoverBgColour());
            dc.DrawLine(Item(i).GetRect().GetTopRight(), Item(i).GetRect().GetBottomRight());
        }
    }
}

void clHeaderBar::UpdateColWidthIfNeeded(size_t col, size_t width, bool force)
{
    if(col >= m_columns.size()) { return; }
    clHeaderItem& column = m_columns[col];
    column.SetWidth(force ? width : wxMax(column.GetWidth(), width));
    
    // Update the offsets
    int xx = 0;
    for(size_t i=0; i<m_columns.size(); ++i) {
        clHeaderItem& column = m_columns[i];
        column.SetX(xx);
        xx += column.GetWidth();
    }
}
