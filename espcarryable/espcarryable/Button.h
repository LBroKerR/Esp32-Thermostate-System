
#ifndef BUTTON_H
#define BUTTON_H
class Buttons
{
	unsigned BTplus; //pins to buttons
	unsigned BTminus;
	unsigned BTprog;
	
	const unsigned getBTPl();//return button pins
	const unsigned getBTM();
	const unsigned getBTpr();



public:
	Buttons(unsigned const pinPl, unsigned const pinM,unsigned const pinPr);

	float tempchange(float step);// counting up or down, according to step. step is number, whitch is delta between two number
	int progchange();//while push down the prog button, you can change prog with plus or minus button. 
};

#endif // BUTTON_H
