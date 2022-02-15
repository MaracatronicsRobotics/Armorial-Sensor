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

#include "sensorclient.h"

#include <src/utils/text/text.h>

SensorClient::SensorClient(Constants *constants) {
    _constants = constants;
    _sensorServiceAddress = getConstants()->getGRPCAddress();
    _sensorServicePort = getConstants()->getGRPCPort();

    connectToServer();
}

SensorClient::~SensorClient() {
    std::cout << Text::blue("[SENSOR CLIENT] ", true) + Text::bold("Disconnected from gRPC server.\n");
}

bool SensorClient::setRobotStatus(RobotStatus robotStatus) {
    if(!isConnectedToServer()) {
        std::cout << Text::red("[ERROR] ") + Text::bold("SensorClient::setRobotStatus(RobotStatus) failed due connection to server unavailable.\n");
        connectToServer();
        return false;
    }

    grpc::ClientContext context;
    google::protobuf::Empty empty;
    grpc::Status status = _stub->SetRobotStatus(&context, robotStatus, &empty);

    if(!status.ok()) {
       std::cout << Text::red("[ERROR] ") + Text::bold("SensorClient::setRobotStatus(RobotStatus) status returned failed.\n");
       return false;
   }

   return true;
}

bool SensorClient::setAllRobotStatus(QList<RobotStatus> robotsStatus) {
    if(!isConnectedToServer()) {
        std::cout << Text::red("[ERROR] ") + Text::bold("SensorClient::setAllRobotStatus(QList<RobotStatus>) failed due connection to server unavailable.\n");
        connectToServer();
        return false;
    }

    grpc::ClientContext context;
    google::protobuf::Empty empty;
    std::unique_ptr<grpc::ClientWriter<RobotStatus>> writer = _stub->SetAllRobotStatus(&context, &empty);

    for (RobotStatus s : robotsStatus) {
        writer->Write(s);
    }

    return true;
}

bool SensorClient::isConnectedToServer() {
    return (   _channel->GetState(false) == GRPC_CHANNEL_READY
            || _channel->GetState(false) == GRPC_CHANNEL_IDLE
            || _channel->GetState(false) == GRPC_CHANNEL_CONNECTING);
}

void SensorClient::connectToServer() {
    QString serviceAddress = QString("%1:%2").arg(_sensorServiceAddress).arg(_sensorServicePort);
    _channel = grpc::CreateChannel(serviceAddress.toStdString(), grpc::InsecureChannelCredentials());
    _stub = Sensor::SensorService::NewStub(_channel);

    std::cout << Text::blue("[SENSOR CLIENT] ", true) + Text::bold("Connected to gRPC in address ") + Text::green(_sensorServiceAddress.toStdString() + ":" + std::to_string(_sensorServicePort), true) + '\n';
}

Constants* SensorClient::getConstants() {
    if(_constants == nullptr) {
        std::cout << Text::yellow("[WARNING] ", true) + Text::bold("Constants with nullptr value at SensorClient.\n");
    }

    return _constants;
}
