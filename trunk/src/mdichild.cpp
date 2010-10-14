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
#include "avroutputdialog.h"
#include "burnconfigdialog.h"
#include "simulatordialog.h"


MdiChild::MdiChild()
{
    setAttribute(Qt::WA_DeleteOnClose);

    this->setFont(QFont("Courier New",12));
    refreshList();
    if(!(this->isMaximized() || this->isMinimized())) this->adjustSize();
    isUntitled = true;

    connect(this, SIGNAL(itemActivated(QListWidgetItem*)), this, SLOT(OpenEditWindow()));
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),this, SLOT(ShowContextMenu(const QPoint&)));
    connect(this,SIGNAL(currentRowChanged(int)), this,SLOT(viableModelSelected(int)));

    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);

}

void MdiChild::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        dragStartPosition = event->pos();

    QListWidget::mousePressEvent(event);
}

void MdiChild::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength()
         < QApplication::startDragDistance())
        return;

    QDrag *drag = new QDrag(this);

    QByteArray gmData;
    doCopy(&gmData);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-eepe", gmData);

    drag->setMimeData(mimeData);

    //Qt::DropAction dropAction =
            drag->exec(Qt::CopyAction);// | Qt::MoveAction);

    //if(dropAction==Qt::MoveAction)

    QListWidget::mouseMoveEvent(event);
}

void MdiChild::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-eepe"))
    {
         event->acceptProposedAction();
         clearSelection();
         itemAt(event->pos())->setSelected(true);
    }
}

void MdiChild::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-eepe"))
    {
         event->acceptProposedAction();
         clearSelection();
         itemAt(event->pos())->setSelected(true);
    }
}

void MdiChild::dropEvent(QDropEvent *event)
{
    int i = this->indexAt(event->pos()).row();
    //QMessageBox::warning(this, tr("eePe"),tr("Index :%1").arg(i));
    if(event->mimeData()->hasFormat("application/x-eepe"))
    {
        QByteArray gmData = event->mimeData()->data("application/x-eepe");
        doPaste(&gmData,i);
    }
    event->acceptProposedAction();
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
           setModified();
    }
}


void MdiChild::doCopy(QByteArray *gmData)
{
    foreach(QModelIndex index, this->selectionModel()->selectedIndexes())
    {
        if(!index.row())
        {
            EEGeneral tgen;
            if(eeFile.getGeneralSettings(&tgen))
            {
                gmData->append('G');
                gmData->append((char*)&tgen,sizeof(tgen));
            }
        }
        else
        {
            ModelData tmod;
            if(eeFile.getModel(&tmod,index.row()-1))
            {
                gmData->append('M');
                gmData->append((char*)&tmod,sizeof(tmod));
            }
        }
    }
}

void MdiChild::copy()
{
    QByteArray gmData;
    doCopy(&gmData);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-eepe", gmData);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setMimeData(mimeData,QClipboard::Clipboard);
}

void MdiChild::doPaste(QByteArray *gmData, int index)
{
    //QByteArray gmData = mimeD->data("application/x-eepe");
    char *gData = gmData->data();//new char[gmData.size() + 1];
    int i = 0;
    int id = index;
    if(!id) id++;

    while((i<gmData->size()) && (id<=MAX_MODELS))
    {
        char c = *gData;
        i++;
        gData++;
        if(c=='G')  //general settings
        {
            if(!eeFile.putGeneralSettings((EEGeneral*)gData))
            {
                QMessageBox::critical(this, tr("Error"),tr("Unable set data!"));
                break;
            }
            gData += sizeof(EEGeneral);
            i     += sizeof(EEGeneral);
        }
        else //model data
        {
            if(!eeFile.putModel((ModelData*)gData,id-1))
            {
                QMessageBox::critical(this, tr("Error"),tr("Unable set model!"));
                break;
            }
            gData += sizeof(ModelData);
            i     += sizeof(ModelData);
            id++;
        }
    }
    setModified();
}

bool MdiChild::hasPasteData()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    return mimeData->hasFormat("application/x-eepe");
}

void MdiChild::paste()
{
    if(hasPasteData())
    {
        const QClipboard *clipboard = QApplication::clipboard();
        const QMimeData *mimeData = clipboard->mimeData();

        QByteArray gmData = mimeData->data("application/x-eepe");
        doPaste(&gmData,this->currentRow());
    }

}

void MdiChild::duplicate()
{
    int i = this->currentRow();
    if(i && i<MAX_MODELS)
    {
        ModelData gmodel;
        if(eeFile.getModel(&gmodel,--i)) eeFile.putModel(&gmodel,i+1);
        setModified();
    }
}

