/*
 * Copyright 2014 - 2019 Jan von Cosel & Sebastian Lenz
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

#include<QVector>
#include<QColor>
#include<QtOpenGL>
#include<chemkit/graphics.h>
#include<chemkit/graphicspainter.h>
#include<chemkit/graphicsvertexbuffer.h>
#include "graphicsaxisitem.h"

class GraphicsAxisItemPrivate
{
public:
    // position:
    Eigen::Vector3f m_position;

    // orientation: basis vectors:
    Eigen::Matrix3f m_vectors;

    float m_axisLength;
    float m_radius;
};

GraphicsAxisItem::GraphicsAxisItem()
    : d(new GraphicsAxisItemPrivate)
{
    d->m_position = Eigen::Vector3f::Zero();
    d->m_vectors = Eigen::Matrix3f::Identity();
    d->m_axisLength = 1.0f;
    d->m_radius = 0.1f;
}

GraphicsAxisItem::GraphicsAxisItem(const Eigen::Vector3d &position, const Eigen::Matrix3d &vectors, const float length, const float radius)
    : d(new GraphicsAxisItemPrivate)
{
    d->m_position = position.cast<float>();
    d->m_vectors = vectors.cast<float>();
    d->m_axisLength = length;
    d->m_radius = radius;
}

GraphicsAxisItem::~GraphicsAxisItem()
{
}

void GraphicsAxisItem::setPosition(const Eigen::Vector3d &newPosition)
{
    d->m_position = newPosition.cast<float>();
}

void GraphicsAxisItem::setVectors(const Eigen::Matrix3d &newVectors)
{
    d->m_vectors = newVectors.cast<float>();
}

void GraphicsAxisItem::paint(chemkit::GraphicsPainter *painter)
{
    // create the vertices:
    // the origin
    Eigen::Vector3f vert0 = d->m_position.cast<float>();
    // the half-way vertices
    Eigen::Vector3f vert1 = d->m_position + 0.5f * d->m_axisLength * d->m_vectors.col(0);
    Eigen::Vector3f vert2 = d->m_position + 0.5f * d->m_axisLength * d->m_vectors.col(1);
    Eigen::Vector3f vert3 = d->m_position + 0.5f * d->m_axisLength * d->m_vectors.col(2);
    // the ends of the axes
    Eigen::Vector3f vert4 = d->m_position + d->m_axisLength * d->m_vectors.col(0);
    Eigen::Vector3f vert5 = d->m_position + d->m_axisLength * d->m_vectors.col(1);
    Eigen::Vector3f vert6 = d->m_position + d->m_axisLength * d->m_vectors.col(2);

    // draw the stuff:
    // the grey sphere at the origin
    painter->setColor(QColor(128, 128, 128));
    painter->drawSphere(vert0, d->m_radius);
    // the grey first half of the axes
    painter->drawCylinder(vert0, vert1, d->m_radius);
    painter->drawCylinder(vert0, vert2, d->m_radius);
    painter->drawCylinder(vert0, vert3, d->m_radius);
    // the colored second half of the axes
    painter->setColor(QColor(255, 0, 0));
    painter->drawCylinder(vert1, vert4, d->m_radius);
    painter->drawSphere(vert4, d->m_radius);
    painter->setColor(QColor(0, 255, 0));
    painter->drawCylinder(vert2, vert5, d->m_radius);
    painter->drawSphere(vert5, d->m_radius);
    painter->setColor(QColor(0, 0, 255));
    painter->drawCylinder(vert3, vert6, d->m_radius);
    painter->drawSphere(vert6, d->m_radius);
}
