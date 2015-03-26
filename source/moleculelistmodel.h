/*
 * Copyright 2014 Jan von Cosel & Sebastian Lenz
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

#ifndef MOLECULELISTMODEL_H
#define MOLECULELISTMODEL_H

#include <QAbstractItemModel>

class MoleculeListModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MoleculeListModel(QObject *parent = 0);

signals:

public slots:

};

#endif // MOLECULELISTMODEL_H
