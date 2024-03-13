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
        for(int j = 0; j < NUM_FLOORS; j++){
            Button* destinationButton = new Button(i, j);
            destinationButtons.push_back(destinationButton);
        }
        Button* openButton = new Button(i, DEFAULT_VALUE);
        Button* closeButton = new Button(i, DEFAULT_VALUE);
        Button* helpButton = new Button(i, DEFAULT_VALUE);
        Bell* bell = new Bell(i);
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
        Button* upButton = new Button(i, DEFAULT_VALUE);
        Button* downButton = new Button(i, DEFAULT_VALUE);
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

    Building* building = new Building(nullptr);


    //creating ecs
    ecs = new ECS(elevators, floors, allFloorSensors, building);

    //giving ecs to elevators and floors
    for(int j = 0; j < NUM_ELEVATORS; j++){
        elevators.at(j)->setECS(ecs);
    }

    for(int i = 0; i < NUM_FLOORS; i++){
        floors.at(i)->setECS(ecs);
    }

    building->setECS(ecs);


    




    //creating destination buttons and connecting
    for(int i = 0; i < NUM_FLOORS; i++){
        IntegerPushButton *button = new IntegerPushButton(i, QString::number(i+1), ui->centralwidget);
        button->setGeometry(500, 520 - (50*i), 83, 25);
        qDestButtons.push_back(button);

        connect(button, SIGNAL(intValueClicked(int)), this, SLOT(clickDestinationButton(int)));
        connect(ecs->getElevators().front()->getDestinationButtons().at(i), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationDestinationButton(int, int, bool))); //have to change in change elevator
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
        elevatorDoorLayout.at(i)->setStyleSheet("background-color: red;");
        ui->addWeightButton->setDisabled(true);
        ui->removeWeightButton->setDisabled(true);
        ui->blockDoorButton->setDisabled(true);
        ui->openDoorButton->setDisabled(true);
        ui->closeDoorButton->setDisabled(true);
    }
    for(int i = 0; i < NUM_FLOORS; i++){
        QTextBrowser *fDoor = new QTextBrowser(ui->centralwidget);
        fDoor->setGeometry(50, 500 - (60*i), 30, 60);
        floorDoorLayout.push_back(fDoor);
        connect(ecs->getFloors().at(i)->getFloorDoor(), SIGNAL(sendDoorOpenSignal(int, bool)), this, SLOT(manageFloorDoor(int, bool)));
        floorDoorLayout.at(i)->setStyleSheet("background-color: red;");
    }

    //Setting base elevators and floor on GUI to be first elevator and first floor
    currentElevatorNum = 0;
    currentFloorNum = 0;

    movementTimer.setInterval(TIME_BETWEEN_MOVEMENT);
    movementTimer.start();


    //connecting everything to first floor and first elevator (have to change all these in change floor/ change elevator
    //elevators
    connect(ecs->getElevators().front()->getBell(), SIGNAL(ringBellSignal(int)), this, SLOT(ringBellGUI(int)));
    connect(&bellTimer, SIGNAL(timeout()), this, SLOT(clearBellGUI()));

    connect(ecs->getElevators().front()->getDisplay(), SIGNAL(displayFloorSignal(int, int)), this, SLOT(displayFloorGUI(int, int)));
    connect(ecs->getElevators().front()->getDisplay(), SIGNAL(displayMessageSignal(int, string)), this, SLOT(displayMessageGUI(int, string)));

    connect(ecs->getElevators().front()->getAudioSystem(), SIGNAL(outputAudioMessageSignal(int, string)), this, SLOT(outputAudioMessageGUI(int, string)));

    connect(ecs->getElevators().front()->getHelpButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationHelpButton(int, int, bool)));

    connect(ecs->getElevators().front()->getOpenButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationOpenButton(int, int, bool)));
    connect(ecs->getElevators().front()->getCloseButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationCloseButton(int, int, bool)));


    //floors
    connect(ecs->getFloors().front()->getUpButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationUpButton(int, int, bool)));
    connect(ecs->getFloors().front()->getDownButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationDownButton(int, int, bool)));



    //connecting clickUpButton and clickDownButton to the functions
    connect(ui->upButton, SIGNAL(clicked()), this, SLOT(clickUpButton()));
    connect(ui->downButton, SIGNAL(clicked()), this, SLOT(clickDownButton()));


    //connecting stuff for weight
    connect(ui->addWeightButton, SIGNAL(clicked()), this, SLOT(addWeightSlot()));
    connect(ui->removeWeightButton, SIGNAL(clicked()), this, SLOT(removeWeightSlot()));

    //connecting stuff for block door light sensor
    connect(ui->blockDoorButton, SIGNAL(clicked()), this, SLOT(blockDoor()));

    //connecting stuff for fire
    connect(ui->buildingFireButton, SIGNAL(clicked()), this, SLOT(buildingFire()));
    connect(ui->fireButton, SIGNAL(clicked()), this, SLOT(elevatorFire()));
    connect(ecs, SIGNAL(outOfOrderSignal(int)), this, SLOT(outOfOrder(int)));

    //connecting stuff for power outage
    connect(ui->buildingPowerOutageButton, SIGNAL(clicked()), this, SLOT(buildingPowerOutage()));

    //connecting stuff for help button
    connect(ui->helpButton, SIGNAL(clicked()), this, SLOT(helpButtonPressed()));
    connect(ui->talkButton, SIGNAL(clicked()), this, SLOT(talkButtonPressed()));


    //connecting change elevator and change floor buttons to the required slots
    connect(ui->changeFloorButton, SIGNAL(clicked()), this, SLOT(changeFloor()));
    connect(ui->changeElevatorButton, SIGNAL(clicked()), this, SLOT(changeElevator()));

    //Creating timer to move when needed
    connect(&movementTimer, SIGNAL(timeout()), this, SLOT(moveElevators()));

    //Showing which elevator and floor are currently being shown
    ui->displayTextBrowser->append(QString::number(ecs->getElevatorFloorNum(0)+1));
    showElevatorNum(currentElevatorNum);
    showFloorNum(currentFloorNum);

    //showing weight stuff
    ui->addWeightTextEdit->appendPlainText(QString::number(0));
    ui->currentWeightTextBrowser->append(QString::number(ecs->getElevators().front()->getCurrentWeight()));

    //showing initial positions of all elevators
    for(int i = 0; i < NUM_ELEVATORS; i++){
        elevatorFloorLayout.at(i).at(ecs->getElevators().at(i)->getElevatorFloorNum())->setStyleSheet("background-color: lightblue;");
    }


    //connecting one thing to ecs
    connect(ecs, SIGNAL(goBackToFloorDisplaySignal(int)), this, SLOT(goBackToFloorDisplay(int)));

    //connecting building stuff for help button
    connect(building, SIGNAL(buildingSafetyCalledSignal(int)), this, SLOT(buildingSafetyCalled(int)));
    connect(building, SIGNAL(passengerTalkedSignal(int)), this, SLOT(passengerTalked(int)));
    connect(building, SIGNAL(called911Signal(int)), this, SLOT(called911(int)));

    //connecting stuff for open and close door buttons
    connect(ui->openDoorButton, SIGNAL(pressed()), this, SLOT(openDoorButtonPressed()));
    connect(ui->closeDoorButton, SIGNAL(pressed()), this, SLOT(closeDoorButtonPressed()));

    connect(ui->openDoorButton, SIGNAL(released()), this, SLOT(openDoorButtonLetGo()));
    connect(ui->closeDoorButton, SIGNAL(released()), this, SLOT(closeDoorButtonLetGo()));


    //Setting buttons that shouldn't be clickable at the start to be disabled
    ui->addWeightButton->setDisabled(true);
    ui->removeWeightButton->setDisabled(true);
    ui->blockDoorButton->setDisabled(true);
    ui->talkButton->setDisabled(true);
    ui->openDoorButton->setDisabled(true);
    ui->closeDoorButton->setDisabled(true);



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


