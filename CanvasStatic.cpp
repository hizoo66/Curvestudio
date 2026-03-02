#include "pch.h"
#include "CanvasStatic.h"
#include "CurvestudioDlg.h"   // PointD 정의를 쓰려고 포함

BEGIN_MESSAGE_MAP(CCanvasStatic, CStatic)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CCanvasStatic::OnPaint()
{
    CPaintDC dc(this);

    CRect rc;
    GetClientRect(&rc);

    // 배경 + 테두리
    dc.FillSolidRect(rc, RGB(255, 255, 255));
    dc.Rectangle(rc);

    if (!m_pPoints) return;

    // 점 그리기 (여기는 캔버스 내부 좌표 그대로 사용)
    for (size_t i = 0; i < m_pPoints->size(); i++)
    {
        int x = (int)(*m_pPoints)[i].x;
        int y = (int)(*m_pPoints)[i].y;

        int r = 3;
        dc.Ellipse(x - r, y - r, x + r, y + r);
    }

    // 점 사이 직선 연결 (제어 다각형)
    for (size_t i = 1; i < m_pPoints->size(); i++)
    {
        dc.MoveTo(
            (int)(*m_pPoints)[i - 1].x,
            (int)(*m_pPoints)[i - 1].y
        );

        dc.LineTo(
            (int)(*m_pPoints)[i].x,
            (int)(*m_pPoints)[i].y
        );
    }
    if (m_curveType == CURVE_BEZIER && m_pPoints && m_pPoints->size() >= 2)
    {
        int n = (int)m_pPoints->size() - 1;

        auto Binomial = [](int n, int k)
            {
                double res = 1;
                for (int i = 1; i <= k; i++)
                    res = res * (n - k + i) / i;
                return res;
            };

        const int STEPS = 200;

        for (int step = 0; step <= STEPS; step++)
        {
            double t = (double)step / STEPS;
            double x = 0;
            double y = 0;

            for (int i = 0; i <= n; i++)
            {
                double coeff =
                    Binomial(n, i) *
                    pow(1 - t, n - i) *
                    pow(t, i);

                x += coeff * (*m_pPoints)[i].x;
                y += coeff * (*m_pPoints)[i].y;
            }

            if (step == 0) dc.MoveTo((int)x, (int)y);
            else           dc.LineTo((int)x, (int)y);
        }
    }

    if (m_curveType == CURVE_NSPLINE && m_pPoints->size() >= 2)
    {
        int n = (int)m_pPoints->size();

        for (int i = 0; i < n - 1; i++)
        {
            PointD p0 = (i == 0) ? (*m_pPoints)[i] : (*m_pPoints)[i - 1];
            PointD p1 = (*m_pPoints)[i];
            PointD p2 = (*m_pPoints)[i + 1];
            PointD p3 = (i + 2 < n) ? (*m_pPoints)[i + 2] : (*m_pPoints)[i + 1];

            for (int step = 0; step <= 50; step++)
            {
                double t = step / 50.0;
                double t2 = t * t;
                double t3 = t2 * t;

                double x =
                    0.5 * ((2 * p1.x) +
                        (-p0.x + p2.x) * t +
                        (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * t2 +
                        (-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3);

                double y =
                    0.5 * ((2 * p1.y) +
                        (-p0.y + p2.y) * t +
                        (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * t2 +
                        (-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3);

                if (step == 0 && i == 0)
                    dc.MoveTo((int)x, (int)y);
                else
                    dc.LineTo((int)x, (int)y);
            }
        }
    }

    if (m_curveType == CURVE_BSPLINE && m_pPoints->size() >= 4)
    {
        for (int i = 0; i < (int)m_pPoints->size() - 3; i++)
        {
            PointD p0 = (*m_pPoints)[i];
            PointD p1 = (*m_pPoints)[i + 1];
            PointD p2 = (*m_pPoints)[i + 2];
            PointD p3 = (*m_pPoints)[i + 3];

            for (int step = 0; step <= 50; step++)
            {
                double t = step / 50.0;
                double t2 = t * t;
                double t3 = t2 * t;

                double x =
                    ((-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * t3
                        + (3 * p0.x - 6 * p1.x + 3 * p2.x) * t2
                        + (-3 * p0.x + 3 * p2.x) * t
                        + (p0.x + 4 * p1.x + p2.x)) / 6.0;

                double y =
                    ((-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * t3
                        + (3 * p0.y - 6 * p1.y + 3 * p2.y) * t2
                        + (-3 * p0.y + 3 * p2.y) * t
                        + (p0.y + 4 * p1.y + p2.y)) / 6.0;

                if (step == 0 && i == 0)
                    dc.MoveTo((int)x, (int)y);
                else
                    dc.LineTo((int)x, (int)y);
            }
        }
    }
}

void CCanvasStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (!m_pPoints) return;

    const int RADIUS = 6;

    for (int i = 0; i < (int)m_pPoints->size(); i++)
    {
        double dx = point.x - (*m_pPoints)[i].x;
        double dy = point.y - (*m_pPoints)[i].y;

        if (sqrt(dx * dx + dy * dy) <= RADIUS)
        {
            m_dragIndex = i;
            m_dragging = true;
            SetCapture();
            break;
        }
    }

    CStatic::OnLButtonDown(nFlags, point);
}

void CCanvasStatic::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_dragging && m_pPoints && m_dragIndex >= 0)
    {
        (*m_pPoints)[m_dragIndex].x = point.x;
        (*m_pPoints)[m_dragIndex].y = point.y;

        Invalidate();
    }

    CStatic::OnMouseMove(nFlags, point);
}

void CCanvasStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_dragging)
    {
        m_dragging = false;
        m_dragIndex = -1;
        ReleaseCapture();
    }

    CStatic::OnLButtonUp(nFlags, point);
}