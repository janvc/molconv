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
};

#endif // GRAPHICSSELECTIONITEM_H
