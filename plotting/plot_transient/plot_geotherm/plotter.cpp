#include <qpainter.h>
#include <qstyle.h>
#include <qtoolbutton.h>
#include <qwmatrix.h> 
#include <qbrush.h>
#include <qprinter.h>

#include <cmath>

#include "plotter.h"

using namespace std;

Plotter::Plotter(QWidget *parent, const char *name, WFlags flags) : QWidget(parent, name, flags | WNoAutoErase)
{
  setBackgroundMode(PaletteLight);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setFocusPolicy(StrongFocus);
  rubberBandIsShown = false;

  zoomInButton = new QToolButton(this);
  zoomInButton->setIconSet(QPixmap::fromMimeSource("zoomin.png"));
  zoomInButton->adjustSize();
  connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

  zoomOutButton = new QToolButton(this);
  zoomOutButton->setIconSet(QPixmap::fromMimeSource("zoomout.png"));
  zoomOutButton->adjustSize();
  connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

  saveButton = new QToolButton(this);
  saveButton->setIconSet(QPixmap::fromMimeSource("save.png"));
  saveButton->adjustSize();
  connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
  
  printButton = new QToolButton(this);
  printButton->setIconSet(QPixmap::fromMimeSource("print.png"));
  printButton->adjustSize();
  connect(printButton, SIGNAL(clicked()), this, SLOT(print()));

  setPlotSettings(PlotSettings());
}

void Plotter::setPlotSettings(const PlotSettings &settings)
{
  zoomStack.resize(1);
  zoomStack[0] = settings;
  curZoom = 0;
	zoomInButton->setEnabled(false);
	zoomInButton->show();

	zoomOutButton->setEnabled(false);
	zoomOutButton->show();
    
  saveButton->setEnabled(true);
  saveButton->show();
    
  printButton->setEnabled(true);
  printButton->show();
    
  refreshPixmap(0);
}

void Plotter::zoomOut()
{
  if(curZoom > 0) {
    --curZoom;
    zoomOutButton->setEnabled(curZoom > 0);
    zoomInButton->setEnabled(true);
    zoomInButton->show();
    refreshPixmap(0);
  }
}

void Plotter::zoomIn()
{
  if(curZoom < (int)zoomStack.size() - 1) {
    ++curZoom;
    zoomInButton->setEnabled(curZoom < (int)zoomStack.size() - 1);
    zoomOutButton->setEnabled(true);
    zoomOutButton->show();
    refreshPixmap(0);
  }
}

void Plotter::save()
{
	refreshPixmap(1);
}

void Plotter::print()
{
	refreshPixmap(2);
}

void Plotter::setCurveData(int id, const CurveData &data)
{
  curveMap[id] = data;
  refreshPixmap(0);
}

void Plotter::clearCurve(int id)
{
  curveMap.erase(id);
  refreshPixmap(0);
}

QSize Plotter::minimumSizeHint() const
{
  return QSize(4 * Margin, 4 * Margin);
}

QSize Plotter::sizeHint() const
{
  return QSize(8 * Margin, 6 * Margin);
}

void Plotter::paintEvent(QPaintEvent *event)
{
	QMemArray<QRect> rects = event->region().rects();
  for(int i = 0; i < (int)rects.size(); ++i)
    bitBlt(this, rects[i].topLeft(), &pixmap, rects[i]);

  QPainter painter(this);

  if(rubberBandIsShown) {
    painter.setPen(colorGroup().dark());
    painter.drawRect(rubberBandRect.normalize());
  }
  if(hasFocus()) {
    style().drawPrimitive(QStyle::PE_FocusRect, &painter,
                          rect(), colorGroup(),
                          QStyle::Style_FocusAtBorder,
                          colorGroup().light());
  }
}

