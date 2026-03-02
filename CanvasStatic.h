#pragma once
#include <vector>

struct PointD; // CurvestudioDlg.h에 이미 있으니 전방선언만
enum CurveType;

class CCanvasStatic : public CStatic
{
public:
    CCanvasStatic() = default;

    void SetPoints(std::vector<PointD>* p) { m_pPoints = p; }

    void SetCurveType(CurveType type) 
    {
        m_curveType = type;
    }
protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
    std::vector<PointD>* m_pPoints = nullptr;
    CurveType m_curveType;
    bool m_dragging = false;
    int m_dragIndex = -1;
};