#include "molconvfile.h"

MolconvFile::MolconvFile()
    : m_system(molconv::System::get())
{
}

bool MolconvFile::read(const QString &fileName)
{
    // check if we have the correct ending ".mcv"
    if (!fileName.endsWith(".mcv"))
    {
        return false;
    }
    QFile file(fileName);
    if (!file.exists())
    {
        return false;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }

    QDomDocument testDoc;
    testDoc.setContent(&file);
    file.close();

    QDomElement systemElement = testDoc.documentElement();

    QDomNode moleculeNode = systemElement.firstChild();

    while (! moleculeNode.isNull())
    {
        QDomElement moleculeElement = moleculeNode.toElement();
        if (! moleculeElement.isNull())
        {
            molconv::moleculePtr currentMolecule(new molconv::Molecule);

            currentMolecule->setName(moleculeElement.attribute("Name").toStdString());

            // create the origin and the basis to transform the atoms to their global positions
            // since only the internal positions are saved in the molconv file:
            QDomElement originElement = moleculeElement.elementsByTagName("Origin").at(0).toElement();

            molconv::origin Origin = static_cast<molconv::origin>(originElement.attribute("Type").toInt());
            double vecx = originElement.attribute("vecX").toDouble();
            double vecy = originElement.attribute("vecY").toDouble();
            double vecz = originElement.attribute("vecZ").toDouble();
            QString originAtomString = originElement.attribute("Atoms");
            QString originListString = originElement.attribute("originList");
            double originFactor = originElement.attribute("Factor").toDouble();

            Eigen::Vector3d originVec(vecx, vecy, vecz);
            int oA1, oA2;
            oA1 = originAtomString.split(",").at(0).toInt();
            oA2 = originAtomString.split(",").at(1).toInt();
            std::vector<bool> originList;
            for (int i = 0; i < originListString.split(",").length(); i++)
                if (originListString.split(",").at(i) == "T")
                    originList.push_back(true);
                else
                    originList.push_back(false);

            QDomElement basisElement = moleculeElement.elementsByTagName("Basis").at(0).toElement();

            molconv::basis Basis = static_cast<molconv::basis>(basisElement.attribute("Type").toInt());
            double phi = basisElement.attribute("phi").toDouble();
            double theta = basisElement.attribute("theta").toDouble();
            double psi = basisElement.attribute("psi").toDouble();
            QString basisAtomString = basisElement.attribute("Atoms");
            QString basisListString = basisElement.attribute("basisList");

            int bA1, bA2, bA3;
            bA1 = basisAtomString.split(",").at(0).toInt();
            bA2 = basisAtomString.split(",").at(1).toInt();
            bA3 = basisAtomString.split(",").at(2).toInt();
            std::vector<bool> basisList;
            for (int i = 0; i < basisListString.split(",").length(); i++)
                if (basisListString.split(",").at(i) == "T")
                    basisList.push_back(true);
                else
                    basisList.push_back(false);

            Eigen::MatrixXd trafo(molconv::Molecule::euler2rot(psi, theta, phi));

            QDomNode atomNode = basisElement.nextSibling();

            while (! atomNode.isNull())
            {
                QDomElement atomElement = atomNode.toElement();
                if (! atomElement.isNull())
                {
                    double x = atomElement.attribute("X").toDouble();
                    double y = atomElement.attribute("Y").toDouble();
                    double z = atomElement.attribute("Z").toDouble();
                    std::string element = atomElement.attribute("Ele").toStdString();

                    Eigen::Vector3d intPos(x, y, z);
                    Eigen::Vector3d globalPos(originVec + trafo * intPos);

                    chemkit::Atom *currentAtom = currentMolecule->addAtom(chemkit::Element(element));


                    currentAtom->setPosition(globalPos);
                }
                atomNode = atomNode.nextSibling();
            }

            currentMolecule->setOrigin(Origin, oA1, oA2, originFactor);
            currentMolecule->setOriginList(originList);
            currentMolecule->setBasis(Basis, bA1, bA2, bA3);
            currentMolecule->setBasisList(basisList);

            chemkit::BondPredictor::predictBonds(currentMolecule.get());
            m_molecules.push_back(currentMolecule);
        }
        moleculeNode = moleculeNode.nextSibling();
    }
    d->m_currentFile = fileName;
    setWindowTitle(fileName.split("/").last() + "[*] - molconv");
    return true;
}