void Plotter::resizeEvent(QResizeEvent *)
{
  int x = width() - (zoomInButton->width() + zoomOutButton->width() + saveButton->width() + printButton->width() + 10);
  zoomInButton->move(x-30, 5);
  zoomOutButton->move(x - 30 + zoomInButton->width() + 5, 5);
  saveButton->move(x - 30 + zoomInButton->width() + zoomOutButton->width() + 10, 5);
  printButton->move(x - 30 + zoomInButton->width() + zoomOutButton->width() + saveButton->width() + 15, 5);
  refreshPixmap(0);
}

void Plotter::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == LeftButton) {
    rubberBandIsShown = true;
    rubberBandRect.setTopLeft(event->pos());
    rubberBandRect.setBottomRight(event->pos());
    updateRubberBandRegion();
    setCursor(crossCursor);
  }
}

void Plotter::mouseMoveEvent(QMouseEvent *event)
{
  if(event->state() & LeftButton) {
    updateRubberBandRegion();
    rubberBandRect.setBottomRight(event->pos());
    updateRubberBandRegion();
  }
}

void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
  if(event->button() == LeftButton) {
    rubberBandIsShown = false;
    updateRubberBandRegion();
    unsetCursor();

    QRect rect = rubberBandRect.normalize();
    if (rect.width() < 4 || rect.height() < 4)
        return;
    rect.moveBy(-Margin, -Margin);

    PlotSettings prevSettings = zoomStack[curZoom];
    PlotSettings settings;
    double dx = prevSettings.spanX() / (width() - 2 * Margin);
    double dy = prevSettings.spanY() / (height() - 2 * Margin);
    settings.minX = prevSettings.minX + dx * rect.left();
    settings.maxX = prevSettings.minX + dx * rect.right();
    settings.minY = prevSettings.maxY - dy * rect.bottom();
    settings.maxY = prevSettings.maxY - dy * rect.top();
    settings.adjust();

    zoomStack.resize(curZoom + 1);
    zoomStack.push_back(settings);
    zoomIn();
  }
}

void Plotter::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Key_Plus:
        zoomIn();
        break;
    case Key_Minus:
        zoomOut();
        break;
    case Key_Left:
        zoomStack[curZoom].scroll(-1, 0);
        refreshPixmap(0);
        break;
    case Key_Right:
        zoomStack[curZoom].scroll(+1, 0);
        refreshPixmap(0);
        break;
    case Key_Down:
        zoomStack[curZoom].scroll(0, -1);
        refreshPixmap(0);
        break;
    case Key_Up:
        zoomStack[curZoom].scroll(0, +1);
        refreshPixmap(0);
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void Plotter::wheelEvent(QWheelEvent *event)
{
  int numDegrees = event->delta() / 8;
  int numTicks = numDegrees / 15;

  if (event->orientation() == Horizontal)
      zoomStack[curZoom].scroll(numTicks, 0);
  else
      zoomStack[curZoom].scroll(0, numTicks);
  refreshPixmap(0);
}

void Plotter::updateRubberBandRegion()
{
  QRect rect = rubberBandRect.normalize();
  update(rect.left(), rect.top(), rect.width(), 1);
  update(rect.left(), rect.top(), 1, rect.height());
  update(rect.left(), rect.bottom(), rect.width(), 1);
  update(rect.right(), rect.top(), 1, rect.height());
}

void Plotter::refreshPixmap(int value)
{
  QPrinter printer;
  pixmap.resize(size());
  pixmap.fill(this, 0, 0);
  QPainter painter(&pixmap, this);
  drawGrid(&painter);
  drawCurves(&painter);
  update();
  if(value == 1)
    pixmap.save("../output/plots/image_geotherm.png", "PNG");
  else if(value == 2)
  {
    if(printer.setup(this))
    {
      printer.setOrientation(QPrinter::Landscape);
      QPainter painter_print(&printer);
      drawGrid(&painter_print);
      drawCurves(&painter_print);
      update();
    }	
  }	
}

