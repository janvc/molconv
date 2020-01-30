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

#include <chemkit/graphicspainter.h>
#include <chemkit/graphicsvertexbuffer.h>
#include <chemkit/graphicssphere.h>
#include "graphicsselectionitem.h"

class GraphicsSelectionItemPrivate
{
public:
    GraphicsSelectionItemPrivate()
    {
        m_radius = 0.6f;
        m_opacity = 0.5f;
        m_color = QColor(88, 88, 255, int(255 * m_opacity));
    }

    std::vector<Eigen::Vector3f> m_positions;
    float m_radius;
    float m_opacity;
    QColor m_color;
};

GraphicsSelectionItem::GraphicsSelectionItem()
    : d(new GraphicsSelectionItemPrivate)
{
}

GraphicsSelectionItem::~GraphicsSelectionItem()
{
}

void GraphicsSelectionItem::addPosition(const Eigen::Vector3d &newPosition)
{
    d->m_positions.push_back(newPosition.cast<float>());
}

void GraphicsSelectionItem::clear()
{
    d->m_positions.clear();
}

void GraphicsSelectionItem::paint(chemkit::GraphicsPainter *painter)
{
    painter->setColor(d->m_color);

    for (int i = 0; i < int(d->m_positions.size()); i++)
        painter->drawSphere(d->m_positions[i], d->m_radius);
}
