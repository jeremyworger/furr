/*
  ImageMagick Drawing Wand API.
*/
#ifndef _MAGICK_DRAWING_WAND_H
#define _MAGICK_DRAWING_WAND_H

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#include "pixel_wand.h"

typedef struct _DrawingWand
  DrawingWand;

extern WandExport char
  *DrawGetClipPath(const DrawingWand *),
  *DrawGetException(const DrawingWand *,ExceptionType *),
  *DrawGetFont(const DrawingWand *),
  *DrawGetFontFamily(const DrawingWand *),
  *DrawGetTextEncoding(const DrawingWand *);

extern WandExport ClipPathUnits
  DrawGetClipUnits(const DrawingWand *);

extern WandExport DecorationType
  DrawGetTextDecoration(const DrawingWand *);

extern WandExport double
  DrawGetFillOpacity(const DrawingWand *),
  DrawGetFontSize(const DrawingWand *),
  *DrawGetStrokeDashArray(const DrawingWand *,unsigned long *),
  DrawGetStrokeDashOffset(const DrawingWand *),
  DrawGetStrokeOpacity(const DrawingWand *),
  DrawGetStrokeWidth(const DrawingWand *);

extern WandExport DrawInfo
  *PeekDrawingWand(const DrawingWand *);

extern WandExport DrawingWand
  *DrawAllocateWand(const DrawInfo *,Image *),
  *NewDrawingWand(void);

extern WandExport FillRule
  DrawGetClipRule(const DrawingWand *),
  DrawGetFillRule(const DrawingWand *);

extern WandExport GravityType
  DrawGetGravity(const DrawingWand *);

extern WandExport LineCap
  DrawGetStrokeLineCap(const DrawingWand *);

extern WandExport LineJoin
  DrawGetStrokeLineJoin(const DrawingWand *);

extern WandExport MagickBooleanType
  DrawGetStrokeAntialias(const DrawingWand *),
  DrawGetTextAntialias(const DrawingWand *),
  DrawRender(DrawingWand *);

extern WandExport StretchType
  DrawGetFontStretch(const DrawingWand *);

extern WandExport StyleType
  DrawGetFontStyle(const DrawingWand *);

extern WandExport unsigned long
  DrawGetFontWeight(const DrawingWand *),
  DrawGetStrokeMiterLimit(const DrawingWand *);

