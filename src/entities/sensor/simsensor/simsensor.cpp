/***
 * Maracatronics Robotics
 * Federal University of Pernambuco (UFPE) at Recife
 * http://www.maracatronics.com/
 *
 * This file is part of Armorial project.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***/

#include "simsensor.h"

#include <QNetworkDatagram>

#include <src/utils/text/text.h>
#include <proto/messages.pb.h>
#include <proto/messages_robocup_ssl_robot_status.pb.h>

SimSensor::SimSensor(SensorClient *sensorClient, Constants* constants) {
    _constants = constants;
    _sensorAddress = getConstants()->getSimAddress();
    _sensorPort = getConstants()->getSimPort();
    _sensorClient = sensorClient;
}

QString SimSensor::name() {
    return "SimSensor";
}

void SimSensor::initialization() {
    connectToNetwork();
    std::cout << Text::blue("[SIMSENSOR] ", true) + Text::bold("Connected to grSim in address ") + Text::green(_sensorAddress.toStdString() + ":" + std::to_string(_sensorPort), true) + '\n';
}

void SimSensor::loop() {
    while(_sensorSocket->hasPendingDatagrams()) {
        // Creating auxiliary vars
        QNetworkDatagram datagram;
        Robots_Status robotsStatus;

        // Reading datagram and checking if it is valid
        datagram = _sensorSocket->receiveDatagram();
        if(!datagram.isValid()) {
            continue;
        }

        // Parsing datagram and checkinf if it is valid
        if(robotsStatus.ParseFromArray(datagram.data().data(), datagram.data().size()) == false) {
            std::cout << Text::cyan("[SENSOR] ", true) << Text::red("Robots_Status packet parsing error.", true) + '\n';
            continue;
        }

        // Reading status data and writing
        int statusSize = robotsStatus.robots_status_size();
        for(int i = 0; i < statusSize; i++) {
            // Creating RobotStatus packet
            RobotStatus *robotStatus = new RobotStatus();

            // Creating RobotIdentifier
            RobotIdentifier *robotIdentifier = new RobotIdentifier();
            Color *robotColor = new Color();
            robotColor->set_isblue(/*getConstants()->isBlue()*/false);
            robotIdentifier->set_robotid(robotsStatus.robots_status(i).robot_id());

            // Allocate robot identifier and set infrared status
            robotStatus->set_allocated_robotidentifier(robotIdentifier);
            robotStatus->set_infrared(robotsStatus.robots_status(i).infrared());

            // Set Robot Status in gRPC sensor client
            getSensorClient()->setRobotStatus(*robotStatus);
        }
    }
}

void SimSensor::finalization() {
    finishConnection();
    std::cout << Text::blue("[SIMSENSOR] " , true) + Text::bold("Disconnected from grSim.") + '\n';
}

SensorClient* SimSensor::getSensorClient() {
    if(_sensorClient == nullptr) {
        std::cout << Text::yellow("[WARNING] ") + Text::bold("SimSensor::getVisionClient() returned nullptr.\n");
    }

    return _sensorClient;
}

Constants* SimSensor::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::yellow("[WARNING] ", true) + Text::bold("Constants with nullptr value at SimSensor.\n");
    }

    return _constants;
}

void SimSensor::connectToNetwork() {
    _sensorSocket = new QUdpSocket();

    if(_sensorSocket->isOpen()) {
        _sensorSocket->close();
    }

    // Binding in defined network
    if(_sensorSocket->bind(QHostAddress(_sensorAddress), _sensorPort) == false) {
        std::cout << Text::cyan("[SENSOR] " , true) << Text::red("Error while binding socket.", true) + '\n';
        return ;
    }
}

void SimSensor::finishConnection() {
    if(_sensorSocket->isOpen()) {
        _sensorSocket->close();
    }

    delete _sensorClient;
}
