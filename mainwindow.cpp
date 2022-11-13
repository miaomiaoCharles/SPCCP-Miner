#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include "widget.h"
using namespace std;

vector< set <string> > algorithm(int timeSpan = 2, double t_threshold = 2, double pi_threshold = 0.5);
vector< vector<string>> inputData;
map<string, int> featureNum;  //存储实例个数
vector<set <string> > ans;
vector<SpatioNode> allInstance; //时空模式专用，保存所有拥堵实例
vector<Road> allRoad; //时空模式专用，保存所有道路。
map<string, vector<string>> roadNeighbor ;//存储道路临近关系
map<SpatioNode*, SpatioNode*> insNeighborMap;
bool checkNeighbor(SpatioNode node1, SpatioNode node2, int timeSpan, int t_threshold){
      for(int i = 0; i < roadNeighbor[node1.roadName()].size(); i++){
          if(roadNeighbor[node1.roadName()][i] == node2.roadName() ){
              if(abs(node1.getTimeSpan()-node2.getTimeSpan()) * timeSpan <= t_threshold){
                   return true;
               }
          }
      }
    return false;
}
struct Table
{
    int _size;
    set<string> _str;
    vector<set<SpatioNode > > _table;
};

bool checkPrivalent(Table t, double piPre){
    // map<string, int> counter;
    map<string, set<string> > counter;
    for(auto it = t._table.begin(); it != t._table.end(); it++){
        set<SpatioNode>& s = *it;
        for(auto sit = s.begin(); sit != s.end(); sit++){
            counter[sit->roadName()].insert(sit->getInsName());
        }
    }
    double pi = 1.0;
    for(auto it = counter.begin(); it != counter.end(); it++){
        double pr = it->second.size()/featureNum[it->first];
        pi = min(pi, pr);
    }
    if(pi >= piPre){
        return true;
    }else{
        return false;
    }
}
Table mergeTable(Table& table1, Table& table2){
    Table newTable;
    set<string> newSet(table1._str);
    for(auto it = table2._str.begin(); it != table2._str.end(); it++){
        if(it == table2._str.begin()) continue;
        newSet.insert(*it);
    }
    newTable._str = newSet;
    newTable._size = newSet.size();
    vector<set<SpatioNode > >& _table1 = table1._table;
    vector<set<SpatioNode > >& _table2 = table2._table;
    for(int i = 0; i < _table1.size(); i++){
        for(int j = 0; j < _table2.size(); j++){
            const SpatioNode& node1 = *(_table1[i].begin());
            const SpatioNode& node2 = *(_table2[j].begin());
            if(node1 == node2){
                set<SpatioNode> temp = _table1[i];
                for(auto it = _table2[j].begin(); it != _table2[j].end(); it++){
                    if(it == _table2[j].begin()) continue;
                    temp.insert(*it);
                }
                newTable._table.push_back(temp);
            }
        }
    }
    return newTable;
}
bool inputCheck(int i){
    int size = inputData[i][0].size();
    if(isdigit(inputData[i][0][size - 1])){
        return true;
    }
    return false;
}





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/../../../Downloads/小车.png"));
    connect(ui->actionopen_2, &QAction::triggered , [=]()->void{
        QString path = QFileDialog::getOpenFileName(this,"open file", "../demo","*.txt");
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QByteArray arrary;
        while(!file.atEnd()){
            QByteArray tempArry = file.readLine();
            arrary += tempArry;
            string str(tempArry);
            char* s = new char[str.size() + 1];
            strcpy(s, str.c_str());
            char* p = strtok(s, " ");
            vector<string> words;
            while(p) {
                words.push_back(p);
                p = strtok(NULL, " ");
            }
            inputData.push_back(words);
            ui->textBrowser_2->setText(arrary);
        }
        file.close();
    });

    write* w = new write(this);
    connect(ui->actionopen_3, &QAction::triggered, this, [=](){
        w->show();
    });

    connect(ui->reset_pushButton, &QPushButton::clicked, [&](){
        ui->textBrowser->clear();
    } );

    connect(ui->start_pushButton, &QPushButton::clicked, [&](){
        int timeSpan = ui->timeSpanlineEdit->text().toInt();
        double f_threshold = ui->f_thresholdlineEdit->text().toDouble();
        int t_threshold = ui->t_thresholdlineEdit->text().toInt();
        double pi_threshold = ui->pi_thresholdlineEdit->text().toDouble();
        vector< set<string> > ans = algorithm(timeSpan, t_threshold, pi_threshold);
        QFont fontAns = QFont("Microsoft YaHei",20,2);
        fontAns.setFamily("微软雅黑");//字体
        fontAns.setPixelSize(30);
        fontAns.setPointSize(20);
        ui->textBrowser->setFont(fontAns);
        for(int i = 0; i < ans.size(); i++){
            for(auto it = ans[i].begin(); it != ans[i].end(); it++){
                string str = *it;
                ui->textBrowser->insertPlainText(QString::fromStdString(str));
            }
            ui->textBrowser->append("");
        }
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}




/*============================== algorithm of SPCCP========================================================================*/
vector< set <string> > algorithm(int timeSpan, double t_threshold, double pi_threshold){
    int i = 0;
    for(i = 0; i < inputData.size(); i++){
        if(inputCheck(i)) break;
        Road r(inputData[i][0]);
        for(int j = 1; j < inputData[i].size(); j++){
            r._neighbor.push_back(inputData[i][j]);
            roadNeighbor[inputData[i][0]].push_back(inputData[i][j]);
        }
        allRoad.push_back(r);
    }
    //开始接收拥塞实例
    for(; i < inputData.size(); i++){
        for(int j = 0; j < inputData[i].size(); j++){
            allInstance.push_back(SpatioNode(inputData[i][j]));
        }
    }

    //将实例放入对应的Road里去
    for(int i = 0; i < allRoad.size(); i++){
        Road& r = allRoad[i];
        for(SpatioNode& ins: allInstance){
            if(ins.roadName() == r.name()){
                r.congestionTimes.push_back(&ins);
                ins.feature = &(allRoad[i]);
            }
        }
    }

    for(SpatioNode& insNode1: allInstance){
        for(SpatioNode& insNode2: allInstance){
            if(insNode1.roadName() != insNode2.roadName() && checkNeighbor(insNode1, insNode2, timeSpan, t_threshold)){
                insNeighborMap.insert({&insNode1, &insNode2});
            }
        }
    }
    for(auto ins: allInstance){
        if(featureNum.find(ins.roadName()) == featureNum.end()){
            featureNum.insert({ins.roadName(), 1});
        }else{
            featureNum[ins.roadName()] ++;
        }
    }
    map< set<string>, Table> tableMap; //存储对应的表实例
    //先计算2阶的情况
    for(auto it = insNeighborMap.begin(); it != insNeighborMap.end(); it++){
        set<string> strSet;
        strSet.insert(it->first->roadName());
        strSet.insert(it->second->roadName());
        if(tableMap.find(strSet) == tableMap.end()){
            Table t;
            t._str = strSet;
            t._size = strSet.size();
        }
        set<SpatioNode> tempSet;
        tempSet.insert(*(it->first));
        tempSet.insert(*(it->second));
        tableMap[strSet]._table.push_back(tempSet);
    }
    vector<Table> prevalentTable;
    for(auto it = tableMap.begin(); it != tableMap.end(); it++){
        if(checkPrivalent(it->second, pi_threshold)){
            ans.push_back(it->first);
            prevalentTable.push_back(it->second);
        }
    }//size-2 complete
    bool flag;
//    while(1){
//        flag = false;
//        for(int i = 0; i < prevalentTable.size()-1; i++){
//            Table& table1 = prevalentTable[i];
//            for(int j = i+1; j < prevalentTable.size(); j++){
//                Table& table2 = prevalentTable[j];
//                if(*(table1._str.begin()) == *(table2._str.begin())){
//                    Table newTable = mergeTable(table1, table2);
//                    if(checkPrivalent(newTable, pi_threshold)){
//                        flag = true;
//                        ans.push_back(newTable._str);
//                        prevalentTable.push_back(newTable);
//                    }
//                }
//            }
//        }
//        if(flag == false) break;
//    }
    return ans;
}

/*=====================================================================================================*/
