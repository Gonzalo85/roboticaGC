#include "ejemplo1.h"

ejemplo1::ejemplo1(): Ui_Counter()
{
	setupUi(this);
	show();
	connect(button, SIGNAL(clicked()), this, SLOT(doButton()) );
	connect(Slider, SIGNAL (sliderReleased()), this, SLOT(cambiar()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(contar()));
	timer.start(1000);
}

void ejemplo1::doButton()
{
    timer.stop();
}

void ejemplo1::contar()
{
    static int i=0;
    lcdNumber->display(i++);
}

void ejemplo1::cambiar()
{
    timer.stop();
    qDebug() << Slider->value();
    timer.start(Slider->value());
}

