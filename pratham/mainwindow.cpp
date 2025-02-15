#include "mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QFile>
#include <QTextStream>
#include <algorithm>`
#include <QTableWidget>
#include <QHeaderView>
#include <QColor>
#include <QBrush>

// Constructor: set up the UI and load data
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    currentQuestionIndex(0),
    score(0)
{
    // Create central widget and main layout
    centralWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // Create the question label
    questionLabel = new QLabel("Question will appear here", this);
    questionLabel->setAlignment(Qt::AlignCenter);
    questionLabel->setMinimumHeight(80);
    mainLayout->addWidget(questionLabel);

    // Create answer buttons and add them to a horizontal layout
    buttonLayout = new QHBoxLayout();
    buttonA = new QPushButton("A", this);
    buttonB = new QPushButton("B", this);
    buttonC = new QPushButton("C", this);
    buttonD = new QPushButton("D", this);
    buttonLayout->addWidget(buttonA);
    buttonLayout->addWidget(buttonB);
    buttonLayout->addWidget(buttonC);
    buttonLayout->addWidget(buttonD);
    mainLayout->addLayout(buttonLayout);

    // Create the score label
    scoreLabel = new QLabel("Score: 0", this);
    scoreLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(scoreLabel);

    // Apply a new style sheet with a different color scheme
    QString style = R"(
        QWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #e0c3fc, stop:1 #8ec5fc);
            font-family: 'Helvetica', Arial, sans-serif;
        }
        QLabel {
            color: #222;
            font-size: 22px;
            font-weight: bold;
        }
        QPushButton {
            background-color: #6a11cb; /* purple */
            color: white;
            border: 2px solid #2575fc; /* blue border */
            border-radius: 10px;
            padding: 12px 24px;
            font-size: 16px;
        }
        QPushButton:hover {
            background-color: #2575fc; /* blue on hover */
        }
    )";
    centralWidget->setStyleSheet(style);

    setCentralWidget(centralWidget);

    // Connect answer buttons to the answer handler slot
    connect(buttonA, &QPushButton::clicked, this, &MainWindow::handleAnswer);
    connect(buttonB, &QPushButton::clicked, this, &MainWindow::handleAnswer);
    connect(buttonC, &QPushButton::clicked, this, &MainWindow::handleAnswer);
    connect(buttonD, &QPushButton::clicked, this, &MainWindow::handleAnswer);

    // Load quiz questions and leaderboard, then display the first question
    loadQuestions();
    loadLeaderboard();
    updateQuestionDisplay();
}

// Destructor
MainWindow::~MainWindow() { }

// Load a new set of 10 general knowledge questions
void MainWindow::loadQuestions()
{
    questions.clear();
    questions.append({"What is the tallest mountain in the world?", "K2", "Mount Everest", "Kangchenjunga", "Lhotse", 'B'});
    questions.append({"Which country hosted the 2016 Summer Olympics?", "China", "Brazil", "United Kingdom", "Russia", 'B'});
    questions.append({"What is the smallest country in the world?", "Vatican City", "Monaco", "Nauru", "San Marino", 'A'});
    questions.append({"Who invented the telephone?", "Thomas Edison", "Nikola Tesla", "Alexander Graham Bell", "Guglielmo Marconi", 'C'});
    questions.append({"What is the largest desert in the world?", "Sahara", "Gobi", "Antarctic Desert", "Arabian Desert", 'C'});
    questions.append({"Which planet is known as the Morning Star?", "Venus", "Mars", "Jupiter", "Mercury", 'A'});
    questions.append({"Who painted the ceiling of the Sistine Chapel?", "Leonardo da Vinci", "Raphael", "Michelangelo", "Donatello", 'C'});
    questions.append({"What is the currency of Japan?", "Yuan", "Yen", "Won", "Dollar", 'B'});
    questions.append({"In which continent is the Amazon Rainforest primarily located?", "Africa", "Asia", "South America", "Australia", 'C'});
    questions.append({"Who wrote the novel '1984'?", "George Orwell", "Aldous Huxley", "Ray Bradbury", "J.K. Rowling", 'A'});
}

