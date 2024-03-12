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

    
    //creating elevators
    vector<Elevator*> elevators;

    for(int i = 0; i < NUM_ELEVATORS; i++){
        int elevatorNum = i;
        vector<Button*> destinationButtons;
        for(int i = 0; i < NUM_FLOORS; i++){
            Button* destinationButton = new Button(i);
            destinationButtons.push_back(destinationButton);
        }
        Button* openButton = new Button();
        Button* closeButton = new Button();
        Button* helpButton = new Button();
        Bell* bell = new Bell();
        Door* eDoor = new Door(i);
        Display* display = new Display();
        AudioSystem* audioSystem = new AudioSystem();
        WeightSensor* weightSensor = new WeightSensor();
        LightSensor* lightSensor = new LightSensor();

        Elevator* elevator1 = new Elevator(elevatorNum, destinationButtons, openButton, closeButton, helpButton, bell, eDoor, display, audioSystem, weightSensor, lightSensor, nullptr);
        elevators.push_back(elevator1);
    }



    //creating floors
    vector<Floor*> floors;

    for(int i = 0; i < NUM_FLOORS; i++){
        int floorNum = i;
        Button* upButton = new Button();
        Button* downButton = new Button();
        Door* floorDoor = new Door(i);

        Floor* floor = new Floor(floorNum, upButton, downButton, floorDoor, nullptr);
        floors.push_back(floor);
    }


    //creating floor sensors
    vector<vector<FloorSensor*>> allFloorSensors;
    for(int i = 0; i < NUM_ELEVATORS; i++){
        allFloorSensors.push_back({});
        for(int j = 0; j < NUM_FLOORS; j++){
            FloorSensor* floorSensor = new FloorSensor(j, elevators.at(i));
            allFloorSensors.at(i).push_back(floorSensor);
            connect(floorSensor, SIGNAL(signalArrivedAtFloor(int, int)), this, SLOT(floorSensorArrivedDisplay(int, int)));
        }
    }


    //creating ecs
    ecs = new ECS(elevators, floors, allFloorSensors, nullptr);

    //giving ecs to elevators and floors
    for(int j = 0; j < NUM_ELEVATORS; j++){
        elevators.at(j)->setECS(ecs);
    }

    for(int i = 0; i < NUM_FLOORS; i++){
        floors.at(i)->setECS(ecs);
    }
    




    //creating destination buttons and connecting
    for(int i = 0; i < NUM_FLOORS; i++){
        IntegerPushButton *button = new IntegerPushButton(i, QString::number(i+1), ui->centralwidget);
        button->setGeometry(500, 520 - (50*i), 83, 25);
        qDestButtons.push_back(button);

        connect(button, SIGNAL(intValueClicked(int)), this, SLOT(clickDestinationButton(int)));
        connect(ecs->getElevators().front()->getDestinationButtons().at(i), SIGNAL(isIlluminatedSignal(int, bool)), this, SLOT(manageIlluminationDestinationButton(int, bool))); //have to change in change elevator
    }


    //creating visual position representation and connecting
    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevatorFloorLayout.push_back({});
        for(int j = 0; j < NUM_FLOORS; j++){
            QTextBrowser* block = new QTextBrowser(ui->centralwidget);
            block->setGeometry(100 + (60*i), 500 - (60*j), 60, 60);
            elevatorFloorLayout.at(i).push_back(block);
        }
    }

    //creating labels for visual representations
    for(int i = 0; i < NUM_ELEVATORS; i++){
        QLabel* elevatorLabel = new QLabel("E" + QString::number(i+1), ui->centralwidget);
        elevatorLabel->setGeometry(120 + (60*i), 563, 40, 10);
        elevatorLabels.push_back(elevatorLabel);
    }

    for(int i = 0; i < NUM_FLOORS; i++){
        QLabel* floorLabel = new QLabel("F" + QString::number(i+1), ui->centralwidget);
        floorLabel->setGeometry(82, 520 - (60*i), 40, 10);
        floorLabels.push_back(floorLabel);
    }

    //creating visual door representations for elevators and floors
    for(int i = 0; i < NUM_ELEVATORS; i++){
        QTextBrowser *eDoor = new QTextBrowser(ui->centralwidget);
        eDoor->setGeometry(100 + (60*i), 580, 60, 30);
        elevatorDoorLayout.push_back(eDoor);
        connect(ecs->getElevators().at(i)->getElevatorDoor(), SIGNAL(sendDoorOpenSignal(int, bool)), this, SLOT(manageElevatorDoor(int, bool)));
        manageElevatorDoor(i, false);
    }
    for(int i = 0; i < NUM_FLOORS; i++){
        QTextBrowser *fDoor = new QTextBrowser(ui->centralwidget);
        fDoor->setGeometry(50, 500 - (60*i), 30, 60);
        floorDoorLayout.push_back(fDoor);
        connect(ecs->getFloors().at(i)->getFloorDoor(), SIGNAL(sendDoorOpenSignal(int, bool)), this, SLOT(manageFloorDoor(int, bool)));
        manageFloorDoor(i, false);
    }

    //Setting base elevators and floor on GUI to be first elevator and first floor
    currentElevatorNum = 0;
    currentFloorNum = 0;

    movementTimer.setInterval(TIME_BETWEEN_MOVEMENT);
    movementTimer.start();


    //connecting everything to first floor and first elevator (have to change all these in change floor/ change elevator
    connect(ecs->getElevators().front()->getBell(), SIGNAL(ringBellSignal()), this, SLOT(ringBellGUI()));
    connect(&bellTimer, SIGNAL(timeout()), this, SLOT(clearBellGUI()));

    connect(ecs->getElevators().front()->getDisplay(), SIGNAL(displayFloorSignal(int, int)), this, SLOT(displayFloorGUI(int, int)));
    connect(ecs->getElevators().front()->getDisplay(), SIGNAL(displayMessageSignal(int, string)), this, SLOT(displayMessageGUI(int, string)));

    connect(ecs->getElevators().front()->getAudioSystem(), SIGNAL(outputAudioMessageSignal(int, string)), this, SLOT(outputAudioMessageGUI(int, string)));



    connect(ecs->getFloors().front()->getUpButton(), SIGNAL(isIlluminatedSignal(int, bool)), this, SLOT(manageIlluminationUpButton(int, bool)));
    connect(ecs->getFloors().front()->getDownButton(), SIGNAL(isIlluminatedSignal(int, bool)), this, SLOT(manageIlluminationDownButton(int, bool)));



    //connecting clickUpButton and clickDownButton to the functions
    connect(ui->upButton, SIGNAL(clicked()), this, SLOT(clickUpButton()));
    connect(ui->downButton, SIGNAL(clicked()), this, SLOT(clickDownButton()));


    //connecting stuff for weight
    connect(ui->addWeightButton, SIGNAL(clicked()), this, SLOT(addWeightSlot()));
    connect(ui->removeWeightButton, SIGNAL(clicked()), this, SLOT(removeWeightSlot()));

    //connecting stuff for block door light sensor
    connect(ui->blockDoorButton, SIGNAL(clicked()), this, SLOT(blockDoor()));

    //connecting stuff for fire
    connect(ui->fireButton, SIGNAL(clicked()), this, SLOT(elevatorFire()));
    connect(ecs, SIGNAL(outOfOrderSignal()), this, SLOT(outOfOrder()));


    //connecting change elevator and change floor buttons to the required slots
    connect(ui->changeFloorButton, SIGNAL(clicked()), this, SLOT(changeFloor()));
    connect(ui->changeElevatorButton, SIGNAL(clicked()), this, SLOT(changeElevator()));

    //Creating timer to move when needed
    connect(&movementTimer, SIGNAL(timeout()), this, SLOT(moveElevators()));

    //Showing which elevator and floor are currently being shown
    displayFloorGUI(0, ecs->getElevatorFloorNum(0));
    showElevatorNum(currentElevatorNum);
    showFloorNum(currentFloorNum);

    //showing weight stuff
    ui->addWeightTextEdit->appendPlainText(QString::number(0));
    ui->currentWeightTextBrowser->append(QString::number(ecs->getElevators().front()->getCurrentWeight()));

    //showing initial positions of all elevators
    for(int i = 0; i < NUM_ELEVATORS; i++){
        floorSensorArrivedDisplay(i, ecs->getElevators().at(i)->getElevatorFloorNum());
    }


    //connecting one thing to ecs
    connect(ecs, SIGNAL(goBackToFloorDisplaySignal(int)), this, SLOT(goBackToFloorDisplay(int)));


}

