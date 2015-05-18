#ifndef PLOTTER_H
#define PLOTTER_H

#include <qpixmap.h>
#include <qwidget.h>

#include <map>
#include <vector>

#include <qstring.h>

class QToolButton;
class PlotSettings;

typedef std::vector<double> CurveData;

class Plotter : public QWidget
{
    Q_OBJECT
public:
    Plotter(QWidget *parent = 0, const char *name = 0,
            WFlags flags = 0);

    void setPlotSettings(const PlotSettings &settings);
    void setCurveData(int id, const CurveData &data);
    void clearCurve(int id);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;

	QString exhumation;
	QString iso_1;
	QString iso_2;
	QString iso_3;
	QString iso_4;

public slots:
    void zoomIn();
    void zoomOut();
    
    void save();
    void print();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void updateRubberBandRegion();
    void refreshPixmap(int);
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);

    enum { Margin = 40 };

    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;
    
    QToolButton *saveButton;
    QToolButton *printButton;
    
    std::map<int, CurveData> curveMap;
    std::vector<PlotSettings> zoomStack;
    int curZoom;
    bool rubberBandIsShown;
    QRect rubberBandRect;
    QPixmap pixmap;
	QPixmap pic;
};

class PlotSettings
{
public:
    PlotSettings();

    void scroll(int dx, int dy);
    void adjust();
    double spanX() const { return maxX - minX; }
    double spanY() const { return maxY - minY; }

    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;

private:
    void adjustAxis(double &min, double &max, int &numTicks);
};

#endif
