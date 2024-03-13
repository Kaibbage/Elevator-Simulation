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
    void greyOutButtons(int elevatorNum, bool grey);

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
    void ringBellGUI(int);
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

    void manageIlluminationDestinationButton(int, int, bool);
    void manageIlluminationUpButton(int, int, bool);
    void manageIlluminationDownButton(int, int, bool);

    void manageElevatorDoor(int, bool);
    void manageFloorDoor(int, bool);

    void addWeightSlot();
    void removeWeightSlot();

    void goBackToFloorDisplay(int elevatorNum);

    void blockDoor();

    void elevatorFire();

    void outOfOrder(int elevatorNum);

    void buildingFire();

    void buildingPowerOutage();

    void helpButtonPressed();
    void talkButtonPressed();
    void buildingSafetyCalled(int elevatorNum);
    void passengerTalked(int elevatorNum);
    void called911(int elevatorNum);

    void manageIlluminationHelpButton(int, int, bool);

    void openDoorButtonPressed();
    void closeDoorButtonPressed();

    void openDoorButtonLetGo();
    void closeDoorButtonLetGo();

    void manageIlluminationOpenButton(int, int, bool);
    void manageIlluminationCloseButton(int, int, bool);
};
#endif // MAINWINDOW_H
