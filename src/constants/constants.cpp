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


#include "constants.h"
#include <iostream>
#include <src/utils/text/text.h>

Constants::Constants(QString fileName) {
    _fileName = fileName;

    file.setFileName(_fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();

    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());

    QJsonObject docObject = document.object();
    QVariantMap doc_map = docObject.toVariantMap();

    _GRPCAddress = doc_map["GRPCAddress"].toString();
    std::cout << Text::bold("GRPC Address: ") + Text::green(getGRPCAddress().toStdString(), true) << std::endl;

    _GRPCPort = doc_map["GRPCPort"].toInt();
    std::cout << Text::bold("GRPC Port: ") + Text::green(std::to_string(getGRPCPort()), true) << std::endl;

    _teamColor = doc_map["teamColor"].toString();
    std::cout << Text::bold("Team Color: ") + Text::green(getTeamColor().toStdString(), true) << std::endl;

    _SimAddress = doc_map["SimAddress"].toString();
    std::cout << Text::bold("Simulator Address: ") + Text::green(getSimAddress().toStdString(), true) << std::endl;

    // Manually setting team port
    QString teamColor = getTeamColor().toLower();
    quint16 teamSensorPort = (teamColor == "yellow") ? 30011 : 30012;

    setSimPort(teamSensorPort);

    std::cout << Text::bold("Simulator Port: ") + Text::green(std::to_string(getSimPort()), true) << std::endl;
}

bool Constants::isTeamBlue(){
    if (getTeamColor() == "blue"){
        return true;
    }
    return false;
}

bool Constants::isTeamYellow(){
    if (getTeamColor() == "yellow"){
        return true;
    }
    return false;
}

QString Constants::getTeamColor() const
{
    return _teamColor;
}

void Constants::setTeamColor(const QString &teamColor)
{
    _teamColor = teamColor;
}

QString Constants::getGRPCAddress() const
{
    return _GRPCAddress;
}

void Constants::setGRPCAddress(const QString &GRPCAddress)
{
    _GRPCAddress = GRPCAddress;
}

quint16 Constants::getGRPCPort() const
{
    return _GRPCPort;
}

void Constants::setGRPCPort(const quint16 &GRPCPort)
{
    _GRPCPort = GRPCPort;
}

QString Constants::getSimAddress() const
{
    return _SimAddress;
}

void Constants::setSimAddress(const QString &SimAddress)
{
    _SimAddress = SimAddress;
}

quint16 Constants::getSimPort() const
{
    return _SimPort;
}

void Constants::setSimPort(const quint16 &SimPort)
{
    _SimPort = SimPort;
}

