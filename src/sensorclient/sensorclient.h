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

#ifndef SENSORCLIENT_H
#define SENSORCLIENT_H

#include <grpc/grpc.h>

#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <QList>
#include <proto/sensorservice.grpc.pb.h>

#include <src/constants/constants.h>

class SensorClient
{
public:
    SensorClient(Constants* constants);
    ~SensorClient();

    bool setRobotStatus(RobotStatus robotStatus);
    bool setAllRobotStatus(QList<RobotStatus> robotsStatus);

protected:
    bool isConnectedToServer();
    void connectToServer();

private:
    std::unique_ptr<Sensor::SensorService::Stub> _stub;
    std::shared_ptr<grpc::Channel> _channel;

    // Constants
    Constants *_constants;
    Constants* getConstants();

    // Network
    QString _sensorServiceAddress;
    quint16 _sensorServicePort;
};

#endif // SENSORCLIENT_H
