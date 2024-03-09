#ifndef INTEGERTIMER_H
#define INTEGERTIMER_H
#include <QTimer>

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