MainWindow::~MainWindow()
{
    delete ui;
    delete ecs;
    for(int i = 0; i < NUM_FLOORS; i++){
        delete qDestButtons.at(i);
    }
    for(int i = 0; i < NUM_ELEVATORS; i++){
        for(int j = 0; j < NUM_FLOORS; j++){
            delete elevatorFloorLayout.at(i).at(j);
        }
    }
    for(int i = 0; i < NUM_ELEVATORS; i++){
        delete elevatorDoorLayout.at(i);
    }
    for(int i = 0; i < NUM_FLOORS; i++){
        delete floorDoorLayout.at(i);
    }
    for(int i = 0; i < NUM_ELEVATORS; i++){
        delete elevatorLabels.at(i);
    }
    for(int i = 0; i < NUM_FLOORS; i++){
        delete floorLabels.at(i);
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

void MainWindow::displayFloorGUI(int elevatorNum, int floorNum){
    if(elevatorNum == currentElevatorNum){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::number(floorNum+1));
    }

}

void MainWindow::displayMessageGUI(int elevatorNum, string message){
    if(elevatorNum == currentElevatorNum){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::fromStdString(message));
    }

}

void MainWindow::outputAudioMessageGUI(int elevatorNum, string audioMessage){
    if(elevatorNum == currentElevatorNum){
        ui->audioTextBrowser->clear();
        ui->audioTextBrowser->append(QString::fromStdString(audioMessage));
    }
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

    //connecting needed stuff
    connect(ecs->getFloors().at(currentFloorNum)->getUpButton(), SIGNAL(isIlluminatedSignal(int, bool)), this, SLOT(manageIlluminationUpButton(int, bool)));
    connect(ecs->getFloors().at(currentFloorNum)->getDownButton(), SIGNAL(isIlluminatedSignal(int, bool)), this, SLOT(manageIlluminationDownButton(int, bool)));

    //clearing buttons
    manageIlluminationUpButton(-1, false);
    manageIlluminationDownButton(-1, false);

    //updating buttons
    if(ecs->getFloors().at(currentFloorNum)->getUpButton()->isIlluminated()){
        manageIlluminationUpButton(-1, true);
    }
    if(ecs->getFloors().at(currentFloorNum)->getDownButton()->isIlluminated()){
        manageIlluminationDownButton(-1, true);
    }
}

