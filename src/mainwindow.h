#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    bool unSaved;
    bool question(QString msg);
    void alert(QString msg);
    void setupContextMenu();
    QString currentFileName;


private slots:
    void on_actionSave_As_activated();
    void on_listWidget_doubleClicked(QModelIndex index);
    void on_actionAbout_activated();
    void on_actionSave_activated();
    void on_actionOpen_activated();
    void on_actionQuit_activated();
    void ShowContextMenu(const QPoint& pos);
    void RefreshList();
    void DeleteSelectedModel();
    void DuplicateSelectedModel();
    void CutSelectedModel();
    void PasteSelectedModel();

};

#endif // MAINWINDOW_H
