#include "WeightSensor.h"

WeightSensor::WeightSensor(int maxWeight){
    this->maxWeight = maxWeight;
}

bool WeightSensor::checkWeightSensorOverload(int weight){
    if(weight > maxWeight){
        return true;
    }
    else{
        return false;
    }
}
