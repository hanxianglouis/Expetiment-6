#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

using namespace std;

struct patent
{
    int key;
    string patent_id;
    string date;
    int citation;
    string company;
    string applicant_id;

    patent(string patent_id, string date, int citation, string company, string applicant_id) : patent_id(patent_id), date(date), citation(citation), company(company), applicant_id(applicant_id)
    {
        key = stoi(patent_id.substr(6, 4));
    }
};

void readfile(vector<patent *> &patents)
{
    fstream file;
    file.open("sort_data.csv", ios::in);
    string line;
    getline(file, line); // 略过标题
    string patent_id, date, company, applicant_id;
    int citation;
    while (!file.eof() && getline(file, line))
    {
        stringstream ss(line);
        string temp;
        getline(ss, patent_id, ',');
        getline(ss, date, ',');
        getline(ss, temp, ',');
        citation = stoi(temp);
        getline(ss, company, ',');
        getline(ss, applicant_id, ',');
        patent *newpatent = new patent(patent_id, date, citation, company, applicant_id);
        patents.push_back(newpatent);
    }
    // 冒泡排序，按照key来排
    int n = patents.size();
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (patents[j]->key > patents[j + 1]->key)
            {
                patent *temp;
                temp = patents[j];
                patents[j] = patents[j + 1];
                patents[j + 1] = temp;
            }
        }
    }
    return;
}

//第一部分
void half_search(vector<patent *> patents)
{
    int key_in;
    cout << "Please enter the key of the patent (numbers from the 7th to the 10th):" << endl;
    cin >> key_in;
    int low = -1, high = patents.size(), mid, judge = 0;
    while (high > low + 1 && key_in <= 2123 && key_in >= 1012)
    { // 避免数据过小或过大，导致mid超界
        mid = floor((low + high) / 2);
        if (patents[mid]->key == key_in)
        {
            judge = 1;
            cout << "The patent is found!" << endl;
            cout << "key:" << patents[mid]->key << "  patent id:" << patents[mid]->patent_id << "  date:" << patents[mid]->date << "  citation:" << patents[mid]->citation << "  company:" << patents[mid]->company << "  applicant id:" << patents[mid]->applicant_id << endl;
            break;
        }
        else if (patents[mid]->key > key_in)
        {
            high = mid;
        }
        else
        {
            low = mid;
        }
    }
    if (judge == 0)
    {
        cout << "The patent doesn't exist!" << endl;
    }
    return;
}

//第二部分
int myhash(int key)
{ // C++自带hash函数，自定义的hash函数不能命名为hash
    return key % 4;//综合对比mod 4最好，避免出现hash table中有的位置没有值
}

struct hash_node
{
    patent *data;
    hash_node *next;

    hash_node(patent *data) : data(data), next(nullptr) {}
};

void hash_search(vector<patent *> patents)
{
    vector<hash_node *> hash_table(4, nullptr);
    for (auto &temp_patent : patents)
    {
        hash_node *newnode=new hash_node(temp_patent);
        int index = myhash(temp_patent->key);
        newnode->next=hash_table[index];
        hash_table[index]=newnode;
    }
    int key_in;
    cout << "Please enter the key of the patent (numbers from the 7th to the 10th):" << endl;
    cin>>key_in;
    int place=myhash(key_in);
    hash_node* nownode=hash_table[place];
    while (nownode!=nullptr){
        if(nownode->data->key==key_in){
            cout << "key:" << nownode->data->key << "  patent id:" << nownode->data->patent_id << "  date:" << nownode->data->date << "  citation:" << nownode->data->citation << "  company:" <<nownode->data->company << "  applicant id:" << nownode->data->applicant_id << endl;
            return;
        }
        nownode=nownode->next;
    }
    cout << "The patent doesn't exist!" << endl;
}

//第三部分
vector<patent*> merge(vector<patent*> list1,vector<patent*> list2){//为了能直接用字符串比较，这里把原来的csv文件中日期进行了更改，日、月数均写为两位
    for(int i=0;i<list2.size();i++){
        list1.push_back(list2[i]);
        int j;
        for(j=list1.size()-2;j>=0;j--){
            if(list1[j]->date > list2[i]->date){
                list1[j+1]=list1[j];
            }
            else{
                list1[j+1]=list2[i];
                break;
            }
        }
        if(j==-1){
            list1[0]=list2[i];
        }
    }
    return list1;
}

vector<patent*> Msort(vector<patent*> list){
    if(list.size()==1){
        return list;
    }
    else{
        int m=floor(list.size()/2);
        vector<patent*> list1,list2;
        for(int i=0;i<m;i++){
            list1.push_back(list[i]);
        }
        for( int i=m;i<list.size();i++){
            list2.push_back(list[i]);
        }
        vector<patent*> sorted1=Msort(list1);
        vector<patent*> sorted2=Msort(list2);
        return merge(sorted1,sorted2);
    }
}