extern WandExport void
  DrawAffine(DrawingWand *,const AffineMatrix *),
  DrawAnnotation(DrawingWand *,const double,const double,const unsigned char *),
  DrawArc(DrawingWand *,const double,const double,const double,const double,
    const double,const double),
  DrawBezier(DrawingWand *,const unsigned long,const PointInfo *),
  DrawCircle(DrawingWand *,const double,const double,const double,const double),
  DrawColor(DrawingWand *,const double,const double,const PaintMethod),
  DrawComment(DrawingWand *,const char *),
  DestroyDrawingWand(DrawingWand *),
  DrawEllipse(DrawingWand *,const double,const double,const double,const double,
    const double,const double),
  DrawComposite(DrawingWand *,const CompositeOperator,const double,const double,
    const double,const double,const Image *),
  DrawGetFillColor(const DrawingWand *,PixelWand *),
  DrawGetStrokeColor(const DrawingWand *,PixelWand *),
  DrawGetTextUnderColor(const DrawingWand *,PixelWand *),
  DrawLine(DrawingWand *,const double, const double,const double,const double),
  DrawMatte(DrawingWand *,const double,const double,const PaintMethod),
  DrawPathClose(DrawingWand *),
  DrawPathCurveToAbsolute(DrawingWand *,const double,const double,const double,
    const double,const double,const double),
  DrawPathCurveToRelative(DrawingWand *,const double,const double,const double,
    const double,const double, const double),
  DrawPathCurveToQuadraticBezierAbsolute(DrawingWand *,const double,
    const double,const double,const double),
  DrawPathCurveToQuadraticBezierRelative(DrawingWand *,const double,
    const double,const double,const double),
  DrawPathCurveToQuadraticBezierSmoothAbsolute(DrawingWand *,const double,
    const double),
  DrawPathCurveToQuadraticBezierSmoothRelative(DrawingWand *,const double,
    const double),
  DrawPathCurveToSmoothAbsolute(DrawingWand *,const double,const double,
    const double,const double),
  DrawPathCurveToSmoothRelative(DrawingWand *,const double,const double,
    const double,const double),
  DrawPathEllipticArcAbsolute(DrawingWand *,const double,const double,
    const double,MagickBooleanType,MagickBooleanType,const double,const double),
  DrawPathEllipticArcRelative(DrawingWand *,const double,const double,
    const double,MagickBooleanType,MagickBooleanType,const double,const double),
  DrawPathFinish(DrawingWand *),
  DrawPathLineToAbsolute(DrawingWand *,const double,const double),
  DrawPathLineToRelative(DrawingWand *,const double,const double),
  DrawPathLineToHorizontalAbsolute(DrawingWand *,const double),
  DrawPathLineToHorizontalRelative(DrawingWand *,const double),
  DrawPathLineToVerticalAbsolute(DrawingWand *,const double),
  DrawPathLineToVerticalRelative(DrawingWand *,const double),
  DrawPathMoveToAbsolute(DrawingWand *,const double,const double),
  DrawPathMoveToRelative(DrawingWand *,const double,const double),
  DrawPathStart(DrawingWand *),
  DrawPoint(DrawingWand *,const double,const double),
  DrawPolygon(DrawingWand *,const unsigned long,const PointInfo *),
  DrawPolyline(DrawingWand *,const unsigned long,const PointInfo *),
  DrawPopClipPath(DrawingWand *),
  DrawPopDefs(DrawingWand *),
  DrawPopPattern(DrawingWand *),
  DrawPushClipPath(DrawingWand *,const char *),
  DrawPushDefs(DrawingWand *),
  DrawPushPattern(DrawingWand *,const char *,const double,const double,
    const double,const double),
  DrawRectangle(DrawingWand *,const double,const double,const double,
    const double),
  DrawRotate(DrawingWand *,const double),
  DrawRoundRectangle(DrawingWand *,double,double,double,double,double,double),
  DrawScale(DrawingWand *,const double,const double),
  DrawSetClipPath(DrawingWand *,const char *),
  DrawSetClipRule(DrawingWand *,const FillRule),
  DrawSetClipUnits(DrawingWand *,const ClipPathUnits),
  DrawSetFillColor(DrawingWand *,const PixelWand *),
  DrawSetFillOpacity(DrawingWand *,const double),
  DrawSetFillRule(DrawingWand *,const FillRule),
  DrawSetFillPatternURL(DrawingWand *,const char *),
  DrawSetFont(DrawingWand *,const char *),
  DrawSetFontFamily(DrawingWand *,const char *),
  DrawSetFontSize(DrawingWand *,const double),
  DrawSetFontStretch(DrawingWand *,const StretchType),
  DrawSetFontStyle(DrawingWand *,const StyleType),
  DrawSetFontWeight(DrawingWand *,const unsigned long),
  DrawSetGravity(DrawingWand *,const GravityType),
  DrawSkewX(DrawingWand *,const double),
  DrawSkewY(DrawingWand *,const double),
  DrawSetStrokeAntialias(DrawingWand *,const MagickBooleanType),
  DrawSetStrokeColor(DrawingWand *,const PixelWand *),
  DrawSetStrokeDashArray(DrawingWand *,const unsigned long,const double *),
  DrawSetStrokeDashOffset(DrawingWand *,const double dashoffset),
  DrawSetStrokeLineCap(DrawingWand *,const LineCap),
  DrawSetStrokeLineJoin(DrawingWand *,const LineJoin),
  DrawSetStrokeMiterLimit(DrawingWand *,const unsigned long),
  DrawSetStrokeOpacity(DrawingWand *, const double),
  DrawSetStrokePatternURL(DrawingWand *,const char *),
  DrawSetStrokeWidth(DrawingWand *,const double),
  DrawSetTextAntialias(DrawingWand *,const MagickBooleanType),
  DrawSetTextDecoration(DrawingWand *,const DecorationType),
  DrawSetTextEncoding(DrawingWand *,const char *),
  DrawSetTextUnderColor(DrawingWand *,const PixelWand *),
  DrawSetViewbox(DrawingWand *,unsigned long,unsigned long,unsigned long,
    unsigned long),
  DrawTranslate(DrawingWand *,const double,const double),
  PopDrawingWand(DrawingWand *),
  PushDrawingWand(DrawingWand *);

/*
  Deprecated.
*/
typedef struct _DrawingWand
  *DrawContext;

extern WandExport DrawInfo
  *DrawPeekGraphicWand(const DrawingWand *);

extern WandExport void
  DrawPopGraphicContext(DrawingWand *),
  DrawPushGraphicContext(DrawingWand *);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif
