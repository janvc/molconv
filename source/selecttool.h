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

#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#ifndef Q_MOC_RUN
    #include <chemkit/graphicstool.h>
#endif

class SelectToolPrivate;
class MolconvWindow;

class SelectTool : public chemkit::GraphicsTool
{
public:
    SelectTool(MolconvWindow *newWindow);
    ~SelectTool();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

private:
    boost::shared_ptr<SelectToolPrivate> d;
};

#endif // SELECTTOOL_H
