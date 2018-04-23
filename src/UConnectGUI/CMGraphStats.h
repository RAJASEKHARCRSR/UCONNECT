#ifndef CMGraphStats_H
#define CMGraphStats_H

#include <qwt_plot.h>

#define HISTORY 60 // seconds

class QwtPlotCurve;

typedef struct generalInfo
{
    int Rsrp;
    int Rsrq;
    int Sinr;
    int interval_timer;
}stStats;
class CMGraphStats : public QwtPlot
{
    Q_OBJECT
public:
    enum StatsData
    {
        RSRQ,
        RSRP,
        SINR,
        NStatsData
    };

    CMGraphStats(QWidget * = 0);
    const QwtPlotCurve *cpuCurve(int id) const
        { return data[id].curve; }

    void updateAndPlot(stStats siginfo);

protected:
//    void timerEvent(QTimerEvent *e);

private Q_SLOTS:
    void showCurve(QwtPlotItem *, bool on);

private:
    struct
    {
        QwtPlotCurve *curve;
        double data[HISTORY];
    } data[NStatsData];
    double timeData[HISTORY];

    int dataCount;

};

#endif // CMGraphStats_H
