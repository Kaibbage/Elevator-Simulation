#include "IntegerPushButton.h"

//Constructor, connects the QPushButton's clicked signal to this class which will send its own signal with an int
IntegerPushButton::IntegerPushButton(int intValue, const QString &text, QWidget *parent)
    : QPushButton(text, parent), intValue(intValue) {
    connect(this, SIGNAL(clicked()), this, SLOT(sendIntValue()));
}

//Sends the button's clicked() signal but with an integer parameter
void IntegerPushButton::sendIntValue() {
    emit intValueClicked(intValue);
}
