#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QTimer>
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

private:
    Ui::MainWindow *ui;
    ECS* ecs;
    vector<QPushButton*> qDestButtons;
    vector<vector<QTextBrowser*>> elevatorFloorLayout;
    QTimer bellTimer;
    int currentFloorNum;
    int currentElevatorNum;
    QTimer movementTimer;

public slots:
    void ringBellGUI();
    void clearBellGUI();

    void displayFloorGUI(int);
    void displayMessageGUI(string);

    void outputAudioMessageGUI(string);

    void clickUpButton();
    void clickDownButton();

    void clickDestinationButton(int);

    void changeFloor();
    void changeElevator();

    void moveElevators();
};
#endif // MAINWINDOW_H
