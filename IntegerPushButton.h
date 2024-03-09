#ifndef INTEGERPUSHBUTTON_H
#define INTEGERPUSHBUTTON_H
#include <QApplication>
#include <QPushButton>

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