void MainWindow::ringBellGUI(int elevatorNum){
    if(elevatorNum == currentElevatorNum){
        ui->bellTextBrowser->append("ring ring");
        bellTimer.setSingleShot(true);
        bellTimer.start(3000);
    }

    ui->console->append("bell rings for Elevator " + QString::number(currentElevatorNum+1));
}

void MainWindow::clearBellGUI(){
    ui->bellTextBrowser->clear();
}

void MainWindow::displayFloorGUI(int elevatorNum, int floorNum){
    if(elevatorNum == currentElevatorNum){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::number(floorNum+1));
    }

    ui->console->append("display updated for Elevator " + QString::number(elevatorNum+1) + ": " + QString::number(floorNum+1));

}

void MainWindow::displayMessageGUI(int elevatorNum, string message){
    if(elevatorNum == currentElevatorNum){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::fromStdString(message));
    }

    ui->console->append("display updated for Elevator " + QString::number(elevatorNum+1) + ": " + QString::fromStdString(message));

}

void MainWindow::outputAudioMessageGUI(int elevatorNum, string audioMessage){
    if(elevatorNum == currentElevatorNum){
        ui->audioTextBrowser->clear();
        ui->audioTextBrowser->append(QString::fromStdString(audioMessage));
    }

    if(audioMessage != ""){
        ui->console->append("audioMessage output for Elevator " + QString::number(elevatorNum+1) + ": " + QString::fromStdString(audioMessage));
    }

}

