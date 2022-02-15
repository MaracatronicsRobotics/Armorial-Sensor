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

#ifndef BASESENSOR_H
#define BASESENSOR_H

#include <src/entities/entity.h>

class BaseSensor : public Entity
{
public:
    BaseSensor();

    virtual QString name();

private:
    // Entity inherited methods
    virtual void initialization() = 0;
    virtual void loop() = 0;
    virtual void finalization() = 0;

};

#endif // BASESENSOR_H
