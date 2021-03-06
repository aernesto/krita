/*
 *  Copyright (c) 2012 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __KIS_DAB_CACHE_H
#define __KIS_DAB_CACHE_H

#include "kritapaintop_export.h"
#include "kis_brush.h"

class KisColorSource;
class KisPressureSharpnessOption;
class KisTextureProperties;
class KisPressureMirrorOption;
class KisPrecisionOption;
struct MirrorProperties;


/**
 * @brief The KisDabCache class provides caching for dabs into the brush paintop
 *
 *  This class adds caching of the dabs to the paintop system of Krita.
 *  Such cache makes the execution of the benchmarks up to 2 times faster.
 *  Subjectively, the real painting becomes much faster, especially with
 *  huge brushes. Artists report up to 20% speed gain while painting.
 *
 *  Of course, such caching makes the painting a bit less precise: we need
 *  to tolerate subpixel differences to allow the cache to work. Sometimes
 *  small difference in the size of a dab can also be acceptable. That is
 *  why I introduced levels of precision. They are graded from 1 to 5: from
 *  the fastest and less precise to the slowest, but with the best quality.
 *  You can see the slider in the paintop settings dialog. The ToolTip text
 *  explains which features of the brush are sacrificed on each precision
 *  level.
 *
 *  The texturing and mirroring problems are solved.
 */
class PAINTOP_EXPORT KisDabCache
{
public:
    KisDabCache(KisBrushSP brush);
    ~KisDabCache();

    void setMirrorPostprocessing(KisPressureMirrorOption *option);
    void setSharpnessPostprocessing(KisPressureSharpnessOption *option);
    void setTexturePostprocessing(KisTextureProperties *option);
    void setPrecisionOption(KisPrecisionOption *option);

    /**
     * Disables handling of the subPixelX and subPixelY values, this
     * is needed at least for the Color Smudge paint op, which reads
     * aligned areas from image, so additional offsets generated by
     * the subpixel precision should be avoided
     */
    void disableSubpixelPrecision();

    bool needSeparateOriginal();

    KisFixedPaintDeviceSP fetchDab(const KoColorSpace *cs,
                                   const KisColorSource *colorSource,
                                   const QPointF &cursorPoint,
                                   KisDabShape const&,
                                   const KisPaintInformation& info,
                                   qreal softnessFactor,
                                   QRect *dstDabRect);

    KisFixedPaintDeviceSP fetchDab(const KoColorSpace *cs,
                                   const KoColor& color,
                                   const QPointF &cursorPoint,
                                   KisDabShape const&,
                                   const KisPaintInformation& info,
                                   qreal softnessFactor,
                                   QRect *dstDabRect);


private:
    struct SavedDabParameters;
    struct DabPosition;
private:
    inline SavedDabParameters getDabParameters(const KoColor& color,
            KisDabShape const&,
            const KisPaintInformation& info,
            double subPixelX, double subPixelY,
            qreal softnessFactor,
            MirrorProperties mirrorProperties);
    inline KisDabCache::DabPosition
    calculateDabRect(const QPointF &cursorPoint,
                     KisDabShape,
                     const KisPaintInformation& info,
                     const MirrorProperties &mirrorProperties);

    inline
    QRect correctDabRectWhenFetchedFromCache(const QRect &dabRect,
            const QSize &realDabSize);

    inline KisFixedPaintDeviceSP tryFetchFromCache(const SavedDabParameters &params,
            const KisPaintInformation& info,
            QRect *dstDabRect);

    inline KisFixedPaintDeviceSP fetchDabCommon(const KoColorSpace *cs,
            const KisColorSource *colorSource,
            const KoColor& color,
            const QPointF &cursorPoint,
            KisDabShape,
            const KisPaintInformation& info,
            qreal softnessFactor,
            QRect *dstDabRect);

    void postProcessDab(KisFixedPaintDeviceSP dab,
                        const QPoint &dabTopLeft,
                        const KisPaintInformation& info);

private:

    struct Private;
    Private * const m_d;

};

#endif /* __KIS_DAB_CACHE_H */