void MainWindow::changeElevator(){
    currentElevatorNum++;
    if(currentElevatorNum == NUM_ELEVATORS){
        currentElevatorNum = 0;
    }
    ui->selectedElevatorOnGUI->clear();
    ui->selectedElevatorOnGUI->append(QString::number(currentElevatorNum+1));

    //clearing bell
    clearBellGUI();

    //connecting needed stuff
    connect(ecs->getElevators().at(currentElevatorNum)->getBell(), SIGNAL(ringBellSignal()), this, SLOT(ringBellGUI()));

    connect(ecs->getElevators().at(currentElevatorNum)->getDisplay(), SIGNAL(displayFloorSignal(int, int)), this, SLOT(displayFloorGUI(int, int)));
    connect(ecs->getElevators().at(currentElevatorNum)->getDisplay(), SIGNAL(displayMessageSignal(int, string)), this, SLOT(displayMessageGUI(int, string)));

    connect(ecs->getElevators().at(currentElevatorNum)->getAudioSystem(), SIGNAL(outputAudioMessageSignal(int, string)), this, SLOT(outputAudioMessageGUI(int, string)));




    //updating buttons and connecting stuff
    for(int i = 0; i < NUM_FLOORS; i++){
        connect(ecs->getElevators().at(currentElevatorNum)->getDestinationButtons().at(i), SIGNAL(isIlluminatedSignal(int, bool)), this, SLOT(manageIlluminationDestinationButton(int, bool)));

        //clearing dest buttons
        manageIlluminationDestinationButton(i, false);
        if(ecs->getElevators().at(currentElevatorNum)->getDestinationButtons().at(i)->isIlluminated()){
            //updating dest buttons
            manageIlluminationDestinationButton(i, true);
        }
    }


    //updating display (maybe check if weight is overloaded and display msg if so)

    if(ecs->getElevators().at(currentElevatorNum)->getWeightOverload()){
        displayMessageGUI(currentElevatorNum, "Weight limit exceeded, please lower weight");
        outputAudioMessageGUI(currentElevatorNum, "Weight limit exceeded, please lower weight");
    }
    else if(ecs->getElevators().at(currentElevatorNum)->getLightSensorBlockedTwice()){
        displayMessageGUI(currentElevatorNum, "Stop blocking the door");
        outputAudioMessageGUI(currentElevatorNum, "Stop blocking the door");
    }
    else{
        displayFloorGUI(currentElevatorNum, ecs->getElevatorFloorNum(currentElevatorNum));
        outputAudioMessageGUI(currentElevatorNum, "");
    }

    if(!ecs->getElevators().at(currentElevatorNum)->getOutOfOrder()){
        for(int i = 0; i < NUM_FLOORS; i++){
            qDestButtons.at(i)->setDisabled(false);
            qDestButtons.at(i)->setStyleSheet("background-color: white;");
        }

        ui->blockDoorButton->setDisabled(false);
        ui->addWeightButton->setDisabled(false);
        ui->removeWeightButton->setDisabled(false);
        ui->helpButton->setDisabled(false);
        ui->fireButton->setDisabled(false);
        ui->openDoorButton->setDisabled(false);
        ui->closeDoorButton->setDisabled(false);

        ui->blockDoorButton->setStyleSheet("background-color: white;");
        ui->addWeightButton->setStyleSheet("background-color: white;");
        ui->removeWeightButton->setStyleSheet("background-color: white;");
        ui->helpButton->setStyleSheet("background-color: white;");
        ui->fireButton->setStyleSheet("background-color: white;");
        ui->openDoorButton->setStyleSheet("background-color: white;");
        ui->closeDoorButton->setStyleSheet("background-color: white;");

        displayMessageGUI(currentElevatorNum, "Please exit immediately");
        outputAudioMessageGUI(currentElevatorNum, "Please exit immediately");
    }
    else{
        for(int i = 0; i < NUM_FLOORS; i++){
            qDestButtons.at(i)->setDisabled(true);
            qDestButtons.at(i)->setStyleSheet("background-color: gray;");
        }

        ui->blockDoorButton->setDisabled(true);
        ui->addWeightButton->setDisabled(true);
        ui->removeWeightButton->setDisabled(true);
        ui->helpButton->setDisabled(true);
        ui->fireButton->setDisabled(true);
        ui->openDoorButton->setDisabled(true);
        ui->closeDoorButton->setDisabled(true);

        ui->blockDoorButton->setStyleSheet("background-color: gray;");
        ui->addWeightButton->setStyleSheet("background-color: gray;");
        ui->removeWeightButton->setStyleSheet("background-color: gray;");
        ui->helpButton->setStyleSheet("background-color: gray;");
        ui->fireButton->setStyleSheet("background-color: gray;");
        ui->openDoorButton->setStyleSheet("background-color: gray;");
        ui->closeDoorButton->setStyleSheet("background-color: gray;");
    }


    //updating weight stuff
    ui->addWeightTextEdit->clear();
    ui->addWeightTextEdit->appendPlainText(QString::number(0));

    ui->currentWeightTextBrowser->clear();
    ui->currentWeightTextBrowser->append(QString::number(ecs->getElevators().at(currentElevatorNum)->getCurrentWeight()));




}