void MainWindow::clickUpButton(){
    ecs->getFloors().at(currentFloorNum)->pressUpButton();

    ui->console->append("Up Button clicked for Floor " + QString::number(currentFloorNum+1));
}

void MainWindow::clickDownButton(){
    ecs->getFloors().at(currentFloorNum)->pressDownButton();
    ui->console->append("Down Button clicked for Floor " + QString::number(currentFloorNum +1));
}

void MainWindow::clickDestinationButton(int destButtonNum){
   ecs->getElevators().at(currentElevatorNum)->pressDestinationButton(destButtonNum);
   ui->console->append("Destination Button " + QString::number(destButtonNum+1) + " clicked for Elevator " + QString::number(currentElevatorNum+1));
}

void MainWindow::changeFloor(){
    currentFloorNum++;
    if(currentFloorNum == NUM_FLOORS){
        currentFloorNum = 0;
    }
    ui->selectedFloorOnGUI->clear();
    ui->selectedFloorOnGUI->append(QString::number(currentFloorNum+1));

    //connecting needed stuff
    connect(ecs->getFloors().at(currentFloorNum)->getUpButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationUpButton(int, int, bool)));
    connect(ecs->getFloors().at(currentFloorNum)->getDownButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationDownButton(int, int, bool)));

    //clearing buttons
    ui->upButton->setStyleSheet("");
    ui->downButton->setStyleSheet("");

    //updating buttons
    if(ecs->getFloors().at(currentFloorNum)->getUpButton()->isIlluminated()){
        ui->upButton->setStyleSheet("background-color: yellow;");
    }
    if(ecs->getFloors().at(currentFloorNum)->getDownButton()->isIlluminated()){
        ui->downButton->setStyleSheet("background-color: yellow;");
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
    connect(ecs->getElevators().at(currentElevatorNum)->getBell(), SIGNAL(ringBellSignal(int)), this, SLOT(ringBellGUI(int)));

    connect(ecs->getElevators().at(currentElevatorNum)->getDisplay(), SIGNAL(displayFloorSignal(int, int)), this, SLOT(displayFloorGUI(int, int)));
    connect(ecs->getElevators().at(currentElevatorNum)->getDisplay(), SIGNAL(displayMessageSignal(int, string)), this, SLOT(displayMessageGUI(int, string)));

    connect(ecs->getElevators().at(currentElevatorNum)->getAudioSystem(), SIGNAL(outputAudioMessageSignal(int, string)), this, SLOT(outputAudioMessageGUI(int, string)));

    connect(ecs->getElevators().at(currentElevatorNum)->getHelpButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationHelpButton(int, int, bool)));

    connect(ecs->getElevators().at(currentElevatorNum)->getOpenButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationOpenButton(int, int, bool)));
    connect(ecs->getElevators().at(currentElevatorNum)->getCloseButton(), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationCloseButton(int, int, bool)));




    //updating buttons and connecting stuff
    for(int i = 0; i < NUM_FLOORS; i++){
        connect(ecs->getElevators().at(currentElevatorNum)->getDestinationButtons().at(i), SIGNAL(isIlluminatedSignal(int, int, bool)), this, SLOT(manageIlluminationDestinationButton(int, int, bool)));

        //clearing dest buttons
        qDestButtons.at(i)->setStyleSheet("");
        if(ecs->getElevators().at(currentElevatorNum)->getDestinationButtons().at(i)->isIlluminated()){
            //updating dest buttons
            qDestButtons.at(i)->setStyleSheet("background-color: yellow;");
        }
    }


    //updating display (maybe check if weight is overloaded and display msg if so)
    if(ecs->getElevators().at(currentElevatorNum)->getWeightOverload()){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::fromStdString("Weight limit exceeded, please lower weight"));

        ui->audioTextBrowser->clear();
        ui->audioTextBrowser->append(QString::fromStdString("Weight limit exceeded, please lower weight"));
    }
    else if(ecs->getElevators().at(currentElevatorNum)->getLightSensorBlockedTwice()){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::fromStdString("Stop blocking the door"));

        ui->audioTextBrowser->clear();
        ui->audioTextBrowser->append(QString::fromStdString("Stop blocking the door"));
    }
    else if(ecs->getElevators().at(currentElevatorNum)->getFireTime()){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::fromStdString("This elevator has a fire, please exit when the doors open at the next floor"));

        ui->audioTextBrowser->clear();
        ui->audioTextBrowser->append(QString::fromStdString("This elevator has a fire, please exit when the doors open at the next floor"));
    }
    else if(ecs->getElevators().at(currentElevatorNum)->getPowerOutTime()){
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::fromStdString("This elevator is out of power, please exit when the doors open at the next floor"));

        ui->audioTextBrowser->clear();
        ui->audioTextBrowser->append(QString::fromStdString("This elevator is out of power, please exit when the doors open at the next floor"));
    }
    else{
        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::number(ecs->getElevatorFloorNum(currentElevatorNum)+1));

        ui->audioTextBrowser->clear();
    }

    //If this elevator is out of order then disabling stuff
    if(!ecs->getElevators().at(currentElevatorNum)->getOutOfOrder()){
        greyOutButtons(currentElevatorNum, false);
    }
    else{
        greyOutButtons(currentElevatorNum, true);

        ui->displayTextBrowser->clear();
        ui->displayTextBrowser->append(QString::fromStdString("Please exit immediately"));

        ui->audioTextBrowser->clear();
        ui->audioTextBrowser->append(QString::fromStdString("Please exit immediately"));
    }

    //If door is open or closed then allowing user to do certain actions
    if(ecs->getElevators().at(currentElevatorNum)->isDoorOpen()){
        ui->addWeightButton->setDisabled(false);
        ui->removeWeightButton->setDisabled(false);
        ui->blockDoorButton->setDisabled(false);
        ui->openDoorButton->setDisabled(false);
        ui->closeDoorButton->setDisabled(false);

    }
    else{
        ui->addWeightButton->setDisabled(true);
        ui->removeWeightButton->setDisabled(true);
        ui->blockDoorButton->setDisabled(true);
        ui->openDoorButton->setDisabled(true);
        ui->closeDoorButton->setDisabled(true);
    }

    //Allowing options for if help button has been clicked
    if(ecs->getElevators().at(currentElevatorNum)->getNeedHelp()){
        ui->talkButton->setDisabled(false);
        ui->helpButton->setStyleSheet("background-color: yellow;");
    }
    else{
        ui->talkButton->setDisabled(true);
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

    ui->console->append("Elevator " + QString::number(elevatorNum+1) + " has reached Floor " + QString::number(floorNum+1));
}

