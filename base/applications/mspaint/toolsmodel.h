/*
 * PROJECT:     PAINT for ReactOS
 * LICENSE:     LGPL
 * FILE:        base/applications/mspaint/toolsmodel.h
 * PURPOSE:     Keep track of tool parameters, notify listeners
 * PROGRAMMERS: Benedikt Freisen
 */

#pragma once

enum TOOLTYPE
{
    TOOL_FREESEL  =  1,
    TOOL_RECTSEL  =  2,
    TOOL_RUBBER   =  3,
    TOOL_FILL     =  4,
    TOOL_COLOR    =  5,
    TOOL_ZOOM     =  6,
    TOOL_PEN      =  7,
    TOOL_BRUSH    =  8,
    TOOL_AIRBRUSH =  9,
    TOOL_TEXT     = 10,
    TOOL_LINE     = 11,
    TOOL_BEZIER   = 12,
    TOOL_RECT     = 13,
    TOOL_SHAPE    = 14,
    TOOL_ELLIPSE  = 15,
    TOOL_RRECT    = 16,
    TOOL_MAX = TOOL_RRECT,
};

/* CLASSES **********************************************************/

struct ToolBase
{
    TOOLTYPE m_tool;
    HDC m_hdc;
    COLORREF m_fg, m_bg;
    static INT s_pointSP;
    static POINT s_pointStack[256];

    ToolBase(TOOLTYPE tool) : m_tool(tool), m_hdc(NULL) { }
    virtual ~ToolBase() { }

    virtual void OnButtonDown(BOOL bLeftButton, LONG x, LONG y, BOOL bDoubleClick) { }
    virtual void OnMouseMove(BOOL bLeftButton, LONG x, LONG y) { }
    virtual void OnButtonUp(BOOL bLeftButton, LONG x, LONG y) { }

    virtual void OnCancelDraw();
    virtual void OnFinishDraw();

    virtual void OnDrawOverlayOnImage(HDC hdc) { }
    virtual void OnDrawOverlayOnCanvas(HDC hdc) { }

    void beginEvent();
    void endEvent();
    void reset();

    static ToolBase* createToolObject(TOOLTYPE type);

protected:
    void OnDrawSelectionOnCanvas(HDC hdc);
};

class ToolsModel
{
private:
    int m_lineWidth;
    int m_shapeStyle;
    int m_brushStyle;
    TOOLTYPE m_activeTool;
    TOOLTYPE m_oldActiveTool;
    int m_airBrushWidth;
    int m_rubberRadius;
    BOOL m_transpBg;
    int m_zoom;
    ToolBase* m_tools[TOOL_MAX + 1];
    ToolBase *m_pToolObject;

    ToolBase *GetOrCreateTool(TOOLTYPE nTool);

public:
    ToolsModel();
    ~ToolsModel();

    BOOL IsSelection() const;
    int GetLineWidth() const;
    void SetLineWidth(int nLineWidth);
    int GetShapeStyle() const;
    void SetShapeStyle(int nShapeStyle);
    int GetBrushStyle() const;
    void SetBrushStyle(int nBrushStyle);
    TOOLTYPE GetActiveTool() const;
    TOOLTYPE GetOldActiveTool() const;
    void SetActiveTool(TOOLTYPE nActiveTool);
    int GetAirBrushWidth() const;
    void SetAirBrushWidth(int nAirBrushWidth);
    int GetRubberRadius() const;
    void SetRubberRadius(int nRubberRadius);
    BOOL IsBackgroundTransparent() const;
    void SetBackgroundTransparent(BOOL bTransparent);
    int GetZoom() const;
    void SetZoom(int nZoom);

    void OnButtonDown(BOOL bLeftButton, LONG x, LONG y, BOOL bDoubleClick);
    void OnMouseMove(BOOL bLeftButton, LONG x, LONG y);
    void OnButtonUp(BOOL bLeftButton, LONG x, LONG y);
    void OnCancelDraw();
    void OnFinishDraw();
    void OnDrawOverlayOnImage(HDC hdc);
    void OnDrawOverlayOnCanvas(HDC hdc);

    void resetTool();
    void selectAll();

    void NotifyToolChanged();
    void NotifyToolSettingsChanged();
    void NotifyZoomChanged();
};

extern ToolsModel toolsModel;

static inline int Zoomed(int xy)
{
    return xy * toolsModel.GetZoom() / 1000;
}

static inline int UnZoomed(int xy)
{
    return xy * 1000 / toolsModel.GetZoom();
}

static inline void Zoomed(POINT& pt)
{
    pt.x = Zoomed(pt.x);
    pt.y = Zoomed(pt.y);
}

static inline void Zoomed(RECT& rc)
{
    rc.left = Zoomed(rc.left);
    rc.top = Zoomed(rc.top);
    rc.right = Zoomed(rc.right);
    rc.bottom = Zoomed(rc.bottom);
}

static inline void UnZoomed(POINT& pt)
{
    pt.x = UnZoomed(pt.x);
    pt.y = UnZoomed(pt.y);
}

static inline void UnZoomed(RECT& rc)
{
    rc.left = UnZoomed(rc.left);
    rc.top = UnZoomed(rc.top);
    rc.right = UnZoomed(rc.right);
    rc.bottom = UnZoomed(rc.bottom);
}