void MainWindow::moveElevators(){
    ecs->moveElevatorsTowardsDestination();
}

void MainWindow::showFloorNum(int floorNum){
    ui->selectedFloorOnGUI->clear();
    ui->selectedFloorOnGUI->append(QString::number(floorNum+1));
}
void MainWindow::showElevatorNum(int elevatorNum){
    ui->selectedElevatorOnGUI->clear();
    ui->selectedElevatorOnGUI->append(QString::number(elevatorNum+1));
}

void MainWindow::floorSensorArrivedDisplay(int elevatorNum, int floorNum){
    if(floorNum != 0){
        elevatorFloorLayout.at(elevatorNum).at(floorNum-1)->setStyleSheet("background-color: white;");
    }
    elevatorFloorLayout.at(elevatorNum).at(floorNum)->setStyleSheet("background-color: lightblue;");
    if(floorNum != NUM_FLOORS-1){
        elevatorFloorLayout.at(elevatorNum).at(floorNum+1)->setStyleSheet("background-color: white;");
    }
}

void MainWindow::manageIlluminationDestinationButton(int destinationNum, bool illuminate){
    if(illuminate == true){
        qDestButtons.at(destinationNum)->setStyleSheet("background-color: yellow;");
    }
    else{
        qDestButtons.at(destinationNum)->setStyleSheet("background-color: white;");
    }
}