void MainWindow::manageIlluminationDestinationButton(int elevatorNum, int destinationNum, bool illuminate){
    if(illuminate == true){
        qDestButtons.at(destinationNum)->setStyleSheet("background-color: yellow;");
        ui->console->append("Destination button " + QString::number(destinationNum+1) + " in Elevator " + QString::number(elevatorNum+1) + " has lit up");
    }
    else{
        qDestButtons.at(destinationNum)->setStyleSheet("");
        ui->console->append("Destination button " + QString::number(destinationNum+1) + " in Elevator " + QString::number(elevatorNum+1) + " has turned off the light");
    }
}

void MainWindow::manageIlluminationUpButton(int floorNum, int destinationNum, bool illuminate){
    if(illuminate == true && destinationNum < 0){
        ui->upButton->setStyleSheet("background-color: yellow;");
        ui->console->append("Up button in Floor " + QString::number(floorNum+1) + " has lit up");
    }
    else{
        ui->upButton->setStyleSheet("");
        ui->console->append("Up button in Floor " + QString::number(floorNum+1) + " has turned off the light");
    }
}

void MainWindow::manageIlluminationDownButton(int floorNum, int destinationNum, bool illuminate){
    if(illuminate == true && destinationNum < 0){
        ui->downButton->setStyleSheet("background-color: yellow;");
        ui->console->append("Down button in Floor " + QString::number(floorNum+1) + " has lit up");
    }
    else{
        ui->downButton->setStyleSheet("");
        ui->console->append("Down button in Floor " + QString::number(floorNum+1) + " has turned off the light");
    }
}

