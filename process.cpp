#ifndef PROCESS_H
#define PROCESS_H

#include<QListWidget>

class Process: QListWidgetItem
{
public:
    QString* texto;

public:
    Process(QString* text):QListWidgetItem(){
        this->setText(text);
    }

    void setText(QString* text){
        this->texto = text;
    }

    QString getText(){
        return *this->texto;
    }
};

#endif // PROCESS_H
