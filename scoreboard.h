#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QDialog>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QMessageBox>

namespace Ui {
class Scoreboard;
}

class Scoreboard : public QDialog
{
    Q_OBJECT

public:
    explicit Scoreboard(QWidget *parent = nullptr);
    ~Scoreboard();

private slots:
    void on_backToMenu_clicked();

    void on_pushButton_clicked();

private:
    Ui::Scoreboard *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
};

#endif // SCOREBOARD_H
