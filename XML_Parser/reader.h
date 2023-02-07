#ifndef READER_H
#define READER_H

#include "tree.h"
#include <QAbstractItemModel>
#include <QFont>
#include <QStack>
#include <QFile>
#include <QMessageBox>
#include <QXmlStreamReader>

class Reader: public QAbstractItemModel {
    Q_OBJECT

public:
    explicit Reader(QObject *parent = nullptr);
    ~Reader();

    void loadData(const QString& fileName);
    void delModelRows();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int sect, Qt::Orientation p_orient, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& p_idx) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;

private:
    QXmlStreamReader* XMLStrReader;
    QPersistentModelIndex focused;
    QFont focusedFont;
    Tree* root;

};

#endif // READER_H