void Plotter::drawGrid(QPainter *painter)
{
	QWMatrix matrix;
	QRect rect(Margin+20, Margin, width() - 2 * Margin, height() - 2 * Margin);
	QRect rect_legend(width() - 135, Margin + 5, 110, 80);
  PlotSettings settings = zoomStack[curZoom];
  QPen quiteLight = colorGroup().light().dark();
	QPen dark(black);
	quiteLight.setStyle(DotLine);
  for(int i = 0; i <= settings.numXTicks; ++i) {
        int x = rect.left() + (i * (rect.width() - 1)
                                 / settings.numXTicks);
        double label = settings.minX + (i * settings.spanX()
                                          / settings.numXTicks);
        painter->setPen(quiteLight);
        painter->drawLine(x, rect.top(), x, rect.bottom());
		//////////////////////////
		painter->drawPixmap(x, rect.top(), pic);
		///////////////////////////
        painter->setPen(dark);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
		/////////////////////////
		painter->drawPixmap(x, rect.bottom(), pic);
		/////////////////////////
		//if((i%4)==0)
        painter->drawText(x - 50, rect.bottom() + 5, 100, 15,
                          AlignHCenter | AlignTop,
                          QString::number(label));
		////////////////////////
		painter->drawPixmap(x - 50, rect.bottom() + 5, pic);
		///////////////////////
    }
	//////////////////////////////////////////////////////////////
    for (int j = 0; j <= settings.numYTicks; ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1)
                                   / settings.numYTicks);
        double label = settings.minY + (j * settings.spanY()
                                          / settings.numYTicks);
        painter->setPen(quiteLight);
		//if((j % 4) == 0)
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(dark);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
		//if((j % 4) == 0)
        painter->drawText(rect.left() - Margin, y - 10,
                          Margin - 5, 20,
                          AlignRight | AlignVCenter,
                          QString::number(label));
    }
	//////////////////////////////////////////////////////////////

  painter->drawRect(rect);
	painter->drawRect(rect_legend);

	dark.setWidth(3);
	painter->setPen(dark);
	painter->drawText(Margin, Margin - 30, width() - 2 * Margin, 20, AlignHCenter | AlignTop, "GEOTHERMS: 1D TRANSIENT EROSION, EROSION RATE = " + exhumation + " mm/yr");
	painter->drawText(Margin, Margin + height() - 65, width() - 2 * Margin, 20, AlignHCenter | AlignBottom, "Temperature (C)");

	matrix.rotate(-90);
	painter->setWorldMatrix(matrix);
	painter->drawText(Margin-400, Margin - 43, height() - 2 * Margin, 20, AlignLeft | AlignBottom, "Depth (m)");
	matrix.rotate(90);
	painter->setWorldMatrix(matrix);
}