bool MdiChild::hasSelection()
{
    return (this->selectionModel()->hasSelection());
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

    if(event->matches(QKeySequence::Underline))
    {
        duplicate();
        return;
    }



    QListWidget::keyPressEvent(event);//run the standard event in case we didn't catch an action
}


void MdiChild::OpenEditWindow()
{
    int i = this->currentRow();

    if(i)
    {
        //TODO error checking

        if(!eeFile.eeModelExists((uint8_t)i-1))
        {
            eeFile.modelDefault(i-1);
            setModified();
        }

        char buf[sizeof(g_model.name)+1];
        eeFile.getModelName((i-1),(char*)&buf);
        ModelEdit t(&eeFile,(i-1),this);
        t.setWindowTitle(tr("Editing model %1: ").arg(i) + QString(buf));
        connect(&t,SIGNAL(modelValuesChanged()),this,SLOT(setModified()));
        t.exec();

    }
    else
    {
        //TODO error checking
        if(eeFile.eeLoadGeneral())
        {
            //setModified();
            GeneralEdit t(&eeFile, this);
            connect(&t,SIGNAL(modelValuesChanged()),this,SLOT(setModified()));
            t.exec();
        }
        else
            QMessageBox::critical(this, tr("Error"),tr("Unable to read settings!"));
    }

}

void MdiChild::newFile()
{
    static int sequenceNumber = 1;

    isUntitled = true;
    curFile = tr("document%1.hex").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");

}

int getValueFromLine(const QString &line, int pos, int len=2)
{
    bool ok;
    int hex = line.mid(pos,len).toInt(&ok, 16);
    return ok ? hex : -1;
}

bool MdiChild::loadFile(const QString &fileName, bool resetCurrentFile)
{
    QFile file(fileName);

    if(!file.exists())
    {
        QMessageBox::critical(this, tr("Error"),tr("Unable to find file %1!").arg(fileName));
        return false;
    }


    int fileType = getFileType(fileName);

    if(fileType==FILE_TYPE_HEX) //read HEX file
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {  //reading HEX TEXT file
            QMessageBox::warning(this, tr("Error"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return false;
        }

        uint8_t temp[EESIZE];
        memset(&temp,0,EESIZE);

        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();

            if(line.left(1)!=":") continue;

            int byteCount = getValueFromLine(line,1);
            int address = getValueFromLine(line,3,4);
            int recType = getValueFromLine(line,7);

            if(byteCount<0 || address<0 || recType<0) {QMessageBox::critical(this, tr("Error"),tr("Error reading file %1!").arg(fileName));return false;}

            QByteArray ba;
            ba.clear();

            quint8 chkSum = 0;
            chkSum -= byteCount;
            chkSum -= recType;
            chkSum -= address & 0xFF;
            chkSum -= address >> 8;
            for(int i=0; i<byteCount; i++)
            {
                quint8 v = getValueFromLine(line,(i*2)+9) & 0xFF;
                chkSum -= v;
                ba.append(v);
            }


            quint8 retV = getValueFromLine(line,(byteCount*2)+9) & 0xFF;
            if(chkSum!=retV) {QMessageBox::critical(this, tr("Error"),tr("Error reading file %1!").arg(fileName));return false;}

            if((recType == 0x00) && ((address+byteCount)<=EESIZE)) //data record - ba holds record
                memcpy(&temp[address],ba.data(),byteCount);

        }

        file.close();
        eeFile.loadFile(&temp);
        refreshList();
        if(resetCurrentFile) setCurrentFile(fileName);

        return true;
    }


    if(fileType==FILE_TYPE_BIN) //read binary
    {
        if(file.size()!=EESIZE)
        {
            QMessageBox::critical(this, tr("Error"),tr("File wrong size - %1").arg(fileName));
            return false;
        }

        if (!file.open(QFile::ReadOnly)) {  //reading binary file   - TODO HEX support
            QMessageBox::warning(this, tr("Error"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return false;
        }

        uint8_t temp[EESIZE];
        long result = file.read((char*)&temp,EESIZE);
        file.close();

        if (result!=EESIZE)
        {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Error reading file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));

            return false;
        }

        eeFile.loadFile(&temp);
        refreshList();
        if(resetCurrentFile) setCurrentFile(fileName);

        return true;
    }

    return false;
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
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),curFile,tr("EEPROM hex files (*.hex);;EEPROM bin files (*.bin)"));
    if (fileName.isEmpty())
        return false;

    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("lastDir",QFileInfo(fileName).dir().absolutePath());
    return saveFile(fileName);
}