void MainWindow::manageElevatorDoor(int elevatorNum, bool open){
    if(open == true){
        elevatorDoorLayout.at(elevatorNum)->setStyleSheet("background-color: green;");
        ui->addWeightButton->setDisabled(false);
        ui->removeWeightButton->setDisabled(false);
        ui->blockDoorButton->setDisabled(false);
        ui->openDoorButton->setDisabled(false);
        ui->closeDoorButton->setDisabled(false);
        ui->console->append("door for Elevator " + QString::number(elevatorNum+1) + " has opened");
    }
    else{
        elevatorDoorLayout.at(elevatorNum)->setStyleSheet("background-color: red;");
        ui->addWeightButton->setDisabled(true);
        ui->removeWeightButton->setDisabled(true);
        ui->blockDoorButton->setDisabled(true);
        ui->openDoorButton->setDisabled(true);
        ui->closeDoorButton->setDisabled(true);
        ui->console->append("door for Elevator " + QString::number(elevatorNum+1) + " has closed");
    }

}
void MainWindow::manageFloorDoor(int floorNum, bool open){
    if(open == true){
        floorDoorLayout.at(floorNum)->setStyleSheet("background-color: green;");
        ui->console->append("door for Floor " + QString::number(floorNum+1) + " has closed");
    }
    else{
        floorDoorLayout.at(floorNum)->setStyleSheet("background-color: red;");
        ui->console->append("door for Floor " + QString::number(floorNum+1) + " has closed");
    }

}

void MainWindow::addWeightSlot(){
    int weightToAdd = ui->addWeightTextEdit->toPlainText().toInt();
    ecs->getElevators().at(currentElevatorNum)->increaseWeight(weightToAdd);
    ui->addWeightTextEdit->clear();
    ui->addWeightTextEdit->appendPlainText(QString::number(0));

    ui->currentWeightTextBrowser->clear();
    ui->currentWeightTextBrowser->append(QString::number(ecs->getElevators().at(currentElevatorNum)->getCurrentWeight()));

    ui->console->append(QString::number(weightToAdd) + " units of weight has been added");

}

void MainWindow::removeWeightSlot(){
    int weightToRemove = ui->addWeightTextEdit->toPlainText().toInt();
    ecs->getElevators().at(currentElevatorNum)->decreaseWeight(weightToRemove);
    ui->addWeightTextEdit->clear();
    ui->addWeightTextEdit->appendPlainText(QString::number(0));

    ui->currentWeightTextBrowser->clear();
    ui->currentWeightTextBrowser->append(QString::number(ecs->getElevators().at(currentElevatorNum)->getCurrentWeight()));

    ui->console->append(QString::number(weightToRemove) + " units of weight has been removed");
}

void MainWindow::goBackToFloorDisplay(int floorNum){
    displayFloorGUI(currentElevatorNum, floorNum);
    outputAudioMessageGUI(currentElevatorNum, "");
}

void MainWindow::blockDoor(){
    ecs->getElevators().at(currentElevatorNum)->blockLightSensor();
    ui->console->append("The door has been blocked for Elevator" + QString::number(currentElevatorNum+1));
}

void MainWindow::elevatorFire(){
    ecs->fireRequestFromElevator(currentElevatorNum);
    ui->console->append("There is a fire in Elevator" + QString::number(currentElevatorNum+1));
}

void MainWindow::outOfOrder(int elevatorNum){
    greyOutButtons(elevatorNum, true);
    ui->console->append("Elevator" + QString::number(currentElevatorNum+1) + " is now out of order");

}

void MainWindow::greyOutButtons(int elevatorNum, bool grey){
    if(elevatorNum == currentElevatorNum){
        if(!grey){
            for(int i = 0; i < NUM_FLOORS; i++){
                qDestButtons.at(i)->setDisabled(false);
                qDestButtons.at(i)->setStyleSheet("");
            }

            ui->blockDoorButton->setDisabled(false);
            ui->addWeightButton->setDisabled(false);
            ui->removeWeightButton->setDisabled(false);
            ui->helpButton->setDisabled(false);
            ui->fireButton->setDisabled(false);
            ui->openDoorButton->setDisabled(false);
            ui->closeDoorButton->setDisabled(false);
            ui->talkButton->setDisabled(false);

            ui->blockDoorButton->setStyleSheet("");
            ui->addWeightButton->setStyleSheet("");
            ui->removeWeightButton->setStyleSheet("");
            ui->helpButton->setStyleSheet("");
            ui->fireButton->setStyleSheet("");
            ui->openDoorButton->setStyleSheet("");
            ui->closeDoorButton->setStyleSheet("");
            ui->talkButton->setStyleSheet("");


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
            ui->talkButton->setDisabled(true);

            ui->blockDoorButton->setStyleSheet("background-color: gray;");
            ui->addWeightButton->setStyleSheet("background-color: gray;");
            ui->removeWeightButton->setStyleSheet("background-color: gray;");
            ui->helpButton->setStyleSheet("background-color: gray;");
            ui->fireButton->setStyleSheet("background-color: gray;");
            ui->openDoorButton->setStyleSheet("background-color: gray;");
            ui->closeDoorButton->setStyleSheet("background-color: gray;");
            ui->talkButton->setStyleSheet("background-color: gray;");

        }
    }

}

