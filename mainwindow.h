#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QTimer>
#include <QLabel>
#include "ECS.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showFloorNum(int floorNum);
    void showElevatorNum(int elevatorNum);

private:
    Ui::MainWindow *ui;
    ECS* ecs;
    vector<QPushButton*> qDestButtons;
    vector<vector<QTextBrowser*>> elevatorFloorLayout;
    QTimer bellTimer;
    int currentFloorNum;
    int currentElevatorNum;
    QTimer movementTimer;
    vector<QTextBrowser*> floorDoorLayout;
    vector<QTextBrowser*> elevatorDoorLayout;
    vector<QLabel*> elevatorLabels;
    vector<QLabel*> floorLabels;

public slots:
    void ringBellGUI();
    void clearBellGUI();

    void displayFloorGUI(int, int);
    void displayMessageGUI(int, string);

    void outputAudioMessageGUI(int, string);

    void clickUpButton();
    void clickDownButton();

    void clickDestinationButton(int);

    void changeFloor();
    void changeElevator();

    void moveElevators();

    void floorSensorArrivedDisplay(int elevatorNum, int floorNum);

    void manageIlluminationDestinationButton(int, bool);
    void manageIlluminationUpButton(int, bool);
    void manageIlluminationDownButton(int, bool);

    void manageElevatorDoor(int, bool);
    void manageFloorDoor(int, bool);

    void addWeightSlot();
    void removeWeightSlot();

    void goBackToFloorDisplay(int elevatorNum);

    void blockDoor();

    void elevatorFire();

    void outOfOrder();
};
#endif // MAINWINDOW_H
