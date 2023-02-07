#ifndef TREE_H
#define TREE_H

#include <QVector>
#include <QVariant>

class Tree
{
public:
    Tree(const QVector<QVariant>& data, Tree* parent = nullptr);
    ~Tree();
    Tree* getPrev();
    int getPrevInd();
    void incNext();
    void setNext(Tree *newNext);
    void delData(int begin, int end);
    QVector<Tree*> getNext();
    QVector<QVariant> getData();

private:
    Tree* prev;
    QVector<Tree*> next;
    QVector<QVariant> data;
};

#endif // TREE_H
