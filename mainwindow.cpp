#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ECS.h"
#include "IntegerPushButton.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    
    vector<Elevator*> elevators;


    for(int i = 0; i < NUM_ELEVATORS; i++){
        int elevatorNum = i;
        vector<Button*> destinationButtons;
        for(int i = 0; i < NUM_FLOORS; i++){
            Button* destinationButton = new Button();
            destinationButtons.push_back(destinationButton);
        }
        Button* openButton = new Button();
        Button* closeButton = new Button();
        Button* helpButton = new Button();
        Bell* bell = new Bell();
        Door* eDoor = new Door();
        Display* display = new Display();
        AudioSystem* audioSystem = new AudioSystem();
        WeightSensor* weightSensor = new WeightSensor();
        LightSensor* lightSensor = new LightSensor();

        Elevator* elevator1 = new Elevator(elevatorNum, destinationButtons, openButton, closeButton, helpButton, bell, eDoor, display, audioSystem, weightSensor, lightSensor, nullptr);
        elevators.push_back(elevator1);
    }



    vector<Floor*> floors;

    for(int i = 0; i < NUM_FLOORS; i++){
        int floorNum = i;
        Button* upButton = new Button();
        Button* downButton = new Button();
        Door* floorDoor = new Door();

        Floor* floor = new Floor(floorNum, upButton, downButton, floorDoor, nullptr);
        floors.push_back(floor);
    }


    vector<vector<FloorSensor*>> allFloorSensors;
    for(int i = 0; i < NUM_ELEVATORS; i++){
        allFloorSensors.push_back({});
        for(int j = 0; j < NUM_FLOORS; j++){
            FloorSensor* floorSensor = new FloorSensor(j, elevators.at(i));
            allFloorSensors.at(i).push_back(floorSensor);
        }
    }


    ecs = new ECS(elevators, floors, allFloorSensors, nullptr);

    for(int j = 0; j < NUM_ELEVATORS; j++){
        elevators.at(j)->setECS(ecs);
    }

    for(int i = 0; i < NUM_FLOORS; i++){
        floors.at(i)->setECS(ecs);
    }
    




    for(int i = 0; i < NUM_FLOORS; i++){
        IntegerPushButton *button = new IntegerPushButton(i, QString::number(i+1), ui->centralwidget);
        button->setGeometry(300, 420 - (50*i), 83, 25);
        qDestButtons.push_back(button);

        connect(button, SIGNAL(intValueClicked(int)), this, SLOT(clickDestinationButton(int)));
    }


    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevatorFloorLayout.push_back({});
        for(int j = 0; j < NUM_FLOORS; j++){
            QTextBrowser* block = new QTextBrowser(ui->centralwidget);
            block->setGeometry(70 + (60*i), 430 - (60*j), 60, 60);
            elevatorFloorLayout.at(i).push_back(block);
        }
    }

    currentElevatorNum = 0;
    currentFloorNum = 0;

    movementTimer.setInterval(1000);
    movementTimer.start();


    connect(ecs->getElevators().front()->getBell(), SIGNAL(ringBellSignal()), this, SLOT(ringBellGUI()));
    connect(&bellTimer, SIGNAL(timeout()), this, SLOT(clearBellGUI()));

    connect(ecs->getElevators().front()->getDisplay(), SIGNAL(displayFloorSignal(int)), this, SLOT(displayFloorGUI(int)));
    connect(ecs->getElevators().front()->getDisplay(), SIGNAL(displayMessageSignal(string)), this, SLOT(displayMessageGUI(string)));

    connect(ecs->getElevators().front()->getAudioSystem(), SIGNAL(outputAudioMessageSignal(string)), this, SLOT(outputAudioMessageGUI(string)));

    connect(ui->upButton, SIGNAL(clicked()), this, SLOT(clickUpButton()));
    connect(ui->downButton, SIGNAL(clicked()), this, SLOT(clickDownButton()));

    connect(ui->changeFloorButton, SIGNAL(clicked()), this, SLOT(changeFloor()));

    connect(ui->changeElevatorButton, SIGNAL(clicked()), this, SLOT(changeElevator()));

    connect(&movementTimer, SIGNAL(timeout()), this, SLOT(moveElevators()));


    //maybe make boxes for each elevator door and each floor door, or can just do 1 and 1, ur choice
    //change ready to leave after ten seconds to happen after a timer


}

MainWindow::~MainWindow()
{
    delete ui;
    delete ecs;
    for(int i = 0; i < qDestButtons.size(); i++){
        delete qDestButtons.at(i);
    }
    for(int i = 0; i < NUM_ELEVATORS; i++){
        for(int j = 0; j < NUM_FLOORS; j++){
            delete elevatorFloorLayout.at(i).at(j);
        }
    }
}


void MainWindow::ringBellGUI(){
    ui->bellTextBrowser->append("ring ring");
    bellTimer.setSingleShot(true);
    bellTimer.start(3000);
}

void MainWindow::clearBellGUI(){
    ui->bellTextBrowser->clear();
}

void MainWindow::displayFloorGUI(int floorNum){
    ui->displayTextBrowser->clear();
    ui->displayTextBrowser->append(QString::number(floorNum));
}

void MainWindow::displayMessageGUI(string message){
    ui->displayTextBrowser->clear();
    ui->displayTextBrowser->append(QString::fromStdString(message));
}

void MainWindow::outputAudioMessageGUI(string audioMessage){
    ui->audioTextBrowser->clear();
    ui->audioTextBrowser->append(QString::fromStdString(audioMessage));
}

void MainWindow::clickUpButton(){
    ecs->getFloors().at(currentFloorNum)->pressUpButton();
}

void MainWindow::clickDownButton(){
    ecs->getFloors().at(currentFloorNum)->pressDownButton();
}

void MainWindow::clickDestinationButton(int destButtonNum){
   ecs->getElevators().at(currentElevatorNum)->pressDestinationButton(destButtonNum);
}

void MainWindow::changeFloor(){
    currentFloorNum++;
    if(currentFloorNum == NUM_FLOORS){
        currentFloorNum = 0;
    }
    ui->selectedFloorOnGUI->clear();
    ui->selectedFloorOnGUI->append(QString::number(currentFloorNum+1));
}

void MainWindow::changeElevator(){
    currentElevatorNum++;
    if(currentElevatorNum == NUM_ELEVATORS){
        currentElevatorNum = 0;
    }
    ui->selectedElevatorOnGUI->clear();
    ui->selectedElevatorOnGUI->append(QString::number(currentElevatorNum+1));
}

void MainWindow::moveElevators(){
    ecs->moveElevatorsTowardsDestination();
}


