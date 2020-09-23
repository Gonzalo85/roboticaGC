#include "ejemplo1.h"


ejemplo1::ejemplo1(): Ui_Counter()
{
    unsigned static t0;
    t0=clock();
    unsigned t1;
	setupUi(this);
	show();
	connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
    connect(Slider, SIGNAL (sliderReleased()), this, SLOT(cambiar()));
    mytimer.connect(std::bind(&ejemplo1::cuenta, this));
    mytimer.start(500);
    t1 = clock();
    lcd_tiempo->display((double(t1-t0)/1000));
}

ejemplo1::~ejemplo1()
{}

void ejemplo1::doButton()
{
	static bool stopped = false;
	stopped = !stopped;
	if(stopped)
		mytimer.stop();
	else
		mytimer.start(500);
	qDebug() << "click on button";
}

void ejemplo1::cuenta()
{
    lcdNumber->display(++cont);
	trick++;

}

void ejemplo1::cambiar()
{
 //   qDebug() << Slider->value();
    mytimer.setPeriod(Slider->value());
    lcd_periodo->display(Slider->value());
}



