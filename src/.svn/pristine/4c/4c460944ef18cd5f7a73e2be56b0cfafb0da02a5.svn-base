#include <qapplication.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_plot_canvas.h>
#include "CMGraphStats.h"
#include <iostream>

using namespace std;




class Background: public QwtPlotItem
{
public:
    Background()
    {
        setZ(0.0);
    }

    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }

    virtual void draw(QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &yMap,
        const QRectF &rect) const
    {
        QColor c(Qt::white);
        QRectF r = rect;

        for ( int i = 100; i > 0; i -= 10 )
        {
            r.setBottom(yMap.transform(i - 10));
            r.setTop(yMap.transform(i));
            painter->fillRect(r, c);

            c = c.dark(110);
        }
    }
};

class StatsCurve: public QwtPlotCurve
{
public:
    StatsCurve(const QString &title):
        QwtPlotCurve(title)
    {
        setRenderHint(QwtPlotItem::RenderAntialiased);
    }

    void setColor(const QColor &color)
    {
        QColor c = color;
        c.setAlpha(150);

        setPen(c);
        //setBrush(c);
    }
};

CMGraphStats::CMGraphStats(QWidget *parent):
    QwtPlot(parent),
    dataCount(0)
{
    setAutoReplot(false);

    canvas()->setBorderRadius( 10 );

    plotLayout()->setAlignCanvasToScales(true);

    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::RightLegend);

    //setAxisTitle(QwtPlot::xBottom, " TIME in Secs");


    //int time = ( 60 * siginfo.interval_timer);
    //qCritical()<<"time@:"<< time << " siginfo.interval_timer:" << siginfo.interval_timer;
    setAxisScale(QwtPlot::xBottom, 0,HISTORY);
    setAxisLabelRotation(QwtPlot::xBottom, -50.0);
    setAxisLabelAlignment(QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);

    /*
     In situations, when there is a label at the most right position of the
     scale, additional space is needed to display the overlapping part
     of the label would be taken by reducing the width of scale and canvas.
     To avoid this "jumping canvas" effect, we add a permanent margin.
     We don't need to do the same for the left border, because there
     is enough space for the overlapping label below the left scale.
     */

    QwtScaleWidget *scaleWidget = axisWidget(QwtPlot::xBottom);
    const int fmh = QFontMetrics(scaleWidget->font()).height();
    scaleWidget->setMinBorderDist(0, fmh / 2);

    //setAxisTitle(QwtPlot::yLeft, "Measurment Values");
    setAxisScale(QwtPlot::yLeft, 0, 100);
    
    StatsCurve *curve;

    curve = new StatsCurve("RSRQ");
    curve->setColor(Qt::red);
    curve->attach(this);
    data[RSRQ].curve = curve;

    curve = new StatsCurve("RSRP");
    curve->setColor(Qt::blue);
    curve->setZ(curve->z() - 1);
    curve->attach(this);
    data[RSRP].curve = curve;

    curve = new StatsCurve("SINR");
    curve->setColor(Qt::darkCyan);
    curve->setZ(curve->z() - 2);
    curve->attach(this);
    data[SINR].curve = curve;


    showCurve(data[RSRQ].curve, true);
    showCurve(data[RSRP].curve, false);
    showCurve(data[SINR].curve, false);



    for ( int i = 0; i < HISTORY; i++ )
     timeData[HISTORY - 1 - i] = i;



    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
        SLOT(showCurve(QwtPlotItem *, bool)));
}



void CMGraphStats::updateAndPlot(stStats siginfo){



    int minY= 99999;
    int maxY= -99999;
    for ( int c = 0; c < NStatsData; c++ )
    {

        for ( int i = dataCount; i > 0; i-- )
        {


                data[c].data[i] = data[c].data[i-1];

                if(data[c].data[i] > maxY){
                 maxY = data[c].data[i];
                }
                if(data[c].data[i]< minY){
                 minY = data[c].data[i];
                }
        }

    }

    data[0].data[0] = siginfo.Rsrq;
    data[1].data[0] = siginfo.Rsrp;
    data[2].data[0] = siginfo.Sinr;

    for ( int c = 0; c < NStatsData; c++ )
    {
     if(data[c].data[0] > maxY){
        maxY = data[c].data[0];
     }
    if(data[c].data[0]< minY){
     minY = data[c].data[0];
    }
    }



    dataCount++;
    if (dataCount >= HISTORY-1)
    {
        dataCount = HISTORY-1;
    }
  setAxisScale(QwtPlot::yLeft, minY, maxY);

    for ( int c = 0; c < NStatsData; c++ )
    {
        data[c].curve->setRawSamples(
            timeData, data[c].data, dataCount);
    }

    replot();


}



void CMGraphStats::showCurve(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
    QWidget *w = legend()->find(item);
    if ( w && w->inherits("QwtLegendItem") )
        ((QwtLegendItem *)w)->setChecked(on);

    replot();
}

