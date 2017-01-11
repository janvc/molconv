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

#include <chemkit/graphicsitem.h>
#include <chemkit/graphicsview.h>
#include <chemkit/graphicsoverlay.h>
#include "selecttool.h"
#include "molconv_window.h"

class SelectToolPrivate
{
public:
    SelectToolPrivate()
        : m_window(0)
    {
    }

    MolconvWindow *m_window;
    QGraphicsRectItem *m_rectItem;
    QPointF m_initPos;
    QPointF m_lastPos;

    enum State
    {
        Selecting,
        Deselecting
    }
    m_state;
};

SelectTool::SelectTool(MolconvWindow *newWindow)
    : chemkit::GraphicsTool()
    , d(new SelectToolPrivate)
{
    d->m_window = newWindow;
}

SelectTool::~SelectTool()
{
}

void SelectTool::mousePressEvent(QMouseEvent *event)
{
    d->m_initPos = event->localPos();
    d->m_lastPos = event->localPos();

    if (event->buttons() & Qt::LeftButton)
        d->m_state = SelectToolPrivate::Selecting;
    else if (event->buttons() & Qt::RightButton)
        d->m_state = SelectToolPrivate::Deselecting;

    chemkit::GraphicsItem *item = view()->itemAt(event->x(), event->y());

    if (item && item->type() == chemkit::GraphicsItem::AtomItem)
    {
        chemkit::GraphicsAtomItem *atomItem = static_cast<chemkit::GraphicsAtomItem *>(item);
        chemkit::Atom *atom = const_cast<chemkit::Atom *>(atomItem->atom());

        if (d->m_state == SelectToolPrivate::Selecting)
        {
            if (event->modifiers() & Qt::ShiftModifier)
                d->m_window->selectAtom(atom, true);
            else
                d->m_window->selectAtom(atom, false);
        }

        if (d->m_state == SelectToolPrivate::Deselecting)
        {
            if (event->modifiers() & Qt::ShiftModifier)
                d->m_window->deselectAtom(atom, true);
            else
                d->m_window->deselectAtom(atom, false);
        }
    }

    d->m_rectItem = new QGraphicsRectItem(event->x(), event->y(), 0, 0);
    d->m_rectItem->setPen(QPen(QBrush(Qt::white), 2, Qt::DashLine));
    d->m_rectItem->setOpacity(0.5);
    view()->overlay()->addItem(d->m_rectItem);
}

void SelectTool::mouseMoveEvent(QMouseEvent *event)
{
    d->m_lastPos = event->localPos();
    d->m_rectItem->setRect(QRectF(d->m_initPos, event->pos()).normalized());
    view()->update();
}

void SelectTool::mouseReleaseEvent(QMouseEvent *event)
{
    QRect rect = d->m_rectItem->rect().toRect().normalized();
    for (int x = rect.x(); x < rect.right(); x+= 5)
        for (int y = rect.y(); y < rect.bottom(); y+= 5)
        {
            chemkit::GraphicsItem *item = view()->itemAt(x, y);
            if (item && item->type() == chemkit::GraphicsItem::AtomItem)
            {
                chemkit::GraphicsAtomItem *atomItem = static_cast<chemkit::GraphicsAtomItem *>(item);
                chemkit::Atom *atom = const_cast<chemkit::Atom *>(atomItem->atom());
                if (d->m_state == SelectToolPrivate::Selecting)
                {
                    if (event->modifiers() & Qt::ShiftModifier)
                        d->m_window->selectAtom(atom, true);
                    else
                        d->m_window->selectAtom(atom, false);
                }

                if (d->m_state == SelectToolPrivate::Deselecting)
                {
                    if (event->modifiers() & Qt::ShiftModifier)
                        d->m_window->deselectAtom(atom, true);
                    else
                        d->m_window->deselectAtom(atom, false);
                }
            }
        }

    view()->overlay()->removeItem(d->m_rectItem);
    delete d->m_rectItem;
    d->m_rectItem = 0;
    view()->update();
}

void SelectTool::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
