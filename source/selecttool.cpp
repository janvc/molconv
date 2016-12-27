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
    chemkit::GraphicsItem *item = view()->itemAt(event->x(), event->y());

    if (item && item->type() == chemkit::GraphicsItem::AtomItem)
    {
        chemkit::GraphicsAtomItem *atomItem = static_cast<chemkit::GraphicsAtomItem *>(item);
        chemkit::Atom *atom = const_cast<chemkit::Atom *>(atomItem->atom());

        if (event->buttons() & Qt::LeftButton)
            if (event->modifiers() & Qt::ShiftModifier)
                d->m_window->selectAtom(atom, true);
            else
                d->m_window->selectAtom(atom, false);

        if (event->buttons() & Qt::RightButton)
            if (event->modifiers() & Qt::ShiftModifier)
                d->m_window->deselectAtom(atom, true);
            else
                d->m_window->deselectAtom(atom, false);

    }
}

void SelectTool::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void SelectTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
}

void SelectTool::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event);
}