void merge_sort(vector<patent*> patents){
    vector<patent*> sorted_patents=Msort(patents);
    for(auto &temp : sorted_patents){
        cout << "key:" << temp->key << "  patent id:" << temp->patent_id << "  date:" << temp->date << "  citation:" << temp->citation << "  company:" <<temp->company << "  applicant id:" << temp->applicant_id << endl;
    }
    return;
}

//第四部分
vector<patent*> Qsort(vector<patent*> list){
    if(list.size()<=1){
        return list;
    }
    else{
        patent* core=list[0];
        int i=0,j=list.size()-1;
        while(j>i){
            while(j>i && list[j]->citation > core->citation){
                j--;
            }
            list[i]=list[j];//如果i，j相等此步无效，不影响
            while(j>i && list[i]->citation < core->citation){
                i++;
            }
            list[j]=list[i];
        }
        list[i]=core;
        vector<patent*> list1,list2;
        for(int k=0;k<i;k++){
            list1.push_back(list[k]);
        }
        for(int k=i+1;k<list.size();k++){
            list2.push_back(list[k]);
        }
        vector<patent*> sorted1=Qsort(list1),sorted2=Qsort(list2);
        vector<patent*> result;
        for(int k=0;k<sorted1.size();k++){
            result.push_back(sorted1[k]);
        }
        result.push_back(core);
        for(int k=0;k<sorted2.size();k++){
            result.push_back(sorted2[k]);
        }
        return result;
    }
}

void quick_sort(vector<patent*> patents){
    vector<patent*> sorted_patents=Qsort(patents);
    for(auto &temp : sorted_patents){
        cout << "key:" << temp->key << "  patent id:" << temp->patent_id << "  date:" << temp->date << "  citation:" << temp->citation << "  company:" <<temp->company << "  applicant id:" << temp->applicant_id << endl;
    }
}

//第五部分
void swap(vector<patent*> &patents,int i,int max){//max用于表示当前还需调整的列表的最后一个元素的index
    int nownode=i;
    while(2*nownode+1 <= max){
        if((patents[nownode]->citation < patents[2*nownode+1]->citation) || (2*nownode+2<=max && patents[nownode]->citation < patents[2*nownode+2]->citation)){//左孩子比自己大或者右孩子存在并且右孩子比自己大
            if(patents[2*nownode+1]->citation >= patents[2*nownode+2]->citation){//左孩子更大
                patent* temp;
                temp=patents[nownode];
                patents[nownode]=patents[2*nownode+1];
                patents[2*nownode+1]=temp;
                nownode=2*nownode+1;
            }
            else{//右孩子更大
                patent* temp;
                temp=patents[nownode];
                patents[nownode]=patents[2*nownode+2];
                patents[2*nownode+2]=temp;
                nownode=2*nownode+2;
            }
        }
        else{
            break;
        }
    }
}


void heap_sort(vector<patent*> patents) {
    //构建大顶堆
    int begin=floor(patents.size()/2);
    for(int i=begin;i>=0;i--){
        swap(patents,i,patents.size()-1);
    }
    for(int i=patents.size()-1;i>0;i--){//i表示当前循环能确定的元素位置
        patent* temp;
        temp=patents[0];
        patents[0]=patents[i];
        patents[i]=temp;
        swap(patents,0,i-1);//保证剩余堆的性质不变
    }
    for(auto &temp : patents){
        cout << "key:" << temp->key << "  patent id:" << temp->patent_id << "  date:" << temp->date << "  citation:" << temp->citation << "  company:" <<temp->company << "  applicant id:" << temp->applicant_id << endl;
    }
}

int main()
{
    vector<patent *> patents;
    readfile(patents);
    int judge = 0; // 判断是否退出
    while (judge == 0)
    {
        int choice;
        cout<<"1.Half Search."<<endl;
        cout<<"2.Hash table search."<<endl;
        cout<<"3.Merge sort. (Based on Date)"<<endl;
        cout<<"4.Quick sort. (Based on citation)"<<endl;
        cout<<"5.Heap sort. (Based on citation)"<<endl;
        cout<<"0.exit"<<endl;
        cout<<"----------------------"<<endl;
        cout<<"Please choose a function:";
        //cin>>choice;
        choice=5;
        switch (choice)
        {
        case 1:
            half_search(patents);
            getchar();
            getchar();
            system("cls");
            break;
        case 2:
            hash_search(patents);
            getchar();
            getchar();
            system("cls");
            break;
        case 3:
            merge_sort(patents);
            getchar();
            getchar();
            system("cls");
            break;
        case 4:
            quick_sort(patents);
            getchar();
            getchar();
            system("cls");
            break;
        case 5:
            heap_sort(patents);
            getchar();
            getchar();
            system("cls");
            break;
        case 0:
            judge = 1;
            break;
        default:
            cout<<"Invalid input! Please choose again!"<<endl;
        }
    }
    return 0;
}