#include <QCoreApplication>

#include <src/exithandler.h>
#include <src/entities/sensor/simsensor/simsensor.h>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ExitHandler::setApplication(&a);
    ExitHandler::setup();

    Constants *constants = new Constants(QString(PROJECT_PATH) + "/src/constants/constants.json");
    SensorClient *sensorClient = new SensorClient(constants);
    BaseSensor *sensor = new SimSensor(sensorClient, constants);

    sensor->start();

    bool exec = a.exec();

    sensor->stopEntity();
    sensor->wait();

    return exec;
}
