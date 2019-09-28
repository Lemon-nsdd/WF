#include<stdio.h>
#include<iostream>
#include<vector>
#include<map> 
#include<algorithm>
#include<cstring>
#include<iomanip>  
#include<ctype.h>
#include<fstream>
#include<io.h>
using namespace std;

/*��ĸ����Ƶ������sort( , , cmp)*/
bool cmp(pair<char,float> a, pair<char,float> b) {
	return a.second > b.second;
}
/*���ʳ��ִ�������sort( , ,cmp1)*/ 
bool cmp1(pair<string,int> a, pair<string,int> b) {
	return a.second > b.second;
}
/*����Ŀ¼��ȡ�ļ���*/
void GetFilesName(string path,vector<string>& files,char flag)
{
    intptr_t  hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
    {
    	if(flag == 'd'){
    		do{
    			files.push_back(p.assign(path).append("/").append(fileinfo.name));
			}while (_findnext(hFile, &fileinfo) == 0);
		}
		else{
			do{
	            if ((fileinfo.attrib &  _A_SUBDIR)){
	                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
	                    GetFilesName(p.assign(path).append("/").append(fileinfo.name),files,'s');
	            }
	            else
	            {
	                files.push_back(p.assign(path).append("/").append(fileinfo.name));
	            }
        	} while (_findnext(hFile, &fileinfo) == 0);
		}
        _findclose(hFile);
    }
}

/*���������в�����ȡ�ļ���*/ 
vector<string> findfile(int argc,char *argv[]){
	vector<string> filename;
	string rel;
	if(!strcmp(argv[1],"-c")||!strcmp(argv[1],"-f")){
		rel="data/";
		if(!strcmp(argv[argc-2],"-n")){
			for(int i=2;i<argc-2;i++){
			rel = rel + argv[i] + " ";
		}
		}
		else{
			for(int i=2;i<argc;i++){
			rel = rel + argv[i] + " ";
		}
		}
		
		filename.push_back(rel);
	}
	else{
		char flag;
		if(!strcmp(argv[2],"-s")){
			rel = argv[3];
			flag = 's';
		}
		else{
			rel = argv[2];
		}
		GetFilesName(rel,filename,flag);
	}
	return filename;
}

/*����file�ļ�����ĸ���ֵ�Ƶ��*/
void alpFrequent(string file){
	FILE *fp;
	int results[26];
	vector<pair<char,float> > vec;
	int sum=0;
	float percent;
	try{
			fp = fopen(file.c_str(),"r") ;
			if(fp == NULL)
				cout<<"�ļ�������"<<endl;
			else{	
				char ch;
				for(int i=0;i<26;i++){
					results[i] = 0;
				}
				while(fscanf(fp,"%c",&ch)!=EOF){
					if(ch>='a'&&ch<='z')
					{
						results[ch-'a']++;
						sum++;
					}
					else if(ch>='A'&&ch<='Z')
					{
						results[ch-'A']++;
						sum++;
					}
					else
						continue;
				}
				for(int i=0;i<26;i++){
					percent = (float)results[i]*100/sum;
					vec.push_back(pair<char,float>(i+97,percent));
				}
				sort(vec.begin(),vec.end(),cmp);
				cout<<"��ĸʹ��Ƶ������Ϊ��"<<endl; 
				for(int i=0;i<26;i++){
					cout<<vec[i].first<<":"<<setiosflags(ios::fixed) << setprecision(2) <<vec[i].second<<"%"<<endl;
				}
			}
	}
	catch(char *e){
		cout<<e<<endl;
	}
}

/*���ַ�����д��ĸ��ΪСд��ĸ*/ 
string toLower(string word){
	int i=0;
	while(word[i]){
		if(word[i]>='A'&&word[i]<='Z')
			word[i] = tolower(word[i]);
		i++;
	}
	return word;
}

/*�����°��зָ�ɵ���*/
vector<string> split(string line){
	vector<string> words;
	string word;
	int i=0;
	while(i<line.size()){
		while(i<line.size()&&(isspace(line[i])||ispunct(line[i])||isdigit(line[i]))){
			i++;
		}
		int j=i;
		while(j<line.size()&& !isspace(line[j])&& !ispunct(line[j])){
			j++;
		}
		if(i!=j){
			word = line.substr(i, j-i);
			words.push_back(toLower(word));
			i=j;
		}
	}
	return words;
}

/*���㲻ͬ���ʵĳ���Ƶ��*/ 
void wordsFrequent(string file,int n){
	map<string,int> count;
	try{
			ifstream in;
			in.open(file.c_str());
			if(!in.is_open())
				cout<<"�ļ�������"<<endl;
			else{
				string line;
				while(getline(in,line)){
					vector<string> words=split(line);
					for(vector<string>::iterator it=words.begin();it!=words.end();++it ){
						count[*it]++;
					}
				}
				vector<pair<string,int> > results;
				copy(count.begin(), count.end(), back_inserter(results));
				sort(results.begin(),results.end(),cmp1);
				cout<<file<<"�е��ʵ�ʹ�ô�����"<<endl;
				if(!n){
					for(int i=0;i<results.size();i++){
					cout<<results[i].first<<":"<<results[i].second<<"��"<<endl;
				}
				}
				else{
					for(int i=0;i<n;i++){
					cout<<results[i].first<<":"<<results[i].second<<"��"<<endl;
				}
				}
			}
		}
	catch(char *e){
		cout<<e<<endl;
	}
	return;
}

int main(int argc,char *argv[]) {
	vector<string> file;	
	if(argc >= 3){	
		if(!strcmp(argv[1],"-c")){
			file = findfile(argc,argv);
			for(vector<string>::iterator it=file.begin();it!=file.end();it++)
				alpFrequent(*it);	
		}
		else if(!strcmp(argv[1],"-f")){
			file = findfile(argc,argv);
			int n=0;
			if(!strcmp(argv[argc-2],"-n")){
				n = abs(atoi(argv[argc-1]));
			}
			for(vector<string>::iterator it=file.begin();it!=file.end();it++)
				wordsFrequent(*it,n); 
		}
		else if(!strcmp(argv[1],"-d")){
			file = findfile(argc,argv);
			int n=0;
			if(!strcmp(argv[argc-2],"-n")){
				n = abs(atoi(argv[argc-1]));
			}
			for(vector<string>::iterator it=file.begin();it!=file.end();it++)
				wordsFrequent(*it,n);
		} 
		else{
		cout<<"��ѡ�������ʽΪ�� "<<endl; 
		cout<<"Wf.exe -c <file name> "<<endl;
		cout<<"Wf.exe -f <file name> (-n -num)"<<endl;
		cout<<"Wf.exe -d <directory> (-n -num)"<<endl;
		cout<<"Wf.exe -d -s <directory> (-n -num)"<<endl;
	}
	}
	else{
		cout<<"�����ʽΪ��wf.exe -x xxxxxxx "<<endl; 
	}
	return 0;
}