void Plotter::drawCurves(QPainter *painter)
{
	static const QColor colorForIds[6] = { black, blue, darkGreen, green, red, darkRed };
  PlotSettings settings = zoomStack[curZoom];
	
  QPen pen;
	PenStyle s;
	QBrush brush(white);
  
  QRect rect(Margin+20, Margin,
             width() - 2 * Margin, height() - 2 * Margin);

  painter->setClipRect(rect.x() + 1, rect.y() + 1,
                       rect.width() - 2, rect.height() - 2);
	int id, x1, y1;
  map<int, CurveData>::const_iterator it = curveMap.begin();

  while (it != curveMap.end()) {
    id = (*it).first;
    const CurveData &data = (*it).second;
    int numPoints = 0;
    int maxPoints = data.size() / 2;
    QPointArray points(maxPoints);

		double dx, dy, x, y;

		for (int i = 0; i < maxPoints; ++i) {
            dx = data[2 * i] - settings.minX;
            dy = data[2 * i + 1] - settings.minY;
            x = rect.left() + (dx * (rect.width() - 1)
                                         / settings.spanX());
            y = rect.bottom() - (dy * (rect.height() - 1)
                                           / settings.spanY());
            if (fabs(x) < 32768 && fabs(y) < 32768) {
                points[numPoints] = QPoint((int)x, (int)y);
                ++numPoints;
            }
        }
        points.truncate(numPoints);
		pen.setColor(colorForIds[(uint)id % 6]);
		switch((uint)id % 5)
		{
		case 0:	
			s = SolidLine;
			pen.setWidth(3);
			break;
		case 1:
			s = DashLine;
			pen.setWidth(3);
			break;
		case 2:
			s = DotLine;
			pen.setWidth(3);
			break;
		case 3:
			s = DashDotLine;
			pen.setWidth(3);
			break;
		case 4:
			s = DashDotDotLine;
			pen.setWidth(3);
			break;
		default:
			s = SolidLine;
			pen.setWidth(3);
			break;
		}
		pen.setStyle(s);
		//painter->setPen(colorForIds[(uint)id % 6]);
		painter->setPen(pen);
        painter->drawPolyline(points);
        ++it;
    }
	painter->fillRect(width() - 134, Margin + 6, 108, 78, brush);
	x1 = width() - 134;
	y1 = Margin + 6;

	s = SolidLine;
	pen.setWidth(3);
	pen.setStyle(s);
	pen.setColor(black);
	painter->setPen(pen);
	painter->drawLine(x1 + 5, y1 + 10, x1 + 35, y1 + 10);
	pen.setColor(black);
	painter->setPen(pen);
	painter->drawText(x1 + 45, y1 + 15, iso_1 + " Myrs");
	
	s = DashLine;
	pen.setWidth(3);
	pen.setStyle(s);
	pen.setColor(blue);
	painter->setPen(pen);
	painter->drawLine(x1 + 5, y1 + 25, x1 + 35, y1 + 25);
	pen.setColor(black);
	painter->setPen(pen);
	painter->drawText(x1 + 45, y1 + 30, iso_2 + " Myrs");
 
	s = DotLine;
	pen.setWidth(3);
	pen.setStyle(s);
	pen.setColor(darkGreen);
	painter->setPen(pen);
	painter->drawLine(x1 + 5, y1 + 40, x1 + 35, y1 + 40);
	pen.setColor(black);
	painter->setPen(pen);
	painter->drawText(x1 + 45, y1 + 45, iso_3 + " Myrs");

	s = DashDotLine;
	pen.setWidth(3);
	pen.setStyle(s);
	pen.setColor(green);
	painter->setPen(pen);
	painter->drawLine(x1 + 5, y1 + 55, x1 + 35, y1 + 55);
	pen.setColor(black);
	painter->setPen(pen);
	painter->drawText(x1 + 45, y1 + 60, iso_4 + " Myrs");

}

PlotSettings::PlotSettings()
{
  minX = 0.0;
  maxX = 10.0;
  numXTicks = 5;

  minY = 0.0;
  maxY = 10.0;
  numYTicks = 5;
}

void PlotSettings::scroll(int dx, int dy)
{
  double stepX = spanX() / numXTicks;
  minX += dx * stepX;
  maxX += dx * stepX;

  double stepY = spanY() / numYTicks;
  minY += dy * stepY;
  maxY += dy * stepY;
}

void PlotSettings::adjust()
{
  adjustAxis(minX, maxX, numXTicks);
  adjustAxis(minY, maxY, numYTicks);
}

void PlotSettings::adjustAxis(double &min, double &max,
                              int &numTicks)
{
  const int MinTicks = 4;
  double grossStep = (max - min) / MinTicks;
  double step = pow(10, floor(log10(grossStep)));

  if (5 * step < grossStep)
      step *= 5;
  else if (2 * step < grossStep)
      step *= 2;

  numTicks = (int)(ceil(max / step) - floor(min / step));
  min = floor(min / step) * step;
  max = ceil(max / step) * step;
}
