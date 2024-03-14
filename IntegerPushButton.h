#ifndef INTEGERPUSHBUTTON_H
#define INTEGERPUSHBUTTON_H
#include <QApplication>
#include <QPushButton>
/*
 * IntegerPush is a child of QPushButton
 * Just a QPushButton that sends an integer with it when it is clicked (for the elevatorNum)
*/
class IntegerPushButton : public QPushButton {
    Q_OBJECT
public:
    explicit IntegerPushButton(int intValue, const QString &text, QWidget *parent = nullptr);

signals:
    void intValueClicked(int value);

private slots:
    void sendIntValue();

private:
    int intValue;
};
#endif // INTEGERPUSHBUTTON_H