bool MdiChild::saveFile(const QString &fileName, bool setCurrent)
{
    QFile file(fileName);

    int fileType = getFileType(fileName);

    if(fileType==FILE_TYPE_HEX) //write hex
    {        
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Error"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return false;
        }


        quint8 temp[EESIZE];
        eeFile.saveFile(&temp);

        QTextStream out(&file);
        for(int i=0; i<(EESIZE/32); i++)
        {
            QString str = tr(":20%1000").arg(i*32,4,16,QChar('0')); //write start, bytecount (32), address and record type
            quint8 chkSum = 0;
            chkSum = -32; //-bytecount; recordtype is zero
            chkSum -= (i*32) & 0xFF;
            chkSum -= (i*32) >> 8;
            for(int j=0; j<32; j++)
            {
                str += tr("%1").arg(temp[i*32+j],2,16,QChar('0'));
                chkSum -= temp[i*32+j];
            }

            str += tr("%1").arg(chkSum,2,16,QChar('0'));
            out << str.toUpper() << "\n"; // output to file and lf;

        }

        out << ":00000001FF";  // write EOF
        file.close();
        if(setCurrent) setCurrentFile(fileName);
        return true;


        //             out << "The magic number is: " << 49 << "\n"
    }

    if(fileType==FILE_TYPE_BIN) //write binary
    {
        if (!file.open(QFile::WriteOnly)) {
            QMessageBox::warning(this, tr("Error"),
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
            QMessageBox::warning(this, tr("Error"),
                                 tr("Error writing file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return false;
        }

        if(setCurrent) setCurrentFile(fileName);
        return true;
    }

    return false;
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

int MdiChild::getFileType(const QString &fullFileName)
{
    if(QFileInfo(fullFileName).suffix().toUpper()=="HEX") return FILE_TYPE_HEX;
    if(QFileInfo(fullFileName).suffix().toUpper()=="BIN") return FILE_TYPE_BIN;
    return 0;
}

void MdiChild::burnTo()  // write to Tx
{
    burnConfigDialog bcd;
    QString avrdudeLoc = bcd.getAVRDUDE();
    QString tempDir    = bcd.getTempDir();
    QString programmer = bcd.getProgrammer();
    QStringList args   = bcd.getAVRArgs();

    QString tempFile = tempDir + "/temp.hex";
    saveFile(tempFile, false);
    if(!QFileInfo(tempFile).exists())
    {
        QMessageBox::critical(this,tr("Error"), tr("Cannot write temporary file!"));
        return;
    }
    QString str = "eeprom:w:" + tempFile + ":i"; // writing eeprom -> MEM:OPR:FILE:FTYPE"

    QStringList arguments;
    arguments << "-c" << programmer << "-p" << "m64" << args << "-U" << str;

    avrOutputDialog ad(this, avrdudeLoc, arguments, "Write EEPROM To Tx");
    ad.exec();
}

void MdiChild::ShowContextMenu(const QPoint& pos)
{
    QPoint globalPos = this->mapToGlobal(pos);

    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    bool hasData = mimeData->hasFormat("application/x-eepe");

    QMenu contextMenu;
    contextMenu.addAction(tr("&Edit"),this,SLOT(OpenEditWindow()));
    contextMenu.addSeparator();
    contextMenu.addAction(tr("&Delete"),this,SLOT(deleteSelected(bool)),tr("Delete"));
    contextMenu.addAction(tr("&Copy"),this,SLOT(copy()),tr("Ctrl+C"));
    contextMenu.addAction(tr("&Cut"),this,SLOT(cut()),tr("Ctrl+X"));
    contextMenu.addAction(tr("&Paste"),this,SLOT(paste()),tr("Ctrl+V"))->setEnabled(hasData);
    contextMenu.addAction(tr("D&uplicate"),this,SLOT(duplicate()),tr("Ctrl+U"));
    contextMenu.addSeparator();
    contextMenu.addAction(tr("&Simulate"),this,SLOT(simulate()),tr("Alt+S"));
    contextMenu.addSeparator();
    contextMenu.addAction(tr("&Write To Tx"),this,SLOT(burnTo()),tr("Ctrl+Alt+W"));
    contextMenu.addAction(tr("&Read From Tx"),this,SLOT(burnFrom()),tr("Ctrl+Alt+R"));

    contextMenu.exec(globalPos);
}

void MdiChild::setModified()
{
    refreshList();
    eeFile.setChanged(true);
    documentWasModified();
}

void MdiChild::simulate()
{
    if(currentRow()<1) return;

    simulatorDialog sd;

    EEGeneral gg;
    if(!eeFile.getGeneralSettings(&gg)) return;

    ModelData gm;
    if(!eeFile.getModel(&gm,currentRow()-1)) return;

    sd.loadParams(&gg,&gm);
    sd.exec();
}

void MdiChild::viableModelSelected(int idx)
{
    if(!isVisible())
        emit copyAvailable(false);
    else if(idx<1)
        emit copyAvailable(false);
    else
        emit copyAvailable(eeFile.eeModelExists(currentRow()-1));
}




