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
    Eigen::Vector3d m_position;

    // orientation: basis vectors:
    Eigen::Matrix3d m_vectors;

    double axisLength;
};

GraphicsAxisItem::GraphicsAxisItem()
    : d(new GraphicsAxisItemPrivate)
{
    d->m_position = Eigen::Vector3d::Zero();
    d->m_vectors = Eigen::Matrix3d::Identity();
    d->axisLength = 1.0;
}

GraphicsAxisItem::GraphicsAxisItem(const Eigen::Vector3d &position, const Eigen::Matrix3d &vectors, const double length)
    : d(new GraphicsAxisItemPrivate)
{
    d->m_position = position;
    d->m_vectors = vectors;
    d->axisLength = length;
}

GraphicsAxisItem::~GraphicsAxisItem()
{
}

void GraphicsAxisItem::setPosition(const Eigen::Vector3d &newPosition)
{
    d->m_position = newPosition;
}

void GraphicsAxisItem::paint(chemkit::GraphicsPainter *painter)
{
    QVector<Eigen::Vector3f> vertices;
    QVector<unsigned short> indices;
    QVector<QColor> colors;

    vertices.append(d->m_position.cast<float>());
    vertices.append(Eigen::Vector3d(d->m_position + d->axisLength * d->m_vectors.col(0)).cast<float>());
    vertices.append(Eigen::Vector3d(d->m_position + d->axisLength * d->m_vectors.col(1)).cast<float>());
    vertices.append(Eigen::Vector3d(d->m_position + d->axisLength * d->m_vectors.col(2)).cast<float>());

    indices << 0 << 1 << 0 << 2 << 0 << 3;

    colors.append(QColor(128, 128, 128));
    colors.append(QColor(255, 0, 0));
    colors.append(QColor(0, 255, 0));
    colors.append(QColor(0, 0, 255));

    chemkit::GraphicsVertexBuffer buffer;

    buffer.setVertices(vertices);
    buffer.setIndices(indices);
    buffer.setColors(colors);

    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    QGLShaderProgram program;

    QString vert("void main(){gl_Position=gl_ModelViewProjectionMatrix*gl_Vertex;gl_FrontColor=gl_Color;}");
    QString frag("void main(){gl_FragColor=gl_Color;}");

    program.addShaderFromSourceCode(QGLShader::Vertex, vert);
    program.addShaderFromSourceCode(QGLShader::Fragment, frag);

    program.link();
    program.bind();

    painter->draw(&buffer, chemkit::GraphicsPainter::Lines);

    program.release();

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
}
