/*
 * Copyright 2014 - 2016 Jan von Cosel & Sebastian Lenz
 *
 * This file is part of molconv.
 *
 * molconv is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * molconv is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have recieved a copy of the GNU General Public License
 * along with molconv. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef GRAPHICSAXISITEM_H
#define GRAPHICSAXISITEM_H

#ifndef Q_MOC_RUN
    #include<chemkit/graphicsitem.h>
    #include<boost/shared_ptr.hpp>
#endif

class GraphicsAxisItemPrivate;

class GraphicsAxisItem : public chemkit::GraphicsItem
{
public:
    GraphicsAxisItem();
    GraphicsAxisItem(const Eigen::Vector3d &position, const Eigen::Matrix3d &vectors, const float length = 1.0f, const float radius = 0.1f);
    ~GraphicsAxisItem();

    void setPosition(const Eigen::Vector3d &newPosition);
    void setVectors(const Eigen::Matrix3d &newVectors);

    void paint(chemkit::GraphicsPainter *painter);

private:
    boost::shared_ptr<GraphicsAxisItemPrivate> d;
};

#endif // GRAPHICSAXISITEM_H
