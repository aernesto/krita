/*
 *  Copyright (c) 2011 Dmitry Kazakov <dimula73@gmail.com>
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

#ifndef __KIS_STROKE_H
#define __KIS_STROKE_H

#include <QQueue>
#include <QMutex>
#include <kis_types.h>
#include "krita_export.h"
#include "kis_stroke_job.h"

class KisStrokeStrategy;


class KRITAIMAGE_EXPORT KisStroke
{
public:
    KisStroke(KisStrokeStrategy *strokeStrategy);
    ~KisStroke();

    void addJob(KisDabProcessingStrategy::DabProcessingData *data);

    bool hasJobs() const;
    KisStrokeJob* popOneJob();

    void endStroke();
    void cancelStroke();

    bool isInitialized() const;
    bool isEnded() const;

    bool isExclusive() const;

    bool prevJobSequential() const;
    bool nextJobSequential() const;

private:
    void enqueue(KisDabProcessingStrategy *strategy,
                 KisDabProcessingStrategy::DabProcessingData *data);
    KisStrokeJob* dequeue();
    void clearQueue();

private:
    // for testing use only, do not use in real code
    friend class KisStrokeTest;
    QQueue<KisStrokeJob*>& testingGetQueue() {
        return m_jobsQueue;
    }

private:
    // the strategies are owned by the stroke
    KisStrokeStrategy *m_strokeStrategy;
    KisDabProcessingStrategy *m_initStrategy;
    KisDabProcessingStrategy *m_dabStrategy;
    KisDabProcessingStrategy *m_cancelStrategy;
    KisDabProcessingStrategy *m_finishStrategy;

    QQueue<KisStrokeJob*> m_jobsQueue;
    bool m_strokeInitialized;
    bool m_strokeEnded;
    bool m_prevJobSequential;
    mutable QMutex m_mutex;
};

#endif /* __KIS_STROKE_H */