void MainWindow::buildingFire(){
    ecs->fireRequestFromBuilding();

    ui->console->append("There is a fire in the building");
}

void MainWindow::buildingPowerOutage(){
    ecs->powerOutageRequest();

    ui->console->append("There is a power outage in the building");
}

void MainWindow::helpButtonPressed(){
    ecs->getElevators().at(currentElevatorNum)->pressHelpButton();
    ui->talkButton->setDisabled(false);

    ui->console->append("The help button has been pressed for Elevator " + QString::number(currentElevatorNum+1));
}
void MainWindow::buildingSafetyCalled(int elevatorNum){
    ui->console->append("Elevator " + QString::number(elevatorNum+1) + " has called building safety.");
    ui->talkButton->setDisabled(true);
}
void MainWindow::passengerTalked(int elevatorNum){
    ui->console->append("Elevator " + QString::number(elevatorNum+1) + " has talked to building safety. Proceeding as normal.");
    ui->talkButton->setDisabled(true);
}
void MainWindow::called911(int elevatorNum){
    ui->console->append("Elevator " + QString::number(elevatorNum+1) + " has not spoken for 5 seconds, 911 has been called.");
}
void MainWindow::talkButtonPressed(){
    ecs->getElevators().at(currentElevatorNum)->talk();
}

void MainWindow::manageIlluminationHelpButton(int elevatorNum, int destinationNum, bool isIlluminated){
    if(destinationNum == DEFAULT_VALUE){
        if(isIlluminated){
            ui->helpButton->setStyleSheet("background-color: yellow;");
            ui->console->append("Help button in Elevator " + QString::number(elevatorNum+1) + " has lit up");
        }
        else{
            ui->helpButton->setStyleSheet("");
            ui->console->append("Help button in Elevator " + QString::number(elevatorNum+1) + " has turned off the light");
        }

    }
}

void MainWindow::openDoorButtonPressed(){
    ecs->getElevators().at(currentElevatorNum)->pressOpenDoorButton();
    ui->console->append("The open door button has been pressed for Elevator " + QString::number(currentElevatorNum+1));
}
void MainWindow::closeDoorButtonPressed(){
    ecs->getElevators().at(currentElevatorNum)->pressCloseDoorButton();
    ui->console->append("The close door button has been pressed for Elevator " + QString::number(currentElevatorNum+1));
}

void MainWindow::openDoorButtonLetGo(){
    ecs->getElevators().at(currentElevatorNum)->letGoOpenDoorButton();
    ui->console->append("The open door button has been released for Elevator " + QString::number(currentElevatorNum+1));
}
void MainWindow::closeDoorButtonLetGo(){
    ecs->getElevators().at(currentElevatorNum)->letGoCloseDoorButton();
}

void MainWindow::manageIlluminationOpenButton(int elevatorNum, int destinationNum, bool isIlluminated){
    if(destinationNum == DEFAULT_VALUE){
        if(isIlluminated){
            ui->openDoorButton->setStyleSheet("background-color: yellow;");
            ui->console->append("Open button in Elevator " + QString::number(elevatorNum+1) + " has lit up");
        }
        else{
            ui->openDoorButton->setStyleSheet("");
            ui->console->append("Open button in Elevator " + QString::number(elevatorNum+1) + " has turned off the light");
        }

    }
}

void MainWindow::manageIlluminationCloseButton(int elevatorNum, int destinationNum, bool isIlluminated){
    if(destinationNum== DEFAULT_VALUE){
        if(isIlluminated){
            ui->closeDoorButton->setStyleSheet("background-color: yellow;");
            ui->console->append("Close button in Elevator " + QString::number(elevatorNum+1) + " has lit up");
        }
        else{
            ui->closeDoorButton->setStyleSheet("");
            ui->console->append("Close button in Elevator " + QString::number(elevatorNum+1) + " has turned off the light");
        }

    }
}