// Update the question label and answer buttons with the current question
void MainWindow::updateQuestionDisplay()
{
    if (currentQuestionIndex < questions.size()) {
        const Question &q = questions[currentQuestionIndex];
        questionLabel->setText(q.text);
        buttonA->setText("A: " + q.optionA);
        buttonB->setText("B: " + q.optionB);
        buttonC->setText("C: " + q.optionC);
        buttonD->setText("D: " + q.optionD);
    }
}

// Slot to handle an answer click
void MainWindow::handleAnswer()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton)
        return;

    QString buttonText = clickedButton->text();
    QChar chosenOption = buttonText.at(0);
    const Question &q = questions[currentQuestionIndex];

    if (chosenOption.toUpper() == q.correctOption.toUpper())
        score += 10;  // Award 10 points for a correct answer

    scoreLabel->setText("Score: " + QString::number(score));
    nextQuestion();
}

// Proceed to the next question or finish the quiz
void MainWindow::nextQuestion()
{
    currentQuestionIndex++;
    if (currentQuestionIndex >= questions.size()) {
        // Quiz is finished: prompt for user's name and update leaderboard
        QString name = QInputDialog::getText(this, "Quiz Finished", "Enter your name for the leaderboard:");
        if (!name.isEmpty()) {
            leaderboard.append(qMakePair(name, score));
            std::sort(leaderboard.begin(), leaderboard.end(), [](const QPair<QString, int>& a, const QPair<QString, int>& b) {
                return a.second > b.second;
            });
            saveLeaderboard();
        }
        showLeaderboard();
        resetQuiz();
    } else {
        updateQuestionDisplay();
    }
}

// Reset quiz data for a new round
void MainWindow::resetQuiz()
{
    currentQuestionIndex = 0;
    score = 0;
    scoreLabel->setText("Score: 0");
    updateQuestionDisplay();
}

// Load leaderboard data from "leaderboard.txt"
void MainWindow::loadLeaderboard()
{
    QFile file("leaderboard.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    leaderboard.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");
        if (parts.size() == 2)
            leaderboard.append(qMakePair(parts[0], parts[1].toInt()));
    }
    file.close();
}

// Save the leaderboard data to "leaderboard.txt"
void MainWindow::saveLeaderboard()
{
    QFile file("leaderboard.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const auto &entry : leaderboard)
        out << entry.first << "," << entry.second << "\n";
    file.close();
}

// Show the leaderboard in a new window with a blue-to-orange gradient
void MainWindow::showLeaderboard()
{
    QWidget *leaderboardWidget = new QWidget;
    leaderboardWidget->setWindowTitle("Leaderboard");
    QVBoxLayout *layout = new QVBoxLayout(leaderboardWidget);

    QTableWidget *table = new QTableWidget(leaderboard.size(), 2, leaderboardWidget);
    table->setHorizontalHeaderLabels(QStringList() << "Name" << "Score");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    for (int i = 0; i < leaderboard.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(leaderboard[i].first));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(leaderboard[i].second)));
    }

    // Apply a color gradient: top row (blue) to bottom row (orange)
    // Blue: (0, 122, 255); Orange: (255, 149, 0)
    int rowCount = table->rowCount();
    for (int i = 0; i < rowCount; i++) {
        double factor = (rowCount == 1) ? 1.0 : static_cast<double>(rowCount - 1 - i) / (rowCount - 1);
        // Linear interpolation between orange and blue:
        int r = static_cast<int>(255 * (1 - factor) + 0 * factor);
        int g = static_cast<int>(149 * (1 - factor) + 122 * factor);
        int b = static_cast<int>(0 * (1 - factor) + 255 * factor);
        QColor bgColor(r, g, b);
        for (int j = 0; j < table->columnCount(); j++) {
            table->item(i, j)->setBackground(QBrush(bgColor));
        }
    }

    layout->addWidget(table);

    // Add a "Reset Leaderboard" button
    QPushButton *resetBtn = new QPushButton("Reset Leaderboard", leaderboardWidget);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::resetLeaderboard);
    layout->addWidget(resetBtn);

    leaderboardWidget->setLayout(layout);
    leaderboardWidget->resize(400, 350);
    leaderboardWidget->show();
}

// Slot to reset (clear) the leaderboard after confirmation
void MainWindow::resetLeaderboard()
{
    int ret = QMessageBox::warning(this, "Reset Leaderboard",
                                   "Are you sure you want to reset the leaderboard?",
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        leaderboard.clear();
        QFile file("leaderboard.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            file.close();
    }
}