bool MolconvFile::write(const QString &fileName)
{
    // check if we have the correct ending ".mcv"
    if (!fileName.endsWith(".mcv"))
    {
        return false;
    }

    QDomDocument testDoc;
    QDomElement system = testDoc.createElement("System");
    testDoc.appendChild(system);

    for (unsigned long id : m_system->getMolIDs())
    {
        QDomElement molecule = testDoc.createElement("Molecule");
        molecule.setAttribute("Name", QString::fromStdString(m_system->getMolecule(id)->name()));

        QDomElement origin = testDoc.createElement("Origin");
        origin.setAttribute("Type", QString::number(m_system->getMolecule(id)->internalOrigin()));
        origin.setAttribute("Factor", QString::number(m_system->getMolecule(id)->internalOriginFactor()));

        QString atomString = QString::number(m_system->getMolecule(id)->internalOriginAtoms()[0]);
        atomString += ",";
        atomString += QString::number(m_system->getMolecule(id)->internalOriginAtoms()[1]);
        origin.setAttribute("Atoms", atomString);

        QString originList;
        for (int k = 0; k < int(m_system->getMolecule(id)->size() - 1); k++)
            if (m_system->getMolecule(id)->originList()[k])
                originList += "T,";
            else
                originList += "F,";
        if (m_system->getMolecule(id)->originList()[m_system->getMolecule(id)->size() - 1])
            originList += "T";
        else
            originList += "F";
        origin.setAttribute("originList", originList);

        origin.setAttribute("vecX", QString::number(m_system->getMolecule(id)->internalOriginPosition()(0), 'e', 16));
        origin.setAttribute("vecY", QString::number(m_system->getMolecule(id)->internalOriginPosition()(1), 'e', 16));
        origin.setAttribute("vecZ", QString::number(m_system->getMolecule(id)->internalOriginPosition()(2), 'e', 16));
        molecule.appendChild(origin);

        QDomElement basis = testDoc.createElement("Basis");
        basis.setAttribute("Type", QString::number(m_system->getMolecule(id)->internalBasis()));

        atomString = QString::number(m_system->getMolecule(id)->internalBasisAtoms()[0]);
        atomString += ",";
        atomString += QString::number(m_system->getMolecule(id)->internalBasisAtoms()[1]);
        atomString += ",";
        atomString += QString::number(m_system->getMolecule(id)->internalBasisAtoms()[2]);
        basis.setAttribute("Atoms", atomString);

        QString basisList;
        for (int k = 0; k < int(m_system->getMolecule(id)->size() - 1); k++)
            if (m_system->getMolecule(id)->basisList()[k])
                basisList += "T,";
            else
                basisList += "F,";
        if (m_system->getMolecule(id)->basisList()[m_system->getMolecule(id)->size() - 1])
            basisList += "T";
        else
            basisList += "F";
        basis.setAttribute("basisList", basisList);

        basis.setAttribute("phi", QString::number(m_system->getMolecule(id)->phi(), 'e', 16));
        basis.setAttribute("theta", QString::number(m_system->getMolecule(id)->theta(), 'e', 16));
        basis.setAttribute("psi", QString::number(m_system->getMolecule(id)->psi(), 'e', 16));
        molecule.appendChild(basis);

        for (int j = 0; j < int(m_system->getMolecule(id)->size()); j++)
        {
            QDomElement atom = testDoc.createElement("Atom");
            atom.setAttribute("Ele", QString::fromStdString(m_system->getMolecule(id)->atom(j)->element().symbol()));
            atom.setAttribute("X", QString::number(m_system->getMolecule(id)->internalPositions()[j](0), 'e', 16));
            atom.setAttribute("Y", QString::number(m_system->getMolecule(id)->internalPositions()[j](1), 'e', 16));
            atom.setAttribute("Z", QString::number(m_system->getMolecule(id)->internalPositions()[j](2), 'e', 16));
            molecule.appendChild(atom);
        }
        system.appendChild(molecule);
    }

    for (int i = 0; i < int(m_system->nGroups()); i++)
    {
        QDomElement group = testDoc.createElement("Group");
        group.setAttribute("Name", QString::fromStdString(m_system->getGroup(i)->name()));
        group.setAttribute("Parent", QString::number(m_system->GroupIndex(m_system->getGroup(i)->parent())));

        QString memberString;
        for (int j = 0; j < int(m_system->getGroup(i)->nMolecules() - 1); j++)
        {
            memberString += QString::number(m_system->MoleculeIndex(m_system->getGroup(i)->getMol(j)));
            memberString += ",";
        }
        memberString += QString::number(m_system->MoleculeIndex(m_system->getGroup(i)->getMol(m_system->getGroup(i)->nMolecules() - 1)));
        group.setAttribute("Members", memberString);

        system.appendChild(group);
    }

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << testDoc.toString();
    file.close();
    return true;
}

std::vector<molconv::moleculePtr> molecules()
{
    return m_molecules;
}
