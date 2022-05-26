#include "renderGDI.h"
#include "renderD3D.h"

CRender* CRender::create(RenderType type)
{
    switch (type)
    {
    case D3D_RENDER:
        return new CRenderD3D;
        break;
    default:
        return new CRenderGDI;
        break;
    }
}
