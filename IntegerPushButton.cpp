#include "IntegerPushButton.h"


IntegerPushButton::IntegerPushButton(int intValue, const QString &text, QWidget *parent)
    : QPushButton(text, parent), intValue(intValue) {
    connect(this, SIGNAL(clicked()), this, SLOT(sendIntValue()));
}

void IntegerPushButton::sendIntValue() {
    emit intValueClicked(intValue);
}
