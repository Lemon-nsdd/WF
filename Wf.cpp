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

/*字母出现频率排序sort( , , cmp)*/
bool cmp(pair<char,float> a, pair<char,float> b) {
	return a.second > b.second;
}
/*单词出现次数排序sort( , ,cmp1)*/ 
bool cmp1(pair<string,int> a, pair<string,int> b) {
	return a.second > b.second;
}
/*对于目录获取文件名*/
void GetFilesName(string path,vector<string>& files,char flag)
{
    intptr_t  hFile = 0;
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("/*").c_str(), &fileinfo)) != -1)
    {
    	if(flag == 'd'){
    		do{
    			cout<<fileinfo.name<<endl;
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

/*根据命令行参数获取文件名*/ 
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
	else if(!strcmp(argv[1],"-d")){
		char flag;
		if(!strcmp(argv[2],"-s")){
			rel = argv[3];
			flag = 's';
		}
		else{
			rel = argv[2];
			cout<<rel<<endl;
		}
		GetFilesName(rel,filename,flag);
	}
	else if(!strcmp(argv[1],"-x")){
		rel = argv[2];
		filename.push_back(rel);
		rel = "data/" ;
		for(int i=4;i<argc;i++){
			rel = rel + argv[i] + " ";
		}
		filename.push_back(rel);
	}
	else if(!strcmp(argv[1],"-p")){
		rel = "data/" ;
		for(int i=4;i<argc;i++){
			rel = rel + argv[i] + " ";
		}
		filename.push_back(rel);
	}
	else;
	return filename;
}

/*计算file文件中字母出现的频率*/
void alpFrequent(string file){
	FILE *fp;
	int results[26];
	vector<pair<char,float> > vec;
	int sum=0;
	float percent;
	try{
			fp = fopen(file.c_str(),"r") ;
			if(fp == NULL)
				cout<<"文件不存在"<<endl;
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
				cout<<"字母使用频率排序为："<<endl; 
				for(int i=0;i<26;i++){
					cout<<vec[i].first<<":"<<setiosflags(ios::fixed) << setprecision(2) <<vec[i].second<<"%"<<endl;
				}
			}
	}
	catch(char *e){
		cout<<e<<endl;
	}
}

/*将字符串大写字母改为小写字母*/ 
string toLower(string word){
	int i=0;
	while(word[i]){
		if(word[i]>='A'&&word[i]<='Z')
			word[i] = tolower(word[i]);
		i++;
	}
	return word;
}

/*将文章按行分割成单词*/
vector<string> split(string line,int number){
	vector<string> words;
	string word;
	int i = 0;
	int space;
	while(i<line.size()){
		space = 0;
		while(i<line.size()&&(!isalpha(line[i]))){
			i++;
		}
		int j=i;
		while(j<line.size()&& (isalpha(line[j])||isdigit(line[j])||isspace(line[j]))&&(space<number)){
			if(isspace(line[j]))
				space++;
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

/*计算不同单词的出现频率*/ 
void wordsFrequent(string file,int n,string stopfile,int number){
	map<string,int> count;
	string line;
	vector<string> stopwords;
	vector<string> words;
	ifstream in;
	ifstream stop;
	try{
			if(!stopfile.empty()){
				stop.open(stopfile.c_str());
				if(!stop.is_open()){
					cout<<"停词文件不存在"<<endl;
					return; 
				}
				while(getline(stop,line)){
					vector<string> stopword = split(line,1);
					stopwords.insert(stopwords.end(),stopword.begin(),stopword.end());
				}
			}
			in.open(file.c_str());
			if(!in.is_open())
				cout<<"文件不存在"<<endl;
			else{
					while(getline(in,line)){
						words=split(line,number);
						for(vector<string>::iterator it=words.begin();it!=words.end();++it ){
							if(!stopfile.empty()){
								for(vector<string>::iterator stopIt=stopwords.begin();stopIt!=stopwords.end();stopIt++){
									if (*it==*stopIt){
										break;
									}
									else{
										if(stopIt==(stopwords.end()-1)){
											count[*it]++;
										}
									}
								}
							}
							else
								count[*it]++;
						}
					}
				}
				vector<pair<string,int> > results;
				copy(count.begin(), count.end(), back_inserter(results));
				sort(results.begin(),results.end(),cmp1);
				cout<<file<<"中单词的使用次数："<<endl;
				if(!n){
					for(int i=0;i<results.size();i++){
					cout<<results[i].first<<":"<<results[i].second<<"次"<<endl;
				}
				}
				else{
					for(int i=0;i<n;i++){
					cout<<results[i].first<<":"<<results[i].second<<"次"<<endl;
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
	string filename;
	string stop;
	int n=0;
	int number=1;	
	if(argc >= 3){
		file = findfile(argc,argv);	
		if(!strcmp(argv[1],"-c")){
			for(vector<string>::iterator it=file.begin();it!=file.end();it++)
				alpFrequent(*it);	
		}
		else if(!strcmp(argv[1],"-f")){
			cout<<"-f"<<endl;
			if(!strcmp(argv[argc-2],"-n")){
				n = abs(atoi(argv[argc-1]));
			}
			filename = file[0];
			cout<<filename<<endl;
			wordsFrequent(filename,n,stop,number);
		}
		else if(!strcmp(argv[1],"-d")){
			if(!strcmp(argv[argc-2],"-n")){
				n = abs(atoi(argv[argc-1]));
			}
			for(vector<string>::iterator it=file.begin();it!=file.end();it++)
				wordsFrequent(*it,n,stop,number);
		}
		else if(!strcmp(argv[1],"-x")){
			filename =file[1];
			stop = file[0],
			wordsFrequent(filename,n,stop,number);
		}
		else if(!strcmp(argv[1],"-p")){
			number = atoi(argv[2]);
			filename = file[0];
			wordsFrequent(filename,n,stop,number);
		}
		else{
		cout<<"可选的命令格式为： "<<endl; 
		cout<<"Wf.exe -c <file name> "<<endl;
		cout<<"Wf.exe -f <file name> (-n -num)"<<endl;
		cout<<"Wf.exe -d <directory> (-n -num)"<<endl;
		cout<<"Wf.exe -d -s <directory> (-n -num)"<<endl;
		cout<<"Wf.exe -x <stopfile> -f <filename>"<<endl;
		cout<<"Wf.exe -p number -f <filename>"<<endl;
	}
	}
	else{
		cout<<"命令格式为：wf.exe -x xxxxxxx "<<endl; 
	}
	return 0;
}
