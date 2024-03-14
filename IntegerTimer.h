#ifndef INTEGERTIMER_H
#define INTEGERTIMER_H
#include <QTimer>

/*
 * IntegerTimer is a child of QTimer
 * Just a QTimer that sends an integer with it when it times out (for the elevatorNum)
*/
class IntegerTimer : public QTimer {
    Q_OBJECT
public:
    explicit IntegerTimer(int elevatorNum = 0, QObject *parent = nullptr);
    void setElevatorNum(int elevatorNum);

private:
    int elevatorNum;

signals:
    void timeoutWithInt(int value);

private slots:
    void handleTimeout();
};
#endif // INTEGERTIMER_H
