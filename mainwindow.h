#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logger.h"

#include <QLineEdit>
#include <QMainWindow>
#include <QTableWidget>

#include <vector>

struct Data {
    QString filePath;
    QString fileName;
    QString fileModificationTime;
    QString fileNameNew;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onLineEditFilterTextEdited();

private:
    void sortDataItems(const uint64_t numberOfSortCols);
    void updateTableWidget();

    QTableWidget* m_tableWidget {};
    QLineEdit* m_lineEditFilter {};

    std::vector<Data> m_dataList;
    std::vector<Data> m_dataFilteredList;

    LoggerPtr m_logger {};
};
#endif // MAINWINDOW_H
