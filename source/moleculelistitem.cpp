/*
 * Copyright 2014-2015 Jan von Cosel & Sebastian Lenz
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


#include "moleculelistitem.h"

class MoleculeListItemPrivate
{
public:
    MoleculeListItemPrivate()
        : m_isMolecule(true)
        , m_ParentItem(0)
        , m_childItems(new QList<MoleculeListItem*>)
        , m_molecule(0)
    {
    }

    bool m_isMolecule;
    MoleculeListItem *m_ParentItem;
    QList<MoleculeListItem*> *m_childItems;
    QVector<QVariant> itemData;
    molconv::moleculePtr m_molecule;
};

MoleculeListItem::MoleculeListItem(molconv::moleculePtr &molecule, MoleculeListItem *parent)
    : d(new MoleculeListItemPrivate)
{
    qDebug("this is the first constructor of MoleculeListItem");

    d->m_ParentItem = parent;
    d->m_molecule = molecule;
    d->itemData << QString::fromStdString(d->m_molecule->name());
    d->itemData << d->m_molecule->mass();
}

MoleculeListItem::MoleculeListItem(const QVector<QVariant> &data, MoleculeListItem *parent)
    : d(new MoleculeListItemPrivate)
{
    qDebug("this is the second constructor of MoleculeListItem");

    d->itemData = data;
}

MoleculeListItem::~MoleculeListItem()
{
    qDeleteAll(d->m_childItems->begin(), d->m_childItems->end());
}

void MoleculeListItem::appendChild(MoleculeListItem *child)
{
    d->m_childItems->append(child);
}

MoleculeListItem *MoleculeListItem::child(const int row)
{
    return d->m_childItems->value(row);
}

MoleculeListItem *MoleculeListItem::parent()
{
    return d->m_ParentItem;
}

int MoleculeListItem::columnCount() const
{
    return d->itemData.count();
}

int MoleculeListItem::childNumber() const
{
    if (d->m_ParentItem)
        return d->m_ParentItem->children()->indexOf(const_cast<MoleculeListItem*>(this));

    return 0;
}

molconv::moleculePtr MoleculeListItem::Molecule() const
{
    return d->m_molecule;
}

void MoleculeListItem::setMolecule(molconv::moleculePtr &mol)
{
    d->m_molecule = mol;
}

int MoleculeListItem::childCount() const
{
    return d->m_childItems->count();
}

int MoleculeListItem::row() const
{
    if (d->m_ParentItem)
        return d->m_ParentItem->children()->indexOf(const_cast<MoleculeListItem*>(this));

    return 0;
}

QList<MoleculeListItem*> *MoleculeListItem::children() const
{
    return d->m_childItems;
}

QVariant MoleculeListItem::data(int column) const
{
    return d->itemData.value(column);
}


