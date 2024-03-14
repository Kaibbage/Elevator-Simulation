#ifndef WEIGHTSENSOR_H
#define WEIGHTSENSOR_H

/*
WeightSensor class is very simple
Represents an elevator weight sensor
Just checks to see if they weight on the elevator is higher than the maximum set weight, default is 1000 units
*/
class WeightSensor {
public:
    WeightSensor(int = 1000);
    bool checkWeightSensorOverload(int weight);

private:
    int maxWeight;

};

#endif
