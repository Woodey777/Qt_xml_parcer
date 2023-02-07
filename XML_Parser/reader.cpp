#include "reader.h"

Reader::Reader(QObject *parent):
    QAbstractItemModel(parent),
    XMLStrReader(new QXmlStreamReader()),
    focused(),
    focusedFont(),
    root(new Tree({tr("")}))
{}

Reader::~Reader(){
    delete root;
    delete XMLStrReader;
}

void Reader::loadData(const QString &filePath){
    QFile file(filePath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QStack<int> count;
    QStack<int> lay;

    XMLStrReader->setDevice(&file);
    QString fileName = filePath.section("/", -1, -1);
    auto fileNode = new Tree{{fileName}, root};
    root->setNext(fileNode);
    Tree* currentNode = fileNode;

    int currentLayer = 0;
    while (!XMLStrReader->atEnd() && !XMLStrReader->hasError()) {
        QXmlStreamReader::TokenType token = XMLStrReader->readNext();
        if (token == QXmlStreamReader::StartElement) {
            ++currentLayer;
            auto text = XMLStrReader->name().toString();
                if (lay.isEmpty() || currentLayer != lay.top()) {
                    count.append(0);
                    lay.append(currentLayer);
                }
            auto node = new Tree({text}, currentNode);
            currentNode->setNext(node);
            currentNode = node;
        }
        if (token == QXmlStreamReader::Characters) {
            auto node = new Tree({XMLStrReader->text().toString()}, currentNode);
            currentNode->setNext(node);
        }
        if (token == QXmlStreamReader::EndElement) {
            --currentLayer;
            currentNode = currentNode->getPrev();
            auto text = XMLStrReader->name().toString();
            if (text == "array" && currentLayer + 1 != lay.top()) {
                count.pop();
                lay.pop();
            }
        }
    }

    if (XMLStrReader->hasError()) {
        QMessageBox message(QMessageBox::Critical,
                            tr("Parsing error"),
                            tr("Parsing error"),
                            QMessageBox::Ok);
        message.exec();
        root->incNext();
     }
}

void Reader::delModelRows(){
    removeRows(0, root->getNext().size(), createIndex(-1, -1, nullptr));
}

int Reader::rowCount(const QModelIndex &parent) const{
    Tree *parentItem;
    if (parent.column() > 0) {
        return 0;
    }
    if (!parent.isValid()) {
        parentItem = root;
    } else {
        Tree* item = static_cast<Tree*>(parent.internalPointer());
        if(item){
            parentItem = item;
        }
    }
    return parentItem->getNext().size();
}

int Reader::columnCount(const QModelIndex &parent) const{
    return root->getData().size();
}

bool Reader::removeRows(int row, int count, const QModelIndex &parent){
    Tree* node;
    QModelIndex correctParent;
    if (!parent.isValid()) {
        node = root;
        correctParent = QModelIndex();
    } else {
        Tree* tree = static_cast<Tree*>(parent.internalPointer());
        if(tree){
            node = tree;
        }
        correctParent = parent;
    }
    beginRemoveRows(correctParent, row, row + count - 1);
    node->delData(row, row + count);
    endRemoveRows();
    return true;
}

bool Reader::setData(const QModelIndex &index, const QVariant &value, int role){
    Tree *tree = static_cast<Tree*>(index.internalPointer());
    if(tree){
        if (role == Qt::EditRole) {
            tree->getData()[index.column()] = value;
            return true;
        }
    }
    if (role == Qt::FontRole) {
        focused = index;
        focusedFont = value.value<QFont>();
        return true;
    }
    return false;
}

QVariant Reader::data(const QModelIndex &index, int role) const{
    if (role == Qt::FontRole && index == focused) {
        return focusedFont;
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    Tree *tree = static_cast<Tree*>(index.internalPointer());
    if(tree) return tree->getData().at(index.column());
}

QVariant Reader::headerData(int sect, Qt::Orientation orient, int role) const{
    if (orient == Qt::Horizontal && role == Qt::DisplayRole) {
        return root->getData().at(sect);
    }
    return QVariant();
}

Qt::ItemFlags Reader::flags(const QModelIndex &index) const{
    if (!index.isValid()) return Qt::NoItemFlags;
    return QAbstractItemModel::flags(index);
}

QModelIndex Reader::index(int row, int column, const QModelIndex &parent) const{
    Tree* prnt;
    if (!parent.isValid()) {
        prnt = root;
    } else {
        Tree* tree = static_cast<Tree*>(parent.internalPointer());
        prnt = tree;
    }
    Tree *chld;

    if (row < 0 || row >= prnt->getNext().size()) {
        chld = nullptr;
    } else {
        chld = prnt->getNext().at(row);
    }

    if (chld) {
        return createIndex(row, column, chld);
    }
    return QModelIndex();
}

QModelIndex Reader::parent(const QModelIndex &child) const{
    Tree* childItem = static_cast<Tree*>(child.internalPointer());
    Tree* parent = childItem->getPrev();

    if (parent == root || childItem == root) return QModelIndex();

    return createIndex(parent->getPrevInd(), 0, parent);
}
