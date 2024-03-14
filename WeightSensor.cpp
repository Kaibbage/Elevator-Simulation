#include "WeightSensor.h"

//Constructor
WeightSensor::WeightSensor(int maxWeight){
    this->maxWeight = maxWeight;
}

//Returns whether the weight parameter is higher or lower than the max weight
bool WeightSensor::checkWeightSensorOverload(int weight){
    if(weight > maxWeight){
        return true;
    }
    else{
        return false;
    }
}