void MainWindow::manageIlluminationUpButton(int destinationNum, bool illuminate){
    if(illuminate == true && destinationNum < 0){
        ui->upButton->setStyleSheet("background-color: yellow;");
    }
    else{
        ui->upButton->setStyleSheet("background-color: white;");
    }
}

void MainWindow::manageIlluminationDownButton(int destinationNum, bool illuminate){
    destinationNum = -1;
    if(illuminate == true && destinationNum < 0){
        ui->downButton->setStyleSheet("background-color: yellow;");
    }
    else{
        ui->downButton->setStyleSheet("background-color: white;");
    }
}

void MainWindow::manageElevatorDoor(int elevatorNum, bool open){
    if(open == true){
        elevatorDoorLayout.at(elevatorNum)->setStyleSheet("background-color: green;");
    }
    else{
        elevatorDoorLayout.at(elevatorNum)->setStyleSheet("background-color: red;");
    }

}
void MainWindow::manageFloorDoor(int floorNum, bool open){
    if(open == true){
        floorDoorLayout.at(floorNum)->setStyleSheet("background-color: green;");
    }
    else{
        floorDoorLayout.at(floorNum)->setStyleSheet("background-color: red;");
    }

}

void MainWindow::addWeightSlot(){
    int weightToAdd = ui->addWeightTextEdit->toPlainText().toInt();
    ecs->getElevators().at(currentElevatorNum)->increaseWeight(weightToAdd);
    ui->addWeightTextEdit->clear();
    ui->addWeightTextEdit->appendPlainText(QString::number(0));

    ui->currentWeightTextBrowser->clear();
    ui->currentWeightTextBrowser->append(QString::number(ecs->getElevators().at(currentElevatorNum)->getCurrentWeight()));

}

void MainWindow::removeWeightSlot(){
    int weightToRemove = ui->addWeightTextEdit->toPlainText().toInt();
    ecs->getElevators().at(currentElevatorNum)->decreaseWeight(weightToRemove);
    ui->addWeightTextEdit->clear();
    ui->addWeightTextEdit->appendPlainText(QString::number(0));

    ui->currentWeightTextBrowser->clear();
    ui->currentWeightTextBrowser->append(QString::number(ecs->getElevators().at(currentElevatorNum)->getCurrentWeight()));
}

void MainWindow::goBackToFloorDisplay(int floorNum){
    displayFloorGUI(currentElevatorNum, floorNum);
    outputAudioMessageGUI(currentElevatorNum, "");
}

void MainWindow::blockDoor(){
    ecs->getElevators().at(currentElevatorNum)->blockLightSensor();
}

void MainWindow::elevatorFire(){
    ecs->fireRequestFromElevator(currentElevatorNum);
}

void MainWindow::outOfOrder(){
    for(int i = 0; i < NUM_FLOORS; i++){
        qDestButtons.at(i)->setDisabled(true);
        qDestButtons.at(i)->setStyleSheet("background-color: gray;");
    }

    ui->blockDoorButton->setDisabled(true);
    ui->addWeightButton->setDisabled(true);
    ui->removeWeightButton->setDisabled(true);
    ui->helpButton->setDisabled(true);
    ui->fireButton->setDisabled(true);
    ui->openDoorButton->setDisabled(true);
    ui->closeDoorButton->setDisabled(true);

    ui->blockDoorButton->setStyleSheet("background-color: gray;");
    ui->addWeightButton->setStyleSheet("background-color: gray;");
    ui->removeWeightButton->setStyleSheet("background-color: gray;");
    ui->helpButton->setStyleSheet("background-color: gray;");
    ui->fireButton->setStyleSheet("background-color: gray;");
    ui->openDoorButton->setStyleSheet("background-color: gray;");
    ui->closeDoorButton->setStyleSheet("background-color: gray;");

}
