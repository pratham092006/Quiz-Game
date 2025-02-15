#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPair>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

// Structure representing a quiz question
struct Question {
    QString text;
    QString optionA;
    QString optionB;
    QString optionC;
    QString optionD;
    QChar correctOption; // 'A', 'B', 'C', or 'D'
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleAnswer();
    void nextQuestion();
    void showLeaderboard();
    void resetLeaderboard();  // Clears the leaderboard

private:
    void loadQuestions();
    void loadLeaderboard();
    void saveLeaderboard();
    void updateQuestionDisplay();
    void resetQuiz();

    // UI Components created in code
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLabel *questionLabel;
    QHBoxLayout *buttonLayout;
    QPushButton *buttonA;
    QPushButton *buttonB;
    QPushButton *buttonC;
    QPushButton *buttonD;
    QLabel *scoreLabel;

    // Data
    QVector<Question> questions;              // List of quiz questions
    int currentQuestionIndex;                   // Index of the current question
    int score;                                  // User's current score
    QVector<QPair<QString, int>> leaderboard;   // Leaderboard data (name, score)
};

#endif // MAINWINDOW_H
