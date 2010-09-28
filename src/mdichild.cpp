/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "mdichild.h"
#include "pers.h"
#include "modeledit.h"
#include "generaledit.h"

MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);

    this->setFont(QFont("Courier New",12));
    refreshList();
    if(!(this->isMaximized() || this->isMinimized())) this->adjustSize();
    isUntitled = true;

    connect(this, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(OpenEditWindow()));
    this->setSelectionMode(QAbstractItemView::MultiSelection);
}


QSize MdiChild::sizeHint()
{
    return QSize(300,400);
}

void MdiChild::refreshList()
{
    clear();
    addItem("General Settings");

    for(uint8_t i=0; i<MAX_MODELS; i++)
    {
        static char buf[sizeof(g_model.name)+10];
        eeFile.eeLoadModelName(i,buf,sizeof(buf));
        QString str = QString(buf);
        addItem(str);
    }

}


void MdiChild::cut()
{
    copy();
    deleteSelected(false);
}

void MdiChild::deleteSelected(bool ask=true)
{
    QMessageBox::StandardButton ret = QMessageBox::No;

    if(ask)
        ret = QMessageBox::warning(this, tr("eePe"),
                 tr("Delete Selected Models?"),
                 QMessageBox::Yes | QMessageBox::No);


    if ((ret == QMessageBox::Yes) || (!ask))
    {
           foreach(QModelIndex index, this->selectionModel()->selectedIndexes())
           {
               if(index.row()>0)eeFile.DeleteModel(index.row());
           }
           refreshList();
           eeFile.setChanged(true);
           documentWasModified();
    }
}

void MdiChild::copy()
{
    //do copy

}

void MdiChild::paste()
{
//    if(clipboard info available)
//    {

//        refreshList();
//        eeFile.setChanged(true);
//        documentWasModified();
//    }

}

bool MdiChild::hasSelection()
{
    return (this->selectedIndexes().count()>0);
}

void MdiChild::keyPressEvent(QKeyEvent *event)
{
    if(event->matches(QKeySequence::Delete))
    {
        deleteSelected();
        return;
    }

    if(event->matches(QKeySequence::Cut))
    {
        cut();
        return;
    }

    if(event->matches(QKeySequence::Copy))
    {
        copy();
        return;
    }

    if(event->matches(QKeySequence::Paste))
    {
        paste();
        return;
    }


    QListWidget::keyPressEvent(event);
}


void MdiChild::OpenEditWindow()
{
    int i = this->currentRow();

    if(i)
    {
        //TODO error checking
        eeFile.setChanged(true);
        documentWasModified();
        if(eeFile.eeLoadModel((uint8_t)i-1))
        {
            char buf[11];
            eeFile.curmodelName((char*)&buf);
            ModelEdit t;
            t.setWindowTitle(tr("Editing model %1: ").arg(i) + QString(buf));
            t.exec();
        }
        else
            QMessageBox::critical(this, tr("eePe"),tr("Unable to read model data!"));
    }
    else
    {
        //TODO error checking
        if(eeFile.eeLoadGeneral())
        {
            eeFile.setChanged(true);
            documentWasModified();
            GeneralEdit t;
            t.exec();
        }
        else
            QMessageBox::critical(this, tr("eePe"),tr("Unable to read settings!"));

    }

}

void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.bin").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

    //connect(eeFile, SIGNAL(contentsChanged()),
    //        this, SLOT(documentWasModified()));
}

bool MdiChild::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {  //reading binary file   - TODO HEX support
        QMessageBox::warning(this, tr("eePe"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }


    if(file.size()!=EESIZE)
    {
        QMessageBox::critical(this, tr("eePe"),tr("Unable to read file %1!").arg(fileName));
        file.close();
        return false;
    }

    uint8_t temp[EESIZE];
    long result = file.read((char*)&temp,EESIZE);
    file.close();

    if (result!=EESIZE)
    {
        QMessageBox::warning(this, tr("eePe"),
                             tr("Error reading file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));

        return false;
    }

    eeFile.loadFile(&temp);
    refreshList();
    setCurrentFile(fileName);

    //connect(eeFile(), SIGNAL(contentsChanged()),
    //        this, SLOT(documentWasModified()));

    return true;
}

bool MdiChild::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool MdiChild::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("eePe"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    uint8_t temp[EESIZE];
    eeFile.saveFile(&temp);

    long result = file.write((char*)&temp,EESIZE);
    if(result!=EESIZE)
    {
        QMessageBox::warning(this, tr("eePe"),
                             tr("Error writing file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    setCurrentFile(fileName);
    return true;
}

QString MdiChild::userFriendlyCurrentFile()
{
    return strippedName(curFile);
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MdiChild::documentWasModified()
{
    setWindowModified(eeFile.Changed());
}

bool MdiChild::maybeSave()
{
    if (eeFile.Changed()) {
	QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("eePe"),
                     tr("'%1' has been modified.\n"
                        "Do you want to save your changes?")
                     .arg(userFriendlyCurrentFile()),
                     QMessageBox::Save | QMessageBox::Discard
		     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;

}

void MdiChild::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    eeFile.setChanged(false);
    setWindowModified(false);
    setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QString MdiChild::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
