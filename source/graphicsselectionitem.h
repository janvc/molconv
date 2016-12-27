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

#ifndef GRAPHICSSELECTIONITEM_H
#define GRAPHICSSELECTIONITEM_H

#ifndef Q_MOC_RUN
    #include <chemkit/graphicsitem.h>
#endif

#include "types.h"

class GraphicsSelectionItemPrivate;

class GraphicsSelectionItem : public chemkit::GraphicsItem
{
public:
    GraphicsSelectionItem();
    ~GraphicsSelectionItem();

    void addPosition(const Eigen::Vector3d &newPosition);
    void clear();

    void paint(chemkit::GraphicsPainter *painter);

private:
    boost::shared_ptr<GraphicsSelectionItemPrivate> d;
};

#endif // GRAPHICSSELECTIONITEM_H
