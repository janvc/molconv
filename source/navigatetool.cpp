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

#include <chemkit/graphicscamera.h>
#include <chemkit/graphicsview.h>
#include "navigatetool.h"

class NavigateToolPrivate
{
public:
    NavigateToolPrivate()
        : m_mousedown(false)
    {
    }

    bool m_mousedown;
    QPoint m_lastPosition;
};

NavigateTool::NavigateTool()
    : chemkit::GraphicsTool()
    , d(new NavigateToolPrivate)
{
}

NavigateTool::~NavigateTool()
{
}

void NavigateTool::mousePressEvent(QMouseEvent *event)
{
    d->m_mousedown = true;
    d->m_lastPosition = event->pos();
}

void NavigateTool::mouseMoveEvent(QMouseEvent *event)
{
    if (d->m_mousedown)
    {
        const boost::shared_ptr<chemkit::GraphicsCamera> &camera = view()->camera();
        int dx = event->x() - d->m_lastPosition.x();
        int dy = event->y() - d->m_lastPosition.y();

        if (event->buttons() & Qt::LeftButton)
        {
            camera->orbit(-dx, dy);
        }

        if (event->buttons() & Qt::RightButton)
        {
            Eigen::Vector3f dir = camera->direction();
            Eigen::Vector3f upv = camera->upVector();
            Eigen::Vector3f xvec = dir.cross(upv);
            Eigen::Vector3f shift = -dx * 0.05 * xvec.normalized() + dy * 0.05 * upv.normalized();

            camera->moveBy(shift);
        }

        d->m_lastPosition = event->pos();
        view()->update();
    }
}

void NavigateTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    d->m_mousedown = false;
}

void NavigateTool::wheelEvent(QWheelEvent *event)
{
    const boost::shared_ptr<chemkit::GraphicsCamera> &camera = view()->camera();

    if (event->delta() > 0)
        camera->moveFoward(1);
    else
        camera->moveBackward(1);

    view()->update();
}
