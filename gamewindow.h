#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QTimer>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    //explicit GameWindow(int timer, bool hintsEnabled, int pack_index, QWidget *parent = nullptr);
    ~GameWindow();
    void GetSettings(int timer, bool hintsEnabled, int pack_index);

private:
    void ShufflePack(int m[], int size);
    void InsertAnswers();
    void SetupNextPicture();
    void LockAllButtons();
    void RestoreButtons();

    Ui::GameWindow *ui;
    QTimer *timer;
    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    int session_time; //Длительность игры
    int timer_time; //Длительность таймера, на 4 сек больше чем длительность игры
    int score;
    bool hintsEnabled;
    QString pack_prefix; //Хранение префикса в зависимости от выбранного пака

    int pack_order[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int pack_idx = 0;

    QString source_path_template;

    QString correct_answer;
    int correct_answer_idx;
    QString incorrect_answers_raw;
    QStringList incorrect_answers;

private slots:
    void TimerSlot();
    void on_PauseButton_clicked();
    void on_Continue_clicked();
    void on_backToMenu_clicked();
    void on_Exit_clicked();
    void on_option_1_clicked();
    void on_option_2_clicked();
    void on_option_3_clicked();
    void on_option_4_clicked();
    void on_hint_5050_clicked();
    void on_hint_friend_clicked();
    void on_hint_gym_clicked();
};

#endif // GAMEWINDOW_H
