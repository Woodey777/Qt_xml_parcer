#include "tree.h"

Tree::Tree(const QVector<QVariant>& data, Tree* parent):
    prev(parent),
    data(data)
{}

Tree::~Tree(){
    prev = nullptr;
    for(auto ptr : next){
        delete ptr;
    }
}

int Tree::getPrevInd(){
    if (!prev) {
        return 0;
    }
    return prev->next.indexOf(this);
}

void Tree::delData(int begin, int end) {
    for (int i = begin; i < end; ++i) {
        delete next[begin];
        next.remove(begin);
    }
}

QVector<Tree*> Tree::getNext() {
    return next;
}

QVector<QVariant> Tree::getData() {
    return data;
}

Tree* Tree::getPrev() {
    return prev;
}

void Tree::setNext(Tree *newNext) {
    next.append(newNext);
}

void Tree::incNext() {
    next.pop_back();
}
