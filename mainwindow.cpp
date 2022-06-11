#include "mainwindow.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QDirIterator>
#include <QFileDialog>
#include <QGroupBox>
#include <QHeaderView>
#include <QLabel>
#include <QLayout>
#include <QRadioButton>

#include <QPushButton>

#include <iostream>

constexpr int table_cols = 4;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_tableWidget(new QTableWidget(0, table_cols))
    , m_lineEditFilter(new QLineEdit())
    , m_logger(std::make_shared<LoggerCout>("MAINW"))
{
    auto* centralWidget = new QWidget;
    auto* lay = new QVBoxLayout(centralWidget);
    setCentralWidget(centralWidget);

    setMinimumSize(800, 600);

    lay->addWidget(m_tableWidget);
    m_tableWidget->setHorizontalHeaderLabels({ "file path", "file name - now", "file name - after", "file modification time" });
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    auto* checkRecursive = new QCheckBox("read directory recursive");
    lay->addWidget(checkRecursive);

    lay->addWidget(new QLabel("filter (regular expression):"));
    lay->addWidget(m_lineEditFilter);
    connect(m_lineEditFilter, &QLineEdit::textEdited, this, [this]() { onLineEditFilterTextEdited(); });

    auto* butOpenDir = new QPushButton("read directory");
    lay->addWidget(butOpenDir);
    connect(butOpenDir, &QPushButton::clicked, this, [this, checkRecursive]() {
        const auto& dirName = QFileDialog::getExistingDirectory(nullptr, "open directory");

        if (!dirName.isEmpty()) {
            m_logger->logInfo("opened dir: " + dirName.toStdString());

            QDirIterator::IteratorFlag iteratorFlag = QDirIterator::IteratorFlag::NoIteratorFlags;
            if (checkRecursive->isChecked())
                iteratorFlag = QDirIterator::IteratorFlag::Subdirectories;

            QDirIterator it(dirName, QDir::Filter::Files, iteratorFlag);

            m_dataList.clear();

            while (it.hasNext()) {
                it.next();
                const auto& fileInfo = it.fileInfo();
                const auto& filePath = fileInfo.absolutePath();
                const auto& fileName = fileInfo.fileName();
                const auto& fileModificationTime = fileInfo.fileTime(QFileDevice::FileModificationTime).toString("yyyy-MM-dd_hh:mm:ss:zzz");

                m_dataList.push_back({ filePath, fileName, fileModificationTime, "" });
            }

            sortDataItems(2);
            onLineEditFilterTextEdited();
        }
    });

    auto* layH = new QHBoxLayout();
    lay->addLayout(layH);

    auto* groupSort = new QGroupBox("table sort by:");
    layH->addWidget(groupSort);
    auto* layGroupSort = new QVBoxLayout(groupSort);
    auto* radioSortByFileName = new QRadioButton("file name");
    layGroupSort->addWidget(radioSortByFileName);
    auto* radioSortByFileModificationTime = new QRadioButton("file mod. time");
    layGroupSort->addWidget(radioSortByFileModificationTime);

    layH->addStretch();
}

void MainWindow::onLineEditFilterTextEdited()
{
    QString text = m_lineEditFilter->text();

    if (!text.isEmpty()) {
        m_dataFilteredList.clear();

        for (const auto& item : m_dataList) {
            if (item.fileName.contains(QRegularExpression(text)))
                m_dataFilteredList.push_back(item);
        }
    } else {
        m_dataFilteredList = m_dataList;
    }

    updateTableWidget();
}

void MainWindow::sortDataItems(const uint64_t numberOfSortCols)
{
    auto funcCompQStr = [](const QString& a, const QString& b) {
        if (a.compare(b) < 0)
            return true;
        return false;
    };

    if (numberOfSortCols >= 2)
        std::stable_sort(m_dataList.begin(), m_dataList.end(), [funcCompQStr](const Data& a, const Data& b) {
            return funcCompQStr(a.fileName, b.fileName);
        });

    if (numberOfSortCols >= 1)
        std::stable_sort(m_dataList.begin(), m_dataList.end(), [funcCompQStr](const Data& a, const Data& b) {
            return funcCompQStr(a.filePath, b.filePath);
        });
}

void MainWindow::updateTableWidget()
{
    m_tableWidget->clearContents();

    m_tableWidget->setRowCount(m_dataFilteredList.size());
    int row = 0;
    for (const auto& item : m_dataFilteredList) {
        int col = 0;
        m_tableWidget->setItem(row, col++, new QTableWidgetItem(item.filePath));
        m_tableWidget->setItem(row, col++, new QTableWidgetItem(item.fileName));
        m_tableWidget->setItem(row, col++, new QTableWidgetItem(""));
        m_tableWidget->setItem(row, col++, new QTableWidgetItem(item.fileModificationTime));
        row++;
    }
}
