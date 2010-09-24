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
    int askSave();


private slots:
    void on_listWidget_doubleClicked(QModelIndex index);
    void on_actionAbout_activated();
    void on_actionSave_activated();
    void on_actionOpen_activated();
    void on_actionQuit_activated();
    void ShowContextMenu(const QPoint& pos);
    void RefreshList();
    void DeleteModel(uint8_t id);

};

#endif // MAINWINDOW_H
