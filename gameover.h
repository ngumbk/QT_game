#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDebug>

namespace Ui {
class GameOver;
}

class GameOver : public QDialog
{
    Q_OBJECT

public:
    explicit GameOver(QWidget *parent = nullptr);
    ~GameOver();
    void get_score(int score);

private slots:
    void on_OkButton_clicked();

private:
    Ui::GameOver *ui;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    int score;
    int new_place;
    QString name; //нужно ли???
};

#endif // GAMEOVER_H
